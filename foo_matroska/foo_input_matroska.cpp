/*
 *  Part of the foobar2000 Matroska plugin
 *
 *  Copyright (C) Jory Stone (jcsston at toughguy net) - 2003
 *
 *	Based on mp4_parser.cpp from foo_std_input
 *	Copyright (c) 2001-2003, Peter Pawlowski
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Trolltech AS of Norway and appearing in the file
 *  copying.txt included in the packaging of this file.
 *
 *  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 *  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*!
    \file foo_input_matroska.cpp
		\version $Id$
    \brief A foobar2000 plugin for Matroska
		\author Jory Stone     <jcsston @ toughguy.net>
*/

//Memory Leak Debuging define
#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>

// Debug Memory Alloc hook
int YourAllocHook(int allocType, void *userData, size_t size, int blockType, long requestNumber, const unsigned char *filename, int lineNumber)
{
	/*static int64 totalSize;
	if (allocType == 1)
		totalSize += size;
	else if (allocType == 3)
		totalSize -= _msize(userData);
	else if (allocType == 500)
		totalSize = 0;
	else if (allocType == 1000)
		*(int64 *)userData = totalSize;*/
	//I can use this to find exactly where a leak started
	if (size == 136)
	{
		//NOTE(_T("Memory Alloc Hook tripped"));
		return 1;
	}
	return 1;
};
#endif

#include "../SDK/foobar2000.h"
//#include "../../Foobar2000_SDK/foobar2000/SDK/foobar2000.h"

#include <commctrl.h>
#include <Shlwapi.h>
#include <time.h>
#include "matroska_parser.h"
#include "DbgOut.h"
#include "resource.h"

//static cfg_string cfg_temp_dir("abx_temp_dir","c:\\");
//static cfg_int cfg_reuse_parser("reuse_parser",1);
//static cfg_int cfg_dsp("abx_dsp",0);
//static cfg_int cfg_keep_position("keep_position",1);

class input_matroska : public input
{
public:
	input_matroska()
	{
		NOTE("input_matroska::input_matroska()");
#ifdef _DEBUG_MEM
	// Setup the debug options
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF 
								| _CRTDBG_LEAK_CHECK_DF //Check for memory leaks on app exit
								| _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetAllocHook(YourAllocHook);	
#endif
		m_Parser = NULL;
		p_decoder = NULL;
		m_TrackNo = 0;
		m_skip_samples = 0;
		m_skip_frames = 0;	
		m_timescale = TIMECODE_SCALE;
	
		m_vbr_last_duration = 0;
		m_vbr_update_frames = 0;
		m_vbr_update_bytes = 0;
		m_vbr_update_time = 0;
		m_vbr_update_enabled = true;
		m_vbr_update_interval = 32;
		m_position = 0;
		m_length = 0;

	}

	~input_matroska()
	{
		NOTE("input_matroska::~input_matroska()");
		cleanup();
	}

	virtual bool test_filename(const char * fn,const char * ext)
	{
		NOTE2("input_matroska::test_filename(fn = %s, ext = %s)", fn, ext);

		return (!stricmp(ext,"MKA") || !stricmp(ext,"MKV"));
	}

	virtual bool open(reader *r, file_info *info, unsigned flags)
	{
		NOTE4("input_matroska::open(r = 0x%p, info = 0x%p, flags = %i) - subsong = %i", r, info, flags, info->get_subsong_index());

		/*if (!r->can_seek())
		{
			console::error("Matroska: unseekable streams not supported.");
			return false;
		}*/

		cleanup();

		m_Parser = new MatroskaAudioParser(r);

		if (flags == OPEN_FLAG_GET_INFO) {		
			if (m_Parser->Parse(true)) {
				console::error("Matroska: Invalid Matroska file.");
				return false;			
			}
			
			m_Parser->SetSubSong(info->get_subsong_index());

			m_TrackNo = m_Parser->GetFirstAudioTrack();
			if (m_TrackNo == -1) {
				console::error("Matroska: no decodable streams found.");
				return false;
			}

			string8 codecname = m_Parser->GetFoobar2000Format(m_TrackNo);
		
			// Set the current track
			m_Parser->SetCurrentTrack(m_TrackNo);

			MatroskaTrackInfo &currentTrack = m_Parser->GetTrack(m_TrackNo);

			m_expected_channels = currentTrack.channels;
			m_expected_sample_rate = currentTrack.samplesOutputPerSec == 0 ? currentTrack.samplesPerSec : currentTrack.samplesOutputPerSec;

			info->info_set("codec", codecname);
			info->info_set_int("channels", m_expected_channels);
			info->info_set_int("samplerate", m_expected_sample_rate);
			m_Parser->SetFB2KInfo(info);
			// The timecode scale in Matroska is in milliseconds, but foobar deals in seconds
			m_timescale = m_Parser->GetTimecodeScale() * 1000;

			m_length = duration_to_samples(m_Parser->GetDuration());
			m_position = 0;

			 
			info->set_length((double)m_Parser->GetDuration() / 1000000000.0);

			info->info_set_bitrate(m_Parser->GetAvgBitrate());

		} else {
			if (m_Parser->Parse()) {
				console::error("Matroska: Invalid Matroska file.");
				return false;			
			}
			
			m_Parser->SetSubSong(info->get_subsong_index());

			m_TrackNo = m_Parser->GetFirstAudioTrack();
			if (m_TrackNo == -1) {
				console::error("Matroska: no decodable streams found.");
				return false;
			}

			string8 codecname = m_Parser->GetFoobar2000Format(m_TrackNo);
		
			p_decoder = packet_decoder::create(codecname);
			if (p_decoder == NULL)
			{
				cleanup();
				console::error(uStringPrintf("Matroska: unable to find a '%s' packet decoder object.", codecname.get_ptr()));
				return false;
			}

			// Set the current track
			m_Parser->SetCurrentTrack(m_TrackNo);

			MatroskaTrackInfo &currentTrack = m_Parser->GetTrack(m_TrackNo);

			unsigned char *buffer = &currentTrack.codecPrivate[0];
			unsigned int buffer_size = currentTrack.codecPrivate.size();

			if (!p_decoder->init(buffer,buffer_size,info))
			{
				console::error(uStringPrintf("Matroska: error initializing %s decoder.", p_decoder->get_name()));
				cleanup();
				return false;
			}		

			m_expected_channels = currentTrack.channels;
			m_expected_sample_rate = currentTrack.samplesOutputPerSec == 0 ? currentTrack.samplesPerSec : currentTrack.samplesOutputPerSec;

			info->info_set("codec", codecname);
			info->info_set_int("channels", m_expected_channels);
			info->info_set_int("samplerate", m_expected_sample_rate);
			m_Parser->SetFB2KInfo(info);
			// The timecode scale in Matroska is in milliseconds, but foobar deals in seconds
			m_timescale = m_Parser->GetTimecodeScale() * 1000;		

			m_length = (uint64)((double)m_expected_sample_rate * (double)m_Parser->GetDuration()/(double)m_timescale);
			m_position = 0;

			info->set_length((double)m_Parser->GetDuration()/1000000000.0);

			info->info_set_bitrate(m_Parser->GetAvgBitrate());
			
			// Be sure we are starting at the 'beginning' of the song ;)

			m_skip_samples = 0;
			m_skip_frames = 0;

			if (!seek(0)) return false;
		}

		

#ifdef SPEED_TEST
		MatroskaAudioParser *test = new MatroskaAudioParser(r);
		MatroskaAudioParser *test2 = new MatroskaAudioParser(r);
		clock_t startTime = clock();
		test->Parse();
		clock_t startTime2 = clock();
		test2->Parse(true);
		clock_t endTime = clock();
		NOTE3("Break at clusters = %i, normal = %i. Saved %i", (int)(endTime - startTime2), (int)(startTime2 - startTime), (int)((startTime2 - startTime) - (endTime - startTime2)));
		delete test;
		delete test2;
#endif

		return true;
	}

	virtual int run(audio_chunk * chunk)
	{
		if (p_decoder == NULL)
		{
			console::error("Matroska: attempting to decode without a loaded decoder.");
			return -1;
		}

		if (m_position>=m_length) return 0;

		bool done = false;

		do
		{
			unsigned char *buffer;
			unsigned int buffer_size;

			buffer = NULL;
			buffer_size = 0;

			bool skip_this_frame = false;

			if (m_Parser->ReadSingleFrame(&m_Frame) > 0)
			{
				return 0;
			}


			{
				int64 delta = duration_to_samples(m_Frame.timecode) - m_position;
				console::info(uStringPrintf("drift: %d",(int)delta));
			}
			

			if (m_Frame.dataBuffer.size() == 1) {
				buffer = (unsigned char*)&m_Frame.dataBuffer.at(0)[0];
				buffer_size = m_Frame.dataBuffer.at(0).size();

				m_tempchunk.reset();
				if (!p_decoder->decode(buffer,buffer_size,&m_tempchunk))
				{
					cleanup();
					console::error(uStringPrintf("Matroska: '%s' decode error.", p_decoder->get_name()));
					return -1;
				}
				else if (m_tempchunk.is_valid())
				{
					m_vbr_update_frames++;
					m_vbr_update_bytes += buffer_size;
					m_vbr_update_time += (m_vbr_last_duration = m_tempchunk.get_duration());
				}

				if (m_skip_frames>0)
				{
					skip_this_frame = true;
					m_skip_frames--;
				}

			} else if (m_Frame.dataBuffer.size() > 1) {
				// We need to decode a laced frame
				// I pack together all the frames of a lace together because of the lack of timecodes

				// This vector is for storing all the decoded frames, 
				// when I'm done I'll copy these to the main temp buffer m_tempchunk
				std::vector<audio_chunk_i> tempBuffer;
				size_t l = 0;
				
				unsigned laced_count = m_Frame.dataBuffer.size();

				// Make the vector large enough to hold all the decoded frames of this lace
				tempBuffer.resize(laced_count);

				
				unsigned to_skip = m_skip_frames;
				if (to_skip > laced_count) to_skip = laced_count;
				m_skip_frames -= to_skip;


				for (l = 0; l < m_Frame.dataBuffer.size(); l++) {
					buffer = (unsigned char*)&m_Frame.dataBuffer[l][0];
					buffer_size = m_Frame.dataBuffer[l].size();

					//tempBuffer[l].reset();
					if (!p_decoder->decode(buffer,buffer_size,&tempBuffer[l]))
					{
						cleanup();
						console::error("Matroska: decode error.");
						return -1;
					}
					else if (tempBuffer[l].is_valid() && l>=to_skip)
					{
						m_vbr_update_frames++;
						m_vbr_update_bytes += buffer_size;
						m_vbr_update_time += (m_vbr_last_duration = tempBuffer[l].get_duration());
					}
				}
				

				m_tempchunk.reset();

				if (to_skip < laced_count)
				{
				
					for (l = to_skip; l < laced_count; l++) {
						if (m_tempchunk.copy_from(&tempBuffer[l]))
							break;
					}

					if (m_tempchunk.get_srate() != 0) {
						buffer_size = 0;
						for (l = to_skip; l < laced_count; l++) {
							buffer_size += tempBuffer[l].get_data_size();
						}
						m_tempchunk.set_data_size(buffer_size);
						buffer = (unsigned char *)m_tempchunk.get_data();

						// Copy over the total duration about this chunk
						m_tempchunk.set_sample_count(buffer_size/m_tempchunk.get_channels());

						//buffer_size = 0;
						for (l = to_skip; l < laced_count; l++) {
							memcpy(buffer, tempBuffer[l].get_data(), tempBuffer[l].get_data_size() * sizeof(audio_sample));
							buffer += tempBuffer[l].get_data_size() * sizeof(audio_sample);
						}
					}
				}
				else skip_this_frame = true;
			} else {
				// End of the stream
				return 0;
			}

			if (!skip_this_frame)
			{				
				unsigned offset = (unsigned)duration_to_samples(0);//m_Frame.timecode);
				unsigned duration = (unsigned)duration_to_samples(m_Frame.duration);
				//			console::info(uStringPrintf("duration: %u, offset: %u",duration,offset));

				if (m_tempchunk.is_empty())
				{
					if (duration > 0)
					{
						m_tempchunk.set_srate(m_expected_sample_rate);
						m_tempchunk.set_channels(m_expected_channels);
						m_tempchunk.pad_with_silence(duration);
#ifdef _DEBUG
						console::warning("Matroska: decoder returned empty chunk from a non-empty Matroska frame.");
#endif
					}
				}
				else if (!m_tempchunk.is_valid())
				{
					cleanup();
					console::error("Matroska: decoder produced invalid chunk.");
					return -1;
				}
				unsigned samplerate,channels,decoded_sample_count;

				samplerate = m_tempchunk.get_srate();
				channels = m_tempchunk.get_channels();
				decoded_sample_count = m_tempchunk.get_sample_count();

				// !!!!!!!!!!!!!!! HACK ALERT !!!!!!!!!!!!!!!!!!!!!!!!
				duration = decoded_sample_count;
				// !!!!!!!!!!!!!!! HACK ALERT !!!!!!!!!!!!!!!!!!!!!!!!

				if (decoded_sample_count < duration)
				{
#ifdef _DEBUG
					console::warning("Matroska: decoded frame smaller than expected.");
#endif
					decoded_sample_count = duration;
					m_tempchunk.pad_with_silence(decoded_sample_count);
				}

				if (duration < offset) duration = 0;
				else duration -= offset;

				if (m_skip_samples>0)
				{
					unsigned int delta = (unsigned)m_skip_samples;
					if (delta > duration) delta = duration;
					offset += delta;
					duration -= delta;
					m_skip_samples -= delta;
				}

				{
					uint64 max = m_length - m_position;
					if (duration>max) duration = max;
					m_position += duration;
					if (m_position==m_length && duration==0) return 0;
				}



				if (duration > 0)
				{
					chunk->set_data_64(m_tempchunk.get_data() + offset * channels,duration,channels,samplerate);
					done = true;
				}
			}
			//done = true;
		}
		while(!done);

		return done ? 1 : 0;
	}

	virtual set_info_t set_info(reader *r, const file_info * info)
	{
		NOTE2("input_matroska::set_info(r = 0x%p, info = 0x%p)", r, info);
		unsigned rv = 1;

		// We have to create a new parser
		MatroskaAudioParser writeReader(r);
		writeReader.Parse(true);
					
		int TrackNo = writeReader.GetFirstAudioTrack();
		if (TrackNo != -1) {				
			// Set the current track
			writeReader.SetCurrentTrack(m_TrackNo);
			writeReader.SetSubSong(info->get_subsong_index());

			rv = writeReader.WriteTags(info);			
		}		

		return !rv ? SET_INFO_SUCCESS : SET_INFO_FAILURE;
	}

	virtual bool seek(double seconds)
	{
		NOTE1("input_matroska::seek(seconds = %f)", seconds);

		if (p_decoder == NULL)
		{
			console::error("Matroska: attempting to seek while not open.");
			return false;
		}

		unsigned max_frame_dependency = p_decoder->get_max_frame_dependency();

		unsigned frames_to_skip = 0;

		m_position = (uint64)(m_expected_sample_rate * seconds + 0.5);

		int64 offset = m_Parser->Seek(seconds,max_frame_dependency,frames_to_skip);
		if (offset<0) return false;

		m_skip_frames = frames_to_skip;

		m_skip_samples = duration_to_samples(offset);
		
		//console::info(uStringPrintf("skip samples: %u",m_skip_samples));

		return true;
	}

	virtual bool is_our_content_type(const char *url, const char *type)
	{
		NOTE2("input_matroska::is_our_content_type(url = %s, type = %s)", url, type);

		return !stricmp(type, "audio/x-matroska") 
			|| !stricmp(type, "video/x-matroska")
			// Just to be safe check for ones without x-
			// However, You're supposed to use x-* unless it's offically registered
			|| !stricmp(type, "audio/matroska") 
			|| !stricmp(type, "video/matroska");
	}

	virtual bool get_dynamic_info(file_info * out,double * timestamp_delta,bool * b_track_change)
	{
		bool ret = false;
		if (m_vbr_update_enabled)
		{
			if (m_vbr_update_time > 0 && m_vbr_update_frames >= m_vbr_update_interval)
			{
				int val = (int) ( ((double)m_vbr_update_bytes * 8.0 / m_vbr_update_time + 500.0) / 1000.0 );
				if (val != out->info_get_bitrate_vbr())
				{
					*timestamp_delta = - (m_vbr_update_time - m_vbr_last_duration);	//relative to last frame beginning;
					out->info_set_bitrate_vbr(val);
					ret = true;
				}
				m_vbr_update_frames = 0; m_vbr_update_bytes = 0;
				m_vbr_update_time = 0;
			}
		}		
		return ret;
	}

protected:
	void cleanup()
	{
		if (p_decoder != NULL) 
		{
			p_decoder->service_release();
			p_decoder = NULL;
		}
		if (m_Parser != NULL) {
			delete m_Parser;
			m_Parser = NULL;
		}
	}
	int64 duration_to_samples(int64 val) {
		return (int64) ( (double)val * (double) m_expected_sample_rate / (double)m_timescale + 0.5);
	}

	int64 samples_to_duration(int64 val)
	{
		return (int64) ( (double)val * (double)m_timescale / (double)m_expected_sample_rate + 0.5);
	}

	MatroskaAudioParser *m_Parser;
	reader *m_matroskaFile;
	packet_decoder *p_decoder;

	int m_TrackNo;
	// Compressed data frame
	MatroskaAudioFrame m_Frame;

	unsigned m_skip_samples;
	unsigned m_skip_frames;
	unsigned m_expected_sample_rate;
	unsigned m_expected_channels;
	uint64 m_timescale;
	
	// VBR bitrate display stuff
	double m_vbr_last_duration;
	unsigned m_vbr_update_frames,m_vbr_update_bytes;
	double m_vbr_update_time;
	bool m_vbr_update_enabled;
	unsigned m_vbr_update_interval;
	uint64 m_length,m_position;

	// A temp decoding buffer
	audio_chunk_i m_tempchunk;
};

static service_factory_t<input, input_matroska> foo_matroska;

class track_indexer_matroska : public track_indexer
{
public:
	virtual int get_tracks(const char* filename, callback * out, reader * r)
	{
		NOTE3("track_indexer_matroska::get_tracks(filename = %s, out = 0x%p, r = 0x%p", filename, out, r);
		// Let's check the filename
		if (!stricmp_utf8_partial(filename, "http://", 7)) {
			// HTTP streams and chapters togther are not supported, requires seeking
			return 0;
		} else if (strlen(filename) > 3) {
			const char *ext = filename+strlen(filename)-3;
			if (!!stricmp(ext, "MKA") && !!stricmp(ext, "MKV"))
				return 0;
		} else {
			return 0;
		}
		if (r == NULL)
			r = file::g_open_read(filename);
		if (r == NULL)
			return 0;
		
		MatroskaAudioParser scanner(r);
		scanner.Parse(true);

		std::vector<MatroskaChapterInfo> &chapters = scanner.GetChapters();
		
		if (chapters.size() > 0) {
			for (size_t c = 0; c < chapters.size(); c++) {
				MatroskaChapterInfo &currentChapter = chapters.at(c);
				out->on_entry(make_playable_location(filename, (int)c));
			}
			r->reader_release();
			return 1;
		}

		r->reader_release();
		return 0;
	}
};

static service_factory_single_t<track_indexer,track_indexer_matroska> foo_matroska1;

#ifdef _DEBUG
DECLARE_COMPONENT_VERSION("Matroska Plugin (Debug)" ,"0.4","Copyright (C) 2003 Jory Stone (jcsston@toughguy.net)");
#else
DECLARE_COMPONENT_VERSION("Matroska Plugin" ,"0.4","Copyright (C) 2003 Jory Stone (jcsston@toughguy.net)");
#endif

/*
 *  Part of the foobar2000 Matroska plugin
 *
 *  Copyright (C) Jory Stone (jcsston at toughguy net) - 2003
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
    \file foo_matroska.cpp
		\version $Id$
    \brief A foobar2000 plugin for Matroska
		\author Jory Stone     <jcsston @ toughguy.net>
*/

#include <windows.h>
#include <commctrl.h>
#include <Shlwapi.h>

#include "../SDK/foobar2000.h"
#include "MatroskaMuxer.h"
#include "mpa/mpafile.h"
#include "flac/flac_reader.h"
#include "DbgOut.h"
#include "resource.h"

class contextmenu_mka : public menu_item_context
{
public:
	virtual int get_num_items() { return 1; }

	virtual const char *enum_item(int n)
	{
		NOTE1("contextmenu_mka::enum_item(n = %i)", n);

		if (n == 0) return "MKA utilities/Converter (MP3 <-> MKA)";
		//else if (n == 1) return "MP4 utilities/Converter (AAC,MP3 <-> MP4), different directory";
		return 0;
	}

	virtual bool context_get_display(int idx, const ptr_list_base<metadb_handle> &data, string_base &out, bool is_playlist)
	{
		NOTE4("contextmenu_mka::context_get_display(idx = %i, data.get_count() = %i, out = %s, is_playlist = %i)", idx, data.get_count(), out.get_ptr(), is_playlist);

		if (idx!=0 && idx!=1) return false;

		int count = data.get_count();
		if (count < 1) return false;
		int type = -1;

		int i;

		for (i = 0; i < count; i++)
		{
			metadb_handle *ptr = data.get_item(i);
			if (!ptr) return false;
			const char *p = ptr->handle_get_path();
			if (!p) return false;
			p = strchr(p, '.');
			if (!p) return false;
			if (type == -1) {
				if (!stricmp(p,".mp3") || !stricmp(p,".flac")) 
					type = 0;
				else if (!stricmp(p, ".mka") || !stricmp(p, ".mkv"))
					type = 1;
				else
					return false;
			}	else {
				if (type == 0 && (stricmp(p, ".mp3") && stricmp(p, ".flac"))) return false;
				if (type == 1 && (stricmp(p, ".mka") && stricmp(p, ".mkv"))) return false;
			}
		}

		if (type == 0)
		{
			if (idx == 0)
				out.set_string("MKA utilities/Convert to MKA (same directory)");
		}
		else
		{
			if (idx == 0)
				out.set_string("MKA utilities/Extract MKA track(s) (same directory)");
		}

		return true;
	}

	virtual void context_command(int idx, const ptr_list_base<metadb_handle> &data, bool is_playlist)
	{
		NOTE3("contextmenu_mka::context_command(idx = %i, data.get_count() = %i, is_playlist = %i)", idx, data.get_count(), is_playlist);

		if (idx!=0 && idx!=1) return;
		if (data.get_count()==0) return;

		string8 dir;
		/*
		if (idx==1)
		{
			if (!core_api::assert_main_thread()) 
				return;
			dir = cfg_last_dir;
			if (dir.is_empty())
			{
				const char * p_fn = data[0]->handle_get_path();
				if (!strncmp(p_fn,"file://",7))
				{
					dir = p_fn + 7;
					dir.truncate(dir.scan_filename());
				}
			}

			if (!uBrowseForFolder(core_api::get_main_window(),"Select output directory",dir)) return;
			cfg_last_dir = dir;
		}
		*/
		const int count = data.get_count();

		for (int i = 0; i < count; i++) {
			metadb_handle *ptr = data.get_item(i);
			if (!ptr) 
				return;

			const char *filename = ptr->handle_get_path();
			if (filename == NULL)
				return;
			const char *ext = strchr(filename, '.');
			if (ext == NULL)
				return;

			if (!stricmp(ext, ".mp3")) {
				using namespace M2W_NAMESPACE;

				std::string inputFilename = filename+strlen("file://");

				MatroskaMuxer muxer;
				muxer.Set_OutputFilename(inputFilename+".mka");

				// open the input file		
				MPA_File input_File(inputFilename, MODE_READ);

				//printf("Converting %s to %s...\n", argv[1], output_Filename.c_str());

				input_File.SkipToFrameStart();

				MPA_Lame_Frame infoFrame;
				bool bInfoFrameOn = false;
				if (input_File.CheckInfoFrame()) {
					*static_cast<MPA_Frame *>(&infoFrame) = input_File.GetInfoFrame();
					bInfoFrameOn = true;
				}

				uint32 FrameNumber = 0;
				uint16 Layer = 0;
				uint64 SampleNumbers = 0;
				uint64 TotalFrameSize = 0;
				uint16 SamplingRate = 0;
				uint16 Channels = 0;
				uint16 SampleSize = 0;
				MPA_Frame frame;
				
				// should be enough for 32kHz/320kbps
				binary frame_buffer[5000+4];

				uint32 prevSamplesNumber = 0;
				double start_timecode, duration_timecode;
				SampleNumbers = 0;	

				while (input_File.FindNextSyncWord()) {
					FrameNumber++;
					frame = input_File.GetFrame();
					/// \todo handle the bitrate of the frame in a key/value table, so that we can detect real CBR files
					TotalFrameSize += frame.DataSize() + 4;
					SampleSize = frame.SampleSize();
					SampleNumbers += SampleSize;		
					assert(SampleSize == 1152 || SampleSize == 572);

					if (prevSamplesNumber == 0) {
						SamplingRate = frame.SamplingRate();
						Channels = frame.Channels();
						SampleSize = frame.SampleSize();
						Layer = frame.Layer();

						//printf("\tSampling Rate: %i\n", SamplingRate);
						//printf("\tChannel Count: %i\n", Channels);
						//printf("\tLayer: %i\n", Layer);
						// First pass, or write headers
						muxer.Set_Track_CodecID(1, "A_MPEG/L3");
						muxer.Set_Track_Audio(1, Channels, SamplingRate);
						muxer.Set_TimecodeScale(TIMECODE_SCALE);
						muxer.WriteHeaders();
					}

					memcpy(frame_buffer, (const binary *)(frame), 4);
					input_File.GetData(frame_buffer+4, 5000);

					start_timecode = (((double)prevSamplesNumber * 1000) / (double)SamplingRate);
					duration_timecode = (((double)SampleSize * 1000) / (double)SamplingRate);
					
					muxer.AddFrame(1, start_timecode*TIMECODE_SCALE, duration_timecode*TIMECODE_SCALE, frame_buffer, frame.DataSize()+4);
					//input_File.SkipData();

					prevSamplesNumber = SampleNumbers;
				}
			  
				uint64 totalByteSize = TotalFrameSize * SamplingRate;
				uint32 AverageBitrate, AverageByterate;
				if ((8 * totalByteSize) % SampleNumbers > SampleNumbers>>1)
					AverageBitrate = 1 + 8 * totalByteSize / SampleNumbers;
				else
					AverageBitrate = 8 * totalByteSize / SampleNumbers;

				if (AverageBitrate % 8 > 4)
					AverageByterate = 1 + (AverageBitrate >> 3);
				else
					AverageByterate = AverageBitrate >> 3;

				//printf("\tAvg Bitrate: %i\n", AverageByterate);

				//printf("Total Length: %i\n", (((int32)SampleNumbers * 1000) / SamplingRate));

				muxer.CloseFile();
			} else if (!stricmp(ext, ".flac")) {
				std::string inputFilename = filename+strlen("file://");

				MatroskaMuxer muxer;
				muxer.Set_OutputFilename(inputFilename+".mka");

				FLAC_Reader file_reader;
				file_reader.ReadStart(inputFilename.c_str());
				
				muxer.Set_Track_CodecID(1, "A_FLAC");
				muxer.Set_Track_CodecPrivate(1, file_reader.metadata_header, file_reader.metadata_size);
				muxer.Set_Track_Audio(1, file_reader.info.channels, file_reader.info.sample_rate, file_reader.info.bits_per_sample);
				muxer.Set_TimecodeScale(file_reader.info.sample_rate);
				muxer.WriteHeaders();
				
				long SamplingRate = file_reader.info.sample_rate;
				uint64 start_timecode = 0;
				uint32 duration_timecode = 0;
				FLAC_Reader_Frame *frame = file_reader.ReadNextFrame();
				while (frame != NULL) {
										
					start_timecode = (((double)frame->sample * 1000) / (double)SamplingRate);
					duration_timecode = (((double)file_reader.info.max_blocksize * 1000) / (double)SamplingRate);
					
					muxer.AddFrame(1, start_timecode*SamplingRate*1000*1000/SamplingRate, duration_timecode*SamplingRate*1000*1000/SamplingRate, frame->data, frame->frame_length);

					frame = file_reader.ReadNextFrame();
				}				

				muxer.CloseFile();
			}
		}
			/*
			file_info_i_full src_info;

			const char * codec = 0;

			if (ptr->handle_query(&src_info))
			{
				string8 extension;
				codec = src_info.info_get("codec");
				if (codec==0)
				{
					extension = string_extension(ptr->handle_get_path());
					codec = extension;
				}
				int type = 1;
				string8 temp;
				if (idx==0) temp = src_info.get_file_path();
				else
				{
					temp = "file://";
					temp += dir;
					temp.fix_dir_separator('\\');
					const char * fn = src_info.get_file_path();
					temp += fn + string8::g_scan_filename(fn);
				}
				const char *p = strrchr((const char *)temp, '.');
				if (p)
				{
					if (!stricmp(p, ".aac") || !stricmp(p, ".mp3")) type = 0;
					temp.truncate(p-(const char *)temp);
				}
				if (type == 0)
					temp.add_string(".mp4");
				else
				{
					if (!stricmp_utf8(codec,"MP3")) temp.add_string(".mp3");
					else if (!stricmp_utf8(codec,"MP2")) temp.add_string(".mp2");
					else if (!stricmp_utf8(codec,"AAC") || !stricmp_utf8(codec,"AAC+SBR")) temp.add_string(".aac");
					else {console::error("Unsupported MP4 codec.");continue;}
				}

				const char *src = (const char *)src_info.get_file_path();
				const char *dst = (const char *)temp;

				if (file::g_exists(dst))
				{
					console::info(uStringPrintf("Destination file '%s' already exists", dst));
					console::popup();
				}
				else
				{
					converter *conv = new converter(src, dst, &src_info);

					if (conv)
					{
						bool ret;

						if (type == 0)
							ret = conv->raw_to_mp4(codec);
						else
							ret = conv->mp4_to_raw();

						delete conv;

						if (ret)
							console::info(uStringPrintf("'%s' converted to '%s'", src, dst));
						else
						{
							console::error(uStringPrintf("Failed to convert '%s' to '%s'", src, dst));
							file::g_remove(dst);
						}                        
					}
					else
					{
						console::error("Failed to create new converter");
					}
				}
			}
			else
			{
				console::error("Failed to get file infos");
			}
		}
		*/
	}

protected:
	int first_num;
	string8 path;
};

static service_factory_single_t<menu_item, contextmenu_mka> foo_mka_context;

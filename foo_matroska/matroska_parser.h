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
    \file matroska_parser.h
		\version $Id$
    \brief An audio slated Matroska Reader+Parser
		\author Jory Stone     <jcsston @ toughguy.net>
*/

#ifndef _MATROSKA_PARSER_H_
#define _MATROSKA_PARSER_H_

#include "../SDK/foobar2000.h"
#include "Foobar2000ReaderIOCallback.h"
#include "DbgOut.h"
#include <queue>
#include <deque>

// libebml includes
#include "ebml/StdIOCallback.h"
#include "ebml/EbmlTypes.h"
#include "ebml/EbmlHead.h"
#include "ebml/EbmlVoid.h"
#include "ebml/EbmlCrc32.h"
#include "ebml/EbmlSubHead.h"
#include "ebml/EbmlStream.h"
#include "ebml/EbmlBinary.h"
#include "ebml/EbmlString.h"
#include "ebml/EbmlUnicodeString.h"
#include "ebml/EbmlContexts.h"
#include "ebml/EbmlVersion.h"

// libmatroska includes
#include "matroska/KaxConfig.h"
#include "matroska/KaxBlock.h"
#include "matroska/KaxSegment.h"
#include "matroska/KaxContexts.h"
#include "matroska/KaxSeekHead.h"
#include "matroska/KaxTracks.h"
#include "matroska/KaxInfo.h"
#include "matroska/KaxInfoData.h"
#include "matroska/KaxTags.h"
#include "matroska/KaxTag.h"
#include "matroska/KaxTagMulti.h"
#include "matroska/KaxCluster.h"
#include "matroska/KaxClusterData.h"
#include "matroska/KaxTrackAudio.h"
#include "matroska/KaxTrackVideo.h"
#include "matroska/KaxAttachments.h"
#include "matroska/KaxAttached.h"
#include "matroska/KaxChapters.h"
#include "matroska/KaxVersion.h"

#include "MatroskaSimpleTags.h"

#define TIMECODE_SCALE  1000000
#define MAX_UINT64 0xFFFFFFFFFFFFFFFF

//Memory Leak Debuging define
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>
#endif

#ifdef _DEBUG
   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
   #define DEBUG_CLIENTBLOCK
#endif // _DEBUG

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

using namespace LIBEBML_NAMESPACE;
using namespace LIBMATROSKA_NAMESPACE;

typedef std::vector<uint8> ByteArray;

typedef struct tagVORBISFORMAT2
{
	DWORD Channels;
	DWORD SamplesPerSec;
	DWORD BitsPerSample;	
	DWORD HeaderSize[3]; // 0: Identification, 1: Comment, 2: Setup
} VORBISFORMAT2, *PVORBISFORMAT2, FAR *LPVORBISFORMAT2;


class MatroskaAudioFrame {
public:
	MatroskaAudioFrame();
	void Reset();

	uint64 timecode;
	uint32 duration;
	std::vector<ByteArray> dataBuffer;
};

struct MatroskaMetaSeekClusterEntry {
	uint32 clusterNo;
	uint64 filePos;
	uint64 timecode;
};

struct MatroskaChapterDisplayInfo {
	MatroskaChapterDisplayInfo();

	UTFstring string;
	std::string lang;
	std::string country;
};

class MatroskaChapterInfo {
public:
	MatroskaChapterInfo();

	uint64 chapterUID;
	uint64 timeStart;
	uint64 timeEnd;
	/// Vector of all the tracks this chapter applies to
	/// if it's empty then this chapter applies to all tracks
	std::vector<uint64> tracks;
	/// Vector of strings we can display for chapter
	std::vector<MatroskaChapterDisplayInfo> display;
};

class MatroskaTrackInfo {
	public:
		/// Initializes the class
		MatroskaTrackInfo();

		uint16 trackNumber;
		uint64 trackUID;		
		std::string codecID;
		std::vector<BYTE> codecPrivate;
		bool codecPrivateReady;
		
		UTFstring name;
		std::string language;
		double duration;
		
    uint8 channels; 
    float samplesPerSec; 
		float samplesOutputPerSec;     
    uint8 bitsPerSample;     
		uint32 avgBytesPerSec; 
		float defaultDuration;
};

class MatroskaAudioParser {
public:
	MatroskaAudioParser(reader *input);
	~MatroskaAudioParser();

	/// The main header parsing function
	/// \return 0 File parsed ok
	/// \return 1 Failed
	int Parse(bool bInfoOnly = false, bool bBreakAtClusters = true);
	/// Writes the tags to the current matroska file
	/// \param info All the tags we need to write
	/// \return 0 Tags written A OK
	/// \return 1 Failed to write tags
	int WriteTags(const file_info *info);

	MatroskaTrackInfo &GetTrack(uint16 trackNo) { return m_Tracks.at(trackNo); };
	uint64 GetTimecodeScale() { return m_TimecodeScale; };
	/// Returns an adjusted duration of the file
	uint64 GetDuration();
	/// Returns the track index of the first decodable track
	int32 GetFirstAudioTrack();

	/// Set the fb2k info from the matroska file
	/// \param info This will be filled up with tags ;)
	bool SetFB2KInfo(file_info *info);

	/// Get the foobar2000 style format string
	const char *GetFoobar2000Format(uint16 trackNo, bool bSetupCodecPrivate = true);

	/// Set the current track to read data from
	void SetCurrentTrack(uint32 newTrackNo);
	/// Set the subsong to play, this adjusts all the duration/timecodes 
	/// reported in public functions. So only use this if you are expecting that to happen
	/// \param subsong This should be within the range of the chapters vector
	void SetSubSong(int subsong);

	std::vector<MatroskaChapterInfo> &GetChapters() { return m_Chapters; };

	int32 GetAvgBitrate();
	/// Seek to a position
	/// \param seconds The absolute position to seek to, in seconds			
	/// \return Current postion offset from where it was requested
	/// If you request to seek to 2.0 and we can only seek to 1.9
	/// the return value would be 100 * m_TimcodeScale

	bool skip_frames_until(uint64 timecode,unsigned & frames,uint64 & last_timecode_delta);
	bool skip_n_frames(unsigned frames);
	void flush_queue();
	uint64 get_current_frame_timecode();
	int64 Seek(double seconds,unsigned max_frame_dependency,unsigned & frames_to_skip);

	/// Seek to a position
	/// \param frame The MatroskaAudioFrame struct to store the frame	
	/// \return 0 If read ok	
	/// \return 1 If file could not be read or it not open	
	/// \return 2 End of track (EOT)
	int ReadSingleFrame(MatroskaAudioFrame *frame);

protected:
	void Parse_MetaSeek(EbmlElement *metaSeekElement, bool bInfoOnly);
	void Parse_Chapters(KaxChapters *chaptersElement);
	void Parse_Tags(KaxTags *tagsElement);
	int FillQueue();
	uint64 GetClusterTimecode(uint64 filePos);
	MatroskaMetaSeekClusterEntry *FindCluster(uint64 timecode);
	void CountClusters();
	void FixChapterEndTimes();
	// See if the chapter uid is already in our vector
	// \return true Yes, we already have this uid
	// \return false Nope
	bool FindChapterUID(uint64 uid);
	/// Adds the info tags to the current file in memory 
	void AddTags(const file_info *info);
	/// Set the info tags to the current tags file in memory
	void SetTags(const file_info *info);

	Foobar2000ReaderIOCallback m_IOCallback;
	EbmlStream m_InputStream;
	/// The main/base/master element, should be the segment
	EbmlElement *m_ElementLevel0;

	MatroskaChapterInfo *m_CurrentChapter;
	uint32 m_CurrentTrackNo;
	std::vector<MatroskaTrackInfo> m_Tracks;
	std::vector<MatroskaChapterInfo> m_Chapters;
	MatroskaTagVector m_Tags;
	
	/// This is the queue of buffered frames to deliver
	std::queue<MatroskaAudioFrame *> m_Queue;

	/// This is the index of clusters in the file, it's used to seek in the file
	std::vector<MatroskaMetaSeekClusterEntry> m_ClusterIndex;

	uint64 m_CurrentTimecode;
	uint64 m_Duration;
	uint64 m_TimecodeScale;
	UTFstring m_WritingApp;
	UTFstring m_MuxingApp;
	UTFstring m_FileTitle;
	int32 m_FileDate;
	UTFstring m_SegmentFilename;

	uint64 m_FileSize;
	uint64 m_TagPos;
	uint32 m_TagSize;
	uint32 m_TagScanRange;
	//int UpperElementLevel;
private:
	// We have no need for these
	//MatroskaAudioParser(const MatroskaAudioParser &refParser) { };
	//MatroskaAudioParser &operator=(const MatroskaAudioParser &refParser) { return *this; };
};

void PrintChapters(std::vector<MatroskaChapterInfo> &theChapters);

#endif // _MATROSKA_PARSER_H_

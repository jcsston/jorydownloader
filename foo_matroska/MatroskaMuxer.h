/*
 *  General Matroska Muxer Interface
 *
 *  Copyright (C) 2003-2004, Jory Stone <jcsston @ toughguy.net>
 *  Copyright (C) 2003, John Cannon <spyder482 @ yahoo.com>
 *  Copyright (C) 2002, Steve Lhomme <steve.lhomme @ free.fr>
 *
 *  SOMa is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  SOMa is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SOMa; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*!
	\version $Id$
	\author Jory Stone           <jcsston @ toughguy.net>
 	\brief

 	\par CVS Log
  	$Log$
  	Revision 1.2  2004/01/04 04:16:01  jcsston
  	It actual works!
  	
  	Revision 1.1  2004/01/04 01:07:31  jcsston
  	Finally some matroska muxing code
  	

*/

#ifndef _MATROSKA_MUXER_H_
#define _MATROSKA_MUXER_H_

#include <time.h>
// std include's
#include <vector>
#include <deque>
#include <queue>
#include <exception>

#include "MatroskaSimpleTags.h"

#include "Foobar2000ReaderIOCallback.h"
// libebml includes
#include "ebml/IOCallback.h"
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
#include "matroska/KaxBlockData.h"
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
#include "matroska/KaxCues.h"
#include "matroska/KaxCuesData.h"
#include "matroska/KaxAttachments.h"
#include "matroska/KaxAttached.h"
#include "matroska/KaxVersion.h"

// Our default timecode scale
#define TIMECODE_SCALE  1000000

using namespace LIBEBML_NAMESPACE;
using namespace LIBMATROSKA_NAMESPACE;

class KaxTimecodeReferenceBlock : public KaxReferenceBlock {
public:
   void SetTimecode(int64 refTimecode) { SetReferencedTimecode(refTimecode); };
};

class MatroskaMuxer {
public:
	MatroskaMuxer();
	~MatroskaMuxer();
	
	int Set_OutputFilename(const std::string &ouputFilename);
	int Set_Lacing(bool bUseLacing);
	int Set_WriteDuration(bool bWriteDuration);
	/// Set the limits of the clusters
	/// \param maxKBSize The number of KBs to limit clusters at
	/// \param maxMSLength The number of milliseconds to limit clusters
	int Set_ClusterLimits(uint32 maxKBSize = 1000, uint32 maxMSLength = 1000);
	/// This sets the global timecode scale,
  /// We divide any timecode you feed us with this to get the timecode in ms
	int Set_TimecodeScale(uint64 timecodeScale);
	int Set_Track_Type(uint8 trackNo, int trackType);
	int Set_Track_CodecID(uint8 trackNo, const std::string &codecID);
	int Set_Track_CodecPrivate(uint8 trackNo, const binary *codecPrivate, uint32 codecPrivateSize);
	int Set_Track_DefaultDuration(uint8 trackNo, uint64 defaultDuration);
	int Set_Track_Language(uint8 trackNo, const std::string &lang);
	int Set_Track_Video(uint8 trackNo, uint32 width, uint32 height, uint32 displayWidth = 0, uint32 displayHeight = 0);
	int Set_Track_Audio(uint8 trackNo, uint8 channels, double sampleRate, uint16 bitdepth = 0, double outputSampleRate = 0);
	/// Set's a tag, these are not written until the very end
	/// \param trackNo The track this tag should go with, use 0 if this is to be a global tag
	/// \param name UTF-8 tag name
	/// \param value UTF-8 tag value
	int Set_Tag(uint8 trackNo, const char *name, const char *value);
	
	
	bool WriteHeaders();

	/// Add a frame
	/// \param trackNo The track this frame belongs to
	/// \param timecode A timecode, it should be unscaled
	int AddFrame(uint16 trackNo, uint64 timecode, uint32 duration, const binary *data, uint32 dataSize, std::vector<int32> *references = NULL);
	
	int CloseFile();

	uint32 framesMuxed;
	uint32 clustersMuxed;
protected:
	bool WriteAttachments();
	bool WriteTags();
	bool ChangeClusters(uint64 timecode, bool bForce = false);
	int UpdateDuration(double newDuration);
	
	KaxTrackEntry *LookupTrackNo(uint8 trackNo);

	// Output file/filename
	IOCallback *m_outputFile;
	std::string MatroskaFilename;
	UTFstring m_MuxingApp;

	// Matroska File Elements
	KaxSegment FileSegment;
	EbmlVoid MyDummy;
	KaxSeekHead MySeekHead;
	KaxSeekHead MySeekHeadOverFill;
	KaxTracks *m_Tracks;
	KaxCluster *clust;
	KaxBlockGroup *m_MyKaxBlockGroup;
	KaxCues *m_AllCues;
	uint16 m_prevTrack;
	uint64 m_prevTimecode;
	uint64 m_lastTimecode;
	uint64 m_timecodeScale;

	MatroskaTagVector m_Tags;

	bool m_WriteDuration;
	bool m_UseLacing;
	uint8 m_LaceCount;
	uint32 m_maxTimePerCluster;
	uint32 m_maxBytesPerCluster;

	uint32 currentClusterSize;
	uint64 currentClusterBaseTimecode;
};

#endif // _MATROSKA_MUXER_H_


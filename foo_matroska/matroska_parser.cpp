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
    \file matroska_parser.cpp
		\version $Id$
    \brief An audio slated Matroska Reader+Parser
		\author Jory Stone     <jcsston @ toughguy.net>
*/

#include "matroska_parser.h"

using namespace LIBEBML_NAMESPACE;
using namespace LIBMATROSKA_NAMESPACE;

MatroskaAudioFrame::MatroskaAudioFrame() 
	: dataBuffer(1) 
{
		timecode = 0;
		duration = 0;
		//dataBufferSize = 0;
};

void MatroskaAudioFrame::Reset()
{
	timecode = 0;
	duration = 0;
	dataBuffer.clear();
};

MatroskaChapterDisplayInfo::MatroskaChapterDisplayInfo()  {
	string = L"";
};

MatroskaChapterInfo::MatroskaChapterInfo() {
	chapterUID = 0;
	timeStart = 0;
	timeEnd = 0;
}

MatroskaTrackInfo::MatroskaTrackInfo() {
	trackNumber = 0;
	trackUID = 0;		
	duration = 0;
	
  channels = 1;
  samplesPerSec = 0;
	samplesOutputPerSec = 0;
  bitsPerSample = 0;
	avgBytesPerSec = 0;
	defaultDuration = 0;

	name = L"";

	codecPrivateReady = false;
};

MatroskaAudioParser::MatroskaAudioParser(reader *input) 
	: m_IOCallback(input),
		m_InputStream(m_IOCallback)
{
	m_TimecodeScale = TIMECODE_SCALE;
	m_FileDate = 0;
	m_Duration = 0;
	m_CurrentTimecode = 0;
	m_ElementLevel0 = NULL;
	//UpperElementLevel = 0;
	m_CurrentChapter = 0;
	m_FileSize = input->get_length();
	m_TagPos = 0;
	m_TagSize = 0;
	m_TagScanRange = 1024 * 64;
	m_CurrentTrackNo = 0;
};

MatroskaAudioParser::~MatroskaAudioParser() {
	if (m_ElementLevel0 != NULL)
		delete m_ElementLevel0;
	
	while (!m_Queue.empty()) {
		MatroskaAudioFrame *currentPacket = m_Queue.front();
		delete currentPacket;
		m_Queue.pop();
	}
};

int MatroskaAudioParser::Parse(bool bInfoOnly, bool bBreakAtClusters) 
{
	try {
		int UpperElementLevel = 0;
		bool bAllowDummy = false;
		// Elements for different levels
		EbmlElement *ElementLevel1 = NULL;
		EbmlElement *ElementLevel2 = NULL;
		EbmlElement *ElementLevel3 = NULL;
		EbmlElement *ElementLevel4 = NULL;
		EbmlElement *ElementLevel5 = NULL;		

		// Be sure we are at the beginning of the file
		m_IOCallback.setFilePointer(0);
		// Find the EbmlHead element. Must be the first one.
		m_ElementLevel0 = m_InputStream.FindNextID(EbmlHead::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (m_ElementLevel0 == NULL) {
			return 1;
		}
		//We must have found the EBML head :)
		m_ElementLevel0->SkipData(m_InputStream, m_ElementLevel0->Generic().Context);
		delete m_ElementLevel0;

		// Next element must be a segment
		m_ElementLevel0 = m_InputStream.FindNextID(KaxSegment::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (m_ElementLevel0 == NULL) {
			//No segment/level 0 element found.
			return 1;
		}
		if (!(EbmlId(*m_ElementLevel0) == KaxSegment::ClassInfos.GlobalId)) {
			delete m_ElementLevel0;
			m_ElementLevel0 = NULL;
			return 1;
		}

		UpperElementLevel = 0;
		// We've got our segment, so let's find the tracks
		ElementLevel1 = m_InputStream.FindNextElement(m_ElementLevel0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
		while (ElementLevel1 != NULL) {
			if (UpperElementLevel > 0) {
				break;
			}
			if (UpperElementLevel < 0) {
				UpperElementLevel = 0;
			}

			if (EbmlId(*ElementLevel1) == KaxSeekHead::ClassInfos.GlobalId) {
				if (m_IOCallback.seekable()) {
					Parse_MetaSeek(ElementLevel1, bInfoOnly);
					if (m_TagPos == 0) {
						// Search for them at the end of the file
						if (m_TagScanRange > 0)
						{
							m_IOCallback.setFilePointer(m_FileSize - m_TagScanRange);
							binary Buffer[4];
							while (m_IOCallback.read(Buffer, 3) >= 3)
							{//0x18
								if ((Buffer[0] == 0x54) && (Buffer[1] == 0xc3) && (Buffer[2] == 0x67))
								{
									uint64 startPos = m_IOCallback.getFilePointer();

									//seek back 3 bytes, so libmatroska can find the Tags element Ebml ID
									m_IOCallback.setFilePointer(-4, seek_current);

									EbmlElement *levelUnknown = m_InputStream.FindNextID(KaxTags::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
									if ((levelUnknown != NULL) 
										&& (m_FileSize >= startPos + levelUnknown->GetSize()) 
										&& (EbmlId(*levelUnknown) == KaxTags::ClassInfos.GlobalId))
									{
										Parse_Tags(static_cast<KaxTags *>(levelUnknown));
										delete levelUnknown;
										levelUnknown = NULL;
										break;
									}
									delete levelUnknown;
									levelUnknown = NULL;

									//Restore the file pos
									m_IOCallback.setFilePointer(startPos);
								}
								//seek back 2 bytes
								m_IOCallback.setFilePointer(-2, seek_current);
							}					
						} else {
							//m_TagPos = m_FileSize;
						}
					}
				}
			}else if (EbmlId(*ElementLevel1) == KaxInfo::ClassInfos.GlobalId) {
				// General info about this Matroska file
				ElementLevel2 = m_InputStream.FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, bAllowDummy);
				while (ElementLevel2 != NULL) {
					if (UpperElementLevel > 0) {
						break;
					}
					if (UpperElementLevel < 0) {
						UpperElementLevel = 0;
					}

					if (EbmlId(*ElementLevel2) == KaxTimecodeScale::ClassInfos.GlobalId) {
						KaxTimecodeScale &TimeScale = *static_cast<KaxTimecodeScale *>(ElementLevel2);
						TimeScale.ReadData(m_InputStream.I_O());

						//matroskaGlobalTrack->SetTimecodeScale(uint64(TimeScale));
						m_TimecodeScale = uint64(TimeScale);
					} else if (EbmlId(*ElementLevel2) == KaxDuration::ClassInfos.GlobalId) {
						KaxDuration &duration = *static_cast<KaxDuration *>(ElementLevel2);
						duration.ReadData(m_InputStream.I_O());

						// it's in milliseconds?
						m_Duration = double(duration) * (double)m_TimecodeScale;
						//matroskaGlobalTrack->SetDuration(float(duration) * int64(m_TimecodeScale) / 1000000000.0);

					} else if (EbmlId(*ElementLevel2) == KaxDateUTC::ClassInfos.GlobalId) {
						KaxDateUTC & DateUTC = *static_cast<KaxDateUTC *>(ElementLevel2);
						DateUTC.ReadData(m_InputStream.I_O());
						
						m_FileDate = DateUTC.GetEpochDate();

					} else if (EbmlId(*ElementLevel2) == KaxSegmentFilename::ClassInfos.GlobalId) {
						KaxSegmentFilename &tag_SegmentFilename = *static_cast<KaxSegmentFilename *>(ElementLevel2);
						tag_SegmentFilename.ReadData(m_InputStream.I_O());

						m_SegmentFilename = *static_cast<EbmlUnicodeString *>(&tag_SegmentFilename);

					} else if (EbmlId(*ElementLevel2) == KaxMuxingApp::ClassInfos.GlobalId)	{
						KaxMuxingApp &tag_MuxingApp = *static_cast<KaxMuxingApp *>(ElementLevel2);
						tag_MuxingApp.ReadData(m_InputStream.I_O());

						m_MuxingApp = *static_cast<EbmlUnicodeString *>(&tag_MuxingApp);

					} else if (EbmlId(*ElementLevel2) == KaxWritingApp::ClassInfos.GlobalId) {
						KaxWritingApp &tag_WritingApp = *static_cast<KaxWritingApp *>(ElementLevel2);
						tag_WritingApp.ReadData(m_InputStream.I_O());
						
						m_WritingApp = *static_cast<EbmlUnicodeString *>(&tag_WritingApp);

					} else if (EbmlId(*ElementLevel2) == KaxTitle::ClassInfos.GlobalId) {
						KaxTitle &Title = *static_cast<KaxTitle*>(ElementLevel2);
						Title.ReadData(m_InputStream.I_O());
						m_FileTitle = UTFstring(Title).c_str();
					}

					if (UpperElementLevel > 0) {	// we're coming from ElementLevel3
						UpperElementLevel--;
						delete ElementLevel2;
						ElementLevel2 = ElementLevel3;
						if (UpperElementLevel > 0)
							break;
					} else {
						ElementLevel2->SkipData(m_InputStream, ElementLevel2->Generic().Context);
						delete ElementLevel2;
						ElementLevel2 = m_InputStream.FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, bAllowDummy);
					}
				}							
			}else if (EbmlId(*ElementLevel1) == KaxChapters::ClassInfos.GlobalId) {
				Parse_Chapters(static_cast<KaxChapters *>(ElementLevel1));

			}else if (EbmlId(*ElementLevel1) == KaxTags::ClassInfos.GlobalId) {
				Parse_Tags(static_cast<KaxTags *>(ElementLevel1));

			} else if (EbmlId(*ElementLevel1) == KaxTracks::ClassInfos.GlobalId) {
				// Yep, we've found our KaxTracks element. Now find all tracks
				// contained in this segment. 
				KaxTracks *Tracks = static_cast<KaxTracks *>(ElementLevel1);
				Tracks->Read(m_InputStream, KaxTracks::ClassInfos.Context, UpperElementLevel, ElementLevel2, bAllowDummy);

				unsigned int Index0;
				for (Index0 = 0; Index0 < Tracks->ListSize(); Index0++) {
					if ((*Tracks)[Index0]->Generic().GlobalId == KaxTrackEntry::ClassInfos.GlobalId) {
						KaxTrackEntry &TrackEntry = *static_cast<KaxTrackEntry *>((*Tracks)[Index0]);
						// Create a new MatroskaTrack
						MatroskaTrackInfo newTrack;
						
						unsigned int Index1;
						for (Index1 = 0; Index1 < TrackEntry.ListSize(); Index1++) {
							if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackNumber::ClassInfos.GlobalId) {
								KaxTrackNumber &TrackNumber = *static_cast<KaxTrackNumber*>(TrackEntry[Index1]);
								newTrack.trackNumber = TrackNumber;

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackUID::ClassInfos.GlobalId) {
								KaxTrackUID &TrackUID = *static_cast<KaxTrackUID*>(TrackEntry[Index1]);
								newTrack.trackUID = TrackUID;

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackType::ClassInfos.GlobalId) {
								KaxTrackType &TrackType = *static_cast<KaxTrackType*>(TrackEntry[Index1]);
								if (uint8(TrackType) != track_audio) {
									newTrack.trackNumber = 0xFFFF;
									break;
								}

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackTimecodeScale::ClassInfos.GlobalId) {
								KaxTrackTimecodeScale &TrackTimecodeScale = *static_cast<KaxTrackTimecodeScale*>(TrackEntry[Index1]);
								// TODO: Support Tracks with different timecode scales?
								//newTrack->TrackTimecodeScale = TrackTimecodeScale;

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackDefaultDuration::ClassInfos.GlobalId) {
								KaxTrackDefaultDuration &TrackDefaultDuration = *static_cast<KaxTrackDefaultDuration*>(TrackEntry[Index1]);
								newTrack.defaultDuration = uint64(TrackDefaultDuration);

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxCodecID::ClassInfos.GlobalId) {
								KaxCodecID &CodecID = *static_cast<KaxCodecID*>(TrackEntry[Index1]);
								newTrack.codecID = std::string(CodecID);

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxCodecPrivate::ClassInfos.GlobalId) {
								KaxCodecPrivate &CodecPrivate = *static_cast<KaxCodecPrivate*>(TrackEntry[Index1]);
								newTrack.codecPrivate.resize(CodecPrivate.GetSize());								
								memcpy(&newTrack.codecPrivate[0], CodecPrivate.GetBuffer(), CodecPrivate.GetSize());

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackFlagDefault::ClassInfos.GlobalId) {
								KaxTrackFlagDefault &TrackFlagDefault = *static_cast<KaxTrackFlagDefault*>(TrackEntry[Index1]);
								//newTrack->FlagDefault = TrackFlagDefault;

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackFlagEnabled::ClassInfos.GlobalId) {
								KaxTrackFlagEnabled &TrackFlagEnabled = *static_cast<KaxTrackFlagEnabled*>(TrackEntry[Index1]);
								//newTrack->FlagEnabled = TrackFlagEnabled;

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackFlagLacing::ClassInfos.GlobalId) {
								KaxTrackFlagLacing &TrackFlagLacing = *static_cast<KaxTrackFlagLacing*>(TrackEntry[Index1]);
								//newTrack->FlagLacing = TrackFlagLacing;

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackLanguage::ClassInfos.GlobalId) {
								KaxTrackLanguage &TrackLanguage = *static_cast<KaxTrackLanguage*>(TrackEntry[Index1]);
								newTrack.language = std::string(TrackLanguage);

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackMaxCache::ClassInfos.GlobalId) {
								KaxTrackMaxCache &TrackMaxCache = *static_cast<KaxTrackMaxCache*>(TrackEntry[Index1]);
								//newTrack->MaxCache = TrackMaxCache;

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackMinCache::ClassInfos.GlobalId) {
								KaxTrackMinCache &TrackMinCache = *static_cast<KaxTrackMinCache*>(TrackEntry[Index1]);
								//newTrack->MinCache = TrackMinCache;

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackName::ClassInfos.GlobalId) {
								KaxTrackName &TrackName = *static_cast<KaxTrackName*>(TrackEntry[Index1]);
								newTrack.name = TrackName;

							} else if (TrackEntry[Index1]->Generic().GlobalId == KaxTrackAudio::ClassInfos.GlobalId) {
								KaxTrackAudio &TrackAudio = *static_cast<KaxTrackAudio*>(TrackEntry[Index1]);

								unsigned int Index2;
								for (Index2 = 0; Index2 < TrackAudio.ListSize(); Index2++) {
									if (TrackAudio[Index2]->Generic().GlobalId == KaxAudioBitDepth::ClassInfos.GlobalId) {
										KaxAudioBitDepth &AudioBitDepth = *static_cast<KaxAudioBitDepth*>(TrackAudio[Index2]);
										newTrack.bitsPerSample = AudioBitDepth;

									} else if (TrackAudio[Index2]->Generic().GlobalId == KaxAudioPosition::ClassInfos.GlobalId) {
										KaxAudioPosition &AudioPosition = *static_cast<KaxAudioPosition*>(TrackAudio[Index2]);

										// TODO: Support multi-channel?
										//newTrack->audio->ChannelPositionSize = AudioPosition.GetSize();
										//newTrack->audio->ChannelPosition = new binary[AudioPosition.GetSize()+1];
										//memcpy(newTrack->audio->ChannelPosition, AudioPosition.GetBuffer(), AudioPosition.GetSize());

									} else if (TrackAudio[Index2]->Generic().GlobalId == KaxAudioChannels::ClassInfos.GlobalId) {
										KaxAudioChannels &AudioChannels = *static_cast<KaxAudioChannels*>(TrackAudio[Index2]);
										newTrack.channels = AudioChannels;

									} else if (TrackAudio[Index2]->Generic().GlobalId == KaxAudioOutputSamplingFreq::ClassInfos.GlobalId) {
										KaxAudioOutputSamplingFreq &AudioOutputSamplingFreq = *static_cast<KaxAudioOutputSamplingFreq*>(TrackAudio[Index2]);
										newTrack.samplesOutputPerSec = AudioOutputSamplingFreq;

									} else if (TrackAudio[Index2]->Generic().GlobalId == KaxAudioSamplingFreq::ClassInfos.GlobalId) {
										KaxAudioSamplingFreq &AudioSamplingFreq = *static_cast<KaxAudioSamplingFreq*>(TrackAudio[Index2]);
										newTrack.samplesPerSec = AudioSamplingFreq;
									}
								}
							}
						}
						if (newTrack.trackNumber != 0xFFFF)
							m_Tracks.push_back(newTrack);
					}
				}
			} else if (EbmlId(*ElementLevel1) == KaxCluster::ClassInfos.GlobalId) {
				
				if (bBreakAtClusters) {
					m_IOCallback.setFilePointer(ElementLevel1->GetElementPosition());
					delete ElementLevel1;
					ElementLevel1 = NULL;
					break;
				}

			} else if (EbmlId(*ElementLevel1) == KaxAttachments::ClassInfos.GlobalId) {
				// Yep, we've found our KaxAttachment element. Now find all attached files
				// contained in this segment. 				
#if 0
				ElementLevel2 = m_InputStream.FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, 0xFFFFFFFFL, true, 1);
				while (ElementLevel2 != NULL) {
					if (UpperElementLevel > 0) {
						break;
					}
					if (UpperElementLevel < 0) {
						UpperElementLevel = 0;
					}
					if (EbmlId(*ElementLevel2) == KaxAttached::ClassInfos.GlobalId) {
						// We actually found a attached file entry :D
						MatroskaAttachment *newAttachment = new MatroskaAttachment();

						ElementLevel3 = m_InputStream.FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, 0xFFFFFFFFL, true, 1);
						while (ElementLevel3 != NULL) {
							if (UpperElementLevel > 0) {
								break;
							}
							if (UpperElementLevel < 0) {
								UpperElementLevel = 0;
							}

							// Now evaluate the data belonging to this track
							if (EbmlId(*ElementLevel3) == KaxFileName::ClassInfos.GlobalId) {
								KaxFileName &attached_filename = *static_cast<KaxFileName *>(ElementLevel3);
								attached_filename.ReadData(m_InputStream.I_O());
								newAttachment->FileName = UTFstring(attached_filename).c_str();

							} else if (EbmlId(*ElementLevel3) == KaxMimeType::ClassInfos.GlobalId) {
								KaxMimeType &attached_mime_type = *static_cast<KaxMimeType *>(ElementLevel3);
								attached_mime_type.ReadData(m_InputStream.I_O());
								newAttachment->MimeType = wxString(std::string(attached_mime_type).c_str(), wxConvUTF8);

							} else if (EbmlId(*ElementLevel3) == KaxFileDescription::ClassInfos.GlobalId) {
								KaxFileDescription &attached_description = *static_cast<KaxFileDescription *>(ElementLevel3);
								attached_description.ReadData(m_InputStream.I_O());
								newAttachment->Description = UTFstring(attached_description).c_str();

							} else if (EbmlId(*ElementLevel3) == KaxFileData::ClassInfos.GlobalId) {
								KaxFileData &attached_data = *static_cast<KaxFileData *>(ElementLevel3);

								//We don't what to read the data into memory because it could be very large
								//attached_data.ReadData(m_InputStream.I_O());

								//Instead we store the Matroska filename, the start of the data and the length, so we can read it
								//later at the users request. IMHO This will save a lot of memory
								newAttachment->SourceFilename = MatroskaFilename;
								newAttachment->SourceStartPos = attached_data.GetElementPosition() + attached_data.HeadSize();
								newAttachment->SourceDataLength = attached_data.GetSize();
							}

							if (UpperElementLevel > 0) {	// we're coming from ElementLevel4
								UpperElementLevel--;
								delete ElementLevel3;
								ElementLevel3 = ElementLevel4;
								if (UpperElementLevel > 0)
									break;
							} else {
								ElementLevel3->SkipData(m_InputStream, ElementLevel3->Generic().Context);
								delete ElementLevel3;
								ElementLevel3 = m_InputStream.FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, 0xFFFFFFFFL, true, 1);
							}					
						} // while (ElementLevel3 != NULL)
						m_AttachmentList.push_back(newAttachment);
					}

					if (UpperElementLevel > 0) {	// we're coming from ElementLevel3
						UpperElementLevel--;
						delete ElementLevel2;
						ElementLevel2 = ElementLevel3;
						if (UpperElementLevel > 0)
							break;
					} else {
						ElementLevel2->SkipData(m_InputStream, ElementLevel2->Generic().Context);
						delete ElementLevel2;
						ElementLevel2 = m_InputStream.FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, 0xFFFFFFFFL, true, 1);
					}
				} // while (ElementLevel2 != NULL)							
#endif
			}

			
			if (UpperElementLevel > 0) {		// we're coming from ElementLevel2
				UpperElementLevel--;
				delete ElementLevel1;
				ElementLevel1 = ElementLevel2;
				if (UpperElementLevel > 0)
					break;
			} else {
				ElementLevel1->SkipData(m_InputStream, ElementLevel1->Generic().Context);
				delete ElementLevel1;
				ElementLevel1 = NULL;

				ElementLevel1 = m_InputStream.FindNextElement(m_ElementLevel0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
			}
		} // while (ElementLevel1 != NULL)		
	
	} catch (std::exception &ex) {
		//wxLogError(wxString(ex.what(), wxConvUTF8));
		return 1;

	} catch (...) {
		return 1;
	}

	CountClusters();
	return 0;
};

int MatroskaAudioParser::WriteTags(const file_info *info)
{
	SetTags(info);

	KaxTags & MyKaxTags = GetChild<KaxTags>(*static_cast<EbmlMaster *>(m_ElementLevel0));

	// On to writing :)
	if (m_TagPos == 0) {
		// Ok, we need to append the tags onto the end of the file, no existing tag element
		m_TagPos = m_FileSize;
	} else {
		
	}	

	//Start going through the list and adding tags
	KaxTag *MyKaxTag_last = NULL;
	for(size_t current_tag_track = 0; current_tag_track < m_Tags.size(); current_tag_track++) {
		KaxTag *MyKaxTag = NULL;
		if (MyKaxTag_last != NULL)
		{
			MyKaxTag = &GetNextChild<KaxTag>(MyKaxTags, *MyKaxTag_last);
			MyKaxTag_last = MyKaxTag;
		}else {
			MyKaxTag = &GetChild<KaxTag>(MyKaxTags);
			MyKaxTag_last = MyKaxTag;
		}
		MatroskaTagInfo &currentTag = m_Tags.at(current_tag_track);
		//The Targets group
		KaxTagTargets & MyKaxTagTargets = GetChild<KaxTagTargets>(*MyKaxTag);
		if (currentTag.targetTrackUID != 0) {
			KaxTagTrackUID & MyKaxTagTrackUID = GetChild<KaxTagTrackUID>(MyKaxTagTargets);
			*static_cast<EbmlUInteger *>(&MyKaxTagTrackUID) = currentTag.targetTrackUID;
		}
		if (currentTag.targetChapterUID != 0) {
			KaxTagChapterUID & MyKaxTagChapterUID = GetChild<KaxTagChapterUID>(MyKaxTagTargets);
			*static_cast<EbmlUInteger *>(&MyKaxTagChapterUID) = currentTag.targetChapterUID;
		}
		if (currentTag.targetAttachmentUID != 0) {
			KaxTagAttachmentUID & MyKaxTagAttachmentUID = GetChild<KaxTagAttachmentUID>(MyKaxTagTargets);
			*static_cast<EbmlUInteger *>(&MyKaxTagAttachmentUID) = currentTag.targetAttachmentUID;
		}
		
		// Add the millions of simple tags we can have ;)
		KaxTagSimple *MySimpleTag_last = NULL;
		for(size_t st = 0; st < currentTag.tags.size(); st++) {
			KaxTagSimple *MySimpleTag = NULL;
			if (MySimpleTag_last != NULL)
			{
				MySimpleTag = &GetNextChild<KaxTagSimple>(*MyKaxTag, *MySimpleTag_last);
				MySimpleTag_last = MySimpleTag;
			}else {
				MySimpleTag = &GetChild<KaxTagSimple>(*MyKaxTag);
				MySimpleTag_last = MySimpleTag;
			}
			MatroskaSimpleTag &currentSimpleTag = currentTag.tags.at(st);

			KaxTagName & MyKaxTagName = GetChild<KaxTagName>(*MySimpleTag);
			*static_cast<EbmlUnicodeString *>(&MyKaxTagName) = currentSimpleTag.name;

			KaxTagString & MyKaxTagString = GetChild<KaxTagString>(*MySimpleTag);
			*static_cast<EbmlUnicodeString *>(&MyKaxTagString) = currentSimpleTag.value;
		}

		m_IOCallback.setFilePointer(m_TagPos);
		// Now we write the tags to the file
		EbmlVoid & Dummy = GetChild<EbmlVoid>(*static_cast<EbmlMaster *>(m_ElementLevel0));
		uint64 size_of_tags = m_TagSize;
		MyKaxTags.UpdateSize(true);
		if (size_of_tags < MyKaxTags.GetSize())
			size_of_tags = MyKaxTags.GetSize();

		Dummy.SetSize(size_of_tags+8); // Size of the previous tag element
		uint64 pos = m_IOCallback.getFilePointer();
		if (pos == 0)
			//We don't want to overwrite the EBML header :P
			return 2;
		Dummy.Render(m_IOCallback);
		m_IOCallback.setFilePointer(Dummy.GetElementPosition());
		pos = m_IOCallback.getFilePointer();
		if (pos == 0)
			//We don't want to overwrite the EBML header :P
			return 2;
		size_of_tags = MyKaxTags.Render(m_IOCallback);
		if (size_of_tags == 0)
			return 1;
	}
	return 0;
};

void MatroskaAudioParser::AddTags(const file_info *info)
{
	// We use the subsong index to find out which track+chapter these tags apply to
	int subsong = info->get_subsong_index();
	if (m_Chapters.size() > 0) {
		// Ok we add the tags to tags with chapter+track UID's
	} else {
		// Just add the tags to the track UID
		MatroskaTagInfo *trackTag = m_Tags.FindTagWithTrackUID(m_Tracks.at(m_CurrentTrackNo).trackUID);
		if (trackTag == NULL) {
			MatroskaTagInfo tempTag;
			tempTag.targetTrackUID = m_Tracks.at(m_CurrentTrackNo).trackUID;
			m_Tags.push_back(tempTag);
			trackTag = &m_Tags.at(m_Tags.size()-1);
		}
		int metaDataCount = info->meta_get_count();
		for (int m = 0; m < metaDataCount; m++) {
			const char *name = info->meta_enum_name(m);
			const char *value = info->meta_enum_value(m);
			if ((name != NULL) && (value != NULL)) {
				trackTag->SetTagValue(name, value);
			}
		}
	}
};

void MatroskaAudioParser::SetTags(const file_info *info)
{
	// We use the subsong index to find out which track+chapter these tags apply to
	//int subsong = info->get_subsong_index();
	if ((m_Chapters.size() > 0) && (m_CurrentChapter != NULL)) {
		// Ok we add the tags to tags with chapter+track UID's
		MatroskaTagInfo *trackTag = m_Tags.FindTagWithChapterUID(m_CurrentChapter->chapterUID, m_Tracks.at(m_CurrentTrackNo).trackUID);
		if (trackTag == NULL) {
			MatroskaTagInfo tempTag;
			tempTag.targetTrackUID = m_Tracks.at(m_CurrentTrackNo).trackUID;
			tempTag.targetChapterUID = m_CurrentChapter->chapterUID;
			m_Tags.push_back(tempTag);
			trackTag = &m_Tags.at(m_Tags.size()-1);
		}
		// Clear the existing tags
		trackTag->tags.clear();
		int metaDataCount = info->meta_get_count();
		for (int m = 0; m < metaDataCount; m++) {
			const char *name = info->meta_enum_name(m);
			const char *value = info->meta_enum_value(m);
			if ((name != NULL) && (value != NULL)) {
				trackTag->SetTagValue(name, value);
			}
		}
	} else {
		// Just add the tags to the track UID
		MatroskaTagInfo *trackTag = m_Tags.FindTagWithTrackUID(m_Tracks.at(m_CurrentTrackNo).trackUID);
		if (trackTag == NULL) {
			MatroskaTagInfo tempTag;
			tempTag.targetTrackUID = m_Tracks.at(m_CurrentTrackNo).trackUID;
			m_Tags.push_back(tempTag);
			trackTag = &m_Tags.at(m_Tags.size()-1);
		}
		// Clear the existing tags
		trackTag->tags.clear();
		int metaDataCount = info->meta_get_count();
		for (int m = 0; m < metaDataCount; m++) {
			const char *name = info->meta_enum_name(m);
			const char *value = info->meta_enum_value(m);
			if ((name != NULL) && (value != NULL)) {
				trackTag->SetTagValue(name, value);
			}
		}
	}
};

uint64 MatroskaAudioParser::GetDuration() { 
	if (m_CurrentChapter != NULL) { 
		return m_CurrentChapter->timeEnd - m_CurrentChapter->timeStart;
	};
	return m_Duration; 
};

int32 MatroskaAudioParser::GetFirstAudioTrack() {
	for (uint16 t = 0; t < m_Tracks.size(); t++) {
		if (lstrlen(GetFoobar2000Format(t, false)) > 0)
			return t;
	}
	return -1;
}

bool MatroskaAudioParser::SetFB2KInfo(file_info *info)
{
	if (m_MuxingApp.length() > 0)
		info->info_set("MUXING_APP", m_MuxingApp.GetUTF8().c_str());
	if (m_WritingApp.length() > 0)
		info->info_set("WRITING_APP", m_WritingApp.GetUTF8().c_str());
	if (m_FileTitle.length() > 0)
		info->info_set("TITLE", m_FileTitle.GetUTF8().c_str());
		
	if (m_CurrentChapter != NULL) {
		MatroskaTagInfo *currentTags = m_Tags.FindTagWithChapterUID(m_CurrentChapter->chapterUID, m_Tracks.at(m_CurrentTrackNo).trackUID);		
		if (currentTags != NULL) {
			for (size_t s = 0; s < currentTags->tags.size(); s++) {
				MatroskaSimpleTag &currentSimpleTag = currentTags->tags.at(s);

				info->meta_set(currentSimpleTag.name.GetUTF8().c_str(), currentSimpleTag.value.GetUTF8().c_str());	
			}
		}
		if ((info->meta_get("TITLE") == NULL) || (strlen(info->meta_get("TITLE")) == 0))
			info->meta_set("TITLE", m_CurrentChapter->display.at(0).string.GetUTF8().c_str());
	} else {
		MatroskaTagInfo *currentTags = m_Tags.FindTagWithTrackUID(m_Tracks.at(m_CurrentTrackNo).trackUID);
		if (currentTags != NULL) {
			for (size_t s = 0; s < currentTags->tags.size(); s++) {
				MatroskaSimpleTag &currentSimpleTag = currentTags->tags.at(s);

				info->meta_set(currentSimpleTag.name.GetUTF8().c_str(), currentSimpleTag.value.GetUTF8().c_str());	
			}
		}
	}
	return true;
};

const char *MatroskaAudioParser::GetFoobar2000Format(uint16 trackNo, bool bSetupCodecPrivate) {
	try {
		MatroskaTrackInfo &currentTrack = m_Tracks.at(trackNo);
		if (currentTrack.codecID.find("A_MPEG/L3") != currentTrack.codecID.npos) {
			return "MP3";

		} else if (currentTrack.codecID.find("A_AAC") != currentTrack.codecID.npos) {
			// We need to setup the codecprivate
			if (bSetupCodecPrivate && !currentTrack.codecPrivateReady) {
				currentTrack.codecPrivate.resize(2);

				char profile, srate_idx;

				if(currentTrack.codecID.find("/MAIN") != currentTrack.codecID.npos) 
					profile = 0;
				else if(currentTrack.codecID.find("/SBR") != currentTrack.codecID.npos)
					profile = -1;
				else if(currentTrack.codecID.find("/LC") != currentTrack.codecID.npos)
					profile = 1;
				else if(currentTrack.codecID.find("/SSR") != currentTrack.codecID.npos) 
					profile = 2;
				else if(currentTrack.codecID.find("/LTP") != currentTrack.codecID.npos) 
					profile = 3;
				else 
					// Unknown AAC format
					return "";

				if(92017 <= currentTrack.samplesPerSec) srate_idx = 0;
				else if(75132 <= currentTrack.samplesPerSec) srate_idx = 1;
				else if(55426 <= currentTrack.samplesPerSec) srate_idx = 2;
				else if(46009 <= currentTrack.samplesPerSec) srate_idx = 3;
				else if(37566 <= currentTrack.samplesPerSec) srate_idx = 4;
				else if(27713 <= currentTrack.samplesPerSec) srate_idx = 5;
				else if(23004 <= currentTrack.samplesPerSec) srate_idx = 6;
				else if(18783 <= currentTrack.samplesPerSec) srate_idx = 7;
				else if(13856 <= currentTrack.samplesPerSec) srate_idx = 8;
				else if(11502 <= currentTrack.samplesPerSec) srate_idx = 9;
				else if(9391 <= currentTrack.samplesPerSec) srate_idx = 10;
				else srate_idx = 11;

				currentTrack.codecPrivate[0] = ((abs(profile) + 1) << 3) | ((srate_idx & 0xe) >> 1);
				currentTrack.codecPrivate[1] = ((srate_idx & 0x1) << 7) | ((BYTE)currentTrack.channels << 3);

				if(profile < 0)
				{
					currentTrack.codecPrivate[0] = ((abs(profile) + 1) << 3) | ((srate_idx & 0xe) >> 1);

					currentTrack.codecPrivate.resize(5);

					//pwfe->nSamplesPerSec *= 2;
					//pwfe->nAvgBytesPerSec *= 2;

					if(92017 <= currentTrack.samplesOutputPerSec) srate_idx = 0;
					else if(75132 <= currentTrack.samplesOutputPerSec) srate_idx = 1;
					else if(55426 <= currentTrack.samplesOutputPerSec) srate_idx = 2;
					else if(46009 <= currentTrack.samplesOutputPerSec) srate_idx = 3;
					else if(37566 <= currentTrack.samplesOutputPerSec) srate_idx = 4;
					else if(27713 <= currentTrack.samplesOutputPerSec) srate_idx = 5;
					else if(23004 <= currentTrack.samplesOutputPerSec) srate_idx = 6;
					else if(18783 <= currentTrack.samplesOutputPerSec) srate_idx = 7;
					else if(13856 <= currentTrack.samplesOutputPerSec) srate_idx = 8;
					else if(11502 <= currentTrack.samplesOutputPerSec) srate_idx = 9;
					else if(9391 <= currentTrack.samplesOutputPerSec) srate_idx = 10;
					else srate_idx = 11;

					currentTrack.codecPrivate[2] = 0x2B7>>3;
					currentTrack.codecPrivate[3] = (0x2B7<<5) | 5;
					currentTrack.codecPrivate[4] = (1<<7) | (srate_idx<<3);
				}
				currentTrack.codecPrivateReady = true;
			}
			return "AAC";

		} else if (currentTrack.codecID.find("A_VORBIS") != currentTrack.codecID.npos) {
			// We need to setup the codecprivate
			if (bSetupCodecPrivate && !currentTrack.codecPrivateReady) {
				std::vector<BYTE> initData;
				initData.resize(sizeof(VORBISFORMAT2) + currentTrack.codecPrivate.size() - 3);
																
				binary *codecPrivate = (unsigned __int8*)&currentTrack.codecPrivate[0];
				uint32 lastHeaderSize = currentTrack.codecPrivate.size() - 1;
				uint8 nbHeaders = *((uint8 *)codecPrivate);
				codecPrivate++;			
				
				VORBISFORMAT2 *pvf2 = (VORBISFORMAT2 *)&initData[0];
				// 3 headers for vorbis
				while(nbHeaders)
				{
					uint32 currentHeaderSize = 0;
					do{
						currentHeaderSize += *(uint8 *)codecPrivate;
						lastHeaderSize--;
					} while((*codecPrivate++) == 0xFF);
					lastHeaderSize -= currentHeaderSize;
					pvf2->HeaderSize[2-nbHeaders] = currentHeaderSize;
					nbHeaders--;
				}			
				pvf2->HeaderSize[2-nbHeaders] = lastHeaderSize;

				uint32 totalHeaderSize = pvf2->HeaderSize[0]+pvf2->HeaderSize[1]+pvf2->HeaderSize[2];
				initData.resize(sizeof(VORBISFORMAT2) + totalHeaderSize);
				memcpy(&initData[0]+sizeof(VORBISFORMAT2), codecPrivate, totalHeaderSize);

				pvf2->Channels = currentTrack.channels;
				pvf2->SamplesPerSec = currentTrack.samplesPerSec;
				pvf2->BitsPerSample = currentTrack.bitsPerSample;						

				currentTrack.codecPrivate = initData;
				currentTrack.codecPrivateReady = true;
			}
			return "Vorbis";

		} else if (currentTrack.codecID.find("A_FLAC") != currentTrack.codecID.npos) {
			return "FLAC";

		} else if (currentTrack.codecID.find("A_PCM/INT/LIT") != currentTrack.codecID.npos) {
			// We need to setup the codecprivate
			if (bSetupCodecPrivate && !currentTrack.codecPrivateReady) {
				currentTrack.codecPrivate.resize(sizeof(WAVEFORMATEX));
				WAVEFORMATEX *wfx = (WAVEFORMATEX *)&currentTrack.codecPrivate[0];
				wfx->wFormatTag = 0x01;
				wfx->nChannels = currentTrack.channels;
				wfx->nSamplesPerSec = currentTrack.samplesPerSec;
				wfx->wBitsPerSample = currentTrack.bitsPerSample;
				wfx->nBlockAlign = wfx->nChannels * wfx->wBitsPerSample / 8;
				wfx->nAvgBytesPerSec = wfx->nBlockAlign * wfx->nSamplesPerSec;
				wfx->cbSize = 0;

				currentTrack.codecPrivateReady = true;
			}
			return "PCM";

		} else if (currentTrack.codecID.find("A_MS/ACM") != currentTrack.codecID.npos) {
			WAVEFORMATEX *wfx = (WAVEFORMATEX *)&currentTrack.codecPrivate[0];
			
			if (wfx == NULL || currentTrack.codecPrivate.size() < sizeof(WAVEFORMATEX))
				return "";

			if (wfx->wFormatTag == 0x01) {
				return "PCM";
			}else if (wfx->wFormatTag == 0x085) {
				return "MP3";
			} else if (wfx->wFormatTag == 0xf1ac) {
				return "FLAC";
			}
		}
		return "";
	} catch (...) {
		return "";
	}
};

void MatroskaAudioParser::SetCurrentTrack(uint32 newTrackNo)
{
	m_CurrentTrackNo = newTrackNo;
	// Clear the current queue (we are changing tracks)
	while (!m_Queue.empty())
		m_Queue.pop();
};

void MatroskaAudioParser::SetSubSong(int subsong)
{
	if (m_Chapters.size() > subsong) {
		MatroskaChapterInfo &selectedChapter = m_Chapters.at(subsong);
		m_CurrentChapter = &selectedChapter;		
	} else {
		m_CurrentChapter = NULL;
	}
};

int32 MatroskaAudioParser::GetAvgBitrate() 
{ 
	double ret = 0;
	ret = (double)m_FileSize / 1024;
	ret = ret / ((double)m_Duration / 1000000000.0);
	ret = ret * 8;
	return (int32)ret;
};

bool MatroskaAudioParser::skip_frames_until(uint64 destination,unsigned & frames,uint64 & last_timecode_delta)
{
	unsigned done = 0;
	unsigned last_laced = 0;
	uint64 last_timecode = get_current_frame_timecode();
	if (last_timecode == (uint64)(-1)) return false;
	for(;;)
	{
		while (!m_Queue.empty()) {
			MatroskaAudioFrame *currentPacket = m_Queue.front();
			if (currentPacket->timecode > destination)
			{
				if (done==0) return false;
				last_timecode_delta = destination - last_timecode;
				frames = done - last_laced;
				return true;
			}
			last_timecode = currentPacket->timecode;
			done += (last_laced = currentPacket->dataBuffer.size());
			delete currentPacket;
			m_Queue.pop();
		}
		if (FillQueue()!=0)
			return false;
	}
}

bool MatroskaAudioParser::skip_n_frames(unsigned frames)
{
	while(frames>0)
	{
		while(frames>0 && !m_Queue.empty())
		{
			frames--;
			MatroskaAudioFrame *currentPacket = m_Queue.front();
			delete currentPacket;
			m_Queue.pop();
		}

		if (frames>0)
		{
			if (FillQueue()!=0) return false;
		}
	}
	return true;
}

void MatroskaAudioParser::flush_queue()
{
	while (!m_Queue.empty()) {
		MatroskaAudioFrame *currentPacket = m_Queue.front();
		delete currentPacket;
		m_Queue.pop();
	}
}

uint64 MatroskaAudioParser::get_current_frame_timecode()
{
	if (m_Queue.empty())
	{
		if (FillQueue()!=0) return -1;
		if (m_Queue.empty()) return -1;
	}
	return m_Queue.front()->timecode;
}

int64 MatroskaAudioParser::Seek(double seconds,unsigned max_frame_dependency,unsigned & frames_to_skip)
{
	int64 ret = 0;	
	if (m_CurrentChapter != NULL) {
		seconds += (double)m_CurrentChapter->timeStart / 1000000000.0;
	}
	// seconds * 44100 * 22050 = current timecode ?!?!?!?!
	// 270 seconds = FindCluster(timecode = 6104715) seeking to cluster 230
	uint64 seekToTimecode = (uint64)(seconds * (double)m_TimecodeScale * 1000 + 0.5);
	
	uint64 timecode_delta = 0;
	

	for(;;)
	{
		flush_queue();
		m_CurrentTimecode = seekToTimecode - timecode_delta;

		uint64 samples_to_skip;
		unsigned delta_frames;
		if (!skip_frames_until(seekToTimecode,delta_frames,samples_to_skip)) return -1;
		if (delta_frames >= max_frame_dependency || timecode_delta>=seekToTimecode)//success
		{
			frames_to_skip = delta_frames;	
			flush_queue();
			m_CurrentTimecode = seekToTimecode - timecode_delta;
			if (FillQueue()!=0) return -1;
			return samples_to_skip;
		}

		if (timecode_delta==0)
		{
			timecode_delta = (uint64)((double)m_TimecodeScale * 1024.0 * 1000.0 / 44100.0);
		}
		else
		{
			timecode_delta<<=1;
			if (timecode_delta>seekToTimecode) timecode_delta = seekToTimecode;
		}

	}
};

int MatroskaAudioParser::ReadSingleFrame(MatroskaAudioFrame *frame)
{
	//NOTE1("MatroskaAudioParser::ReadSingleFrame(frame = 0x%p)", frame);
	int ret = 0;
	if (!m_Queue.empty()) {
		MatroskaAudioFrame *newFrame = m_Queue.front();		
/*		if (m_CurrentChapter != NULL) {
			if (newFrame->timecode > m_CurrentChapter->timeEnd) {
				return 2;
			}
		}*/
		*frame = *newFrame;
		delete newFrame;
		m_Queue.pop();
	} else {
		// blah
		ret = FillQueue();
		if (ret != 0) {
			frame->Reset();
			return ret;
		}
		return ReadSingleFrame(frame);
	}
	return ret;
};


void MatroskaAudioParser::Parse_MetaSeek(EbmlElement *metaSeekElement, bool bInfoOnly) 
{
	uint64 lastSeekPos = 0;
	uint64 endSeekPos = 0;
	EbmlElement *l2 = NULL;
	EbmlElement *l3 = NULL;
	int UpperElementLevel = 0;

	if (metaSeekElement == NULL)
		return;

	l2 = m_InputStream.FindNextElement(metaSeekElement->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
	while (l2 != NULL) {
		if (UpperElementLevel > 0) {
			break;
		}
		if (UpperElementLevel < 0) {
			UpperElementLevel = 0;
		}

		if (EbmlId(*l2) == KaxSeek::ClassInfos.GlobalId) {
			//Wow we found the SeekEntries, time to speed up reading ;)
			l3 = m_InputStream.FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);

			EbmlId *id = NULL;
			while (l3 != NULL) {
				if (UpperElementLevel > 0) {
					break;
				}
				if (UpperElementLevel < 0) {
					UpperElementLevel = 0;
				}

				if (EbmlId(*l3) == KaxSeekID::ClassInfos.GlobalId) {
					binary *b = NULL;
					uint16 s = 0;
					KaxSeekID &seek_id = static_cast<KaxSeekID &>(*l3);
					seek_id.ReadData(m_InputStream.I_O(), SCOPE_ALL_DATA);
					b = seek_id.GetBuffer();
					s = (uint16)seek_id.GetSize();
					delete id;
					id = NULL;
					id = new EbmlId(b, s);

				} else if (EbmlId(*l3) == KaxSeekPosition::ClassInfos.GlobalId) {
					KaxSeekPosition &seek_pos = static_cast<KaxSeekPosition &>(*l3);
					seek_pos.ReadData(m_InputStream.I_O());				
					lastSeekPos = uint64(seek_pos);
					if (endSeekPos < lastSeekPos)
						endSeekPos = uint64(seek_pos);

					if ((*id == KaxCluster::ClassInfos.GlobalId) && !bInfoOnly) {
						//NOTE1("Found Cluster Seek Entry Postion: %u", (unsigned long)lastSeekPos);
						//uint64 orig_pos = inputFile.getFilePointer();									
						MatroskaMetaSeekClusterEntry newCluster;
						newCluster.timecode = MAX_UINT64;
						newCluster.filePos = static_cast<KaxSegment *>(m_ElementLevel0)->GetGlobalPosition(lastSeekPos);
						m_ClusterIndex.push_back(newCluster);

					} else if ((*id == KaxSeekHead::ClassInfos.GlobalId) && !bInfoOnly) {
						NOTE1("Found MetaSeek Seek Entry Postion: %u", (unsigned long)lastSeekPos);
						uint64 orig_pos = m_IOCallback.getFilePointer();									
						m_IOCallback.setFilePointer(static_cast<KaxSegment *>(m_ElementLevel0)->GetGlobalPosition(lastSeekPos));
						
						EbmlElement *levelUnknown = m_InputStream.FindNextID(KaxSeekHead::ClassInfos, 0xFFFFFFFFFFFFFFFFL);										
						Parse_MetaSeek(levelUnknown, false);

						m_IOCallback.setFilePointer(orig_pos);
					}

				} else {

				}
				l3->SkipData(m_InputStream, l3->Generic().Context);
				delete l3;
				l3 = NULL;
				l3 = m_InputStream.FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
			} // while (l3 != NULL)
			delete id;
			id = NULL;
		} else {

		}

		if (UpperElementLevel > 0) {    // we're coming from l3
			UpperElementLevel--;
			delete l2;
			l2 = l3;
			if (UpperElementLevel > 0)
				break;

		} else {
			l2->SkipData(m_InputStream, l2->Generic().Context);
			delete l2;
			l2 = NULL;
			l2 = m_InputStream.FindNextElement(metaSeekElement->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
		}
	} // while (l2 != NULL)
}

void MatroskaAudioParser::Parse_Chapters(KaxChapters *chaptersElement)
{
	EbmlElement *ElementLevel2 = NULL;
	EbmlElement *ElementLevel3 = NULL;
	EbmlElement *ElementLevel4 = NULL;
	EbmlElement *ElementLevel5 = NULL;
	int UpperElementLevel = 0;

	if (chaptersElement == NULL)
		return;

	// Even this doesn't scan past the second chapter in my test file
	//chaptersElement->Read(m_InputStream, KaxChapters::ClassInfos.Context, UpperElementLevel, ElementLevel2, false);

	ElementLevel2 = m_InputStream.FindNextElement(chaptersElement->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
	while (ElementLevel2 != NULL) {
		if (UpperElementLevel > 0) {
			break;
		}
		if (UpperElementLevel < 0) {
			UpperElementLevel = 0;
		}

		if (EbmlId(*ElementLevel2) == KaxEditionEntry::ClassInfos.GlobalId) {
			//Wow we found the EditionEntry, time to speed up reading ;)
			ElementLevel3 = m_InputStream.FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);

			while (ElementLevel3 != NULL) {
				if (UpperElementLevel > 0) {
					break;
				}
				if (UpperElementLevel < 0) {
					UpperElementLevel = 0;
				}

				if (EbmlId(*ElementLevel3) == KaxChapterAtom::ClassInfos.GlobalId) {					
					// A new chapter :)
					MatroskaChapterInfo newChapter;
					ElementLevel4 = m_InputStream.FindNextElement(ElementLevel3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);

					while (ElementLevel4 != NULL) {
						if (UpperElementLevel > 0) {
							break;
						}
						if (UpperElementLevel < 0) {
							UpperElementLevel = 0;
						}

						if (EbmlId(*ElementLevel4) == KaxChapterUID::ClassInfos.GlobalId) {							
							KaxChapterUID &chapterUID = *static_cast<KaxChapterUID *>(ElementLevel4);
							chapterUID.ReadData(m_InputStream.I_O());

							newChapter.chapterUID = uint64(chapterUID);

						} else if (EbmlId(*ElementLevel4) == KaxChapterTimeStart::ClassInfos.GlobalId) {							
							KaxChapterTimeStart &timeStart = *static_cast<KaxChapterTimeStart *>(ElementLevel4);
							timeStart.ReadData(m_InputStream.I_O());

							newChapter.timeStart = uint64(timeStart);

						} else if (EbmlId(*ElementLevel4) == KaxChapterTimeEnd::ClassInfos.GlobalId) {							
							KaxChapterTimeEnd &timeEnd = *static_cast<KaxChapterTimeEnd *>(ElementLevel4);
							timeEnd.ReadData(m_InputStream.I_O());

							newChapter.timeEnd = uint64(timeEnd);

						} else if (EbmlId(*ElementLevel4) == KaxChapterTrack::ClassInfos.GlobalId) {							
							ElementLevel5 = m_InputStream.FindNextElement(ElementLevel4->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);

							while (ElementLevel5 != NULL) {
								if (UpperElementLevel > 0) {
									break;
								}
								if (UpperElementLevel < 0) {
									UpperElementLevel = 0;
								}

								if (EbmlId(*ElementLevel5) == KaxChapterTrackNumber::ClassInfos.GlobalId) {							
									KaxChapterTrackNumber &chapTrackNo = *static_cast<KaxChapterTrackNumber *>(ElementLevel5);
									chapTrackNo.ReadData(m_InputStream.I_O());

									newChapter.tracks.push_back(uint64(chapTrackNo));
								}

								ElementLevel5->SkipData(m_InputStream, ElementLevel5->Generic().Context);
								delete ElementLevel5;
								ElementLevel5 = NULL;
								ElementLevel5 = m_InputStream.FindNextElement(ElementLevel4->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
							} // while (ElementLevel5 != NULL)

						} else if (EbmlId(*ElementLevel4) == KaxChapterDisplay::ClassInfos.GlobalId) {							
							// A new chapter display string+lang+country
							MatroskaChapterDisplayInfo newChapterDisplay;

							ElementLevel5 = m_InputStream.FindNextElement(ElementLevel4->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);

							while (ElementLevel5 != NULL) {
								if (UpperElementLevel > 0) {
									break;
								}
								if (UpperElementLevel < 0) {
									UpperElementLevel = 0;
								}

								if (EbmlId(*ElementLevel5) == KaxChapterString::ClassInfos.GlobalId) {							
									KaxChapterString &chapString = *static_cast<KaxChapterString *>(ElementLevel5);
									chapString.ReadData(m_InputStream.I_O());

									newChapterDisplay.string = UTFstring(chapString);

								} else if (EbmlId(*ElementLevel5) == KaxChapterLanguage::ClassInfos.GlobalId) {							
									KaxChapterLanguage &chapLang = *static_cast<KaxChapterLanguage *>(ElementLevel5);
									chapLang.ReadData(m_InputStream.I_O());

									newChapterDisplay.lang = std::string(chapLang);

								} else if (EbmlId(*ElementLevel5) == KaxChapterCountry::ClassInfos.GlobalId) {							
									KaxChapterCountry &chapCountry = *static_cast<KaxChapterCountry *>(ElementLevel5);
									chapCountry.ReadData(m_InputStream.I_O());

									newChapterDisplay.country = std::string(chapCountry);
								}

								ElementLevel5->SkipData(m_InputStream, ElementLevel5->Generic().Context);
								delete ElementLevel5;
								ElementLevel5 = NULL;
								ElementLevel5 = m_InputStream.FindNextElement(ElementLevel4->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
							} // while (ElementLevel5 != NULL)
							// A emtpy string in a chapter display string is usless
							if (newChapterDisplay.string.length() > 0)
								newChapter.display.push_back(newChapterDisplay);
						}

						if (UpperElementLevel > 0) {	// we're coming from ElementLevel5
							// Yet another HACK !!!!
							if ((ElementLevel5 != NULL) && (EbmlId(*ElementLevel5) == KaxChapterAtom::ClassInfos.GlobalId)) {
								// For some reason it doesn't want to read more than one chapter atom
								// The UpperElementLevel is not high enough
								UpperElementLevel++;								
							}
							UpperElementLevel--;
							delete ElementLevel4;
							ElementLevel4 = ElementLevel5;
							if (UpperElementLevel > 0)
								break;
						} else {
							ElementLevel4->SkipData(m_InputStream, ElementLevel4->Generic().Context);
							delete ElementLevel4;
							ElementLevel4 = NULL;
							ElementLevel4 = m_InputStream.FindNextElement(ElementLevel3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
						}
					} // while (ElementLevel4 != NULL)
					if ((newChapter.chapterUID != 0) && !FindChapterUID(newChapter.chapterUID))
						m_Chapters.push_back(newChapter);
				}
				if (UpperElementLevel > 0) {	// we're coming from ElementLevel4
					UpperElementLevel--;
					delete ElementLevel3;
					ElementLevel3 = ElementLevel4;
					if (UpperElementLevel > 0)
						break;
				} else {
					ElementLevel3->SkipData(m_InputStream, ElementLevel3->Generic().Context);
					delete ElementLevel3;
					ElementLevel3 = NULL;
					ElementLevel3 = m_InputStream.FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
				}
			} // while (ElementLevel3 != NULL)
		}

		if (UpperElementLevel > 0) {    // we're coming from ElementLevel3
			UpperElementLevel--;
			delete ElementLevel2;
			ElementLevel2 = ElementLevel3;
			if (UpperElementLevel > 0)
				break;

		} else {
			ElementLevel2->SkipData(m_InputStream, ElementLevel2->Generic().Context);
			delete ElementLevel2;
			ElementLevel2 = NULL;
			ElementLevel2 = m_InputStream.FindNextElement(chaptersElement->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
		}
	} // while (ElementLevel2 != NULL)	
	FixChapterEndTimes();
}

void MatroskaAudioParser::Parse_Tags(KaxTags *tagsElement)
{
	EbmlElement *ElementLevel2 = NULL;
	EbmlElement *ElementLevel3 = NULL;
	EbmlElement *ElementLevel4 = NULL;
	EbmlElement *ElementLevel5 = NULL;
	int UpperElementLevel = 0;

	if (tagsElement == NULL)
		return;

	m_TagPos = tagsElement->GetElementPosition();
	m_TagSize = tagsElement->GetSize();

	ElementLevel2 = m_InputStream.FindNextElement(tagsElement->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, false, 1);
	while (ElementLevel2 != NULL) {
		if (UpperElementLevel > 0) {
			break;
		}
		if (UpperElementLevel < 0) {
			UpperElementLevel = 0;
		}
		if (EbmlId(*ElementLevel2) == KaxTag::ClassInfos.GlobalId) {
			// We actually found a tag :) We're happy now.
			//uint64 tagPos = ElementLevel2->GetElementPosition();
			MatroskaTagInfo newTag;

			ElementLevel3 = m_InputStream.FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, false, 1);
			while (ElementLevel3 != NULL) {
				if (UpperElementLevel > 0) {
					break;
				}
				if (UpperElementLevel < 0) {
					UpperElementLevel = 0;
				}

				if (EbmlId(*ElementLevel3) == KaxTagTargets::ClassInfos.GlobalId) {

					ElementLevel4 = m_InputStream.FindNextElement(ElementLevel3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, false, 1);
					while (ElementLevel4 != NULL) {
						if (UpperElementLevel > 0) {
							break;
						}
						if (UpperElementLevel < 0) {
							UpperElementLevel = 0;
						}

						// Now evaluate the data belonging to this track
						if (EbmlId(*ElementLevel4) == KaxTagTrackUID::ClassInfos.GlobalId) {
							KaxTagTrackUID &tuid = *static_cast<KaxTagTrackUID *>(ElementLevel4);
							tuid.ReadData(m_InputStream.I_O());
							newTag.targetTrackUID = (uint64)tuid;
						
						} else if (EbmlId(*ElementLevel4) == KaxTagChapterUID::ClassInfos.GlobalId) {
							KaxTagChapterUID &tuid = *static_cast<KaxTagChapterUID *>(ElementLevel4);
							tuid.ReadData(m_InputStream.I_O());
							newTag.targetChapterUID = (uint64)tuid;

						} else if (EbmlId(*ElementLevel4) == KaxTagAttachmentUID::ClassInfos.GlobalId) {
							KaxTagAttachmentUID &tuid = *static_cast<KaxTagAttachmentUID *>(ElementLevel4);
							tuid.ReadData(m_InputStream.I_O());
							newTag.targetAttachmentUID = (uint64)tuid;
						}

						if (UpperElementLevel > 0) {
							UpperElementLevel--;
							delete ElementLevel4;
							ElementLevel4 = ElementLevel5;
							if (UpperElementLevel > 0)
								break;

							//assert(1 == 0);	// this should never happen
						} else {
							ElementLevel4->SkipData(m_InputStream, ElementLevel4->Generic().Context);
							delete ElementLevel4;
							ElementLevel4 = m_InputStream.FindNextElement(ElementLevel3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
						}
					} // while (ElementLevel4 != NULL)
				} else if (EbmlId(*ElementLevel3) == KaxTagSimple::ClassInfos.GlobalId) {
					MatroskaSimpleTag newSimpleTag;

					ElementLevel4 = m_InputStream.FindNextElement(ElementLevel3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, false, 1);
					while (ElementLevel4 != NULL) {
						if (UpperElementLevel > 0) {
							break;
						}
						if (UpperElementLevel < 0) {
							UpperElementLevel = 0;
						}

						// Now evaluate the data belonging to this track
						if (EbmlId(*ElementLevel4) == KaxTagName::ClassInfos.GlobalId) {
							KaxTagName &tagName = *static_cast<KaxTagName *>(ElementLevel4);
							tagName.ReadData(m_InputStream.I_O());
							newSimpleTag.name = wcsupr((wchar_t *)UTFstring(tagName).c_str());
						
						} else if (EbmlId(*ElementLevel4) == KaxTagString::ClassInfos.GlobalId) {
							KaxTagString &tagString = *static_cast<KaxTagString *>(ElementLevel4);
							tagString.ReadData(m_InputStream.I_O());
							newSimpleTag.value = tagString;
						}

						if (UpperElementLevel > 0) {
							UpperElementLevel--;
							delete ElementLevel4;
							ElementLevel4 = ElementLevel5;
							if (UpperElementLevel > 0)
								break;

							//assert(1 == 0);	// this should never happen
						} else {
							ElementLevel4->SkipData(m_InputStream, ElementLevel4->Generic().Context);
							delete ElementLevel4;
							ElementLevel4 = m_InputStream.FindNextElement(ElementLevel3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, false, 1);
						}
					} // while (ElementLevel4 != NULL)
					newTag.tags.push_back(newSimpleTag);
				}

				if (UpperElementLevel > 0) {	// we're coming from ElementLevel4
					UpperElementLevel--;
					delete ElementLevel3;
					ElementLevel3 = ElementLevel4;
					if (UpperElementLevel > 0)
						break;
				} else {
					ElementLevel3->SkipData(m_InputStream, ElementLevel3->Generic().Context);
					delete ElementLevel3;
					ElementLevel3 = m_InputStream.FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, false, 1);
				}
			} // while (ElementLevel3 != NULL)							
			m_Tags.push_back(newTag);
		}
		
		if (UpperElementLevel > 0) {	// we're coming from ElementLevel3
			UpperElementLevel--;
			delete ElementLevel2;
			ElementLevel2 = ElementLevel3;
			if (UpperElementLevel > 0)
				break;
		} else {
			ElementLevel2->SkipData(m_InputStream, ElementLevel2->Generic().Context);
			delete ElementLevel2;
			ElementLevel2 = m_InputStream.FindNextElement(tagsElement->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, false, 1);
		}
	} // while (ElementLevel2 != NULL)							

};

int MatroskaAudioParser::FillQueue() 
{
	NOTE("MatroskaAudioParser::FillQueue()");

	int UpperElementLevel = 0;
	bool bAllowDummy = false;
	// Elements for different levels
	EbmlElement *ElementLevel1 = NULL;
	EbmlElement *ElementLevel2 = NULL;
	EbmlElement *ElementLevel3 = NULL;
	EbmlElement *ElementLevel4 = NULL;

	if (m_IOCallback.seekable()) {
		MatroskaMetaSeekClusterEntry *currentCluster = FindCluster(m_CurrentTimecode);
		if (currentCluster == NULL)
			return 2;
		int64 clusterFilePos = currentCluster->filePos;

		m_IOCallback.setFilePointer(clusterFilePos);
		// Find the element data
		ElementLevel1 = m_InputStream.FindNextID(KaxCluster::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (ElementLevel1 == NULL)
			return 1;

		if (EbmlId(*ElementLevel1) == KaxCluster::ClassInfos.GlobalId) {
			KaxCluster *SegmentCluster = static_cast<KaxCluster *>(ElementLevel1);
			uint32 ClusterTimecode = 0;
			MatroskaAudioFrame *prevFrame = NULL;

			// read blocks and discard the ones we don't care about
			ElementLevel2 = m_InputStream.FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, ElementLevel1->ElementSize(), bAllowDummy);
			while (ElementLevel2 != NULL) {
				if (UpperElementLevel > 0) {
					break;
				}
				if (UpperElementLevel < 0) {
					UpperElementLevel = 0;
				}
				if (EbmlId(*ElementLevel2) == KaxClusterTimecode::ClassInfos.GlobalId) {						
					KaxClusterTimecode & ClusterTime = *static_cast<KaxClusterTimecode*>(ElementLevel2);
					ClusterTime.ReadData(m_InputStream.I_O());
					ClusterTimecode = uint32(ClusterTime);
					currentCluster->timecode = ClusterTimecode * m_TimecodeScale;
					SegmentCluster->InitTimecode(ClusterTimecode, m_TimecodeScale);
				} else  if (EbmlId(*ElementLevel2) == KaxBlockGroup::ClassInfos.GlobalId) {
					//KaxBlockGroup & aBlockGroup = *static_cast<KaxBlockGroup*>(ElementLevel2);

					// Create a new frame
					MatroskaAudioFrame *newFrame = new MatroskaAudioFrame;

					ElementLevel3 = m_InputStream.FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, ElementLevel2->ElementSize(), bAllowDummy);
					while (ElementLevel3 != NULL) {
						if (UpperElementLevel > 0) {
							break;
						}
						if (UpperElementLevel < 0) {
							UpperElementLevel = 0;
						}
						if (EbmlId(*ElementLevel3) == KaxBlock::ClassInfos.GlobalId) {								
							KaxBlock & DataBlock = *static_cast<KaxBlock*>(ElementLevel3);														
							DataBlock.ReadData(m_InputStream.I_O());		
							DataBlock.SetParent(*SegmentCluster);

							//NOTE4("Track # %u / %u frame%s / Timecode %I64d", DataBlock.TrackNum(), DataBlock.NumberFrames(), (DataBlock.NumberFrames() > 1)?"s":"", DataBlock.GlobalTimecode()/m_TimecodeScale);
							if (DataBlock.TrackNum() == m_Tracks.at(m_CurrentTrackNo).trackNumber) {											
								newFrame->timecode = DataBlock.GlobalTimecode();							

								if (DataBlock.NumberFrames() > 1) {	
									// The evil lacing has been used
									newFrame->duration = m_Tracks.at(m_CurrentTrackNo).defaultDuration * DataBlock.NumberFrames();

									newFrame->dataBuffer.resize(DataBlock.NumberFrames());
									for (uint32 f = 0; f < DataBlock.NumberFrames(); f++) {
										DataBuffer &buffer = DataBlock.GetBuffer(f);
										newFrame->dataBuffer[f].resize(buffer.Size());								
										memcpy(&newFrame->dataBuffer[f][0], buffer.Buffer(), buffer.Size());
										//newFrame->dataBufferSize = newFrame->dataBuffer.size();
									}
								} else {
									// Non-lacing block		
									newFrame->duration = m_Tracks.at(m_CurrentTrackNo).defaultDuration;

									DataBuffer &buffer = DataBlock.GetBuffer(0);
									newFrame->dataBuffer.at(0).resize(buffer.Size());								
									memcpy(&newFrame->dataBuffer[0][0], buffer.Buffer(), buffer.Size());
								}
							} else {
								//newFrame->timecode = MAX_UINT64;
							}
						/*
						} else if (EbmlId(*ElementLevel3) == KaxReferenceBlock::ClassInfos.GlobalId) {
							KaxReferenceBlock & RefTime = *static_cast<KaxReferenceBlock*>(ElementLevel3);
							RefTime.ReadData(m_InputStream.I_O());
							newFrame->frameReferences.push_back(int32(RefTime));
							//wxLogDebug("  Reference frame at scaled (%d) timecode %ld\n", int32(RefTime), int32(int64(RefTime) * TimecodeScale));
							*/
						} else if (EbmlId(*ElementLevel3) == KaxBlockDuration::ClassInfos.GlobalId) {
							KaxBlockDuration & BlockDuration = *static_cast<KaxBlockDuration*>(ElementLevel3);
							BlockDuration.ReadData(m_InputStream.I_O());
							newFrame->duration = uint32(BlockDuration);
						}
						if (UpperElementLevel > 0) {
							UpperElementLevel--;
							delete ElementLevel3;
							ElementLevel3 = ElementLevel4;
							if (UpperElementLevel > 0)
								break;
						} else {
							ElementLevel3->SkipData(m_InputStream, ElementLevel3->Generic().Context);
							delete ElementLevel3;
							ElementLevel3 = NULL;

							ElementLevel3 = m_InputStream.FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, ElementLevel2->ElementSize(), bAllowDummy);
						}							
						//newFrame = new MatroskaReadFrame();
					}
					if (newFrame->dataBuffer[0].size() > 0) {
						m_Queue.push(newFrame);
						if (prevFrame != NULL && prevFrame->duration == 0) {
							prevFrame->duration = newFrame->timecode - prevFrame->timecode;
							//if (newFrame->duration == 0)
							//	newFrame->duration = prevFrame->duration;
						}

						// !!!!!!!!!!!!!!! HACK ALERT !!!!!!!!!!!!!!!!!!!!!!!!
						// This is an ugly hack to keep us from re-seeking to the same cluster
						m_CurrentTimecode = newFrame->timecode + (newFrame->duration * 2);
						if (newFrame->duration == 0) {
							m_CurrentTimecode += m_Tracks.at(m_CurrentTrackNo).defaultDuration * 2;
						}
						// !!!!!!!!!!!!!!! HACK ALERT !!!!!!!!!!!!!!!!!!!!!!!!

						prevFrame = newFrame;
					}
				}

				if (UpperElementLevel > 0) {
					UpperElementLevel--;
					delete ElementLevel2;
					ElementLevel2 = ElementLevel3;
					if (UpperElementLevel > 0)
						break;
				} else {
					ElementLevel2->SkipData(m_InputStream, ElementLevel2->Generic().Context);
					//if (ElementLevel2 != pChecksum)
						delete ElementLevel2;								
					ElementLevel2 = NULL;

					ElementLevel2 = m_InputStream.FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, ElementLevel1->ElementSize(), bAllowDummy);
				}
			}
		}
		delete ElementLevel1;

		if (currentCluster->clusterNo < m_ClusterIndex.size()) {
			if (m_ClusterIndex[currentCluster->clusterNo+1].timecode == MAX_UINT64)
				m_ClusterIndex[currentCluster->clusterNo+1].timecode = GetClusterTimecode(m_ClusterIndex[currentCluster->clusterNo+1].filePos);
			m_CurrentTimecode = m_ClusterIndex[currentCluster->clusterNo+1].timecode;
		}	
	} else {
		// Find the element data
		ElementLevel1 = m_InputStream.FindNextID(KaxCluster::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (ElementLevel1 == NULL)
			return 1;

		if (EbmlId(*ElementLevel1) == KaxCluster::ClassInfos.GlobalId) {
			KaxCluster *SegmentCluster = static_cast<KaxCluster *>(ElementLevel1);
			uint32 ClusterTimecode = 0;

			// read blocks and discard the ones we don't care about
			ElementLevel2 = m_InputStream.FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, ElementLevel1->ElementSize(), bAllowDummy);
			while (ElementLevel2 != NULL) {
				if (UpperElementLevel > 0) {
					break;
				}
				if (UpperElementLevel < 0) {
					UpperElementLevel = 0;
				}
				if (EbmlId(*ElementLevel2) == KaxClusterTimecode::ClassInfos.GlobalId) {						
					KaxClusterTimecode & ClusterTime = *static_cast<KaxClusterTimecode*>(ElementLevel2);
					ClusterTime.ReadData(m_InputStream.I_O());
					ClusterTimecode = uint32(ClusterTime);
					SegmentCluster->InitTimecode(ClusterTimecode, m_TimecodeScale);
				} else  if (EbmlId(*ElementLevel2) == KaxBlockGroup::ClassInfos.GlobalId) {
					//KaxBlockGroup & aBlockGroup = *static_cast<KaxBlockGroup*>(ElementLevel2);

					// Create a new frame
					MatroskaAudioFrame *newFrame = new MatroskaAudioFrame;				

					ElementLevel3 = m_InputStream.FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, ElementLevel2->ElementSize(), bAllowDummy);
					while (ElementLevel3 != NULL) {
						if (UpperElementLevel > 0) {
							break;
						}
						if (UpperElementLevel < 0) {
							UpperElementLevel = 0;
						}
						if (EbmlId(*ElementLevel3) == KaxBlock::ClassInfos.GlobalId) {								
							KaxBlock & DataBlock = *static_cast<KaxBlock*>(ElementLevel3);														
							DataBlock.ReadData(m_InputStream.I_O());		
							DataBlock.SetParent(*SegmentCluster);

							//NOTE4("Track # %u / %u frame%s / Timecode %I64d", DataBlock.TrackNum(), DataBlock.NumberFrames(), (DataBlock.NumberFrames() > 1)?"s":"", DataBlock.GlobalTimecode()/m_TimecodeScale);
							if (DataBlock.TrackNum() == m_Tracks.at(m_CurrentTrackNo).trackNumber) {											
								newFrame->timecode = DataBlock.GlobalTimecode();							

								if (DataBlock.NumberFrames() > 1) {	
									// The evil lacing has been used
									newFrame->duration = m_Tracks.at(m_CurrentTrackNo).defaultDuration * DataBlock.NumberFrames();

									newFrame->dataBuffer.resize(DataBlock.NumberFrames());
									for (uint32 f = 0; f < DataBlock.NumberFrames(); f++) {
										DataBuffer &buffer = DataBlock.GetBuffer(f);
										newFrame->dataBuffer[f].resize(buffer.Size());								
										memcpy(&newFrame->dataBuffer[f][0], buffer.Buffer(), buffer.Size());
										//newFrame->dataBufferSize = newFrame->dataBuffer.size();
									}
								} else {
									// Non-lacing block		
									newFrame->duration = m_Tracks.at(m_CurrentTrackNo).defaultDuration;

									DataBuffer &buffer = DataBlock.GetBuffer(0);
									newFrame->dataBuffer.at(0).resize(buffer.Size());								
									memcpy(&newFrame->dataBuffer[0][0], buffer.Buffer(), buffer.Size());
								}
							} else {
								//newFrame->timecode = MAX_UINT64;
							}
						/*
						} else if (EbmlId(*ElementLevel3) == KaxReferenceBlock::ClassInfos.GlobalId) {
							KaxReferenceBlock & RefTime = *static_cast<KaxReferenceBlock*>(ElementLevel3);
							RefTime.ReadData(m_InputStream.I_O());
							newFrame->frameReferences.push_back(int32(RefTime));
							//wxLogDebug("  Reference frame at scaled (%d) timecode %ld\n", int32(RefTime), int32(int64(RefTime) * TimecodeScale));
							*/
						} else if (EbmlId(*ElementLevel3) == KaxBlockDuration::ClassInfos.GlobalId) {
							KaxBlockDuration & BlockDuration = *static_cast<KaxBlockDuration*>(ElementLevel3);
							BlockDuration.ReadData(m_InputStream.I_O());
							newFrame->duration = uint32(BlockDuration);
						}
						if (UpperElementLevel > 0) {
							UpperElementLevel--;
							delete ElementLevel3;
							ElementLevel3 = ElementLevel4;
							if (UpperElementLevel > 0)
								break;
						} else {
							ElementLevel3->SkipData(m_InputStream, ElementLevel3->Generic().Context);
							delete ElementLevel3;
							ElementLevel3 = NULL;

							ElementLevel3 = m_InputStream.FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, ElementLevel2->ElementSize(), bAllowDummy);
						}							
						//newFrame = new MatroskaReadFrame();
					}
					if (newFrame->dataBuffer[0].size() > 0)
						m_Queue.push(newFrame);
				}

				if (UpperElementLevel > 0) {
					UpperElementLevel--;
					delete ElementLevel2;
					ElementLevel2 = ElementLevel3;
					if (UpperElementLevel > 0)
						break;
				} else {
					ElementLevel2->SkipData(m_InputStream, ElementLevel2->Generic().Context);
					//if (ElementLevel2 != pChecksum)
						delete ElementLevel2;								
					ElementLevel2 = NULL;

					ElementLevel2 = m_InputStream.FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, ElementLevel1->ElementSize(), bAllowDummy);
				}
			}
		}
		ElementLevel1->SkipData(m_InputStream, ElementLevel1->Generic().Context);
		delete ElementLevel1;
	}
	//NOTE1("MatroskaAudioParser::FillQueue() - Queue now has %u frames", m_Queue.size());
	return 0;
};

uint64 MatroskaAudioParser::GetClusterTimecode(uint64 filePos) {	
	try {
		uint64 ret = MAX_UINT64;

		int UpperElementLevel = 0;
		// Elements for different levels
		EbmlElement *ElementLevel1 = NULL;
		EbmlElement *ElementLevel2 = NULL;		
		EbmlElement *ElementLevel3 = NULL;		

		m_IOCallback.setFilePointer(filePos);
		// Find the element data
		ElementLevel1 = m_InputStream.FindNextID(KaxCluster::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (ElementLevel1 == NULL)
			return MAX_UINT64;

		if (EbmlId(*ElementLevel1) == KaxCluster::ClassInfos.GlobalId) {
			//KaxCluster *SegmentCluster = static_cast<KaxCluster *>(ElementLevel1);
			//uint32 ClusterTimecode = 0;

			// read blocks and discard the ones we don't care about
			ElementLevel2 = m_InputStream.FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, ElementLevel1->ElementSize(), false);
			while (ElementLevel2 != NULL) {
				if (UpperElementLevel > 0) {
					break;
				}
				if (UpperElementLevel < 0) {
					UpperElementLevel = 0;
				}
				if (EbmlId(*ElementLevel2) == KaxClusterTimecode::ClassInfos.GlobalId) {						
					KaxClusterTimecode & ClusterTime = *static_cast<KaxClusterTimecode*>(ElementLevel2);
					ClusterTime.ReadData(m_InputStream.I_O());
					ret = uint64(ClusterTime) * m_TimecodeScale;
					
				}

				if (UpperElementLevel > 0) {
					UpperElementLevel--;
					delete ElementLevel2;
					ElementLevel2 = ElementLevel3;
					if (UpperElementLevel > 0)
						break;
				} else {
					ElementLevel2->SkipData(m_InputStream, ElementLevel2->Generic().Context);
					//if (ElementLevel2 != pChecksum)
					delete ElementLevel2;								
					ElementLevel2 = NULL;
					if (ret == -1)
						ElementLevel2 = m_InputStream.FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, ElementLevel1->ElementSize(), false);
				}
			}
		}
		delete ElementLevel1;

		return ret;
	} catch (...) {
		return MAX_UINT64;
	}	
};

MatroskaMetaSeekClusterEntry *MatroskaAudioParser::FindCluster(uint64 timecode)
{
	try {
		#ifdef _DEBUG_NO_SEEKING
		static size_t callCount = 0;
		if (callCount < m_ClusterIndex.size())
			return &m_ClusterIndex[callCount++];
		else return NULL;
		#endif

		if (timecode == 0)
			// Special case
			return &m_ClusterIndex.at(0);
		
		MatroskaMetaSeekClusterEntry *correctEntry = NULL;
		double clusterDuration = (double)m_ClusterIndex.size() / (double)m_Duration;
		size_t clusterIndex = clusterDuration * (double)timecode;
		//int lookCount = 0;
		if (clusterIndex > m_ClusterIndex.size())
			clusterIndex = m_ClusterIndex.size()-1;

		MatroskaMetaSeekClusterEntry *clusterEntry = NULL;
		MatroskaMetaSeekClusterEntry *prevClusterEntry = NULL;
		MatroskaMetaSeekClusterEntry *nextClusterEntry = NULL;
		while (correctEntry == NULL) {
			clusterEntry = &m_ClusterIndex.at(clusterIndex);		
			if (clusterIndex > 0)
				prevClusterEntry = &m_ClusterIndex.at(clusterIndex-1);
			if (clusterIndex+1 < m_ClusterIndex.size())
				nextClusterEntry = &m_ClusterIndex.at(clusterIndex+1);			
			
			// We need timecodes to do good seeking
			if (clusterEntry->timecode == MAX_UINT64) {				
				clusterEntry->timecode = GetClusterTimecode(clusterEntry->filePos);			
			}
			if (prevClusterEntry != NULL && prevClusterEntry->timecode == MAX_UINT64) {
				prevClusterEntry->timecode = GetClusterTimecode(prevClusterEntry->filePos);			
			}
			if (nextClusterEntry != NULL && nextClusterEntry->timecode == MAX_UINT64) {				
				nextClusterEntry->timecode = GetClusterTimecode(nextClusterEntry->filePos);			
			}

			if (clusterEntry->timecode == timecode) {
				// WOW, we are seeking directly to this cluster
				correctEntry = clusterEntry;
				break;
			}

			if (prevClusterEntry != NULL) {
				if (clusterEntry->timecode > timecode && timecode > prevClusterEntry->timecode) {
					// We found it !!!
					correctEntry = prevClusterEntry;
					break;
				}
				if (prevClusterEntry->timecode == timecode) {
					// WOW, we are seeking directly to this cluster
					correctEntry = prevClusterEntry;
					break;
				}
				// Check if we overshot the needed cluster
				if (timecode < prevClusterEntry->timecode) {
					clusterIndex--;
					//lookCount++; // This is how many times we have 'looked'
					continue;
				}
			}
			
			if (nextClusterEntry != NULL) {
				if (clusterEntry->timecode < timecode && timecode < nextClusterEntry->timecode) {
					// We found it !!!
					correctEntry = clusterEntry;
					break;
				}			
				if (nextClusterEntry->timecode == timecode) {
					// WOW, we are seeking directly to this cluster
					correctEntry = nextClusterEntry;
					break;
				}
				// Check if we undershot the needed cluster
				if (timecode > nextClusterEntry->timecode) {
					clusterIndex++;
					//lookCount--;
					continue;
				}
			}
			// We should never get here, unless this is the last cluster
			assert(clusterEntry != NULL);	
			if (timecode <= m_Duration)
				correctEntry = clusterEntry;
			else
				break;
		}

		if (correctEntry != NULL)
			NOTE3("MatroskaAudioParser::FindCluster(timecode = %u) seeking to cluster %i at %u", (uint32)(timecode / m_TimecodeScale), (uint32)correctEntry->clusterNo, (uint32)correctEntry->filePos);
		else
			NOTE1("MatroskaAudioParser::FindCluster(timecode = %u) seeking failed", (uint32)(timecode / m_TimecodeScale));

		return correctEntry;
	} catch (...) {
		return NULL;
	}
}

void MatroskaAudioParser::CountClusters() 
{
	for (uint32 c = 0; c < m_ClusterIndex.size(); c++) {
		MatroskaMetaSeekClusterEntry &clusterEntry = m_ClusterIndex.at(c);		
		clusterEntry.clusterNo = c;
	}
}

void MatroskaAudioParser::FixChapterEndTimes()
{
	if (m_Chapters.size() > 0) {
		MatroskaChapterInfo *nextChapter = &m_Chapters.at(m_Chapters.size()-1);
		if (nextChapter->timeEnd == 0) {
			nextChapter->timeEnd = m_Duration;
		}
		for (uint32 c = 0; c < m_Chapters.size()-1; c++) {
			MatroskaChapterInfo &currentChapter = m_Chapters.at(c);	
			nextChapter = &m_Chapters.at(c+1);
			if (currentChapter.timeEnd == 0) {
				currentChapter.timeEnd = nextChapter->timeStart;
			}
		}
		nextChapter = &m_Chapters.at(m_Chapters.size()-1);
		if ((nextChapter->timeEnd == 0) || (nextChapter->timeEnd == nextChapter->timeStart)) {
			nextChapter->timeEnd = m_Duration;
		}
	}	
}

bool MatroskaAudioParser::FindChapterUID(uint64 uid)
{
	for (uint32 c = 0; c < m_Chapters.size(); c++) {
		MatroskaChapterInfo &currentChapter = m_Chapters.at(c);	
		if (currentChapter.chapterUID == uid)
			return true;
	}	
	return false;
}

void PrintChapters(std::vector<MatroskaChapterInfo> &theChapters) 
{
	for (uint32 c = 0; c < theChapters.size(); c++) {
		MatroskaChapterInfo &currentChapter = theChapters.at(c);	
		NOTE2("Chapter %u, UID: %u", c, (uint32)currentChapter.chapterUID);
		NOTE1("\tStart Time: %u", (uint32)currentChapter.timeStart);
		NOTE1("\tEnd Time: %u", (uint32)currentChapter.timeEnd);
		for (uint32 d = 0; d < currentChapter.display.size(); d++) {
			NOTE3("\tDisplay %u, String: %s Lang: %s", d, currentChapter.display.at(d).string.GetUTF8().c_str(), currentChapter.display.at(d).lang.c_str());
		}
		for (uint32 t = 0; t < currentChapter.tracks.size(); t++) {
			NOTE2("\tTrack %u, UID: %%u", t, (uint32)currentChapter.tracks.at(t));
		}

	}
};

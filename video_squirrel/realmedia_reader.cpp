/*
 *  Video Squirrel, a cross-platform movie cataloger
 *
 *  realmedia_reader.cpp
 *
 *  Copyright (C) Jory Stone - June 2003
 *  Copyright (C) Gabest - ?2003
 *
 *  Video Squirrel is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  Video Squirrel is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Video Squirrel; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*!
	\file realmedia_reader.cpp
	\version \$Id$
	\brief a cross-platform movie cataloger
	\author Jory Stone            <jcsston @ toughguy.net>
	\author Gabest
*/
#pragma pack(1)

#include "realmedia_reader.h"

/// Consructor, Sets the default values
RealMedia_Reader::RealMedia_Reader()
{
	media_properties_block_count = 0;
	content_description_block_count = 0;

	for (int i = 0; i < 255; i++)
	{
		media_properties_block[i] = NULL;
		content_description_block[i] = NULL;
	}
	packet_array = NULL;
	packet_count = 0;
	current_packet_no = 0;
	packet_start_pos = 0;
	last_packet_start_pos = 0;
};

/// De-consructor, Frees the memory
RealMedia_Reader::~RealMedia_Reader()
{
	int i = 0;
	while (media_properties_block[i] != NULL)
	{
		delete media_properties_block[i]->type_specific_data;
		delete media_properties_block[i]->stream_name;
		delete media_properties_block[i]->mime_type;
		delete media_properties_block[i];
		i++;
	}
	i = 0;
	while (content_description_block[i] != NULL)
	{
		delete content_description_block[i]->author;
		delete content_description_block[i]->comment;
		delete content_description_block[i]->copyright;
		delete content_description_block[i]->title;
		delete content_description_block[i];
		i++;
	}
	fclose(real_media);
};

int RealMedia_Reader::Read(const char *filename, bool bPreReadDataPackets)
{
	if (filename != NULL)
	{
		the_filename = new char[strlen(filename)+1];
		strcpy(the_filename, filename);

		int error = 0;
		real_media = fopen(filename, "rb");

		bool keep_looping = true;
		while (keep_looping)
		{
			//First we store the current file position
			unsigned int file_pos = ftell(real_media);
			//Read the header
			//First we read the block id
			char block_id[5];
			memset(&block_id, 0, sizeof(block_id));
			error = fread((void *)&block_id, 4, 1, real_media);
			//Add the NULL char
			block_id[4] = 0;
			//Read the block size
			UINT32 block_size = 0;
			error = fread((void *)&block_size, 4, 1, real_media);
			bswap((BYTE *)&block_size, 4);
			//Now the block version
			UINT16 block_version = 0;
			error = fread((void *)&block_version, 2, 1, real_media);
			bswap((BYTE *)&block_version, 2);

			//Now we find out what block type this is
			if (!strcmpi(block_id, ".RMF"))
			{
				//This is the main header chunk
				RealMedia_File_Header *main_header = new RealMedia_File_Header;
				memset(main_header, 0, sizeof(RealMedia_File_Header));

				if (block_version == 0)
				{
					error = fread((void *)&main_header->file_version, 4, 1, real_media);
					bswap((BYTE *)&main_header->file_version, 4);

					error = fread((void *)&main_header->num_headers, 4, 1, real_media);
					bswap((BYTE *)&main_header->num_headers, 4);
				}
			}
			else if (!strcmpi(block_id, "PROP"))
			{
				//The Properties Header
				RealMedia_Properties *info = &properties_block;
				memset(info, 0, sizeof(RealMedia_Properties));
				if (block_version == 0)
				{
					error = fread((void *)&info->max_bit_rate, 4, 1, real_media);
					bswap((BYTE *)&info->max_bit_rate, 4);

					error = fread((void *)&info->avg_bit_rate, 4, 1, real_media);
					bswap((BYTE *)&info->avg_bit_rate, 4);

					error = fread((void *)&info->max_packet_size, 4, 1, real_media);
					bswap((BYTE *)&info->max_packet_size, 4);

					error = fread((void *)&info->avg_packet_size, 4, 1, real_media);
					bswap((BYTE *)&info->avg_packet_size, 4);

					error = fread((void *)&info->num_packets, 4, 1, real_media);
					bswap((BYTE *)&info->num_packets, 4);

					error = fread((void *)&info->duration, 4, 1, real_media);
					bswap((BYTE *)&info->duration, 4);

					error = fread((void *)&info->preroll, 4, 1, real_media);
					bswap((BYTE *)&info->preroll, 4);

					error = fread((void *)&info->index_offset, 4, 1, real_media);
					bswap((BYTE *)&info->index_offset, 4);

					error = fread((void *)&info->data_offset, 4, 1, real_media);
					bswap((BYTE *)&info->data_offset, 4);

					error = fread((void *)&info->num_streams, 2, 1, real_media);
					bswap((BYTE *)&info->num_streams, 2);

					error = fread((void *)&info->flags, 2, 1, real_media);
					bswap((BYTE *)&info->flags, 2);

					//printf("Properties Header\nMax Bitrate: %i\tAvg Bitrate: %i\nMax Packet Size: %i\tAvg Packet Size: %i", info->max_bit_rate, info->avg_bit_rate, info->max_packet_size, info->avg_packet_size);
				}
				else
				{
					delete info;
					info = NULL;
				}

			}
			else if (!strcmpi(block_id, "CONT"))
			{
				//The Content Description Header
				RealMedia_Content_Description *content_desc = new RealMedia_Content_Description;
				memset(content_desc, 0, sizeof(RealMedia_Content_Description));
				if (block_version == 0)
				{
					//Read Title text
					error = fread((void *)&content_desc->title_len, 2, 1, real_media);
					bswap((BYTE *)&content_desc->title_len, 2);

					content_desc->title = new char[content_desc->title_len+1];
					error = fread((void *)content_desc->title, content_desc->title_len, 1, real_media);
					content_desc->title[content_desc->title_len] = 0;

					//Read Author text
					error = fread((void *)&content_desc->author_len, 2, 1, real_media);
					bswap((BYTE *)&content_desc->author_len, 2);

					content_desc->author = new char[content_desc->author_len+1];
					error = fread((void *)content_desc->author, content_desc->author_len, 1, real_media);
					content_desc->author[content_desc->author_len] = 0;

					//Read Copyright text
					error = fread((void *)&content_desc->copyright_len, 2, 1, real_media);
					bswap((BYTE *)&content_desc->copyright_len, 2);

					content_desc->copyright = new char[content_desc->copyright_len+1];
					error = fread((void *)content_desc->copyright, content_desc->copyright_len, 1, real_media);
					content_desc->copyright[content_desc->copyright_len] = 0;

					//Read Comment text
					error = fread((void *)&content_desc->comment_len, 2, 1, real_media);
					bswap((BYTE *)&content_desc->comment_len, 2);

					content_desc->comment = new char[content_desc->comment_len+1];
					error = fread((void *)content_desc->comment, content_desc->comment_len, 1, real_media);
					content_desc->comment[content_desc->comment_len] = 0;

					//Add to the array
					content_description_block[content_description_block_count] = content_desc;
					content_description_block_count++;
				}
				else
				{
					delete content_desc;
					content_desc = NULL;
				}
			}
			else if (!strcmpi(block_id, "MDPR"))
			{
				RealMedia_Media_Properties *media_info = new RealMedia_Media_Properties;
				memset(media_info, 0, sizeof(RealMedia_Media_Properties));

				if (block_version == 0)
				{
					error = fread((void *)&media_info->stream_number, 2, 1, real_media);
					bswap((BYTE *)&media_info->stream_number, 2);

					error = fread((void *)&media_info->max_bit_rate, 4, 1, real_media);
					bswap((BYTE *)&media_info->max_bit_rate, 4);

					error = fread((void *)&media_info->avg_bit_rate, 4, 1, real_media);
					bswap((BYTE *)&media_info->avg_bit_rate, 4);

					error = fread((void *)&media_info->max_packet_size, 4, 1, real_media);
					bswap((BYTE *)&media_info->max_packet_size, 4);

					error = fread((void *)&media_info->avg_packet_size, 4, 1, real_media);
					bswap((BYTE *)&media_info->avg_packet_size, 4);

					error = fread((void *)&media_info->start_time, 4, 1, real_media);
					bswap((BYTE *)&media_info->start_time, 4);

					error = fread((void *)&media_info->preroll, 4, 1, real_media);
					bswap((BYTE *)&media_info->preroll, 4);

					error = fread((void *)&media_info->duration, 4, 1, real_media);
					bswap((BYTE *)&media_info->duration, 4);

					//Read Stream Name text
					error = fread((void *)&media_info->stream_name_size, 1, 1, real_media);

					media_info->stream_name = new char[media_info->stream_name_size+1];
					error = fread((void *)media_info->stream_name, media_info->stream_name_size, 1, real_media);
					media_info->stream_name[media_info->stream_name_size] = 0;

					//Read Mime Type text
					error = fread((void *)&media_info->mime_type_size, 1, 1, real_media);

					media_info->mime_type = new char[media_info->mime_type_size+1];
					error = fread((void *)media_info->mime_type, media_info->mime_type_size, 1, real_media);
					media_info->mime_type[media_info->mime_type_size] = 0;

					//Read Type Specific text
					error = ftell(real_media);
					ReadData((void *)&media_info->type_specific_len, 4, true);

					media_info->type_specific_data = new UINT8[media_info->type_specific_len+1];
					error = fread((void *)media_info->type_specific_data, media_info->type_specific_len, 1, real_media);
					media_info->type_specific_data[media_info->type_specific_len] = 0;

					//Now depending on the mime type we can read more infomation about this track
					//Thanks to Dark-Cracker for help
					if (!stricmp(media_info->mime_type, "video/x-pn-realvideo"))
					{		
						media_info->video_header = (RealMedia_VideoHeader *)media_info->type_specific_data;
						bswap((BYTE *)&media_info->video_header->dwSize, 4);
						bswap((BYTE *)&media_info->video_header->w, 2);
						bswap((BYTE *)&media_info->video_header->h, 2);
						bswap((BYTE *)&media_info->video_header->bpp, 2);
						bswap((BYTE *)&media_info->video_header->unk1, 4);
						bswap((BYTE *)&media_info->video_header->unk2, 4);
						bswap((BYTE *)&media_info->video_header->type1, 4);
						bswap((BYTE *)&media_info->video_header->type2, 4);

						media_info->frame_rate = (float)media_info->video_header->unk2 / 0x10000;//*(float *)(((UINT8 *)&media_info->video_header->unk2 - 1));
						//media_info->frame_rate = media_info->video_header->unk1 + ((float)(*((UINT8 *)&media_info->video_header->unk2 + 3)) / 256);
						/*error = ftell(real_media);
						fseek(real_media, 2, SEEK_CUR);
						media_info->extra_videocodec_data_size = 34;
						media_info->extra_videocodec_data = new UINT8[media_info->extra_videocodec_data_size];						
						ReadData(media_info->extra_videocodec_data, media_info->extra_videocodec_data_size, false);

						fseek(real_media, error, SEEK_SET);
						fseek(real_media, 6, SEEK_CUR);
						media_info->codec_name = new char[9+1];
						error = fread((void *)media_info->codec_name, 9, 1, real_media);
						media_info->codec_name[9] = 0;

						error = fread((void *)&media_info->frame_width, 2, 1, real_media);
						bswap((BYTE *)&media_info->frame_width, 2);

						error = fread((void *)&media_info->frame_height, 2, 1, real_media);
						bswap((BYTE *)&media_info->frame_height, 2);
						
						fseek(real_media, 6, SEEK_CUR);
						UINT16 FpsRM_Part1 = 0;
						UINT8 FpsRM_Part2 = 0;

						error = fread((void *)&FpsRM_Part1, 2, 1, real_media);
						bswap((BYTE *)&FpsRM_Part1, 2);						
						
						error = fread((void *)&FpsRM_Part2, 1, 1, real_media);

						media_info->frame_rate = FpsRM_Part1 + ((float)FpsRM_Part2 / 256);*/

					}
					else if (!stricmp(media_info->mime_type, "audio/x-pn-realaudio"))
					{
						media_info->audio_header = (RealMedia_AudioHeader *)media_info->type_specific_data;
						int size = sizeof(RealMedia_AudioHeader);
						size++;
						bswap((BYTE *)&media_info->audio_header->codecdata_length, 4);
						bswap((BYTE *)&media_info->audio_header->channels, 2);
						bswap((BYTE *)&media_info->audio_header->sample_rate, 2);
						/*error = ftell(real_media);
						UINT16 version = 0;
						UINT16 flavor = 0;
						UINT16 sub_packet_h = 0;
						UINT16 frame_size = 0;
						UINT16 sub_packet_size = 0;

						UINT32 header_size = 0;
						UINT32 coded_frame_size = 0;
						UINT32 codecdata_length = 0;
						UINT8 *dummy_buffer = new UINT8[4];

						//Seek past the unknown stuff
						fseek(real_media, 7, SEEK_CUR);

						//Read in the version header???
						ReadData((void *)&version, 2, true);
						// 00 00
						fread((void *)dummy_buffer, 2, 1, real_media); 
						
						// .ra4 or .ra5
						fread((void *)dummy_buffer, 4, 1, real_media); 
						// ???
						fread((void *)dummy_buffer, 4, 1, real_media);						
						// version (4 or 5)
						ReadData((void *)&version, 2, true);
						// header size == 0x4E 
						ReadData((void *)&header_size, 4, true);
						
						//Read in the codec_audience_number
						// codec flavor id, Also called the codec_audience_number
						ReadData((void *)&flavor, 2, true);

						// coded frame size, needed by codec
						ReadData((void *)&coded_frame_size, 4, true);

						fread((void *)dummy_buffer, 4, 1, real_media); // big number
						fread((void *)dummy_buffer, 4, 1, real_media); // bigger number
						fread((void *)dummy_buffer, 4, 1, real_media); // 2 || -''-
						//		stream_skip(demuxer->stream, 2); // 0x10
						ReadData((void *)&sub_packet_h, 2, true);
						// coded frame size
						ReadData((void *)&frame_size, 2, true);
						ReadData((void *)&sub_packet_size, 2, true);
						//Skip past the next 2 btyes of 0's
						fread((void *)dummy_buffer, 2, 1, real_media);  // 0

						if (version == 5)
							fseek(real_media, 6, SEEK_CUR); //0,srate,0
												
						//Good, now we read the sample rate info
						error = ftell(real_media);

						ReadData((void *)&media_info->frequency, 2, true);

						//Skip some 0's
						fseek(real_media, 2, SEEK_CUR);
						
						ReadData((void *)&media_info->samplesize, 2, true);

						ReadData((void *)&media_info->channels, 2, true);

						if (version == 5)
						{
							//Seek to the codec name pos
							fseek(real_media, 4, SEEK_CUR);

							media_info->codec_name = new char[4+1];
							error = fread((void *)media_info->codec_name, 4, 1, real_media);
							media_info->codec_name[4] = 0;
						}
						else
						{		
							//I don't know what to do with this code
							// Desc #1
							//skip_str(1, demuxer);
							// Desc #2
							//get_str(1, demuxer, buf, sizeof(buf));
						}

						//Emulate WAVEFORMATEX struct:
						WAVEFORMATEX_real *wf;
						wf = (WAVEFORMATEX_real *)malloc(sizeof(WAVEFORMATEX_real));
						memset(wf, 0, sizeof(WAVEFORMATEX_real));
						wf->nChannels = media_info->channels;
						wf->wBitsPerSample = media_info->samplesize*8;
						wf->nSamplesPerSec = media_info->frequency;
						//wf->nAvgBytesPerSec = bitrate;
						wf->nBlockAlign = frame_size;
						wf->cbSize = 0;
						media_info->wav_data = wf;
						//sh->format = MKTAG(buf[0], buf[1], buf[2], buf[3]);
						
						//Find out what format the audio is
						if (!stricmp(media_info->codec_name,"atrc")) {
							//"Audio: Sony ATRAC3 (RealAudio 8) (unsupported)"
							//sh->format = 0x270;
							// 14 bytes extra header needed !
							wf->cbSize = 14;
							wf = (WAVEFORMATEX_real *)realloc(wf, sizeof(WAVEFORMATEX_real) + wf->cbSize);
							wf->nAvgBytesPerSec = 16537; // 8268
							wf->nBlockAlign = 384; // 192
							wf->wBitsPerSample = 0; // from AVI created by VirtualDub

						}else	if (!stricmp(media_info->codec_name, "cook")) {
							//"Audio: Real's GeneralCooker (?) (RealAudio G2?) (unsupported)"
							// realaudio codec plugins - common:
							//			    sh->wf->cbSize = 4+2+24;
							fseek(real_media, 3, SEEK_CUR);  // Skip 3 unknown bytes 
							if (version==5)
								fseek(real_media, 1, SEEK_CUR);  // Skip 1 additional unknown byte 

							ReadData(&codecdata_length ,4, true);
							
							wf->cbSize = 10 + codecdata_length;
							wf = (WAVEFORMATEX_real *)realloc(wf, sizeof(WAVEFORMATEX_real) + wf->cbSize);
							( (short*) (wf+1) )[0] = sub_packet_size;
							( (short*) (wf+1) )[1] = sub_packet_h;
							( (short*) (wf+1) )[2] = flavor;
							( (short*) (wf+1) )[3] = coded_frame_size;
							( (short*) (wf+1) )[4] = codecdata_length;
							//			    stream_read(demuxer->stream, ((char*)(sh->wf+1))+6, 24); // extras
							// extras
							ReadData(((char*)(wf+1))+10, codecdata_length, true);

						}else if (!stricmp(media_info->codec_name, "dnet")) {
							//"Audio: DNET (AC3 with low-bitrate extension)"
							
							//sh->format = 0x2000;							
						}else	if (!stricmp(media_info->codec_name, "sipr")) {
							//"Audio: SiproLab's ACELP.net"
							
							//sh->format = 0x130;
							//for buggy directshow loader
							wf->cbSize = 4;
							wf = (WAVEFORMATEX_real *)realloc(wf, sizeof(WAVEFORMATEX_real) + wf->cbSize);
							wf->wBitsPerSample = 0;
							wf->nAvgBytesPerSec = 1055;
							wf->nBlockAlign = 19;
							//	wf->nBlockAlign = frame_size / 288;
							dummy_buffer[0] = 30;
							dummy_buffer[1] = 1;
							dummy_buffer[2] = 1;
							dummy_buffer[3] = 0;
							memcpy((wf + 18), (char *)&dummy_buffer[0], 4);
						}*/
					}
					else if (!stricmp(media_info->mime_type, "logical-fileinfo"))
					{
						error = ftell(real_media);
						//Seek to the audience number pos
						/*fseek(real_media, 11, SEEK_CUR);
						
						//Read in the codec_audience_number
						UINT32 field_count = 0;
						ReadData((void *)&field_count, 4, true);

						for (int current_field = 0; current_field < field_count; current_field++)
						{
							UINT32 test_len = 0;
							UINT16 field_title_len = 0;
							UINT16 field_text_len = 0;
							
							RealMedia_Infomation_Field *new_item = new RealMedia_Infomation_Field();

							//Read the total length of this field and text
							ReadData((void *)&test_len, 4, true);
														
							//Seek ahead one byte, for some reason
							fseek(real_media, 1, SEEK_CUR);
							//Read the length of the field name
							ReadData((void *)&field_title_len, 2, true);
							//Read in field name
							new_item->field_title = new char[field_title_len+1];
							error = fread((void *)new_item->field_title, field_title_len, 1, real_media);
							new_item->field_title[field_title_len] = 0;														

							error = ftell(real_media);
							//Seek ahead one byte, for some reason
							fseek(real_media, 4, SEEK_CUR);
							//Read the length of the field text
							ReadData((void *)&field_text_len, 2, true);
							//Read in the field text
							new_item->field_text = new char[field_text_len+1];
							error = fread((void *)new_item->field_text, field_text_len, 1, real_media);
							new_item->field_text[field_text_len] = 0;

							field_list.AddItem(new_item);
						}*/

					}

					//Add to the array
					media_properties_block[media_properties_block_count] = media_info;
					media_properties_block_count++;

				}
				else
				{
					delete media_info;
					media_info = NULL;
				}
			}
			else if (!strcmpi(block_id, "DATA"))
			{
				//printf("\nReading Data chunk\n");
				RealMedia_Data_Chunk_Header data_chunk;
				data_chunk.size = block_size;

				ReadData((void *)&data_chunk.num_packets, 4, true);
								
				ReadData((void *)&data_chunk.next_data_header, 4, true);

				ReadData((void *)&data_chunk.object_version, 2, true);

				//Store the start of the packets
				packet_start_pos = ftell(real_media);
				//Since we haven't read any packets yet, we are at the beginning
				last_packet_start_pos = packet_start_pos;
				if (bPreReadDataPackets)
				{
					this->packet_array = new RealMedia_Packet[data_chunk.num_packets];
					//Now we read all the packets
					for (int current_packet_no = 0; current_packet_no < data_chunk.num_packets; current_packet_no++)
					{
						RealMedia_Media_Packet_Header *new_packet = new RealMedia_Media_Packet_Header();
						memset(new_packet, 0, sizeof(*new_packet));
						
						long packet_start_pos = ftell(real_media);
						
						ReadData((void *)&new_packet->length, 2, true);

						ReadData((void *)&new_packet->stream_number, 2, true);

						ReadData((void *)&new_packet->timestamp, 4, true);

						ReadData((void *)&new_packet->reserved, 1, true);

						ReadData((void *)&new_packet->flags, 1, true);
												
						long data_length = new_packet->length - 10;
						long data_start = ftell(real_media);
						
						fseek(real_media, packet_start_pos + new_packet->length, SEEK_SET);				
						
						RealMedia_Packet min_packet_data;
						memset(&min_packet_data, 0, sizeof(min_packet_data));
						min_packet_data.stream_number = new_packet->stream_number;
						min_packet_data.timestamp = new_packet->timestamp;
						min_packet_data.reserved = new_packet->reserved;
						min_packet_data.flags = new_packet->flags;
						min_packet_data.packet_start_pos = data_start;
						min_packet_data.packet_data_length = data_length;

						//packets.AddItem(min_packet_data, false);
						this->packet_array[packet_count] = min_packet_data;						
						packet_count++;

						delete new_packet;
					}
				}else {
					//If we don't read all the packets at first we have to asume
					//that data_chunk.num_packets contains the correct count
					packet_count = data_chunk.num_packets;
				}
			}
			error = ftell(real_media);
			//Now we seek to the end of the block, (just in case we didn't)
			fseek(real_media, file_pos+block_size, SEEK_SET);
			//If we are seeking to the same point it's useless
			if (file_pos == ftell(real_media))
				keep_looping = false;
		}		
		return 0;
	}
	return 1;
};

RealMedia_Packet *RealMedia_Reader::GetPacketData(RealMedia_Packet *packet_info)
{
	if (the_filename == NULL || real_media == NULL || packet_info == NULL)
		return NULL;


	packet_info->data = new BYTE[packet_info->packet_data_length];

	fseek(real_media, packet_info->packet_start_pos, SEEK_SET);
	fread(packet_info->data, packet_info->packet_data_length, 1, real_media);

	return packet_info;
};

RealMedia_Packet *RealMedia_Reader::GetNextDataPacket()
{
	if (the_filename == NULL || real_media == NULL)
		return NULL;

	if (current_packet_no > packet_count)
	{
		//Reset the packet reading
		//current_packet_no = 0;
		//last_packet_start_pos = packet_start_pos;
		//return NULL so we know we reached the end
		return NULL;
	}

	int error = 0;
	//Now we read the next packet
	fseek(real_media, last_packet_start_pos, SEEK_SET);				

	RealMedia_Packet *new_packet = new RealMedia_Packet();
	memset((void *)new_packet, 0, sizeof(*new_packet));
		
	ReadData((void *)&new_packet->packet_data_length, 2, true);	
	new_packet->packet_data_length = new_packet->packet_data_length - 10;

	ReadData((void *)&new_packet->stream_number, 2, true);

	ReadData((void *)&new_packet->timestamp, 4, true);

	ReadData((void *)&new_packet->reserved, 1, true);

	ReadData((void *)&new_packet->flags, 1, true);		

	new_packet->data = new UINT8[new_packet->packet_data_length];
	error = fread((void *)new_packet->data, new_packet->packet_data_length, 1, real_media);
		
	last_packet_start_pos = ftell(real_media);

	return new_packet;
};

UINT32 RealMedia_Reader::GetLongestTrackLength()
{
	UINT32 longest_length = 0;
	for (int t = 0; t < media_properties_block_count; t++)
	{
		if (media_properties_block[t]->duration > longest_length)
			longest_length = media_properties_block[t]->duration;
	}
	return longest_length;
};

int RealMedia_Reader::ReadData(void *store_here, UINT32 store_length, bool bByteSwap)
{
	if ((store_here == NULL) || (real_media == NULL))
		return 0;

	int error = 0;
	
	error = fread(store_here, store_length, 1, real_media);
	
	if (bByteSwap)
		bswap((BYTE *)store_here, store_length);

	return error;
};

void inline RealMedia_Reader::bswap(BYTE* s, int len)
{
	for(BYTE* d = s + len-1; s < d; s++, d--)
		*s ^= *d, *d ^= *s, *s ^= *d;
};

/***************************************************
 *   Begin RealMedia_Infomation_Field_List class   *
 ***************************************************/
RealMedia_Infomation_Field_List::RealMedia_Infomation_Field_List() {
	this->first_item = NULL;
};

RealMedia_Infomation_Field_List::~RealMedia_Infomation_Field_List() {
	//I should free memory here
	RealMedia_Infomation_Field *temp = NULL;

	temp = first_item;
	while (temp != NULL)
	{		
		if (first_item != NULL)
		{
			first_item = first_item->next_item;
		}
		delete temp;
		temp = first_item;
	}
};

//Add a Attachment item to the list
int RealMedia_Infomation_Field_List::AddItem(RealMedia_Infomation_Field *new_item, bool bCheckIfExisting) {
	//Check that some idiot didn't pass a NULL pointer
	if (new_item == NULL)
	{
		//So some idiot tried to pass a NULL pointer :P
		return 1;
	}
	if (bCheckIfExisting)
	{
		//Check if this item is already in the list
		RealMedia_Infomation_Field *current = first_item;
		while (current != NULL)
		{
			if (current == new_item)
			{
				//Item already in List !!!!!
				return 1;
			}
			//Not found so far so lets keep going
			current = current->next_item;
		}
	}
	//It passed the checks so lets add it
	new_item->next_item = first_item;
	first_item = new_item;
	return 0;
};

//returns the number of fields in this list
int RealMedia_Infomation_Field_List::GetListCount() {
	RealMedia_Infomation_Field *current = first_item;
	int item_count = 0;
	while (current != NULL)
	{
		item_count++;
		//Keep going
		current = current->next_item;
	}
	return item_count;
};

RealMedia_Infomation_Field *RealMedia_Infomation_Field_List::operator[] (int requested_index)
{
	RealMedia_Infomation_Field *current = first_item;
	for (int item_count = (this->GetListCount()); item_count > 0; item_count--)
	{
		if (requested_index == item_count)
			return current;
		//Keep searching
		current = current->next_item;
	}	
	return NULL;	
};

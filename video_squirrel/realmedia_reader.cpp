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

int RealMedia_Reader::Read(const char *filename)
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
				RealMedia_Properties *info = &properties_block;//new RealMedia_Properties;
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

					printf("Properties Header\nMax Bitrate: %i\tAvg Bitrate: %i\nMax Packet Size: %i\tAvg Packet Size: %i", info->max_bit_rate, info->avg_bit_rate, info->max_packet_size, info->avg_packet_size);
					//Add to the array
					//properties_block[properties_block_count] = info;
					//properties_block_count++;
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
					error = fread((void *)&media_info->type_specific_len, 1, 1, real_media);

					media_info->type_specific_data = new char[media_info->type_specific_len+1];
					error = fread((void *)media_info->type_specific_data, media_info->type_specific_len, 1, real_media);
					media_info->type_specific_data[media_info->type_specific_len] = 0;

					//Now depending on the mime type we can read more infomation about this track
					//Thanks to Dark-Cracker for help
					if (!stricmp(media_info->mime_type, "video/x-pn-realvideo"))
					{
						media_info->codec_name = new char[8+1];
						fseek(real_media, 6, SEEK_CUR);
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

						media_info->frame_rate = FpsRM_Part1 + ((float)FpsRM_Part2 / 256);

					}
					else if (!stricmp(media_info->mime_type, "audio/x-pn-realaudio"))
					{
						//Seek to the audience number pos
						fseek(real_media, 26, SEEK_CUR);
						
						//Read in the codec_audience_number
						error = fread((void *)&media_info->codec_audience_number, 1, 1, real_media);
						bswap((BYTE *)&media_info->codec_audience_number, 1);

						//Seek to the frequency pos
						fseek(real_media, 26, SEEK_CUR);
												
						error = fread((void *)&media_info->frequency, 2, 1, real_media);
						bswap((BYTE *)&media_info->frequency, 2);

						//Seek to the channels pos
						fseek(real_media, 9, SEEK_CUR);
						
						error = fread((void *)&media_info->channels, 1, 1, real_media);
						bswap((BYTE *)&media_info->channels, 1);
						
						//Seek to the codec name pos
						fseek(real_media, 4, SEEK_CUR);

						media_info->codec_name = new char[4+1];
						error = fread((void *)media_info->codec_name, 4, 1, real_media);
						media_info->codec_name[4] = 0;

					}
					else if (!stricmp(media_info->mime_type, "logical-fileinfo"))
					{
						error = ftell(real_media);
						//Seek to the audience number pos
						fseek(real_media, 11, SEEK_CUR);
						
						//Read in the codec_audience_number
						UINT32 field_count = 0;
						error = fread((void *)&field_count, 4, 1, real_media);
						bswap((BYTE *)&field_count, 4);

						for (int current_field = 0; current_field < field_count; current_field++)
						{
							UINT32 test_len = 0;
							UINT16 field_title_len = 0;
							UINT16 field_text_len = 0;
							
							RealMedia_Infomation_Field *new_item = new RealMedia_Infomation_Field();

							//Read the total length of this field and text
							error = fread((void *)&test_len, 4, 1, real_media);
							bswap((BYTE *)&test_len, 4);
														
							//Seek ahead one byte, for some reason
							fseek(real_media, 1, SEEK_CUR);
							//Read the length of the field name
							error = fread((void *)&field_title_len, 2, 1, real_media);
							bswap((BYTE *)&field_title_len, 2);	
							//Read in field name
							new_item->field_title = new char[field_title_len+1];
							error = fread((void *)new_item->field_title, field_title_len, 1, real_media);
							new_item->field_title[field_title_len] = 0;														

							error = ftell(real_media);
							//Seek ahead one byte, for some reason
							fseek(real_media, 4, SEEK_CUR);
							//Read the length of the field text
							error = fread((void *)&field_text_len, 2, 1, real_media);
							bswap((BYTE *)&field_text_len, 2);
							//Read in the field text
							new_item->field_text = new char[field_text_len+1];
							error = fread((void *)new_item->field_text, field_text_len, 1, real_media);
							new_item->field_text[field_text_len] = 0;

							field_list.AddItem(new_item);
						}

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
				printf("\nReading Data chunk\n");
				RealMedia_Data_Chunk_Header data_chunk;
				data_chunk.size = block_size;

				error = fread((void *)&data_chunk.num_packets, 4, 1, real_media);
				bswap((BYTE *)&data_chunk.num_packets, 4);

				error = fread((void *)&data_chunk.next_data_header, 4, 1, real_media);
				bswap((BYTE *)&data_chunk.next_data_header, 4);

				error = fread((void *)&data_chunk.object_version, 2, 1, real_media);
				bswap((BYTE *)&data_chunk.object_version, 2);

				//Now we read all the packets
				for (int current_packet_no = 0; current_packet_no < data_chunk.num_packets; current_packet_no++)
				{
					RealMedia_Media_Packet_Header *new_packet = new RealMedia_Media_Packet_Header();
					memset(new_packet, 0, sizeof(*new_packet));
					
					int packet_start_pos = ftell(real_media);
					
					error = fread((void *)&new_packet->length, 2, 1, real_media);
					bswap((BYTE *)&new_packet->length, 2);

					error = fread((void *)&new_packet->stream_number, 2, 1, real_media);
					bswap((BYTE *)&new_packet->stream_number, 2);

					error = fread((void *)&new_packet->timestamp, 4, 1, real_media);
					bswap((BYTE *)&new_packet->timestamp, 4);

					error = fread((void *)&new_packet->reserved, 1, 1, real_media);
					bswap((BYTE *)&new_packet->reserved, 1);

					error = fread((void *)&new_packet->flags, 1, 1, real_media);
					bswap((BYTE *)&new_packet->flags, 1);
					
					
					int data_length = new_packet->length - 10;
					int data_start = ftell(real_media);
					
					fseek(real_media, packet_start_pos + new_packet->length, SEEK_SET);				
					
					RealMedia_Packet *min_packet_data = new RealMedia_Packet();
					memset(min_packet_data, 0, sizeof(*min_packet_data));
					min_packet_data->stream_number = new_packet->stream_number;
					min_packet_data->timestamp = new_packet->timestamp;
					min_packet_data->reserved = new_packet->reserved;
					min_packet_data->flags = new_packet->flags;
					min_packet_data->packet_start_pos = data_start;
					min_packet_data->packet_data_length = data_length;

					packets.AddItem(min_packet_data);
					packets.list_count++;
									
					delete new_packet;
				}
			}

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

BYTE *RealMedia_Reader::GetPacketData(UINT16 packet_no)
{
	if (the_filename == NULL || real_media == NULL)
		return NULL;

	if ((packet_no < packets.list_count) && (packets[packet_no] != NULL))
	{
		BYTE *packet_data = new BYTE[packets[packet_no]->packet_data_length];

		fseek(real_media, packets[packet_no]->packet_start_pos, SEEK_SET);
		fread(packet_data, packets[packet_no]->packet_data_length, 1, real_media);

		return packet_data;
	}

	return NULL;
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

/*******************************************
 *    Begin RealMedia_Packet_List class    *
 *******************************************/
RealMedia_Packet_List::RealMedia_Packet_List() {
	//OutputDebugString("Matroska Attachment List Created\n");
	this->first_item = NULL;
	list_count = 0;
};

RealMedia_Packet_List::~RealMedia_Packet_List() {
	//OutputDebugString("Matroska Attachment List Deleted\n");
	//I should free memory here
	RealMedia_Packet *temp = NULL;

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
int RealMedia_Packet_List::AddItem(RealMedia_Packet *new_item, bool bCheckIfExisting) {
	//Check that some idiot didn't pass a NULL pointer
	if (new_item == NULL)
	{
		//So some idiot tried to pass a NULL pointer :P
		return 1;
	}
	if (bCheckIfExisting)
	{
		//Check if this item is already in the list
		RealMedia_Packet *current = first_item;
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

RealMedia_Packet *RealMedia_Packet_List::operator[] (int requested_index)
{
	RealMedia_Packet *current = first_item;
	for (int item_count = (this->list_count); item_count > 0; item_count--)
	{
		if (requested_index == item_count)
			return current;
		//Keep searching
		current = current->next_item;
	}	
	return NULL;	
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

void bswap(BYTE* s, int len)
{
	for(BYTE* d = s + len-1; s < d; s++, d--)
		*s ^= *d, *d ^= *s, *s ^= *d;
};


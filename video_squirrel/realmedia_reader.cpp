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

RealMedia_Reader::RealMedia_Reader()
{
	properties_block_count = 0;
	media_properties_block_count = 0;
	content_description_block_count = 0;

	for (int i = 0; i < 255; i++)
	{
		properties_block[i] = NULL;
		media_properties_block[i] = NULL;
		content_description_block[i] = NULL;
	}
};

int RealMedia_Reader::Read(const char *filename)
{
	int error = 0;
	FILE *real_media = fopen(filename, "r");

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
			RealMedia_Properties *info = new RealMedia_Properties;
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

				//Add to the array
				properties_block[properties_block_count] = info;
				properties_block_count++;
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
				error = fread((void *)content_desc->title, content_desc->comment_len, 1, real_media);
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

		//Now we seek to the end of the block, (just in case we didn't)
		fseek(real_media, file_pos+block_size, SEEK_SET);
		//If we are seeking to the same point it's useless
		if (file_pos == ftell(real_media))
			keep_looping = false;
	}
	fclose(real_media);
	return 0;
};

void bswap(BYTE* s, int len)
{
	for(BYTE* d = s + len-1; s < d; s++, d--)
		*s ^= *d, *d ^= *s, *s ^= *d;
};


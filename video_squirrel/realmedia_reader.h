/*
 *  Video Squirrel, a cross-platform movie cataloger
 *
 *  realmedia_reader.h
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
	\file realmedia_reader.h
	\version \$Id$
	\brief a cross-platform movie cataloger
	\author Jory Stone            <jcsston @ toughguy.net>
	\author Gabest
*/

#include <stdio.h>
#include <string.h>

#define BYTE unsigned char
#define UINT8 unsigned char
#define UINT16 unsigned short
#define UINT32 unsigned long

#ifndef strcmpi
#define strcmpi strcmp
#endif

/// RealMedia_Properties.flags
#define PN_SAVE_ENABLED 0x0001										//< Allows clients to save a copy of the RealMedia file to disk.
#define PN_PERFECT_PLAY_ENABLED 0x0002						//< Allows clients to use extra buffering to ensure Perfect Play.
#define PN_LIVE_BROADCAST 0x0004									//< The RealMedia file is being generated by a live broadcast.

/// RealMedia_Media_Packet_Header.flags
#define PN_RELIABLE_FLAG 0x0001 //< If this flag is set, the packet is delivered reliably.
#define PN_KEYFRAME_FLAG 0x0002 //< If this flag is set, the packet is part of a key frame or in some way marks a boundary in your data stream. 

struct RealMedia_File_Header
{
	/// The unique object ID for a RealMedia File (.RMF). All RealMedia files begin with this identifier.
	char object_id[4];
	/// The size of the RealMedia header section in bytes.
	UINT32 size;
	/// The version of the RealMedia File Header object. All files created according to this specification have an object_version number of 0 (zero).
	UINT16 object_version;
	/// The version of the RealMedia file in PN Version format. All files created according to this specification have a major version number of 1. This member is present on all RealMedia_File_Header objects with an object_version of 0 (zero).
	UINT32 file_version;
	/// The number of headers in the header section that follow the RealMedia File Header. This member is present on all RealMedia_File_Header objects with an object_version of 0 (zero).
	UINT32 num_headers;
};

struct RealMedia_Properties
{
	//These are not needing, we assume an object_version of 0
	//char object_id[4];
	//UINT32    size;
	//UINT16    object_version;

	/// The maximum bit rate required to deliver this file over a network. This member is present on all Properties objects with an object_version of 0 (zero).
	UINT32 max_bit_rate;
	/// The average bit rate required to deliver this file over a network. This member is present on all Properties objects with an object_version of 0 (zero).
	UINT32 avg_bit_rate;
	/// The largest packet size (in bytes) in the media data. This member is present on all Properties objects with an object_version of 0 (zero).
	UINT32 max_packet_size;
	/// The average packet size (in bytes) in the media data. This member is present on all Properties objects with an object_version of 0 (zero).
	UINT32 avg_packet_size;
	/// The number of packets in the media data. This member is present on all Properties objects with an object_version of 0 (zero).
	UINT32 num_packets;
	/// The duration of the file in milliseconds. This member is present on all Properties objects with an object_version of 0 (zero).
	UINT32 duration;
	/// The number of milliseconds to pre-buffer before starting playback. This member is present on all Properties objects with an object_version of 0 (zero).
	UINT32 preroll;
	/// The offset in bytes from the start of the file to the start of the index header object. This member is present on all Properties objects with an object_version of 0 (zero).
	UINT32 index_offset;
	/// The offset in bytes from the start of the file to the start of the Data Section. This member is present on all Properties objects with an object_version of 0 (zero).
	UINT32 data_offset;
	/// The number of media streams contained in the file. This member is present on all Properties objects with an object_version of 0 (zero).
	UINT16 num_streams;
	/// Flags indicating characteristics of the RealMedia file. Look at RealMedia_Properties.flags for values
	UINT16 flags;
};

struct RealMedia_Media_Properties
{
	//char object_id[4];
	//UINT32     size;
	//UINT16     object_version;

	UINT16 stream_number;
	UINT32                      max_bit_rate;
	UINT32                      avg_bit_rate;
	UINT32                      max_packet_size;
	UINT32                      avg_packet_size;
	UINT32                      start_time;
	UINT32                      preroll;
	UINT32                      duration;
	UINT8                       stream_name_size;
	char     *stream_name;
	UINT8                       mime_type_size;
	char *mime_type;
	UINT32                      type_specific_len;
	char *type_specific_data;
};

struct RealMedia_Content_Description
{
	//char object_id[4];
	//UINT32     size;
	//UINT16      object_version;

	UINT16    title_len;
	char *title;
	UINT16    author_len;
	char *author;
	UINT16    copyright_len;
	char *copyright;
	UINT16    comment_len;
	char *comment;
};

struct RealMedia_Data_Chunk_Header
{
	/// The unique object ID for the Data Chunk Header ('DATA').
  UINT32 object_id;

	/// The size of the Data Chunk in bytes.
  UINT32 size;
	/// The version of the Data Chunk Header object.
  UINT16 object_version;
	/// Number of packets in the data chunk. This member is present on all Data Chunk Header objects with an object_version of 0 (zero).
  UINT32 num_packets; 
	/// Offset from start of file to the next data chunk. This field is not typically used. This member is present on all Data Chunk Header objects with an object_version of 0 (zero).
  UINT32 next_data_header;
};

struct RealMedia_Media_Packet_Header
{
	/// The version of the Media Packet Header object.
	UINT16 object_version;

	/// The length of the packet in bytes. This member is present on all Media Packet Header objects with an object_version of 0 (zero).
	UINT16 length;
	/// The 16-bit alias used to associate data packets with their associated Media Properties Header. This member is present on all Media Packet Header objects with an object_version of 0 (zero).
  UINT16 stream_number;
	/// The timestamp of the packet in milliseconds This member is present on all Media Packet Header objects with an object_version of 0 (zero).
  UINT32 timestamp;
	/// This is not used. This member is present on all Media Packet Header objects with an object_version of 0 (zero).
  UINT8 reserved; 
	/// Flags describing the properties of the packet. Look at RealMedia_Media_Packet_Header.flags for values
  UINT8 flags; 
	/// The application-specific media data. This member is present on all Media Packet Header objects with an object_version of 0 (zero).
  UINT8 *data; 
};


class RealMedia_Reader
{
	public:
		RealMedia_Reader();
		~RealMedia_Reader();
		int Read(const char *filename);

		RealMedia_File_Header the_header;

		RealMedia_Properties properties_block;
		//UINT8 properties_block_count;

		RealMedia_Media_Packet_Header *packets[10000];
		UINT16 packet_count;

		RealMedia_Media_Properties *media_properties_block[255];
		UINT8 media_properties_block_count;

		RealMedia_Content_Description *content_description_block[255];
		UINT8 content_description_block_count;
};

///Byte Swaping Function by Gabest
void bswap(BYTE* s, int len);


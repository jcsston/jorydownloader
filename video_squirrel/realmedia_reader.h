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
#include <malloc.h>
#include <string.h>

#define BYTE unsigned char
#define UINT8 unsigned char
#define UINT16 unsigned short
#define UINT32 unsigned long

#ifndef strcmpi
#define strcmpi stricmp
#endif

typedef struct {
    UINT8 wFormatTag[2];
    UINT32 nChannels;
    UINT32 nSamplesPerSec;
    UINT32 nAvgBytesPerSec;
    UINT16 nBlockAlign;
    UINT16 wBitsPerSample;
    UINT16 cbSize;
} WAVEFORMATEX_real;

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

#pragma pack(push, 1)
struct RealMedia_VideoHeader 
{
	UINT32 dwSize;
	UINT32 fcc1;
	UINT32 fcc2; 
	UINT16 w; 
	UINT16 h; 
	UINT16 bpp; 
	UINT32 unk1; 
	UINT32 unk2; 
	UINT32 type1; 
	UINT32 type2;
	UINT8 w2;
	UINT8 h2;
	UINT8 w3;
	UINT8 h3;
};

struct RealMedia_AudioHeader {
	UINT32 unknown1; // No clue
	UINT32 unknown2; // just need to skip 4, 6? bytes
	UINT16 header_version;
	UINT16 unknown3; // 00 00
	UINT32 format; //? .ra4 or .ra5
	UINT32 unknown4; // ???
	UINT16 format_version; // version (4 or 5)
	UINT32 header_size; // header size == 0x4E 
	UINT16 codec_flavor; // codec flavor id, Also called the codec_audience_number
	UINT32 coded_frame_size; // coded frame size, needed by codec
	UINT32 unknown5; // big number
	UINT32 unknown6; // bigger number
	UINT32 unknown7; // 2 || -''-
	UINT16 sub_packet_h; //Sub packet header???
	UINT16 frame_size;
	UINT16 sub_packet_size;
	UINT16 unknown8; //Two bytes of 0's

	//Here if format_version == 5
	UINT32 unknown9;
	UINT16 unknown10;
	//End if format_version == 5

	UINT16 sample_rate;
	UINT16 unknown11;
	UINT16 sample_size;
	UINT16 channels;

	//Here if format_version == 5
	UINT32 codec_prefix; // Often 'genr'
	UINT32 codec_name; // Can be 'cook'
	//End if format_version == 5

	//If codec_name == "cook"
	UINT16 unknown13; // Skip 3 unknown bytes 
	UINT8 unknown14;
	UINT8 unknown15;  // Skip 1 additional unknown byte If format_version == 5
	UINT32 codecdata_length;
	UINT8 *codecdata; //This is the size codecdata_length
	//End if codec_name == "cook"
};
#pragma pack()

struct RealMedia_Media_Properties
{
	//char object_id[4];
	//UINT32     size;
	//UINT16     object_version;

	UINT16 stream_number;
	UINT32 max_bit_rate;
	UINT32 avg_bit_rate;
	UINT32 max_packet_size;
	UINT32 avg_packet_size;
	UINT32 start_time;
	UINT32 preroll;
	UINT32 duration;
	UINT8 stream_name_size;
	char *stream_name;
	UINT8 mime_type_size;
	char *mime_type;
	UINT32 type_specific_len;
	UINT8 *type_specific_data;

  /// This one sort of applies to all a/v streams :P
	char *codec_name;
	
	//Logical file-info
	//This stuff goes in the RealMedia_Infomation_Field_List

	//Audio Stream
	RealMedia_AudioHeader *audio_header;
	WAVEFORMATEX_real *wav_data;
  //UINT32 frequency;
	//UINT16 samplesize;
  //UINT16 channels;
  //UINT8 codec_audience_number;

	//Video Stream
	RealMedia_VideoHeader *video_header;
	//void *extra_videocodec_data;
	//UINT8 extra_videocodec_data_size;
	//UINT16 frame_width;
  //UINT16 frame_height;
  float frame_rate;

};

struct RealMedia_Content_Description
{
	//char object_id[4];
	//UINT32     size;
	//UINT16      object_version;

	UINT16 title_len;
	char *title;
	UINT16 author_len;
	char *author;
	UINT16 copyright_len;
	char *copyright;
	UINT16 comment_len;
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


struct RealMedia_Packet
{
	/// The start of the raw packet data in the file
	UINT16 packet_start_pos;
	/// The length of the raw packet data in bytes
	UINT16 packet_data_length;
	
	/// The 16-bit alias used to associate data packets with their associated Media Properties Header
  UINT16 stream_number;
	/// The timestamp of the packet in milliseconds
  UINT32 timestamp;
	/// This is not used
  UINT8 reserved; 
	/// Flags describing the properties of the packet. Look at RealMedia_Media_Packet_Header.flags for values
  UINT8 flags; 	
	/// The application-specific media data.
	UINT8 *data; 
	//RealMedia_Packet *next_item;
};

struct RealMedia_Infomation_Field
{
	char *field_title;
	char *field_text;

	RealMedia_Infomation_Field *next_item;
};

class RealMedia_Infomation_Field_List
{
public:
	public:
	RealMedia_Infomation_Field_List();
	~RealMedia_Infomation_Field_List();
	int AddItem(RealMedia_Infomation_Field *new_item, bool bCheckIfExisting = true);	
	int GetListCount();
	RealMedia_Infomation_Field *operator[] (int requested_index);

protected:
	RealMedia_Infomation_Field *first_item;
};
class RealMedia_Reader
{
	public:
		RealMedia_Reader();
		~RealMedia_Reader();
		/// Processes a RealMedia file
		/// Call this first!
		/// \param filename The filename of the file to process
		/// \param bPreReadDataPackets If set to true the data packets are read and the headers stored
		/// \return 0 if successful		
		int Read(const char *filename, bool bPreReadDataPackets = false);
		/// The pre-loading method of reading data packets from a RM file
		/// \param packet_info A pointer to the needed packet info, this can be found in the packet_array[]
		/// \return NULL on error
		/// \return A RealMedia_Packet pointer to the same packet info you with the data memer filled, you have to free the memory
		RealMedia_Packet *GetPacketData(RealMedia_Packet *packet_info);
		/// The on-the-fly method of reading data packets from a RM file
		/// \return NULL on error
		/// \return A RealMedia_Packet pointer with the data member filled, remeber you have to free the memory for both the RealMedia_Packet pointer and the RealMedia_Packet->data pointer
		RealMedia_Packet *GetNextDataPacket();
		/// Get the length of the longest track
		/// \return The length of the longest track in milliseconds
		UINT32 GetLongestTrackLength();

		/// Easy way to read some data
		/// \param store_here Pointer to the location to store the data
		/// \param store_length How much to read, WARNING the store_here pointer must have enough memory
		/// \param bByteSwap Byte Swap the data or not, the main use for the function
		/// \return 0 if failed
		/// \return 1 if succeful, The value returned by fread()
		int ReadData(void *store_here, UINT32 store_length, bool bByteSwap = false);

		/// Byte Swaping Function by Gabest
		/// It's inline because it's so small, the function calling overhead would likely result in more memory and less speed
		/// \param s  A BTYE pointer to the data to swap
		/// \param len Length of the data pointed by s
		void inline bswap(BYTE* s, int len);
		
	//Data members
		//Filename of the current file
		char *the_filename;
		//FILE handle for current file
		FILE *real_media;

		RealMedia_Properties properties_block;

		//Used for both reading methods
		UINT32 packet_count;
		//Used for pre-reading method
		RealMedia_Packet* packet_array;
		//Used for on-the-fly reading method
		UINT32 current_packet_no;
		UINT32 packet_start_pos;
		UINT32 last_packet_start_pos;

		RealMedia_Media_Properties *media_properties_block[255];
		UINT8 media_properties_block_count;

		RealMedia_Infomation_Field_List field_list;

		RealMedia_Content_Description *content_description_block[255];
		UINT8 content_description_block_count;
};

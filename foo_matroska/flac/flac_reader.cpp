/*
 *  FLAC Reader,
 *
 *  flac_reader.cpp
 *
 *  Copyright (C) Jory Stone - July 2003
 *
 *  FLAC Reader is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  FLAC Reader is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with FLAC Reader; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*!
	\file flac_reader.cpp
	\version \$Id$
	\brief 
	\author Jory Stone            <jcsston @ toughguy.net>
	
	\version $Log$
	\version Revision 1.2  2003/08/30 02:01:16  jcsston
	\version Updated CVS copy with my local copy
	\version
	\version Revision 1.1  2003/07/13 08:53:15  jcsston
	\version First Commit,
	\version Has dual reading modes, one full functioning one using libflac
	\version and one written from scratch that can currently read the headers
	\version

*/

#include "flac_reader.h"

FLAC_Reader::FLAC_Reader()
{
	total_file_size = 0;
	last_file_read_pos = 0;
	metadata_header = NULL;
	total_frame_count = 0;
	last_frame_pos = 0;

	frame_buffer = new FLAC__uint8[1024];
	frame_buffer_size = 0;
	frame_buffer_total_size = 1024;
};

FLAC_Reader::~FLAC_Reader()
{
	if (m_FLACDec != NULL)
		FLAC__stream_decoder_delete(m_FLACDec);

	if (metadata_header != NULL)
		delete metadata_header;
};

void FLAC_Reader::ReadStart(const char *flac_file)
{
	//Open up the file
	flac_inputfile = fopen(flac_file, "rb");
	if (flac_inputfile == NULL)
		return;
	
	//Get the total file size
	fseek(flac_inputfile, 0, SEEK_END);
	total_file_size = ftell(flac_inputfile);
	fseek(flac_inputfile, 0, SEEK_SET);
	
	m_FLACDec = FLAC__stream_decoder_new();	

  // Setup the callbacks
	FLAC__stream_decoder_set_read_callback(m_FLACDec, (FLAC__StreamDecoderReadCallback)read_callback);
	FLAC__stream_decoder_set_write_callback(m_FLACDec, (FLAC__StreamDecoderWriteCallback)write_callback);
	FLAC__stream_decoder_set_metadata_callback(m_FLACDec, (FLAC__StreamDecoderMetadataCallback)metadata_callback);
	FLAC__stream_decoder_set_error_callback(m_FLACDec, (FLAC__StreamDecoderErrorCallback)error_callback);

	// Set the client data to this class
	FLAC__stream_decoder_set_client_data(m_FLACDec, (void *)this);

	// Init the decoder
	int ret = FLAC__stream_decoder_init(m_FLACDec);
	if (ret != FLAC__STREAM_DECODER_SEARCH_FOR_METADATA) {
		// Failed to init correctly
		FLAC__stream_decoder_delete(m_FLACDec);
		m_FLACDec = NULL;
		return;
	}
	
	//Decode the metablocks and stop at the first data frame
	FLAC__stream_decoder_process_until_end_of_metadata(m_FLACDec);

	metadata_size = ftell(flac_inputfile);
	
	//Seek to the start of the metablock data
	fseek(flac_inputfile, 0, SEEK_SET);
	metadata_header = new FLAC__byte[metadata_size];
	fread((void *)metadata_header, 1, metadata_size, flac_inputfile);
	
	//Restore the orignal file pos
	fseek(flac_inputfile, metadata_size, SEEK_SET);

	last_frame_pos = last_file_read_pos;	
};

FLAC_Reader_Frame *FLAC_Reader::ReadNextFrame()
{
	frame_buffer_size = 0;

	if (FLAC__stream_decoder_get_state(m_FLACDec) != FLAC__STREAM_DECODER_END_OF_STREAM)
	{
		FLAC__stream_decoder_process_single(m_FLACDec);
		
		m_frame.data = frame_buffer;

		return &m_frame;
	}

	return NULL;
};

FLAC__StreamDecoderReadStatus FLAC_Reader::read_callback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], unsigned *bytes, void *client_data)
{
  FLAC_Reader *flac_reader = (FLAC_Reader *)client_data;
  
	*bytes = fread((void *)buffer, 1, 1, flac_reader->flac_inputfile);
	flac_reader->last_file_read_pos += *bytes;
	
	if (flac_reader->frame_buffer_total_size <= flac_reader->frame_buffer_size+1) {
		flac_reader->frame_buffer_total_size = flac_reader->frame_buffer_size+256;
		
		FLAC__uint8 *local_frame_buffer = new FLAC__uint8[flac_reader->frame_buffer_total_size];
		memcpy(local_frame_buffer, flac_reader->frame_buffer, flac_reader->frame_buffer_size);		

		delete flac_reader->frame_buffer;

		flac_reader->frame_buffer = local_frame_buffer;
	}
	flac_reader->frame_buffer[flac_reader->frame_buffer_size++] = buffer[0];
	
	if (*bytes == 0)
		return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
	
	return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
};

FLAC__StreamDecoderWriteStatus FLAC_Reader::write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
  FLAC_Reader *flac_reader = (FLAC_Reader *)client_data;
  
	flac_reader->total_frame_count++;

	flac_reader->info.max_blocksize = frame->header.blocksize;

	flac_reader->m_frame.file_pos = flac_reader->last_frame_pos;
	flac_reader->m_frame.frame_length = flac_reader->last_file_read_pos - flac_reader->m_frame.file_pos;
	flac_reader->m_frame.timecode = (float)(frame->header.number.sample_number) / (float)frame->header.sample_rate * 1000;
	flac_reader->m_frame.sample = frame->header.number.sample_number;

	//flac_reader->info.bitrate = (float)flac_reader->last_file_read_pos / (flac_reader->m_frame.timecode + 1) * 8;	

	//Update the frame start pos
	flac_reader->last_frame_pos = flac_reader->last_file_read_pos;

	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
};

void FLAC_Reader::metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	FLAC_Reader *flac_reader = (FLAC_Reader *)client_data;

	flac_reader->info = metadata->data.stream_info;
};

void FLAC_Reader::error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
	//Oh an error, Do nothing :P
};


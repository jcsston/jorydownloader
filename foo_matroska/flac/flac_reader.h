/*
 *  FLAC Reader,
 *
 *  flac_reader.h
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
	\file flac_reader.h
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#define FLAC__NO_DLL
#include "FLAC/format.h"
#include "FLAC/stream_decoder.h"
#include "FLAC/seekable_stream_decoder.h"
#include "FLAC/all.h"

class FLAC_Reader_Frame
{
public:
	FLAC_Reader_Frame() {memset((void *)this, 0, sizeof(*this));}
	~FLAC_Reader_Frame() {};

	/// Timecode start of frame (in ms)
	FLAC__uint64 timecode;
	/// Sample start of frame
	FLAC__uint64 sample;
	/// File pos Start
	FLAC__uint64 file_pos;
	/// Length of frame
	FLAC__uint64 frame_length;
	/// Frame Data
	const FLAC__uint8 *data;
};

class FLAC_Reader
{
public:
	FLAC_Reader();
	~FLAC_Reader();
	void ReadStart(const char *flac_file);
	FLAC_Reader_Frame *ReadNextFrame();

	//Callbacks
	static ::FLAC__StreamDecoderReadStatus read_callback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], unsigned *bytes, void *client_data);
	static ::FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data);
	static void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data);
	static void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);

  FLAC__StreamDecoder *m_FLACDec;
  
	FILE *flac_inputfile;
	FLAC__StreamMetadata_StreamInfo info;
	FLAC__uint64 total_file_size;
	FLAC__uint64 last_file_read_pos;
	FLAC__uint64 last_frame_pos;
	FLAC__uint32 total_frame_count;
	
	FLAC__uint8 *metadata_header;
	FLAC__uint32 metadata_size;

	FLAC_Reader_Frame m_frame;

	FLAC__uint8 *frame_buffer;
	FLAC__uint32 frame_buffer_size;
	FLAC__uint32 frame_buffer_total_size;
};


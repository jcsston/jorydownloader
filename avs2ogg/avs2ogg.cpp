
#include "avs2ogg.h"

void usage()
{
	fprintf(stderr, "Usage: -n <input filename> <output filename> \n");
	fprintf(stderr, "\t -n         Normalize Audio \n");		
	fprintf(stderr, "\t <input filename>  Input AVS Script \n");		
	fprintf(stderr, "\t <output filename>  Output PCM WAV, can be - for stdout output \n");			
	fprintf(stderr, "\nThe Deafult Output file is the input filename appended with .wav\n");
}

int main(int argc, const char **args)
{
	bool bNormalize = false;
	std::string inputFilename;
	std::string outputFilename;
	for (int c = 1; c < argc; c++) {
		if (args[c][0] == '-' && args[c][1] == 'n') {
			bNormalize = true;
		
		} else if (inputFilename.length() == 0) {
				inputFilename = args[c];

		} else if (outputFilename.length() == 0) {
				outputFilename = args[c];

		} else {
			usage();
			return -1;
		}
	}	
	
	fprintf(stderr, AVS2OGG_NAME " by Jory Stone <jcsston@toughguy.net>\n");
	if (inputFilename.length() == 0) {
		usage();
		return -1;
	}
	if (outputFilename.length() == 0)
		outputFilename = inputFilename + ".wav";

	fprintf(stderr, "\tInput: %s\n", inputFilename.c_str());
	fprintf(stderr, "\tOutput: %s\n", outputFilename.c_str());

	Avs2Ogg objMain;
	objMain.ReadHeaders(inputFilename, outputFilename);
	
	fprintf(stderr, "\nScanning...\n");
	clock_t startTime = clock();
	// Do scan pass
	while (objMain.WriteWav() == 0);
	float timeSpan = (((double)clock() / CLOCKS_PER_SEC) - ((double)startTime / CLOCKS_PER_SEC));
	
	fprintf(stderr, "\nTook %.2f seconds for the writing pass\n", timeSpan);
	
	if (bNormalize) {
		float ratio = objMain.GetNomalizingFactor();
		fprintf(stderr, "Normalizing with a factor of %.4f ...\n", ratio);
		
		startTime = clock();
		//while (objMain.ReadFrame() == 0);
		objMain.NomalizeWav();
		timeSpan = (((double)clock() / CLOCKS_PER_SEC) - ((double)startTime / CLOCKS_PER_SEC));
		
		fprintf(stderr, "Took %.2f seconds for the normalizing pass\n", timeSpan);
	}
	return 0;
};


Avs2Ogg::Avs2Ogg()  {
	m_Writer = NULL;
#ifdef HAVE_OGG
	m_Encoder = NULL;
#endif // HAVE_OGG
	m_AVIFile = NULL;
	myAVIStream = NULL;
	frameBuffer = NULL;
	frameBufferSize = 0;
	frameBufferTotalSize = 0;
	frameSample = 0;
	lastFrameTimecode = 0;
	m_WavHeader = NULL;
	m_MaxSample = 0;
	//m_bNormalize = false;
	m_Ratio = 0;
	AVIFileInit();
};

Avs2Ogg::~Avs2Ogg() {
	delete frameBuffer; 
	if (myAVIStream != NULL) 
		AVIStreamRelease(myAVIStream); 

	AVIFileRelease(m_AVIFile);

	if (m_Writer != NULL)
		delete m_Writer;
#ifdef HAVE_OGG	
	if (m_Encoder != NULL)
		delete m_Encoder;
#endif // HAVE_OGG
	if (m_WavHeader != NULL)
		free(m_WavHeader);

	AVIFileExit();
};

WORD Avs2Ogg::ReadHeaders(const std::string &inputFilename, const std::string &outputFilename) {
	HRESULT ret = 0;
	m_Filename = inputFilename;
	
	ret = AVIFileOpen(&m_AVIFile, m_Filename.c_str(), OF_SHARE_DENY_WRITE, 0L);
	if (ret != AVIERR_OK) {
		fprintf(stderr, "AVIFileOpen failed, unable to open %s :", inputFilename.c_str());
		return -1;
	}

	fprintf(stderr, "Scanning for Audio Stream...\n");
	LONG currentAVIStream = 0;
	LONG streamSampleLength = 0;
	bool bMoreStreams = true;
	while (bMoreStreams) {		
		// Go through the streams
		ret = AVIFileGetStream(m_AVIFile, &myAVIStream, 0, currentAVIStream);
		if (ret != AVIERR_OK) {
			if (currentAVIStream == 0) {
				fprintf(stderr, "AVIFileGetStream failed, unable to get any streams!");
				return -1;
			}
			break;
		}		
		ret = AVIStreamInfo(myAVIStream, &myAVIStreamInfo, sizeof(myAVIStreamInfo));
		if (ret != AVIERR_OK) {
			fprintf(stderr, "AVIStreamInfo failed, unable to get stream info!");
			return -1;
		}

		if (myAVIStreamInfo.fccType == streamtypeAUDIO) {			
			fprintf(stderr, "Found Audio Stream\n");
			m_WavHeader = (WAVEFORMATEX *)malloc(sizeof(WAVEFORMATEX));
			LONG cbACM = sizeof(WAVEFORMATEX);
			ZeroMemory(m_WavHeader, cbACM);
			m_WavHeader->cbSize = 0;
			if(AVIStreamReadFormat(myAVIStream, 0, m_WavHeader, &cbACM) != AVIERR_OK) {
				fprintf(stderr, "AVIStreamReadFormat failed!");
				return -1;
			}
			streamSampleLength = AVIStreamLength(myAVIStream);
			break;
		}
		currentAVIStream++;
	}
	
	if (m_WavHeader != NULL) {		
#ifdef HAVE_OGG
		m_Encoder = new OggEncoder(m_Filename + ".ogg");
		m_Encoder->WriteHeaders(m_WavHeader);
#endif // HAVE_OGG
		m_Writer = new WavWriter(outputFilename);
		m_Writer->WriteHeaders(m_WavHeader, streamSampleLength);
	}
	return 0;
};

#ifdef HAVE_OGG
WORD Avs2Ogg::EncodeVorbis() {		
	if (frameSample >= myAVIStreamInfo.dwLength)
		return 2;
	LONG samplesRead;
	LONG hr = AVIStreamRead(myAVIStream, frameSample, AVISTREAMREAD_CONVENIENT, frameBuffer, frameBufferTotalSize, &frameBufferSize, &samplesRead);
	if (frameBufferTotalSize < frameBufferSize) {
		//fprintf(stderr, "\nFrame Buffer was too small for AVIStreamRead.\n");
		frameBufferTotalSize = frameBufferSize+1;
		delete frameBuffer;
		frameBuffer = new BYTE[frameBufferTotalSize+1];					
		hr = AVIStreamRead(myAVIStream, frameSample, AVISTREAMREAD_CONVENIENT, frameBuffer, frameBufferTotalSize, &frameBufferSize, &samplesRead);
	}				
	if (hr != AVIERR_OK) {
		fprintf(stderr, "AVIStreamRead returned error.");
		return 1;
	}				

	if (frameBufferSize > 0) {
		if (m_Ratio != 0) {
			if (m_WavHeader->wBitsPerSample == 16) {
				DWORD sampleCount = frameBufferSize / 2;
				for (DWORD s = 0; s < sampleCount; s++) {
						((short *)frameBuffer)[s] = ((short *)frameBuffer)[s] * m_Ratio;
				}
			}	else if (m_WavHeader->wBitsPerSample == 8) {
				DWORD sampleCount = frameBufferSize / 2;
				for (DWORD s = 0; s < sampleCount; s++) {
					((char *)frameBuffer)[s] = ((char *)frameBuffer)[s] * m_Ratio;
				}
			}
		}
		m_Encoder->EncodeFrame(frameBuffer, frameBufferSize);
		frameSample += samplesRead;			
		//bufferedPacket->m_Timecode = AVIStreamSampleToTime(stream->myAVIStream, stream->frameSample);
	}
	return 0;
};
#endif // HAVE_OGG
WORD Avs2Ogg::WriteWav() {		
	if (frameSample >= myAVIStreamInfo.dwLength)
		return 2;
	LONG samplesRead;
	LONG hr = AVIStreamRead(myAVIStream, frameSample, AVISTREAMREAD_CONVENIENT, frameBuffer, frameBufferTotalSize, &frameBufferSize, &samplesRead);
	if (frameBufferTotalSize < frameBufferSize) {
		//wxLogDebug(_T("\nFrame Buffer was too small for AVIStreamRead.\n"));
		frameBufferTotalSize = frameBufferSize+1;
		delete frameBuffer;
		frameBuffer = new BYTE[frameBufferTotalSize+1];					
		hr = AVIStreamRead(myAVIStream, frameSample, AVISTREAMREAD_CONVENIENT, frameBuffer, frameBufferTotalSize, &frameBufferSize, &samplesRead);
	}				
	if (hr != AVIERR_OK) {
		fprintf(stderr, "AVIStreamRead returned error.");
		return 1;
	}				

	if (frameBufferSize > 0) {		
		//m_Encoder->EncodeFrame(frameBuffer, frameBufferSize);
		m_Writer->WriteData(frameBuffer, frameBufferSize);
		frameSample += samplesRead;		
		if (m_WavHeader->wBitsPerSample == 16) {
			DWORD sampleCount = frameBufferSize / 2;
			for (DWORD s = 0; s < sampleCount; s++) {
				if (((short *)frameBuffer)[s] > m_MaxSample)
					m_MaxSample = ((short *)frameBuffer)[s];
			}
		}	else if (m_WavHeader->wBitsPerSample == 8) {
			DWORD sampleCount = frameBufferSize / 2;
			for (DWORD s = 0; s < sampleCount; s++) {
				if (((char *)frameBuffer)[s] > m_MaxSample)
					m_MaxSample = ((char *)frameBuffer)[s];
			}
		}
		//bufferedPacket->m_Timecode = AVIStreamSampleToTime(stream->myAVIStream, stream->frameSample);
	}
	return 0;
};

WORD Avs2Ogg::ScanOnly() {		
	if (frameSample >= myAVIStreamInfo.dwLength)
		return 2;
	LONG samplesRead;
	LONG hr = AVIStreamRead(myAVIStream, frameSample, AVISTREAMREAD_CONVENIENT, frameBuffer, frameBufferTotalSize, &frameBufferSize, &samplesRead);
	if (frameBufferTotalSize < frameBufferSize) {
		//wxLogDebug(_T("\nFrame Buffer was too small for AVIStreamRead.\n"));
		frameBufferTotalSize = frameBufferSize+1;
		delete frameBuffer;
		frameBuffer = new BYTE[frameBufferTotalSize+1];					
		hr = AVIStreamRead(myAVIStream, frameSample, AVISTREAMREAD_CONVENIENT, frameBuffer, frameBufferTotalSize, &frameBufferSize, &samplesRead);
	}				
	if (hr != AVIERR_OK) {
		fprintf(stderr, "AVIStreamRead returned error.");
		return 1;
	}				

	if (frameBufferSize > 0) {		
		//m_Encoder->EncodeFrame(frameBuffer, frameBufferSize);
		//m_Writer->WriteData(frameBuffer, frameBufferSize);
		frameSample += samplesRead;		
		if (m_WavHeader->wBitsPerSample == 16) {
			DWORD sampleCount = frameBufferSize / 2;
			for (DWORD s = 0; s < sampleCount; s++) {
				if (((short *)frameBuffer)[s] > m_MaxSample)
					m_MaxSample = ((short *)frameBuffer)[s];
			}
		}	else if (m_WavHeader->wBitsPerSample == 8) {
			DWORD sampleCount = frameBufferSize / 2;
			for (DWORD s = 0; s < sampleCount; s++) {
				if (((char *)frameBuffer)[s] > m_MaxSample)
					m_MaxSample = ((char *)frameBuffer)[s];
			}
		}
		//bufferedPacket->m_Timecode = AVIStreamSampleToTime(stream->myAVIStream, stream->frameSample);
	}
	return 0;
};

WORD Avs2Ogg::NomalizeWav() {
	m_Writer->Nomalize(m_Ratio);
	return 0;
};

float Avs2Ogg::GetNomalizingFactor() {
	//m_bNormalize = true;
	frameSample = 0;
	if (m_WavHeader->wBitsPerSample == 16) {
		return m_Ratio = ((unsigned)0xFFFF / 2) / ((float)m_MaxSample * 1.01f);

	} else if (m_WavHeader->wBitsPerSample == 8) {
		return m_Ratio = ((unsigned)0xFF / 2) / ((float)m_MaxSample * 1.01f);

	} else {
		return m_Ratio = 1.0f;
	}
};

WavWriter::WavWriter(const std::string &filename) {
	m_bClosed = false;
	m_Outputfilename = filename;
	if (m_Outputfilename == "-") {
		m_OutputFile = stdout;
		fprintf(stderr, "\tUsing stdout for WAV Output\n");
	} else {
		m_OutputFile = fopen(m_Outputfilename.c_str(), "wb");
	}
	m_DataSize = 0;
	m_DataSizePos = 0;
	//m_MaxSample = 0;
};

WavWriter::~WavWriter() {
	//float ratio = ((unsigned)0xFFFF / 2) / ((float)m_MaxSample * 1.1f);
	if (!m_bClosed) {
		Close();
	}
};

WORD WavWriter::WriteHeaders(WAVEFORMATEX *wavhdr, LONG streamSampleLength) {
	fprintf(stderr, "\nWriting WAV Headers...");

	DWORD dwTemp;
	WORD wTemp;
	BYTE bTemp;

	if (m_bClosed)
		return -1;
	
	memcpy(&m_WavHeader, wavhdr, sizeof(WAVEFORMATEX));
	m_WavHeader.cbSize = 0;
	// Write the base header
	fwrite("RIFF", 1, 4, m_OutputFile);
	dwTemp = streamSampleLength * m_WavHeader.nBlockAlign + 450;
	fwrite(&dwTemp, 1, 4, m_OutputFile);
	fwrite("WAVE", 1, 4, m_OutputFile);

	// Format chunk header
	fwrite("fmt ", 1, 4, m_OutputFile);
	// Size of the chunk
	dwTemp = 0x10;
	fwrite(&dwTemp, 1, 4, m_OutputFile);
	// Format Tag
	wTemp = wavhdr->wFormatTag;
	fwrite(&wTemp, 1, 2, m_OutputFile);
	// Channel count
	wTemp = wavhdr->nChannels;
	fwrite(&wTemp, 1, 2, m_OutputFile);
	// Sample Rate
	dwTemp = wavhdr->nSamplesPerSec;
	fwrite(&dwTemp, 1, 4, m_OutputFile);
	// Bytes Per Second
	dwTemp = wavhdr->nAvgBytesPerSec;
	fwrite(&dwTemp, 1, 4, m_OutputFile);
	// Bytes Per Sample (all chanels)
	wTemp = wavhdr->nBlockAlign;
	fwrite(&wTemp, 1, 2, m_OutputFile);
	// Bits Per Sample (per channel)
	wTemp = wavhdr->wBitsPerSample;
	fwrite(&wTemp, 1, 2, m_OutputFile);

	fwrite("data", 1, 4, m_OutputFile);
	m_DataSizePos = ftell(m_OutputFile);
	dwTemp = streamSampleLength * m_WavHeader.nBlockAlign;
	fwrite(&dwTemp, 1, 4, m_OutputFile);
	// Reserve space for the data chunk size
	//fwrite("    ", 1, 4, m_OutputFile);
	return 0;
};

WORD WavWriter::WriteData(const BYTE *data, DWORD size) {
	if (m_bClosed)
		return -1;

	if (size == fwrite(data, 1, size, m_OutputFile)) {
		m_DataSize += size;

	} else {
		return 1;
	}
	return 0;
};

WORD WavWriter::Nomalize(float ratio) {
	fseek(m_OutputFile, m_DataSizePos+4, SEEK_SET);

	if (m_WavHeader.wBitsPerSample == 16) {
		short sample;
		DWORD sampleCount = m_DataSize / 2;
		for (DWORD s = 0; s < sampleCount; s++) {			
			fread(&sample, 2, 1, m_OutputFile);
			sample = sample * ratio;
			fwrite(&sample, 2, 1, m_OutputFile);
		}
	}	else if (m_WavHeader.wBitsPerSample == 8) {
		char sample;
		DWORD sampleCount = m_DataSize / 2;
		for (DWORD s = 0; s < sampleCount; s++) {
			fread(&sample, 1, 1, m_OutputFile);
			sample = sample * ratio;
			fwrite(&sample, 1, 1, m_OutputFile);
		}
	}	
	return 0;
};

WORD WavWriter::Close() {		
	// Update the chunk sizes
	fseek(m_OutputFile, m_DataSizePos, SEEK_SET);
	fwrite(&m_DataSize, 1, 4, m_OutputFile);

	m_DataSize += m_DataSizePos - 8;
	fseek(m_OutputFile, 4, SEEK_SET);
	fwrite(&m_DataSize, 1, 4, m_OutputFile);

	fclose(m_OutputFile);
	m_OutputFile = NULL;
	
	m_bClosed = true;
	return 0;
};
#ifdef HAVE_OGG
OggEncoder::OggEncoder(const std::string &filename) {	
	m_bEOS = false;
	m_bClosed = false;
	m_Outputfilename = filename;
	
	m_OggOutFile = fopen(m_Outputfilename.c_str(), "wb");

	vorbis_info_init(&m_vi);		
};

WORD OggEncoder::WriteHeaders(WAVEFORMATEX *wavhdr, float quality) {
	int ret;
	
	if (wavhdr != NULL)
		memcpy(&m_WavHeader, wavhdr, sizeof(WAVEFORMATEX));
	else 
		return -1;
	
	// Example quality mode .4: 44kHz stereo coupled, roughly 128kbps VBR
	ret = vorbis_encode_init_vbr(&m_vi, m_WavHeader.nChannels, m_WavHeader.nSamplesPerSec, quality);

  vorbis_comment_init(&m_vc);
  vorbis_comment_add_tag(&m_vc, "ENCODER", AVS2OGG_NAME);	

  /* set up the analysis state and auxiliary encoding storage */
  vorbis_analysis_init(&m_vd, &m_vi);
  vorbis_block_init(&m_vd, &m_vb);
  
  /* set up our packet->stream encoder */
  /* pick a random serial number; that way we can more likely build
     chained streams just by concatenation */
  srand(time(NULL));
  ogg_stream_init(&m_os, rand());

  /* Vorbis streams begin with three headers; the initial header (with
     most of the codec setup parameters) which is mandated by the Ogg
     bitstream spec.  The second header holds any comment fields.  The
     third header holds the bitstream codebook.  We merely need to
     make the headers, then pass them to libvorbis one at a time;
     libvorbis handles the additional Ogg bitstream constraints */

	{
		ogg_packet header;
		ogg_packet header_comm;
		ogg_packet header_code;

		vorbis_analysis_headerout(&m_vd, &m_vc, &header,& header_comm, &header_code);
		ogg_stream_packetin(&m_os, &header); /* automatically placed in its own page */
		ogg_stream_packetin(&m_os, &header_comm);
		ogg_stream_packetin(&m_os, &header_code);

		// This ensures the actual audio data will start on a new page, 
		// as per spec
		int result = 1;
		while (result != 0) {
			result = ogg_stream_flush(&m_os, &m_og);
			if (result == 0)
				break;
			fwrite(m_og.header, 1, m_og.header_len, m_OggOutFile);
			fwrite(m_og.body, 1, m_og.body_len, m_OggOutFile);
		}
	}

	return 0;
};

WORD OggEncoder::EncodeFrame(const BYTE *data, DWORD size) {
	/* data to encode */

	/* expose the buffer to submit data */
	float **buffer = vorbis_analysis_buffer(&m_vd, size / m_WavHeader.nBlockAlign);

	DWORD i;
	/* uninterleave samples */
	for(i=0;i<size/m_WavHeader.nBlockAlign;i++){
		buffer[0][i]=((data[i*4+1]<<8)|
			(0x00ff&(int)data[i*4]))/32768.f;
		buffer[1][i]=((data[i*4+3]<<8)|
			(0x00ff&(int)data[i*4+2]))/32768.f;
	}

	/* tell the library how much we actually submitted */
	vorbis_analysis_wrote(&m_vd, i);

	/* vorbis does some data preanalysis, then divvies up blocks for
	more involved (potentially parallel) processing.  Get a single
	block for encoding now */
	while (vorbis_analysis_blockout(&m_vd, &m_vb) == 1) {

		/* analysis, assume we want to use bitrate management */
		vorbis_analysis(&m_vb, NULL);
		vorbis_bitrate_addblock(&m_vb);

		while (vorbis_bitrate_flushpacket(&m_vd, &m_op)) {

			/* weld the packet into the bitstream */
			ogg_stream_packetin(&m_os, &m_op);

			/* write out pages (if any) */
			while (!m_bEOS) {
				if (ogg_stream_pageout(&m_os, &m_og) == 0)
					break;
				fwrite(m_og.header, 1, m_og.header_len, m_OggOutFile);
				fwrite(m_og.body, 1, m_og.body_len, m_OggOutFile);

				/* this could be set above, but for illustrative purposes, I do
				it here (to show that vorbis does know where the stream ends) */

				if (ogg_page_eos(&m_og))
					m_bEOS = true;
			}
		}
	}
	return 0;
};

WORD OggEncoder::Close() {
	if (!m_bClosed) {
		m_bClosed = true;
		vorbis_analysis_wrote(&m_vd, 0);
		while(!m_bEOS){
			if (ogg_stream_pageout(&m_os, &m_og) == 0)
				break;
			fwrite(m_og.header, 1, m_og.header_len, m_OggOutFile);
			fwrite(m_og.body, 1, m_og.body_len, m_OggOutFile);

			/* this could be set above, but for illustrative purposes, I do
			it here (to show that vorbis does know where the stream ends) */

			if (ogg_page_eos(&m_og))
				m_bEOS = true;
		}
		
		ogg_stream_clear(&m_os);
		vorbis_block_clear(&m_vb);
		vorbis_dsp_clear(&m_vd);
		vorbis_comment_clear(&m_vc);
		vorbis_info_clear(&m_vi);
		
		fclose(m_OggOutFile);
		return 0;
	}
	return 1;
}

OggEncoder::~OggEncoder() {
	Close();	
};
#endif // HAVE_OGG

#if 0
struct MadDecoderData {
  unsigned char const *start;
  unsigned long length;
};

/*
 * This is the input callback. The purpose of this callback is to (re)fill
 * the stream buffer which is to be decoded. In this example, an entire file
 * has been mapped into memory, so we just call mad_stream_buffer() with the
 * address and length of the mapping. When this callback is called a second
 * time, we are finished decoding.
 */

enum mad_flow CMad::input(void *data, struct mad_stream *stream)
{
  CMad *pCMad = (CMad *)data;

  if (pCMad->m_DataLength > 0)
    return MAD_FLOW_STOP;

  mad_stream_buffer(stream, &pCMad->m_Data[0], pCMad->m_DataLength);

  pCMad->m_DataLength = 0;

  return MAD_FLOW_CONTINUE;
}

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */

inline signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */

enum mad_flow CMad::output(void *data, struct mad_header const *header, struct mad_pcm *pcm)
{
	CMad *pCMad = (CMad *)data;
  unsigned int nchannels, nsamples;
  mad_fixed_t const *left_ch, *right_ch;

  /* pcm->samplerate contains the sampling frequency */

  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];

  while (nsamples--) {
    signed int sample;

    /* output sample(s) in 16-bit signed little-endian PCM */

    sample = scale(*left_ch++);
    putchar((sample >> 0) & 0xff);
    putchar((sample >> 8) & 0xff);

    if (nchannels == 2) {
      sample = scale(*right_ch++);
      putchar((sample >> 0) & 0xff);
      putchar((sample >> 8) & 0xff);
    }
  }

  return MAD_FLOW_CONTINUE;
}

/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or
 * libmad/stream.h) header file.
 */

enum mad_flow CMad::error(void *data, struct mad_stream *stream, struct mad_frame *frame)
{
  CMad *pCMad = (CMad *)data;

/*  ffprintf(stderr, stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
	  stream->error, mad_stream_errorstr(stream),
	  stream->this_frame - buffer->start);
*/
  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;
}

int CMad::decode(unsigned char const *start, unsigned long length)
{  
  int result;


  /* configure input, output, and error functions */
  mad_decoder_init(&m_Decoder, this,
		   input, 0 /* header */, 0 /* filter */, output,
		   error, 0 /* message */);

  /* start decoding */

  result = mad_decoder_run(&m_Decoder, MAD_DECODER_MODE_SYNC);

  /* release the decoder */

  mad_decoder_finish(&m_Decoder);

  return result;
}
#endif

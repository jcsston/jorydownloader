
#ifndef _AVS2OGG_H_
#define _AVS2OGG_H_

#define AVS2OGG_NAME "avs2wav v1.1"

#include <windows.h>
#include <string.h>
#include <string>
#include <tchar.h>
#include <time.h>
#include <streams.h>
#include <vector>
#include <Vfw.h>
#define wxLogError _tprintf
#define wxLogDebug _tprintf



class WavWriter {
public:
	WavWriter(const std::string &filename);
	~WavWriter();

	WORD WriteHeaders(WAVEFORMATEX *wavhdr, LONG streamSampleLength);
	WORD WriteData(const BYTE *data, DWORD size);
	WORD Nomalize(double ratio);
	WORD Close();

protected:
	FILE *m_OutputFile;
	std::string m_Outputfilename;
	bool m_bClosed;
	
	DWORD m_DataSizePos;
	DWORD m_DataSize;	
	WAVEFORMATEX m_WavHeader;
};
#ifdef HAVE_OGG
class OggEncoder {
public:
	OggEncoder(const std::string &filename);
	~OggEncoder();
	
	WORD WriteHeaders(WAVEFORMATEX *wavhdr, float quality = 0.3);
	WORD EncodeFrame(const BYTE *data, DWORD size);
	WORD Close();

protected:
	FILE *m_OggOutFile;
	std::string m_Outputfilename;
	WAVEFORMATEX m_WavHeader;
	bool m_bClosed;
	bool m_bEOS;

	/// take physical pages, weld into a logical stream of packets
  ogg_stream_state m_os;
	/// one Ogg bitstream page.  Vorbis packets are inside
  ogg_page m_og;
	/// one raw packet of data for decode
  ogg_packet m_op; 
	///struct that stores all the static vorbis bitstream settings
	vorbis_info m_vi; 
	/// struct that stores all the user comments
  vorbis_comment m_vc;
  vorbis_dsp_state m_vd; /* central working state for the packet->PCM decoder */
  vorbis_block     m_vb; /* local working space for packet->PCM decode */
};
#endif // HAVE_OGG
class Avs2Ogg {
public:
  Avs2Ogg();
  ~Avs2Ogg();

	WORD ReadHeaders(const std::string &inputFilename, const std::string &outputFilename);	
	WORD ScanOnly();
	WORD WriteWavWithGain(double fGain);
#ifdef HAVE_OGG
	WORD EncodeVorbis();	
#endif // HAVE_OGG
	WORD WriteWav();
	WORD NomalizeWav();
	float GetNomalizingFactor();	

protected:
	WavWriter *m_Writer;
#ifdef HAVE_OGG
	OggEncoder *m_Encoder;
#endif // HAVE_OGG
	PAVIFILE m_AVIFile;
	PAVISTREAM m_AVIStream;
	AVISTREAMINFO m_AVIStreamInfo;		

	std::string m_Filename;

	BYTE *frameBuffer;
	LONG frameBufferSize;
	LONG frameBufferTotalSize;
	LONG frameSample;
	DWORD lastFrameTimecode;
	long m_MaxSample;
	//bool m_bNormalize;
	double m_Ratio;

	PAVISTREAM myAVIStream;
	AVISTREAMINFO myAVIStreamInfo;			

	WAVEFORMATEX *m_WavHeader;
};

struct InputAVSStruct {
	std::string in_filename;
	std::string out_filename;
	int output_type;
	float ogg_quality;
};

#if 0
class CMad {
public:
	int decode(unsigned char const *start, unsigned long length);

protected:
	static enum mad_flow input(void *data, struct mad_stream *stream);
	static enum mad_flow output(void *data, struct mad_header const *header, struct mad_pcm *pcm);
	static enum mad_flow error(void *data, struct mad_stream *stream, struct mad_frame *frame);

	mad_decoder m_Decoder;
	std::vector<BYTE> m_Data;	
	long m_DataLength;
};
#endif

#endif _AVS2OGG_H_

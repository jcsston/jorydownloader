#ifndef __MPEGINFO_H__
#define __MPEGINFO_H__

//hmm
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <conio.h>
#include <sys/types.h>
#include <string.h>
//#include <unistd.h>

#define READINSIZE 5000000

class CMPEGInfo
{
public:
	CMPEGInfo();
	bool LoadMPEG2(const char *szPath);
	bool LoadSlowMPEG2(const char *szPath);
	bool LoadRawMPEG2(const char *szPath);
	bool LoadMPEG24Info(const char *szPath);
	double GetFrameRate();
	unsigned int GetTotalFrames();
	int GetWidth();
	int GetHeight();
	char *GetARText() { return szAspectRatio; };

private:
	unsigned int Get_Bits(unsigned int N);
	unsigned int CurrentBfr;
	unsigned int BitsLeft;

	unsigned int total_frames;
	double frame_rate;
	int horiz_size, vert_size;
	char szAspectRatio[20];
};

#endif

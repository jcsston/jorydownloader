
#include "MPEGInfo.h"
#include <stdio.h>

double Frame_Rates[] = { 0,						// forbidden
						 24000.0/1001.0,		// 23.976
						 24,					// FILM
						 25,					// PAL
						 30000.0/1001.0,		// 29.97
						 30,
						 50,
						 60000.0/1001.0,		// 59.94
						 60,
						 24,					// Reserved
						 -1,					// Reserved
						 -1,					// Reserved
						 -1,					// Reserved
						 -1,					// Reserved
						 -1,					// Reserved	
						 -1 					// Reserved
};

char Aspect_Ratios[][10] = { "Forbidden",
							 "Bad AR",
							 "4:3",
							 "16:9",
							 "1.21",
							 "Reserved",
							 "Reserved",
							 "Reserved",
							 "Reserved",
							 "Reserved",
							 "Reserved",
							 "Reserved",
							 "Reserved",
							 "Reserved",
							 "Reserved",
							 "Reserved"
};

CMPEGInfo::CMPEGInfo()
{
	strcpy(szAspectRatio, "");
}

// Finds gop header at start & finish. Then find sequence header info.
bool CMPEGInfo::LoadRawMPEG2(const char *szPath)
{
	total_frames = 0;
	int start_hour, start_min, start_sec;
	int end_hour, end_min, end_sec;

	int in = open(szPath, _O_BINARY | _O_RDONLY);
	if ( in == -1 )
		return false;

	unsigned char *buffer = (unsigned char *) malloc(READINSIZE);
	
	int count = 0;
	int frame_count = 0;

	int readin = read(in, buffer, READINSIZE);

	if ( readin < 1 )
	{
		printf("The read Failed!\n");
		return false;
	}

	printf("\nTime Info\n---------\n");

	// Loop once to get start, then loop to get end
	for ( int j = 0; j != 2; j++ )
	{
		for ( int i = 0; i != readin; i++ )
		{
			// Look for the GOP sequence 0 0 1 b8
			if ( buffer[i] == 0 && buffer[i+1] == 0 )
			{
				if ( buffer[i+2] == 1 && buffer[i+3] == 0xb8 )
				{
					// read in DWORD
					CurrentBfr = ( buffer[i+4] << 24 ) |
								 ( buffer[i+5] << 16 ) |
								 ( buffer[i+6] << 8  ) |
								 ( buffer[i+7] );
					BitsLeft = 32;

					int gop_hour;
					int gop_minute;
					int gop_sec;
					int gop_frame;

					int drop_flag;
					int closed_gop;
					int broken_link;

					drop_flag   = Get_Bits(1);
					gop_hour    = Get_Bits(5);
					gop_minute  = Get_Bits(6);
					BitsLeft -= 1; // marker bit
					gop_sec     = Get_Bits(6);
					gop_frame	= Get_Bits(6);
					closed_gop  = Get_Bits(1);
					broken_link = Get_Bits(1);

					// Checks there isnt any "bad" timecodes
					if ( (gop_hour >= 0 && gop_minute >= 0) )
					{
						if ( gop_sec < 0 ) 
							gop_sec = gop_sec + 60;	// correct gop_seconds
						
						// If getting start time
						if ( j == 0 )
						{
							start_hour = gop_hour;
							start_min = gop_minute;
							start_sec = gop_sec;
							break;
						}

						end_hour = gop_hour;
						end_min = gop_minute;
						end_sec = gop_sec;
					}
				}
			}
		}

		// Start Time got, now get end time
		// Just got to find the last GOP & read the Time code
		lseek(in, -READINSIZE, SEEK_END);
		readin = read(in, buffer, READINSIZE);
	}

	int SecLength = ((end_hour * 3600) + ( end_min * 60) + end_sec) -
					((start_hour * 3600) + ( start_min * 60) + start_sec);

	// Now get Sequence header & find other info so now were searching for 0 0 01 b3
	int horiz, vert, fps, AR;
	lseek(in, 0, SEEK_SET);
	readin = read(in, buffer, READINSIZE);
	for ( int i = 0; i != readin; i++ )
	{
		// Look for the sequence header:- 0 0 1 b3
		if ( buffer[i] == 0 && buffer[i+1] == 0 )
		{
			if ( buffer[i+2] == 1 && buffer[i+3] == 0xb3 )
			{
				// read in DWORD
				CurrentBfr = ( buffer[i+4] << 24 ) |
							 ( buffer[i+5] << 16 ) |
							 ( buffer[i+6] << 8  ) |
							 ( buffer[i+7] );
				BitsLeft = 32;

				horiz = Get_Bits(12);
				vert = Get_Bits(12);
				AR = Get_Bits(4); 
				fps = Get_Bits(4); 
				break;
			}
		}
	}


	frame_rate = Frame_Rates[fps];
	horiz_size = horiz;
	vert_size = vert;
	total_frames = (int)(((float)SecLength * Frame_Rates[fps])+0.5);//(endTime - startTime)*frame_rate;

	// free mem, close file & bail
	free(buffer);
	close(in);
	return true;
}

// Get First & last PTS, Then gets sequence header
bool CMPEGInfo::LoadMPEG2(const char *szPath)
{

	int in = open(szPath, _O_BINARY | _O_RDONLY);

	total_frames = 0;

	if ( in == -1 )
		return false;

	long double startTime = 0.0, endTime = 0.0;

	unsigned char *buffer = (unsigned char *) malloc(READINSIZE);
	
	if ( buffer == NULL )
		return false;

	int count = 0;
	int frame_count = 0;

	int readin = read(in, buffer, READINSIZE);

	if ( readin < 1 )
	{
		return false;
	}

	// Loop once to get start, then loop to get end
	for ( int j = 0; j != 2; j++ )
	{
		for ( int i = 0; i != readin; i++ )
		{
			if ( buffer[i] == 0 && buffer[i+1] == 0 )
			{
				if ( buffer[i+2] == 1 )// && buffer[i+3] == 0x1 )
				{
					// e0 
					if ( buffer[i+3] == 0xe0 )
					{
						// Skip two bytes for packet length
						unsigned char *ptr = &buffer[i+6];
						// read in DWORD
						CurrentBfr = ( ptr[0] << 24 ) |
									 ( ptr[1] << 16 ) |
									 ( ptr[2] << 8  ) |
									 ( ptr[3] );
						BitsLeft = 32;
						// skip 8 bits
						Get_Bits(8);
						int PTS_DTS_flag = Get_Bits(2);
						if ( PTS_DTS_flag == 2 || PTS_DTS_flag == 3 )
						{
							unsigned long pts1=0, pts2=0, pts3=0;
							// skip three bytes to read PTS
							ptr += 3;
							CurrentBfr = ( ptr[0] << 24 ) |
										 ( ptr[1] << 16 ) |
										 ( ptr[2] << 8  ) |
										 ( ptr[3] );
							BitsLeft = 32;
							// Skip '0010'
							Get_Bits(4);
							pts1 = Get_Bits(3);		// PTS [32..30]
							ptr += 1;
							CurrentBfr = ( ptr[0] << 24 ) |
										 ( ptr[1] << 16 ) |
										 ( ptr[2] << 8  ) |
										 ( ptr[3] );
							BitsLeft = 32;
							pts2 = Get_Bits(15);	// PTS [29..15]
							Get_Bits(1);			// marker bit
							pts3 = Get_Bits(15);	// PTS [14..0]

							unsigned long PTS;
							PTS = pts1 << 30;
							PTS |= pts2 << 15;
							PTS |= pts3;

							if ( j == 0 )
							{
								startTime = ((long double)PTS) / 90000.0;
								break;
							}

							long double temp = ((long double)PTS) / 90000.0;
							if ( temp > endTime )
								endTime = temp;

						}
					}
				}
			}
		}

		// Start Time got, now get end time
		// Just got to find the last PTS & read the Time code
		lseek(in, -READINSIZE, SEEK_END);
		readin = read(in, buffer, READINSIZE);
	}

	// Now get Sequence header & find other info so now were searching for 0 0 01 b3
	int horiz, vert, fps, AR;
	lseek(in, 0, SEEK_SET);
	readin = read(in, buffer, READINSIZE);
	for ( int i = 0; i != readin; i++ )
	{
		// Look for the sequence header:- 0 0 1 b3
		if ( buffer[i] == 0 && buffer[i+1] == 0 )
		{
			if ( buffer[i+2] == 1 && buffer[i+3] == 0xb3 )
			{
				// read in DWORD
				CurrentBfr = ( buffer[i+4] << 24 ) |
							 ( buffer[i+5] << 16 ) |
							 ( buffer[i+6] << 8  ) |
							 ( buffer[i+7] );
				BitsLeft = 32;

				horiz = Get_Bits(12);
				vert = Get_Bits(12);
				AR = Get_Bits(4); 
				fps = Get_Bits(4); 
				break;
			}
		}
	}

	frame_rate = Frame_Rates[fps];
	horiz_size = horiz;
	vert_size = vert;
	total_frames = (unsigned int)((endTime - startTime)*frame_rate);
	free(buffer);
	close(in);

	return true;
}

unsigned int CMPEGInfo::Get_Bits(unsigned int N)
{
		int Val = (CurrentBfr << (32 - BitsLeft)) >> (32 - N);
		BitsLeft -= N;
		return Val;
}

double CMPEGInfo::GetFrameRate()
{
	return frame_rate;
}

unsigned int CMPEGInfo::GetTotalFrames()
{
	return total_frames;
}

int CMPEGInfo::GetWidth()
{
	return horiz_size;
}

int CMPEGInfo::GetHeight()
{
	return vert_size;
}

////////////////////////////////////////////////////////////////////
// Experimental													  //
////////////////////////////////////////////////////////////////////

bool CMPEGInfo::LoadSlowMPEG2(const char *szPath)
{

	unsigned long tot_frames = 0;

	int in = open(szPath, _O_BINARY | _O_RDONLY);
	if ( in == -1 )
		return false;

	unsigned char *buffer = (unsigned char *) malloc(READINSIZE);
	
	int count = 0;
	int frame_count = 0;

	int readin = read(in, buffer, READINSIZE);

	if ( readin < 1 )
	{
		printf("The read Failed!\n");
		return false;
	}

	printf("\nTime Info\n---------\n");

	// Loop once to get start, then loop to get end
	
	while ( true )
	{
		for ( int i = 0; i != readin; i++ )
		{
			// Look for the GOP sequence 0 0 1 b8
			if ( buffer[i] == 0 && buffer[i+1] == 0 )
			{
				if ( buffer[i+2] == 1 && buffer[i+3] == 0xb8 )
				{
					// read in DWORD
					CurrentBfr = ( buffer[i+4] << 24 ) |
								 ( buffer[i+5] << 16 ) |
								 ( buffer[i+6] << 8  ) |
								 ( buffer[i+7] );
					BitsLeft = 32;

					int gop_hour;
					int gop_minute;
					int gop_sec;
					int gop_frame;

					int drop_flag;
					int closed_gop;
					int broken_link;

					drop_flag   = Get_Bits(1);
					gop_hour    = Get_Bits(5);
					gop_minute  = Get_Bits(6);
					BitsLeft -= 1; // marker bit
					gop_sec     = Get_Bits(6);
					gop_frame	= Get_Bits(6);
					closed_gop  = Get_Bits(1);
					broken_link = Get_Bits(1);

					tot_frames += gop_frame;
					printf("%d\n",gop_frame);
					// Checks there isnt any "bad" timecodes
				}
			}
		}

		// Start Time got, now get end time
		// Just got to find the last GOP & read the Time code
		readin = read(in, buffer, READINSIZE);
		if ( readin <= 0 )
			break;
	}

	lseek(in, 0, SEEK_SET);
	readin = read(in, buffer, READINSIZE);

	long double startTime, endTime=0, lastendTime=0;
	// Loop once to get start, then loop to get end
	for ( int j = 0; j != 2; j++ )
	{
		for ( int i = 0; i != readin; i++ )
		{
			if ( buffer[i] == 0 && buffer[i+1] == 0 )
			{
				if ( buffer[i+2] == 1 )// && buffer[i+3] == 0x1 )
				{
					// e0 
					if ( buffer[i+3] == 0xe0 )
					{
						// Skip two bytes for packet length
						unsigned char *ptr = &buffer[i+6];
						// read in DWORD
						CurrentBfr = ( ptr[0] << 24 ) |
									 ( ptr[1] << 16 ) |
									 ( ptr[2] << 8  ) |
									 ( ptr[3] );
						BitsLeft = 32;
						// skip 8 bits
						Get_Bits(8);
						int PTS_DTS_flag = Get_Bits(2);
						if ( PTS_DTS_flag == 2 || PTS_DTS_flag == 3 )
						{
							unsigned long pts1=0, pts2=0, pts3=0;
							// skip three bytes to read PTS
							ptr += 3;
							CurrentBfr = ( ptr[0] << 24 ) |
										 ( ptr[1] << 16 ) |
										 ( ptr[2] << 8  ) |
										 ( ptr[3] );
							BitsLeft = 32;
							// Skip '0010'
							Get_Bits(4);
							pts1 = Get_Bits(3);		// PTS [32..30]
							ptr += 1;
							CurrentBfr = ( ptr[0] << 24 ) |
										 ( ptr[1] << 16 ) |
										 ( ptr[2] << 8  ) |
										 ( ptr[3] );
							BitsLeft = 32;
							pts2 = Get_Bits(15);	// PTS [29..15]
							Get_Bits(1);			// marker bit
							pts3 = Get_Bits(15);	// PTS [14..0]

							unsigned long PTS;
							PTS = pts1 << 30;
							PTS |= pts2 << 15;
							PTS |= pts3;

							//printf("%u %Lf\n", PTS, ((long double)PTS) / 90000.0);
							if ( j == 0 )
							{
								startTime = ((long double)PTS) / 90000.0;
								break;
							}

							long double temp = ((long double)PTS) / 90000.0;
							if ( temp > endTime )
							{
								endTime = temp;
							}
							else
								lastendTime = temp;

							printf("PTS: %lu (%d)\n", PTS, PTS_DTS_flag);
						}
					}
				}
			}
		}

		// Start Time got, now get end time
		// Just got to find the last GOP & read the Time code
		lseek(in, -READINSIZE, SEEK_END);
		readin = read(in, buffer, READINSIZE);
	}



//	printf("Start Time: %02d:%02d:%02d\n", start_hour, start_min, start_sec);
//	printf("End Time:   %02d:%02d:%02d\n", end_hour, end_min, end_sec);
/*
	int SecLength = ((end_hour * 3600) + ( end_min * 60) + end_sec) -
					((start_hour * 3600) + ( start_min * 60) + start_sec);
*/
//	printf("\nDuration: %02d:%02d:%02d\n", HOURS(SecLength), MINS(SecLength), SECS(SecLength));

	// Now get Sequence header & find other info so now were searching for 0 0 01 b3
	int horiz, vert, fps, AR;
	lseek(in, 0, SEEK_SET);
	readin = read(in, buffer, READINSIZE);
	for ( int i = 0; i != readin; i++ )
	{
		// Look for the sequence header:- 0 0 1 b3
		if ( buffer[i] == 0 && buffer[i+1] == 0 )
		{
			if ( buffer[i+2] == 1 && buffer[i+3] == 0xb3 )
			{
				// read in DWORD
				CurrentBfr = ( buffer[i+4] << 24 ) |
							 ( buffer[i+5] << 16 ) |
							 ( buffer[i+6] << 8  ) |
							 ( buffer[i+7] );
				BitsLeft = 32;

				horiz = Get_Bits(12);
				vert = Get_Bits(12);
				AR = Get_Bits(4); 
				fps = Get_Bits(4); 
				break;
			}
		}
	}

/*	printf("\nMPEG-2 Info\n-----------\n");
	printf("Horizontal Size: %d\n", horiz);
	printf("Vertical Size: %d\n", vert);
	printf("Frame Rate: %f\n", Frame_Rates[fps]);
	printf("Aspect Ratio: %s\n", Aspect_Ratios[AR]);

	printf("Estimated number of frames: %d", (int)(((float)SecLength * Frame_Rates[fps])+0.5) );
*/
	frame_rate = Frame_Rates[fps];
	horiz_size = horiz;
	vert_size = vert;
	total_frames = (unsigned int)(((endTime+lastendTime - startTime)*frame_rate)+0.5);
	///dprintf("%d", total_frames);
	
 	//total_frames = (int)(((float)SecLength * Frame_Rates[fps])+0.5);//(endTime - startTime)*frame_rate;
	//printf("%d", endTime - startTime);
	// free mem, close file & bail
	free(buffer);
	close(in);
	return true;
}


bool CMPEGInfo::LoadMPEG24Info(const char *szPath)
{
	int in = open(szPath, _O_BINARY | _O_RDONLY);
	if ( in == -1 )
		return false;
#define LOADIN (1024*256)
	unsigned char *buffer = (unsigned char *) malloc(LOADIN);

	int readin = read(in, buffer, LOADIN);

	int horiz = 0, vert = 0 , fps = 0, AR = 0;

	for ( int i = 0; i != readin; i++ )
	{
		// Look for the sequence header:- 0 0 1 b3
		if ( buffer[i] == 0 && buffer[i+1] == 0 )
		{
			if ( buffer[i+2] == 1 && buffer[i+3] == 0xb3 )
			{
				// read in DWORD
				CurrentBfr = ( buffer[i+4] << 24 ) |
							 ( buffer[i+5] << 16 ) |
							 ( buffer[i+6] << 8  ) |
							 ( buffer[i+7] );
				BitsLeft = 32;

				horiz = Get_Bits(12);
				vert = Get_Bits(12);
				AR = Get_Bits(4); 
				fps = Get_Bits(4); 
				break;
			}
		}
	}

	// load in 10mb to find it if that didnt work
	if ( horiz == 0 )
	{
		free(buffer);
		buffer = (unsigned char *) malloc(READINSIZE*2);

		lseek(in, 0, SEEK_SET);
		readin = read(in, buffer, READINSIZE*2);

		for ( int i = 0; i != readin; i++ )
		{
			// Look for the sequence header:- 0 0 1 b3
			if ( buffer[i] == 0 && buffer[i+1] == 0 )
			{
				if ( buffer[i+2] == 1 && buffer[i+3] == 0xb3 )
				{
					// read in DWORD
					CurrentBfr = ( buffer[i+4] << 24 ) |
								 ( buffer[i+5] << 16 ) |
								 ( buffer[i+6] << 8  ) |
								 ( buffer[i+7] );
					BitsLeft = 32;

					horiz = Get_Bits(12);
					vert = Get_Bits(12);
					AR = Get_Bits(4); 
					fps = Get_Bits(4); 
					break;
				}
			}
		}
	}

	if ( horiz == 0 )
	{
		///dprintf("Major Read in Error! Couldn't find Sequence Header!!!!\n");
	}

	frame_rate = Frame_Rates[fps];
	horiz_size = horiz;
	vert_size = vert;

	strcpy(szAspectRatio, Aspect_Ratios[AR]);

	close(in);
	free(buffer);
	return true;
}

// ImageProcessingTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Profiler.h"
#include "ImageProcessing.h"

int cycleCount = 500;

bool SaveBMPFile(const char *filename, BYTE *pData, BITMAPINFOHEADER *bih)
{	
	BITMAPFILEHEADER header;
	size_t write;
	FILE *pFile;
	
	pFile = fopen(filename, "wb");
	if (pFile == NULL)
		return false;

	// Write the file header
	memset(&header, 0, sizeof(BITMAPFILEHEADER));
	header.bfType = 0x4D42;
	header.bfSize = bih->biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	header.bfOffBits = 54;
	write = fwrite(&header, sizeof(BITMAPFILEHEADER), 1, pFile);
	if (write == 0)
		return false;
	
	// Bitmap header
	write = fwrite(bih, sizeof(BITMAPINFOHEADER), 1, pFile);
	if (write == 0)
		return false;

	write = fwrite(pData, bih->biSizeImage, 1, pFile);

	fclose(pFile);

	return true;
}

bool LoadBMPFile(const char *filename, BYTE **ppData, BITMAPINFOHEADER *bih)
{	
	BITMAPFILEHEADER header;
	size_t read;
	int size;
	FILE *pFile;
	BYTE *pData;
	long pos;
	int i;	
	
	pFile = fopen(filename, "rb");
	if (pFile == NULL)
		return false;

	// Read the bmp header
	read = fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);
	if (read == 0)
		return false;

	size = (int)header.bfSize;

	// Bitmap header
	read = fread(bih, sizeof(BITMAPINFOHEADER), 1, pFile);
	if (read == 0)
		return false;

	bih->biHeight = abs(bih->biHeight);

	if (bih->biBitCount != 24) {
		printf("source_image.bmp needs to have a color depth of 24-bits, exiting... \n");
		return false;
	}
	bih->biBitCount = 32;
	bih->biSizeImage = bih->biHeight * bih->biWidth * 4;

	// Bitmap data
	size = size-sizeof(BITMAPINFOHEADER)-sizeof(BITMAPINFOHEADER);

	pData = new BYTE[bih->biSizeImage];
	*ppData = pData;
	
	pos = ftell(pFile);
	fseek(pFile, pos, SEEK_SET);
	
	for (i = 0; i < size; i += 3) {
		read = fread(pData, 3, 1, pFile);
		if (read == 0)
			return false;
		pData[3] = 0xFF;
		pData += 4;
	}

#if 0
	// Do each conversion
	{
		CProfile profile2("On-the-fly alpha set");
		for (i = 0; i < cycleCount; i++) {
			fseek(pFile, pos, SEEK_SET);
			pData = *ppData;
			for (i = 0; i < size; i += 3) {
				read = fread(pData, 3, 1, pFile);
				if (read == 0)
					return false;
				pData[3] = 0xFF;
				pData += 4;
			}
		}
	}
	{
		CProfile profile2("Two-part alpha set");
		for (i = 0; i < cycleCount; i++) {
			fseek(pFile, pos, SEEK_SET);
			pData = *ppData;
			for (i = 0; i < size; i += 3) {
				read = fread(pData, 3, 1, pFile);
				if (read == 0)
					return false;
				pData += 4;
			}
			ImageProcessing_RGB32_AlphaSet_MMX(*ppData, bih->biWidth, bih->biHeight, 0xFF);
		}
	}
#endif

	fclose(pFile);

	return true;
}

int main(int argc, char* argv[])
{
	bool bOverlayImageGood = false;
	int i = 0;	
	BYTE *pTargetImage = NULL;
	BYTE *pOverlayImage = NULL;
	BITMAPINFOHEADER bihTargetImage;
	BITMAPINFOHEADER bihOverlayImage;

	memset(&bihTargetImage, 0, sizeof(BITMAPINFOHEADER));
	memset(&bihOverlayImage, 0, sizeof(BITMAPINFOHEADER));

	printf("ImageProcessingTest - compile date " __DATE__ " - " __TIME__ " \n");		
	printf("  Loads source_image.bmp and overlay_image.bmp \n");
	printf("  files and times processing functions on them \n");	
	printf("\n");

	if (argc > 1) {
		cycleCount = atoi(argv[1]);
	}	

	if (!LoadBMPFile("source_image.bmp", &pTargetImage, &bihTargetImage)) {
		printf("Unable to load source_image.bmp, exiting... \n");
		return -1;
	}

	if (!LoadBMPFile("overlay_image.bmp", &pOverlayImage, &bihOverlayImage)) {
		printf("Unable to load overlay_image.bmp, overlay tests disabled. \n\n");		
		bOverlayImageGood = false;
	}

	
	if (bihTargetImage.biWidth == bihOverlayImage.biWidth
		&& bihTargetImage.biHeight == bihOverlayImage.biHeight
		&& bihTargetImage.biBitCount == bihOverlayImage.biBitCount
		&& pOverlayImage != NULL)
	{
		bOverlayImageGood = true;
	}

	printf("\t Timing Cycles : %i \n", cycleCount);	
	printf("\t Image Size: %i x %i \n", bihTargetImage.biWidth, bihTargetImage.biHeight);		
	printf("\t Using Overlay Image: %s \n", bOverlayImageGood ? "Yes" : "No" );		
	
	printf("\n");
	printf("Timing flipping routines... \n");


	printf("ImageProcessing_RGB32_Flip_C\n");
	{
		CProfile profile2("ImageProcessing_RGB32_Flip_C");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_Flip_C(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}
	printf("ImageProcessing_RGB32_Flip_ASM\n");
	{
		CProfile profile2("ImageProcessing_RGB32_Flip_ASM");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_Flip_ASM(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}

	printf("ImageProcessing_RGB32_Flip_MMX\n");
	{
		CProfile profile2("ImageProcessing_RGB32_Flip_MMX");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_Flip_MMX(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
			//SaveBMPFile("test.bmp", pTargetImage, &bihTargetImage);
		}
	}

	printf("\n");
	printf("Timing alpha setting routines... \n");
	
	printf("ImageProcessing_RGB32_AlphaSet_C\n");
	{
		CProfile profile2("ImageProcessing_RGB32_AlphaSet_C");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_AlphaSet_C(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight, 0xFF);
		}
	}
	printf("ImageProcessing_RGB32_AlphaSet8_C\n");
	{
		CProfile profile2("ImageProcessing_RGB32_AlphaSet8_C");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_AlphaSet8_C(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight, 0xFF);
		}
	}
	printf("ImageProcessing_RGB32_AlphaSet_ASM\n");
	{
		CProfile profile2("ImageProcessing_RGB32_AlphaSet_ASM");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_AlphaSet_ASM(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight, 0xFF);
		}
	}		
	printf("ImageProcessing_RGB32_AlphaSet_MMX\n");
	{
		CProfile profile2("ImageProcessing_RGB32_AlphaSet_MMX");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_AlphaSet_MMX(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight, 0xFF);
		}
	}
	printf("ImageProcessing_RGB32_AlphaSet16_MMX\n");
	{
		CProfile profile2("ImageProcessing_RGB32_AlphaSet16_MMX");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_AlphaSet16_MMX(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight, 0xFF);
		}
	}
	
	printf("\n");
	printf("Timing overlay routines... \n");
	
	printf("ImageProcessing_RGB32_Overlay_C\n");
	{
		CProfile profile2("ImageProcessing_RGB32_Overlay_C");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_Overlay_C(pTargetImage, pOverlayImage, bihTargetImage.biWidth, bihTargetImage.biHeight, 0xFFFFFFFF);
		}
	}
	printf("ImageProcessing_RGB32_Overlay_ASM\n");
	{
		CProfile profile2("ImageProcessing_RGB32_Overlay_ASM");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_Overlay_ASM(pTargetImage, pOverlayImage, bihTargetImage.biWidth, bihTargetImage.biHeight, 0xFFFFFFFF);
		}
	}
	printf("ImageProcessing_RGB32_Overlay_MMX\n");
	{
		CProfile profile2("ImageProcessing_RGB32_Overlay_MMX");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_RGB32_Overlay_MMX(pTargetImage, pOverlayImage, (int)bihTargetImage.biWidth, bihTargetImage.biHeight, 0xFFFFFFFF);
		}
	}

	printf("\n");
	printf("Timing complete. \n");
	printf("\n");

	if (pTargetImage != NULL)
		delete [] pTargetImage;

	if (pOverlayImage != NULL)
		delete [] pOverlayImage;

	printf("Timing results: \n");
	CProfiler::Report(cycleCount);
	printf("\n");

	system("pause");

	return 0;
}


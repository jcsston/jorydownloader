// ImageProcessingTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Profiler.h"
#include "ImageProcessing.h"

int cycleCount = 500;

bool LoadBMPFile(const char *filename, BYTE **ppData, BITMAPINFOHEADER *bih)
{	
	size_t read;
	char buffer[64];
	DWORD dwSize;
	int size;
	FILE *pFile;
	BYTE *pData;
	long pos;
	int i;	
	
	pFile = fopen(filename, "rb");
	if (pFile == NULL)
		return false;

	// Read the BM
	read = fread(buffer, 2, 1, pFile);
	if (read == 0)
		return false;

	// Total size of the bitmap
	read = fread(&dwSize, sizeof(DWORD), 1, pFile);
	if (read == 0)
		return false;
	size = (int)dwSize;

	// Not sure what these are
	read = fread(buffer, sizeof(DWORD), 1, pFile);
	read = fread(buffer, sizeof(DWORD), 1, pFile);
	
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

	// Bitmap data
	size = size-sizeof(BITMAPINFOHEADER)-2-(sizeof(DWORD)*3);

	pData = new BYTE[bih->biHeight * bih->biWidth * 4];
	*ppData = pData;
	
	pos = ftell(pFile);
	fseek(pFile, pos, SEEK_SET);
	
	for (i = 0; i < size; i += 3) {
		read = fread(pData, 3, 1, pFile);
		if (read == 0)
			return false;
		pData += 4;
	}

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
			ImageProcessing_MMXAlphaSet(*ppData, bih->biWidth, bih->biHeight);
		}
	}

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
	printf("Timing alpha setting routines... \n");

	printf("ImageProcessing_CAlphaSet\n");
	{
		CProfile profile2("ImageProcessing_CAlphaSet");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_CAlphaSet(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}
	printf("ImageProcessing_C8AlphaSet\n");
	{
		CProfile profile2("ImageProcessing_C8AlphaSet");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_C8AlphaSet(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}
	printf("ImageProcessing_ASMAlphaSet\n");
	{
		CProfile profile2("ImageProcessing_ASMAlphaSet");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_ASMAlphaSet(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}		
	printf("ImageProcessing_MMXAlphaSet\n");
	{
		CProfile profile2("ImageProcessing_MMXAlphaSet");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_MMXAlphaSet(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}
	printf("ImageProcessing_MMX16AlphaSet\n");
	{
		CProfile profile2("ImageProcessing_MMX16AlphaSet");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_MMX16AlphaSet(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}
/*
	printf("\n");
	printf("Timing flipping routines... \n");

	printf("ImageProcessing_CFlip\n");
	{
		CProfile profile2("ImageProcessing_CFlip");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_CFlip(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}
	printf("ImageProcessing_ASMFlip\n");
	{
		CProfile profile2("ImageProcessing_ASMFlip");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_ASMFlip(pTargetImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}

	printf("\n");
	printf("Timing overlay routines... \n");
	
	printf("ImageProcessing_COverlay\n");
	{
		CProfile profile2("ImageProcessing_COverlay");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_COverlay(pTargetImage, pOverlayImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}
	printf("ImageProcessing_ASMOverlay\n");
	{
		CProfile profile2("ImageProcessing_ASMOverlay");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_ASMOverlay(pTargetImage, pOverlayImage, bihTargetImage.biWidth, bihTargetImage.biHeight);
		}
	}
	printf("ImageProcessing_MMXOverlay\n");
	{
		CProfile profile2("ImageProcessing_MMXOverlay");
		for (i = 0; i < cycleCount; i++) {
			ImageProcessing_MMXOverlay(pTargetImage, pOverlayImage, (int)bihTargetImage.biWidth, (int)abs(bihTargetImage.biHeight));
		}
	}
*/
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


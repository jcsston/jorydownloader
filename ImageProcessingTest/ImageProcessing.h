///
/// Various RGB32 routines
///


#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

/// Takes about 0.0023 seconds for a 352x240 frame, compiler set to max speed
void ImageProcessing_CFlip(BYTE *image, int w, int h);

/// This function is slightly slower than CFlip in debug builds
/// but it's slightly faster in release builds (which are the ones that matter ;))
void ImageProcessing_ASMFlip(BYTE *image, int w, int h);

/// Takes about 0.002 seconds on a 352x240 frame with compiler set to max speed
void ImageProcessing_COverlay(BYTE *imageDest, BYTE *overlaySrc, int w, int h);

/// About 1.2x the speed of COverlay with compiler set to max speed
void ImageProcessing_ASMOverlay(BYTE *imageDest, BYTE *overlaySrc, int w, int h);

/// Majorly b0rked
/// Horridly slow 0.0592 seconds for a 352x240 frame
/// 0.0347x of the COverlay version
void ImageProcessing_MMXOverlay(BYTE *imageDest, BYTE *overlaySrc, int w, int h);

void ImageProcessing_CAlphaSet(BYTE *image, int w, int h);

// 8-mod width required
void ImageProcessing_C8AlphaSet(BYTE *image, int w, int h);

void ImageProcessing_ASMAlphaSet(BYTE *image, int w, int h);

// 8-mod width required
void ImageProcessing_MMXAlphaSet(BYTE *image, int w, int h);
// 16-mod width
void ImageProcessing_MMX16AlphaSet(BYTE *image, int w, int h);

#endif // _IMAGE_PROCESSING_H_

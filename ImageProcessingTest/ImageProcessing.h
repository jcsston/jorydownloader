///
/// Various RGB32 routines
///

#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

#include <assert.h>

/// Takes about 0.0023 seconds for a 352x240 frame, compiler set to max speed
void ImageProcessing_RGB32_Flip_C(BYTE *image, int w, int h);

/// This function is slightly slower than CFlip in debug builds
/// but it's slightly faster in release builds (which are the ones that matter ;))
void ImageProcessing_RGB32_Flip_ASM(BYTE *image, int w, int h);

/// Mode-8 Width required
void ImageProcessing_RGB32_Flip_MMX(BYTE *image, int w, int h);

/// Takes about 0.002 seconds on a 352x240 frame with compiler set to max speed
void ImageProcessing_RGB32_Overlay_C(BYTE *imageDest, BYTE *overlaySrc, int w, int h, int transparentColor);

/// About 1.2x the speed of COverlay with compiler set to max speed
void ImageProcessing_RGB32_Overlay_ASM(BYTE *imageDest, BYTE *overlaySrc, int w, int h, int transparentColor);

/// 8-Mod Width required
/// iSSE or better required (MASKMOVQ instruction used)
void ImageProcessing_RGB32_Overlay_MMX(BYTE *imageDest, BYTE *overlaySrc, int w, int h, int transparentColor);

void ImageProcessing_RGB32_AlphaSet_C(BYTE *image, int w, int h, int alpha);

/// 8-mod width required
void ImageProcessing_RGB32_AlphaSet8_C(BYTE *image, int w, int h, int alpha);

void ImageProcessing_RGB32_AlphaSet_ASM(BYTE *image, int w, int h, int alpha);

/// 8-mod width required
void ImageProcessing_RGB32_AlphaSet_MMX(BYTE *image, int w, int h, int alpha);
/// 16-mod width required
void ImageProcessing_RGB32_AlphaSet16_MMX(BYTE *image, int w, int h, int alpha);

#endif // _IMAGE_PROCESSING_H_

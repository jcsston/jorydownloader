#include "stdafx.h"

#include "ImageProcessing.h"

/// Takes about 0.0023 seconds for a 352x240 frame, compiler set to max speed
void ImageProcessing_RGB32_Flip_C(BYTE *image, int w, int h)
{
	assert(image != NULL);
	assert(w > 0 && h > 0);
	// Mod-2 Height Required
	assert(h % 2 == 0);

	COLORREF *pTopImage = (COLORREF *)image;
	COLORREF *pBottomImage = ((COLORREF *)image) + (w * (h - 1));
	
	int rowsize = w*4;
	BYTE *row = new BYTE[rowsize];
	
	h = h / 2;
	int y;
	for (y = 0; y < h; y++) {
		memcpy(row, pTopImage, rowsize);
		memcpy(pTopImage, pBottomImage, rowsize);
		memcpy(pBottomImage, row, rowsize);
		
		pTopImage += w;
		pBottomImage -= w;
	}

	delete [] row;
}

static void inline memcpy_local(void *pDst, const void *pSrc, size_t count)
{
	memcpy(pDst, pSrc, count);
}

void ImageProcessing_RGB32_Flip_ASM(BYTE *image, int w, int h)
{
	assert(image != NULL);
	assert(w > 0 && h > 0);
	// Mod-2 Height Required
	assert(h % 2 == 0);

	void *row = malloc(w*4);
	__asm
	{
		mov     ecx, [w]    ; width
		shl     ecx, 2      ; Multiply by 4		
		mov			eax, ecx		; Keep a copy of the rowsize, we'll need it
		mov			ebx, [h]		; height
		dec			ebx					; we start at the first pixel of the last row
		mul			ebx					; calc bottom addr offset
		mov			ebx, [row]		; put temp row addr to ebx reg
		mov			esi, [image]	; put top addr to esi reg		
		mov			edi, esi		; put bottom addr to edi reg
		add			edi, eax		; offset bottom addr
		mov			eax, ecx		; restore the rowsize

		mov			ecx, [h]		; put height to ecx reg
		shr     ecx, 1      ; Reduce by 2

		align		16
codeloop:		
		; preseve our loop count
		push		ecx
		
		; it may seem strange to call the memcpy function
		; however memcpy is written in asm and is already very fast
		; duping the code here wouldn't have much of a speed increase
		; call memcpy(row, pTopImage, rowsize);
		push		eax ; rowsize
		push		esi ; top image pointer
		push		ebx ; row temp pointer
		call		memcpy_local
		; restore our regs
		pop			ebx
		pop			esi
		pop			eax

		; call memcpy(pTopImage, pBottomImage, rowsize);
		push		eax ; rowsize
		push		edi ; bottom image pointer
		push		esi ; top image pointer
		call		memcpy_local
		; restore our regs
		pop			esi
		pop			edi
		pop			eax

		; call memcpy(pBottomImage, row, rowsize);
		push		eax ; rowsize
		push		ebx ; row temp image pointer
		push		edi ; bottom image pointer
		call		memcpy_local
		; restore our regs
		pop			edi
		pop			ebx
		pop			eax

		add			esi, eax				; move top pointer down to next row
		sub			edi, eax				; move bottom pointer up to previous row

		; get our preseved loop count
		pop			ecx
		dec			ecx					; decrement count by 1
		jnz			codeloop		; jump to codeloop if not Zero		
	}

	free(row);
}

void ImageProcessing_RGB32_Flip_MMX(BYTE *image, int w, int h)
{
	assert(image != NULL);
	assert(w > 0 && h > 0);
	// Mod-8 Width Required
	assert(w % 8 == 0);
	// Mod-2 Height Required
	assert(h % 2 == 0);

	__asm
	{
		mov     ecx, [w]    ; width
		shl     ecx, 2      ; Multiply by 4		
		mov			eax, ecx
		mov			edx, ecx				
		
		mov			ebx, [h]		; height
		dec			ebx					; we start at the first pixel of the last row
		mul			ebx					; calc bottom addr offset
		mov			esi, [image]	; put top addr to esi reg		
		mov			edi, esi		; put bottom addr to edi reg
		add			edi, eax		; offset bottom addr

		mov     eax, [w]    ; width
		mov			edx, eax
		shl     edx, 3      ; Multiply by 8
		shr     eax, 3      ; Divide by 8
		mov			ebx, eax

		mov			ecx, [h]		; put height to ecx reg
		shr     ecx, 1      ; Reduce by 2

		align		16
codeloop:		
		movq		mm0, [esi]
		movq		mm1, [esi+8]
		movq		mm2, [esi+16]
		movq		mm3, [esi+24]
		
		movq		mm4, [edi]
		movq		mm5, [edi+8]
		movq		mm6, [edi+16]
		movq		mm7, [edi+24]		

		movq		[edi], mm0
		movq		[edi+8], mm1
		movq		[edi+16], mm2
		movq		[edi+24], mm3

		movq		[esi], mm4
		movq		[esi+8], mm5
		movq		[esi+16], mm6
		movq		[esi+24], mm7

		add			esi, (4*8)
		add			edi, (4*8)
		
		dec			eax
		cmp			eax, 0
		jne			codeloop				; jump if we are not at the end of the row
		
		sub			edi, edx				; move the bottom pointer up 2 rows
		mov			eax, ebx				; reset the row position

		dec			ecx					; decrement count by 1
		jnz			codeloop		; jump if we have not processed all rows
		emms
	}
}

/// Takes about 0.002 seconds on a 352x240 frame with compiler set to max speed
void ImageProcessing_RGB32_Overlay_C(BYTE *imageDest, BYTE *overlaySrc, int w, int h, int transparentColor)
{
	assert(imageDest != NULL);
	assert(overlaySrc != NULL);
	assert(w > 0 && h > 0);

	COLORREF *pTarget = (COLORREF *)imageDest;
	COLORREF *pOverlay = (COLORREF *)overlaySrc;
	int y, x;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {	
			if (*pOverlay != (COLORREF)transparentColor) {
				*pTarget = *pOverlay;
			}
			pTarget++;
			pOverlay++;
		}
	}
}

/// About 1.2x the speed of COverlay with compiler set to max speed
void ImageProcessing_RGB32_Overlay_ASM(BYTE *imageDest, BYTE *overlaySrc, int w, int h, int transparentColor)
{
	assert(imageDest != NULL);
	assert(overlaySrc != NULL);
	assert(w > 0 && h > 0);

	// we assume all data in the register is not used by others
	__asm
	{
		// Assign pointers to register
		mov			esi, [overlaySrc]			; put src addr to esi reg
		mov			edi, [imageDest]			; put dest addr to edi reg
		mov			ecx, [w]		; put width to ecx reg
		mov			eax, [h]		; put height to ecx reg
		imul		ecx					; eax = eax * ecx
		mov			ecx, eax		; put count to ecx reg
		
		align 16
codeloop:
		mov			eax, dword ptr [esi] 
		cmp			eax, [transparentColor] 
		je			nooverlay
		mov			dword ptr [edi], eax 
		/*		
		mov			eax, dword ptr [esi] 
		inc			eax
		mov			ebx, dword ptr [edi]
		cmovo		ebx, dword ptr [esi]
		mov			dword ptr [edi], ebx

		add			esi, 4				; add src pointer by 4 bytes
		add			edi, 4				; add dest pointer by 4 bytes

		mov			eax, dword ptr [esi] 
		inc			eax
		mov			ebx, dword ptr [edi]
		cmovz		ebx, eax
		mov			dword ptr [edi], ebx

		add			esi, 4				; add src pointer by 4 bytes
		add			edi, 4				; add dest pointer by 4 bytes

		mov			eax, dword ptr [esi] 
		inc			eax
		mov			ebx, dword ptr [edi]
		cmovz		ebx, eax
		mov			dword ptr [edi], ebx

		add			esi, 4				; add src pointer by 4 bytes
		add			edi, 4				; add dest pointer by 4 bytes

		mov			eax, dword ptr [esi] 
		inc			eax
		mov			ebx, dword ptr [edi]
		cmovz		ebx, eax
		mov			dword ptr [edi], ebx		
		*/
nooverlay:		
		add			esi, 4				; add src pointer by 4 bytes
		add			edi, 4				; add dest pointer by 4 bytes

		dec			ecx					; decrement count by 1
		jnz			codeloop			; jump to codeloop if not Zero		
	}
}

/// Majorly b0rked
/// Horridly slow 0.0592 seconds for a 352x240 frame
/// 0.0347x of the COverlay version
void ImageProcessing_RGB32_Overlay_MMX(BYTE *imageDest, BYTE *overlaySrc, int w, int h, int transparentColor)
{
	assert(imageDest != NULL);
	assert(overlaySrc != NULL);
	assert(w > 0 && h > 0);
	// Mod-2 width required
	assert(w % 2 == 0);

	int iCount = w * h;
	int pTransparentColorData[2] = { transparentColor, transparentColor };

	// we assume all data in the register is not used by others
	__asm
	{
		// Assign pointers to register
		mov			esi, [overlaySrc]			; put src addr to esi reg
		mov			edi, [imageDest]			; put dest addr to edi reg
		mov			ecx, [iCount]		; put count to ecx reg
		shl			ecx, 2        ; multiple count with 4 by shifting 2 bits to the left
		shr			ecx, 3				; divide count with 8 by shifting 3 bits to right

		; We compare two pixels at a time
		; This increases speed, however the some white pixels get through
		movq		mm2, [pTransparentColorData]
		align 16 
codeloop:
		movq		mm0, [esi]			; mov 8 bytes of src data to mmx reg 0
		movq		mm1, [edi]			; mov 8 bytes of dest data to mmx reg 1
		PCMPEQD	mm0, mm2        ; compare and see if these two pixels are the transparent color
		; if they are, then the source pixels (mm0) will be set to zero, so the following add operation will
		; have null effect		
		PADDUSB	  mm0, mm1			  ; Add
		movq		[edi], mm0			; dump back the added 8 bytes of data to dest memory

; move to next two pixels
		add			esi, 8				; add src pointer by 8 bytes
		add			edi, 8				; add dest pointer by 8 bytes

		dec			ecx					; decrement count by 1
		jnz			codeloop			; jump to codeloop if not Zero
		emms							; Restore FPU state to normal

	}
}

void ImageProcessing_RGB32_AlphaSet_C(BYTE *image, int w, int h, int alpha)
{
	assert(image != NULL);
	assert(w > 0 && h > 0);

	int y, x;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {	
			image[3] = (BYTE)alpha;
			image += 4;
		}
	}	
}

/// Processed 8 pixels at a time
void ImageProcessing_RGB32_AlphaSet8_C(BYTE *image, int w, int h, int alpha)
{
	assert(image != NULL);
	assert(w > 0 && h > 0);
	// Mod-8 Width Required
	assert(w % 8 == 0);

	int y, x;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w/8; x++) {	
			image[3+(4*0)] = (BYTE)alpha;
			image[3+(4*1)] = (BYTE)alpha;
			image[3+(4*2)] = (BYTE)alpha;
			image[3+(4*3)] = (BYTE)alpha;
			image[3+(4*4)] = (BYTE)alpha;
			image[3+(4*5)] = (BYTE)alpha;
			image[3+(4*6)] = (BYTE)alpha;
			image[3+(4*7)] = (BYTE)alpha;
			image += 4*8;
		}
	}	
}

void ImageProcessing_RGB32_AlphaSet_ASM(BYTE *image, int w, int h, int alpha)
{
	assert(image != NULL);
	assert(w > 0 && h > 0);

	// we assume all data in the register is not used by others
	__asm
	{
		// Assign pointers to register
		mov			esi, [image]			; put src addr to esi reg
		mov			ecx, [w]		; put width to ecx reg
		mov			eax, [h]		; put height to ecx reg
		imul		ecx					; eax = eax * ecx
		mov			ecx, eax		; put count to ecx reg
		
		mov			edx, [alpha]
		shl			edx, 24
		align 16
codeloop:
		mov			eax, dword ptr [esi] 
		and			eax, edx 
		mov			dword ptr [esi], eax 

		; get ready for next pass
		add			esi, 4				; add src pointer by 4 bytes
		add			edi, 4				; add dest pointer by 4 bytes

		dec			ecx					; decrement count by 1
		jnz			codeloop			; jump to codeloop if not Zero		
	}
}

// 8-mod width required
void ImageProcessing_RGB32_AlphaSet_MMX(BYTE *image, int w, int h, int alpha)
{
	assert(image != NULL);
	assert(w > 0 && h > 0);
	// Mod-8 Width Required
	assert(w % 8 == 0);

	// we assume all data in the register is not used by others
	__asm
	{
		// Assign pointers to register
		mov			ecx, [w]		; put width to ecx reg
		mov			eax, [h]		; put height to ecx reg
		imul		ecx					; eax = eax * ecx
		mov			ecx, eax		; put count to ecx reg
		shr			ecx, 4				; divide count with 16 by shifting 4 bits to right
		
		; we do 8 pixels at a time
		mov			esi, [image]			; put src addr to esi reg
		mov			edi, [image]			; put src addr to edi reg
		mov			edx, [image]			; put src addr to edx reg
		mov			ebx, [image]			; put src addr to ebx reg		
		add			edi, 8
		add			edx, 16
		add			edi, 24
		add			ebx, 32
		
		; Load the alpha channel value
		mov			eax, [alpha]
		shl			eax, 24
		movd		mm3, eax
		movd		mm4, eax
		psll		mm4, 32
		por			mm4, mm3
		
		align 16 
codeloop:
		movq		mm0, [esi]			; mov 8 bytes of src data to mmx reg 0
		movq		mm1, [edi]			; mov 8 bytes of src data to mmx reg 1
		movq		mm2, [edx]			; mov 8 bytes of src data to mmx reg 2
		movq		mm3, [ebx]			; mov 8 bytes of src data to mmx reg 3
		
		; We AND 8*2 rgb32 pixels at a time
		por			mm0, mm4			; AND the image data with the set alpha
		por			mm1, mm4			; AND the image data with the set alpha
		por			mm2, mm4			; AND the image data with the set alpha
		por			mm3, mm4			; AND the image data with the set alpha
				
		movq		[esi], mm0			; mov the processed mmx reg 0
		movq		[edi], mm1			; mov the processed mmx reg 1
		movq		[edx], mm2			; mov the processed mmx reg 2
		movq		[ebx], mm3			; mov the processed mmx reg 3

		; get ready for next pass
		add			esi, 40
		add			edx, 40
		add			edi, 40
		add			ebx, 40

		dec			ecx					; decrement count by 1
		jnz			codeloop			; jump to codeloop if not Zero
		emms							; Restore FPU state to normal
	}
}

// 16-mod
void ImageProcessing_RGB32_AlphaSet16_MMX(BYTE *image, int w, int h, int alpha)
{
	assert(image != NULL);
	assert(w > 0 && h > 0);
	// Mod-16 Width Required
	assert(w % 16 == 0);

	// we assume all data in the register is not used by others
	__asm
	{
		// Assign pointers to register
		mov			ecx, [w]		; put width to ecx reg
		mov			eax, [h]		; put height to ecx reg
		imul		ecx					; eax = eax * ecx
		mov			ecx, eax		; put count to ecx reg
		shr			ecx, 5				; divide count with 32 by shifting 4 bits to right
		
		; we do 16 pixels per loop, or 8 pixels twice
		mov			esi, [image]			; put src addr to esi reg
		mov			edi, [image]			; put src addr to edi reg
		mov			edx, [image]			; put src addr to edx reg
		mov			ebx, [image]			; put src addr to ebx reg		
		add			edi, 8
		add			edx, 16
		add			edi, 24
		add			ebx, 32
		
		; The alpha channel value
		mov			eax, [alpha]
		shl			eax, 24
		movd		mm3, eax
		movd		mm4, eax
		psll		mm4, 32
		por			mm4, mm3
		
		align 16 
codeloop:
		movq		mm0, [esi]			; mov 8 bytes of src data to mmx reg 0
		movq		mm1, [edi]			; mov 8 bytes of src data to mmx reg 1
		movq		mm2, [edx]			; mov 8 bytes of src data to mmx reg 2
		movq		mm3, [ebx]			; mov 8 bytes of src data to mmx reg 3
		
		; We AND 8*2 rgb32 pixels at a time
		por			mm0, mm4			; AND the image data with the set alpha
		por			mm1, mm4			; AND the image data with the set alpha
		por			mm2, mm4			; AND the image data with the set alpha
		por			mm3, mm4			; AND the image data with the set alpha
				
		movq		[esi], mm0			; mov the processed mmx reg 0
		movq		[edi], mm1			; mov the processed mmx reg 1
		movq		[edx], mm2			; mov the processed mmx reg 2
		movq		[ebx], mm3			; mov the processed mmx reg 3

		; get ready for next pass
		add			esi, 40
		add			edx, 40
		add			edi, 40
		add			ebx, 40

		movq		mm0, [esi]			; mov 8 bytes of src data to mmx reg 0
		movq		mm1, [edi]			; mov 8 bytes of src data to mmx reg 1
		movq		mm2, [edx]			; mov 8 bytes of src data to mmx reg 2
		movq		mm3, [ebx]			; mov 8 bytes of src data to mmx reg 3
		
		; We AND 8*2 rgb32 pixels at a time
		por			mm0, mm4			; AND the image data with the set alpha
		por			mm1, mm4			; AND the image data with the set alpha
		por			mm2, mm4			; AND the image data with the set alpha
		por			mm3, mm4			; AND the image data with the set alpha
				
		movq		[esi], mm0			; mov the processed mmx reg 0
		movq		[edi], mm1			; mov the processed mmx reg 1
		movq		[edx], mm2			; mov the processed mmx reg 2
		movq		[ebx], mm3			; mov the processed mmx reg 3

		; get ready for next pass
		add			esi, 40
		add			edx, 40
		add			edi, 40
		add			ebx, 40

		dec			ecx					; decrement count by 1
		jnz			codeloop			; jump to codeloop if not Zero
		emms							; Restore FPU state to normal
	}
}

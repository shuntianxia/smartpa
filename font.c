#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include "font.h"

#define BMP_FILE_DIGIT    "res/digits1.png"
#define BMP_FILE_LETTER   "res/digits2.png"
#define BMP_FILE_SYMBOL   "res/digits3.png"
#define BMP_FILE_DDOT     "res/digits4.png"

#define  FONTFILE_PATH   "/usr/local/share/minigui/res/"

static BITMAP digit_bmp;
static BITMAP letter_bmp;
static BITMAP symbol_bmp;
static BITMAP ddot_bmp;

void create_font (void)
{   
    font18 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
        FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
        FONT_FLIP_NIL, FONT_OTHER_NIL,
        FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
        18, 0);
    font20 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
        FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
        FONT_FLIP_NIL, FONT_OTHER_NIL,
        FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
        20, 0);
    font22 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
        FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
        FONT_FLIP_NIL, FONT_OTHER_NIL,
        FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
        22, 0);
    font24 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
        FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
        FONT_FLIP_NIL, FONT_OTHER_NIL,
        FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
        24, 0);
     font26 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
        FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
        FONT_FLIP_NIL, FONT_OTHER_NIL,
        FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
        26, 0);
	 font28 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
		 FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
		 FONT_FLIP_NIL, FONT_OTHER_NIL,
		 FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
		 28, 0);
	 font30 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
		 FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
		 FONT_FLIP_NIL, FONT_OTHER_NIL,
		 FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
		 30, 0);
	 font36 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
		 FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
		 FONT_FLIP_NIL, FONT_OTHER_NIL,
		 FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
		 36, 0);
	 font40 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
		 FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
		 FONT_FLIP_NIL, FONT_OTHER_NIL,
		 FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
		 40, 0);
	 font46 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
		 FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
		 FONT_FLIP_NIL, FONT_OTHER_NIL,
		 FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
		 46, 0);
	 font160 = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "simfang", "UTF-8",
		 FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
		 FONT_FLIP_NIL, FONT_OTHER_NIL,
		 FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
		 160, 0);

#if 0
	if (LoadBitmap (HDC_SCREEN, &digit_bmp, BMP_FILE_DIGIT)) {
		fprintf (stderr, "Fail to load bitmap. \n");
		return ;
	}

	if (LoadBitmap (HDC_SCREEN, &letter_bmp, BMP_FILE_LETTER)) {
		fprintf (stderr, "Fail to load bitmap. \n");
		return ;
	}

	if (LoadBitmap (HDC_SCREEN, &symbol_bmp, BMP_FILE_SYMBOL)) {
		fprintf (stderr, "Fail to load bitmap. \n");
		return ;
	}

	if (LoadBitmap (HDC_SCREEN, &ddot_bmp, BMP_FILE_DDOT)) {
		fprintf (stderr, "Fail to load bitmap. \n");
		return ;
	}

	dev_font = CreateBMPDevFont ("bmp-led-rrncnn-10-15-ISO8859-1",
			&letter_bmp, "A", 6, 10);
	AddGlyphsToBMPFont (dev_font, &digit_bmp, "0", 10, 10);
	AddGlyphsToBMPFont (dev_font, &symbol_bmp, "+", 4, 10);
	AddGlyphsToBMPFont (dev_font, &ddot_bmp, ":", 1, 10);

	logfont_bmp = CreateLogFont (FONT_TYPE_NAME_BITMAP_BMP, "led",
			"ISO8859-1",
			FONT_WEIGHT_BOLD, FONT_SLANT_ITALIC,
			FONT_SETWIDTH_NORMAL, FONT_SPACING_CHARCELL,
			FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
			10, 0);
#endif
}

void destroy_font (void)
{
    DestroyLogFont (font18);
    DestroyLogFont (font20);
    DestroyLogFont (font22);
    DestroyLogFont (font24);
    DestroyLogFont (font26);
    DestroyLogFont (font28);
    DestroyLogFont (font30);
    DestroyLogFont (font36);
    DestroyLogFont (font40);
    DestroyLogFont (font46);
    DestroyLogFont (font160);
#if 0
	UnloadBitmap (&digit_bmp);
	UnloadBitmap (&letter_bmp);
	UnloadBitmap (&symbol_bmp);
	UnloadBitmap (&ddot_bmp);
	if (dev_font != NULL)
		DestroyBMPFont (dev_font);
	DestroyLogFont (logfont_bmp);
#endif
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

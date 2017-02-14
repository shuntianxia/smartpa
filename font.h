#ifndef	FONT_H
#define	FONT_H

//#include "defs.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

LOGFONT *logfont_bmp; 
DEVFONT *dev_font;
DEVFONT* devfont[68];

PLOGFONT font18, font20, font22, font24, font26, font28, font30, font36, font40, font46, font160;

void create_font (void);
void destroy_font (void);
#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif


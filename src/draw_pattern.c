/*
 * draw_pattern.c
 *
 * Copyright (c) 2021 Phillip Stevens
 * Create Time: July 2021
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted,free of charge,to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),to deal
 * in the Software without restriction,including without limitation the rights
 * to use,copy,modify,merge,publish,distribute,sublicense,and/or sell
 * copies of the Software,and to permit persons to whom the Software is
 * furnished to do so,subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS",WITHOUT WARRANTY OF ANY KIND,EXPRESS OR
 * IMPLIED,INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,DAMAGES OR OTHER
 * LIABILITY,WHETHER IN AN ACTION OF CONTRACT,TORT OR OTHERWISE,ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __AVR
#include <avr/pgmspace.h>
#endif

#include "ReGIS.h"

/****************************************************************************/
/***       Functions                                                      ***/
/****************************************************************************/

/* Set writing pattern */
void draw_pattern(window_t * win, w_pattern_t pattern)
{
#ifdef __AVR
    switch (pattern)
    {
        case _P0: fputs_P(PSTR("W(P0)"), win->fp); break;
        case _P1: fputs_P(PSTR("W(P1)"), win->fp); break;
        case _P2: fputs_P(PSTR("W(P2)"), win->fp); break;
        case _P3: fputs_P(PSTR("W(P3)"), win->fp); break;
        case _P4: fputs_P(PSTR("W(P4)"), win->fp); break;
        case _P5: fputs_P(PSTR("W(P5)"), win->fp); break;
        case _P6: fputs_P(PSTR("W(P6)"), win->fp); break;
        case _P7: fputs_P(PSTR("W(P7)"), win->fp); break;
        case _P8: fputs_P(PSTR("W(P8)"), win->fp); break;
        case _P9: fputs_P(PSTR("W(P9)"), win->fp); break;
    }
#else
    switch (pattern)
    {
        case _P0: fputs("W(P0)", win->fp); break;
        case _P1: fputs("W(P1)", win->fp); break;
        case _P2: fputs("W(P2)", win->fp); break;
        case _P3: fputs("W(P3)", win->fp); break;
        case _P4: fputs("W(P4)", win->fp); break;
        case _P5: fputs("W(P5)", win->fp); break;
        case _P6: fputs("W(P6)", win->fp); break;
        case _P7: fputs("W(P7)", win->fp); break;
        case _P8: fputs("W(P8)", win->fp); break;
        case _P9: fputs("W(P9)", win->fp); break;
    }
#endif
}

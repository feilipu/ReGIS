/*
 * draw_mode.c
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

/* Set writing mode */
void draw_mode(window_t * win, w_mode_t mode)
{
#ifdef __AVR
    switch (mode)
    {
        case _REP: fputs_P(PSTR("W(R)"), win->fp); break;
        case _ERA: fputs_P(PSTR("W(E)"), win->fp); break;
        case _OVL: fputs_P(PSTR("W(V)"), win->fp); break;
        case _CPL: fputs_P(PSTR("W(C)"), win->fp); break;
    }
#else
    switch (mode)
    {
        case _REP: fputs("W(R)", win->fp); break;
        case _ERA: fputs("W(E)", win->fp); break;
        case _OVL: fputs("W(V)", win->fp); break;
        case _CPL: fputs("W(C)", win->fp); break;
    }
#endif
}

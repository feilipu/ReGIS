/*
 * draw_intensity.c
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
/***       Private Functions                                              ***/
/****************************************************************************/

extern void appendstring(window_t * win, char const * text);

/****************************************************************************/
/***       Functions                                                      ***/
/****************************************************************************/

/* Set writing intensity (colour) */
void draw_intensity(window_t * win, w_intensity_t intensity)
{
    char s[8];

#ifdef __AVR
switch (intensity)
    {
        case _D: sprintf_P(s,PSTR("W(I(D))")); break;
        case _B: sprintf_P(s,PSTR("W(I(B))")); break;
        case _R: sprintf_P(s,PSTR("W(I(R))")); break;
        case _M: sprintf_P(s,PSTR("W(I(M))")); break;
        case _G: sprintf_P(s,PSTR("W(I(G))")); break;
        case _C: sprintf_P(s,PSTR("W(I(C))")); break;
        case _Y: sprintf_P(s,PSTR("W(I(Y))")); break;
        case _W: sprintf_P(s,PSTR("W(I(W))")); break;
    }
#else
    switch (intensity)
    {
        case _D: sprintf(s,"W(I(D))"); break;
        case _B: sprintf(s,"W(I(B))"); break;
        case _R: sprintf(s,"W(I(R))"); break;
        case _M: sprintf(s,"W(I(M))"); break;
        case _G: sprintf(s,"W(I(G))"); break;
        case _C: sprintf(s,"W(I(C))"); break;
        case _Y: sprintf(s,"W(I(Y))"); break;
        case _W: sprintf(s,"W(I(W))"); break;
    }
#endif

    appendstring(win, s);
}

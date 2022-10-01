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

#include "ReGIS.h"

/****************************************************************************/
/***       Private Functions                                              ***/
/****************************************************************************/

extern void appendstring(window_t * win, char * text);

/****************************************************************************/
/***       Functions                                                      ***/
/****************************************************************************/

/* Set writing pattern */
void draw_pattern(window_t * win, pattern_t pattern)
{
    char s[6];

    switch (pattern)
    {
        case _P0: sprintf(s,"W(P0)"); break;
        case _P1: sprintf(s,"W(P1)"); break;
        case _P2: sprintf(s,"W(P2)"); break;
        case _P3: sprintf(s,"W(P3)"); break;
        case _P4: sprintf(s,"W(P4)"); break;
        case _P5: sprintf(s,"W(P5)"); break;
        case _P6: sprintf(s,"W(P6)"); break;
        case _P7: sprintf(s,"W(P7)"); break;
        case _P8: sprintf(s,"W(P8)"); break;
        case _P9: sprintf(s,"W(P9)"); break;
    }

    appendstring(win, s);
}


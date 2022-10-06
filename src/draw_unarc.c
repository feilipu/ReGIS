/*
 * draw_unarc.c
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

/* Erase an arc (circle) in anticlockwise degrees (0 - 360), centred on current position */
void draw_unarc(window_t * win, uint16_t radius, int16_t arc)
{
    char s[20];

#ifdef __AVR
    sprintf_P(s, PSTR("C(W(E))(A%+.3d)[%+.3d]"), arc, radius);
#else
    sprintf(s, "C(W(E))(A%+.3d)[%+.3d]", arc, radius);
#endif

    appendstring(win, s);
}

/*
 * window_new.c
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

/* Open a graphics window, in graphics mode, and inititialise graphics */
uint8_t window_new(window_t * win, uint16_t width, uint16_t height, FILE * fp)
{
    if(win != NULL)
    {
        if (fp != NULL) win->fp = fp; else return 0;
        if (width && width < WIDTH_MAX) win->width = width; else win->width = WIDTH_MAX-1;
        if (height && height < HEIGHT_MAX) win->height = height; else win->height = HEIGHT_MAX-1;
#ifdef __AVR
        fprintf_P(win->fp, PSTR("%cP1p"), ASCII_ESC);
#else
        fprintf(win->fp, "%cP1p", ASCII_ESC);
#endif
        return 1;
    }
    else
    {
        return 0;
    }
}

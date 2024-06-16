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

#include "Arduino.h"

/****************************************************************************/
/***        STDIO Implementation                                          ***/
/****************************************************************************/

#ifdef __cplusplus
    extern "C" {
#endif

// Function that printf and related will use to print
static int serial_putchar(char c, FILE *f)
{
    (void)(f);
    Serial.write(c);
    return 1;
}

// Function that scanf and related will use to read
static int serial_getchar(FILE *)
{
    // Wait until character is avilable
    while(Serial.available() <= 0) {}

    return Serial.read();
}

void stdio_init(void)
{
    static FILE serial_stdinout = { .buf = NULL, .unget = 0, .flags = _FDEV_SETUP_RW, .size = 0, .len = 0, .put = serial_putchar, .get = serial_getchar, .udata = 0 };

    // Set up stdout and stdin
    stdout = &serial_stdinout;
    stdin  = &serial_stdinout;
    stderr = &serial_stdinout;
}

#ifdef __cplusplus
}
#endif

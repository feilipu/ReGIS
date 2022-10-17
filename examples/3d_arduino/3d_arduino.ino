/*
 * 3D animated graphics over the REGIS protocol for Arduino systems
 *
 * Copyright (c) 2022 Phillip Stevens
 *
 * Derived from original C++ code by:
 *
 * Copyright (C) 2021 Adam Williams <broadcast at earthling dot net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*
 * 3D homogeneous coordinate definition
 * https://en.wikipedia.org/wiki/Homogeneous_coordinates
 *
 * 3D Clipping in Homogeneous Coordinates
 * https://chaosinmotion.com/2016/05/22/3d-clipping-in-homogeneous-coordinates/
 *
 * project 3D coords onto 2D screen
 * https://stackoverflow.com/questions/724219/how-to-convert-a-3d-point-into-2d-perspective-projection
 *
 * transformation matrix
 * https://www.tutorialspoint.com/computer_graphics/3d_transformation.htm
 *
 */

// display using XTerm & picocom
// xterm +u8 -geometry 132x50 -ti 340 -tn 340 -e picocom -b 115200 -p 2 -f h /dev/ttyUSB0

// these are the demonstration options
#define CUBE '1'
#define ICOS '2'
#define GEAR '3'        // not working on UNO, insufficient RAM
#define GLXGEARS '4'    // not working on UNO, insufficient RAM

uint8_t demo = ICOS;    // select a demonstration from above options

uint8_t animate = 1;

float user_rotx = 0;
float user_roty = 0;

#define W 480
#define H 480
#define NEAR -100.0
#define FAR 100.0
#define FOV 3.0         // degrees
#define FPS 15          // max FPS

#ifndef __AVR

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#define PROGMEM

#endif // !__AVR

#include <ReGIS.h>      // ReGIS library
#include <3d.h>         // 3D library

#include "models.h"

float half_width;
float half_height;

// create the matrix which transforms from 3D to 2D
matrix_t projection_matrix;

// set up the display window for REGIS library
window_t my_window;


void clear_screen(void)
{
    Serial.write("\x1B[2J");
}


void begin_projection()
{
    half_width = (float)WIDTH_MAX * 0.5;
    half_height = (float)HEIGHT_MAX * 0.5;
    projection_opengl_m(&projection_matrix, FOV * (M_PI / 180.0), (float)W/(float)H, NEAR, FAR);    // or
//  projection_w3woody_m(&projection_matrix, FOV * (M_PI / 180.0), (float)W/(float)H, NEAR, FAR);
}


#ifndef __AVR

void read_point(point_t * point, unsigned char ** ptr)
{
    memcpy((uint8_t *)point, *ptr, sizeof(point_t));
    (*ptr) += sizeof(point_t);


void manage_fps(void)
{
    static struct timespec start_time = { 0 };
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    int diff = current_time.tv_sec * 1000 + current_time.tv_nsec / 1e6 -
        start_time.tv_sec * 1000 - start_time.tv_nsec / 1e6;
    start_time = current_time;
    if(diff < 1000 / FPS)
    {
        usleep((1000 / FPS - diff) * 1000);
    }
}

#else // !__AVR

// read a point from atmega flash
void read_point(point_t * point, unsigned char ** ptr)
{
    memcpy_P((uint8_t *)point, *ptr, sizeof(point_t));
    (*ptr) += sizeof(point_t);
}

#define manage_fps()

#endif // !__AVR


// draw the model
void regis_plot(const point_t *model, uint16_t count, matrix_t * transform, w_intensity_t intensity, uint8_t do_init)
{
    if(do_init)
    {
        window_new(&my_window, H, W);
        window_clear(&my_window);
    }

    draw_intensity(&my_window, intensity);

    unsigned char *ptr = (unsigned char*)model;

    for(uint16_t i = 0; i < count; ++i)
    {
        point_t point;
        vector_t vertex;

        read_point(&point, &ptr);

        vertex.x = point.x;
        vertex.y = point.y;
        vertex.z = point.z;
        vertex.w = 1.0;

        mult_v(&vertex,transform);

        scale_v(&vertex, 1.0/(vertex.w));

/* TODO: Clipping here */

        vertex.x = (vertex.x * (float)W) / (vertex.w * 2.0) + half_width;
        vertex.y = (vertex.y * (float)H) / (vertex.w * 2.0) + half_height;

        if(point.begin_poly)
        {
            draw_abs(&my_window, (uint16_t)vertex.x, (uint16_t)vertex.y);
        }
        else
        {
            draw_line_abs(&my_window, (uint16_t)vertex.x, (uint16_t)vertex.y);
        }
    }

    if(do_init)
    {
        window_write(&my_window);
        window_close(&my_window);
    }
}


void glxgears_loop()
{
    static float rotz = 0;
    static float roty = 30.0 / 180 * M_PI;
    static float step = -1.0 / 180 * M_PI;

    matrix_t view_transform;
    matrix_t transform;

    window_new(&my_window, H, W);
    window_clear(&my_window);

    identity_m(&view_transform);
    if(user_rotx != 0) rotx_m(&view_transform, user_rotx);
    if(user_roty != 0) roty_m(&view_transform, user_roty);
    translate_m(&view_transform, 0, 1.0, 20.0);      // view transform

    identity_m(&transform);
    rotz_m(&transform, rotz);
    translate_m(&transform, -1.0, 2.0, 0);
    roty_m(&transform, roty);
//  rotx_m(&transform, 0.0 / 180 * M_PI);
    mult_m(&transform, &view_transform);
    mult_m(&transform, &projection_matrix);

    regis_plot(glxgear1, sizeof(glxgear1) / sizeof(point_t), &transform, _R, 0);

    window_write(&my_window);
    window_reset(&my_window);

    identity_m(&transform);
    rotz_m(&transform, -2.0 * rotz + 9.0 / 180 * M_PI);
    translate_m(&transform, 5.2, 2.0, 0);
    roty_m(&transform, roty);
//  rotx_m(&transform, 0.0 / 180 * M_PI);
    mult_m(&transform, &view_transform);
    mult_m(&transform, &projection_matrix);

    regis_plot(glxgear2, sizeof(glxgear2) / sizeof(point_t), &transform, _G, 0);

    window_write(&my_window);
    window_reset(&my_window);

    identity_m(&transform);
    rotz_m(&transform, -2.0 * rotz + 30.0 / 180 * M_PI);
    translate_m(&transform, -1.1, -4.2, 0);
    roty_m(&transform, roty);
//  rotx_m(&transform, 0.0 / 180 * M_PI);
    mult_m(&transform, &view_transform);
    mult_m(&transform, &projection_matrix);

    regis_plot(glxgear3, sizeof(glxgear3) / sizeof(point_t), &transform, _B, 0);

    window_write(&my_window);
    window_close(&my_window);

    if(animate)
    {
        rotz += 2.0 / 180 * M_PI;
        roty += step;
        if((step > 0 && roty >= 45.0 / 180 * M_PI) ||
            (step < 0 && roty <= -45.0 / 180 * M_PI))
        {
            step = -step;
        }
    }

    manage_fps();
}


void gear_loop()
{
    static float rotz = 0;
    static float roty = 0;
    static float step2 = 1.0 / 180 * M_PI;

    matrix_t transform;

    identity_m(&transform);
    rotz_m(&transform, rotz);
    roty_m(&transform, roty);
    if(user_rotx != 0) rotx_m(&transform, user_rotx);
    if(user_roty != 0) roty_m(&transform, user_roty);
    translate_m(&transform, 0, 0, 8.0);
    mult_m(&transform, &projection_matrix);

    regis_plot(gear, sizeof(gear) / sizeof(point_t), &transform, _W, 1);

    if(animate)
    {
        rotz += 2.0 / 360 * M_PI * 2;
        roty += step2;
        if((step2 > 0 && roty >= 45.0 / 180 * M_PI) ||
            (step2 < 0 && roty <= -45.0 / 180 * M_PI))
        {
            step2 = -step2;
        }
    }

    manage_fps();
}


void icos_loop(void)
{
    static float rotz = 0;
    static float roty = 0;

    matrix_t transform;

    identity_m(&transform);
    rotz_m(&transform, rotz);
    roty_m(&transform, roty);
    rotx_m(&transform, M_PI/2);
    if(user_rotx != 0) rotx_m(&transform, user_rotx);
    if(user_roty != 0) roty_m(&transform, user_roty);
    translate_m(&transform, 0, 0, 8.0);
    mult_m(&transform, &projection_matrix);

    regis_plot(icos, sizeof(icos) / sizeof(point_t), &transform, _W, 1);

    if(animate)
    {
        rotz += 0.25 / 360 * M_PI * 2;
        roty += 2.0 / 360 * M_PI * 2;
    }

    manage_fps();
}


void cube_loop(void)
{
    static float rotz = 0;
    static float roty = 0;

    matrix_t transform;

    identity_m(&transform);
    rotz_m(&transform, rotz);
    roty_m(&transform, roty);
    if(user_rotx != 0) rotx_m(&transform, user_rotx);
    if(user_roty != 0) roty_m(&transform, user_roty);
    translate_m(&transform, 0, 0, 10.0);
    mult_m(&transform, &projection_matrix);

    regis_plot(cube, sizeof(cube) / sizeof(point_t), &transform, _W, 1);

    if(animate)
    {
        rotz += 2.0 / 360 * M_PI * 2;
        roty += 0.5 / 360 * M_PI * 2;
    }

    manage_fps();
}


void setup() {
    // initialize serial communication at 115200 bits per second:
    Serial.begin(115200);

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
    }
    
    clear_screen();
    begin_projection();
}

void loop() {
    switch(demo)
    {
        case CUBE:
            cube_loop();
            break;
        case ICOS:
            icos_loop();
            break;
        case GEAR:
            gear_loop();
            break;
        case GLXGEARS:
            glxgears_loop();
            break;
        default:
            exit(0);
    }
}

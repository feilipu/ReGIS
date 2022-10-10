/*
 * 3D animated graphics over the REGIS protocol
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

// project 3D coords onto 2D screen:
// https://stackoverflow.com/questions/724219/how-to-convert-a-3d-point-into-2d-perspective-projection

// transformation matrix:
// https://www.tutorialspoint.com/computer_graphics/3d_transformation.htm

// to set the terminal resolution:
// echo 'xterm*regisScreenSize:640x480' | xrdb -merge
// echo 'xterm*regisScreenSize' | xrdb -remove
// start new xterm after xrdb


// these are the demonstration options
#define CUBE 0
#define ICOS 1
#define GEAR 2 // not working fully on UNO, insufficient RAM
#define GLXGEARS 3 // not working on UNO, insufficient RAM

// select a demonstration from above options
int demo = ICOS;

int animate = 1;
float user_rotx = 0;
float user_roty = 0;

#define W 480
#define H 480
#define NEAR -100
#define FAR 100
#define FOV 3.0 // degrees
#define FPS 15 // max FPS

#ifndef __AVR

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>

#define PROGMEM

class Serial_
{
public:
    static void begin(int x)
    {
	    struct termios info;
	    tcgetattr(fileno(stdin), &info);
	    info.c_lflag &= ~ICANON;
	    info.c_lflag &= ~ECHO;
	    tcsetattr(fileno(stdin), TCSANOW, &info);
    }

    static void print(const char *x)
    {
        printf("%s", x);
        fflush(stdout);
    }

    static void print(int x)
    {
        printf("%d", x);
        fflush(stdout);
    }

    static int available()
    {
        fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(fileno(stdin), &rfds);
		FD_SET(0, &rfds);
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		int result = select(fileno(stdin) + 1,
			&rfds,
			0,
			0,
			&timeout);

		if(FD_ISSET(fileno(stdin), &rfds))
        {
            return 1;
        }
        return 0;
    }

    static int read()
    {
        int c = getc(stdin);
        return c;
    }

};

Serial_ Serial;

#endif // !__AVR

#include "models.h"

class Vector
{
public:
    float x, y, z, w;

    Vector() : x(0),y(0),z(0),w(1){}
    Vector(float a, float b, float c) :
        x(a),y(b),z(c),w(1){}
    Vector(float a, float b, float c, float d) :
        x(a),y(b),z(c),w(d){}

// scale all but w by m
    void scale(float m)
    {
        x = x * m;
        y = y * m;
        z = z * m;
    }
};

#define MATRIX_SIZE 16

class Matrix
{
public:
    Matrix()
    {
        clear();
    }

    void clear()
    {
        for(uint8_t i = 0; i < MATRIX_SIZE; ++i)
        {
            data[i] = 0;
        }
    }

#ifndef __AVR

    void dump()
    {
        for(int i = 0; i < 4; i++)
        {
            printf("%f %f %f %f\n",
                data[i * 4 + 0],
                data[i * 4 + 1],
                data[i * 4 + 2],
                data[i * 4 + 3]);
        }
    }

#endif

    void identity()
    {
        for(uint8_t i = 0; i < MATRIX_SIZE; ++i)
        {
            data[i] = 0;
        }
        data[0] = data[5] = data[10] = data[15] = 1.0f;
    }

    static Matrix get_rx(float angle)
    {
        float cos_angle = cos(angle);
        float sin_angle = sin(angle);
        Matrix result;

        result.identity();
        result.data[5] = cos_angle;
        result.data[6] = -sin_angle;
        result.data[9] = sin_angle;
        result.data[10] = cos_angle;
        return result;
    }

    static Matrix get_ry(float angle)
    {
        float cos_angle = cos(angle);
        float sin_angle = sin(angle);
        Matrix result;

        result.identity();
        result.data[0] = cos_angle;
        result.data[2] = sin_angle;
        result.data[8] = -sin_angle;
        result.data[10] = cos_angle;
        return result;
    }

    static Matrix get_rz(float angle)
    {
        float cos_angle = cos(angle);
        float sin_angle = sin(angle);
        Matrix result;

        result.identity();
        result.data[0] = cos_angle;
        result.data[1] = -sin_angle;
        result.data[4] = sin_angle;
        result.data[5] = cos_angle;
        return result;
    }

    static Matrix get_translate(float x, float y, float z)
    {
        Matrix result;

        result.identity();
        result.data[12] = x; // Tx
        result.data[13] = y; // Ty
        result.data[14] = z; // Tz
        return result;
    }

    Matrix operator*(const Matrix& m)
    {
        Matrix dst;
        int col;
        for(int y = 0; y < 4; ++y){
            col = y * 4;
            for(int x = 0; x < 4; ++x)
            {
                for(int i = 0; i < 4; ++i)
                {
                    dst.data[col + x] += data[col + i] * m.data[i * 4 + x];
                }
            }
        }
        return dst;
    }
    Matrix& operator*=(const Matrix& m)
    {
        *this = (*this) * m;
        return *this;
    }

    /* The interesting stuff */
    void SetupClipMatrix(float fov, float aspectRatio, float near, float far)
    {
        identity();
        float f = 1.0f / tan(fov * 0.5f);
        data[0] = f * aspectRatio;
        data[5] = f;
        data[10] = (far + near) / (far - near);
        data[11] = 1.0f; /* this 'plugs' the old z into w */
        data[14] = (2.0f * near * far) / (near - far);
        data[15] = 0.0f;
    }

    float data[MATRIX_SIZE];
};


// Must declare function prototype with user class to compile in Arduino
inline Vector operator*(const Vector& v, const Matrix& m);
inline Vector operator*(const Vector& v, const Matrix& m)
{
    Vector dst;
    dst.x = v.x*m.data[0] + v.y*m.data[4] + v.z*m.data[8 ] + v.w*m.data[12];
    dst.y = v.x*m.data[1] + v.y*m.data[5] + v.z*m.data[9 ] + v.w*m.data[13];
    dst.z = v.x*m.data[2] + v.y*m.data[6] + v.z*m.data[10] + v.w*m.data[14];
    dst.w = v.x*m.data[3] + v.y*m.data[7] + v.z*m.data[11] + v.w*m.data[15];
    return dst;
}

// REGIS library
#include <ReGIS.h>

window_t mywindow;

// VT100 codes
void clear_screen()
{
    Serial.print("\e[2J");
}

// create the matrix which transforms from 3D to 2D
Matrix clipMatrix;
float halfWidth;
float halfHeight;

void begin_projection()
{
    float aspect = (float)W / (float)H;
    halfWidth = (float)WIDTH_MAX * 0.5f;
    halfHeight = (float)HEIGHT_MAX * 0.5f;
    clipMatrix.SetupClipMatrix(FOV * (M_PI / 180.0f), aspect, NEAR, FAR);
}

#ifndef __AVR

point_t read_point(unsigned char **ptr)
{
    point_t result;
    unsigned char *buffer = (unsigned char*)&result;
    memcpy(buffer, *ptr, sizeof(point_t));
    (*ptr) += sizeof(point_t);
    return result;
}

void manage_fps()
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
point_t read_point(unsigned char **ptr)
{
    point_t result;
    unsigned char *buffer = (unsigned char*)&result;
    for(unsigned int j = 0; j < sizeof(point_t); j++)
    {
        *buffer++ = pgm_read_byte(*ptr);
        (*ptr)++;
    }
    return result;
}

#endif // !__AVR


// draw the model
// Must declare function prototype with user class to compile in Arduino
void regis_plot(const point_t *model, int count, Matrix transform, intensity_t intensity, int do_init);

void regis_plot(const point_t *model, int count, Matrix transform, intensity_t intensity, int do_init)
{
    if(do_init)
    {
        window_new( &mywindow, H, W );
        window_clear ( &mywindow );
    }

    draw_intensity( &mywindow, intensity );

    unsigned char *ptr = (unsigned char*)model;
    for(int i = 0; i < count; i++)
    {
        point_t point = read_point(&ptr);
        Vector vertex1 = Vector(point.x,
            point.y,
            point.z);
        Vector vertex2 = vertex1 * transform;
        vertex2.scale(1/(vertex2.w));
/* TODO: Clipping here */
        vertex2.x = (vertex2.x * (float)W) / (2.0f * vertex2.w) + halfWidth;
        vertex2.y = (vertex2.y * (float)H) / (2.0f * vertex2.w) + halfHeight;
        if(point.begin_poly)
        {
            draw_abs( &mywindow, (uint16_t)vertex2.x, (uint16_t)vertex2.y );
        }
        else
        {
            draw_line_abs( &mywindow, (uint16_t)vertex2.x, (uint16_t)vertex2.y );
        }
    }

    if(do_init)
    {
        window_write( &mywindow );
        window_close( &mywindow );
    }
}

void glxgears_loop()
{
    static float rotz = 0;
    static float roty = 30.0 / 180 * M_PI;
    static float step = -1.0 / 180 * M_PI;

    window_new( &mywindow, H, W );
    window_clear ( &mywindow );

    Matrix user_rotx_ = Matrix::get_rx(user_rotx);
    Matrix user_roty_ = Matrix::get_ry(user_roty);
    Matrix view_rotx = Matrix::get_rx(0.0 / 180 * M_PI);
    Matrix view_roty = Matrix::get_ry(roty);
    Matrix view_transform = Matrix::get_translate(0, 1, 20);

    Matrix big_matrix;
    Matrix transform = Matrix::get_translate(-1, 2, 0);
    Matrix rz = Matrix::get_rz(rotz);
    big_matrix = rz *
        transform *
        view_roty *
        view_rotx *
        user_rotx_ *
        user_roty_ *
        view_transform *
        clipMatrix;

    regis_plot(glxgear1, sizeof(glxgear1) / sizeof(point_t), big_matrix, _R, 0);

    window_write( &mywindow );
    window_reset( &mywindow );

    transform = Matrix::get_translate(5.2, 2, 0);
    rz = Matrix::get_rz(-2.0 * rotz + 9.0 / 180 * M_PI);
    big_matrix = rz *
        transform *
        view_roty *
        view_rotx *
        user_rotx_ *
        user_roty_ *
        view_transform *
        clipMatrix;

    regis_plot(glxgear2, sizeof(glxgear2) / sizeof(point_t), big_matrix, _G, 0);

    window_write( &mywindow );
    window_reset( &mywindow );

    transform = Matrix::get_translate(-1.1, -4.2, 0);
    rz = Matrix::get_rz(-2.0 * rotz + 30.0 / 180 * M_PI);
    big_matrix = rz *
        transform *
        view_roty *
        view_rotx *
        user_rotx_ *
        user_roty_ *
        view_transform *
        clipMatrix;

    regis_plot(glxgear3, sizeof(glxgear3) / sizeof(point_t), big_matrix, _B, 0);

    window_write( &mywindow );
    window_close( &mywindow );

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

#ifndef __AVR
    manage_fps();
#endif
}

void gear_loop()
{
    static float rotz = 0;
    static float roty = 0;
    static float step2 = 1.0 / 180 * M_PI;
    Matrix transform = Matrix::get_translate(0, 0, 8);
    Matrix user_rotx_ = Matrix::get_rx(user_rotx);
    Matrix user_roty_ = Matrix::get_ry(user_roty);
    Matrix rz = Matrix::get_rz(rotz);
    Matrix ry = Matrix::get_ry(roty);

    Matrix big_matrix;
    big_matrix = rz *
        ry *
        user_rotx_ *
        user_roty_ *
        transform *
        clipMatrix;
    regis_plot(gear, sizeof(gear) / sizeof(point_t), big_matrix, _W, 1);

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

#ifndef __AVR
    manage_fps();
#endif
}

void cube_loop()
{
    static float rotz = 0;
    static float roty = 0;
    Matrix transform = Matrix::get_translate(0, 0, 10);
    Matrix user_rotx_ = Matrix::get_rx(user_rotx);
    Matrix user_roty_ = Matrix::get_ry(user_roty);
    Matrix rz = Matrix::get_rz(rotz);
    Matrix ry = Matrix::get_ry(roty);


    Matrix big_matrix;
    big_matrix = rz *
        ry *
        user_rotx_ *
        user_roty_ *
        transform *
        clipMatrix;
    regis_plot(cube, sizeof(cube) / sizeof(point_t), big_matrix, _W, 1);

#ifndef __AVR
    manage_fps();
#else
    delay(30);
#endif

    if(animate)
    {
        rotz += 2.0 / 360 * M_PI * 2;
        roty += .5 / 360 * M_PI * 2;
    }
}



void icos_loop()
{
    static float rotz = 0;
    static float roty = 0;
    Matrix transform = Matrix::get_translate(0, 0, 8);
    Matrix rz = Matrix::get_rz(rotz);
    Matrix user_rotx_ = Matrix::get_rx(user_rotx);
    Matrix user_roty_ = Matrix::get_ry(user_roty);
    Matrix rx = Matrix::get_rx(M_PI / 2);
    Matrix ry = Matrix::get_ry(roty);


    Matrix big_matrix;
    big_matrix = rx *
        ry *
        rz *
        user_rotx_ *
        user_roty_ *
        transform *
        clipMatrix;

    regis_plot(icos, sizeof(icos) / sizeof(point_t), big_matrix, _W, 1);

#ifndef __AVR
    manage_fps();
#endif

    if(animate)
    {
        rotz += .25 / 360 * M_PI * 2;
        roty += 2.0 / 360 * M_PI * 2;
    }
}


void setup() {
    // initialize serial communication at 115200 bits per second:
    Serial.begin(115200);

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
    }

    begin_projection();
    clear_screen();
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
    }
}

#ifndef __AVR

void sig_catch(int sig)
{
    // reset the console
    window_close( &mywindow );
    struct termios info;
    tcgetattr(fileno(stdin), &info);
    info.c_lflag |= ICANON;
    info.c_lflag |= ECHO;
    tcsetattr(fileno(stdin), TCSANOW, &info);
    exit(0);
}

int main()
{
    signal(SIGKILL, sig_catch);
    signal(SIGHUP, sig_catch);
    signal(SIGINT, sig_catch);
    signal(SIGQUIT, sig_catch);
    signal(SIGTERM, sig_catch);

    setup();

    while(1)
    {
        loop();
    }

    return 0;
}
#endif

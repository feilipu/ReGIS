## ReGIS - Remote Graphics Instruction Set
------------

ReGIS interprets commands that allow you to simply and efficiently control a video monitor screen and draw pictures on the screen with lines, curves, and circles using a serial interface (USART). Also, ReGIS provides commands to include scalable text characters in pictures. The ReGIS graphics language is designed for conciseness and easy transport of code from the host to the ReGIS device. The language consists of commands that are modified by options.

Read here for a full description on [how to enable ReGIS for Windows 10 and Linux desktop machines](https://feilipu.me/2022/09/28/regis-serial-graphics-for-arduino-rc2014/).

There is a [Programmer Reference Manual for VT330/VT340 Terminals](https://vt100.net/docs/vt3xx-gp/) describing how to use ReGIS, as well as the [VT125 Primer](https://github.com/feilipu/ReGIS/blob/main/doc/EK-VT125-GI-001_VT125_ReGIS_Primer_May82.pdf) document provided here.

Homogeneous coordinates are ubiquitous in computer graphics because they allow common vector operations such as translation, rotation, scaling and perspective projection to be represented as a matrix by which the vector is multiplied. By the chain rule, any sequence of such operations can be multiplied out into a single matrix, allowing simple and efficient processing.<br>
Basic homogeneous coordinate vector and matrix operations are included in this library.

## Preparing XTerm to support ReGIS

XTerm is the only known software solution supporting ReGIS commands (to be improved I'm sure). But it doesn't support ReGIS in the default build. You'll need to enable ReGIS yourself.
``` sh
$ sudo apt install -y libxaw7-dev libncurses-dev libxft-dev
$ wget https://invisible-island.net/datafiles/release/xterm.tar.gz
$ tar xf xterm.tar.gz
$ cd xterm-392
$ ./configure --enable-regis-graphics
$ make
$ sudo make install
```

As XTerm has no serial interface itself, so you'll need to use one. A suggestion is to use picocom.<br>
It is also useful for working with Arduino, and other embedded devices, generally. First test that your installation is working as per below.

``` sh
$ sudo apt install -y picocom
$ picocom -b 115200 -f h /dev/ttyUSB0
```

And, finally XTerm using VT340 emulation together with picocom.
``` sh
xterm +u8 -geometry 132x50 -ti 340 -tn 340 -e picocom -b 115200 -f h /dev/ttyUSB0
```

Another alternative is using XTerm VT125 emulation.
``` sh
xterm +u8 -geometry 132x50 -ti 125 -tn 125 -e picocom -b 115200 -f h /dev/ttyUSB0
```

### Windows Subsystem for Linux

WSL1 supports connection of Serial Devices via USB. WSL2 has some issues with this, particuarly with Windows 10.

But, the supported picocom distribution for Ubuntu 22.04 Version 3.1 (for example), utilises some modern terminal capabilities that WSL does not support.
So to avoid the use of these terminal system calls we have to use an older release of picocom. For example the [Version 2.2.2 provided with Ubuntu 18.04 LTS](https://manpages.ubuntu.com/manpages/bionic/man1/picocom.1.html). Be sure to use this older version of picocom if you are using WSL to run XTerm and picocom.

The tested method of accessing XTerm on WSL is [MobaXTerm](https://mobaxterm.mobatek.net/). This enhanced terminal for Windows includes an integrated Xserver. If MobaXTerm is used to access WSL XTerm, its window will automatically connect to the Windows desktop.

Read here for a full description on [how to enable ReGIS for Windows 10 and Linux desktop machines](https://feilipu.me/2022/09/28/regis-serial-graphics-for-arduino-rc2014/).

## Demonstration

There is a demonstration program, which should produce the below result (subject to improvement).

<div>
<table style="border: 2px solid #cccccc;">
<tbody>
<tr>
<td style="border: 1px solid #cccccc; padding: 6px;"><a href="https://github.com/feilipu/ReGIS/blob/main/examples/regis_demo/regis_demo.png" target="_blank"><img src="https://github.com/feilipu/ReGIS/blob/main/examples/regis_demo/regis_demo.png"/></a></td>
</tr>
<tr>
<th style="border: 1px solid #cccccc; padding: 6px;"><centre>RC2014 ReGIS - Picocom in XTerm<center></th>
</tr>
</tbody>
</table>
</div>

This is image generated from the below ReGIS code.<br>
Expected output (where `^` is the `ESC` character).
```
^P1pS(E)W(I(M))P[600,200]V[][-200,+200]V[][400,100]W(I(G))P[700,100]V(B)[+050,][,+050][-050,](E)V(W(S1))(B)[-100,][,-050][+100,](E)V(W(S1,E))(B)[-050,][,-025][+050,](E)W(I(C))P[200,100]C(A-180)[+100]C(A+180)[+050]W(I(B))P[200,300]C(W(S1))[+100]C(W(S1,E))[+050]W(I(W))T(S02)"hello world"^\
```

## 3D functions

Vector functions

```C
/* Produce a unit vector */
void unit_v(vector_t * vect);

/* Scale a vector by m, but don't touch w dimension */
void scale_v(vector_t * vect, float scale);

/* Produce a dot product between vectors */
float dot_v(vector_t * vect1, vector_t * vect2);

/* Vector Matrix Multiplication */
void mult_v(vector_t * vect,matrix_t * multiplier);
```

Matrix 3D (Homogeneous Coordinate) functions

```C
/* Produce an identity matrix */
void identity_m(matrix_t * matrix);

/* Produce a transformation (translation) */
void translate_m(matrix_t * matrix, float x, float y, float z);

/* Produce a transformation (scale) */
void scale_m(matrix_t * matrix, float x, float y, float z);

/* Produce a transformation (shear) */
void shear_m(matrix_t * matrix, float x, float y, float z);

/* Rotation in x dimension */
void rotx_m(matrix_t * matrix, float angle);

/* Rotation in y dimension */
void roty_m(matrix_t * matrix, float angle);

/* Rotation in z dimension */
void rotz_m(matrix_t * matrix, float angle);

/* Set up a projection matrix - OpenGL */
void projection_opengl_m(matrix_t * matrix, float fov, float aspect_ratio, float near, float far);

/* Set up a projection matrix - W3Woody */
void projection_w3woody_m(matrix_t * matrix, float fov, float aspect_ratio, float near, float far);

/* Matrix Multiplication */
void mult_m(matrix_t * multiplicand, matrix_t * multiplier);
```


## Credits

For describing [how to get XTerm working with ReGIS](https://groups.google.com/g/rc2014-z80/c/fuji5iuJ3Jc/m/FNYwGGbaAQAJ), thanks Rob Gowin.<br>
For advising on how to get [ReGIS fonts in XTerm working](https://github.com/feilipu/z88dk-libraries/commit/107c23d2f31791b0a35bfc8833a7747a84544649#diff-45390165fce0bae9bf04313a098b2a42d5d727bf90d4be040f01ec0f7fee969d), thanks Thomas Dickey.

## License

This library is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check the LICENSE file for more information.

Contributing to this software is warmly welcomed. You can do this by [forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pull requests](https://help.github.com/articles/using-pull-requests).

## ReGIS - Remote Graphics Instruction Set
------------

Read here for a full description on [how to enable ReGIS for Windows 10 and Linux desktop machines](https://feilipu.me/2022/09/28/regis-serial-graphics-for-arduino-rc2014/).

ReGIS interprets commands that allow you to simply and efficiently control a video monitor screen and draw pictures on the screen with lines, curves, and circles using a serial interface (USART). Also, ReGIS provides commands to include 

## Preparing XTERM to support ReGIS

XTERM is the only known software solution supporting ReGIS commands (to be improved I'm sure). But it doesn't support ReGIS in the default build. You'll need to enable ReGIS yourself.
``` sh
$ sudo apt install -y libxaw7-dev libncurses-dev libxft-dev
$ wget https://invisible-island.net/datafiles/release/xterm.tar.gz
$ tar xf xterm.tar.gz
$ cd xterm-373
$ ./configure --enable-regis-graphics
$ make
$ sudo make install
```

As XTERM has no serial interface itself, so you'll need to use one. A suggestion is to use picocom.<br>
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
So to avoid the use of these terminal system calls we have to use an older release of picocom. For example the [Version 2.2.2 provided with Ubuntu 18.04 LTS](https://manpages.ubuntu.com/manpages/bionic/man1/picocom.1.html). Be sure to use this older version of picocom if you are using WSL to run XTERM and picocom.

The tested method of accessing XTERM on WSL is [MobaXterm](https://mobaxterm.mobatek.net/). This enhanced terminal for Windows includes an integrated Xserver. If MobaXterm is used to access WSL XTERM, its window will automatically connect to the Windows desktop.

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
<th style="border: 1px solid #cccccc; padding: 6px;"><centre>RC2014 ReGIS - Picocom in XTERM<center></th>
</tr>
</tbody>
</table>
</div>

This is image generated from the below ReGIS code.<br>
Expected output (where `^` is the `ESC` character).
```
^P1pS(E)W(I(M))P[600,200]V[][-200,+200]V[][400,100]W(I(G))P[700,100]V(B)[+050,][,+050][-050,](E)V(W(S1))(B)[-100,][,-050][+100,](E)V(W(S1,E))(B)[-050,][,-025][+050,](E)W(I(C))P[200,100]C(A-180)[+100]C(A+180)[+050]W(I(B))P[200,300]C(W(S1))[+100]C(W(S1,E))[+050]W(I(W))T(S02)"hello world"^\
```

## Credits

For describing [how to get XTERM working with ReGIS](https://groups.google.com/g/rc2014-z80/c/fuji5iuJ3Jc/m/FNYwGGbaAQAJ), thanks Rob Gowin.<br>
For advising on how to get [ReGIS fonts in XTERM working](https://github.com/feilipu/z88dk-libraries/commit/107c23d2f31791b0a35bfc8833a7747a84544649#diff-45390165fce0bae9bf04313a098b2a42d5d727bf90d4be040f01ec0f7fee969d), thanks Thomas Dickey.

## License

This library is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check the LICENSE file for more information.

Contributing to this software is warmly welcomed. You can do this by [forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pull requests](https://help.github.com/articles/using-pull-requests).

# Planet Motion

A rough calculation of planetary motion - to test ReGIS graphics and floating point solutions

[![Geocentric Planetary Motion](https://github.com/feilipu/planet-motion/blob/master/doc/planets.png)](https://youtu.be/9HGfSPHtk7Y "Planetary motion - ReGIS & RC2014 APU Module")

# Purpose

To test various floating point packages available for the CP/M, RC2014 and other z88dk supported targets.

# Usage

This planetary motion visualisation requires the ReGIS library to be installed in z88dk.<br>
For instructions on how to do this, and to enable xterm to support ReGIS [please read here](https://github.com/feilipu/z88dk-libraries/tree/master/regis).

Once ReGIS library is available then one of the below compilation lines can be used to prepare the motion binary.

```sh
    zcc +rc2014 -subtype=cpm -clib=new -v -m --list -O2 -lm -llib/rc2014/regis @planet_motion.lst -o motion_new -create-app
    zcc +rc2014 -subtype=cpm -v -m --list -lm -llib/rc2014/regis --max-allocs-per-node100000 @planet_motion.lst -o motion_48 -create-app
    zcc +rc2014 -subtype=cpm -v -m --list --math32 -llib/rc2014/regis --max-allocs-per-node100000 @planet_motion.lst -o motion_32 -create-app
    zcc +rc2014 -subtype=cpm -v -m --list --am9511 -llib/rc2014/regis --max-allocs-per-node100000 @planet_motion.lst -o motion_apu -create-app
```

```sh
    zcc +yaz180 -subtype=cpm -v -m --list --math32 -llib/rc2014/regis --max-allocs-per-node100000 @planet_motion.lst -o motion_32 -create-app
```

```sh
    zcc +cpm -clib=new -v -m --list -O2 -lm -llib/rc2014/regis @planet_motion.lst -o motion_new -create-app
    zcc +cpm -clib=sdcc_iy -v -m --list -lm -llib/rc2014/regis --max-allocs-per-node100000 @planet_motion.lst -o motion_48 -create-app
    zcc +cpm -clib=sdcc_iy -v -m --list --math32 -llib/rc2014/regis --max-allocs-per-node100000 @planet_motion.lst -o motion_32 -create-app
    zcc +cpm -clib=sdcc_iy -v -m --list --am9511 -llib/rc2014/regis --max-allocs-per-node100000 @planet_motion.lst -o motion_apu -create-app
```


# Credits

Based on the work of [Paul Schlyter](http://www.stjarnhimlen.se/english.php).

 - [How to compute planetary postitions](http://www.stjarnhimlen.se/comp/ppcomp.html).
 - [Tutorial on computing planetary positions](http://www.stjarnhimlen.se/comp/tutorial.html).

And the implementations of [Cosine Kitty (Don Cross)](http://cosinekitty.com/)

 - [Solar System Calculator](https://cosinekitty.com/solar_system.html).
 - [Astronomy Sky View](http://cosinekitty.com/sky_view.html).

# Licence

MIT License

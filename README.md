# FreeSpace / FreeSpace 2
----------------------------------------------
Freespace 2 (http://www.freespace2.com)
            - A game by Volition, Inc. (http://www.volition-inc.com)

This project is NOT supported by Volition or Interplay.

http://www.icculus.org/freespace2/

Based on the Freespace 2 source code:
Copyright (C) Volition, Inc. 1999.  All rights reserved.

All source code herein is the property of Volition, Inc. You may not sell or otherwise commercially exploit the source or things you created based on the source.
----------------------------------------------

# How to Compile


By default, the PANDORA version of Freespace 2 will be compiled.
just type
```bash
make
```

To build:
- ODroid version, add `ODROID=true`
- Linux version add `PANDORA=false`
- a Demo version, add `DEMO=true`
- FreeSpace 1 add `FS1=true`

Multiple option can be combined. So for example to build a release FreeSpace 1 version for ODroid, use
```bash
make ODROID=true FS1=true
```

# GoG Data
On Linux, you can use the Windows Good Old Game (GoG) version to extract the datas.
You will innoextract for that. It should be availbale in your distro, or just compile from sources.
Just put all vp and MEV/mve files in the same folder as the freespace binary.

To extract FreeSpace - The Great War in current folder, use this script:
```bash
innoextract -I app setup_freespace_*.exe
mv app/*.vp .
mv app/Data/*.vp .
mv app/Data/movies/*.mve .
mv app/Data/movies/*.MVE .
mv app/data1/Data/*.vp .
mv app/data2/data/movies/* .
rm app/data2/data/movies -rf
mv app/data2/data/*.vp .
rm app -rf
```

For the FreeSpace 2 data, you can use this one:
```bash
innoextract -I app setup_freespace2_*.exe
mv app/*.vp .
mv app/data2/*.vp .
mv app/data2/*.MVE .
mv app/data3/*.vp .
mv app/data3/*.MVE .
rm app -rf
```

# Original Readme

06/18/2002

Current status:
- You need data files to run this.  You can use the Freespace 2 demo or
install the full game with wine.  If your commercial version of Freespace 2
is not the Game of the Year edition, you'll need to update to the latest
patch before copying the data files over.

- It currently uses SDL (http://www.libsdl.org) and OpenAL
(http://opensource.creative.com)

- The OpenGL code is complete minus one or two things, but it is unoptimized.

- Keyboard, mouse, and joystick work.

- Lots of debug messages.  Recompile with NDEBUG defined to get rid of them.

- The build currently defaults to the full version.  To build the demo
version, edit the Makefile by changing the DEMO flag from "false" to "true".
Mods, etc. have not been tested.

- Networking (TCP) seems to work OK.  Linux<->Linux works great, Lin<->Win
tested a little (with Linux client acting as the server, Windows server may
cause problems with filenames and transferred files).

- Sound code is only partially implemented.

- Currently all files (*.vp) need to be in the current working directory.

- The Windows version might still compile, but wasn't tested.  (The
Linux-specific code is separated by a PLAT_UNIX define).  Fred2, etc., were
not touched.

- This code builds on MacOS X. Get the latest toolchain from Apple. Flip the
MACOSX flag in the Makefile to "true" and run "make" from a terminal in the
root of the source directory. Note that the game compiles and links, but will
NOT run at this time, due to byte ordering bugs in the source.

For more information about the game and its source code:
http://www.freespace2.com/
http://www.volitionwatch.com/
http://fs2source.warpcore.org/

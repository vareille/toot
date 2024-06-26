SPDX-License-Identifier: Zlib
Copyright (c) 2017 - 2024 Guillaume Vareille http://ysengrin.com
	 ________________________________________________________________
	|                                                                |
	| 100% compatible C C++  ->  You can rename all .c files as .cpp |
	|________________________________________________________________|

I've got tired of beep not working, here is toot (cross-platform C C++), toot works.
It simply tries to call different ways to produce a sine wave that might be present on your machine.
   __              __
  / /_____  ____  / /_
 / __/ __ \/ __ \/ __/
/ /_/ /_/ / /_/ / /_
\__/\____/\____/\__/
 a beep that beeps

it's both a C/C++ library and a command line tool
Windows OSX Linux BSD Solaris Minix Raspbian

toot v1.2.2 [Jan 23, 2024]

https://github.com/vareille/toot
git clone https://github.com/vareille/toot
     _____________________________________________________________
    |                                                             |
    | my email address is at the top of the header file tootlib.h |
    |_____________________________________________________________|
	 ________________________________________________________________
	|                                                                |
	| 100% compatible C C++  ->  You can rename all .c files as .cpp |
	|________________________________________________________________|
     ____________________________________________________________
    |  _________   ______________   ____________   ____________  |
    | |         | |              | |            | |            | |
    | | NO INIT | | NO MAIN LOOP | | NO LINKING | | NO INCLUDE | |
    | |_________| |______________| |____________| |____________| |
    |____________________________________________________________|

void toot(float aFrequency_Hz, int aLength_ms) ;

Usage:  toot [-f frequency_Hz] [-l length_ms]
        toot [-v] [--verbose]
        toot [-h] [--help]
        toot [--version]
Default: toot -f 440. -l 400
Example: toot

the release pane contains windows and osx executables

make sure the volume is on (though some beeps may use the hardware speaker)
if you can't adjust frequency / length (or if it doesn't beep)
	on osx:		install sox (from macport.org or brew.sh)
	on unix:	install sox

If you know of any other way to reliably generate sine wave sound or beep on
anykind of machine, I'll be happy to add them to toot.

UNIX (including MacOS) :
$ clang -o toot tootlib.c toot.c
( or gcc tcc icx)
( or g++ clang++ icpx )

MinGW :
> gcc -o toot.exe tootlib.c toot.c -luser32

Windows Borland C :
> bcc32c -o toot.exe toot.c tootlib.c -luser32

Windows TinyCC v0.9.27 :
> tcc -o toot.exe toot.c tootlib.c -luser32

Windows Intel C :
> icx-cc -o toot.exe toot.c tootlib.c -luser32
> icx-cl -o toot.exe toot.c tootlib.c user32.lib


Windows cli :
> cl toot.c tootlib.c user32.lib /W4

VisualStudio :
Create an application project or an empty project (it links against user32.lib)

************************************************************************
I am also the author of tiny file dialogs
it's a cross-platform C C++ file that offers many native dialogs
no init, no main loop, no include, no linking
 _________
/         \   NotifyPopup InputBox PasswordBox MessageBox ColorPicker
|tiny file|   OpenFileDialog SaveFileDialog SelectFolderDialog
| dialogs |   ASCII UTF-8 (and also MBCS UTF-16 on windows)
\____  ___/   Native dialog library for WINDOWS MAC OSX GTK+ QT CONSOLE
     \|       http://tinyfiledialogs.sourceforge.net
************************************************************************

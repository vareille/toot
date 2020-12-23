I've got tired of beep not working, here is toot (cross-platform C C++), toot works. 
   __              __
  / /_____  ____  / /_
 / __/ __ \/ __ \/ __/
/ /_/ /_/ / /_/ / /_
\__/\____/\____/\__/
 a beep that beeps

it's both a C C++ library and a command line tool
Windows OSX Linux BSD Solaris Minix Raspbian

toot v1.0.8 [Dec 23, 2020]
(c) 2017 Guillaume Vareille http://ysengrin.com zlib licence
https://github.com/vareille/toot
git clone https://github.com/vareille/toot.git toot

void toot(float aFrequency_Hz, int aLength_ms) ;

Usage:  toot [-f frequency_Hz] [-l length_ms]  
        toot [-v] [--verbose]  
        toot [-h] [--help]
        toot [--version]  
Default: toot -f 440. -l 300
Example: toot

the release tab contains windows and osx executables

make sure the volume is on (though some beeps may use the hardware speaker)
if you can't adjust frequency / length (or if it doesn't beep)
	on osx:		you can install sox http://macappstore.org/sox
	on unix:	you can install pactl or speaker-test or sox

If you know of any other way to reliably generate sine wave sound or beep on
anykind of machine, I'll be happy to add them to toot. Let me know: toot at ysengrin.com

OSX :
$ gcc -o toot.app toot_cmd_line_main.c toot.c
( or clang )

UNIX :
$ gcc -o toot toot_cmd_line_main.c toot.c
( or clang tcc cc CC )

MinGW :
> gcc -o toot.exe toot_cmd_line_main.c toot.c -LC:/mingw/lib -lkernel32
( or tcc )

VisualStudio :
Create an application project or an empty project (it links against Kernel32.lib)

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


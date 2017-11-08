I've got tired of beep not working, here is toot (cross-platform C C++), it works. 
   __              __
  / /_____  ____  / /_
 / __/ __ \/ __ \/ __/
/ /_/ /_/ / /_/ / /_
\__/\____/\____/\__/
 a beep that beeps

it is both a C C++ library and a command line tool
Windows OSX Linux BSD Solaris Minix Raspbian

toot v1.0.2 (c) 2017 Guillaume Vareille http://ysengrin.com zlib licence

void toot(float aFrequence_Hz, int aLength_ms) ;

Usage:  toot [-f frequence_Hz] [-l length_ms]  
        toot [-v] [--verbose]  
        toot [-h] [--help]  
        toot [--version]  
Default: toot -f 440 -l 300
Example: toot

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

the release tab contains windows and osx executables

************************************************************************
I am also the author of tinyfiledialogs on sourceforge
it's a cross-platform C C++ file that offers many native dialogs
no init, no linking, no include, no main loop
 _________
/         \   NotifyPopup InputBox PasswordBox MessageBox ColorPicker
|tiny file|   OpenFileDialog SaveFileDialog SelectFolderDialog
| dialogs |   ASCII UTF-8 (and also MBCS UTF-16 on windows)
\____  ___/   Native dialog library for WINDOWS MAC OSX GTK+ QT CONSOLE
     \|
************************************************************************


/* SPDX-License-Identifier: Zlib
Copyright (c) 2017 - 2024 Guillaume Vareille http://ysengrin.com
	 ________________________________________________________________
	|                                                                |
	| 100% compatible C C++  ->  You can rename this .c file as .cpp |
	|________________________________________________________________|
     ____________________________________________________________
    |  _________   ______________   ____________   ____________  |
    | |         | |              | |            | |            | |
    | | NO INIT | | NO MAIN LOOP | | NO LINKING | | NO INCLUDE | |
    | |_________| |______________| |____________| |____________| |
    |____________________________________________________________|
   __              __
  / /_____  ____  / /_  toot.c v1.2.2 [Jan 23, 2024] zlib licence
 / __/ __ \/ __ \/ __/  cross-platform library and command line tool to toot "tooooot"
/ /_/ /_/ / /_/ / /_    file created [November 7, 2017]
\__/\____/\____/\__/
 a beep that beeps      https://github.com/vareille/toot
						git clone https://github.com/vareille/toot
						 ____________________________________
						|                                    |
                        |    email: toot at ysengrin.com     |
                        |____________________________________|
- License -

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software.  If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifdef _WIN32
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <stdint.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tootlib.h"

int main(int argc, char * argv[])
{
#ifdef _WIN32
	int lFd;
	HANDLE lHStdOut;
#endif
	int i;
	float lFrequency = 0.f ;
	int lLength = 0 ;

	for (i = 0; i < argc; i++)
	{
		if (!strcmp(argv[i], "-f")) lFrequency = (float) atof(argv[i + 1]);
		else if (!strcmp(argv[i], "-l")) lLength = atoi(argv[i + 1]);
		else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")
			|| !strcmp(argv[i], "-V") || !strcmp(argv[i], "--version")
			|| !strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose") )
		{
#ifdef _WIN32
			if ( !GetConsoleWindow()
#if ! defined(__TINYC__) && ! defined(__DMC__)
				&& AttachConsole(ATTACH_PARENT_PROCESS)
#endif
				)
			{
				lHStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
				lFd = _open_osfhandle((intptr_t)lHStdOut, _O_TEXT);
				if (lFd > 0) *stdout = *_fdopen(lFd, "w");
				printf("\n");
			}
#endif
			if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) toot_verbose = 1;
			else if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--version")) { printf("%s\n", toot_version); return 0; }
			else
			{
				printf("    __              __ \n");
				printf("   / /_____  ____  / /_\n");
				printf("  / __/ __ \\/ __ \\/ __/\n");
				printf(" / /_/ /_/ / /_/ / /_  \n");
				printf(" \\__/\\____/\\____/\\__/  \n");
				printf("  a beep that beeps\n\n");
				printf("toot v%s (c) 2017 - 2024 Guillaume Vareille http://ysengrin.com zlib licence\n", toot_version);
				printf("cross-platform library and command line tool to toot \"tooooot\"\n");
				printf("OSX Windows Linux BSD Solaris Minix Raspbian\n");
				printf("Usage:\ttoot [-f frequency_Hz] [-l length_ms]\n");
				printf("\ttoot [-v] [--verbose]\n");
				printf("\ttoot [-V] [--version]\n");
				printf("\ttoot [-h] [--help]\n");
				printf("Default: toot -f 440. -l 400\n");
				printf("Example: toot\n");
				return 0;
			}
		}
	}
	toot( lFrequency , lLength ) ;
	return 0;
}

/*
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
*/

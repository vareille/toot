/* SPDX-License-Identifier: Zlib
Copyright (c) 2017 - 2025 Guillaume Vareille http://ysengrin.com
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
  / /_____  ____  / /_  tootlib.c v1.2.4 [Sep 7, 2025] zlib licence
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
3. This notice may not be removed or altered from any source distributon.
*/

#ifdef _WIN32
 #include <windows.h>
#else /* UNIX */
 #ifndef __SUNPRO_C
  #define _POSIX_C_SOURCE 2 /* to accept POSIX 2 in old ANSI C standards */
 #endif
 #include <string.h>
 #include <dirent.h> /* on old systems try <sys/dir.h> instead */
 #include <errno.h>
 #include <signal.h> /* on old systems try <sys/signal.h> instead */
#endif
#include <stdio.h>

#include "tootlib.h"

char toot_version[8] = "1.2.4"; /* contains toots current version number */

int toot_verbose = 0 ; /* 0 (default) or 1 : prints the command line calls */

#ifdef _MSC_VER
#pragma warning(disable:4996) /* allows usage of strncpy, strcpy, strcat, sprintf, fopen */
#pragma warning(disable:4100) /* allows usage of strncpy, strcpy, strcat, sprintf, fopen */
#pragma warning(disable:4706) /* allows usage of strncpy, strcpy, strcat, sprintf, fopen */
#endif


#ifdef _WIN32
static int powershellPresent(void)
{
	static int lPowershellPresent = -1;
	char lBuff[MAX_PATH];
	FILE* lIn;
	char const* lString = "powershell.exe";

	if (lPowershellPresent < 0)
	{
		lIn = _popen("where powershell.exe", "r");
		if ( ! lIn )
		{
			lPowershellPresent = 0;
			return 0;
		}
		while (fgets(lBuff, sizeof(lBuff), lIn) != NULL)
		{
		}
		_pclose(lIn);
		if (lBuff[strlen(lBuff) - 1] == '\n')
		{
			lBuff[strlen(lBuff) - 1] = '\0';
		}
		if (strcmp(lBuff + strlen(lBuff) - strlen(lString), lString))
		{
			lPowershellPresent = 0;
		}
		else
		{
			lPowershellPresent = 1;
		}
	}
	return lPowershellPresent;
}


static int windowsVersion(void)
{
#if !defined(__MINGW32__) || defined(__MINGW64_VERSION_MAJOR)
	typedef LONG NTSTATUS;
	typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
	HMODULE hMod;
	RtlGetVersionPtr lFxPtr;
	RTL_OSVERSIONINFOW lRovi = { 0 };

	hMod = GetModuleHandleW(L"ntdll.dll");
	if (hMod) {
		lFxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
		if (lFxPtr)
		{
			lRovi.dwOSVersionInfoSize = sizeof(lRovi);
			if (!lFxPtr(&lRovi))
			{
				return lRovi.dwMajorVersion;
			}
		}
	}
#endif
	if (powershellPresent()) return 6 ; /*minimum is vista or installed on xp*/
	return 0;
}
#else
static void sigHandler(int sig)
{
	FILE * lIn = ( FILE * ) popen( "pactl unload-module module-sine" , "r" ) ;

	if ( lIn )
    {
        pclose( lIn ) ;
    }
}

static int detectPresence(char const * aExecutable)
{
	char lBuff [256] ;
	char lTestedString [256] = "command -v " ;
	FILE * lIn ;

	strcat( lTestedString , aExecutable ) ;
	strcat( lTestedString, " 2>/dev/null ");
	lIn = ( FILE * ) popen( lTestedString , "r" ) ;
	if ( ( fgets( lBuff , sizeof( lBuff ) , lIn ) != NULL )
		&& ( ! strchr( lBuff , ':' ) )
		&& ( strncmp(lBuff, "no ", 3) ) )
	{	/* present */
		pclose( lIn ) ;
		if (toot_verbose) printf("detectPresence %s %d\n", aExecutable, 1);
		return 1 ;
	}
	else
	{
		pclose( lIn ) ;
		if (toot_verbose) printf("detectPresence %s %d\n", aExecutable, 0);
		return 0 ;
	}
}


static int osascriptPresent( void )
{
	static int lOsascriptPresent = -1 ;
	if ( lOsascriptPresent < 0 )
	{
		lOsascriptPresent = detectPresence( "osascript" ) ;
	}
	return lOsascriptPresent;
}


static int afplayPresent( void )
{
	static int lAfplayPresent = -1 ;
	char lBuff [256] ;
	FILE * lIn ;

	if ( lAfplayPresent < 0 )
	{
		lAfplayPresent = detectPresence("afplay") ;
		if ( lAfplayPresent )
		{
			lIn = ( FILE * ) popen( "test -e /System/Library/Sounds/Ping.aiff || echo Ping" , "r" ) ;
			if ( fgets( lBuff , sizeof( lBuff ) , lIn ) == NULL )
			{
				lAfplayPresent = 2 ;
			}
			pclose( lIn ) ;
			if (toot_verbose) printf("afplay %d\n", lAfplayPresent);
		}
	}
	return lAfplayPresent ;
}


static int ffplayPresent( void )
{
	static int lFFplayPresent = -1;
	if (lFFplayPresent < 0)
	{
		lFFplayPresent = detectPresence("ffplay") ;
	}
	return lFFplayPresent ;
}


static int pactlPresent( void )
{
	static int lPactlPresent = -1 ;
	char lBuff [256] ;
	FILE * lIn ;

	if ( lPactlPresent < 0 )
	{
		lPactlPresent = detectPresence("pactl") ;
		if ( lPactlPresent )
		{
			lIn = ( FILE * ) popen( "pactl info | grep -iF pulseaudio" , "r" ) ;
			if ( ! (fgets( lBuff , sizeof( lBuff ) , lIn ) && ! strstr(lBuff, "PipeWire") ) )
			{
				lPactlPresent = 0 ;
			}
			pclose( lIn ) ;
			if (toot_verbose) printf("is pactl valid ? %d\n", lPactlPresent);
		}
	}
	return lPactlPresent ;
}


static int speakertestPresent( void )
{
	static int lSpeakertestPresent = -1 ;
	if ( lSpeakertestPresent < 0 )
	{
		lSpeakertestPresent = detectPresence("speaker-test") ;
	}
	return lSpeakertestPresent ;
}


static int beepexePresent( void )
{
	static int lBeepexePresent = -1;
	if (lBeepexePresent < 0)
	{
		lBeepexePresent = detectPresence("beep.exe");
	}
	return lBeepexePresent;
}


/*static int beepPresent( void )
{
	static int lBeepPresent = -1 ;
	if ( lBeepPresent < 0 )
	{
		lBeepPresent = detectPresence("beep") ;
	}
	return lBeepPresent ;
}*/


static int playPresent( void ) /* play is part of sox*/
{
	static int lPlayPresent = -1;
	if (lPlayPresent < 0)
	{
		lPlayPresent = detectPresence("sox") ;
	}
	return lPlayPresent ;
}

#endif /* UNIX */

void toot(float aFrequency_Hz, int aLength_ms)
{
#ifndef _WIN32
	char lDialogString[256];
	FILE * lIn;
	DIR * lDir;
#endif

	aFrequency_Hz = aFrequency_Hz > 0 ? aFrequency_Hz : 440.f;
	aLength_ms = aLength_ms > 0 ? aLength_ms : 400;

#ifdef _WIN32
	if (toot_verbose) printf("Windows function Beep(dwFreq, dwDuration) %dHz %dms\n", (int)aFrequency_Hz, aLength_ms);
	if (windowsVersion() > 5) Beep((DWORD)aFrequency_Hz, aLength_ms);
	else MessageBeep(MB_OK);
#else /* UNIX */

	if ( pactlPresent() )
	{
		(void) signal(SIGINT, sigHandler);

		/*strcpy( lDialogString , "pactl load-module module-sine frequency=440;sleep .3;pactl unload-module module-sine" ) ;*/
		sprintf(lDialogString,
            "thnum=$(pactl load-module module-sine frequency=%d);sleep %f;pactl unload-module $thnum",
			(int) aFrequency_Hz, aLength_ms / 1000. ) ;
	}
	else if ( speakertestPresent() )
	{
		/*strcpy( lDialogString , "timeout -k .3 .3 speaker-test --frequency 440 --test sine > /dev/tty" ) ;*/
		lDir = opendir("/dev/tty");
		if ( !lDir && (ENOENT!=errno) )
		{
			sprintf(lDialogString,
				"(speaker-test -t sine -f %f > /dev/tty) & pid=$!;sleep %f;kill -9 $pid",
				(double) aFrequency_Hz, aLength_ms / 1000. ) ;
		}
		else
		{
			sprintf(lDialogString,
				"(speaker-test -t sine -f %f) & pid=$!;sleep %f;kill -9 $pid",
				(double) aFrequency_Hz, aLength_ms / 1000. ) ;
		}
	}
	else if (ffplayPresent())
	{
		sprintf(lDialogString, "ffplay -f lavfi -i sine=f=%f:d=%f -autoexit -nodisp", (double) aFrequency_Hz , aLength_ms/1000. );
	}
	else if (playPresent()) /* play is part of sox */
	{
		sprintf(lDialogString, "play -n synth %f sine %f\n", aLength_ms/1000., (double) aFrequency_Hz);
	}
	else if (beepexePresent())
	{
		sprintf(lDialogString, "beep.exe %f %d\n", (double) aFrequency_Hz, aLength_ms);
	}
	/*else if ( beepPresent() )
	{
		sprintf(lDialogString, "beep -f %f -l %d\n", (double) aFrequency_Hz, aLength_ms);
	}*/
	else if ( osascriptPresent() )
	{
		printf( "On MacOS you must install sox (from macport.org or brew.sh)\n" ) ;
		if ( afplayPresent() >= 2 )
		{
			strcpy( lDialogString , "afplay /System/Library/Sounds/Ping.aiff") ;
		}
		else
		{
			strcpy( lDialogString , "osascript -e 'tell application \"System Events\" to beep'") ;
		}
	}
	else
	{
	    printf( "On this system, you need to install sox\n" ) ;
		lDir = opendir("/dev/tty");
		if ( !lDir && (ENOENT!=errno) ) strcpy( lDialogString , "printf '\\a' > /dev/tty" ) ;
		else strcpy( lDialogString , "printf '\\a'" ) ;
	}

	if (toot_verbose) printf( "toot-cmdline: %s\n" , lDialogString ) ;

	lIn = ( FILE * ) popen( lDialogString , "r" ) ;
	if ( lIn )
	{
		pclose( lIn ) ;
	}

	if ( pactlPresent() )
	{
		(void) signal(SIGINT, SIG_DFL);
	}

#endif /* UNIX */
}

#ifdef _MSC_VER
#pragma warning(default:4996)
#pragma warning(default:4100)
#pragma warning(default:4706)
#endif


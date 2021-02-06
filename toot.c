/* this file can be renamed with extension ".cpp" as the code is 100% compatible C C++ */

/* __              __ 
  / /_____  ____  / /_  toot.c v1.0.9 [Jan 17, 2021] zlib licence
 / __/ __ \/ __ \/ __/  cross-platform library and command line tool to toot "tooooot"
/ /_/ /_/ / /_/ / /_    file created [November 7, 2017]
\__/\____/\____/\__/    Copyright (c) 2017 - 2021 Guillaume Vareille http://ysengrin.com
 a beep that beeps      https://github.com/vareille/toot
                        git clone https://github.com/vareille/toot.git toot
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
 #include <Windows.h>
#else /* UNIX */
 #define _POSIX_C_SOURCE 2 /* to accept POSIX 2 in old ANSI C standards */
 #include <string.h>
 #include <dirent.h> /* on old systems try <sys/dir.h> instead */
 #include <errno.h>
 #include <signal.h> /* on old systems try <sys/signal.h> instead */
#endif
#include <stdio.h>

char toot_version[8] = "1.0.9"; /* contains toots current version number */

int toot_verbose = 0; /* 0 (default) or 1 : prints the command line calls */

#ifdef _WIN32
static int powershellPresent(void)
{
	static int lPowershellPresent = -1;
	char lBuff[MAX_PATH];
	FILE* lIn;
	char const* lString = "powershell.exe";

	if (lPowershellPresent < 0)
	{
		if (!(lIn = _popen("where powershell.exe", "r")))
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
	FILE * lIn ;
	if ( ( lIn = popen( "pactl unload-module module-sine" , "r" ) ) )
    {
        pclose( lIn ) ;
    }
}

static int detectPresence(char const * aExecutable)
{
	char lBuff [256] ;
	char lTestedString [256] = "which " ;
	FILE * lIn ;

	strcat( lTestedString , aExecutable ) ;
	strcat( lTestedString, " 2>/dev/null ");
	lIn = popen( lTestedString , "r" ) ;
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


static int osascriptPresent( )
{
	static int lOsascriptPresent = -1 ;
	if ( lOsascriptPresent < 0 )
	{
		lOsascriptPresent = detectPresence( "osascript" ) ;
	}
	return lOsascriptPresent;
}


static int afplayPresent( )
{
	static int lAfplayPresent = -1 ;
	char lBuff [256] ;
	FILE * lIn ;

	if ( lAfplayPresent < 0 )
	{
		lAfplayPresent = detectPresence("afplay") ;
		if ( lAfplayPresent )
		{
			lIn = popen( "test -e /System/Library/Sounds/Ping.aiff || echo Ping" , "r" ) ;
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


static int pactlPresent( )
{
	static int lPactlPresent = -1 ;
	if ( lPactlPresent < 0 )
	{
		lPactlPresent = detectPresence("pactl") ;
	}
	return lPactlPresent ;
}


static int speakertestPresent( )
{
	static int lSpeakertestPresent = -1 ;
	if ( lSpeakertestPresent < 0 )
	{
		lSpeakertestPresent = detectPresence("speaker-test") ;
	}
	return lSpeakertestPresent ;
}


static int beepexePresent()
{
	static int lBeepexePresent = -1;
	if (lBeepexePresent < 0)
	{
		lBeepexePresent = detectPresence("beep.exe");
	}
	return lBeepexePresent;
}


static int beepPresent( )
{
	static int lBeepPresent = -1 ;
	if ( lBeepPresent < 0 )
	{
		lBeepPresent = detectPresence("beep") ;
	}
	return lBeepPresent ;
}


static int playPresent()
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
	if (toot_verbose) printf("windows Beep %dHz %dms\n", (int)aFrequency_Hz, aLength_ms);
	if (windowsVersion() > 5) Beep((DWORD)aFrequency_Hz, aLength_ms);
	else MessageBeep(-1);
#else /* UNIX */

	if ( pactlPresent() ) 
	{
		signal(SIGINT, sigHandler);

		/*strcpy( lDialogString , "pactl load-module module-sine frequency=440;sleep .3;pactl unload-module module-sine" ) ;*/
		sprintf(lDialogString,
"thnum=$(pactl load-module module-sine frequency=%d);sleep %f;pactl unload-module $thnum",
			(int)aFrequency_Hz, aLength_ms / 1000.f);
	}
	else if ( speakertestPresent() ) 
	{
		/*strcpy( lDialogString , "timeout -k .3 .3 speaker-test --frequency 440 --test sine > /dev/tty" ) ;*/
		lDir = opendir("/dev/tty");
		if ( !lDir && (ENOENT!=errno) )
		{
			sprintf(lDialogString,
				"(speaker-test -t sine -f %f > /dev/tty) & pid=$!;sleep %f;kill -9 $pid",
				aFrequency_Hz, aLength_ms / 1000.f);
		}
		else
		{
			sprintf(lDialogString,
				"(speaker-test -t sine -f %f) & pid=$!;sleep %f;kill -9 $pid",
				aFrequency_Hz, aLength_ms / 1000.f);
		}
	}
	else if (beepexePresent())
	{
		sprintf(lDialogString, "beep.exe %f %d\n", aFrequency_Hz, aLength_ms);
	}
	else if (playPresent()) /* play is part of sox */
	{
		sprintf(lDialogString, "play -n synth %f sine %f\n", aLength_ms/1000.f, aFrequency_Hz);
	}
	else if ( beepPresent() )
	{
		sprintf(lDialogString, "beep -f %f -l %d\n", aFrequency_Hz, aLength_ms);
	}
	else if ( osascriptPresent() )
	{
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
		lDir = opendir("/dev/tty");
		if ( !lDir && (ENOENT!=errno) ) strcpy( lDialogString , "printf '\a' > /dev/tty" ) ;
		else strcpy( lDialogString , "printf '\a'" ) ;
	}

	if (toot_verbose) printf( "toot-cmdline: %s\n" , lDialogString ) ;

	if ( ( lIn = popen( lDialogString , "r" ) ) )
	{
		pclose( lIn ) ;
	}

	if ( pactlPresent() )
	{
		signal(SIGINT, SIG_DFL);
	}

#endif /* UNIX */
}

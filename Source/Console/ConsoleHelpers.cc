
#include <cstdio>
#include <Windows.h>
#include <Console/ConsoleHelpers.hh>

void SetConsoleTextColor(EConsoleColor color)
{
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (color)
	{
	case kEConsoleColorStandard:
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case kEConsoleColorWarning:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case kEConsoleColorError:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case kEConsoleColorVerbose:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	default:
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		break;
	}
}

void Msg(const wchar_t* pszMsg)
{
	SetConsoleTextColor(kEConsoleColorStandard);
	wprintf(TEXT("%s\n"), pszMsg);
	SetConsoleTextColor(kEConsoleColorReset);
}

void WarningMsg(const wchar_t* pszMsg)
{
	SetConsoleTextColor(kEConsoleColorWarning);
	wprintf(TEXT("%s\n"), pszMsg);
	SetConsoleTextColor(kEConsoleColorReset);
}

void ErrorMsg(const wchar_t* pszMsg)
{
	SetConsoleTextColor(kEConsoleColorError);
	wprintf(TEXT("%s\n"), pszMsg);
	SetConsoleTextColor(kEConsoleColorReset);
}

void DevMsg(const wchar_t* pszMsg)
{
#if defined(_DEBUG)
	SetConsoleTextColor(kEConsoleColorVerbose);
	wprintf(TEXT("%s\n"), pszMsg);
	SetConsoleTextColor(kEConsoleColorReset);
#endif
}
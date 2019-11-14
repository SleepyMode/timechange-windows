#pragma once

enum EConsoleColor
{
	kEConsoleColorReset,
	kEConsoleColorStandard,
	kEConsoleColorWarning,
	kEConsoleColorError,
	kEConsoleColorVerbose
};

void SetConsoleTextColor(EConsoleColor color);

void Msg(const wchar_t* pszMsg);
void WarningMsg(const wchar_t* pszMsg);
void ErrorMsg(const wchar_t* pszMsg);
void DevMsg(const wchar_t* pszMsg);
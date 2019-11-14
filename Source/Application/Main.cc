
#include <cstdio>
#include <Windows.h>
#include <Console/ConsoleHelpers.hh>

#define WINDOWS_WIDECHAR_MAX_PATH 32767

SYSTEMTIME g_creationTime;				// The creation time to set.
SYSTEMTIME g_lastAccessTime;			// The last access time to set.
SYSTEMTIME g_lastWriteTime;				// The last write time to set (last modified).

bool g_bCreationTimeSet		= false;	// Whether or not the creation time has been set.
bool g_bLastAccessTimeSet	= false;	// Whether or not the last access time has been set.
bool g_bLastWriteTimeSet	= false;	// Whether or not the last write time has been set.

wchar_t g_path[WINDOWS_WIDECHAR_MAX_PATH];		// The path of the file/directory.
bool g_bPathSet = false;						// Whether or not the path has been set.

bool g_bDirectory = false;		// Whether or not the path is to a directory.
bool g_bInclusive = false;		// Whether or not the directory's files should also be set.
bool g_bInclusiveStarted = false;

bool HandleFile(const wchar_t* path);
bool HandleDirectory(const wchar_t* path);

#pragma warning(suppress: 4100)
int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
#if defined(_DEBUG)
	SetConsoleTitle(TEXT("[smTools | Debug] Timestamp Changer v2.0.0"));
#else
	SetConsoleTitle(TEXT("[smTools] Timestamp Changer v2.0.0"));
#endif

	Msg(TEXT("\nWelcome to the Timestamp Changer."));

	DevMsg(TEXT("Parsing arguments..."));

	// First argument is always that silly path to the application itself.
	if (argc <= 1)
	{
		Msg(TEXT("No arguments received, help displayed."));

		Msg(TEXT("\nThe path to the file/directory is set via the -path parameter."));
		Msg(TEXT("-path <path>"));
		Msg(TEXT("\nThere are two ways to set times, one is to set each one manually."));
		Msg(TEXT("The creation time may be set by setting the -creationTime parameter."));
		Msg(TEXT("The last access time may be set by setting the -lastAccessTime parameter."));
		Msg(TEXT("The last write time may be set by setting the -lastWriteTime parameter (This is what shows in the explorer)."));
		Msg(TEXT("-creationTime <year> <month> <day> <hour> <minute> <second> <milliseconds>"));
		Msg(TEXT("-lastAccessTime <year> <month> <day> <hour> <minute> <second> <milliseconds>"));
		Msg(TEXT("-lastWriteTime <year> <month> <day> <hour> <minute> <second> <milliseconds>"));
		Msg(TEXT("\nThe second way is to set the -globalTime parameter, which will set all three."));
		Msg(TEXT("-globalTime <year> <month> <day> <hour> <minute> <second> <milliseconds>"));
		Msg(TEXT("It's important to note that any of the prior three arguments will override the -globalTime settings."));
		Msg(TEXT("\nIf you use this tool for a directory, you need to set a flag:"));
		Msg(TEXT("--directory means the file is a directory."));
		Msg(TEXT("\nAdditionally, if you wish that all files in a directory be set to the aforementioned time, set the flag:"));
		Msg(TEXT("--inclusive means all files in the directory will have their time set."));

		Msg(TEXT("\n\nPress any key to exit."));
		getchar();
		return 0;
	}

	for (int i = 0; i < argc; ++i)
	{
		wchar_t* arg = _wcslwr(argv[i]);

		if (arg[0] == '-')
		{
			if (wcscmp(arg, TEXT("-path")) == 0)
			{
				wcscpy_s<WINDOWS_WIDECHAR_MAX_PATH>(g_path, argv[i + 1]);
				g_bPathSet = true;
			}
			else if (wcscmp(arg, TEXT("--directory")) == 0)
			{
				g_bDirectory = true;
			}
			else if (wcscmp(arg, TEXT("--inclusive")) == 0)
			{
				g_bInclusive = true;
			}
			else if (wcscmp(arg, TEXT("-creationtime")) == 0)
			{
				WORD wYear;
				WORD wMonth;
				WORD wDay;
				WORD wHour;
				WORD wMinute;
				WORD wSecond;
				WORD wMilliseconds;

				swscanf(argv[i + 1], TEXT("%hu"), &wYear);
				swscanf(argv[i + 2], TEXT("%hu"), &wMonth);
				swscanf(argv[i + 3], TEXT("%hu"), &wDay);
				swscanf(argv[i + 4], TEXT("%hu"), &wHour);
				swscanf(argv[i + 5], TEXT("%hu"), &wMinute);
				swscanf(argv[i + 6], TEXT("%hu"), &wSecond);
				swscanf(argv[i + 7], TEXT("%hu"), &wMilliseconds);

				g_creationTime = {
					wYear,
					wMonth,
					static_cast<WORD>(-1),			// Day of Week. Ignored.
					wDay,
					wHour,
					wMinute,
					wSecond,
					wMilliseconds
				};

				g_bCreationTimeSet = true;
			}
			else if (wcscmp(arg, TEXT("-lastaccesstime")) == 0)
			{
				WORD wYear;
				WORD wMonth;
				WORD wDay;
				WORD wHour;
				WORD wMinute;
				WORD wSecond;
				WORD wMilliseconds;

				swscanf(argv[i + 1], TEXT("%hu"), &wYear);
				swscanf(argv[i + 2], TEXT("%hu"), &wMonth);
				swscanf(argv[i + 3], TEXT("%hu"), &wDay);
				swscanf(argv[i + 4], TEXT("%hu"), &wHour);
				swscanf(argv[i + 5], TEXT("%hu"), &wMinute);
				swscanf(argv[i + 6], TEXT("%hu"), &wSecond);
				swscanf(argv[i + 7], TEXT("%hu"), &wMilliseconds);

				g_lastAccessTime = {
					wYear,
					wMonth,
					static_cast<WORD>(-1),			// Day of Week. Ignored.
					wDay,
					wHour,
					wMinute,
					wSecond,
					wMilliseconds
				};

				g_bLastAccessTimeSet = true;
			}
			else if (wcscmp(arg, TEXT("-lastwritetime")) == 0)
			{
				WORD wYear;
				WORD wMonth;
				WORD wDay;
				WORD wHour;
				WORD wMinute;
				WORD wSecond;
				WORD wMilliseconds;

				swscanf(argv[i + 1], TEXT("%hu"), &wYear);
				swscanf(argv[i + 2], TEXT("%hu"), &wMonth);
				swscanf(argv[i + 3], TEXT("%hu"), &wDay);
				swscanf(argv[i + 4], TEXT("%hu"), &wHour);
				swscanf(argv[i + 5], TEXT("%hu"), &wMinute);
				swscanf(argv[i + 6], TEXT("%hu"), &wSecond);
				swscanf(argv[i + 7], TEXT("%hu"), &wMilliseconds);

				g_lastWriteTime = {
					wYear,
					wMonth,
					static_cast<WORD>(-1),			// Day of Week. Ignored.
					wDay,
					wHour,
					wMinute,
					wSecond,
					wMilliseconds
				};

				g_bLastWriteTimeSet = true;
			}
			else if (wcscmp(arg, TEXT("-timeglobal")) == 0)
			{
				WORD wYear;
				WORD wMonth;
				WORD wDay;
				WORD wHour;
				WORD wMinute;
				WORD wSecond;
				WORD wMilliseconds;

				swscanf(argv[i + 1], TEXT("%hu"), &wYear);
				swscanf(argv[i + 2], TEXT("%hu"), &wMonth);
				swscanf(argv[i + 3], TEXT("%hu"), &wDay);
				swscanf(argv[i + 4], TEXT("%hu"), &wHour);
				swscanf(argv[i + 5], TEXT("%hu"), &wMinute);
				swscanf(argv[i + 6], TEXT("%hu"), &wSecond);
				swscanf(argv[i + 7], TEXT("%hu"), &wMilliseconds);

				if (!g_bCreationTimeSet)
				{
					g_creationTime = {
						wYear,
						wMonth,
						static_cast<WORD>(-1),			// Day of Week. Ignored.
						wDay,
						wHour,
						wMinute,
						wSecond,
						wMilliseconds
					};

					g_bCreationTimeSet = true;
				}

				if (!g_bLastAccessTimeSet)
				{
					g_lastAccessTime = {
						wYear,
						wMonth,
						static_cast<WORD>(-1),			// Day of Week. Ignored.
						wDay,
						wHour,
						wMinute,
						wSecond,
						wMilliseconds
					};

					g_bLastAccessTimeSet = true;
				}

				if (!g_bLastWriteTimeSet)
				{
					g_lastWriteTime = {
						wYear,
						wMonth,
						static_cast<WORD>(-1),			// Day of Week. Ignored.
						wDay,
						wHour,
						wMinute,
						wSecond,
						wMilliseconds
					};

					g_bLastWriteTimeSet = true;
				}
			}
			else
			{
				wchar_t buffer[255];
				wsprintf(buffer, TEXT("Received unknown arguments \"%s\", skipping."), argv[i]);

				WarningMsg(buffer);
			}
		}
	}

	if (g_bPathSet)
	{
		if (g_bDirectory)
			HandleDirectory(g_path);
		else
			HandleFile(g_path);
	}
	else
	{
		ErrorMsg(TEXT("No path has been entered."));
	}

#pragma warning(suppress: 6031)
	Msg(TEXT("Complete. Press any key to exit."));
	getchar();

	return 0;
}

bool HandleFile(const wchar_t* path)
{
	HANDLE hFile;

	hFile = CreateFile(
		path,
		FILE_ALL_ACCESS,
		(g_bInclusive) ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : 0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FILETIME fCreationTime;
		FILETIME fLastAccessTime;
		FILETIME fLastWriteTime;

		if (GetFileTime(hFile, &fCreationTime, &fLastAccessTime, &fLastWriteTime) == TRUE)
		{
			if (g_bCreationTimeSet)
			{
				if (!g_bInclusive)
					Msg(TEXT("Converting creation time..."));

				BOOL bSuccess = SystemTimeToFileTime(&g_creationTime, &fCreationTime);

				if (bSuccess != TRUE)
					ErrorMsg(TEXT("Failed to convert creation time."));
			}

			if (g_bLastAccessTimeSet)
			{
				if (!g_bInclusive)
					Msg(TEXT("Converting last access time..."));

				BOOL bSuccess = SystemTimeToFileTime(&g_lastAccessTime, &fLastAccessTime);

				if (bSuccess != TRUE)
					ErrorMsg(TEXT("Failed to convert last access time."));
			}

			if (g_bLastWriteTimeSet)
			{
				if (!g_bInclusive)
					Msg(TEXT("Converting last write time..."));

				BOOL bSuccess = SystemTimeToFileTime(&g_lastWriteTime, &fLastWriteTime);

				if (bSuccess != TRUE)
					ErrorMsg(TEXT("Failed to convert last write time."));
			}

			Msg(TEXT("Writing file time..."));

			if (SetFileTime(hFile, &fCreationTime, &fLastAccessTime, &fLastWriteTime) == TRUE)
			{
				if (!g_bInclusive)
					Msg(TEXT("Successfully written file time."));
			}
			else
			{
				ErrorMsg(TEXT("Failed to write file time."));
			}
		}
		else
		{
			ErrorMsg(TEXT("Failed to receive file time."));
		}

		DevMsg(TEXT("Releasing file handle..."));
		if (CloseHandle(hFile))
			DevMsg(TEXT("Successfully released handle."));
		else
			ErrorMsg(TEXT("Failed to release handle."));
	}
	else
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			ErrorMsg(TEXT("File could not be located."));
		else
			ErrorMsg(TEXT("Invalid handle."));
	}

	return false;
}

bool HandleDirectory(const wchar_t* path)
{
	HANDLE hFile;

	hFile = CreateFile(
		path,
		FILE_ALL_ACCESS,
		(g_bInclusive) ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : 0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
		NULL
	);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FILETIME fCreationTime;
		FILETIME fLastAccessTime;
		FILETIME fLastWriteTime;

		if (GetFileTime(hFile, &fCreationTime, &fLastAccessTime, &fLastWriteTime) == TRUE)
		{
			if (g_bCreationTimeSet)
			{
				if (!g_bInclusive)
					Msg(TEXT("Converting creation time..."));

				BOOL bSuccess = SystemTimeToFileTime(&g_creationTime, &fCreationTime);

				if (bSuccess != TRUE)
					ErrorMsg(TEXT("Failed to convert creation time."));
			}

			if (g_bLastAccessTimeSet)
			{
				if (!g_bInclusive)
					Msg(TEXT("Converting last access time..."));

				BOOL bSuccess = SystemTimeToFileTime(&g_lastAccessTime, &fLastAccessTime);

				if (bSuccess != TRUE)
					ErrorMsg(TEXT("Failed to convert last access time."));
			}

			if (g_bLastWriteTimeSet)
			{
				if (!g_bInclusive)
					Msg(TEXT("Converting last write time..."));

				BOOL bSuccess = SystemTimeToFileTime(&g_lastWriteTime, &fLastWriteTime);

				if (bSuccess != TRUE)
					ErrorMsg(TEXT("Failed to convert last write time."));
			}

			Msg(TEXT("Writing directory time..."));

			if (SetFileTime(hFile, &fCreationTime, &fLastAccessTime, &fLastWriteTime) == TRUE)
			{
				if (!g_bInclusive)
					Msg(TEXT("Successfully written file time."));
			}
			else
			{
				ErrorMsg(TEXT("Failed to write file time."));
			}
		}
		else
		{
			ErrorMsg(TEXT("Failed to receive file time."));
		}

		if (g_bInclusive)
		{
			HANDLE hFindStream;
			WIN32_FIND_DATA findData;

			wchar_t search_path[WINDOWS_WIDECHAR_MAX_PATH];
			//wcscpy(search_path, path);
			GetFullPathName(path, WINDOWS_WIDECHAR_MAX_PATH, search_path, NULL);
			wcscat(search_path, L"\\*");
			DevMsg(search_path);

			hFindStream = FindFirstFile(search_path, &findData);

			if (hFindStream != INVALID_HANDLE_VALUE)
			{
				while (FindNextFile(hFindStream, &findData))
				{
					wchar_t new_path[WINDOWS_WIDECHAR_MAX_PATH];
					wcscpy(new_path, path);
					wcscat(new_path, L"\\");
					wcscat(new_path, findData.cFileName);

					DevMsg(findData.cFileName);
					DevMsg(path);
					DevMsg(new_path);

					if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
					{
						if ((!lstrcmpW(findData.cFileName, L".")) || (!lstrcmpW(findData.cFileName, L"..")))
							continue;

						HandleDirectory(new_path);
					}
					else
					{
						HandleFile(new_path);
					}
				}

				if (GetLastError() == ERROR_NO_MORE_FILES)
					DevMsg(TEXT("Out of files."));
				else
					WarningMsg(TEXT("FindNextFile call failed."));

				FindClose(hFindStream);
			}
			else
			{
				ErrorMsg(TEXT("Invalid stream handle."));
			}
		}

		DevMsg(TEXT("Releasing file handle..."));
		if (CloseHandle(hFile))
			DevMsg(TEXT("Successfully released handle."));
		else
			ErrorMsg(TEXT("Failed to release handle."));
	}
	else
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			ErrorMsg(TEXT("Directory could not be located."));
		else
			ErrorMsg(TEXT("Invalid handle."));
	}

	return false;
}


#if !defined(AFX_FILETOFILE_H__C2B5A4C7_500D_11D7_B72C_0010A4EF1954__INCLUDED_)
#define AFX_FILETOFILE_H__C2B5A4C7_500D_11D7_B72C_0010A4EF1954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "resource.h"


#define MAX_LOADSTRING 100
//Globals to control the number of instances 
TCHAR szMutexName[]="$MutexFor3Threads$";
HANDLE hMutex;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
// Globals for Options Box
	BOOL bDirection;
	TCHAR szKey[MAX_LOADSTRING];
	UINT  uLengthKey;
// Globals for Source File
	TCHAR szSourceFilePath[260];
	TCHAR szSourceDirName[260];
	TCHAR szFileTitle[260];
	TCHAR szFilter[260]=TEXT("Text Files\0*.txt\0Encripted Files\0*.crpt\0Any Files\0*.*\0\0");
	TCHAR szDlgTitleOpen[260]=TEXT("Select source file");
	TCHAR szDlgTitleSave[260]=TEXT("Select destination file");
	TCHAR szDestinationFilePath[260];
#endif // !defined(AFX_FILETOFILE_H__C2B5A4C7_500D_11D7_B72C_0010A4EF1954__INCLUDED_)

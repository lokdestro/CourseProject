// WndHandles.cpp: implementation of the WndHandles .
//

#include "stdafx.h"
//#include "resource.h"  //COMMON
//#include "WndHandles.h"
 
#define MAX_LOADSTRING 100

extern "C" { //Declared outside, here used 
// Global Variables:
	HINSTANCE hInst;								// current instance
	TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
	TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text
// Globals for Options Box
	BOOL bDirection;
	TCHAR szKey[MAX_LOADSTRING];
	UINT  uLengthKey; 
// Globals for Source File 
}

LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ExitBox(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK OptionsBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL OptionsBox_OnInitDialog(HWND hDlg, HWND hwndFocus, LPARAM lParam);
LONG OptionsBox_OnCommand (HWND hDlg,int id,HWND hwndCtl,UINT codeNotify);

//Menu item handlers
BOOL GetFileOpen	 (  HWND hWnd,LPOPENFILENAME lpofn);
BOOL GetFileEncripted(  HWND hWnd,LPOPENFILENAME lpofn);

//Basic functions
BOOL EncriptFileToFile(  LPSTR lpstrSourceFile, LPSTR lpstrDestinationFile);
bool CopyFileToFile(HANDLE hSourceFile,HANDLE hDestFile);
bool Simplest    (unsigned uLengthKey,LPSTR szKey,HANDLE hDestFile);

//bool Simplest    (unsigned uLengthKey,LPSTR szKey,HANDLE hSourceFile,HANDLE hDestFile);
//void SimplestXtoY(unsigned uLengthKey,LPSTR szKey,DWORD nBytesRead,LPSTR xBuf,LPSTR yBuf);
#define RFORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, fn) \
    (LONG)(fn)((hwnd), WM_COMMAND, MAKEWPARAM((UINT)(id),(UINT)(codeNotify)), (LPARAM)(HWND)(hwndCtl))


static OPENFILENAME ofn;

LONG WndProc_OnCommand (HWND hWnd,int id,HWND hwndCtl,UINT codeNotify)
{
		// Parse the menu selections:
			switch (id)
			{
				case IDM_OPENFILE:
					//Get File Name and File Type
					 if(GetFileOpen(hWnd,&ofn))
					 {
						 MessageBox(NULL,ofn.lpstrFile,"Get File Open",MB_OK);
					 }
				   return 0L;
				     break;
				case IDM_ENCRIPT:
					//Call Encript Function
					if(GetFileEncripted(hWnd,&ofn))
					{
						MessageBox(NULL,ofn.lpstrFile,"Encripted File",MB_OK);
					}else
						MessageBox(NULL,ofn.lpstrFile,"Not Encripted File",MB_OK);
				   return 0L;
				     break;
				case IDM_OPTIONS:	//Done!
					DialogBox(hInst, (LPCTSTR)IDD_DIALOG_OPTIONS, hWnd, (DLGPROC)OptionsBox);
				   return 0L;
				     break;

				case IDM_ABOUT:		//Done!
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   return 0L;
				     break;
				case IDM_EXIT:		//Done!
						PostQuitMessage(0);
						return 0L;
				   //?DestroyWindow(hWnd);					
				  break;
				default:break;
				  // return DefWindowProc(hWnd, message, wParam, lParam);
			}
		return RFORWARD_WM_COMMAND(hWnd,  id,  hwndCtl,  codeNotify,DefWindowProc);
}

void WndProc_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];

	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	hdc = BeginPaint(hWnd, &ps);
// TODO: Add any drawing code here...
	RECT rt;
	GetClientRect(hWnd, &rt);
	DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
    EndPaint(hWnd, &ps);
}

 
LONG WndProc_OnCreate(HWND hWnd,LPCREATESTRUCT lpCreateStruct)
{
	extern  TCHAR szDestinationFilePath[260];
	extern TCHAR szSourceFilePath[260];

	bDirection=TRUE;
	LoadString(hInst, IDS_APP_TITLE, szKey, MAX_LOADSTRING);
	uLengthKey=strlen(szKey);

	szSourceFilePath[0]=CHAR('\0');
	szDestinationFilePath[0]=CHAR('\0');
		return TRUE;//BOOL
}
void WndProc_OnDestroy(HWND hWnd)
{	
		PostQuitMessage(0);	
}

void WndProc_OnClose(HWND hWnd)
{

//	SendMessage(hWnd,WM_COMMAND,IDM_ABOUT,0);//DlgBox!!! 
	DialogBox(hInst, (LPCTSTR)IDD_ONCLOSE, hWnd, (DLGPROC)ExitBox);

	PostQuitMessage(0);
}

#define RHANDLE_MSG(hwnd, message, fn)    \
    case (message): return RHANDLE_##message((hwnd), (wParam), (lParam), (fn))

#define RHANDLE_WM_COMMAND(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (int)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam)), -1L)

#define RHANDLE_WM_INITDIALOG(hwnd, wParam, lParam, fn) \
    (LRESULT)(DWORD)(UINT)(BOOL)(fn)((hwnd), (HWND)(wParam), lParam)


// Message handler for Options box: Window Procedure for DialogBox

LRESULT CALLBACK OptionsBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	RHANDLE_MSG(hDlg,WM_INITDIALOG, OptionsBox_OnInitDialog);//!!
 	RHANDLE_MSG(hDlg,WM_COMMAND, OptionsBox_OnCommand);//!!
	//return true; for all handled messages
	default:
	 	return FALSE;//not handled messages		 
	}
}//OptionsBox   
 
	static UINT uLenNewKey;
	static TCHAR szNewKey[260];
	

LONG OptionsBox_OnCommand (HWND hDlg,int id,HWND hwndCtl,UINT codeNotify)
{
		// Parse the Options Box commands:
			switch (id)
			{
			case IDOK:
				
				bDirection = IsDlgButtonChecked(hDlg,IDC_RADIO_DIRECT) ? TRUE : FALSE;
				strcpy(szKey,szNewKey);
				uLengthKey=uLenNewKey;

			case IDCANCEL:
				EndDialog(hDlg, id);
				return TRUE;//handled

			case IDC_EDIT_KEY:
					if(codeNotify==EN_CHANGE)
					{
						uLenNewKey= GetDlgItemText(
							hDlg,				// handle of dialog box
							IDC_EDIT_KEY,		// identifier of control
							szNewKey,			// address of buffer for text
							MAX_LOADSTRING );   // maximum size of string						
					}
				return TRUE;
			default:
				return TRUE;
			}//return TRUE for all handled commands,any others are not here
}//OptionsBox_OnCommand

BOOL OptionsBox_OnInitDialog(HWND hDlg, HWND hwndFocus, LPARAM lParam)
{
	//Set Current Direction and Key Values
	 
	CheckDlgButton(hDlg,IDC_RADIO_DIRECT,bDirection);
	CheckDlgButton(hDlg,IDC_RADIO_BACKWARD,!bDirection);
 
	SendDlgItemMessage(hDlg,IDC_EDIT_KEY,WM_SETTEXT ,0,(LPARAM)szKey);//EM_REPLACESEL

	return TRUE;//handled
}//OptionsBox_OnInitDialog

BOOL GetFileOpen( HWND hWnd, LPOPENFILENAME lpofn)
{
 
 
// Globals for Source File
extern	TCHAR szSourceFilePath[260];
extern	TCHAR szSourceDirName[260];

extern	TCHAR szFileTitle[260];
extern	TCHAR szFilter[260];
extern	TCHAR szDlgTitleOpen[260];
 
	memset(lpofn,0,sizeof(OPENFILENAME));
	GetCurrentDirectory(sizeof(szSourceDirName),szSourceDirName);
	szSourceFilePath[0]=TCHAR('\0');

	lpofn->lStructSize	=sizeof(OPENFILENAME);
	lpofn->hwndOwner	=hWnd;

	lpofn->lpstrFilter	=szFilter;
	lpofn->nFilterIndex	=1;

	lpofn->lpstrInitialDir=szSourceDirName;	
	lpofn->lpstrFile	=szSourceFilePath;
	lpofn->nMaxFile		=sizeof(szSourceFilePath);

	lpofn->lpstrFileTitle=szFileTitle;
	lpofn->nMaxFileTitle=sizeof(szFileTitle);
	lpofn->lpstrTitle	=szDlgTitleOpen;

	lpofn->Flags		=OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
	if(GetOpenFileName(lpofn))
	{
		//File chosen
		return TRUE;
	}
	//File was not chosen
	return FALSE;
}

BOOL GetFileEncripted(  HWND hWnd,LPOPENFILENAME lpofn)
{
	// Globals for Source File
extern	TCHAR szSourceDirName[260];
extern	TCHAR szSourceFilePath[260];
extern  TCHAR szDestinationFilePath[260];


extern	TCHAR szFileTitle[260];
extern	TCHAR szFilter[260];

extern	TCHAR szDlgTitleOpen[260];
extern	TCHAR szDlgTitleSave[260];

if(strlen(szSourceFilePath)==0) 
{
	MessageBox(NULL,TEXT("Select a source file name!"),TEXT("Encript File"),MB_OK);
	return FALSE;
}

lpofn->lpstrTitle=szDlgTitleSave;
lpofn->Flags	 =OFN_HIDEREADONLY;

lpofn->lpstrFile=szDestinationFilePath;

 if(GetSaveFileName(lpofn))
 {
	 if(0==strcmp(szDestinationFilePath,szSourceFilePath))
	 {
			MessageBox(NULL,TEXT("Select different file names!"),TEXT("Encript File"),MB_OK);
			return FALSE;
	 }
	return EncriptFileToFile(szSourceFilePath,szDestinationFilePath) ;
 }
return FALSE;
}/////GetFileEncripted

BOOL EncriptFileToFile(  LPSTR lpstrSourceFile, LPSTR lpstrDestinationFile)
{
	TCHAR message[260];

	wsprintf(message,"Source: %s\nDestination: %s\nDirection: %s\nKey: %s\n",
		lpstrSourceFile,
		lpstrDestinationFile,
		bDirection ? "Direct" : "Backward",
		szKey);
	MessageBox(NULL,message,TEXT("Encript File To File"),MB_OK);
	 
//-----------------------------------------
//Create Handles to Source and Destination Files 
//-----------------------------------------
	HANDLE hSourceFile, hDestFile;

//Create Handle to Source File 
hSourceFile = CreateFile(
			lpstrSourceFile,        // pointer to name of the file
			GENERIC_READ,			// access (read-write) mode
			FILE_SHARE_READ,        // share mode
			NULL,                   // pointer to security attributes
			OPEN_EXISTING,          // how to create
			FILE_ATTRIBUTE_READONLY|FILE_FLAG_SEQUENTIAL_SCAN,
		//	|FILE_FLAG_OVERLAPPED,   // file attributes
			NULL                    // handle to file with attributes to copy
		);

	if(hSourceFile == INVALID_HANDLE_VALUE)
	{
		wsprintf(message,"hSourceFile is INVALID_HANDLE_VALUE; ERROR %ld\n",GetLastError());
		MessageBox(NULL,message,TEXT("Encript File To File"),MB_OK);
		return FALSE;
	}
//hSourceFile has just created!
//--------------------------------------------
//Create Handle to Destination File 
hDestFile = CreateFile(
			lpstrDestinationFile, // pointer to name of the file
			GENERIC_WRITE,		  // access (read-write) mode
			0,					  // If dwShareMode is 0, the object cannot be shared. 
						          //Subsequent open operations on the object will fail,
						          //until the handle is closed. 
			NULL,                   // pointer to security attributes
			CREATE_ALWAYS,          // how to create
			FILE_ATTRIBUTE_NORMAL,
			//|FILE_FLAG_OVERLAPPED,   // file attributes
			NULL                    // handle to file with attributes to copy
		);

	if(hDestFile == INVALID_HANDLE_VALUE)
	{
		wsprintf(message,"1)hDestFile is INVALID_HANDLE_VALUE; ERROR %ld\n",GetLastError());
		MessageBox(NULL,message,TEXT("Encript File To File"),MB_OK);

		CloseHandle(hSourceFile);
		return FALSE;
	}
//hDestFile has just created!
//-------------------------------------------- 
	if(strlen(szKey)!=uLengthKey)
	{
		MessageBox(NULL,TEXT("strlen(szKey)!=uLengthKey"),TEXT("Encript File To File"),MB_OK);
		CloseHandle(hSourceFile);
		CloseHandle(hDestFile);
		return false;
	}

	bool res;

	res= CopyFileToFile(hSourceFile,hDestFile);

	if(!res)
	{
		MessageBox(NULL,TEXT("CopyFileToFile failed... "),TEXT("Encript File To File"),MB_OK);
	    CloseHandle(hSourceFile);
	    CloseHandle(hDestFile);
	    return false;
	}
	CloseHandle(hSourceFile);
	CloseHandle(hDestFile);
 
//##################################################//
//Create Handle to Destination File to be mapped onto memory
	
hDestFile = CreateFile(
			lpstrDestinationFile,				// pointer to name of the file
			GENERIC_READ|GENERIC_WRITE,		// access (read-write) mode
			0,					  // If dwShareMode is 0, the object cannot be shared. 
						          //Subsequent open operations on the object will fail,
						          //until the handle is closed. 
			NULL,                   // pointer to security attributes
			OPEN_EXISTING,          // how to create
			0,						// file attributes
			NULL                    // handle to file with attributes to copy
		);

	if(hDestFile == INVALID_HANDLE_VALUE)
	{
		wsprintf(message,"2)hDestFile is INVALID_HANDLE_VALUE; ERROR %ld\n",GetLastError());
		MessageBox(NULL,message,TEXT("Encript File To File"),MB_OK);		
		return FALSE;
	}

//2)hDestFile has just created!
//-------------------------------------------- 	
	res= Simplest(uLengthKey,szKey,hDestFile);
		
	CloseHandle(hDestFile);	 
	return res;

}/////EncriptFileToFile

bool Simplest(unsigned uLengthKey,LPSTR szKey,HANDLE hDestFile)
{
	DWORD dwFileSize;
	HANDLE hFileMapping;
	LPVOID lpFileMap;
	
	dwFileSize = GetFileSize(hDestFile,
								NULL);// pointer to high-order word for file size
	hFileMapping= CreateFileMapping(hDestFile,
		NULL,
		PAGE_READWRITE,	//The file specified by hFile must have been created
						//with GENERIC_READ and GENERIC_WRITE access.
		0,
		dwFileSize,
		NULL);			//the name of the mapping object. 

	if(hFileMapping == NULL)
		return false;

	lpFileMap=MapViewOfFile(hFileMapping,
		FILE_MAP_WRITE, //The hFileMappingObject parameter 
						//must have been created with PAGE_READWRITE protection. 
						//A read-write view of the file is mapped.
		0,0,			//an offset within the file
		0);				//Specifies the number of bytes of the file to map.
						//If dwNumberOfBytesToMap is zero, the entire file is mapped. 
	if(NULL == lpFileMap){
		CloseHandle(hFileMapping);
		return false;
	}
//|Encript
	unsigned k;
	for(k=0; k<dwFileSize; k++) ((char*)lpFileMap)[k] = ((char*)lpFileMap)[k] ^ szKey[0];	 
	 
	UnmapViewOfFile(lpFileMap);

	CloseHandle(hFileMapping);	return true;
}//Simplest

bool CopyFileToFile(HANDLE hSourceFile,HANDLE hDestFile){

	TCHAR  inBuf[512]; 
	DWORD  nBytesRead,  nBytesWritten; 
	BOOL   bResult;
	
	while(true)
	{
		bResult=ReadFile(hSourceFile,inBuf,512,&nBytesRead,	NULL);//Offset not used
			if(bResult && nBytesRead==0)
				break;//End of File
		WriteFile(hDestFile,inBuf,nBytesRead,&nBytesWritten,NULL);
	}	 
	return true;
}//CopyFileToFile

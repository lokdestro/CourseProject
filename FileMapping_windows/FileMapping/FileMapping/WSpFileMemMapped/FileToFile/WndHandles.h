// WndHandles.h: interface for the CWndHandles class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDHANDLES_H__040CBFCB_44B6_11D7_B72C_0010A4EF1954__INCLUDED_)
#define AFX_WNDHANDLES_H__040CBFCB_44B6_11D7_B72C_0010A4EF1954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

LONG WndProc_OnCreate(HWND hWnd,LPCREATESTRUCT lpCreateStruct);
void WndProc_OnDestroy(HWND hWnd);
void WndProc_OnPaint(HWND hWnd);
LONG WndProc_OnCommand (HWND hWnd,int id,HWND hwndCtl,UINT codeNotify);
void WndProc_OnClose(HWND hWnd);

#endif // !defined(AFX_WNDHANDLES_H__040CBFCB_44B6_11D7_B72C_0010A4EF1954__INCLUDED_)

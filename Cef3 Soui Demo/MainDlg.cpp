// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
	
#ifdef DWMBLUR	//win7毛玻璃开关
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#endif
#include "CefWebView/CefClientApp.h"
#include "CefWebView/Cef3WebView.h"

#define CEF_BROWSER_CLOSE_EVENT 100
	
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	#ifdef DWMBLUR	//win7毛玻璃开关
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	#endif

	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	return 0;
}
//TODO:消息映射
void CMainDlg::OnClose()
{
	// 关闭窗口之前，必须要先确保所有的浏览器实例已经全部关闭了
	if (CefClientApp::GetOpenedBrowserCount() != 0) {
		SetTimer(100, 1000);
		SOUI::SCefWebView *pCefBrowser = FindChildByName2<SOUI::SCefWebView>(L"CefBrowser");
		if (pCefBrowser != NULL) {
			pCefBrowser->Close();
		}
	} else {
		CSimpleWnd::DestroyWindow();
	}
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

void CMainDlg::OnTimer(UINT_PTR idEvent)
{
	if (idEvent == CEF_BROWSER_CLOSE_EVENT) {
		if (CefClientApp::GetOpenedBrowserCount() != 0) {
			SetTimer(CEF_BROWSER_CLOSE_EVENT, 1000);
		} else {
			CSimpleWnd::DestroyWindow();
		}
	}
}


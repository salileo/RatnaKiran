#include "stdafx.h"
#include "Ratnakiran.h"

#include "MainFrm.h"
#include "RatnakiranDoc.h"
#include "RatnakiranView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRatnakiranApp, CWinApp)
	//{{AFX_MSG_MAP(CRatnakiranApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CRatnakiranApp::CRatnakiranApp()
{
}

CRatnakiranApp theApp;

BOOL CRatnakiranApp::InitInstance()
{
	if(!CheckRegistration())
	{
		MessageBox(NULL, "This application has not been registered. Please contact the creator for a license.", "Error", MB_OK);
		return FALSE;
	}

	if(!AfxOleInit())
	{
		MessageBox(NULL, "Could not initialize COM dll.", "Error", MB_OK);
		return FALSE;
	}

#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif

	SetRegistryKey(_T("RatnaKiran - Stock Management System"));
	LoadStdProfileSettings(0);

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CRatnakiranDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CRatnakiranView));
	AddDocTemplate(pDocTemplate);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!ProcessShellCommand(cmdInfo))
	{
		UnHandledError(__FILE__, __LINE__);
		return FALSE;
	}

	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


BOOL CRatnakiranApp::CheckRegistration()
{
	CString value;
	HKEY hKey;

	char val[10];
	DWORD count = 10;

	CString main = "SOFTWARE\\Microsoft\\Stock";
	CString entry = "Value";

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, main, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		return FALSE;

	if(RegQueryValueEx(hKey, entry, NULL, NULL, (LPBYTE)val, &count) != ERROR_SUCCESS)
		return FALSE;

	RegCloseKey(hKey);
	value = val;

	if(value != "sumi")
		return FALSE;

	return TRUE;
}





class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CRatnakiranApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

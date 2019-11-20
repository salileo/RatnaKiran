#include "stdafx.h"
#include "RatnaKiran.h"
#include "RatnaKiranView.h"
#include <afxdisp.h>

#include "MainGlobal.h"
#include "AboutDialog.h"
#include "GenericTwoEditDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRatnaKiranApp, CWinApp)
	//{{AFX_MSG_MAP(CRatnaKiranApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CRatnaKiranApp::CRatnaKiranApp()
{
}

CRatnaKiranApp theApp;

BOOL CRatnaKiranApp::InitInstance()
{
	if(!CheckRegistration())
	{
		SHOW_ERROR("This application has not been registered. Please contact the creator for a license.");
		return FALSE;
	}

	if(!AfxOleInit())
	{
		SHOW_INTERNAL_ERROR_REASON("Could not initialize COM dll");
		return FALSE;
	}

#if 0
#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif
#endif

	SetRegistryKey(_T("RatnaKiran - Stock Management System"));
	LoadStdProfileSettings(0);

	gShowErrors = TRUE;

	while(TRUE)
	{
		CString section;
		if(!section.LoadString(IDS_REGISTRY_SECTION))
		{
			SHOW_INTERNAL_ERROR_REASON("Could not find registry section.");
			break;
		}

		CString entry;
		if(!entry.LoadString(IDS_SHOW_ERRORS))
		{
			SHOW_INTERNAL_ERROR_REASON("Could not find registry key 'ShowErrors'");
			break;
		}

		int val = GetProfileInt(section, entry, 2);
		if(val == 2) //not found
		{
			if(!WriteProfileInt(section, entry, 1))
				SHOW_INTERNAL_ERROR_REASON("Could not write registery values for 'ShowErrors'");
		}
		else if(val == 0)
			gShowErrors = FALSE;

		break;
	}

	GetDatabasePath();
	if(gFilePath.IsEmpty())
	{
		SHOW_ERROR("No database folder found. Quitting the application");
		return FALSE;
	}

	if(!CreateDirectory(gFilePath, NULL))
	{
		DWORD error = GetLastError();
		if(error != ERROR_ALREADY_EXISTS)
		{
			CString errstr = "Could not create the directory " + gFilePath + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

#if _DEBUG
	if(!CPermissions::SetCurrentUser("admin"))
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}
#else
	CGenericTwoEditDialog<CString, CString> userdlg("", "Username", "", "Password", "Login");
	userdlg.SetLimits1("", "", FALSE, "Please enter a username.");
	userdlg.SetProperties(Simple_Prop, Password_Prop);
	while(TRUE)
	{
		if(userdlg.DoModal() == IDCANCEL)
			return FALSE;

		if(!CPermissions::IsUserExisting(userdlg.m_val_1))
		{
			CString errstr = "User '" + userdlg.m_val_1 + "' does not exist";
			SHOW_ERROR(errstr);
			continue;
		}

		CString correct_password;
		if(!CPermissions::GetPassword(userdlg.m_val_1, &correct_password))
		{
			CString errstr = "Could not fetch details of the user '" + userdlg.m_val_1 + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			continue;
		}

		if(userdlg.m_val_2 != correct_password)
		{
			SHOW_ERROR("Invalid password.");
			continue;
		}

		if(!CPermissions::SetCurrentUser(userdlg.m_val_1))
		{
			CString errstr = "Could not set the current user as '" + userdlg.m_val_1 + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			continue;
		}

		break;
	}
#endif

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CRatnaKiranDoc),
		RUNTIME_CLASS(CMainFrame),
		RUNTIME_CLASS(CRatnaKiranView));
	AddDocTemplate(pDocTemplate);

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if(!ProcessShellCommand(cmdInfo))
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


void CRatnaKiranApp::OnAppAbout()
{
	CAboutDialog aboutDlg;
	aboutDlg.DoModal();
}


BOOL CRatnaKiranApp::OnIdle(LONG lCount) 
{
	static CTime last_executed = CTime::GetCurrentTime();
	CTime now = CTime::GetCurrentTime();

	if(gDoc)
	{
		CTimeSpan span = now - last_executed;
		if(span.GetTotalSeconds() >= 60)
		{
			if(!gDoc->WriteVouchers())
				SHOW_INTERNAL_ERROR_REASON("Could not write voucher data to file at idle time.");

			last_executed = now;
		}
	}

	return CWinApp::OnIdle(lCount);
}


BOOL CRatnaKiranApp::GetDatabasePath()
{
	gFilePath = "";

	CString section;
	if(!section.LoadString(IDS_REGISTRY_SECTION))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find registry section.");
		return FALSE;
	}

	CString entry;
	if(!entry.LoadString(IDS_FILE_PATH))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find registry key 'FilePath'");
		return FALSE;
	}

	CString str = GetProfileString(section, entry);
	if(str.IsEmpty())
	{
		if(!SetDatabasePath())
		{
			SHOW_INTERNAL_ERROR_REASON("Could not set the database path.");
			return FALSE;
		}

		str = GetProfileString(section, entry);
	}

	gFilePath = str;
	return TRUE;
}

#if !defined(BIF_NEWDIALOGSTYLE)
#define BIF_NEWDIALOGSTYLE 0x0040
#endif

BOOL CRatnaKiranApp::SetDatabasePath()
{
	if(!gFilePath.IsEmpty())
	{
		CString str = "The current file path is '" + gFilePath + "'. Do you want to change it ?";
		int retval;
		SHOW_WARNING_YESNO(str, retval);
		if(retval == IDNO)
			return TRUE;
	}

	char displayname[MAX_PATH];
	int image = 0;

	BROWSEINFO info;
	info.hwndOwner = NULL;
	info.pidlRoot = NULL;
	info.pszDisplayName = displayname;
	info.lpszTitle = "Select database folder :";
	info.lpfn = NULL;
	info.lParam = NULL;
	info.iImage = image;
	info.ulFlags = 0;//BIF_NEWDIALOGSTYLE;//BIF_EDITBOX;

	ITEMIDLIST *item = SHBrowseForFolder(&info);
	if(!item)
		return TRUE;

	if(!SHGetPathFromIDList(item, displayname))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not decipher actual path from selected option.");
		return FALSE;
	}

	CString section;
	if(!section.LoadString(IDS_REGISTRY_SECTION))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find registry section.");
		return FALSE;
	}

	CString entry;
	if(!entry.LoadString(IDS_FILE_PATH))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find registry key 'FilePath'");
		return FALSE;
	}

	CString str = displayname;
	if(!CreateDirectory(str, NULL))
	{
		DWORD error = GetLastError();
		if(error != ERROR_ALREADY_EXISTS)
		{
			CString errstr = "Could not create the directory '" + str + "'.";
			SHOW_INTERNAL_ERROR_LAST(errstr);
			return FALSE;
		}
	}

	if(!WriteProfileString(section, entry, str))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not write registery values for 'FilePath'");
		return FALSE;
	}
	
	gFilePath = str;
	return TRUE;
}


BOOL CRatnaKiranApp::CheckRegistration()
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

	if(value != "simba")
		return FALSE;

	return TRUE;
}
#include "stdafx.h"
#include "Registeration.h"
#include "RegisterationDlg.h"
#include <shlobj.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CRegisterationApp, CWinApp)
	//{{AFX_MSG_MAP(CRegisterationApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CRegisterationApp theApp;

static BOOL SelfDelete()
{
  SHELLEXECUTEINFO sei;

  TCHAR szModule [MAX_PATH],
        szComspec[MAX_PATH],
        szParams [MAX_PATH];

  // get file path names:
  if((GetModuleFileName(0,szModule,MAX_PATH)!=0) &&
     (GetShortPathName(szModule,szModule,MAX_PATH)!=0) &&
     (GetEnvironmentVariable("COMSPEC",szComspec,MAX_PATH)!=0))
  {
    // set command shell parameters
    lstrcpy(szParams,"/c del ");
    lstrcat(szParams, szModule);
    lstrcat(szParams, " > nul");

    // set struct members
    sei.cbSize       = sizeof(sei);
    sei.hwnd         = 0;
    sei.lpVerb       = "Open";
    sei.lpFile       = szComspec;
    sei.lpParameters = szParams;
    sei.lpDirectory  = 0;
    sei.nShow        = SW_HIDE;
    sei.fMask        = SEE_MASK_NOCLOSEPROCESS;

    // increase resource allocation to program
    SetPriorityClass(GetCurrentProcess(),
                     REALTIME_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(),
                      THREAD_PRIORITY_TIME_CRITICAL);

    // invoke command shell
    if(ShellExecuteEx(&sei))
    {
      // suppress command shell process until program exits
      SetPriorityClass(sei.hProcess,IDLE_PRIORITY_CLASS);
      SetProcessPriorityBoost(sei.hProcess,TRUE);

      // notify explorer shell of deletion
      SHChangeNotify(SHCNE_DELETE,SHCNF_PATH,szModule,0);
      return TRUE;
    }
    else // if error, normalize allocation
    {
      SetPriorityClass(GetCurrentProcess(),
                       NORMAL_PRIORITY_CLASS);
      SetThreadPriority(GetCurrentThread(),
                        THREAD_PRIORITY_NORMAL);
    }
  }

  return FALSE;
}

CRegisterationApp::CRegisterationApp()
{
	success = FALSE;
}

BOOL CRegisterationApp::InitInstance()
{
	CRegisterationDlg dlg;
	m_pMainWnd = &dlg;

	if(dlg.DoModal() == IDOK)
		success = TRUE;

	return FALSE;
}

int CRegisterationApp::ExitInstance() 
{
	if(success)
		SelfDelete();

	return CWinApp::ExitInstance();
}


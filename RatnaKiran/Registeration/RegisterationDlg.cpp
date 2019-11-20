#include "stdafx.h"
#include "Registeration.h"
#include "RegisterationDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CRegisterationDlg::CRegisterationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegisterationDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRegisterationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisterationDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegisterationDlg, CDialog)
	//{{AFX_MSG_MAP(CRegisterationDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CRegisterationDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog())
		return FALSE;

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	return TRUE;
}

void CRegisterationDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


HCURSOR CRegisterationDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRegisterationDlg::OnOK() 
{
	if(SetRegisteration())
	{
		MessageBox("Done");
		CDialog::OnOK();
	}
	else
	{
		MessageBox("Could not perform registeration.");
		CDialog::OnCancel();
	}
}

BOOL CRegisterationDlg::SetRegisteration()
{
	CString main = "SOFTWARE\\Microsoft";
	CString section = "Stock";
	CString entry = "Value";

	char *value = "simba";

	DWORD dwDisposition;
	HKEY hKey;
	HKEY hkSub;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, main, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return FALSE;

	if(RegCreateKeyEx(hKey, section, 0, "", 0, KEY_ALL_ACCESS, NULL, &hkSub, &dwDisposition) != ERROR_SUCCESS)
		return FALSE;

	if(RegSetValueEx(hkSub, entry, 0, REG_SZ, (LPBYTE)value, (DWORD)(strlen(value) + 1)) != ERROR_SUCCESS)
		return FALSE;

	RegCloseKey(hkSub);
	RegCloseKey(hKey);
	return TRUE;
}

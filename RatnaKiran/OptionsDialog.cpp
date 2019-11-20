#include "stdafx.h"
#include "RatnaKiran.h"
#include "MainGlobal.h"
#include "OptionsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


COptionsDialog::COptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDialog)
	//}}AFX_DATA_INIT

	m_filename = "";
}


void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	//{{AFX_MSG_MAP(COptionsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL COptionsDialog::OnInitDialog() 
{
	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}
	
	CheckRadioButton(IDC_OPTION_PRINT, IDC_OPTION_SAVEFILE, IDC_OPTION_PRINT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsDialog::OnOK() 
{
	int option = GetCheckedRadioButton(IDC_OPTION_PRINT, IDC_OPTION_SAVEFILE);
	switch(option)
	{
	case IDC_OPTION_PRINT:
		m_filename.Empty();
		break;
	case IDC_OPTION_SAVEFILE:
		m_filename = GetFileName("Excel Document (*.xls)","*.xls","xls",FALSE);
		if(m_filename.IsEmpty())
			return;
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", option);
			errstr = "Invalid selected option '" + errstr + "'.";
			SHOW_INTERNAL_ERROR;
			return;
		}
	}
	
	CDialog::OnOK();
}

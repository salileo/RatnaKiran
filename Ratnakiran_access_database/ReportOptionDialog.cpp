#include "stdafx.h"
#include "RatnaKiran.h"
#include "ReportOptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CReportOptionDialog::CReportOptionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CReportOptionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportOptionDialog)
	//}}AFX_DATA_INIT

	m_filename = "";
}


void CReportOptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportOptionDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportOptionDialog, CDialog)
	//{{AFX_MSG_MAP(CReportOptionDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CReportOptionDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CheckRadioButton(IDC_OPTION_PRINT, IDC_OPTION_SAVEFILE, IDC_OPTION_PRINT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportOptionDialog::OnOK() 
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
	}
	
	CDialog::OnOK();
}

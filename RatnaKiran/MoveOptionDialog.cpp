#include "stdafx.h"
#include "RatnaKiran.h"
#include "DatabaseGlobals.h"
#include "MoveOptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMoveOptionDialog::CMoveOptionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMoveOptionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMoveOptionDialog)
	//}}AFX_DATA_INIT
}

void CMoveOptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveOptionDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveOptionDialog, CDialog)
	//{{AFX_MSG_MAP(CMoveOptionDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMoveOptionDialog::OnInitDialog() 
{
	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CheckRadioButton(IDC_STOCK, IDC_KUNDAN, IDC_STOCK);
	return TRUE;
}

void CMoveOptionDialog::OnOK() 
{
	int option = GetCheckedRadioButton(IDC_STOCK, IDC_KUNDAN);
	switch(option)
	{
	case IDC_STOCK:
		m_type = tStock;
		break;
	case IDC_EXPORT:
		m_type = tExport;
		break;
	case IDC_KUNDAN:
		m_type = tKundan;
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
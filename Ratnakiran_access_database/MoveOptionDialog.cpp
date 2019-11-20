#include "stdafx.h"
#include "RatnaKiran.h"
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
	CDialog::OnInitDialog();
	CheckRadioButton(IDC_STOCK, IDC_KUNDAN, IDC_STOCK);
	return TRUE;
}

void CMoveOptionDialog::OnOK() 
{
	int option = GetCheckedRadioButton(IDC_STOCK, IDC_KUNDAN);
	switch(option)
	{
	case IDC_STOCK:
		m_type = STOCK_TYPE;
		break;
	case IDC_EXPORT:
		m_type = EXPORT_TYPE;
		break;
	case IDC_KUNDAN:
		m_type = KUNDAN_TYPE;
		break;
	}
	
	CDialog::OnOK();
}
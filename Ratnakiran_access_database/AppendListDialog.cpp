#include "stdafx.h"
#include "RatnaKiran.h"
#include "AppendListDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAppendListDialog::CAppendListDialog(CString type, CWnd* pParent /*=NULL*/)
	: CDialog(CAppendListDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppendListDialog)
	//}}AFX_DATA_INIT

	m_type = type;
	m_selection = "";
}


void CAppendListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppendListDialog)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAppendListDialog, CDialog)
	//{{AFX_MSG_MAP(CAppendListDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CAppendListDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!MakeGlobalsValid())
	{
		UnHandledError(__FILE__, __LINE__);
		CDialog::OnCancel();
		return FALSE;
	}

	CList<CString,CString> list;
	if(!gDoc->GetVoucherList(m_type, &list))
	{
		UnHandledError(__FILE__, __LINE__);
		CDialog::OnCancel();
		return FALSE;
	}

	if(list.IsEmpty())
	{
		MessageBox("No vouchers existing, please create a new voucher first.", "ERROR");
		CDialog::OnCancel();
		return FALSE;
	}

	POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		CString str = list.GetAt(pos);
		m_list.AddString(str);
		list.GetNext(pos);
	}

	m_list.SetCurSel(0);
	return TRUE;
}


void CAppendListDialog::OnOK() 
{
	int selection = m_list.GetCurSel();
	if(selection == CB_ERR)
	{
		MessageBox("Please select an entry before proceeding.", "ERROR");
		return;
	}

	m_list.GetLBText(selection, m_selection);
	CDialog::OnOK();
}

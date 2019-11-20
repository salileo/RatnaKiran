#include "stdafx.h"
#include "RatnaKiran.h"
#include "DatabaseGlobals.h"
#include "AppendListDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAppendListDialog::CAppendListDialog(CList<CProductVoucher *, CProductVoucher *> *list, CWnd* pParent /*=NULL*/)
	: CDialog(CAppendListDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppendListDialog)
	//}}AFX_DATA_INIT

	m_voucher_list = list;
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
	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}
	
	if(!m_voucher_list)
	{
		SHOW_INTERNAL_ERROR;
		CDialog::OnCancel();
		return FALSE;
	}

	if(m_voucher_list->IsEmpty() || (m_voucher_list->GetCount() == 1)) //if it is empty or has just 1 (the global) voucher
	{
		SHOW_ERROR("No vouchers existing, please create a new voucher first.");
		CDialog::OnCancel();
		return FALSE;
	}

	POSITION pos = m_voucher_list->GetHeadPosition();
	while(pos)
	{
		CProductVoucher *voucher = m_voucher_list->GetAt(pos);
		if(voucher)
		{
			if((CString(voucher->m_voucher_name) == STOCK_ROOT_NAME) ||
			   (CString(voucher->m_voucher_name) == KUNDAN_ROOT_NAME) ||
			   (CString(voucher->m_voucher_name) == EXPORT_ROOT_NAME))
			{
				//don't do anything
			}
			else if(m_list.AddString(voucher->m_voucher_name) == CB_ERR)
			{
				CString errstr = "Could not add voucher '" + CString(voucher->m_voucher_name) + "' to the list.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		m_voucher_list->GetNext(pos);
	}

	if(m_list.GetCount() > 0)
	{
		if(m_list.SetCurSel(0) == CB_ERR)
			SHOW_INTERNAL_ERROR_REASON("Could not select the first entry in the combo box.");
	}

	return TRUE;
}


void CAppendListDialog::OnOK() 
{
	int selection = m_list.GetCurSel();
	if(selection == CB_ERR)
	{
		SHOW_ERROR("Please select an entry before proceeding.");
		return;
	}

	m_list.GetLBText(selection, m_selection);
	CDialog::OnOK();
}

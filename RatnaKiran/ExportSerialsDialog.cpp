#include "stdafx.h"
#include "RatnaKiran.h"
#include "MainGlobal.h"
#include "GenericListBoxDialog.h"
#include "ExportSerialsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExportSerialsDialog::CExportSerialsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExportSerialsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportSerialsDialog)
	m_from = 0;
	m_to = -1;
	m_tagname = _T("");
	m_use_old = FALSE;
	m_use_new = TRUE;
	m_use_productID = FALSE;
	//}}AFX_DATA_INIT
}

void CExportSerialsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportSerialsDialog)
	DDX_Text(pDX, IDC_FROM, m_from);
	DDV_MinMaxInt(pDX, m_from, -1, 2147483647);
	DDX_Text(pDX, IDC_TO, m_to);
	DDV_MinMaxInt(pDX, m_to, -1, 2147483647);
	DDX_Text(pDX, IDC_TAGNAME, m_tagname);
	DDX_Check(pDX, IDC_USE_OLD, m_use_old);
	DDX_Check(pDX, IDC_USE_NEW, m_use_new);
	DDX_Check(pDX, IDC_USE_PRODUCTID, m_use_productID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportSerialsDialog, CDialog)
	//{{AFX_MSG_MAP(CExportSerialsDialog)
	ON_BN_CLICKED(IDC_USE_OLD, OnUseOld)
	ON_BN_CLICKED(IDC_TAGNAMES, OnTagnames)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CExportSerialsDialog::OnInitDialog() 
{
	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}
	
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		CDialog::OnCancel();
		return FALSE;
	}

	m_tagnames.RemoveAll();
	POSITION pos = gDoc->m_export_vouchers.GetHeadPosition();
	while(pos)
	{
		CProductVoucher *voucher = gDoc->m_export_vouchers.GetAt(pos);
		if(voucher)
		{
			if(!voucher->IsRootVoucher() && !gDoc->IsIgnored(voucher->m_voucher_name))
			{
				SStringPairs detail;
				detail.m_string1 = voucher->m_voucher_name;
				m_tagnames.AddTail(detail);
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		gDoc->m_export_vouchers.GetNext(pos);
	}

	if(gDoc->m_selected_voucher->IsRootVoucher())
	{
		CWnd *editbox = GetDlgItem(IDC_TAGNAME);
		if(editbox)
			editbox->ShowWindow(FALSE);
		else
			SHOW_INTERNAL_ERROR_REASON("Could not find tagname control.");

		CWnd *text = GetDlgItem(IDC_STATIC_TAGNAME);
		if(text)
			text->ShowWindow(FALSE);
		else
			SHOW_INTERNAL_ERROR_REASON("Could not find tagname string control.");

		CButton *button = (CButton *)GetDlgItem(IDC_TAGNAMES);
		if(button)
			button->EnableWindow(m_use_old);
		else
			SHOW_INTERNAL_ERROR_REASON("Could not find tagnames button control.");
	}
	else
	{
		CWnd *button = GetDlgItem(IDC_TAGNAMES);
		if(button)
			button->ShowWindow(FALSE);
		else
			SHOW_INTERNAL_ERROR_REASON("Could not find tagnames button control.");

		CEdit *ctrl = (CEdit *)GetDlgItem(IDC_TAGNAME);
		if(ctrl)
			ctrl->SetReadOnly(!m_use_old);
		else
			SHOW_INTERNAL_ERROR_REASON("Could not find tagname control.");
	}

	UpdateData(FALSE);
	return TRUE;
}


void CExportSerialsDialog::OnUseOld() 
{
	if(!UpdateData(TRUE))
		return;

	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		CDialog::OnCancel();
		return;
	}

	if(gDoc->m_selected_voucher->IsRootVoucher())
	{
		CButton *button = (CButton *)GetDlgItem(IDC_TAGNAMES);
		if(button)
			button->EnableWindow(m_use_old);
		else
			SHOW_INTERNAL_ERROR_REASON("Could not find tagnames button control.");
	}
	else
	{
		CEdit *ctrl = (CEdit *)GetDlgItem(IDC_TAGNAME);
		if(ctrl)
			ctrl->SetReadOnly(!m_use_old);
		else
			SHOW_INTERNAL_ERROR_REASON("Could not find tagname control.");
	}
}


void CExportSerialsDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if((m_from != -1) && (m_to != -1))
	{
		if(m_from > m_to)
		{
			int temp = m_to;
			m_to = m_from;
			m_from = temp;
		}
	}
	else if(m_from == -1)
	{
		if(m_to == -1)
		{
			m_from = 0;
		}
		else
		{
			int temp = m_from;
			m_from = m_to;
			m_to = temp;
		}
	}
	else if(m_to == -1)
	{
		//Nothing to do
	}
		
	UpdateData(FALSE);
	CDialog::OnOK();
}


void CExportSerialsDialog::OnTagnames() 
{
	CGenericListBoxDialog tagnameDlg(TagnamesList);
	tagnameDlg.m_itemlist = &m_tagnames;
	tagnameDlg.DoModal();
}

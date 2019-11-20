#include "stdafx.h"
#include "RatnaKiran.h"
#include "DetailsDialogStock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDetailsDialogStock::CDetailsDialogStock(BOOL show_common_only, CWnd* pParent /*=NULL*/)
	: CDialog(CDetailsDialogStock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailsDialogStock)
	m_gold_rate = 0.0;
	m_price_quote = _T("");
	//}}AFX_DATA_INIT

	m_show_common = show_common_only;
}


void CDetailsDialogStock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailsDialogStock)
	DDX_Text(pDX, IDC_STOCK_GOLD_RATE, m_gold_rate);
	DDX_Text(pDX, IDC_STOCK_PRICE_QUOTE, m_price_quote);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailsDialogStock, CDialog)
	//{{AFX_MSG_MAP(CDetailsDialogStock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDetailsDialogStock::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(!MakeGlobalsValid())
	{
		UnHandledError(__FILE__, __LINE__);
		CDialog::OnCancel();
		return FALSE;
	}

	m_gold_rate = (gDoc->m_vouchers.m_record.m_Gold_rate == -1.0) ? 0.0 : gDoc->m_vouchers.m_record.m_Gold_rate;
	m_price_quote = gDoc->m_vouchers.m_record.m_Price_quotation_formula;
	
	if(m_show_common)
	{
		CEdit *editCtrl = (CEdit *)GetDlgItem(IDC_KUNDAN_PRICE_QUOTE);
		if(editCtrl)
			editCtrl->SetReadOnly();
	}

	UpdateData(FALSE);
	return TRUE;
}


void CDetailsDialogStock::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(!MakeGlobalsValid())
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	if(!gDoc->m_vouchers.DBEditEntry())
		return;

	gDoc->m_vouchers.m_record.m_Gold_rate = (m_gold_rate <= 0.0) ? -1.0 : m_gold_rate;
	gDoc->m_vouchers.m_record.m_Price_quotation_formula = m_price_quote;

	if(!gDoc->m_vouchers.DBUpdateEntry(""))
		return;

	CDialog::OnOK();
}

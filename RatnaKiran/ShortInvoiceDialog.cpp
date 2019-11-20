#include "stdafx.h"
#include "RatnaKiran.h"
#include "ShortInvoiceDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CShortInvoiceDialog::CShortInvoiceDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CShortInvoiceDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShortInvoiceDialog)
	m_buyer = _T("");
	m_gr_number = _T("");
	m_order_number = _T("");
	m_place_receipt = _T("");
	m_precarriage = _T("");
	m_terms = _T("");
	m_vessel = _T("");
	m_exporter_gr_number = _T("");
	//}}AFX_DATA_INIT
}


void CShortInvoiceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShortInvoiceDialog)
	DDX_Text(pDX, IDC_EXPORTS_BUYER, m_buyer);
	DDX_Text(pDX, IDC_EXPORTS_GR_NUMBER, m_gr_number);
	DDX_Text(pDX, IDC_EXPORTS_ORDER_NUMBER, m_order_number);
	DDX_Text(pDX, IDC_EXPORTS_PLACE_RECEIPT, m_place_receipt);
	DDX_Text(pDX, IDC_EXPORTS_PRECARRIAGE, m_precarriage);
	DDX_Text(pDX, IDC_EXPORTS_TERMS, m_terms);
	DDX_Text(pDX, IDC_EXPORTS_VESSEL, m_vessel);
	DDX_Text(pDX, IDC_EXPORTS_EXPORTER_GR_NUMBER, m_exporter_gr_number);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShortInvoiceDialog, CDialog)
	//{{AFX_MSG_MAP(CShortInvoiceDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CShortInvoiceDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	m_buyer.Remove('\r');
	m_terms.Remove('\r');

	UpdateData(FALSE);
	CDialog::OnOK();
}

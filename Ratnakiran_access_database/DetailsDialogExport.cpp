#include "stdafx.h"
#include "RatnaKiran.h"
#include "DetailsDialogExport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDetailsDialogExport::CDetailsDialogExport(BOOL show_common_only, CWnd* pParent /*=NULL*/)
	: CDialog(CDetailsDialogExport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailsDialogExport)
	m_consignee = _T("");
	m_conversion_rate = 0.0;
	m_country = _T("");
	m_currency = _T("");
	m_gold_rate = 0;
	m_invoice_number = _T("");
	m_diamond_rate = _T("");
	m_pearl_rate = _T("");
	m_price_quote = _T("");
	m_stone_rate = _T("");
	m_value_added_labour = _T("");
	m_usd_rate = 0.0;
	m_exporter = _T("");
	//}}AFX_DATA_INIT

	m_show_common = show_common_only;
}


void CDetailsDialogExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailsDialogExport)
	DDX_Text(pDX, IDC_EXPORTS_CONSIGNEE, m_consignee);
	DDX_Text(pDX, IDC_EXPORTS_EXPORTER, m_exporter);
	DDX_Text(pDX, IDC_EXPORTS_CONVERSION_RATE, m_conversion_rate);
	DDX_Text(pDX, IDC_EXPORTS_COUNTRY, m_country);
	DDX_Text(pDX, IDC_EXPORTS_CURRENCY, m_currency);
	DDX_Text(pDX, IDC_EXPORTS_GOLD_RATE, m_gold_rate);
	DDX_Text(pDX, IDC_EXPORTS_INVOICE, m_invoice_number);
	DDX_Text(pDX, IDC_FORMULAS_DIAMOND_RATE, m_diamond_rate);
	DDX_Text(pDX, IDC_FORMULAS_PEARL_RATE, m_pearl_rate);
	DDX_Text(pDX, IDC_FORMULAS_PRICE_QUOTE, m_price_quote);
	DDX_Text(pDX, IDC_FORMULAS_STONE_RATE, m_stone_rate);
	DDX_Text(pDX, IDC_FORMULAS_VALUE_ADDED_LABOUR, m_value_added_labour);
	DDX_Text(pDX, IDC_EXPORTS_USD_RATE, m_usd_rate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailsDialogExport, CDialog)
	//{{AFX_MSG_MAP(CDetailsDialogExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDetailsDialogExport::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(!MakeGlobalsValid())
	{
		UnHandledError(__FILE__, __LINE__);
		CDialog::OnCancel();
		return FALSE;
	}

	m_consignee = gDoc->m_vouchers.m_record.m_Consignee;
	m_consignee.Replace("\n", "\r\n");

	m_exporter = gDoc->m_vouchers.m_record.m_Exporter;
	m_exporter.Replace("\n", "\r\n");

	m_country = gDoc->m_vouchers.m_record.m_Country;
	m_currency = gDoc->m_vouchers.m_record.m_Currency_name;
	m_conversion_rate = gDoc->m_vouchers.m_record.m_Currency_conversion_rate;
	m_usd_rate = gDoc->m_vouchers.m_record.m_Dollar_rate;
	m_invoice_number = gDoc->m_vouchers.m_record.m_Invoice_number;

	m_gold_rate = (gDoc->m_vouchers.m_record.m_Gold_rate == -1.0) ? 0.0 : ((gDoc->m_vouchers.m_record.m_Gold_rate * 40.0) / (m_usd_rate * 3.0));

	m_diamond_rate = gDoc->m_vouchers.m_record.m_Exports_diamond_rate_formula;
	m_pearl_rate = gDoc->m_vouchers.m_record.m_Exports_pearl_rate_formula;
	m_stone_rate = gDoc->m_vouchers.m_record.m_Exports_stone_rate_formula;
	m_value_added_labour = gDoc->m_vouchers.m_record.m_Exports_labour_cost_formula;
	m_price_quote = gDoc->m_vouchers.m_record.m_Price_quotation_formula;


	if(m_show_common)
	{
		CEdit *editCtrl = (CEdit *)GetDlgItem(IDC_FORMULAS_DIAMOND_RATE);
		if(editCtrl)
			editCtrl->SetReadOnly();

		editCtrl = (CEdit *)GetDlgItem(IDC_FORMULAS_STONE_RATE);
		if(editCtrl)
			editCtrl->SetReadOnly();

		editCtrl = (CEdit *)GetDlgItem(IDC_FORMULAS_PEARL_RATE);
		if(editCtrl)
			editCtrl->SetReadOnly();

		editCtrl = (CEdit *)GetDlgItem(IDC_FORMULAS_VALUE_ADDED_LABOUR);
		if(editCtrl)
			editCtrl->SetReadOnly();

		editCtrl = (CEdit *)GetDlgItem(IDC_FORMULAS_PRICE_QUOTE);
		if(editCtrl)
			editCtrl->SetReadOnly();
	}
	
	UpdateData(FALSE);
	return TRUE;
}


void CDetailsDialogExport::OnOK() 
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

	m_consignee.Remove('\r');
	gDoc->m_vouchers.m_record.m_Consignee = m_consignee;

	m_exporter.Remove('\r');
	gDoc->m_vouchers.m_record.m_Exporter = m_exporter;

	gDoc->m_vouchers.m_record.m_Country = m_country;
	gDoc->m_vouchers.m_record.m_Currency_name = m_currency;
	gDoc->m_vouchers.m_record.m_Invoice_number = m_invoice_number;

	gDoc->m_vouchers.m_record.m_Exports_diamond_rate_formula = m_diamond_rate;
	gDoc->m_vouchers.m_record.m_Exports_stone_rate_formula = m_stone_rate;
	gDoc->m_vouchers.m_record.m_Exports_pearl_rate_formula = m_pearl_rate;
	gDoc->m_vouchers.m_record.m_Exports_labour_cost_formula = m_value_added_labour;
	gDoc->m_vouchers.m_record.m_Price_quotation_formula = m_price_quote;

	m_conversion_rate = (m_conversion_rate <= 0.0) ? 1.0 : m_conversion_rate;
	gDoc->m_vouchers.m_record.m_Currency_conversion_rate = m_conversion_rate;

	m_usd_rate = (m_usd_rate <= 0.0) ? 1.0 : m_usd_rate;
	gDoc->m_vouchers.m_record.m_Dollar_rate = m_usd_rate;

	m_gold_rate = (m_gold_rate <= 0.0) ? -1.0 : ((m_gold_rate * m_usd_rate * 3.0) / 40.0); //((value * dollar_rate) / 10) * (18 / 24)
	gDoc->m_vouchers.m_record.m_Gold_rate = m_gold_rate;
	
	if(!gDoc->m_vouchers.DBUpdateEntry(""))
		return;

	CDialog::OnOK();
}
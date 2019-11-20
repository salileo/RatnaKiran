#include "stdafx.h"
#include "ratnakiran.h"
#include "DatabaseGlobals.h"
#include "DetailsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDetailsDialog::CDetailsDialog(DatabaseType type, CProductVoucher *voucher, BOOL show_common_only, CWnd* pParent /*=NULL*/)
: CDialog(((type == tStock) ? IDD_DETAILS_DIALOG_STOCK : ((type == tKundan) ? IDD_DETAILS_DIALOG_KUNDAN : ((type == tExport) ? IDD_DETAILS_DIALOG_EXPORT : 0))), pParent)
{
	//{{AFX_DATA_INIT(CDetailsDialog)
	m_gold_rate = 0;
	m_price_quote = _T("");
	m_consignee = _T("");
	m_conversion_rate = 0.0;
	m_country = _T("");
	m_currency = _T("");
	m_invoice_number = _T("");
	m_diamond_rate = _T("");
	m_pearl_rate = _T("");
	m_stone_rate = _T("");
	m_value_added_labour = _T("");
	m_usd_rate = 0.0;
	m_exporter = _T("");
	//}}AFX_DATA_INIT

	m_type = type;
	m_voucher = voucher;
	m_show_common = show_common_only;
}


void CDetailsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailsDialog)
	switch(m_type)
	{
	case tStock:
		DDX_Text(pDX, IDC_STOCK_GOLD_RATE, m_gold_rate);
		DDX_Text(pDX, IDC_FORMULAS_PRICE_QUOTE, m_price_quote);
		break;
	case tKundan:
		DDX_Text(pDX, IDC_KUNDAN_GOLD_RATE, m_gold_rate);
		DDX_Text(pDX, IDC_FORMULAS_PRICE_QUOTE, m_price_quote);
		break;
	case tExport:
		DDX_Text(pDX, IDC_EXPORT_CONSIGNEE, m_consignee);
		DDX_Text(pDX, IDC_EXPORT_EXPORTER, m_exporter);
		DDX_Text(pDX, IDC_EXPORT_CONVERSION_RATE, m_conversion_rate);
		DDX_Text(pDX, IDC_EXPORT_COUNTRY, m_country);
		DDX_Text(pDX, IDC_EXPORT_CURRENCY, m_currency);
		DDX_Text(pDX, IDC_EXPORT_USD_RATE, m_usd_rate);
		DDX_Text(pDX, IDC_EXPORT_GOLD_RATE, m_gold_rate);
		DDX_Text(pDX, IDC_EXPORT_INVOICE, m_invoice_number);
		DDX_Text(pDX, IDC_FORMULAS_DIAMOND_RATE, m_diamond_rate);
		DDX_Text(pDX, IDC_FORMULAS_PEARL_RATE, m_pearl_rate);
		DDX_Text(pDX, IDC_FORMULAS_PRICE_QUOTE, m_price_quote);
		DDX_Text(pDX, IDC_FORMULAS_STONE_RATE, m_stone_rate);
		DDX_Text(pDX, IDC_FORMULAS_VALUE_ADDED_LABOUR, m_value_added_labour);
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailsDialog, CDialog)
	//{{AFX_MSG_MAP(CDetailsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDetailsDialog::OnInitDialog() 
{
	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!m_voucher)
	{
		SHOW_INTERNAL_ERROR;
		CDialog::OnCancel();
		return FALSE;
	}

	switch(m_type)
	{
	case tStock:
		m_gold_rate = (m_voucher->m_gold_rate == -1.0) ? 0.0 : m_voucher->m_gold_rate;
		m_price_quote = m_voucher->m_price_quote;
		break;
	case tKundan:
		m_gold_rate = (m_voucher->m_gold_rate == -1.0) ? 0.0 : m_voucher->m_gold_rate;
		m_price_quote = m_voucher->m_price_quote;
		break;
	case tExport:
		m_consignee = m_voucher->m_consignee;
		m_consignee.Replace("\n", "\r\n");
		m_exporter = m_voucher->m_exporter;
		m_exporter.Replace("\n", "\r\n");
		m_conversion_rate = m_voucher->m_currency_conversion;
		m_usd_rate = m_voucher->m_dollar_rate;
		m_country = m_voucher->m_country;
		m_currency = m_voucher->m_currency_name;
		m_invoice_number = m_voucher->m_invoice_number;
		m_gold_rate = (m_voucher->m_gold_rate == -1.0) ? 0.0 : ((m_voucher->m_gold_rate * 40.0) / (m_usd_rate * 3.0));
		m_diamond_rate = m_voucher->m_diamond_rate;
		m_pearl_rate = m_voucher->m_pearl_rate;
		m_stone_rate = m_voucher->m_stone_rate;
		m_value_added_labour = m_voucher->m_labour_cost;
		m_price_quote = m_voucher->m_price_quote;

		if(m_show_common)
		{
			CEdit *editCtrl = (CEdit *)GetDlgItem(IDC_FORMULAS_DIAMOND_RATE);
			if(editCtrl)
				editCtrl->SetReadOnly();
			else
				SHOW_INTERNAL_ERROR_REASON("Could not find diamond rate control.");

			editCtrl = (CEdit *)GetDlgItem(IDC_FORMULAS_PEARL_RATE);
			if(editCtrl)
				editCtrl->SetReadOnly();
			else
				SHOW_INTERNAL_ERROR_REASON("Could not find pearl rate control.");

			editCtrl = (CEdit *)GetDlgItem(IDC_FORMULAS_PRICE_QUOTE);
			if(editCtrl)
				editCtrl->SetReadOnly();
			else
				SHOW_INTERNAL_ERROR_REASON("Could not find price quote control.");

			editCtrl = (CEdit *)GetDlgItem(IDC_FORMULAS_STONE_RATE);
			if(editCtrl)
				editCtrl->SetReadOnly();
			else
				SHOW_INTERNAL_ERROR_REASON("Could not find stone rate control.");

			editCtrl = (CEdit *)GetDlgItem(IDC_FORMULAS_VALUE_ADDED_LABOUR);
			if(editCtrl)
				editCtrl->SetReadOnly();
			else
				SHOW_INTERNAL_ERROR_REASON("Could not find value added labour control.");
		}

		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	UpdateData(FALSE);
	return TRUE;
}


void CDetailsDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(!m_voucher)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	switch(m_type)
	{
	case tStock:
		m_voucher->m_gold_rate = (m_gold_rate <= 0.0) ? -1.0 : m_gold_rate;
		strcpy(m_voucher->m_price_quote, LPCTSTR(m_price_quote));
		break;
	case tKundan:
		m_voucher->m_gold_rate = (m_gold_rate <= 0.0) ? -1.0 : m_gold_rate;
		strcpy(m_voucher->m_price_quote, LPCTSTR(m_price_quote));
		break;
	case tExport:
		m_consignee.Remove('\r');
		strcpy(m_voucher->m_consignee , LPCTSTR(m_consignee));
		m_exporter.Remove('\r');
		strcpy(m_voucher->m_exporter , LPCTSTR(m_exporter));
		strcpy(m_voucher->m_country , LPCTSTR(m_country));
		strcpy(m_voucher->m_invoice_number , LPCTSTR(m_invoice_number));
		strcpy(m_voucher->m_currency_name , LPCTSTR(m_currency));
		strcpy(m_voucher->m_diamond_rate, LPCTSTR(m_diamond_rate));
		strcpy(m_voucher->m_pearl_rate, LPCTSTR(m_pearl_rate));
		strcpy(m_voucher->m_stone_rate, LPCTSTR(m_stone_rate));
		strcpy(m_voucher->m_labour_cost, LPCTSTR(m_value_added_labour));
		strcpy(m_voucher->m_price_quote, LPCTSTR(m_price_quote));
		m_conversion_rate = (m_conversion_rate <= 0.0) ? 1.0 : m_conversion_rate;
		m_voucher->m_currency_conversion = m_conversion_rate;
		m_usd_rate = (m_usd_rate <= 0.0) ? 1.0 : m_usd_rate;
		m_voucher->m_dollar_rate = m_usd_rate;
		m_gold_rate = (m_gold_rate <= 0.0) ? -1.0 : ((m_gold_rate * m_usd_rate * 3.0) / 40.0); //((value * dollar_rate) / 10) * (18 / 24)
		m_voucher->m_gold_rate = m_gold_rate;
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}
	
	CDialog::OnOK();
}
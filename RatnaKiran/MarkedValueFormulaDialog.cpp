#include "stdafx.h"
#include "RatnaKiran.h"
#include "DatabaseGlobals.h"
#include "MarkedValueFormulaDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMarkedValueFormulaDialog::CMarkedValueFormulaDialog(DatabaseType type, CProductItem *item, double gold_rate, CWnd* pParent /*=NULL*/)
: CDialog(((type == tStock) ? IDD_MARKED_VALUE_FORMULA_DIALOG_STOCK : ((type == tKundan) ? IDD_MARKED_VALUE_FORMULA_DIALOG_KUNDAN : 0)), pParent)
{
	//{{AFX_DATA_INIT(CMarkedValueFormulaDialog)
	m_costing = _T("");
	m_diamond = _T("");
	m_labour = _T("");
	m_pearl = _T("");
	m_precious_stone = _T("");
	m_marked_price = 0;
	//}}AFX_DATA_INIT

	m_type = type;
	m_gold_rate = gold_rate;
	m_original_item = item;

}


void CMarkedValueFormulaDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkedValueFormulaDialog)
	switch(m_type)
	{
	case tStock:
		DDX_Text(pDX, IDC_FORMULAS_DIAMOND, m_diamond);
		DDX_Text(pDX, IDC_FORMULAS_LABOUR, m_labour);
		DDX_Text(pDX, IDC_FORMULAS_PEARL, m_pearl);
		DDX_Text(pDX, IDC_FORMULAS_PRECIOUS_STONE, m_precious_stone);
		DDX_Text(pDX, IDC_FORMULAS_MARKED_PRICE, m_marked_price);
		break;
	case tKundan:
		DDX_Text(pDX, IDC_FORMULAS_COSTING, m_costing);
		break;
	case tExport:
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


BEGIN_MESSAGE_MAP(CMarkedValueFormulaDialog, CDialog)
	//{{AFX_MSG_MAP(CMarkedValueFormulaDialog)
	ON_EN_CHANGE(IDC_FORMULAS_COSTING, OnChangeFormulas)
	ON_EN_CHANGE(IDC_FORMULAS_DIAMOND, OnChangeFormulas)
	ON_EN_CHANGE(IDC_FORMULAS_LABOUR, OnChangeFormulas)
	ON_EN_CHANGE(IDC_FORMULAS_MARKED_PRICE, OnChangeFormulas)
	ON_EN_CHANGE(IDC_FORMULAS_PEARL, OnChangeFormulas)
	ON_EN_CHANGE(IDC_FORMULAS_PRECIOUS_STONE, OnChangeFormulas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMarkedValueFormulaDialog::OnInitDialog() 
{
	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}
	
	if(!m_original_item)
	{
		SHOW_INTERNAL_ERROR;
		CDialog::OnCancel();
		return FALSE;
	}

	m_item = *m_original_item;

	switch(m_type)
	{
	case tStock:
		m_diamond = m_item.m_diamond_marked_value;
		m_labour = m_item.m_labour_marked_cost;
		m_pearl = m_item.m_pearl_marked_value;
		m_precious_stone = m_item.m_stone_marked_value;
		m_marked_price = m_item.GetTotalMarkedValue(m_item.m_use_net_weight_for_gold, TRUE, m_gold_rate, 1.0);
		break;
	case tKundan:
		m_costing = m_item.m_marked_costing;
		m_marked_price = m_item.GetTotalMarkedCosting(1.0);
		break;
	case tExport:
		return FALSE;
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


void CMarkedValueFormulaDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(!m_original_item)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	switch(m_type)
	{
	case tStock:
		strcpy(m_original_item->m_diamond_marked_value, LPCTSTR(m_diamond));
		strcpy(m_original_item->m_labour_marked_cost, LPCTSTR(m_labour));
		strcpy(m_original_item->m_pearl_marked_value, LPCTSTR(m_pearl));
		strcpy(m_original_item->m_stone_marked_value, LPCTSTR(m_precious_stone));
		break;
	case tKundan:
		strcpy(m_original_item->m_marked_costing, LPCTSTR(m_costing));
		break;
	case tExport:
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


void CMarkedValueFormulaDialog::OnChangeFormulas() 
{
	if(!UpdateData(TRUE))
		return;

	switch(m_type)
	{
	case tStock:
		strcpy(m_item.m_diamond_marked_value, LPCTSTR(m_diamond));
		strcpy(m_item.m_labour_marked_cost, LPCTSTR(m_labour));
		strcpy(m_item.m_pearl_marked_value, LPCTSTR(m_pearl));
		strcpy(m_item.m_stone_marked_value, LPCTSTR(m_precious_stone));
		m_marked_price = m_item.GetTotalMarkedValue(m_item.m_use_net_weight_for_gold, TRUE, m_gold_rate, 1.0);
		break;
	case tKundan:
		strcpy(m_item.m_marked_costing, LPCTSTR(m_costing));
		m_marked_price = m_item.GetTotalMarkedCosting(1.0);
		break;
	case tExport:
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

	UpdateData(FALSE);
}

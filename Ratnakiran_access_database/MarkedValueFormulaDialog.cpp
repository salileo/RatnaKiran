#include "stdafx.h"
#include "RatnaKiran.h"
#include "MarkedValueFormulaDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMarkedValueFormulaDialog::CMarkedValueFormulaDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkedValueFormulaDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkedValueFormulaDialog)
	m_diamond = _T("");
	m_stone = _T("");
	m_pearl = _T("");
	m_labour = _T("");
	m_marked_price = 0;
	//}}AFX_DATA_INIT

	m_gold_rate = 0.0;
	m_original_diamond = _T("");
	m_original_stone = _T("");
	m_original_pearl = _T("");
	m_original_labour = _T("");
}


void CMarkedValueFormulaDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkedValueFormulaDialog)
	DDX_Text(pDX, IDC_FORMULAS_DIAMOND, m_diamond);
	DDX_Text(pDX, IDC_FORMULAS_STONE, m_stone);
	DDX_Text(pDX, IDC_FORMULAS_PEARL, m_pearl);
	DDX_Text(pDX, IDC_FORMULAS_LABOUR, m_labour);
	DDX_Text(pDX, IDC_FORMULAS_MARKED_PRICE, m_marked_price);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarkedValueFormulaDialog, CDialog)
	//{{AFX_MSG_MAP(CMarkedValueFormulaDialog)
	ON_EN_CHANGE(IDC_FORMULAS_DIAMOND, OnChangeFormulas)
	ON_EN_CHANGE(IDC_FORMULAS_STONE, OnChangeFormulas)
	ON_EN_CHANGE(IDC_FORMULAS_PEARL, OnChangeFormulas)
	ON_EN_CHANGE(IDC_FORMULAS_LABOUR, OnChangeFormulas)
	ON_EN_CHANGE(IDC_FORMULAS_MARKED_PRICE, OnChangeFormulas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMarkedValueFormulaDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(!MakeGlobalsValid())
	{
		UnHandledError(__FILE__, __LINE__);
		CDialog::OnCancel();
		return FALSE;
	}
	
	m_gold_rate = gDoc->m_vouchers.GetGoldRate();
	CTableItem *item = &(gDoc->m_vouchers.m_sets.m_items);

	m_original_diamond = m_diamond = item->m_record.m_Diamond_marked_value;
	m_original_stone = m_stone = item->m_record.m_Stone_marked_value;
	m_original_pearl = m_pearl = item->m_record.m_Pearl_marked_value;
	m_original_labour = m_labour = item->m_record.m_Labour_marked_value;
	
	m_marked_price = ROUNDUP(item->GetTotalMarkedValueDouble(item->m_record.m_Use_net_weight_for_gold, TRUE, m_gold_rate, 1.0));

	UpdateData(FALSE);
	return TRUE;
}


void CMarkedValueFormulaDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(!MakeGlobalsValid())
		return;

	CTableItem *item = &(gDoc->m_vouchers.m_sets.m_items);

	if(!item->DBEditEntry())
		return;

	item->m_record.m_Diamond_marked_value = m_diamond;
	item->m_record.m_Stone_marked_value = m_stone;
	item->m_record.m_Pearl_marked_value = m_pearl;
	item->m_record.m_Labour_marked_value = m_labour;

	if(!item->DBUpdateEntry(""))
		return;

	CDialog::OnOK();
}


void CMarkedValueFormulaDialog::OnChangeFormulas() 
{
	if(!UpdateData(TRUE))
		return;

	if(!MakeGlobalsValid())
		return;

	CTableItem *item = &(gDoc->m_vouchers.m_sets.m_items);

	item->m_record.m_Diamond_marked_value = m_diamond;
	item->m_record.m_Stone_marked_value = m_stone;
	item->m_record.m_Pearl_marked_value = m_pearl;
	item->m_record.m_Labour_marked_value = m_labour;

	m_marked_price = ROUNDUP(item->GetTotalMarkedValueDouble(item->m_record.m_Use_net_weight_for_gold, TRUE, m_gold_rate, 1.0));

	UpdateData(FALSE);
}

void CMarkedValueFormulaDialog::OnCancel() 
{
	if(MakeGlobalsValid())
	{
		CTableItem *item = &(gDoc->m_vouchers.m_sets.m_items);
		item->m_record.m_Diamond_marked_value = m_original_diamond;
		item->m_record.m_Stone_marked_value = m_original_stone;
		item->m_record.m_Pearl_marked_value = m_original_pearl;
		item->m_record.m_Labour_marked_value = m_original_labour;
	}
	
	CDialog::OnCancel();
}

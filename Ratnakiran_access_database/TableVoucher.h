#if !defined(AFX_TABLEVOUCHER_H__3814178B_DC14_4FDE_BEEA_C01CE4C29509__INCLUDED_)
#define AFX_TABLEVOUCHER_H__3814178B_DC14_4FDE_BEEA_C01CE4C29509__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TableSet.h"

#define STOCK_ROOT "Stock Vouchers"
#define EXPORT_ROOT "Export Vouchers"
#define KUNDAN_ROOT "Kundan Vouchers"

#define STOCK_MAIN "stock_main"
#define EXPORT_MAIN "export_main"
#define KUNDAN_MAIN "kundan_main"

#define STOCK_TYPE "Stock"
#define EXPORT_TYPE "Export"
#define KUNDAN_TYPE "Kundan"

class CRecordVoucher
{
public:
	CString	m_Voucher_ID;
	CString	m_Type;
	long	m_Set_count;
	double	m_Gold_rate;
	CString	m_Invoice_number;
	CString	m_Consignee;
	CString	m_Exporter;
	CString	m_Country;
	CString	m_Currency_name;
	double	m_Currency_conversion_rate;
	double	m_Dollar_rate;
	CString	m_Exports_labour_cost_formula;
	CString	m_Exports_diamond_rate_formula;
	CString	m_Exports_pearl_rate_formula;
	CString	m_Exports_stone_rate_formula;
	CString	m_Price_quotation_formula;

	CRecordVoucher()
	{
		m_Voucher_ID = _T("");
		m_Type = _T("");
		m_Set_count = 0;
		m_Gold_rate = 0.0;
		m_Invoice_number = _T("");
		m_Consignee = _T("");
		m_Exporter = _T("");
		m_Country = _T("");
		m_Currency_name = _T("");
		m_Currency_conversion_rate = 0.0;
		m_Dollar_rate = 0.0;
		m_Exports_diamond_rate_formula = _T("");
		m_Exports_stone_rate_formula = _T("");
		m_Exports_pearl_rate_formula = _T("");
		m_Exports_labour_cost_formula = _T("");
		m_Price_quotation_formula = _T("");
	}

	CRecordVoucher(CRecordVoucher &src)
	{
		m_Voucher_ID = src.m_Voucher_ID;
		m_Type = src.m_Type;
		m_Set_count = src.m_Set_count;
		m_Gold_rate = src.m_Gold_rate;
		m_Invoice_number = src.m_Invoice_number;
		m_Consignee = src.m_Consignee;
		m_Exporter = src.m_Exporter;
		m_Country = src.m_Country;
		m_Currency_name = src.m_Currency_name;
		m_Currency_conversion_rate = src.m_Currency_conversion_rate;
		m_Dollar_rate = src.m_Dollar_rate;
		m_Exports_diamond_rate_formula = src.m_Exports_diamond_rate_formula;
		m_Exports_stone_rate_formula = src.m_Exports_stone_rate_formula;
		m_Exports_pearl_rate_formula = src.m_Exports_pearl_rate_formula;
		m_Exports_labour_cost_formula = src.m_Exports_labour_cost_formula;
		m_Price_quotation_formula = src.m_Price_quotation_formula;
	}

	operator=(CRecordVoucher &src)
	{
		m_Voucher_ID = src.m_Voucher_ID;
		m_Type = src.m_Type;
		m_Set_count = src.m_Set_count;
		m_Gold_rate = src.m_Gold_rate;
		m_Invoice_number = src.m_Invoice_number;
		m_Consignee = src.m_Consignee;
		m_Exporter = src.m_Exporter;
		m_Country = src.m_Country;
		m_Currency_name = src.m_Currency_name;
		m_Currency_conversion_rate = src.m_Currency_conversion_rate;
		m_Dollar_rate = src.m_Dollar_rate;
		m_Exports_diamond_rate_formula = src.m_Exports_diamond_rate_formula;
		m_Exports_stone_rate_formula = src.m_Exports_stone_rate_formula;
		m_Exports_pearl_rate_formula = src.m_Exports_pearl_rate_formula;
		m_Exports_labour_cost_formula = src.m_Exports_labour_cost_formula;
		m_Price_quotation_formula = src.m_Price_quotation_formula;
	}

	~CRecordVoucher()
	{
	}
};

class CTableVoucher : public CRecordset
{
public:
	CTableVoucher(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTableVoucher)

	//{{AFX_FIELD(CTableVoucher, CRecordset)
	//}}AFX_FIELD


	//{{AFX_VIRTUAL(CTableVoucher)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

private:
	DatabaseState RequerySetsFor(CString voucher);

public:
	CRecordVoucher	m_record;
	CTableSet		m_sets;

	BOOL DBOpen(CString in_func, BOOL open_children);
	BOOL DBClose(BOOL close_children);
	BOOL DBBeginTransaction(BOOL include_children);
	BOOL DBRollbackTransaction(BOOL include_children);
	BOOL DBCommitTransaction(BOOL include_children);

	BOOL DBAddEntry();
	BOOL DBEditEntry();
	BOOL DBUpdateEntry(CString in_func);
	BOOL DBDeleteEntry();
	DatabaseState DBRequeryEntries(CString in_func, BOOL preserve_state);

	DatabaseState GotoFirstVoucher(BOOL requery = TRUE);
	DatabaseState GotoNextVoucher(BOOL requery = TRUE);
	DatabaseState GotoPrevVoucher(BOOL requery = TRUE);
	DatabaseState GotoLastVoucher(BOOL requery = TRUE);
	DatabaseState GotoPosVoucher(int pos, BOOL requery = TRUE);
	DatabaseState GotoVoucher(CString ID, BOOL requery = TRUE);
	
	BOOL GetList(CString type, CList<CString, CString> *list);
	double GetGoldRate();

	BOOL AddSet(CRecordSet new_set);
	BOOL MoveSet(CString from, CString to);
	BOOL DeleteSet(CString set_ID);
	BOOL DeleteAllSet();
	BOOL DeleteEmptySets();
	BOOL GetSetList(CString productID, CList<CString, CString> *setlist, CList<CString, CString> *voucherlist = NULL);
	int GetSetCount(BOOL force_requery);

	BOOL IsRootVoucher();
	BOOL AssignSerialNumbers(int *starting);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_TABLEVOUCHER_H__3814178B_DC14_4FDE_BEEA_C01CE4C29509__INCLUDED_)

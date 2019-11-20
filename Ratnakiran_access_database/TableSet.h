#if !defined(AFX_TABLESET_H__26C0B4D2_F7F6_48FE_8177_07A78D77CEF2__INCLUDED_)
#define AFX_TABLESET_H__26C0B4D2_F7F6_48FE_8177_07A78D77CEF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TableItem.h"

class CRecordSet
{
public:
	CRecordSet()
	{
		m_Set_ID = _T("");
		m_Voucher_ID = _T("");
		m_Description = _T("");
		m_Item_count_sold = 0;
		m_Item_count_unsold = 0;
	}

	CRecordSet(CRecordSet &src)
	{
		m_Set_ID = src.m_Set_ID;
		m_Voucher_ID = src.m_Voucher_ID;
		m_Description = src.m_Description;
		m_Item_count_sold = src.m_Item_count_sold;
		m_Item_count_unsold = src.m_Item_count_unsold;
	}

	operator=(CRecordSet &src)
	{
		m_Set_ID = src.m_Set_ID;
		m_Voucher_ID = src.m_Voucher_ID;
		m_Description = src.m_Description;
		m_Item_count_sold = src.m_Item_count_sold;
		m_Item_count_unsold = src.m_Item_count_unsold;
	}

	~CRecordSet()
	{
	}

	CString	m_Set_ID;
	CString	m_Voucher_ID;
	CString	m_Description;
	long	m_Item_count_sold;
	long	m_Item_count_unsold;
};

class CTableSet : public CRecordset
{
public:
	CTableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTableSet)

	//{{AFX_FIELD(CTableSet, CRecordset)
	//}}AFX_FIELD

	//{{AFX_VIRTUAL(CTableSet)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

private:
	DatabaseState RequeryItemsFor(CString set_ID);

public:
	CRecordSet		m_record;
	CTableItem		m_items;

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

	DatabaseState GotoFirstSet(BOOL requery = TRUE);
	DatabaseState GotoNextSet(BOOL requery = TRUE);
	DatabaseState GotoPrevSet(BOOL requery = TRUE);
	DatabaseState GotoLastSet(BOOL requery = TRUE);
	DatabaseState GotoPosSet(int pos, BOOL requery = TRUE);
	DatabaseState GotoSet(CString ID, BOOL requery = TRUE);

	BOOL AddItem(CRecordItem new_item, BOOL update_ID);
	BOOL DeleteItem(long item_identifier);
	BOOL DeleteAllItems();
	BOOL DeleteSoldItems();
	int GetItemCount(Sold_Status type, BOOL force_requery);
	int GetTotalItemCount(Sold_Status type);
	BOOL HasEmptyItems();

	BOOL UpdateProductID(CString productID, BOOL showmessage);
	BOOL UpdateItemIDs();
	BOOL UpdateItemSerials(int *starting);
	BOOL MarkSetAsSold(BOOL sold, CString country);
	BOOL StoreOldSerialNumbers(CString tagname);
	BOOL ResetOldSerialNumbers();
	CString GenerateSerialString(int *lowest_serial);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_TABLESET_H__26C0B4D2_F7F6_48FE_8177_07A78D77CEF2__INCLUDED_)

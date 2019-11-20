#if !defined(AFX_TABLEITEM_H__B47D864F_9DB2_415B_9BC8_C15FC785E114__INCLUDED_)
#define AFX_TABLEITEM_H__B47D864F_9DB2_415B_9BC8_C15FC785E114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TableSubItem.h"
enum Sold_Status {Sold, UnSold, Both};

class CRecordOldSerial
{
public:
	CRecordOldSerial()
	{
		m_tag = "";
		m_serial = 0;
	}

	CRecordOldSerial(CRecordOldSerial &src)
	{
		m_tag = src.m_tag;
		m_serial = src.m_serial;
	}

	operator=(CRecordOldSerial &src)
	{
		m_tag = "";
		m_serial = 0;
	}

	~CRecordOldSerial()
	{
	}

	CString m_tag;
	int m_serial;
};

class CRecordItem
{
public:
	CRecordItem()
	{
		m_Identifier = 0;
		m_Item_ID = _T("");
		m_Set_ID = _T("");
		m_Description = _T("");
		m_Piece_count = 0;
		m_Serial_number = 0;
		m_Old_serials = _T("");
		m_Photo_filename = _T("");
		m_Gold_rate = 0.0;
		m_Use_net_weight_for_gold = FALSE;
		m_Gross_weight = 0.0;
		m_Other_weight = 0.0;
		m_Labour_cost = 0;
		m_Diamond_marked_value = _T("");
		m_Stone_marked_value = _T("");
		m_Pearl_marked_value = _T("");
		m_Labour_marked_value = _T("");
		m_Sold = FALSE;
		m_Sold_in_country = _T("");

		m_Photo.m_hData = NULL;
		m_Photo.m_dwDataLength = 0;
	}

	CRecordItem(CRecordItem &src)
	{
		m_Identifier = src.m_Identifier;
		m_Item_ID = src.m_Item_ID;
		m_Set_ID = src.m_Set_ID;
		m_Description = src.m_Description;
		m_Piece_count = src.m_Piece_count;
		m_Serial_number = src.m_Serial_number;
		m_Old_serials = src.m_Old_serials;
		m_Photo_filename = src.m_Photo_filename;
		m_Gold_rate = src.m_Gold_rate;
		m_Use_net_weight_for_gold = src.m_Use_net_weight_for_gold;
		m_Gross_weight = src.m_Gross_weight;
		m_Other_weight = src.m_Other_weight;
		m_Labour_cost = src.m_Labour_cost;
		m_Diamond_marked_value = src.m_Diamond_marked_value;
		m_Stone_marked_value = src.m_Stone_marked_value;
		m_Pearl_marked_value = src.m_Pearl_marked_value;
		m_Labour_marked_value = src.m_Labour_marked_value;
		m_Sold = src.m_Sold;
		m_Sold_in_country = src.m_Sold_in_country;

		m_Photo.m_dwDataLength = src.m_Photo.m_dwDataLength;
		m_Photo.m_hData = (HGLOBAL)malloc(src.m_Photo.m_dwDataLength);
		memcpy(m_Photo.m_hData, src.m_Photo.m_hData, src.m_Photo.m_dwDataLength);
	}

	operator=(CRecordItem &src)
	{
		m_Identifier = src.m_Identifier;
		m_Item_ID = src.m_Item_ID;
		m_Set_ID = src.m_Set_ID;
		m_Description = src.m_Description;
		m_Piece_count = src.m_Piece_count;
		m_Serial_number = src.m_Serial_number;
		m_Old_serials = src.m_Old_serials;
		m_Photo_filename = src.m_Photo_filename;
		m_Gold_rate = src.m_Gold_rate;
		m_Use_net_weight_for_gold = src.m_Use_net_weight_for_gold;
		m_Gross_weight = src.m_Gross_weight;
		m_Other_weight = src.m_Other_weight;
		m_Labour_cost = src.m_Labour_cost;
		m_Diamond_marked_value = src.m_Diamond_marked_value;
		m_Stone_marked_value = src.m_Stone_marked_value;
		m_Pearl_marked_value = src.m_Pearl_marked_value;
		m_Labour_marked_value = src.m_Labour_marked_value;
		m_Sold = src.m_Sold;
		m_Sold_in_country = src.m_Sold_in_country;

		m_Photo.m_dwDataLength = src.m_Photo.m_dwDataLength;
		if(m_Photo.m_hData)
		{
			free(m_Photo.m_hData);
			m_Photo.m_hData = NULL;
		}

		m_Photo.m_hData = (HGLOBAL)malloc(src.m_Photo.m_dwDataLength);
		memcpy(m_Photo.m_hData, src.m_Photo.m_hData, src.m_Photo.m_dwDataLength);
	}

	~CRecordItem()
	{
		m_Photo.m_dwDataLength = 0;
		if(m_Photo.m_hData)
		{
			free(m_Photo.m_hData);
			m_Photo.m_hData = NULL;
		}
	}

	long	m_Identifier;
	CString	m_Item_ID;
	CString	m_Set_ID;
	CString	m_Description;
	int		m_Piece_count;
	int		m_Serial_number;
	CString	m_Old_serials;
	CString	m_Photo_filename;
	CLongBinary	m_Photo;
	double	m_Gold_rate;
	BOOL	m_Use_net_weight_for_gold;
	double	m_Gross_weight;
	double	m_Other_weight;
	long	m_Labour_cost;
	CString	m_Diamond_marked_value;
	CString	m_Stone_marked_value;
	CString	m_Pearl_marked_value;
	CString	m_Labour_marked_value;
	BOOL	m_Sold;
	CString	m_Sold_in_country;
};

class CTableItem : public CRecordset
{
public:
	CTableItem(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTableItem)

	//{{AFX_FIELD(CTableItem, CRecordset)
	//}}AFX_FIELD

	//{{AFX_VIRTUAL(CTableItem)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

private:
	DatabaseState RequerySubItemsFor(long item_identifier);

public:
	CRecordItem		m_record;
	CTableSubItem	m_sub_items;

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

	DatabaseState GotoFirstItem(BOOL requery = TRUE);
	DatabaseState GotoNextItem(BOOL requery = TRUE);
	DatabaseState GotoPrevItem(BOOL requery = TRUE);
	DatabaseState GotoLastItem(BOOL requery = TRUE);
	DatabaseState GotoPosItem(int pos, BOOL requery = TRUE);
	DatabaseState GotoItem(CString ID, BOOL requery = TRUE);
	DatabaseState GotoItem(int val, BOOL use_serial, BOOL requery = TRUE);

	BOOL AddSubItem(CString category, CRecordSubItem new_sub_item);
	BOOL DeleteSubItem(long sub_item_identifier);
	BOOL DeleteSubItemList(CString catagory);
	BOOL DeleteAllSubItem();
	int GetSubItemCount(CString catagory);

	BOOL UpdateProductID(CString productID, BOOL showmessage);

	BOOL AddImage(DWORD size, BYTE *data);
	BOOL DeleteImage();
	static void ScaleImage(CSize original, CSize desired, CSize *final);

	void GetSerialList(CList<CRecordOldSerial, CRecordOldSerial> *list);
	BOOL PutSerialList(CList<CRecordOldSerial, CRecordOldSerial> *list);
	int GetOldSerial(CString tagname);

	BOOL StoreOldSerialNumber(CString tagname);
	BOOL ResetOldSerialNumber();

	BOOL MarkItemAsSold(BOOL sold, CString country);
	BOOL IsItemSold();
	BOOL IsEmpty();

	double GetGrossWeight();
	double GetNetWeight();
	double GetGoldValueDouble(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion);

	double GetSubItemWeight(CString catagory);
	double GetSubItemValueDouble(CString catagory, double coversion);
	double GetSubItemValueUsingDouble(CString catagory, int rate, double conversion);
	int GetSubItemRate(CString catagory, double conversion);
	double GetMarkedSubItemValueDouble(CString catagory, double gold_rate, double conversion);
	int GetMarkedSubItemRate(CString catagory, double gold_rate, double conversion);

	double GetLabourCostDouble(double conversion);
	double GetMarkedLabourCostDouble(double gold_rate, double conversion);
	double GetOtherWeight();

	double GetTotalValueDouble(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion);
	double GetTotalMarkedValueDouble(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_TABLEITEM_H__B47D864F_9DB2_415B_9BC8_C15FC785E114__INCLUDED_)

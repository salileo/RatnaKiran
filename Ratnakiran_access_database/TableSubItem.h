#if !defined(AFX_TABLESUBITEM_H__401B97F6_4254_48F5_88CB_0952492199A3__INCLUDED_)
#define AFX_TABLESUBITEM_H__401B97F6_4254_48F5_88CB_0952492199A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DIAMOND "Diamond"
#define STONE "Stone"
#define PEARL "Pearl"

enum DatabaseState { DBInvalid = 0, DBOK, DBEmpty };
#define MAX_LONG 1246576928

class CRecordSubItem
{
public:
	CRecordSubItem()
	{
		m_Identifier = 0;
		m_Item_identifier = 0;
		m_Item_ID = _T("");
		m_Category = _T("");
		m_Type = _T("");
		m_Weight = 0.0;
		m_Rate = 0;
	}

	CRecordSubItem(CRecordSubItem &src)
	{
		m_Identifier = src.m_Identifier;
		m_Item_identifier = src.m_Item_identifier;
		m_Item_ID = src.m_Item_ID;
		m_Category = src.m_Category;
		m_Type = src.m_Type;
		m_Weight = src.m_Weight;
		m_Rate = src.m_Rate;
	}

	operator=(CRecordSubItem &src)
	{
		m_Identifier = src.m_Identifier;
		m_Item_identifier = src.m_Item_identifier;
		m_Item_ID = src.m_Item_ID;
		m_Category = src.m_Category;
		m_Type = src.m_Type;
		m_Weight = src.m_Weight;
		m_Rate = src.m_Rate;
	}

	~CRecordSubItem()
	{
	}

	long	m_Identifier;
	long	m_Item_identifier;
	CString	m_Item_ID;
	CString	m_Category;
	CString	m_Type;
	double	m_Weight;
	long	m_Rate;
};

class CTableSubItem : public CRecordset
{
public:
	CTableSubItem(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTableSubItem)

	//{{AFX_FIELD(CTableSubItem, CRecordset)
	//}}AFX_FIELD


	//{{AFX_VIRTUAL(CTableSubItem)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

public:
	CRecordSubItem m_record;

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

	DatabaseState GotoFirstSubItem(BOOL requery = TRUE);
	DatabaseState GotoNextSubItem(BOOL requery = TRUE);
	DatabaseState GotoPrevSubItem(BOOL requery = TRUE);
	DatabaseState GotoLastSubItem(BOOL requery = TRUE);
	DatabaseState GotoPosSubItem(int pos, BOOL requery = TRUE);
	DatabaseState GotoSubItem(long identifier, BOOL requery = TRUE);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_TABLESUBITEM_H__401B97F6_4254_48F5_88CB_0952492199A3__INCLUDED_)

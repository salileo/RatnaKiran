#if !defined(AFX_TABLEDESCRIPTION_H__55DAF729_864D_49C6_A269_ABD5CD77E636__INCLUDED_)
#define AFX_TABLEDESCRIPTION_H__55DAF729_864D_49C6_A269_ABD5CD77E636__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTableDescription : public CRecordset
{
public:
	CTableDescription(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTableDescription)

	//{{AFX_FIELD(CTableDescription, CRecordset)
	CString	m_Description;
	CString	m_Prefix;
	int		m_Piece_count;
	//}}AFX_FIELD

	//{{AFX_VIRTUAL(CTableDescription)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	BOOL AddEntry();
	BOOL EditEntry();
	BOOL UpdateEntry(CString in_func);
	BOOL DeleteEntry();

	DatabaseState RequeryEntry(CString in_func, BOOL preserve_state = FALSE);

	DatabaseState GotoFirstEntry();
	DatabaseState GotoNextEntry();
	DatabaseState GotoPrevEntry();
	DatabaseState GotoLastEntry();
	DatabaseState GotoPosEntry(int pos);
	DatabaseState GotoEntry(CString desc);

	BOOL AddDescription(CString desc, CString prefix, int piece_count);
	BOOL DeleteAll();

	CString GetPrefix(CString desc);
	int GetPieceCount(CString desc);

	static CString ExtractActualID(CString ID);
	static CString ExtractPrefix(CString ID);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_TABLEDESCRIPTION_H__55DAF729_864D_49C6_A269_ABD5CD77E636__INCLUDED_)

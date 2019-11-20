#if !defined(AFX_TABLESETTINGS_H__AF7DD413_283F_4ABF_976A_110D0C56EE5D__INCLUDED_)
#define AFX_TABLESETTINGS_H__AF7DD413_283F_4ABF_976A_110D0C56EE5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTableSettings : public CRecordset
{
public:
	CTableSettings(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTableSettings)

	//{{AFX_FIELD(CTableSettings, CRecordset)
	long	m_ID;
	double	m_Gold_rate;
	BOOL	m_Use_native_currency;
	//}}AFX_FIELD

	//{{AFX_VIRTUAL(CTableSettings)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	BOOL EditEntry();
	BOOL UpdateEntry(CString in_func);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_TABLESETTINGS_H__AF7DD413_283F_4ABF_976A_110D0C56EE5D__INCLUDED_)

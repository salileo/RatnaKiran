#if !defined(AFX_EXPORTSERIALSDIALOG_H__F05D1E5E_97D4_4216_BDDB_387D187A5AD6__INCLUDED_)
#define AFX_EXPORTSERIALSDIALOG_H__F05D1E5E_97D4_4216_BDDB_387D187A5AD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExportSerialsDialog : public CDialog
{
public:
	CExportSerialsDialog(CWnd* pParent = NULL);
	CList<SStringPairs, SStringPairs> m_tagnames;

	//{{AFX_DATA(CExportSerialsDialog)
	enum { IDD = IDD_EXPORT_SERIALS_DIALOG };
	int		m_from;
	int		m_to;
	CString	m_tagname;
	BOOL	m_use_old;
	BOOL	m_use_new;
	BOOL	m_use_productID;
	//}}AFX_DATA

private:
	//{{AFX_VIRTUAL(CExportSerialsDialog)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExportSerialsDialog)
	afx_msg void OnUseOld();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTagnames();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_EXPORTSERIALSDIALOG_H__F05D1E5E_97D4_4216_BDDB_387D187A5AD6__INCLUDED_)

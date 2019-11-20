#if !defined(AFX_MARKEDVALUEFORMULADIALOG_H__8D10640E_0C9F_4D3F_86EF_AF6608756A3E__INCLUDED_)
#define AFX_MARKEDVALUEFORMULADIALOG_H__8D10640E_0C9F_4D3F_86EF_AF6608756A3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMarkedValueFormulaDialog : public CDialog
{
public:
	CMarkedValueFormulaDialog(DatabaseType type, CProductItem *item, double gold_rate, CWnd* pParent = NULL);

private:
	DatabaseType m_type;
	CProductItem *m_original_item;
	CProductItem m_item;
	double m_gold_rate;

	//{{AFX_DATA(CMarkedValueFormulaDialog)
	CString	m_costing;
	CString	m_diamond;
	CString	m_labour;
	CString	m_pearl;
	CString	m_precious_stone;
	int		m_marked_price;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMarkedValueFormulaDialog)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMarkedValueFormulaDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeFormulas();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_MARKEDVALUEFORMULADIALOG_H__8D10640E_0C9F_4D3F_86EF_AF6608756A3E__INCLUDED_)

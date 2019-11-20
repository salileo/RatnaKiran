#if !defined(AFX_MARKEDVALUEFORMULADIALOG_H__8D10640E_0C9F_4D3F_86EF_AF6608756A3E__INCLUDED_)
#define AFX_MARKEDVALUEFORMULADIALOG_H__8D10640E_0C9F_4D3F_86EF_AF6608756A3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMarkedValueFormulaDialog : public CDialog
{
public:
	CMarkedValueFormulaDialog(CWnd* pParent = NULL);

	//{{AFX_DATA(CMarkedValueFormulaDialog)
	enum { IDD = IDD_MARKED_VALUE_FORMULA_DIALOG };
	CString	m_diamond;
	CString	m_stone;
	CString	m_pearl;
	CString	m_labour;
	int		m_marked_price;
	//}}AFX_DATA

	double m_gold_rate;
	CString	m_original_diamond;
	CString	m_original_stone;
	CString	m_original_pearl;
	CString	m_original_labour;

	//{{AFX_VIRTUAL(CMarkedValueFormulaDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMarkedValueFormulaDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeFormulas();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_MARKEDVALUEFORMULADIALOG_H__8D10640E_0C9F_4D3F_86EF_AF6608756A3E__INCLUDED_)

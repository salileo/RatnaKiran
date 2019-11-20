#if !defined(AFX_DETAILSDIALOGKUNDAN_H__AD3AE828_BA2E_4FE1_AED3_1F5526D10A04__INCLUDED_)
#define AFX_DETAILSDIALOGKUNDAN_H__AD3AE828_BA2E_4FE1_AED3_1F5526D10A04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDetailsDialogKundan : public CDialog
{
public:
	CDetailsDialogKundan(BOOL show_common_only = FALSE, CWnd* pParent = NULL);   // standard constructor

	//{{AFX_DATA(CDetailsDialogKundan)
	enum { IDD = IDD_DETAILS_DIALOG_KUNDAN };
	double	m_gold_rate;
	CString	m_price_quote;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDetailsDialogKundan)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL m_show_common;
	
protected:
	//{{AFX_MSG(CDetailsDialogKundan)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DETAILSDIALOGKUNDAN_H__AD3AE828_BA2E_4FE1_AED3_1F5526D10A04__INCLUDED_)

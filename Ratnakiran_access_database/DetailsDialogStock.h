#if !defined(AFX_DETAILSDIALOGSTOCK_H__CC11F600_C0BE_4D70_A500_34F83E37F46F__INCLUDED_)
#define AFX_DETAILSDIALOGSTOCK_H__CC11F600_C0BE_4D70_A500_34F83E37F46F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDetailsDialogStock : public CDialog
{
public:
	CDetailsDialogStock(BOOL show_common_only = FALSE, CWnd* pParent = NULL);

	//{{AFX_DATA(CDetailsDialogStock)
	enum { IDD = IDD_DETAILS_DIALOG_STOCK };
	double	m_gold_rate;
	CString	m_price_quote;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDetailsDialogStock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL m_show_common;

protected:
	//{{AFX_MSG(CDetailsDialogStock)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DETAILSDIALOGSTOCK_H__CC11F600_C0BE_4D70_A500_34F83E37F46F__INCLUDED_)

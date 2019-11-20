#if !defined(AFX_DETAILSDIALOGEXPORT_H__92A2B1A3_30CD_4BCE_A06A_D9B4A249581B__INCLUDED_)
#define AFX_DETAILSDIALOGEXPORT_H__92A2B1A3_30CD_4BCE_A06A_D9B4A249581B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDetailsDialogExport : public CDialog
{
public:
	CDetailsDialogExport(BOOL show_common_only = FALSE, CWnd* pParent = NULL);

	//{{AFX_DATA(CDetailsDialogExport)
	enum { IDD = IDD_DETAILS_DIALOG_EXPORT };
	CString	m_consignee;
	double	m_conversion_rate;
	CString	m_country;
	CString	m_currency;
	double	m_gold_rate;
	CString	m_invoice_number;
	CString	m_diamond_rate;
	CString	m_pearl_rate;
	CString	m_price_quote;
	CString	m_stone_rate;
	CString	m_value_added_labour;
	double	m_usd_rate;
	CString	m_exporter;
	//}}AFX_DATA

	BOOL m_show_common;

	//{{AFX_VIRTUAL(CDetailsDialogExport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CDetailsDialogExport)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DETAILSDIALOGEXPORT_H__92A2B1A3_30CD_4BCE_A06A_D9B4A249581B__INCLUDED_)

#if !defined(AFX_DETAILSDIALOG_H__8EA1C353_834B_4A85_943C_6182D0CB4768__INCLUDED_)
#define AFX_DETAILSDIALOG_H__8EA1C353_834B_4A85_943C_6182D0CB4768__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDetailsDialog : public CDialog
{
public:
	CDetailsDialog(DatabaseType type, CProductVoucher *voucher, BOOL show_common_only = FALSE, CWnd* pParent = NULL);

private:
	DatabaseType m_type;
	CProductVoucher *m_voucher;
	BOOL m_show_common;

	//{{AFX_DATA(CDetailsDialog)
	double	m_gold_rate;
	CString	m_price_quote;
	CString	m_consignee;
	double	m_conversion_rate;
	CString	m_country;
	CString	m_currency;
	CString	m_invoice_number;
	CString	m_diamond_rate;
	CString	m_pearl_rate;
	CString	m_stone_rate;
	CString	m_value_added_labour;
	double	m_usd_rate;
	CString	m_exporter;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDetailsDialog)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDetailsDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_DETAILSDIALOG_H__8EA1C353_834B_4A85_943C_6182D0CB4768__INCLUDED_)

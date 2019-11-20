#if !defined(AFX_SHORTINVOICEDIALOG_H__C553AA05_7AD1_4F0E_B6C0_F8880281A124__INCLUDED_)
#define AFX_SHORTINVOICEDIALOG_H__C553AA05_7AD1_4F0E_B6C0_F8880281A124__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShortInvoiceDialog : public CDialog
{
public:
	CShortInvoiceDialog(CWnd* pParent = NULL);

	//{{AFX_DATA(CShortInvoiceDialog)
	enum { IDD = IDD_SHORTINVOICE_DETAILS };
	CString	m_buyer;
	CString	m_gr_number;
	CString	m_order_number;
	CString	m_place_receipt;
	CString	m_precarriage;
	CString	m_terms;
	CString	m_vessel;
	CString	m_exporter_gr_number;
	//}}AFX_DATA

private:
	//{{AFX_VIRTUAL(CShortInvoiceDialog)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CShortInvoiceDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_SHORTINVOICEDIALOG_H__C553AA05_7AD1_4F0E_B6C0_F8880281A124__INCLUDED_)

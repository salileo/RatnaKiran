#if !defined(AFX_APPENDLISTDIALOG_H__DB019A8B_C930_44FD_A9ED_3F6EB100936C__INCLUDED_)
#define AFX_APPENDLISTDIALOG_H__DB019A8B_C930_44FD_A9ED_3F6EB100936C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAppendListDialog : public CDialog
{
public:
	CAppendListDialog(CList<CProductVoucher *, CProductVoucher *> *list, CWnd* pParent = NULL);

	CString m_selection;

private:
	CList<CProductVoucher *, CProductVoucher *> *m_voucher_list;

	//{{AFX_DATA(CAppendListDialog)
	enum { IDD = IDD_APPENDLIST_DIALOG };
	CComboBox	m_list;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAppendListDialog)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAppendListDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_APPENDLISTDIALOG_H__DB019A8B_C930_44FD_A9ED_3F6EB100936C__INCLUDED_)

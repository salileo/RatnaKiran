#if !defined(AFX_ADDUSERDIALOG_H__75F0363A_F59F_4FE0_9D53_EB7FE1257B9E__INCLUDED_)
#define AFX_ADDUSERDIALOG_H__75F0363A_F59F_4FE0_9D53_EB7FE1257B9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ADD_USER 0
#define CHANGE_PASSWORD 1

class CAddUserDialog : public CDialog
{
public:
	CAddUserDialog(int type, CWnd* pParent = NULL);

private:
	int m_type;

	//{{AFX_DATA(CAddUserDialog)
	enum { IDD = IDD_ADD_USER_DIALOG };
	CString	m_first_val;
	CString	m_title;
	CString	m_password;
	CString	m_password_again;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAddUserDialog)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAddUserDialog)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_ADDUSERDIALOG_H__75F0363A_F59F_4FE0_9D53_EB7FE1257B9E__INCLUDED_)

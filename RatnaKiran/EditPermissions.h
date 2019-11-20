#if !defined(AFX_EDITPERMISSIONS_H__C2AB624A_8FCE_4FEF_95BC_C85FD209E09F__INCLUDED_)
#define AFX_EDITPERMISSIONS_H__C2AB624A_8FCE_4FEF_95BC_C85FD209E09F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEditPermissions : public CDialog
{
public:
	CEditPermissions(CWnd* pParent = NULL);   // standard constructor

private:
	void UpdateLists();
	CString m_username;

	//{{AFX_DATA(CEditPermissions)
	enum { IDD = IDD_EDIT_PERMISSIONS };
	CComboBox	m_user_combo;
	CTreeCtrl	m_disallowed_list;
	CTreeCtrl	m_allowed_list;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEditPermissions)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEditPermissions)
	afx_msg void OnSelchangeUsernames();
	afx_msg void OnToLeft();
	afx_msg void OnToRight();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_EDITPERMISSIONS_H__C2AB624A_8FCE_4FEF_95BC_C85FD209E09F__INCLUDED_)

#include "stdafx.h"
#include "ratnakiran.h"
#include "DatabaseGlobals.h"
#include "EditPermissions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CEditPermissions::CEditPermissions(CWnd* pParent /*=NULL*/)
	: CDialog(CEditPermissions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditPermissions)
	//}}AFX_DATA_INIT
}


void CEditPermissions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditPermissions)
	DDX_Control(pDX, IDC_USERNAMES, m_user_combo);
	DDX_Control(pDX, IDC_TREE_DISALLOWED, m_disallowed_list);
	DDX_Control(pDX, IDC_TREE_ALLOWED, m_allowed_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditPermissions, CDialog)
	//{{AFX_MSG_MAP(CEditPermissions)
	ON_CBN_SELCHANGE(IDC_USERNAMES, OnSelchangeUsernames)
	ON_BN_CLICKED(IDC_TO_LEFT, OnToLeft)
	ON_BN_CLICKED(IDC_TO_RIGHT, OnToRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CEditPermissions::UpdateLists()
{
	m_allowed_list.DeleteAllItems();
	m_disallowed_list.DeleteAllItems();

	if(m_username.IsEmpty())
		return;

	int action = 0;
	for(action = 0; action < CPermissions::LastEntry(); action++)
	{
		CString str = CPermissions::GetStringFromAction((AppAction)action);
		if(CPermissions::GetPerm(m_username, (AppAction)action))
		{
			if(!m_allowed_list.InsertItem(str))
			{
				CString errstr = "Could not add entry '" + str + "' to the allowed list.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}
		else
		{
			if(!m_disallowed_list.InsertItem(str))
			{
				CString errstr = "Could not add entry '" + str + "' to the dis-allowed list.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}
	}

	HTREEITEM item = m_allowed_list.GetFirstVisibleItem();
	if(item)
		m_allowed_list.SelectItem(item);

	item = m_disallowed_list.GetFirstVisibleItem();
	if(item)
		m_disallowed_list.SelectItem(item);

	UpdateData(FALSE);
}


void CEditPermissions::OnSelchangeUsernames() 
{
	if(!UpdateData(TRUE))
		return;

	int index = m_user_combo.GetCurSel();
	if(index == CB_ERR)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not get the selected combo box entry.");
		return;
	}

	m_user_combo.GetLBText(index, m_username);
	UpdateLists();
}


void CEditPermissions::OnToLeft() 
{
	HTREEITEM item = m_allowed_list.GetSelectedItem();
	if(item)
	{
		CString text = m_allowed_list.GetItemText(item);
		AppAction action = CPermissions::GetActionFromString(text);
		if(CPermissions::SetPerm(m_username, action, FALSE))
		{
			CString errstr = "Could not disallow user '" + m_username + "' for '" + text + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		UpdateLists();
	}
}


void CEditPermissions::OnToRight() 
{
	HTREEITEM item = m_disallowed_list.GetSelectedItem();
	if(item)
	{
		CString text = m_disallowed_list.GetItemText(item);
		AppAction action = CPermissions::GetActionFromString(text);
		if(!CPermissions::SetPerm(m_username, action, TRUE))
		{
			CString errstr = "Could not allow user '" + m_username + "' for '" + text + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		UpdateLists();
	}
}


void CEditPermissions::OnOK() 
{
	CDialog::OnOK();
}


void CEditPermissions::OnCancel() 
{
	CDialog::OnCancel();
}


BOOL CEditPermissions::OnInitDialog() 
{
	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CList<CString, CString> userlist;
	if(!CPermissions::GetUsernames(&userlist))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not get user list from settings.");
		return FALSE;
	}

	POSITION pos = userlist.GetHeadPosition();
	while(pos)
	{
		CString user = userlist.GetAt(pos);
		if(m_user_combo.AddString(LPCTSTR(user)) == LB_ERR)
		{
			CString errstr = "Could not add username '" + user + "' to the list.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		userlist.GetNext(pos);
	}

	if(m_user_combo.GetCount() > 0)
	{
		if(m_user_combo.SetCurSel(0) == CB_ERR)
			SHOW_INTERNAL_ERROR_REASON("Could not select the first entry of the combo box.");

		int index = m_user_combo.GetCurSel();
		if(index != CB_ERR)
		{
			m_user_combo.GetLBText(index, m_username);
			UpdateLists();
		}
		else
			SHOW_INTERNAL_ERROR;
	}
	
	return TRUE;
}

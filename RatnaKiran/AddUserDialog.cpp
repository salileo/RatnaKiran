#include "stdafx.h"
#include "ratnakiran.h"
#include "DatabaseGlobals.h"
#include "AddUserDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAddUserDialog::CAddUserDialog(int type, CWnd* pParent /*=NULL*/)
	: CDialog(CAddUserDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddUserDialog)
	m_first_val = _T("");
	m_title = _T("");
	m_password = _T("");
	m_password_again = _T("");
	//}}AFX_DATA_INIT

	m_type = type;
}


void CAddUserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddUserDialog)
	DDX_Text(pDX, IDC_EDIT_1, m_first_val);
	DDX_Text(pDX, IDC_FIRST_ENTRY, m_title);
	DDX_Text(pDX, IDC_NEW_PASSWORD, m_password);
	DDX_Text(pDX, IDC_RECONFIRM_NEW_PASSWORD, m_password_again);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddUserDialog, CDialog)
	//{{AFX_MSG_MAP(CAddUserDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CAddUserDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_password != m_password_again)
	{
		SHOW_ERROR("The 2 passwords don't match. Please try again.");
		return;
	}

	if(m_type == ADD_USER)
	{
		if(m_first_val.IsEmpty())
		{
			SHOW_ERROR("Please enter a username.");
			return;
		}

		if(m_first_val == CPermissions::GetAdminUsername())
		{
			SHOW_ERROR("User 'admin' already exists.");
			return;
		}

		if(CPermissions::IsUserExisting(m_first_val))
		{
			CString errstr = "User '" + m_first_val + "' already exists.";
			SHOW_ERROR(errstr);
			return;
		}

		if(!CPermissions::AddUser(m_first_val, m_password))
		{
			CString errstr = "Could not add user '" + m_first_val + "' to the list of users.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}
	else if(m_type == CHANGE_PASSWORD)
	{
		CString username, password;
		if(!CPermissions::GetCurrentUser(&username))
		{
			SHOW_INTERNAL_ERROR_REASON("Could not get the current user.");
			return;
		}

		if(!CPermissions::GetPassword(username, &password))
		{
			CString errstr = "Could not fetch the password of the user '" + username + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}

		if(password != m_first_val)
		{
			SHOW_ERROR("The old password does not match. Please try again.");
			return;
		}

		if(password != m_password)
		{
			if(!CPermissions::SetPassword(username, m_password))
			{
				CString errstr = "Could not set the password of the user '" + username + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return;
			}
		}
	}
	else
	{
		CString errstr;
		errstr.Format("%d", m_type);
		errstr = "Invalid dialog type '" + errstr + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}

	CDialog::OnOK();
}


void CAddUserDialog::OnCancel() 
{
	CDialog::OnCancel();
}


BOOL CAddUserDialog::OnInitDialog() 
{
	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}
	
	if(m_type == ADD_USER)
	{
		m_title = "Username";
		CEdit *ctrl = (CEdit *)GetDlgItem(IDC_EDIT_1);
		if(ctrl)
			ctrl->SetPasswordChar(0);
		else
			SHOW_INTERNAL_ERROR_REASON("Could not find first edit control.");

		SetWindowText("Add User");
	}
	else if(m_type == CHANGE_PASSWORD)
	{
		m_title = "Old Password";
		SetWindowText("Change Password");
	}
	else
	{
		CString errstr;
		errstr.Format("%d", m_type);
		errstr = "Invalid dialog type '" + errstr + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		CDialog::OnCancel();
		return FALSE;
	}

	UpdateData(FALSE);
	return TRUE;
}

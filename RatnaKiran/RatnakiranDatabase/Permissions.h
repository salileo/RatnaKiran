#if !defined(AFX_PERMISSIONS_H__7C564159_F45A_475D_A82E_BB3E6F4FEFE1__INCLUDED_)
#define AFX_PERMISSIONS_H__7C564159_F45A_475D_A82E_BB3E6F4FEFE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum AppAction
{
	changefilepath=0,
	updategoldrate,
	updateusingcurrency,
	updatedescriptiondatabase,
	addset,
	deleteset,
	editset,
	moveset,
	viewsets,
	createvoucher,
	deletevoucher,
	updatevoucherinfo,
	stockreports,
	exportreports,
	kundanreports,
	adduser,
	deleteuser,
	edituser,
	enderror
};


class CPermissions  
{
public:
	CPermissions();
	virtual ~CPermissions();

	static BOOL AddUser(CString user, CString password);
	static BOOL DeleteUser(CString user);
	static BOOL GetUsernames(CList<CString, CString> *users);
	static CString GetAdminUsername();
	static BOOL IsUserExisting(CString user);

	static BOOL SetPassword(CString user, CString password);
	static BOOL GetPassword(CString user, CString *password);
	static BOOL SetPerm(CString user, AppAction action, BOOL allowed);
	static BOOL GetPerm(CString user, AppAction action);

	static BOOL GetCurrentUser(CString *user);
	static BOOL SetCurrentUser(CString user);

	//for current user
	static BOOL IsAdmin();
	static BOOL IsAllowed(AppAction action);

	static int LastEntry() { return (int)enderror; }
	static CString GetStringFromAction(AppAction action);
	static AppAction GetActionFromString(CString action);

private:
	struct UserInfo
	{
		char m_username[256];
		char m_password[256];
		unsigned int m_perms[3];

		UserInfo()
		{
			strcpy(m_username, "");
			strcpy(m_password, "");
			m_perms[0] = 0;
			m_perms[1] = 0;
			m_perms[2] = 0;
		}
	};

	CList<UserInfo, UserInfo> m_UserList;
	CString m_CurrentUser;
	BOOL m_PermsInitialized;

	BOOL ReadUsers();
	BOOL WriteUsers();
};

#endif // !defined(AFX_PERMISSIONS_H__7C564159_F45A_475D_A82E_BB3E6F4FEFE1__INCLUDED_)

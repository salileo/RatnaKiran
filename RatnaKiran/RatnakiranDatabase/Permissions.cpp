#include "DatabaseGlobals.h"

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "jaiganesh"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ADDENTRY(x,y) \
case x:\
retval = y;\
break;


#define CHECKENTRY(x,y) \
{\
CString str = y;\
if(str == action)\
return x;\
}

static CPermissions gPermissions;

CPermissions::CPermissions()
{
	m_PermsInitialized = FALSE;
}


CPermissions::~CPermissions()
{
	m_PermsInitialized = FALSE;
}


CString CPermissions::GetStringFromAction(AppAction action)
{
	CString retval;

	switch(action)
	{
	ADDENTRY(changefilepath, "Change file path");
	ADDENTRY(updategoldrate, "Update gold rate")
	ADDENTRY(updateusingcurrency, "Update using currency");
	ADDENTRY(updatedescriptiondatabase, "Update description database");
	ADDENTRY(addset, "Add set");
	ADDENTRY(deleteset, "Delete set");
	ADDENTRY(editset, "Edit set");
	ADDENTRY(moveset, "Move set");
	ADDENTRY(viewsets, "View sets");
	ADDENTRY(createvoucher, "Create voucher");
	ADDENTRY(deletevoucher, "Delete voucher");
	ADDENTRY(updatevoucherinfo, "Update voucher info");
	ADDENTRY(stockreports, "Create stock reports");
	ADDENTRY(exportreports, "Create export reports");
	ADDENTRY(kundanreports, "Create kundan reports");
	ADDENTRY(adduser, "Add user");
	ADDENTRY(deleteuser, "Delete user");
	ADDENTRY(edituser, "Edit user");
	ADDENTRY(enderror, "--error--");
	}

	return retval;
}


AppAction CPermissions::GetActionFromString(CString action)
{
	CHECKENTRY(changefilepath, "Change file path");
	CHECKENTRY(updategoldrate, "Update gold rate")
	CHECKENTRY(updateusingcurrency, "Update using currency");
	CHECKENTRY(updatedescriptiondatabase, "Update description database");
	CHECKENTRY(addset, "Add set");
	CHECKENTRY(deleteset, "Delete set");
	CHECKENTRY(editset, "Edit set");
	CHECKENTRY(moveset, "Move set");
	CHECKENTRY(viewsets, "View sets");
	CHECKENTRY(createvoucher, "Create voucher");
	CHECKENTRY(deletevoucher, "Delete voucher");
	CHECKENTRY(updatevoucherinfo, "Update voucher info");
	CHECKENTRY(stockreports, "Create stock reports");
	CHECKENTRY(exportreports, "Create export reports");
	CHECKENTRY(kundanreports, "Create kundan reports");
	CHECKENTRY(adduser, "Add user");
	CHECKENTRY(deleteuser, "Delete user");
	CHECKENTRY(edituser, "Edit user");

	return enderror;
}


BOOL CPermissions::ReadUsers()
{
	m_UserList.RemoveAll();

	{
		CString admin_password;
		char val[256];
		DWORD count = 256;
		HKEY hKey;

		CString main = "SOFTWARE\\Microsoft\\Stock";
		CString entry = "Password";

		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, main, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		{
			SHOW_INTERNAL_ERROR_REASON("Could not find password location for admin.");
			return FALSE;
		}

		if(RegQueryValueEx(hKey, entry, NULL, NULL, (LPBYTE)val, &count) != ERROR_SUCCESS)
			admin_password = ADMIN_PASSWORD;
		else
			admin_password = val;

		RegCloseKey(hKey);

		UserInfo master_user;
		strcpy(master_user.m_username, ADMIN_USERNAME);
		strcpy(master_user.m_password, LPCTSTR(admin_password));
		master_user.m_perms[0] = -1;
		master_user.m_perms[1] = -1;
		master_user.m_perms[2] = -1;
		m_UserList.AddHead(master_user);
	}

	CString filename = gFilePath + "\\perms.rsf";
	CFile inputfile;
	CFileException openerror;
	if(!inputfile.Open(LPCTSTR(filename), CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		if(IsFileExisting(filename))
		{
			CString errstr;
			errstr.Format("ERROR : #%d.", openerror.m_cause);
			errstr = "Could not open permissions file '" + filename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
		else
		{
			m_PermsInitialized = TRUE;
			return TRUE;
		}
	}

	try
	{
		inputfile.SeekToBegin();
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		SHOW_INTERNAL_ERROR_REASON(errstr);
		inputfile.Close();
		return FALSE;
	}

	while(TRUE)
	{
		UserInfo buffer;
		int readcount;
		try
		{
			readcount = inputfile.Read(&buffer,sizeof(UserInfo));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.",readerror->m_cause);
			errstr = "Could not read to permissions file '" + filename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			inputfile.Close();
			return FALSE;
		}

		if(readcount < sizeof(UserInfo))
			break;

		DECODE(&buffer, UserInfo);
		m_UserList.AddTail(buffer);
	}

	inputfile.Close();
	m_PermsInitialized = TRUE;
	return TRUE;
}


BOOL CPermissions::WriteUsers()
{
	CString filename = gFilePath + "\\temp_perms.tmp";
	CFile outputfile;
	CFileException openerror;
	if(!outputfile.Open(LPCTSTR(filename), CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", openerror.m_cause);
		errstr = "Could not open temporary permissions file '" + filename + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	try
	{
		outputfile.SeekToBegin();
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		SHOW_INTERNAL_ERROR_REASON(errstr);
		outputfile.Close();
		return FALSE;
	}

	POSITION pos = m_UserList.GetHeadPosition();
	while(pos)
	{
		UserInfo entry = m_UserList.GetAt(pos);
		if(strcmp(entry.m_username, ADMIN_USERNAME) == 0)
		{
			m_UserList.GetNext(pos);
			continue;
		}

		ENCODE(&entry, UserInfo);

		try
		{
			outputfile.Write(&entry, sizeof(UserInfo));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.",readerror->m_cause);
			errstr = "Could not write to temporary permissions file '" + filename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			outputfile.Close();
			return FALSE;
		}

		m_UserList.GetNext(pos);
	}

	outputfile.Close();
	CString finalfile = gFilePath + "\\perms.rsf";

	if(!MoveDatabaseFile(filename, finalfile))
	{
		CString errstr = "Could not move file '" + filename + "' to '" + finalfile + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}


BOOL CPermissions::AddUser(CString user, CString password)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if(user.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(user == ADMIN_USERNAME)
	{
		SHOW_INTERNAL_ERROR_REASON("Cannot add user 'admin', as it already exists.");
		return FALSE;
	}

	if(IsUserExisting(user))
	{
		CString errstr = "Cannot add user '" + user + "' as it already exists.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	UserInfo entry;
	strcpy(entry.m_username, LPCTSTR(user));
	if(!password.IsEmpty())
		strcpy(entry.m_password, LPCTSTR(password));

	gPermissions.m_UserList.AddTail(entry);

	if(!gPermissions.WriteUsers())
	{
		//restore state
		gPermissions.m_UserList.RemoveTail();
		SHOW_INTERNAL_ERROR_REASON("Unable to update stored permission.");
		return FALSE;
	}

	return TRUE;
}


BOOL CPermissions::DeleteUser(CString user)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if(user.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(user == ADMIN_USERNAME)
	{
		SHOW_INTERNAL_ERROR_REASON("Cannot delete the 'admin' user.");
		return FALSE;
	}

	if(user == gPermissions.m_CurrentUser)
	{
		SHOW_INTERNAL_ERROR_REASON("Cannot delete the self username.");
		return FALSE;
	}

	POSITION pos = gPermissions.m_UserList.GetHeadPosition();
	UserInfo orig_entry;
	while(pos)
	{
		orig_entry = gPermissions.m_UserList.GetAt(pos);
		if(strcmp(orig_entry.m_username, LPCTSTR(user)) == 0)
			break;
		gPermissions.m_UserList.GetNext(pos);
	}

	if(!pos)
	{
		CString errstr = "Could not find user '" + user + "' for deletion.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	gPermissions.m_UserList.RemoveAt(pos);

	if(!gPermissions.WriteUsers())
	{
		//restore state
		gPermissions.m_UserList.AddTail(orig_entry);
		SHOW_INTERNAL_ERROR_REASON("Unable to update stored permission.");
		return FALSE;
	}

	return TRUE;
}


BOOL CPermissions::GetUsernames(CList<CString, CString> *users)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if(!users)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	POSITION pos = gPermissions.m_UserList.GetHeadPosition();
	while(pos)
	{
		UserInfo entry = gPermissions.m_UserList.GetAt(pos);
		CString str = entry.m_username;
		if(str != ADMIN_USERNAME)
			users->AddTail(str);

		gPermissions.m_UserList.GetNext(pos);
	}

	return TRUE;
}


CString CPermissions::GetAdminUsername()
{
	CString admin = ADMIN_USERNAME;
	return admin;
}


BOOL CPermissions::IsUserExisting(CString user)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if(user.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(user == ADMIN_USERNAME)
		return TRUE;

	POSITION pos = gPermissions.m_UserList.GetHeadPosition();
	UserInfo entry;
	while(pos)
	{
		entry = gPermissions.m_UserList.GetAt(pos);
		if(strcmp(entry.m_username, LPCTSTR(user)) == 0)
			return TRUE;
		gPermissions.m_UserList.GetNext(pos);
	}

	return FALSE;
}


BOOL CPermissions::SetPassword(CString user, CString password)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if(user.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(user == ADMIN_USERNAME)
	{
		CString admin_password;
		DWORD dwDisposition;
		HKEY hKey;
		HKEY hkSub;

		CString main = "SOFTWARE\\Microsoft";
		CString section = "Stock";
		CString entry = "Password";

		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, main, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		{
			SHOW_INTERNAL_ERROR_REASON("Could not find password location for admin.");
			return FALSE;
		}

		if(RegCreateKeyEx(hKey, section, 0, "", 0, KEY_ALL_ACCESS, NULL, &hkSub, &dwDisposition) != ERROR_SUCCESS)
		{
			SHOW_INTERNAL_ERROR_REASON("Could not find password location for admin.");
			return FALSE;
		}

		if(RegSetValueEx(hkSub, entry, 0, REG_SZ, (LPBYTE)(LPCTSTR(password)), (DWORD)(password.GetLength() + 1)) != ERROR_SUCCESS)
		{
			SHOW_INTERNAL_ERROR_REASON("Could not update admin password.");
			return FALSE;
		}

		RegCloseKey(hkSub);
		RegCloseKey(hKey);
		return TRUE;
	}
	else
	{
		UserInfo orig_entry;
		POSITION pos = gPermissions.m_UserList.GetHeadPosition();
		while(pos)
		{
			orig_entry = gPermissions.m_UserList.GetAt(pos);
			if(strcmp(orig_entry.m_username, LPCTSTR(user)) == 0)
				break;
			gPermissions.m_UserList.GetNext(pos);
		}

		if(!pos)
		{
			CString errstr = "Could not find user '" + user + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}

		UserInfo new_entry = orig_entry;
		strcpy(new_entry.m_password, LPCTSTR(password));

		gPermissions.m_UserList.RemoveAt(pos);
		gPermissions.m_UserList.AddTail(new_entry);

		if(!gPermissions.WriteUsers())
		{
			//restore state
			gPermissions.m_UserList.RemoveTail();
			gPermissions.m_UserList.AddTail(orig_entry);
			SHOW_INTERNAL_ERROR_REASON("Unable to update stored permission.");
			return FALSE;
		}

		return TRUE;
	}
}


BOOL CPermissions::GetPassword(CString user, CString *password)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if((user.IsEmpty()) || !password)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	UserInfo entry;
	POSITION pos = gPermissions.m_UserList.GetHeadPosition();
	while(pos)
	{
		entry = gPermissions.m_UserList.GetAt(pos);
		if(strcmp(entry.m_username, LPCTSTR(user)) == 0)
			break;
		gPermissions.m_UserList.GetNext(pos);
	}

	if(!pos)
	{
		CString errstr = "Could not find user '" + user + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	*password = entry.m_password;
	return TRUE;
}


BOOL CPermissions::SetPerm(CString user, AppAction action, BOOL allowed)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if((user.IsEmpty()) || (action == enderror))
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(user == ADMIN_USERNAME)
	{
		SHOW_INTERNAL_ERROR_REASON("Cannot cange permissions of 'admin' user.");
		return FALSE;
	}

	POSITION pos = gPermissions.m_UserList.GetHeadPosition();
	UserInfo orig_entry;
	while(pos)
	{
		orig_entry = gPermissions.m_UserList.GetAt(pos);
		if(strcmp(orig_entry.m_username, LPCTSTR(user)) == 0)
			break;
		gPermissions.m_UserList.GetNext(pos);
	}

	if(!pos)
	{
		CString errstr = "Could not find user '" + user + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	UserInfo new_entry = orig_entry;
	int index = (int)action;
	int sizeofperm = sizeof(int) * 8; //in bits
	int bytenumber = index / sizeofperm;
	int bitnumber = index % sizeofperm;
	unsigned int *val = &(new_entry.m_perms[bytenumber]);
	int join = 1 << bitnumber;

	if(allowed)
	{
		*val = *val | join;
	}
	else
	{
		join = ~join;
		*val = *val & join;
	}

	gPermissions.m_UserList.RemoveAt(pos);
	gPermissions.m_UserList.AddTail(new_entry);

	if(!gPermissions.WriteUsers())
	{
		//restore state
		gPermissions.m_UserList.RemoveTail();
		gPermissions.m_UserList.AddTail(orig_entry);
		SHOW_INTERNAL_ERROR_REASON("Unable to update stored permission.");
		return FALSE;
	}

	return TRUE;
}


BOOL CPermissions::GetPerm(CString user, AppAction action)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if((user.IsEmpty()) || (action == enderror))
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(user == ADMIN_USERNAME)
		return TRUE;

	POSITION pos = gPermissions.m_UserList.GetHeadPosition();
	UserInfo entry;
	while(pos)
	{
		entry = gPermissions.m_UserList.GetAt(pos);
		if(strcmp(entry.m_username, LPCTSTR(user)) == 0)
			break;
		gPermissions.m_UserList.GetNext(pos);
	}

	if(!pos)
	{
		CString errstr = "Could not find user '" + user + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	int index = (int)action;
	int sizeofperm = sizeof(int) * 8; //in bits
	int bytenumber = index / sizeofperm;
	int bitnumber = index % sizeofperm;
	unsigned int val = entry.m_perms[bytenumber];
	val = val >> bitnumber;
	BOOL allowed;
	allowed = val & 1;
	return allowed;
}


BOOL CPermissions::GetCurrentUser(CString *user)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if(!user)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	*user = gPermissions.m_CurrentUser;
	return TRUE;
}


BOOL CPermissions::SetCurrentUser(CString user)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if(user.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!IsUserExisting(user))
	{
		CString errstr = "Could not find the user '" + user + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	gPermissions.m_CurrentUser = user;
	return TRUE;
}


BOOL CPermissions::IsAdmin()
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	CString admin = ADMIN_USERNAME;
	return (gPermissions.m_CurrentUser == admin);
}


BOOL CPermissions::IsAllowed(AppAction action)
{
	if(!gPermissions.m_PermsInitialized)
	{
		if(!gPermissions.ReadUsers())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored permission.");
			return FALSE;
		}
	}

	if(gPermissions.m_CurrentUser.IsEmpty())
	{
		SHOW_INTERNAL_ERROR_REASON("No valid user selected.");
		return FALSE;
	}

	if(IsAdmin())
		return TRUE;

	return GetPerm(gPermissions.m_CurrentUser, action);
}

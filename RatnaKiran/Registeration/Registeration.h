#if !defined(AFX_REGISTERATION_H__76D9CF8C_94D7_44E8_9348_1A48CEDD1E92__INCLUDED_)
#define AFX_REGISTERATION_H__76D9CF8C_94D7_44E8_9348_1A48CEDD1E92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class CRegisterationApp : public CWinApp
{
public:
	CRegisterationApp();
	BOOL success;

	//{{AFX_VIRTUAL(CRegisterationApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CRegisterationApp)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_REGISTERATION_H__76D9CF8C_94D7_44E8_9348_1A48CEDD1E92__INCLUDED_)

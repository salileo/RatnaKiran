#if !defined(AFX_RATNAKIRAN_H__38214444_548B_46C8_8A62_982B8FC63F9B__INCLUDED_)
#define AFX_RATNAKIRAN_H__38214444_548B_46C8_8A62_982B8FC63F9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class CRatnaKiranApp : public CWinApp
{
public:
	CRatnaKiranApp();
	BOOL SetDatabasePath();

private:
	BOOL GetDatabasePath();
	BOOL CheckRegistration();

	//{{AFX_VIRTUAL(CRatnaKiranApp)
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CRatnaKiranApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_RATNAKIRAN_H__38214444_548B_46C8_8A62_982B8FC63F9B__INCLUDED_)

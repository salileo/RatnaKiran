#if !defined(AFX_RATNAKIRAN_H__FCFB0602_9088_465A_90B3_FC6E33791207__INCLUDED_)
#define AFX_RATNAKIRAN_H__FCFB0602_9088_465A_90B3_FC6E33791207__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "globals.h"

class CRatnakiranApp : public CWinApp
{
public:
	CRatnakiranApp();

	//{{AFX_VIRTUAL(CRatnakiranApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CRatnakiranApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL CheckRegistration();
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_RATNAKIRAN_H__FCFB0602_9088_465A_90B3_FC6E33791207__INCLUDED_)

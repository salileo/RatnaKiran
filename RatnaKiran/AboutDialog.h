#if !defined(AFX_ABOUTDIALOG_H__96D5EE9E_666E_43D6_AECE_00CFF8363446__INCLUDED_)
#define AFX_ABOUTDIALOG_H__96D5EE9E_666E_43D6_AECE_00CFF8363446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAboutDialog : public CDialog
{
public:
	CAboutDialog(CWnd* pParent = NULL);

private:
	//{{AFX_DATA(CAboutDialog)
	enum { IDD = IDD_ABOUT_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDialog)
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAboutDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_ABOUTDIALOG_H__96D5EE9E_666E_43D6_AECE_00CFF8363446__INCLUDED_)

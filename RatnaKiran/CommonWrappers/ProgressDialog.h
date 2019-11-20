#if !defined(AFX_PROGRESSDIALOG_H__4768250E_FD77_4968_9DCD_9E35E8D6BB67__INCLUDED_)
#define AFX_PROGRESSDIALOG_H__4768250E_FD77_4968_9DCD_9E35E8D6BB67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../resource.h"

class CProgressDialog : public CDialog
{
public:
	CProgressDialog(CString work, DWORD length = 0, CWnd* pParent = NULL);
	virtual ~CProgressDialog();

	BOOL StartDialog();
	BOOL SetupDialog(DWORD length, BOOL reset);
	BOOL FinishDialog();
	BOOL ProgressForward(DWORD step_size = 1);

private:
	DWORD m_max; //progress count
	double m_scale;
	CString m_work;
	BOOL m_created;

public:
	//{{AFX_DATA(CProgressDialog)
	enum { IDD = IDD_PROGRESS_DIALOG };
	CProgressCtrl	m_progress;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProgressDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProgressDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_PROGRESSDIALOG_H__4768250E_FD77_4968_9DCD_9E35E8D6BB67__INCLUDED_)

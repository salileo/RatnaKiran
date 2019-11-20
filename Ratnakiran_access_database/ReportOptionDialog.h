#if !defined(AFX_REPORTOPTIONDIALOG_H__F22D66DA_7EC6_47E8_891C_358A6ECE3ADE__INCLUDED_)
#define AFX_REPORTOPTIONDIALOG_H__F22D66DA_7EC6_47E8_891C_358A6ECE3ADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReportOptionDialog : public CDialog
{
public:
	CReportOptionDialog(CWnd* pParent = NULL);

	//{{AFX_DATA(CReportOptionDialog)
	enum { IDD = IDD_REPORT_OPTION_DIALOG };
	//}}AFX_DATA

	CString m_filename;

	//{{AFX_VIRTUAL(CReportOptionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CReportOptionDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_REPORTOPTIONDIALOG_H__F22D66DA_7EC6_47E8_891C_358A6ECE3ADE__INCLUDED_)

#if !defined(AFX_OPTIONSDIALOG_H__F22D66DA_7EC6_47E8_891C_358A6ECE3ADE__INCLUDED_)
#define AFX_OPTIONSDIALOG_H__F22D66DA_7EC6_47E8_891C_358A6ECE3ADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COptionsDialog : public CDialog
{
public:
	COptionsDialog(CWnd* pParent = NULL);
	CString m_filename;

	//{{AFX_DATA(COptionsDialog)
	enum { IDD = IDD_OPTION_DIALOG };
	//}}AFX_DATA

private:
	//{{AFX_VIRTUAL(COptionsDialog)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COptionsDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_OPTIONSDIALOG_H__F22D66DA_7EC6_47E8_891C_358A6ECE3ADE__INCLUDED_)

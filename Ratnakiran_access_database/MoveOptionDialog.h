#if !defined(AFX_MOVEOPTIONDIALOG_H__F22D66DA_7EC6_47E8_891C_358A6ECE3ADE__INCLUDED_)
#define AFX_MOVEOPTIONDIALOG_H__F22D66DA_7EC6_47E8_891C_358A6ECE3ADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMoveOptionDialog : public CDialog
{
public:
	CMoveOptionDialog(CWnd* pParent = NULL);

	//{{AFX_DATA(CMoveOptionDialog)
	enum { IDD = IDD_MOVE_OPTION_DIALOG };
	//}}AFX_DATA

	CString m_type;

	//{{AFX_VIRTUAL(CMoveOptionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CMoveOptionDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_MOTIONOPTIONDIALOG_H__F22D66DA_7EC6_47E8_891C_358A6ECE3ADE__INCLUDED_)

#if !defined(AFX_DESCRIPTIONADDDIALOG_H__97A87EDE_19D9_4173_B5C1_CB6CA23E811F__INCLUDED_)
#define AFX_DESCRIPTIONADDDIALOG_H__97A87EDE_19D9_4173_B5C1_CB6CA23E811F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDescriptionAddDialog : public CDialog
{
public:
	CDescriptionAddDialog(CWnd* pParent = NULL);

	//{{AFX_DATA(CDescriptionAddDialog)
	enum { IDD = IDD_DESCRIPTION_ADD_DIALOG };
	CString	m_name;
	CString	m_prefix;
	int	m_number_of_pieces;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDescriptionAddDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CDescriptionAddDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DESCRIPTIONADDDIALOG_H__97A87EDE_19D9_4173_B5C1_CB6CA23E811F__INCLUDED_)

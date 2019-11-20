#if !defined(AFX_REGISTERATIONDLG_H__88DD0F13_0E20_4865_BB24_C86B595CB6CB__INCLUDED_)
#define AFX_REGISTERATIONDLG_H__88DD0F13_0E20_4865_BB24_C86B595CB6CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegisterationDlg : public CDialog
{
public:
	CRegisterationDlg(CWnd* pParent = NULL);	// standard constructor

	//{{AFX_DATA(CRegisterationDlg)
	enum { IDD = IDD_REGISTERATION_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CRegisterationDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;
	BOOL SetRegisteration();

	//{{AFX_MSG(CRegisterationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_REGISTERATIONDLG_H__88DD0F13_0E20_4865_BB24_C86B595CB6CB__INCLUDED_)

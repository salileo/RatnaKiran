#if !defined(AFX_GENERICLISTBOXDIALOG_H__5A536C1B_69DB_4BB5_8AC4_C29DD9144C32__INCLUDED_)
#define AFX_GENERICLISTBOXDIALOG_H__5A536C1B_69DB_4BB5_8AC4_C29DD9144C32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define DESCRIPTION_LIST_DIALOG 0
#define DESCRIPTION_MODIFY_DIALOG 1
#define DIAMOND_DETAIL_DIALOG 2
#define STONE_DETAIL_DIALOG 3
#define SERIAL_DETAIL_DIALOG 4
#define PRODUCT_LIST_DIALOG 5

class CGenericListBoxDialog : public CDialog
{
public:
	CGenericListBoxDialog(int type, CWnd* pParent = NULL);

	//{{AFX_DATA(CGenericListBoxDialog)
	enum { IDD = IDD_GENERIC_LIST_BOX_DIALOG };
	CListCtrl	m_list;
	CString	m_details;
	//}}AFX_DATA

	CList<CProductIDList, CProductIDList> *m_item_list;

	int m_type;
	int m_selected_item;
	int m_selected_subitem;
	int m_number_of_columns;
	CEdit m_editCtrl;
	CString m_selection;

	void SetButton1(CString name);
	void SetButton2(CString name);
	void CreateColumns();
	BOOL PopulateList();
	void InsertEntry(CTableDescription *entry, int position);
	void InsertEntry(CRecordSubItem *entry, int position);
	void InsertEntry(CRecordOldSerial *entry, int position);
	
	void EditEntryAt(int item, int subitem, int type = 0);

	//{{AFX_VIRTUAL(CGenericListBoxDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGenericListBoxDialog)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_GENERICLISTBOXDIALOG_H__5A536C1B_69DB_4BB5_8AC4_C29DD9144C32__INCLUDED_)

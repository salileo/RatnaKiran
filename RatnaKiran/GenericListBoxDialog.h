#if !defined(AFX_GENERICLISTBOXDIALOG_H__5A536C1B_69DB_4BB5_8AC4_C29DD9144C32__INCLUDED_)
#define AFX_GENERICLISTBOXDIALOG_H__5A536C1B_69DB_4BB5_8AC4_C29DD9144C32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum GenericListBoxType
{
	DescriptionList,
	DescriptionModify,
	DiamondDetail,
	StoneDetail,
	SerialDetail,
	ProductList,
	TagnamesList
};

class CGenericListBoxDialog : public CDialog
{
public:
	CGenericListBoxDialog(GenericListBoxType type, CWnd* pParent = NULL);

	CProductItem *m_item;
	CList<SStringPairs, SStringPairs> *m_itemlist;
	CString m_selection;

private:
	GenericListBoxType m_type;
	int m_number_of_columns;

	int m_selected_item;
	int m_selected_subitem;
	CEdit m_editCtrl;

	void SetButton1(CString name);
	void SetButton2(CString name);
	BOOL CreateColumns();
	BOOL PopulateList();
	BOOL InsertEntry(const CString description, CString prefix, int number_of_pieces, int index);
	BOOL InsertEntry(const CProductSubItem *type, int index);
	BOOL InsertEntry(const CProductOldSerial *entry, int index);
	BOOL InsertEntry(const SStringPairs *entry, int index);

	void EditEntryAt(int item, int subitem, int type = 0);

	//{{AFX_DATA(CGenericListBoxDialog)
	enum { IDD = IDD_GENERIC_LIST_BOX_DIALOG };
	CListCtrl	m_list;
	CString	m_details;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CGenericListBoxDialog)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

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

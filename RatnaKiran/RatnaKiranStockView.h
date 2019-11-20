#if !defined(AFX_RATNAKIRANSTOCKVIEW_H__0194EABF_E4DF_4CD1_A08E_5668792EEA36__INCLUDED_)
#define AFX_RATNAKIRANSTOCKVIEW_H__0194EABF_E4DF_4CD1_A08E_5668792EEA36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define ON 1
#define OFF 2
#define TOGGLE 3

enum SortOrder {Ascending, Descending};

class CRatnaKiranStockView : public CFormView
{
protected:
	DECLARE_DYNCREATE(CRatnaKiranStockView)

public:
	CRatnaKiranStockView();
	virtual ~CRatnaKiranStockView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL RefreshEntries(BOOL reset_columns);
	void GetCheckedList(CList<CString, CString> *list);
	void SetCheckedList(CList<CString, CString> *list, int status);
	void GetHighlightedList(CList<CString, CString> *list);
	void SetHighlightedList(CList<CString, CString> *list, int status);
	CString GetSelectedProduct(int *index = NULL);
	void SetSelectedProduct(CString productID);

	int GetItemCount();

private:
	BOOL m_IsListExisting; //a flag specifying whether the list has been created till now or not
	CImageList *m_StateImageList;
	CArray<SortOrder, SortOrder> m_ColumnArray;
	DatabaseType m_ListType;
	CString m_voucher;

	CString m_LastSelectedID;
	int m_LastSelectedIndex;

	struct SColumnSort
	{
	public:
		SColumnSort()
		{
			m_view_ptr = NULL;
		}

		~SColumnSort()
		{
		}

		void *m_view_ptr;
		int m_sub_item;
		SortOrder m_sort_order;
	};

	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	BOOL InsertColumns();
	BOOL InsertEntry(CProductSet *set, int position);
	BOOL RemoveEntry(CString productID);
	void SortColumn(int column_num, BOOL reset_columns);
	CString GetProductAt(int index);
	void UpdateStatusBar(BOOL highlighted_only = FALSE);
	
	//{{AFX_DATA(CRatnaKiranStockView)
	enum { IDD = IDD_STOCK_VIEW };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CRatnaKiranStockView)
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CRatnaKiranStockView)
	afx_msg void OnClickStockList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkStockList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickStockList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_RATNAKIRANSTOCKVIEW_H__0194EABF_E4DF_4CD1_A08E_5668792EEA36__INCLUDED_)

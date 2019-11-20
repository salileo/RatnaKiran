#if !defined(AFX_RATNAKIRANTREEVIEW_H__FD130C96_6424_4701_9452_B7A5C908F87A__INCLUDED_)
#define AFX_RATNAKIRANTREEVIEW_H__FD130C96_6424_4701_9452_B7A5C908F87A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRatnaKiranTreeView : public CTreeView
{
protected:
	DECLARE_DYNCREATE(CRatnaKiranTreeView)

public:
	CRatnaKiranTreeView();           // protected constructor used by dynamic creation
	virtual ~CRatnaKiranTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CString GetSelectedListName();
	BOOL IsRoot(CString name);
	BOOL IsStockRoot(CString name);
	BOOL IsStockList(CString name);
	BOOL IsExportRoot(CString name);
	BOOL IsExportList(CString name);
	BOOL IsKundanRoot(CString name);
	BOOL IsKundanList(CString name);

	//{{AFX_VIRTUAL(CRatnaKiranTreeView)
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

private:
	HTREEITEM m_stock_list;
	HTREEITEM m_export_list;
	HTREEITEM m_kundan_list;

	//{{AFX_MSG(CRatnaKiranTreeView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_RATNAKIRANTREEVIEW_H__FD130C96_6424_4701_9452_B7A5C908F87A__INCLUDED_)

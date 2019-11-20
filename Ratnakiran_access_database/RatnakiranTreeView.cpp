#include "stdafx.h"
#include "Ratnakiran.h"
#include "RatnakiranTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRatnakiranTreeView, CTreeView)

CRatnakiranTreeView::CRatnakiranTreeView()
{
	m_stock_list = NULL;
	m_export_list = NULL;
	m_kundan_list = NULL;
	m_during_creation = TRUE;
}

CRatnakiranTreeView::~CRatnakiranTreeView()
{
}


BEGIN_MESSAGE_MAP(CRatnakiranTreeView, CTreeView)
	//{{AFX_MSG_MAP(CRatnakiranTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static int CALLBACK MyTreeCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   CTreeCtrl* pmyTreeCtrl = (CTreeCtrl*) lParamSort;

   CString strItem1 = pmyTreeCtrl->GetItemText((HTREEITEM) lParam1);
   CString strItem2 = pmyTreeCtrl->GetItemText((HTREEITEM) lParam2);

   if(IsNumber(strItem1) && IsNumber(strItem2))
   {
	   int val1 = atoi(LPCTSTR(strItem1));
	   int val2 = atoi(LPCTSTR(strItem2));
	   return (val1 - val2);
   }
   else
   {
	   return strcmp(LPCTSTR(strItem1), LPCTSTR(strItem2));
   }
}


void CRatnakiranTreeView::OnInitialUpdate() 
{
	m_during_creation = TRUE;
	CTreeView::OnInitialUpdate();
	CTreeCtrl &treectrl = GetTreeCtrl();
	treectrl.DeleteAllItems();

	if(!MakeGlobalsValid())
		return;

	//set the tree styles
	long StyleOld;
	StyleOld = GetWindowLong(m_hWnd, GWL_STYLE);
	StyleOld |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	SetWindowLong(m_hWnd, GWL_STYLE, StyleOld);

	//insert the root
	m_stock_list = treectrl.InsertItem(STOCK_ROOT);
	if(m_stock_list == NULL)
	{
		MessageBox("Could not insert first root item in tree.", "ERROR");
	}
	else
	{
		CList<CString, CString> list;
		if(gDoc->GetVoucherList(STOCK_TYPE, &list))
		{
			POSITION pos = list.GetHeadPosition();
			while(pos)
			{
				CString str = list.GetAt(pos);
				if(str != STOCK_MAIN)
				{
					HTREEITEM item = treectrl.InsertItem(str, m_stock_list);
					
					TVITEM itemDetails;
					itemDetails.mask = TVIF_HANDLE | TVIF_PARAM;
					itemDetails.hItem = item;
					itemDetails.lParam = (LPARAM)item;
					treectrl.SetItem(&itemDetails);
				}

				list.GetNext(pos);
			}

			TVSORTCB tvs;
			tvs.hParent = m_stock_list;
			tvs.lpfnCompare = MyTreeCompareProc;
			tvs.lParam = (LPARAM) &treectrl;
			treectrl.SortChildrenCB(&tvs);
		}
	}

	m_export_list = treectrl.InsertItem(EXPORT_ROOT);
	if(m_export_list == NULL)
	{
		MessageBox("Could not insert second root item in tree.", "ERROR");
	}
	else
	{
		CList<CString, CString> list;
		if(gDoc->GetVoucherList(EXPORT_TYPE, &list))
		{
			POSITION pos = list.GetHeadPosition();
			while(pos)
			{
				CString str = list.GetAt(pos);
				if(str != EXPORT_MAIN)
				{
					HTREEITEM item = treectrl.InsertItem(str, m_export_list);
					
					TVITEM itemDetails;
					itemDetails.mask = TVIF_HANDLE | TVIF_PARAM;
					itemDetails.hItem = item;
					itemDetails.lParam = (LPARAM)item;
					treectrl.SetItem(&itemDetails);
				}

				list.GetNext(pos);
			}

			treectrl.SortChildren(m_export_list);
		}
	}

	m_kundan_list = treectrl.InsertItem(KUNDAN_ROOT);
	if(m_kundan_list == NULL)
	{
		MessageBox("Could not insert third root item in tree.", "ERROR");
	}
	else
	{
		CList<CString, CString> list;
		if(gDoc->GetVoucherList(KUNDAN_TYPE, &list))
		{
			POSITION pos = list.GetHeadPosition();
			while(pos)
			{
				CString str = list.GetAt(pos);
				if(str != KUNDAN_MAIN)
				{
					HTREEITEM item = treectrl.InsertItem(str, m_kundan_list);
					
					TVITEM itemDetails;
					itemDetails.mask = TVIF_HANDLE | TVIF_PARAM;
					itemDetails.hItem = item;
					itemDetails.lParam = (LPARAM)item;
					treectrl.SetItem(&itemDetails);
				}

				list.GetNext(pos);
			}

			treectrl.SortChildren(m_kundan_list);
		}
	}

	if(m_stock_list)
		treectrl.Expand(m_stock_list,TVE_EXPAND);

	if(m_export_list)
		treectrl.Expand(m_export_list,TVE_EXPAND);

	if(m_kundan_list)
		treectrl.Expand(m_kundan_list,TVE_EXPAND);

	HTREEITEM selection = NULL;
	if(m_stock_list)
		selection = m_stock_list;
	else if(m_export_list)
		selection = m_export_list;
	else if(m_kundan_list)
		selection = m_kundan_list;
	else
		selection = NULL;

	m_during_creation = FALSE;
	if(selection)
		treectrl.SelectItem(selection);
}


CString CRatnakiranTreeView::GetSelectedListName()
{
	CString name;
	CTreeCtrl& TreeCtrl = GetTreeCtrl();
	HTREEITEM selection = TreeCtrl.GetSelectedItem( );
	if(selection)
		name = TreeCtrl.GetItemText(selection);

	return name;
}


BOOL CRatnakiranTreeView::IsRoot(CString name)
{
	return (IsStockRoot(name) || IsExportRoot(name) || IsKundanRoot(name));
}


BOOL CRatnakiranTreeView::IsStockRoot(CString name)
{
	CString curname_root = STOCK_ROOT;
	CString curname_main = STOCK_MAIN;
	return ((name.CompareNoCase(curname_root) == 0) || (name.CompareNoCase(curname_main) == 0));
}


BOOL CRatnakiranTreeView::IsExportRoot(CString name)
{
	CString curname_root = EXPORT_ROOT;
	CString curname_main = EXPORT_MAIN;
	return ((name.CompareNoCase(curname_root) == 0) || (name.CompareNoCase(curname_main) == 0));
}


BOOL CRatnakiranTreeView::IsKundanRoot(CString name)
{
	CString curname_root = KUNDAN_ROOT;
	CString curname_main = KUNDAN_MAIN;
	return ((name.CompareNoCase(curname_root) == 0) || (name.CompareNoCase(curname_main) == 0));
}


BOOL CRatnakiranTreeView::IsStockList(CString name)
{
	if(m_stock_list)
	{
		HTREEITEM current;

		CTreeCtrl& TreeCtrl = GetTreeCtrl();
		current = TreeCtrl.GetNextItem(m_stock_list, TVGN_CHILD);

		while(current)
		{
			CString curname = TreeCtrl.GetItemText(current);
			if(curname.CompareNoCase(name) == 0)
				return TRUE;

			current = TreeCtrl.GetNextItem(current, TVGN_NEXT);
		}
	}

	return FALSE;
}


BOOL CRatnakiranTreeView::IsExportList(CString name)
{
	if(m_export_list)
	{
		HTREEITEM current;

		CTreeCtrl& TreeCtrl = GetTreeCtrl();
		current = TreeCtrl.GetNextItem(m_export_list, TVGN_CHILD);

		while(current)
		{
			CString curname = TreeCtrl.GetItemText(current);
			if(curname.CompareNoCase(name) == 0)
				return TRUE;

			current = TreeCtrl.GetNextItem(current, TVGN_NEXT);
		}
	}

	return FALSE;
}


BOOL CRatnakiranTreeView::IsKundanList(CString name)
{
	if(m_kundan_list)
	{
		HTREEITEM current;

		CTreeCtrl& TreeCtrl = GetTreeCtrl();
		current = TreeCtrl.GetNextItem(m_kundan_list, TVGN_CHILD);

		while(current)
		{
			CString curname = TreeCtrl.GetItemText(current);
			if(curname.CompareNoCase(name) == 0)
				return TRUE;

			current = TreeCtrl.GetNextItem(current, TVGN_NEXT);
		}
	}

	return FALSE;
}


void CRatnakiranTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 1;

	HTREEITEM selected = pNMTreeView->itemNew.hItem;
	if(selected == NULL)
		return;

	CTreeCtrl &treectrl = GetTreeCtrl();
	CString name = treectrl.GetItemText(selected);

	if(name == STOCK_ROOT)
		name = STOCK_MAIN;
	else if(name == EXPORT_ROOT)
		name = EXPORT_MAIN;
	else if(name == KUNDAN_ROOT)
		name = KUNDAN_MAIN;

	if(gDoc->m_vouchers.GotoVoucher(name)  != DBOK)
		return;
	
	gStockView->RefreshEntries(TRUE);
	*pResult = 0;
}


void CRatnakiranTreeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CTreeView::OnRButtonDown(nFlags, point);

	UINT uFlags = 0;
	CTreeCtrl& TreeCtrl = GetTreeCtrl();
	HTREEITEM HitItem = TreeCtrl.HitTest(point, &uFlags);
	if(HitItem != NULL)
		TreeCtrl.SelectItem(HitItem);

	CMenu	mnuCtxt;
	if(!mnuCtxt.LoadMenu(IDR_MAINFRAME))
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	ClientToScreen(&point);

	//CMenu* pMenu = mnuCtxt.GetSubMenu(0);
	CMenu* pMenu = &mnuCtxt;
	int pos = FindMenuItem(pMenu,"&Vouchers");
	if(pos == -1)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	pMenu = pMenu->GetSubMenu(pos);
	if(pMenu != NULL)
		pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, gFrame);
}


#ifdef _DEBUG
void CRatnakiranTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CRatnakiranTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG


void CRatnakiranTreeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if((nChar == VK_TAB) && gStockView)
		gStockView->SetFocus();
		
	CTreeView::OnKeyDown(nChar, nRepCnt, nFlags);
}

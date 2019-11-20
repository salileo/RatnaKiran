#include "stdafx.h"
#include "RatnaKiran.h"
#include "MainGlobal.h"
#include "RatnaKiranTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IGNORE_MARKER "****** "

IMPLEMENT_DYNCREATE(CRatnaKiranTreeView, CTreeView)

CRatnaKiranTreeView::CRatnaKiranTreeView()
{
	m_stock_list = NULL;
	m_export_list = NULL;
	m_kundan_list = NULL;
}

CRatnaKiranTreeView::~CRatnaKiranTreeView()
{
}


BEGIN_MESSAGE_MAP(CRatnaKiranTreeView, CTreeView)
	//{{AFX_MSG_MAP(CRatnaKiranTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static int CALLBACK MyTreeCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   CTreeCtrl* pmyTreeCtrl = (CTreeCtrl*) lParamSort;
   if(!pmyTreeCtrl)
   {
	   SHOW_INTERNAL_ERROR;
	   return -1;
   }

   CString strItem1 = pmyTreeCtrl->GetItemText((HTREEITEM) lParam1);
   CString strItem2 = pmyTreeCtrl->GetItemText((HTREEITEM) lParam2);

   if(strItem1.Find(IGNORE_MARKER) == 0)
	   strItem1.Delete(0, strlen(IGNORE_MARKER));

   if(strItem2.Find(IGNORE_MARKER) == 0)
	   strItem2.Delete(0, strlen(IGNORE_MARKER));

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


void CRatnaKiranTreeView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	CTreeCtrl &treectrl = GetTreeCtrl();

	if(!treectrl.DeleteAllItems())
	{
		SHOW_INTERNAL_ERROR_REASON("Could not delete all existing entries in the tree.");
		return;
	}

	if(!MakeGlobalsValid(FALSE))
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}
	
	//set the tree styles
	long StyleOld;
	StyleOld = GetWindowLong(m_hWnd, GWL_STYLE);
	StyleOld |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	SetWindowLong(m_hWnd, GWL_STYLE, StyleOld);

	//insert the root
	m_stock_list = treectrl.InsertItem(STOCK_ROOT);
	if(m_stock_list == NULL)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not insert stock root item in the tree list.");
	}
	else
	{
		POSITION pos = gDoc->m_stock_vouchers.GetHeadPosition();
		while(pos)
		{
			CProductVoucher *voucher = gDoc->m_stock_vouchers.GetAt(pos);
			if(voucher)
			{
				if(!voucher->IsRootVoucher())
				{
					CString str = voucher->m_voucher_name;
					if(gDoc->IsIgnored(str))
						str = IGNORE_MARKER + str;

					HTREEITEM item = treectrl.InsertItem(str, m_stock_list);
					if(item)
					{
						TVITEM itemDetails;
						itemDetails.mask = TVIF_HANDLE | TVIF_PARAM;
						itemDetails.hItem = item;
						itemDetails.lParam = (LPARAM)item;
						treectrl.SetItem(&itemDetails);
					}
					else
					{
						CString errstr = "Could not add entry for '" + str + "' in the tree list.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
					}
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			gDoc->m_stock_vouchers.GetNext(pos);
		}

		TVSORTCB tvs;
		tvs.hParent = m_stock_list;
		tvs.lpfnCompare = MyTreeCompareProc;
		tvs.lParam = (LPARAM) &treectrl;
		treectrl.SortChildrenCB(&tvs);
	}

	m_export_list = treectrl.InsertItem(EXPORT_ROOT);
	if(m_export_list == NULL)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not insert export root item in the tree list.");
	}
	else
	{
		POSITION pos = gDoc->m_export_vouchers.GetHeadPosition();
		while(pos)
		{
			CProductVoucher *voucher = gDoc->m_export_vouchers.GetAt(pos);
			if(voucher)
			{
				if(!voucher->IsRootVoucher())
				{
					CString str = voucher->m_voucher_name;
					if(gDoc->IsIgnored(str))
						str = IGNORE_MARKER + str;

					HTREEITEM item = treectrl.InsertItem(str, m_export_list);
					if(item)
					{
						TVITEM itemDetails;
						itemDetails.mask = TVIF_HANDLE | TVIF_PARAM;
						itemDetails.hItem = item;
						itemDetails.lParam = (LPARAM)item;
						treectrl.SetItem(&itemDetails);
					}
					else
					{
						CString errstr = "Could not add entry for '" + str + "' in the tree list.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
					}
				}
			}
			else
				SHOW_INTERNAL_ERROR;
			
			gDoc->m_export_vouchers.GetNext(pos);
		}

		TVSORTCB tvs;
		tvs.hParent = m_export_list;
		tvs.lpfnCompare = MyTreeCompareProc;
		tvs.lParam = (LPARAM) &treectrl;
		treectrl.SortChildrenCB(&tvs);
	}

	m_kundan_list = treectrl.InsertItem(KUNDAN_ROOT);
	if(m_kundan_list == NULL)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not insert kundan root item in the tree list.");
	}
	else
	{
		POSITION pos = gDoc->m_kundan_vouchers.GetHeadPosition();
		while(pos)
		{
			CProductVoucher *voucher = gDoc->m_kundan_vouchers.GetAt(pos);
			if(voucher)
			{
				if(!voucher->IsRootVoucher())
				{
					CString str = voucher->m_voucher_name;
					if(gDoc->IsIgnored(str))
						str = IGNORE_MARKER + str;

					HTREEITEM item = treectrl.InsertItem(str, m_kundan_list);
					if(item)
					{
						TVITEM itemDetails;
						itemDetails.mask = TVIF_HANDLE | TVIF_PARAM;
						itemDetails.hItem = item;
						itemDetails.lParam = (LPARAM)item;
						treectrl.SetItem(&itemDetails);
					}
					else
					{
						CString errstr = "Could not add entry for '" + str + "' in the tree list.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
					}
				}
			}
			else
				SHOW_INTERNAL_ERROR;
			
			gDoc->m_kundan_vouchers.GetNext(pos);
		}

		TVSORTCB tvs;
		tvs.hParent = m_kundan_list;
		tvs.lpfnCompare = MyTreeCompareProc;
		tvs.lParam = (LPARAM) &treectrl;
		treectrl.SortChildrenCB(&tvs);
	}

	if(m_stock_list)
	{
		if(treectrl.ItemHasChildren(m_stock_list))
		{
			if(!treectrl.Expand(m_stock_list,TVE_EXPAND))
				SHOW_INTERNAL_ERROR;
		}
	}

	if(m_export_list)
	{
		if(treectrl.ItemHasChildren(m_export_list))
		{
			if(!treectrl.Expand(m_export_list,TVE_EXPAND))
				SHOW_INTERNAL_ERROR;
		}
	}

	if(m_kundan_list)
	{
		if(treectrl.ItemHasChildren(m_kundan_list))
		{
			if(!treectrl.Expand(m_kundan_list,TVE_EXPAND))
				SHOW_INTERNAL_ERROR;
		}
	}

	HTREEITEM selection = NULL;
	if(m_stock_list)
		selection = m_stock_list;
	else if(m_export_list)
		selection = m_export_list;
	else if(m_kundan_list)
		selection = m_kundan_list;
	else
		selection = NULL;

	if(selection)
	{
		if(!treectrl.SelectItem(selection))
			SHOW_INTERNAL_ERROR;
	}
}


CString CRatnaKiranTreeView::GetSelectedListName()
{
	CString name;
	CTreeCtrl& TreeCtrl = GetTreeCtrl();
	HTREEITEM selection = TreeCtrl.GetSelectedItem( );
	if(selection)
	{
		name = TreeCtrl.GetItemText(selection);
		if(name.Find(IGNORE_MARKER) == 0)
		   name.Delete(0, strlen(IGNORE_MARKER));
	}

	return name;
}


BOOL CRatnaKiranTreeView::IsRoot(CString name)
{
	return (IsStockRoot(name) || IsExportRoot(name) || IsKundanRoot(name));
}


BOOL CRatnaKiranTreeView::IsStockRoot(CString name)
{
	CString curname = STOCK_ROOT;
	return (name.CompareNoCase(curname) == 0);
}


BOOL CRatnaKiranTreeView::IsExportRoot(CString name)
{
	CString curname = EXPORT_ROOT;
	return (name.CompareNoCase(curname) == 0);
}


BOOL CRatnaKiranTreeView::IsKundanRoot(CString name)
{
	CString curname = KUNDAN_ROOT;
	return (name.CompareNoCase(curname) == 0);
}


BOOL CRatnaKiranTreeView::IsStockList(CString name)
{
	if(m_stock_list)
	{
		HTREEITEM current;

		CTreeCtrl& TreeCtrl = GetTreeCtrl();
		current = TreeCtrl.GetNextItem(m_stock_list, TVGN_CHILD);
		while(current)
		{
			CString curname = TreeCtrl.GetItemText(current);
			if(curname.Find(IGNORE_MARKER) == 0)
			   curname.Delete(0, strlen(IGNORE_MARKER));

			if(curname.CompareNoCase(name) == 0)
				return TRUE;

			current = TreeCtrl.GetNextItem(current, TVGN_NEXT);
		}
	}

	return FALSE;
}


BOOL CRatnaKiranTreeView::IsExportList(CString name)
{
	if(m_export_list)
	{
		HTREEITEM current;

		CTreeCtrl& TreeCtrl = GetTreeCtrl();
		current = TreeCtrl.GetNextItem(m_export_list, TVGN_CHILD);
		while(current)
		{
			CString curname = TreeCtrl.GetItemText(current);
			if(curname.Find(IGNORE_MARKER) == 0)
			   curname.Delete(0, strlen(IGNORE_MARKER));

			if(curname.CompareNoCase(name) == 0)
				return TRUE;

			current = TreeCtrl.GetNextItem(current, TVGN_NEXT);
		}
	}

	return FALSE;
}


BOOL CRatnaKiranTreeView::IsKundanList(CString name)
{
	if(m_kundan_list)
	{
		HTREEITEM current;

		CTreeCtrl& TreeCtrl = GetTreeCtrl();
		current = TreeCtrl.GetNextItem(m_kundan_list, TVGN_CHILD);
		while(current)
		{
			CString curname = TreeCtrl.GetItemText(current);
			if(curname.Find(IGNORE_MARKER) == 0)
			   curname.Delete(0, strlen(IGNORE_MARKER));

			if(curname.CompareNoCase(name) == 0)
				return TRUE;

			current = TreeCtrl.GetNextItem(current, TVGN_NEXT);
		}
	}

	return FALSE;
}


void CRatnaKiranTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if(!pNMTreeView)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	if(!MakeGlobalsValid(FALSE))
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	*pResult = 1;
	HTREEITEM selected = pNMTreeView->itemNew.hItem;
	if(selected == NULL)
	{
		SHOW_INTERNAL_ERROR_REASON("No voucher selected.");
		return;
	}

	CTreeCtrl &treectrl = GetTreeCtrl();
	CString name = treectrl.GetItemText(selected);
	if(name.Find(IGNORE_MARKER) == 0)
	   name.Delete(0, strlen(IGNORE_MARKER));

	if(!gDoc->SetCurrentVoucher(name))
	{
		CString errstr = "Could not set the current voucher as '" + name + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}

	*pResult = 0;
}


void CRatnaKiranTreeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CTreeView::OnRButtonDown(nFlags, point);

	UINT uFlags = 0;
	CTreeCtrl& TreeCtrl = GetTreeCtrl();
	HTREEITEM HitItem = TreeCtrl.HitTest(point, &uFlags);
	if(HitItem != NULL)
	{
		if(!TreeCtrl.SelectItem(HitItem))
		{
			SHOW_INTERNAL_ERROR_REASON("Could not select clicked voucher.");
			return;
		}
	}

	CMenu	mnuCtxt;
	if(!mnuCtxt.LoadMenu(IDR_MAINFRAME))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find the main menu.");
		return;
	}

	ClientToScreen(&point);

	//CMenu* pMenu = mnuCtxt.GetSubMenu(0);
	CMenu* pMenu = &mnuCtxt;
	int pos = FindMenuItem(pMenu,"&Vouchers");
	if(pos == -1)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find 'Vouchers' menu.");
		return;
	}

	pMenu = pMenu->GetSubMenu(pos);
	if(pMenu == NULL)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	if(!pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, gFrame))
	{
		SHOW_INTERNAL_ERROR;
		return;
	}
}


#ifdef _DEBUG
void CRatnaKiranTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CRatnaKiranTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG


void CRatnaKiranTreeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	if((nChar == VK_TAB) && gStockView)
		gStockView->SetFocus();
		
	CTreeView::OnKeyDown(nChar, nRepCnt, nFlags);
}

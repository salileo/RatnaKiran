#include "stdafx.h"
#include "RatnaKiran.h"
#include "MainGlobal.h"
#include "RatnaKiranStockView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRatnaKiranStockView, CFormView)

CRatnaKiranStockView::CRatnaKiranStockView()
	: CFormView(CRatnaKiranStockView::IDD)
{
	//{{AFX_DATA_INIT(CRatnaKiranStockView)
	//}}AFX_DATA_INIT

	m_IsListExisting = FALSE;
	m_StateImageList = NULL;
	m_ListType	= tNone;

	m_LastSelectedID = "";
	m_LastSelectedIndex = -1;

	const int size = 5;
	m_ColumnArray.SetSize(size);
	for(int i=0; i<size; i++)
		m_ColumnArray[i] = Descending;
}

CRatnaKiranStockView::~CRatnaKiranStockView()
{
	if(m_StateImageList)
		delete m_StateImageList;
}

void CRatnaKiranStockView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatnaKiranStockView)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRatnaKiranStockView, CFormView)
	//{{AFX_MSG_MAP(CRatnaKiranStockView)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickStockList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkStockList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickStockList)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnclickList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST, OnKeydownList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#ifdef _DEBUG
void CRatnaKiranStockView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRatnaKiranStockView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


void CRatnaKiranStockView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	m_List.SetExtendedStyle(m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if(!m_IsListExisting)
	{
		//setting up icon list
		SALIL_NEW(m_StateImageList, CImageList);
		if(!m_StateImageList->Create(IDB_STATE_IMAGE, 16, 1, RGB(255, 0, 0)))
		{
			SHOW_INTERNAL_ERROR_REASON("Could not create image list.");
			return;
		}
		else
		{
			m_List.SetImageList(m_StateImageList, LVSIL_STATE);
		}
	}

	//remove all items in the list
	m_IsListExisting = TRUE;
}


void CRatnaKiranStockView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	CWnd *listwindow = GetDlgItem(IDC_LIST);
	if(!listwindow)
		return;

	listwindow->MoveWindow(10, 10, cx - 20, cy - 20);
}


BOOL CRatnaKiranStockView::InsertColumns()
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return FALSE;
	}

	switch(gDoc->m_selected_voucher->m_type)
	{
	case tStock:
		{
			if(m_List.InsertColumn(0, "Product ID", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Product ID'.");
				return FALSE;
			}

			if(m_List.InsertColumn(1, "Description", LVCFMT_LEFT, 150, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Description'.");
				return FALSE;
			}

			if(m_List.InsertColumn(2, "Sold Items", LVCFMT_LEFT, 80, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Sold Items'.");
				return FALSE;
			}

			if(m_List.InsertColumn(3, "UnSold Items", LVCFMT_LEFT, 80, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'UnSold Items'.");
				return FALSE;
			}

			m_ListType = tStock;
		}
		break;
	case tKundan:
		{
			if(m_List.InsertColumn(0, "Product ID", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Product ID'.");
				return FALSE;
			}

			if(m_List.InsertColumn(1, "Description", LVCFMT_LEFT, 150, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Description'.");
				return FALSE;
			}

			if(m_List.InsertColumn(2, "Sold Items", LVCFMT_LEFT, 80, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Sold Items'.");
				return FALSE;
			}

			if(m_List.InsertColumn(3, "UnSold Items", LVCFMT_LEFT, 80, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'UnSold Items'.");
				return FALSE;
			}

			m_ListType = tKundan;
		}
		break;
	case tExport:
		{
			if(m_List.InsertColumn(0, "Serial numbers", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Serial numbers'.");
				return FALSE;
			}

			if(m_List.InsertColumn(1, "Product ID", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Product ID'.");
				return FALSE;
			}

			if(m_List.InsertColumn(2, "Description", LVCFMT_LEFT, 150, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Description'.");
				return FALSE;
			}

			if(m_List.InsertColumn(3, "Sold Items", LVCFMT_LEFT, 80, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Sold Items'.");
				return FALSE;
			}

			if(m_List.InsertColumn(4, "UnSold Items", LVCFMT_LEFT, 80, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'UnSold Items'.");
				return FALSE;
			}

			m_ListType = tExport;
		}
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", gDoc->m_selected_voucher->m_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CRatnaKiranStockView::RefreshEntries(BOOL reset_columns)
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return FALSE;
	}

	CList<CString, CString> highlight_list;
	CList<CString, CString> check_list;

	GetHighlightedList(&highlight_list);
	GetCheckedList(&check_list);

	int must_show_index = 0;
	CString new_voucher_name = gDoc->m_selected_voucher->m_voucher_name;

	if((GetItemCount() > 0) && (m_voucher == new_voucher_name))
		must_show_index = m_List.GetTopIndex() + m_List.GetCountPerPage() - 1;

	m_voucher = new_voucher_name;
	if(reset_columns)
	{
		int i;
		int nColumnCount = m_List.GetHeaderCtrl()->GetItemCount();
		for (i=0; i<nColumnCount; i++)
		{
			if(!m_List.DeleteColumn(0))
			{
				SHOW_INTERNAL_ERROR_REASON("Could not delete all existing columns.");
				return FALSE;
			}
		}

		int arraysize = m_ColumnArray.GetSize();
		for(i=0; i<arraysize; i++)
			m_ColumnArray[i] = Descending;

		if(!InsertColumns())
		{
			SHOW_INTERNAL_ERROR_REASON("Could not add columns to the list.");
			return FALSE;
		}
	}

	if(!m_List.DeleteAllItems())
	{
		SHOW_INTERNAL_ERROR_REASON("Could not delete all existing entries from the list.");
		UpdateStatusBar();
		return FALSE;
	}

	m_LastSelectedID = "";
	m_LastSelectedIndex = -1;

	if(!CPermissions::IsAllowed(viewsets))
	{
		UpdateStatusBar();
		return TRUE;
	}

	if(gDoc->m_selected_voucher->IsRootVoucher())
	{
		CList<CProductVoucher *, CProductVoucher *> *vlist;

		switch(m_ListType)
		{
		case tStock:
			vlist = &(gDoc->m_stock_vouchers);
			break;
		case tKundan:
			vlist = &(gDoc->m_kundan_vouchers);
			break;
		case tExport:
			vlist = &(gDoc->m_export_vouchers);
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", m_ListType);
				errstr = "Invalid voucher type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				UpdateStatusBar();
				return FALSE;
			}
		}

		int position = 0;
		POSITION pos = vlist->GetHeadPosition();
		while(pos)
		{
			CProductVoucher *voucher = vlist->GetAt(pos);
			if(voucher)
			{
				if(!gDoc->IsIgnored(voucher->m_voucher_name))
				{
					int index = 0;
					while(index < voucher->GetSetCount())
					{
						CProductSet *set = voucher->GetSetAt(index);
						if(set)
						{
							if(!InsertEntry(set, position))
							{
								CString errstr = "Could not add set '" + CString(set->m_product_ID) + "' to the list.";
								SHOW_INTERNAL_ERROR_REASON(errstr);
							}
								
							position++;
						}
						else
						{
							CString errstr;
							errstr.Format("%d", index);
							errstr = "Could not get set at position '" + errstr + "' in voucher '" + CString(voucher->m_voucher_name) + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);
						}

						index++;
					}
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			vlist->GetNext(pos);
		}
	}
	else
	{
		int position = 0;
		CProductVoucher *voucher = gDoc->m_selected_voucher;
		if(voucher)
		{
			int index = 0;
			while(index < voucher->GetSetCount())
			{
				CProductSet *set = voucher->GetSetAt(index);
				if(set)
				{
					if(!InsertEntry(set, position))
					{
						CString errstr = "Could not add set '" + CString(set->m_product_ID) + "' to the list.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
					}

					position++;
				}
				else
				{
					CString errstr;
					errstr.Format("%d", index);
					errstr = "Could not get set at position '" + errstr + "' in voucher '" + CString(voucher->m_voucher_name) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}

				index++;
			}
		}
		else
			SHOW_INTERNAL_ERROR;
	}

	switch(m_ListType)
	{
	case tStock:
		SortColumn(0, reset_columns);
		break;
	case tKundan:
		SortColumn(0, reset_columns);
		break;
	case tExport:
		SortColumn(1, reset_columns);
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_ListType);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			UpdateStatusBar();
			return FALSE;
		}
	}

	if(highlight_list.IsEmpty() && check_list.IsEmpty())
	{
		//highlight the first record
		m_List.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	else
	{
		if(!check_list.IsEmpty())
			SetCheckedList(&check_list, ON);

		if(!highlight_list.IsEmpty())
			SetHighlightedList(&highlight_list, ON);

		m_List.EnsureVisible(must_show_index, FALSE);
	}

	UpdateStatusBar();
	return TRUE;
}


BOOL CRatnaKiranStockView::InsertEntry(CProductSet *set, int position)
{
	if(!set)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CString str;
	int setindex;

	switch(m_ListType)
	{
	case tStock:
		{
			setindex = m_List.InsertItem(LVIF_TEXT | LVIF_STATE | LVIF_PARAM, position , set->m_product_ID , INDEXTOSTATEIMAGEMASK(1) , LVIS_STATEIMAGEMASK , 0 , (LPARAM)(set));
			if(setindex == -1)
			{
				CString errstr = "Could not insert productID of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}

			if(!m_List.SetItemText(setindex,1,set->m_description))
			{
				CString errstr = "Could not insert description of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}

			str.Format("%d",set->GetSoldItemCount());
			if(!m_List.SetItemText(setindex,2,str))
			{
				CString errstr = "Could not insert sold item count of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
			
			str.Format("%d",set->GetUnsoldItemCount());
			if(!m_List.SetItemText(setindex,3,str))
			{
				CString errstr = "Could not insert unsold item count of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}
		break;
	case tKundan:
		{
			setindex = m_List.InsertItem(LVIF_TEXT | LVIF_STATE | LVIF_PARAM, position , set->m_product_ID , INDEXTOSTATEIMAGEMASK(1) , LVIS_STATEIMAGEMASK , 0 , (LPARAM)(set));
			if(setindex == -1)
			{
				CString errstr = "Could not insert productID of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}

			if(!m_List.SetItemText(setindex,1,set->m_description))
			{
				CString errstr = "Could not insert description of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}

			str.Format("%d",set->GetSoldItemCount());
			if(!m_List.SetItemText(setindex,2,str))
			{
				CString errstr = "Could not insert sold item count of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
			
			str.Format("%d",set->GetUnsoldItemCount());
			if(!m_List.SetItemText(setindex,3,str))
			{
				CString errstr = "Could not insert unsold item count of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}
		break;
	case tExport:
		{
			str = set->GenerateSerialString();
			setindex = m_List.InsertItem(LVIF_TEXT | LVIF_STATE | LVIF_PARAM, position , str, INDEXTOSTATEIMAGEMASK(1) , LVIS_STATEIMAGEMASK , 0 , (LPARAM)(set));
			if(setindex == -1)
			{
				CString errstr = "Could not insert serial string of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}

			if(!m_List.SetItemText(setindex,1,set->m_product_ID))
			{
				CString errstr = "Could not insert productID of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}

			if(!m_List.SetItemText(setindex,2,set->m_description))
			{
				CString errstr = "Could not insert description of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}

			str.Format("%d",set->GetSoldItemCount());
			if(!m_List.SetItemText(setindex,3,str))
			{
				CString errstr = "Could not insert sold item count of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
			
			str.Format("%d",set->GetUnsoldItemCount());
			if(!m_List.SetItemText(setindex,4,str))
			{
				CString errstr = "Could not insert unsold item count of set '" + CString(set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_ListType);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CRatnaKiranStockView::RemoveEntry(CString productID)
{
	if((GetItemCount() == 0) || productID.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	int index = 0;
	int count = GetItemCount();
	while(index < count)
	{
		CString str = GetProductAt(index);
		if(str.CompareNoCase(productID) == 0)
		{
			if(m_List.DeleteItem(index))
			{
				UpdateStatusBar();
				return TRUE;
			}
			else
			{
				CString errstr;
				errstr.Format("%d", index);
				errstr = "Could not delete item at index '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}

			UpdateStatusBar();
			return FALSE;
		}

		index++;
	}

	UpdateStatusBar();
	return FALSE;
}


int CRatnaKiranStockView::GetItemCount()
{
	return m_List.GetItemCount();
}


void CRatnaKiranStockView::UpdateStatusBar(BOOL highlighted_only)
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	int highlighted_count = 0;
	int checked_count = 0;
	CString highlight_str;
	CString checked_str;

	static int last_checked_count = 0;

	if(!highlighted_only)
	{
		CList<CString, CString> list;
		GetCheckedList(&list);
		checked_count = list.GetCount();
		last_checked_count = checked_count;
	}
	else
	{
		checked_count = last_checked_count;
	}

	highlighted_count = m_List.GetSelectedCount();
	
	checked_str.Format("%d Checked Sets", checked_count);
	highlight_str.Format("%d Highlighted Sets", highlighted_count);

	CString final = "\t" + checked_str + "         " + highlight_str + "\t";
	gFrame->SetStatusBarString(final);
}


CString CRatnaKiranStockView::GetProductAt(int index)
{
	CString val;
	int count = GetItemCount();
	if(index >= count)
	{
		SHOW_INTERNAL_ERROR;
		return val;
	}

	switch(m_ListType)
	{
	case tStock:
		val = m_List.GetItemText(index,0);
		break;
	case tKundan:
		val = m_List.GetItemText(index,0);
		break;
	case tExport:
		val = m_List.GetItemText(index,1);
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_ListType);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}

	return val;
}


void CRatnaKiranStockView::GetCheckedList(CList<CString, CString> *list)
{
	if(!list)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	if(GetItemCount() == 0)
		return;

	int index = 0;
	int count = GetItemCount();
	while(index < count)
	{
		if(m_List.GetItemState(index, LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2))
		{
			CString str = GetProductAt(index);
			if(!list->Find(str))
				list->AddTail(str);
		}

		index++;
	}
}


void CRatnaKiranStockView::SetCheckedList(CList<CString, CString> *list, int status)
{
	if(GetItemCount() == 0)
	{
		UpdateStatusBar();
		return;
	}

	int index = 0;
	int count = GetItemCount();
	while(index < count)
	{	
		if(!list)
		{
			if(status == ON)
				m_List.SetItemState(index, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK);

			else if(status == OFF)
				m_List.SetItemState(index, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);

			else if(status == TOGGLE)
			{
				if(m_List.GetItemState(index, LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2))
					m_List.SetItemState(index, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);
				else
					m_List.SetItemState(index, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK);
			}
		}
		else
		{
			CString str = GetProductAt(index);
			if(list->Find(str))
			{
				if(status == ON)
					m_List.SetItemState(index, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK);
				else if(status == OFF)
					m_List.SetItemState(index, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);
				else if(status == TOGGLE)
				{
					if(m_List.GetItemState(index, LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2))
						m_List.SetItemState(index, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);
					else
						m_List.SetItemState(index, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK);
				}
			}	
		}

		index++;
	}

	UpdateStatusBar();
}


void CRatnaKiranStockView::GetHighlightedList(CList<CString, CString> *list)
{
	if(!list)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	if(GetItemCount() == 0)
		return;

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	while(pos)
	{
		int index = m_List.GetNextSelectedItem(pos);

		CString str = GetProductAt(index);
		if(!list->Find(str))
			list->AddTail(str);
	}
}


void CRatnaKiranStockView::SetHighlightedList(CList<CString, CString> *list, int status)
{
	if(GetItemCount() == 0)
	{
		UpdateStatusBar();
		return;
	}

	int index = 0;
	int count = GetItemCount();
	while(index < count)
	{
		if(!list)
		{
			if(status == ON)
				m_List.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			else if(status == OFF)
				m_List.SetItemState(index, 0, LVIS_SELECTED | LVIS_FOCUSED);
			else if(status == TOGGLE)
			{
				if(m_List.GetItemState(index, LVIS_SELECTED | LVIS_FOCUSED) == 0)
					m_List.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				else
					m_List.SetItemState(index, 0, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
		else
		{
			CString str = GetProductAt(index);
			if(list->Find(str))
			{
				if(status == ON)
					m_List.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				else if(status == OFF)
					m_List.SetItemState(index, 0, LVIS_SELECTED | LVIS_FOCUSED);
				else if(status == TOGGLE)
				{
					if(m_List.GetItemState(index, LVIS_SELECTED | LVIS_FOCUSED) == 0)
						m_List.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
					else
						m_List.SetItemState(index, 0, LVIS_SELECTED | LVIS_FOCUSED);
				}
			}
		}

		index++;
	}

	UpdateStatusBar();
}


CString CRatnaKiranStockView::GetSelectedProduct(int *index)
{
	if(index)
		*index = m_LastSelectedIndex;

	return m_LastSelectedID;
}


void CRatnaKiranStockView::SetSelectedProduct(CString productID)
{
	if(GetItemCount() == 0)
	{
		SHOW_INTERNAL_ERROR;
		UpdateStatusBar();
		return;
	}

	int index = 0;
	int count = GetItemCount();
	while(index < count)
	{	
		CString str = GetProductAt(index);
		if(str.CompareNoCase(productID) == 0)
		{
				m_List.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				UpdateStatusBar();
				return;
		}

		index++;
	}

	UpdateStatusBar();
}


void CRatnaKiranStockView::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 1;

	if(!pNMListView)
	{
		SHOW_INTERNAL_ERROR;
		UpdateStatusBar();
		return;
	}

	m_LastSelectedIndex = pNMListView->iItem;
	m_LastSelectedID = GetProductAt(m_LastSelectedIndex);
	UpdateStatusBar(TRUE);
	*pResult = 0;
}


void CRatnaKiranStockView::OnClickStockList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE *data = (NMITEMACTIVATE *)pNMHDR;
	*pResult = 1;

	if(!data)
	{
		SHOW_INTERNAL_ERROR;
		UpdateStatusBar();
		return;
	}

	CRect rect;
	m_List.GetItemRect(data->iItem, rect, LVIR_LABEL);
	if(data->ptAction.x >= rect.left)
	{
		UpdateStatusBar();
		*pResult = 0;
		return;
	}

	if(m_List.GetItemState(data->iItem, LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(1))
		m_List.SetItemState(data->iItem, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK);
	else
		m_List.SetItemState(data->iItem, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);

	UpdateStatusBar();
	*pResult = 0;
}


void CRatnaKiranStockView::OnDblclkStockList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE *data = (NMITEMACTIVATE *)pNMHDR;
	*pResult = 1;

	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		UpdateStatusBar();
		return;
	}

	if(!data)
	{
		SHOW_INTERNAL_ERROR;
		UpdateStatusBar();
		return;
	}

	if(data->iItem >= 0)
	{
		CString productID = GetProductAt(data->iItem);
		if(!gFrame->OpenForEdit(productID))
		{
			CList<CString, CString> list;
			list.AddHead(productID);
			ShowErrorMessage("edit", &list, FALSE);
		}
	}

	*pResult = 0;
}


void CRatnaKiranStockView::OnRclickStockList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE *data = (NMITEMACTIVATE *)pNMHDR;
	*pResult = 1;

	if(!data)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	CMenu	mnuCtxt;
	if(!mnuCtxt.LoadMenu(IDR_MAINFRAME))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find the main menu");
		UpdateStatusBar();
		return;
	}

	ClientToScreen(&(data->ptAction));
	
	//CMenu* pMenu = mnuCtxt.GetSubMenu(0);
	CMenu* pMenu = &mnuCtxt;
	int pos = FindMenuItem(pMenu,"&Sets");
	if(pos == -1)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find 'Sets' menu.");
		UpdateStatusBar();
		return;
	}

	pMenu = pMenu->GetSubMenu(pos);
	if(pMenu == NULL)
	{
		SHOW_INTERNAL_ERROR;
		UpdateStatusBar();
		return;
	}

	if(!pMenu->TrackPopupMenu(TPM_LEFTALIGN, data->ptAction.x, data->ptAction.y, gFrame))
		SHOW_INTERNAL_ERROR;

	UpdateStatusBar();
	*pResult = 0;
}


int CALLBACK CRatnaKiranStockView::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	SColumnSort *info = (SColumnSort *) lParamSort;

	if(!info || !info->m_view_ptr)
	{
		SHOW_INTERNAL_ERROR;
		return 0;
	}

	CProductSet *set1 = (CProductSet *)lParam1;
	CProductSet *set2 = (CProductSet *)lParam2;

	if(!set1 || !set2)
	{
		SHOW_INTERNAL_ERROR;
		return 0;
	}

	CString str1,str2;
	int val1, val2;
	int retval = 0;
	CRatnaKiranStockView *view_ptr = (CRatnaKiranStockView *)info->m_view_ptr;

	switch(view_ptr->m_ListType)
	{
	case tStock:
		{
			switch(info->m_sub_item)
			{
			case 0:
				str1 = set1->m_product_ID;
				str2 = set2->m_product_ID;
				retval = strcmp(str1, str2);
				break;
			case 1:
				str1 = set1->m_description;
				str2 = set2->m_description;
				retval = strcmp(str1, str2);
				break;
			case 2:
				val1 = set1->GetSoldItemCount();
				val2 = set2->GetSoldItemCount();

				if(val1 > val2)
					retval = -1;
				else if(val1 < val2)
					retval = 1;
				else
					retval = 0;

				break;
			case 3:
				val1 = set1->GetUnsoldItemCount();
				val2 = set2->GetUnsoldItemCount();

				if(val1 > val2)
					retval = -1;
				else if(val1 < val2)
					retval = 1;
				else
					retval = 0;

				break;
			}
		}
		break;
	case tKundan:
		{
			switch(info->m_sub_item)
			{
			case 0:
				str1 = set1->m_product_ID;
				str2 = set2->m_product_ID;
				retval = strcmp(str1, str2);
				break;
			case 1:
				str1 = set1->m_description;
				str2 = set2->m_description;
				retval = strcmp(str1, str2);
				break;
			case 2:
				val1 = set1->GetSoldItemCount();
				val2 = set2->GetSoldItemCount();

				if(val1 > val2)
					retval = -1;
				else if(val1 < val2)
					retval = 1;
				else
					retval = 0;

				break;
			case 3:
				val1 = set1->GetUnsoldItemCount();
				val2 = set2->GetUnsoldItemCount();

				if(val1 > val2)
					retval = -1;
				else if(val1 < val2)
					retval = 1;
				else
					retval = 0;

				break;
			}
		}
		break;
	case tExport:
		{
			switch(info->m_sub_item)
			{
			case 0:
				str1 = set1->GenerateSerialString(&val1);
				str2 = set2->GenerateSerialString(&val2);
				retval = strcmp(str1, str2);

				if(val1 > val2)
					retval = -1;
				else if(val1 < val2)
					retval = 1;
				else
					retval = 0;

				break;
			case 1:
				str1 = set1->m_product_ID;
				str2 = set2->m_product_ID;
				retval = strcmp(str1, str2);
				break;
			case 2:
				str1 = set1->m_description;
				str2 = set2->m_description;
				retval = strcmp(str1, str2);
				break;
			case 3:
				val1 = set1->GetSoldItemCount();
				val2 = set2->GetSoldItemCount();

				if(val1 > val2)
					retval = -1;
				else if(val1 < val2)
					retval = 1;
				else
					retval = 0;

				break;
			case 4:
				val1 = set1->GetUnsoldItemCount();
				val2 = set2->GetUnsoldItemCount();

				if(val1 > val2)
					retval = -1;
				else if(val1 < val2)
					retval = 1;
				else
					retval = 0;

				break;
			}
		}
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", view_ptr->m_ListType);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return 0;
		}
	}

	if(info->m_sort_order == Ascending)
		retval = retval;
	else if(info->m_sort_order == Descending)
		retval = -retval;
	else
	{
		CString errstr;
		errstr.Format("%d", info->m_sort_order);
		errstr = "Invalid sort order '" + errstr + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	return retval;
}


void CRatnaKiranStockView::SortColumn(int column_num, BOOL reset_columns)
{
	SColumnSort info;
	info.m_view_ptr = this;
	info.m_sub_item = column_num;

	info.m_sort_order = m_ColumnArray[info.m_sub_item];

	if(reset_columns)
	{
		if(info.m_sort_order != Ascending)
			m_ColumnArray[info.m_sub_item] = info.m_sort_order = Ascending;
		else
			m_ColumnArray[info.m_sub_item] = info.m_sort_order = Descending;
	}
	
	if(!m_List.SortItems(MyCompareProc, (LPARAM) &info))
		SHOW_INTERNAL_ERROR_REASON("Could not sort all items properly.");
}


void CRatnaKiranStockView::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 1;

	if(!pNMListView)
	{
		SHOW_INTERNAL_ERROR;
		UpdateStatusBar();
		return;
	}

	SortColumn(pNMListView->iSubItem, TRUE);
	*pResult = 0;
}


void CRatnaKiranStockView::OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	*pResult = 1;

	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	if(!pLVKeyDow)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}
	
	if(pLVKeyDow)
	{
		switch(pLVKeyDow->wVKey)
		{
		case VK_SPACE:
			{
				CList<CString, CString> list;
				GetHighlightedList(&list);
				SetCheckedList(&list, TOGGLE);
			}
			break;
		case VK_RETURN:
			{
				int index;
				CString productID = GetSelectedProduct(&index);
				if(index >= 0)
				{
					if(!gFrame->OpenForEdit(productID))
					{
						CList<CString, CString> list;
						list.AddHead(productID);
						ShowErrorMessage("edit", &list, FALSE);
					}
				}
			}
			break;
		case VK_DELETE:
			{
				if((GetItemCount() == 0) ||
				   !CPermissions::IsAllowed(deleteset))
				{
					//don't perform
				}
				else if(!gFrame->PerformSetDeleteCheckedHighlighted(FALSE))
				{
					SHOW_INTERNAL_ERROR_REASON("Could not delete all checked/highlighted sets.");
					return;
				}
			}
			break;
		case VK_INSERT:
			{
				if((m_ListType != tStock) ||
				   (gDoc->m_stock_vouchers.IsEmpty()) ||
				   !CPermissions::IsAllowed(addset))
				{
					//don't perform
				}
				else if(!gFrame->PerformSetAddNew())
				{
					SHOW_INTERNAL_ERROR_REASON("Could not add a new set.");
					return;
				}
			}
			break;
		case 65:
			//'A' key
			if(::GetKeyState (VK_CONTROL) < 0)
				SetHighlightedList(NULL, ON);

			break;
		default:
			break;
		}
	}

	*pResult = 0;
}

BOOL CRatnaKiranStockView::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			{
				int index;
				CString productID = GetSelectedProduct(&index);
				if(index >= 0)
				{
					if(!MakeGlobalsValid())
					{
						SHOW_INTERNAL_ERROR_REASON("Global values invalid");
						return TRUE;
					}

					if(!gFrame->OpenForEdit(productID))
					{
						CList<CString, CString> list;
						list.AddHead(productID);
						ShowErrorMessage("edit", &list, FALSE);
					}
				}
			}
			return TRUE;
			break;
		case VK_TAB:
			{
				if(!MakeGlobalsValid())
				{
					SHOW_INTERNAL_ERROR_REASON("Global values invalid");
					return TRUE;
				}

				if(gTreeView)
					gTreeView->SetFocus();
			}
			return TRUE;
			break;
		}
		break;
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}

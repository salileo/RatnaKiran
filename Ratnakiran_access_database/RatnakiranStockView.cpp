#include "stdafx.h"
#include "Ratnakiran.h"
#include "RatnakiranStockView.h"
#include "ProgressDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRatnakiranStockView, CFormView)

CRatnakiranStockView::CRatnakiranStockView()
	: CFormView(CRatnakiranStockView::IDD)
{
	//{{AFX_DATA_INIT(CRatnakiranStockView)
	//}}AFX_DATA_INIT

	m_IsListExisting = FALSE;
	m_StateImageList = NULL;
	m_ListType	= "";

	m_LastSelectedID = "";
	m_LastSelectedIndex = -1;

	const int size = 5;
	m_ColumnArray.SetSize(size);
	for(int i=0; i<size; i++)
		m_ColumnArray[i] = Descending;
}

CRatnakiranStockView::~CRatnakiranStockView()
{
	if(m_StateImageList)
		delete m_StateImageList;
}

void CRatnakiranStockView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatnakiranStockView)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRatnakiranStockView, CFormView)
	//{{AFX_MSG_MAP(CRatnakiranStockView)
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
void CRatnakiranStockView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRatnakiranStockView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


void CRatnakiranStockView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_List.SetExtendedStyle(m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if(!m_IsListExisting)
	{
		//setting up icon list
		SALIL_NEW(m_StateImageList, CImageList);
		m_StateImageList->Create(IDB_STATE_IMAGE, 16, 1, RGB(255, 0, 0));
		m_List.SetImageList(m_StateImageList, LVSIL_STATE);
	}

	//remove all items in the list
	m_IsListExisting = TRUE;
	RefreshEntries(TRUE);
}


void CRatnakiranStockView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	CWnd *listwindow = GetDlgItem(IDC_LIST);
	if(!listwindow)
		return;

	listwindow->MoveWindow(10, 10, cx - 20, cy - 20);
}


void CRatnakiranStockView::InsertColumns()
{
	if(!MakeGlobalsValid())
		return;

	if(gDoc->m_vouchers.m_record.m_Type == STOCK_TYPE)
	{
		m_List.InsertColumn(0, "Product ID", LVCFMT_LEFT, 100, 0);
		m_List.InsertColumn(1, "Description", LVCFMT_LEFT, 150, 0);
		m_List.InsertColumn(2, "Sold Items", LVCFMT_LEFT, 80, 0);
		m_List.InsertColumn(3, "UnSold Items", LVCFMT_LEFT, 80, 0);

		m_ListType = STOCK_TYPE;
	}
	else if(gDoc->m_vouchers.m_record.m_Type == KUNDAN_TYPE)
	{
		m_List.InsertColumn(0, "Product ID", LVCFMT_LEFT, 100, 0);
		m_List.InsertColumn(1, "Description", LVCFMT_LEFT, 150, 0);
		m_List.InsertColumn(2, "Sold Items", LVCFMT_LEFT, 80, 0);
		m_List.InsertColumn(3, "UnSold Items", LVCFMT_LEFT, 80, 0);

		m_ListType = KUNDAN_TYPE;
	}
	else if(gDoc->m_vouchers.m_record.m_Type == EXPORT_TYPE)
	{
		m_List.InsertColumn(0, "Serial numbers", LVCFMT_LEFT, 100, 0);
		m_List.InsertColumn(1, "Product ID", LVCFMT_LEFT, 100, 0);
		m_List.InsertColumn(2, "Description", LVCFMT_LEFT, 150, 0);
		m_List.InsertColumn(3, "Sold Items", LVCFMT_LEFT, 80, 0);
		m_List.InsertColumn(4, "UnSold Items", LVCFMT_LEFT, 80, 0);

		m_ListType = EXPORT_TYPE;
	}
}


void CRatnakiranStockView::RefreshEntries(BOOL reset_columns)
{
	static BOOL firsttime = TRUE;
	if(!MakeGlobalsValid())
	{
		if(firsttime)
		{
			firsttime = FALSE;
			return;
		}
		else
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}
	}

	m_IDArray.SetSize(gDoc->m_vouchers.GetSetCount(FALSE));

	CProgressDialog progress("Refreshing entries", gDoc->m_vouchers.GetSetCount(FALSE) + 3);
	progress.StartDialog();

	CList<CString, CString> highlight_list;
	CList<CString, CString> check_list;

	GetHighlightedList(&highlight_list);
	GetCheckedList(&check_list);

	int must_show_index = m_List.GetTopIndex() + m_List.GetCountPerPage() - 1;
	progress.ProgressForward();

	if(reset_columns)
	{
		int i;
		int nColumnCount = m_List.GetHeaderCtrl()->GetItemCount();
		for (i=0; i<nColumnCount; i++)
		{
			m_List.DeleteColumn(0);
		}

		int arraysize = m_ColumnArray.GetSize();
		for(i=0; i<arraysize; i++)
			m_ColumnArray[i] = Descending;

		InsertColumns();
	}

	progress.ProgressForward();
	m_List.DeleteAllItems();
	m_LastSelectedID = "";
	m_LastSelectedIndex = -1;

	int position = 0;
	DatabaseState state = gDoc->m_vouchers.m_sets.GotoFirstSet();

	while(state == DBOK)
	{
		CTableSet *set = &(gDoc->m_vouchers.m_sets);
		InsertEntry(set, position);

		position++;
		progress.ProgressForward();
		
		state = gDoc->m_vouchers.m_sets.GotoNextSet();
	}

	if(GetType() == STOCK_TYPE)
		SortColumn(0, reset_columns);
	else if(GetType() == KUNDAN_TYPE)
		SortColumn(0, reset_columns);
	else if(GetType() == EXPORT_TYPE)
		SortColumn(1, reset_columns);

	progress.ProgressForward();
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

	progress.FinishDialog();
}


void CRatnakiranStockView::InsertEntry(CTableSet *set, int position)
{
	if(!set)
		return;
	
	CString str;
	int setindex;

	int new_pos = m_List.GetItemCount();
	m_IDArray[new_pos] = set->m_record.m_Set_ID;

	if(GetType() == STOCK_TYPE)
	{
		setindex = m_List.InsertItem(LVIF_TEXT | LVIF_STATE | LVIF_PARAM, position , set->m_record.m_Set_ID , INDEXTOSTATEIMAGEMASK(1) , LVIS_STATEIMAGEMASK , 0 , (LPARAM)(new_pos));
		m_List.SetItemText(setindex,1,set->m_record.m_Description);

		str.Format("%d",set->GetItemCount(Sold, FALSE));
		m_List.SetItemText(setindex,2,str);
		
		str.Format("%d",set->GetItemCount(UnSold, FALSE));
		m_List.SetItemText(setindex,3,str);
	}
	else if(GetType() == KUNDAN_TYPE)
	{
		setindex = m_List.InsertItem(LVIF_TEXT | LVIF_STATE | LVIF_PARAM, position , set->m_record.m_Set_ID , INDEXTOSTATEIMAGEMASK(1) , LVIS_STATEIMAGEMASK , 0 , (LPARAM)(new_pos));
		m_List.SetItemText(setindex,1,set->m_record.m_Description);

		str.Format("%d",set->GetItemCount(Sold, FALSE));
		m_List.SetItemText(setindex,2,str);
		
		str.Format("%d",set->GetItemCount(UnSold, FALSE));
		m_List.SetItemText(setindex,3,str);
	}
	else if(GetType() == EXPORT_TYPE)
	{
		str = set->GenerateSerialString(NULL);
		setindex = m_List.InsertItem(LVIF_TEXT | LVIF_STATE | LVIF_PARAM, position , str, INDEXTOSTATEIMAGEMASK(1) , LVIS_STATEIMAGEMASK , 0 , (LPARAM)(new_pos));

		m_List.SetItemText(setindex,1,set->m_record.m_Set_ID);
		m_List.SetItemText(setindex,2,set->m_record.m_Description);

		str.Format("%d",set->GetItemCount(Sold, FALSE));
		m_List.SetItemText(setindex,3,str);
		
		str.Format("%d",set->GetItemCount(UnSold, FALSE));
		m_List.SetItemText(setindex,4,str);
	}
}


BOOL CRatnakiranStockView::RemoveEntry(CString productID)
{
	if(GetItemCount() == 0)
		return FALSE;

	int index = 0;
	int count = GetItemCount();
	while(index < count)
	{
		CString str = GetProductAt(index);
		if(str.CompareNoCase(productID) == 0)
		{
			return m_List.DeleteItem(index);
		}

		index++;
	}

	return FALSE;
}


int CRatnakiranStockView::GetItemCount()
{
	return m_List.GetItemCount();
}


CString CRatnakiranStockView::GetProductAt(int index)
{
	CString val;
	int count = GetItemCount();
	if(index >= count)
		return val;

	if(GetType() == STOCK_TYPE)
		val = m_List.GetItemText(index,0);
	else if(GetType() == KUNDAN_TYPE)
		val = m_List.GetItemText(index,0);
	else if(GetType() == EXPORT_TYPE)
		val = m_List.GetItemText(index,1);

	return val;
}


void CRatnakiranStockView::GetCheckedList(CList<CString, CString> *list)
{
	if((GetItemCount() == 0) || !list)
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


void CRatnakiranStockView::SetCheckedList(CList<CString, CString> *list, int status)
{
	if(GetItemCount() == 0)
		return;

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
}


void CRatnakiranStockView::GetHighlightedList(CList<CString, CString> *list)
{
	if((GetItemCount() == 0) || !list)
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


void CRatnakiranStockView::SetHighlightedList(CList<CString, CString> *list, int status)
{
	if(GetItemCount() == 0)
		return;

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
}


CString CRatnakiranStockView::GetSelectedProduct(int *index)
{
	if(index)
		*index = m_LastSelectedIndex;

	return m_LastSelectedID;
}


void CRatnakiranStockView::SetSelectedProduct(CString productID)
{
	if(GetItemCount() == 0)
		return;

	int index = 0;
	int count = GetItemCount();
	while(index < count)
	{	
		CString str = GetProductAt(index);
		if(str.CompareNoCase(productID) == 0)
		{
				m_List.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				return;
		}

		index++;
	}
}


void CRatnakiranStockView::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_LastSelectedIndex = pNMListView->iItem;
	m_LastSelectedID = GetProductAt(m_LastSelectedIndex);
	*pResult = 0;
}


void CRatnakiranStockView::OnClickStockList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE *data = (NMITEMACTIVATE *)pNMHDR;

	CRect rect;
	m_List.GetItemRect(data->iItem, rect, LVIR_LABEL);

	if(data->ptAction.x >= rect.left)
	{
		*pResult = 0;
		return;
	}

	if(m_List.GetItemState(data->iItem, LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(1))
		m_List.SetItemState(data->iItem, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK);
	else
		m_List.SetItemState(data->iItem, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);

	*pResult = 0;
}


void CRatnakiranStockView::OnDblclkStockList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE *data = (NMITEMACTIVATE *)pNMHDR;

	if(data->iItem >= 0)
	{
		CString productID = GetProductAt(data->iItem);
		if(!gFrame->OpenForEdit(productID))
		{
			CList<CString, CString> list;
			list.AddHead(productID);
			//ShowErrorMessage("edit", &list, FALSE);
		}
	}

	*pResult = 0;
}


void CRatnakiranStockView::OnRclickStockList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE *data = (NMITEMACTIVATE *)pNMHDR;

	CMenu	mnuCtxt;
	if(!mnuCtxt.LoadMenu(IDR_MAINFRAME))
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	ClientToScreen(&(data->ptAction));
	
	//CMenu* pMenu = mnuCtxt.GetSubMenu(0);
	CMenu* pMenu = &mnuCtxt;
	int pos = FindMenuItem(pMenu,"&Sets");
	if(pos == -1)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	pMenu = pMenu->GetSubMenu(pos);
	if(pMenu != NULL)
		pMenu->TrackPopupMenu(TPM_LEFTALIGN, data->ptAction.x, data->ptAction.y, gFrame);
	
	*pResult = 0;
}


int CALLBACK CRatnakiranStockView::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	SColumnSort *info = (SColumnSort *) lParamSort;

	if(!info || !info->m_view_ptr || !gDoc->m_vouchers.IsOpen() || !gDoc->m_vouchers.m_sets.IsOpen())
		return 0;

	CRatnakiranStockView *view_ptr = (CRatnakiranStockView *)info->m_view_ptr;
	int index1 = (int)lParam1;
	int index2 = (int)lParam2;

	CString productID_1	= view_ptr->m_IDArray[index1];
	CString productID_2 = view_ptr->m_IDArray[index2];

	CString str1,str2;
	int val1, val2;
	int retval = 0;

	if(view_ptr->GetType() == STOCK_TYPE)
	{
		switch(info->m_sub_item)
		{
		case 0:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			str1 = gDoc->m_vouchers.m_sets.m_record.m_Set_ID;

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			str2 = gDoc->m_vouchers.m_sets.m_record.m_Set_ID;

			retval = strcmp(str1, str2);
			break;
		case 1:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			str1 = gDoc->m_vouchers.m_sets.m_record.m_Description;

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			str2 = gDoc->m_vouchers.m_sets.m_record.m_Description;

			retval = strcmp(str1, str2);
			break;
		case 2:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			val1 = gDoc->m_vouchers.m_sets.GetItemCount(Sold, FALSE);

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			val2 = gDoc->m_vouchers.m_sets.GetItemCount(Sold, FALSE);

			if(val1 > val2)
				retval = -1;
			else if(val1 < val2)
				retval = 1;
			else
				retval = 0;

			break;
		case 3:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			val1 = gDoc->m_vouchers.m_sets.GetItemCount(UnSold, FALSE);

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			val2 = gDoc->m_vouchers.m_sets.GetItemCount(UnSold, FALSE);

			if(val1 > val2)
				retval = -1;
			else if(val1 < val2)
				retval = 1;
			else
				retval = 0;

			break;
		}
	}
	else if(view_ptr->GetType() == KUNDAN_TYPE)
	{
		switch(info->m_sub_item)
		{
		case 0:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			str1 = gDoc->m_vouchers.m_sets.m_record.m_Set_ID;

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			str2 = gDoc->m_vouchers.m_sets.m_record.m_Set_ID;

			retval = strcmp(str1, str2);
			break;
		case 1:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			str1 = gDoc->m_vouchers.m_sets.m_record.m_Description;

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			str2 = gDoc->m_vouchers.m_sets.m_record.m_Description;

			retval = strcmp(str1, str2);
			break;
		case 2:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			val1 = gDoc->m_vouchers.m_sets.GetItemCount(Sold, FALSE);

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			val2 = gDoc->m_vouchers.m_sets.GetItemCount(Sold, FALSE);

			if(val1 > val2)
				retval = -1;
			else if(val1 < val2)
				retval = 1;
			else
				retval = 0;

			break;
		case 3:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			val1 = gDoc->m_vouchers.m_sets.GetItemCount(UnSold, FALSE);

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			val2 = gDoc->m_vouchers.m_sets.GetItemCount(UnSold, FALSE);

			if(val1 > val2)
				retval = -1;
			else if(val1 < val2)
				retval = 1;
			else
				retval = 0;

			break;
		}
	}
	else if(view_ptr->GetType() == EXPORT_TYPE)
	{
		switch(info->m_sub_item)
		{
		case 0:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			str1 = gDoc->m_vouchers.m_sets.GenerateSerialString(&val1);

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			str2 = gDoc->m_vouchers.m_sets.GenerateSerialString(&val2);

			retval = strcmp(str1, str2);

			if(val1 > val2)
				retval = -1;
			else if(val1 < val2)
				retval = 1;
			else
				retval = 0;

			break;
		case 1:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			str1 = gDoc->m_vouchers.m_sets.m_record.m_Set_ID;

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			str2 = gDoc->m_vouchers.m_sets.m_record.m_Set_ID;

			retval = strcmp(str1, str2);
			break;
		case 2:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			str1 = gDoc->m_vouchers.m_sets.m_record.m_Description;

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			str2 = gDoc->m_vouchers.m_sets.m_record.m_Description;

			retval = strcmp(str1, str2);
			break;
		case 3:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			val1 = gDoc->m_vouchers.m_sets.GetItemCount(Sold, FALSE);

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			val2 = gDoc->m_vouchers.m_sets.GetItemCount(Sold, FALSE);

			if(val1 > val2)
				retval = -1;
			else if(val1 < val2)
				retval = 1;
			else
				retval = 0;

			break;
		case 4:
			if(gDoc->m_vouchers.m_sets.GotoSet(productID_1) != DBOK)
				return 0;
			val1 = gDoc->m_vouchers.m_sets.GetItemCount(UnSold, FALSE);

			if(gDoc->m_vouchers.m_sets.GotoSet(productID_2) != DBOK)
				return 0;
			val2 = gDoc->m_vouchers.m_sets.GetItemCount(UnSold, FALSE);

			if(val1 > val2)
				retval = -1;
			else if(val1 < val2)
				retval = 1;
			else
				retval = 0;

			break;
		}
	}

	if(info->m_sort_order == Ascending)
		retval = retval;
	else if(info->m_sort_order == Descending)
		retval = -retval;

	return retval;
}


void CRatnakiranStockView::SortColumn(int column_num, BOOL reset_columns)
{
	SColumnSort info;
	info.m_view_ptr = this;
	info.m_sub_item = column_num;

	info.m_sort_order = m_ColumnArray[info.m_sub_item];

	if(reset_columns)
	{
		if(info.m_sort_order != Ascending)
		{
			m_ColumnArray[info.m_sub_item] = info.m_sort_order = Ascending;
		}
		else
		{
			m_ColumnArray[info.m_sub_item] = info.m_sort_order = Descending;
		}
	}
	
	m_List.SortItems(MyCompareProc, (LPARAM) &info);
}


void CRatnakiranStockView::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	SortColumn(pNMListView->iSubItem, TRUE);
	*pResult = 0;
}


void CRatnakiranStockView::OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	
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
						//ShowErrorMessage("edit", &list, FALSE);
					}
				}
			}
			break;
		case VK_DELETE:
			gFrame->OnSetDeletecheckedHighlighted();
			break;
		case VK_INSERT:
			gFrame->OnSetAddnew();
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

BOOL CRatnakiranStockView::PreTranslateMessage(MSG* pMsg) 
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
					if(!gFrame->OpenForEdit(productID))
					{
						CList<CString, CString> list;
						list.AddHead(productID);
						//ShowErrorMessage("edit", &list, FALSE);
					}
				}
			}
			return TRUE;
			break;
		case VK_TAB:
			{
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

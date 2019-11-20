#include "stdafx.h"
#include "RatnaKiran.h"
#include "GenericListBoxDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EDIT_CONTROL 12345
#define BEGIN_EDIT 1
#define END_EDIT 2

CGenericListBoxDialog::CGenericListBoxDialog(int type, CWnd* pParent /*=NULL*/)
	: CDialog(CGenericListBoxDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenericListBoxDialog)
	m_details = _T("");
	//}}AFX_DATA_INIT

	m_item_list = NULL;
	m_type = type;
	m_selection = "";
	m_selected_item = -1;
	m_selected_subitem = 0;
	m_number_of_columns = 0;

	if(m_type == PRODUCT_LIST_DIALOG)
		m_details = "Select the products that you would like to use.";
}


void CGenericListBoxDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenericListBoxDialog)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_TEXT, m_details);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenericListBoxDialog, CDialog)
	//{{AFX_MSG_MAP(CGenericListBoxDialog)
	ON_BN_CLICKED(ID_BUTTON_1, OnButton1)
	ON_BN_CLICKED(ID_BUTTON_2, OnButton2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetfocusList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CGenericListBoxDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_type == DESCRIPTION_LIST_DIALOG)
	{
		SetButton1("Modify");
		SetButton2("");
		SetWindowText("Select Description from list");
	}
	else if(m_type == DESCRIPTION_MODIFY_DIALOG)
	{
		SetButton1("Add");
		SetButton2("Delete");
		SetWindowText("Modify Description list");

		CRect size(0, 0, 0, 17);
		m_editCtrl.Create(ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER, size, &m_list, EDIT_CONTROL);
		m_editCtrl.SetLimitText(1023);
		m_editCtrl.ShowWindow(SW_HIDE);
	}
	else if((m_type == DIAMOND_DETAIL_DIALOG) || 
			(m_type == STONE_DETAIL_DIALOG) || 
			(m_type == SERIAL_DETAIL_DIALOG))
	{
		SetButton1("Add");
		SetButton2("Delete");

		if(m_type == DIAMOND_DETAIL_DIALOG)
			SetWindowText("Diamond Details");
		else if(m_type == STONE_DETAIL_DIALOG)
			SetWindowText("Stone Details");
		else if(m_type == SERIAL_DETAIL_DIALOG)
			SetWindowText("Old Serial Details");

		CRect size(0, 0, 0, 17);
		m_editCtrl.Create(ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER, size, &m_list, EDIT_CONTROL);
		m_editCtrl.SetLimitText(1023);
		m_editCtrl.ShowWindow(SW_HIDE);
	}
	else if(m_type == PRODUCT_LIST_DIALOG)
	{
		SetButton1("");
		SetButton2("");
		SetWindowText("Product List");
	}

	CreateColumns();
	if(!PopulateList())
	{
		CDialog::OnCancel();
		return FALSE;
	}

	return TRUE;
}


void CGenericListBoxDialog::SetButton1(CString name)
{
	CButton *bt = (CButton *)GetDlgItem(ID_BUTTON_1);
	if(bt)
	{
		if(name.IsEmpty())
			bt->ShowWindow(SW_HIDE);
		else
			bt->SetWindowText(name);
	}
}


void CGenericListBoxDialog::SetButton2(CString name)
{
	CButton *bt = (CButton *)GetDlgItem(ID_BUTTON_2);
	if(bt)
	{
		if(name.IsEmpty())
			bt->ShowWindow(SW_HIDE);
		else
			bt->SetWindowText(name);
	}
}


void CGenericListBoxDialog::CreateColumns()
{
	if((m_type == DESCRIPTION_LIST_DIALOG) || (m_type == DESCRIPTION_MODIFY_DIALOG))
	{
		m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		m_list.InsertColumn(0, "Name", LVCFMT_LEFT, 100, 0);
		m_list.InsertColumn(1, "Prefix", LVCFMT_LEFT, 100, 0);
		m_list.InsertColumn(2, "No. of pieces", LVCFMT_LEFT, 100, 0);
		m_number_of_columns = 3;
	}
	else if((m_type == DIAMOND_DETAIL_DIALOG) || (m_type == STONE_DETAIL_DIALOG))
	{
		m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		m_list.InsertColumn(0, "Type", LVCFMT_LEFT, 100, 0);
		m_list.InsertColumn(1, "Weight (Crts.)", LVCFMT_LEFT, 100, 0);
		m_list.InsertColumn(2, "Rate (Rs./Crts.)", LVCFMT_LEFT, 100, 0);
		m_number_of_columns = 3;
	}
	else if(m_type == SERIAL_DETAIL_DIALOG)
	{
		m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		m_list.InsertColumn(0, "Tagname", LVCFMT_LEFT, 100, 0);
		m_list.InsertColumn(1, "Serial No.", LVCFMT_LEFT, 100, 0);
		m_number_of_columns = 2;
	}
	else if(m_type == PRODUCT_LIST_DIALOG)
	{
		m_list.ModifyStyle(LVS_SINGLESEL, LVS_AUTOARRANGE);
		m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	
		m_list.InsertColumn(0, "ProductID", LVCFMT_LEFT, 100, 0);
		m_list.InsertColumn(1, "Voucher", LVCFMT_LEFT, 100, 0);
		m_number_of_columns = 2;
	}
}


void CGenericListBoxDialog::InsertEntry(CTableDescription *entry, int position)
{
	CString number;
	number.Format("%d", entry->m_Piece_count);
	m_list.InsertItem(position, entry->m_Description);
	m_list.SetItemText(position, 1, entry->m_Prefix);
	m_list.SetItemText(position, 2, number);

}

void CGenericListBoxDialog::InsertEntry(CRecordSubItem *entry, int position)
{
	m_list.InsertItem(position, entry->m_Type);

	CString str;
	str.Format("%.2f", entry->m_Weight);
	m_list.SetItemText(position, 1, str);

	str.Format("%d", entry->m_Rate);
	m_list.SetItemText(position, 2, str);
}

void CGenericListBoxDialog::InsertEntry(CRecordOldSerial *entry, int position)
{
	CString serial;
	serial.Format("%d",entry->m_serial);

	m_list.InsertItem(position, entry->m_tag);
	m_list.SetItemText(position, 1, serial);
}

BOOL CGenericListBoxDialog::PopulateList()
{
	m_list.DeleteAllItems();

	if(!MakeGlobalsValid())
		return FALSE;

	if((m_type == DESCRIPTION_LIST_DIALOG) || (m_type == DESCRIPTION_MODIFY_DIALOG))
	{
		if(DB_IS_EMPTY(gDesc))
			return TRUE;

		DatabaseState state = gDesc.GotoFirstEntry();

		if(state == DBInvalid)
			return FALSE;

		int index = 0;
		while(state == DBOK)
		{
			InsertEntry(&gDesc, index);
			index++;

			state = gDesc.GotoNextEntry();
		} 
			
		if(!gDesc.IsEOF())
			return FALSE;

		m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	else if((m_type == DIAMOND_DETAIL_DIALOG) || (m_type == STONE_DETAIL_DIALOG))
	{
		CTableSubItem *sub_items = &(gDoc->m_vouchers.m_sets.m_items.m_sub_items);

		DatabaseState state = sub_items->GotoFirstSubItem();

		if(state == DBInvalid)
			return FALSE;

		int index = 0;
		while(state == DBOK)
		{
			if(((sub_items->m_record.m_Category == DIAMOND) && (m_type == DIAMOND_DETAIL_DIALOG)) ||
			   ((sub_items->m_record.m_Category == STONE) && (m_type == STONE_DETAIL_DIALOG)))
			{
				InsertEntry(&(sub_items->m_record), index);
				index++;
			}

			state = sub_items->GotoNextSubItem();
		} 

		if(!sub_items->IsEOF())
			return FALSE;

		m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	else if(m_type == SERIAL_DETAIL_DIALOG)
	{
		CTableItem *items = &(gDoc->m_vouchers.m_sets.m_items);

		int index = 0;
		CList<CRecordOldSerial, CRecordOldSerial> list;
		items->GetSerialList(&list);
		POSITION pos = list.GetHeadPosition();

		while(pos)
		{
			CRecordOldSerial entry = list.GetAt(pos);
			InsertEntry(&entry, index);
			list.GetNext(pos);
			index++;
		}

		m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	else if(m_type == PRODUCT_LIST_DIALOG)
	{
		if(!m_item_list)
		{
			UnHandledError(__FILE__, __LINE__);
			return FALSE;
		}

		int index = 0;
		while(!(m_item_list->IsEmpty()))
		{
			CProductIDList details = m_item_list->RemoveHead();
			m_list.InsertItem(index,details.m_productID);
			m_list.SetItemText(index,1,details.m_voucherName);
			index++;
		}
	}

	return TRUE;
}


void CGenericListBoxDialog::EditEntryAt(int item, int subitem, int type)
{
	if(item == -1)
		return;

	if(type != BEGIN_EDIT)
	{
		char editText[1024];
		int pos = m_editCtrl.GetLine(0, editText, 1023);
		editText[pos] = '\0';

		CString str = editText;
		m_list.SetItemText(m_selected_item, m_selected_subitem, str);
	}

	m_selected_item = item;
	m_selected_subitem = subitem;

	if(type != END_EDIT)
	{
		CRect itemRect;
		CRect editRect;
		m_list.GetSubItemRect(m_selected_item, m_selected_subitem, LVIR_BOUNDS, itemRect);
		m_editCtrl.GetWindowRect(&editRect);

		m_editCtrl.MoveWindow(itemRect.left, itemRect.top, 100, editRect.Height());
		m_editCtrl.ShowWindow(SW_SHOW);
		m_editCtrl.SetFocus();

		CString str = m_list.GetItemText(m_selected_item, m_selected_subitem);
		m_editCtrl.SetWindowText(str);
		m_editCtrl.SetSel(0,-1);
		//m_editCtrl.ReplaceSel(str);
	}
}


void CGenericListBoxDialog::OnButton1() 
{
	if(m_type == DESCRIPTION_LIST_DIALOG)
	{
		CGenericListBoxDialog dlg(DESCRIPTION_MODIFY_DIALOG);
		if(dlg.DoModal() == IDCANCEL)
			return;

		PopulateList();
	}
	else if((m_type == DESCRIPTION_MODIFY_DIALOG) ||
			(m_type == DIAMOND_DETAIL_DIALOG) || 
			(m_type == STONE_DETAIL_DIALOG) ||
			(m_type == SERIAL_DETAIL_DIALOG))
	{
		if(m_editCtrl.IsWindowVisible())
		{
			EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
			m_editCtrl.ShowWindow(SW_HIDE);
			m_selected_item = -1;
			m_selected_subitem = 0;
		}

		int position = m_list.GetItemCount();
		if(m_type == DESCRIPTION_MODIFY_DIALOG)
		{
			CTableDescription empty;
			InsertEntry(&empty, position);
		}
		else if((m_type == DIAMOND_DETAIL_DIALOG) || (m_type == STONE_DETAIL_DIALOG))
		{
			CRecordSubItem empty;
			InsertEntry(&empty, position);
		}
		else if(m_type == SERIAL_DETAIL_DIALOG)
		{
			CRecordOldSerial empty;
			InsertEntry(&empty, position);
		}
	
		EditEntryAt(position, 0, BEGIN_EDIT);
	}
}

void CGenericListBoxDialog::OnButton2() 
{
	if((m_type == DESCRIPTION_MODIFY_DIALOG) ||
	   (m_type == DIAMOND_DETAIL_DIALOG) || 
	   (m_type == STONE_DETAIL_DIALOG) ||
	   (m_type == SERIAL_DETAIL_DIALOG))
	{
		if(m_editCtrl.IsWindowVisible())
		{
			EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
			m_editCtrl.ShowWindow(SW_HIDE);
			m_selected_item = -1;
			m_selected_subitem = 0;
		}

		POSITION pos = m_list.GetFirstSelectedItemPosition();
		if(!pos)
		{
			MessageBox("Please select an item to delete.","ERROR");
			return;
		}

		int selection = m_list.GetNextSelectedItem(pos);
		m_list.DeleteItem(selection);

		if(selection < m_list.GetItemCount())
			m_list.SetItemState(selection, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		else
			m_list.SetItemState(m_list.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CGenericListBoxDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	if(!MakeGlobalsValid())
		return;

	if(m_type == DESCRIPTION_LIST_DIALOG)
	{
		POSITION pos = m_list.GetFirstSelectedItemPosition();
		if(pos)
		{
			int index = m_list.GetNextSelectedItem(pos);
			m_selection = m_list.GetItemText(index, 0);
		}
	}
	else if(m_type == DESCRIPTION_MODIFY_DIALOG)
	{
		if(m_editCtrl.IsWindowVisible())
		{
			EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
			m_editCtrl.ShowWindow(SW_HIDE);
			m_selected_item = -1;
			m_selected_subitem = 0;
		}

		if(!gDesc.DeleteAll())
			return;

		int total = m_list.GetItemCount();
		int current = 0;
		while(current < total)
		{
			CString desc = m_list.GetItemText(current, 0);
			CString prefix = m_list.GetItemText(current, 1); 

			CString number = m_list.GetItemText(current, 2);
			int count = atoi(LPCTSTR(number));

			if(!desc.IsEmpty())
			{
				if(!gDesc.AddDescription(desc, prefix, count))
					return;
			}

			current++;
		}

		if(gDesc.RequeryEntry("") == DBInvalid)
			return;
	}
	else if((m_type == DIAMOND_DETAIL_DIALOG) || (m_type == STONE_DETAIL_DIALOG))
	{
		if(m_editCtrl.IsWindowVisible())
		{
			EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
			m_editCtrl.ShowWindow(SW_HIDE);
			m_selected_item = -1;
			m_selected_subitem = 0;
		}

		CTableItem *items = &(gDoc->m_vouchers.m_sets.m_items);

		if(((m_type == DIAMOND_DETAIL_DIALOG) && items->DeleteSubItemList(DIAMOND)) ||
		   ((m_type == STONE_DETAIL_DIALOG) && items->DeleteSubItemList(STONE)))
		   return;

		int total = m_list.GetItemCount();
		int current = 0;
		while(current < total)
		{
			CString name = m_list.GetItemText(current, 0);
			CString weight = m_list.GetItemText(current, 1);
			CString rate = m_list.GetItemText(current, 2);
			
			if(!name.IsEmpty())
			{
				CRecordSubItem sub_item;
			
				sub_item.m_Type = name;
				sub_item.m_Weight = RoundOff(atof(LPCTSTR(weight)), 2);
				sub_item.m_Rate = atoi(LPCTSTR(rate));

				if(m_type == DIAMOND_DETAIL_DIALOG)
				{
					if(!items->AddSubItem(DIAMOND, sub_item))
					{
						MessageBox("Could not add diamond details to item. Please try again.","ERROR");
						return;
					}
				}
				else if(m_type == STONE_DETAIL_DIALOG)
				{
					if(!items->AddSubItem(STONE, sub_item))
					{
						MessageBox("Could not add stone detail to item. Please try again.","ERROR");
						return;
					}
				}
			}

			current++;
		}
	}
	else if(m_type == SERIAL_DETAIL_DIALOG)
	{
		if(m_editCtrl.IsWindowVisible())
		{
			EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
			m_editCtrl.ShowWindow(SW_HIDE);
			m_selected_item = -1;
			m_selected_subitem = 0;
		}

		CTableItem *items = &(gDoc->m_vouchers.m_sets.m_items);

		CList<CRecordOldSerial, CRecordOldSerial> list;
		int total = m_list.GetItemCount();
		int current = 0;
		while(current < total)
		{
			CString tagname = m_list.GetItemText(current, 0);
			CString serial = m_list.GetItemText(current, 1);
			
			if(!tagname.IsEmpty())
			{
				CRecordOldSerial entry;
				entry.m_tag = tagname;
				entry.m_serial = atoi(LPCTSTR(serial));
				list.AddTail(entry);
			}

			current++;
		}

		if(!items->DBEditEntry())
			return;
		
		items->PutSerialList(&list);

		if(!items->DBUpdateEntry(""))
			return;
	}
	else if(m_type == PRODUCT_LIST_DIALOG)
	{
		POSITION pos = m_list.GetFirstSelectedItemPosition();
		while(pos)
		{
			int index = m_list.GetNextSelectedItem(pos);
			CProductIDList details;
			details.m_productID = m_list.GetItemText(index, 0);
			details.m_voucherName = m_list.GetItemText(index, 1);
			m_item_list->AddTail(details);
		}
	}
	
	CDialog::OnOK();
}


void CGenericListBoxDialog::OnCancel() 
{
	CDialog::OnCancel();
}


void CGenericListBoxDialog::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE *data = (NMITEMACTIVATE *)pNMHDR;

	if((m_type == DESCRIPTION_MODIFY_DIALOG) ||
	   (m_type == DIAMOND_DETAIL_DIALOG) || 
	   (m_type == STONE_DETAIL_DIALOG) ||
	   (m_type == SERIAL_DETAIL_DIALOG))
	{
		if(data->iItem >= 0)
		{
			if(m_editCtrl.IsWindowVisible())
				EditEntryAt(data->iItem, data->iSubItem);
			else
				EditEntryAt(data->iItem, data->iSubItem, BEGIN_EDIT);
		}
		else
		{
			if(m_list.GetItemCount() == 0)
			{
				int position = 0;
				if(m_type == DESCRIPTION_MODIFY_DIALOG)
				{
					CTableDescription empty;
					InsertEntry(&empty, position);
				}
				else if((m_type == DIAMOND_DETAIL_DIALOG) || (m_type == STONE_DETAIL_DIALOG))
				{
					CRecordSubItem empty;
					InsertEntry(&empty, position);
				}
				else if(m_type == SERIAL_DETAIL_DIALOG)
				{
					CRecordOldSerial empty;
					InsertEntry(&empty, position);
				}
				
				m_selected_item = 0;
				m_selected_subitem = 0;

				EditEntryAt(m_selected_item, m_selected_subitem, BEGIN_EDIT);
			}
			else
				m_editCtrl.ShowWindow(SW_HIDE);
		}
	}
	
	*pResult = 0;
}


void CGenericListBoxDialog::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE *data = (NMITEMACTIVATE *)pNMHDR;

	if(m_type == DESCRIPTION_LIST_DIALOG)
	{
		if(data->iItem >= 0)
		{
			m_selection = m_list.GetItemText(data->iItem,0);
			*pResult = 0;
			CDialog::OnOK();
		}
	}
	else if((m_type == DESCRIPTION_MODIFY_DIALOG) ||
			(m_type == DIAMOND_DETAIL_DIALOG) || 
			(m_type == STONE_DETAIL_DIALOG) ||
			(m_type == SERIAL_DETAIL_DIALOG))
	{
		if(data->iItem >= 0)
		{
			if(m_editCtrl.IsWindowVisible())
				EditEntryAt(data->iItem, data->iSubItem);
			else
				EditEntryAt(data->iItem, data->iSubItem, BEGIN_EDIT);
		}
		else
		{
			m_editCtrl.ShowWindow(SW_HIDE);
		}
	}
	else if(m_type == PRODUCT_LIST_DIALOG)
	{
		if((data->iItem >= 0) && (m_item_list))
		{
			CProductIDList details;
			details.m_productID = m_list.GetItemText(data->iItem, 0);
			details.m_voucherName = m_list.GetItemText(data->iItem, 1);
			m_item_list->AddTail(details);
			CDialog::OnOK();
		}
	}

	*pResult = 0;
}


void CGenericListBoxDialog::OnSetfocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if((m_type == DESCRIPTION_MODIFY_DIALOG) ||
	   (m_type == DIAMOND_DETAIL_DIALOG) || 
	   (m_type == STONE_DETAIL_DIALOG) ||
	   (m_type == SERIAL_DETAIL_DIALOG))
	{
		if(m_editCtrl.IsWindowVisible())
		{
			EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
			m_editCtrl.ShowWindow(SW_HIDE);
			m_selected_item = -1;
			m_selected_subitem = 0;
		}
	}
	
	*pResult = 0;
}


BOOL CGenericListBoxDialog::PreTranslateMessage(MSG* pMsg)
{
	if((m_type == DESCRIPTION_MODIFY_DIALOG) ||
	   (m_type == DIAMOND_DETAIL_DIALOG) || 
	   (m_type == STONE_DETAIL_DIALOG) ||
	   (m_type == SERIAL_DETAIL_DIALOG))
	{
		CWnd *pCont = GetFocus();
		if (pCont == &m_editCtrl)
		{
			static BOOL inShift = FALSE;
			switch(pMsg->message)
			{
			case WM_KEYDOWN:
				switch(pMsg->wParam)
				{
				case VK_TAB:
					{
						int item = m_selected_item;
						int pos = m_selected_subitem;
						if(inShift)
						{
							pos--;
							if(pos == -1)//begin of item
							{
								if(m_selected_item == 0)
								{
									pos = 0;
									return TRUE;
								}
								else
								{
									item--;
									pos = m_number_of_columns - 1;
								}
							}
						}
						else
						{
							pos++;
							if(pos == m_number_of_columns)//end of item
							{
								if((m_selected_item + 1) == m_list.GetItemCount())
								{
									int position = m_list.GetItemCount();
									if(m_type == DESCRIPTION_MODIFY_DIALOG)
									{
										CTableDescription empty;
										InsertEntry(&empty, position);
									}
									else if((m_type == DIAMOND_DETAIL_DIALOG) || (m_type == STONE_DETAIL_DIALOG))
									{
										CRecordSubItem empty;
										InsertEntry(&empty, position);
									}
									else if(m_type == SERIAL_DETAIL_DIALOG)
									{
										CRecordOldSerial empty;
										InsertEntry(&empty, position);
									}

									item = position;
								}
								else
								{
									item++;
								}

								pos = 0;
							}
						}

						EditEntryAt(item, pos);
						return TRUE;
					}
					break;
				case VK_SHIFT:
					inShift = TRUE;
					break;
				case VK_ESCAPE:
					if(m_editCtrl.IsWindowVisible())
					{
						m_editCtrl.ShowWindow(SW_HIDE);
						m_selected_item = -1;
						m_selected_subitem = 0;
					}
					return TRUE;
					break;
				case VK_RETURN:
					if(m_editCtrl.IsWindowVisible())
					{
						EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
						m_editCtrl.ShowWindow(SW_HIDE);
						m_selected_item = -1;
						m_selected_subitem = 0;
					}

					m_list.SetFocus();
					return TRUE;
					break;
				}
				break;
			case WM_KEYUP:
				switch(pMsg->wParam)
				{
				case VK_SHIFT:
					inShift = FALSE;
					break;
				}
				break;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

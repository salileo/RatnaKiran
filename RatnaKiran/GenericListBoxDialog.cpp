#include "stdafx.h"
#include "RatnaKiran.h"
#include "MainGlobal.h"
#include "GenericListBoxDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EDIT_CONTROL 12345
#define BEGIN_EDIT 1
#define END_EDIT 2

CGenericListBoxDialog::CGenericListBoxDialog(GenericListBoxType type, CWnd* pParent /*=NULL*/)
	: CDialog(CGenericListBoxDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenericListBoxDialog)
	m_details = _T("");
	//}}AFX_DATA_INIT

	m_type = type;
	m_selection = "";
	m_details = "";
	m_selected_item = -1;
	m_selected_subitem = 0;
	m_number_of_columns = 0;

	m_item = NULL;
	m_itemlist = NULL;
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
	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}
	
	switch(m_type)
	{
	case DescriptionList:
		{
			m_details = "Double click on entry to enforce it.";

			if(CPermissions::IsAllowed(updatedescriptiondatabase))
				SetButton1("Modify");
			else
				SetButton1("");

			SetButton2("");
			SetWindowText("Select Description from list");
		}
		break;
	case DescriptionModify:
		{
			SetButton1("Add");
			SetButton2("Delete");
			SetWindowText("Modify Description list");

			CRect size(0, 0, 0, 17);
			if(!m_editCtrl.Create(ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER, size, &m_list, EDIT_CONTROL))
			{
				SHOW_INTERNAL_ERROR_REASON("Could not create editing control.");
				CDialog::OnCancel();
				return FALSE;
			}

			m_editCtrl.SetLimitText(1023);
			m_editCtrl.ShowWindow(SW_HIDE);
		}
		break;
	case DiamondDetail:
	case StoneDetail:
	case SerialDetail:
		{
			if(!m_item)
			{
				SHOW_INTERNAL_ERROR;
				CDialog::OnCancel();
				return FALSE;
			}

			SetButton1("Add");
			SetButton2("Delete");

			switch(m_type)
			{
			case DiamondDetail:
				SetWindowText("Diamond Details");
				break;
			case StoneDetail:
				SetWindowText("Stone Details");
				break;
			case SerialDetail:
				SetWindowText("Old Serial Details");
				break;
			default:
				{
					CString errstr;
					errstr.Format("%d", m_type);
					errstr = "Invalid dialog type '" + errstr + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return FALSE;
				}
			}

			CRect size(0, 0, 0, 17);
			if(!m_editCtrl.Create(ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER, size, &m_list, EDIT_CONTROL))
			{
				SHOW_INTERNAL_ERROR_REASON("Could not create editing control.");
				CDialog::OnCancel();
				return FALSE;
			}

			m_editCtrl.SetLimitText(1023);
			m_editCtrl.ShowWindow(SW_HIDE);
		}
		break;
	case ProductList:
		{
			m_details = "Select all the products that you would like to use.";

			if(!m_itemlist)
			{
				SHOW_INTERNAL_ERROR;
				CDialog::OnCancel();
				return FALSE;
			}

			SetButton1("");
			SetButton2("");
			SetWindowText("Product List");
		}
		break;
	case TagnamesList:
		{
			m_details = "Click on any entry to change the corresponding tagname.";

			if(!m_itemlist)
			{
				SHOW_INTERNAL_ERROR;
				CDialog::OnCancel();
				return FALSE;
			}

			SetButton1("");
			SetButton2("");
			SetWindowText("Tagnames");

			CRect size(0, 0, 0, 17);
			if(!m_editCtrl.Create(ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER, size, &m_list, EDIT_CONTROL))
			{
				SHOW_INTERNAL_ERROR_REASON("Could not create editing control.");
				CDialog::OnCancel();
				return FALSE;
			}

			m_editCtrl.SetLimitText(1023);
			m_editCtrl.ShowWindow(SW_HIDE);
		}
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid dialog type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			CDialog::OnCancel();
			return FALSE;
		}
	}

	if(!CreateColumns())
	{
		SHOW_INTERNAL_ERROR_REASON("Could not create columns.");
		return FALSE;
	}

	if(!PopulateList())
	{
		SHOW_INTERNAL_ERROR_REASON("Could not populate the list.");
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
	else
		SHOW_INTERNAL_ERROR_REASON("Could not find the first button control.");
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
	else
		SHOW_INTERNAL_ERROR_REASON("Could not find the second button control.");
}


BOOL CGenericListBoxDialog::CreateColumns()
{
	switch(m_type)
	{
	case DescriptionList:
	case DescriptionModify:
		{
			m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
			if(m_list.InsertColumn(0, "Name", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Name'.");
				return FALSE;
			}

			if(m_list.InsertColumn(1, "Prefix", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Prefix'.");
				return FALSE;
			}

			if(m_list.InsertColumn(2, "No. of pieces", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'No. of pieces'.");
				return FALSE;
			}

			m_number_of_columns = 3;
		}
		break;
	case DiamondDetail:
	case StoneDetail:
		{
			m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
			if(m_list.InsertColumn(0, "Type", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Type'.");
				return FALSE;
			}

			if(m_list.InsertColumn(1, "Weight (Crts.)", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Weight (Crts.)'.");
				return FALSE;
			}

			if(m_list.InsertColumn(2, "Rate (Rs./Crts.)", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Rate (Rs./Crts.)'.");
				return FALSE;
			}

			if(m_list.InsertColumn(3, "Number of pieces", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Number of pieces'.");
				return FALSE;
			}

			m_number_of_columns = 4;
		}
		break;
	case SerialDetail:
		{
			m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
			if(m_list.InsertColumn(0, "Tagname", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Tagname'.");
				return FALSE;
			}

			if(m_list.InsertColumn(1, "Serial No.", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Serial No.'.");
				return FALSE;
			}

			m_number_of_columns = 2;
		}
		break;
	case ProductList:
		{
			m_list.ModifyStyle(LVS_SINGLESEL, LVS_AUTOARRANGE);
			m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	
			if(m_list.InsertColumn(0, "ProductID", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'ProductID'.");
				return FALSE;
			}

			if(m_list.InsertColumn(1, "Voucher", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Voucher'.");
				return FALSE;
			}

			m_number_of_columns = 2;
		}
		break;
	case TagnamesList:
		{
			m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	
			if(m_list.InsertColumn(0, "Voucher", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Voucher'.");
				return FALSE;
			}

			if(m_list.InsertColumn(1, "Tagname", LVCFMT_LEFT, 100, 0) == -1)
			{
				SHOW_INTERNAL_ERROR_REASON("Could not add column 'Tagname'.");
				return FALSE;
			}

			m_number_of_columns = 2;
		}
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid dialog type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CGenericListBoxDialog::PopulateList()
{
	if(!m_list.DeleteAllItems())
	{
		SHOW_INTERNAL_ERROR_REASON("Could not remove existing list entries.");
		return FALSE;
	}

	switch(m_type)
	{
	case DescriptionList:
	case DescriptionModify:
		{
			int index = 0;
			int pos = 0;
			while(pos < CSettings::GetCategoryCount())
			{
				CString description;
				CString prefix;
				int number_of_pieces = 0;

				if(CSettings::GetCategory(pos, &description, &prefix, &number_of_pieces))
				{
					if(InsertEntry(description, prefix, number_of_pieces, index))
						index++;
					else
					{
						CString errstr = "Could not add entry for category '" + description + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return FALSE;
					}
				}
				else
				{
					CString errstr;
					errstr.Format("%d", pos);
					errstr = "Could not get category at position '" + errstr + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return FALSE;
				}

				pos++;
			}

			m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
		break;
	case DiamondDetail:
	case StoneDetail:
		{
			if(!m_item)
			{
				SHOW_INTERNAL_ERROR;
				return FALSE;
			}

			int count = 0;
			switch(m_type)
			{
			case DiamondDetail:
				count = m_item->GetSubItemCount(tDiamond);
				break;
			case StoneDetail:
				count = m_item->GetSubItemCount(tStone);
				break;
			default:
				{
					CString errstr;
					errstr.Format("%d", m_type);
					errstr = "Invalid dialog type '" + errstr + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return FALSE;
				}
			}

			int index = 0;
			int pos = 0;
			while(pos < count)
			{
				CProductSubItem *type = NULL;

				switch(m_type)
				{
				case DiamondDetail:
					type = m_item->GetSubItemAt(tDiamond, pos);
					break;
				case StoneDetail:
					type = m_item->GetSubItemAt(tStone, pos);
					break;
				default:
					{
						CString errstr;
						errstr.Format("%d", m_type);
						errstr = "Invalid dialog type '" + errstr + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return FALSE;
					}
				}

				if(type)
				{
					if(InsertEntry(type, index))
						index++;
					else
					{
						CString errstr = "Could not add entry for subitem '" + CString(type->m_type) + "' of item '" + CString(m_item->m_product_ID) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return FALSE;
					}
				}
				else
				{
					CString errstr;
					errstr.Format("%d", pos);
					errstr = "Could not get subitem at position '" + errstr + "' in item '" + CString(m_item->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return FALSE;
				}

				pos++;
			}

			m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
		break;
	case SerialDetail:
		{
			if(!m_item)
			{
				SHOW_INTERNAL_ERROR;
				return FALSE;
			}

			int index = 0;
			int pos = 0;
			while(pos < m_item->GetOldSerialCount())
			{
				CProductOldSerial *entry = m_item->GetOldSerialAt(pos);
				if(entry)
				{
					if(InsertEntry(entry, index))
						index++;
					else
					{
						CString errstr = "Could not add entry for old serial number with tagname '" + CString(entry->m_tagname) + "' of item '" + CString(m_item->m_product_ID) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return FALSE;
					}
				}
				else
				{
					CString errstr;
					errstr.Format("%d", pos);
					errstr = "Could not get old serial number at position '" + errstr + "' in the item '" + CString(m_item->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return FALSE;
				}

				pos++;
			}

			m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
		break;
	case ProductList:
		{
			if(!m_itemlist)
			{
				SHOW_INTERNAL_ERROR;
				return FALSE;
			}

			int index = 0;
			while(!(m_itemlist->IsEmpty()))
			{
				SStringPairs details = m_itemlist->RemoveHead();
				if(InsertEntry(&details, index))
					index++;
				else
				{
					CString errstr = "Could not add entry for productID '" + details.m_string1 + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return FALSE;
				}
			}
		}
		break;
	case TagnamesList:
		{
			if(!m_itemlist)
			{
				SHOW_INTERNAL_ERROR;
				return FALSE;
			}

			int index = 0;
			POSITION pos = m_itemlist->GetHeadPosition();
			while(pos)
			{
				SStringPairs details = m_itemlist->GetAt(pos);
				if(InsertEntry(&details, index))
					index++;
				else
				{
					CString errstr = "Could not add entry for voucher '" + details.m_string1 + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return FALSE;
				}

				m_itemlist->GetNext(pos);
			}

			m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid dialog type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CGenericListBoxDialog::InsertEntry(const SStringPairs *entry, int index)
{
	if(!entry)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(m_list.InsertItem(index,entry->m_string1) == -1)
	{
		CString errstr = "Could not insert first value as '" + entry->m_string1 + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!m_list.SetItemText(index,1,entry->m_string2))
	{
		CString errstr = "Could not insert second value as '" + entry->m_string2 + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}


BOOL CGenericListBoxDialog::InsertEntry(const CProductOldSerial *entry, int index)
{
	if(!entry)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CString serial;
	serial.Format("%d",entry->m_serial);

	if(m_list.InsertItem(index,entry->m_tagname) == -1)
	{
		CString errstr = "Could not insert tagname as '" + CString(entry->m_tagname) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!m_list.SetItemText(index,1,serial))
	{
		CString errstr = "Could not insert serial as '" + serial + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	m_list.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	return TRUE;
}


BOOL CGenericListBoxDialog::InsertEntry(const CString description, const CString prefix, const int number_of_pieces, int index)
{
	CString number;
	number.Format("%d",number_of_pieces);

	if(m_list.InsertItem(index, description) == -1)
	{
		CString errstr = "Could not insert description as '" + description + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!m_list.SetItemText(index, 1, prefix))
	{
		CString errstr = "Could not insert prefix as '" + prefix + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!m_list.SetItemText(index, 2, number))
	{
		CString errstr = "Could not insert number of pieces as '" + number + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	m_list.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	return TRUE;
}


BOOL CGenericListBoxDialog::InsertEntry(const CProductSubItem *type, int index)
{
	if(!type)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(m_list.InsertItem(index , type->m_type) == -1)
	{
		CString errstr = "Could not insert type as '" + CString(type->m_type) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	CString str;
	str.Format("%.2f",type->m_weight);
	if(!m_list.SetItemText(index,1,str))
	{
		CString errstr = "Could not insert weight as '" + str + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	str.Format("%d",type->m_rate);
	if(!m_list.SetItemText(index,2,str))
	{
		CString errstr = "Could not insert rate as '" + str + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	str.Format("%d", type->m_count);
	if(!m_list.SetItemText(index,3,str))
	{
		CString errstr = "Could not insert piece count as '" + str + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	m_list.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	return TRUE;
}


void CGenericListBoxDialog::EditEntryAt(int item, int subitem, int type)
{
	if(item == -1)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	if(m_type == TagnamesList)
		subitem = 1;

	if(type != BEGIN_EDIT)
	{
		char editText[1024];
		int pos = m_editCtrl.GetLine(0, editText, 1023);
		editText[pos] = '\0';

		CString str = editText;
		if(!m_list.SetItemText(m_selected_item, m_selected_subitem, str))
		{
			CString errstr;
			errstr.Format("%d,%d", m_selected_item, m_selected_subitem);
			errstr = "Could not set the item text at position (" + errstr + ") to '" + str + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
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
	switch(m_type)
	{
	case DescriptionList:
		{
			CGenericListBoxDialog dlg(DescriptionModify);
			if(dlg.DoModal() == IDCANCEL)
				return;

			if(!PopulateList())
			{
				SHOW_INTERNAL_ERROR_REASON("Could not populate list.");
				return;
			}
		}
		break;
	case DescriptionModify:
	case DiamondDetail:
	case StoneDetail:
	case SerialDetail:
		{
			if(m_editCtrl.IsWindowVisible())
			{
				EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
				m_editCtrl.ShowWindow(SW_HIDE);
				m_selected_item = -1;
				m_selected_subitem = 0;
			}

			int position = m_list.GetItemCount();
			switch(m_type)
			{
			case DescriptionModify:
				{
					CString desc;
					CString prefix;
					int count = 0;
					if(!InsertEntry(desc, prefix, count, position))
					{
						SHOW_INTERNAL_ERROR_REASON("Could not insert empty description.");
						return;
					}
				}
				break;
			case DiamondDetail:
			case StoneDetail:
				{
					CProductSubItem emptytype;
					if(!InsertEntry(&emptytype, position))
					{
						SHOW_INTERNAL_ERROR_REASON("Could not insert empty subitem.");
						return;
					}
				}
				break;
			case SerialDetail:
				{
					CProductOldSerial entry;
					if(!InsertEntry(&entry, position))
					{
						SHOW_INTERNAL_ERROR_REASON("COuld not insert empty old serial entry.");
						return;
					}
				}
				break;
			default:
				{
					CString errstr;
					errstr.Format("%d", m_type);
					errstr = "Invalid dialog type '" + errstr + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return;
				}
			}
		
			EditEntryAt(position, 0, BEGIN_EDIT);
		}
		break;
	case ProductList:
	case TagnamesList:
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid dialog type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}
}

void CGenericListBoxDialog::OnButton2() 
{
	switch(m_type)
	{
	case DescriptionModify:
	case DiamondDetail:
	case StoneDetail:
	case SerialDetail:
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
				SHOW_ERROR("Please select an item to delete.");
				return;
			}

			int selection = m_list.GetNextSelectedItem(pos);
			if(!m_list.DeleteItem(selection))
			{
				SHOW_INTERNAL_ERROR_REASON("Could not delete selected entry.");
				return;
			}

			if(selection < m_list.GetItemCount())
				m_list.SetItemState(selection, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			else
				m_list.SetItemState(m_list.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
		break;
	case DescriptionList:
	case ProductList:
	case TagnamesList:
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid dialog type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}
}

void CGenericListBoxDialog::OnOK() 
{
	if(!UpdateData(TRUE))
		return;

	switch(m_type)
	{
	case DescriptionList:
		{
			POSITION pos = m_list.GetFirstSelectedItemPosition();
			if(pos)
			{
				int index = m_list.GetNextSelectedItem(pos);
				m_selection = m_list.GetItemText(index, 0);
			}
		}
		break;
	case DescriptionModify:
		{
			if(m_editCtrl.IsWindowVisible())
			{
				EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
				m_editCtrl.ShowWindow(SW_HIDE);
				m_selected_item = -1;
				m_selected_subitem = 0;
			}

			if(!CSettings::RemoveAllCategories())
			{
				SHOW_INTERNAL_ERROR_REASON("Could not delete all categories in settings.");
				return;
			}

			int total = m_list.GetItemCount();
			int current = 0;
			while(current < total)
			{
				CString description = m_list.GetItemText(current, 0);
				CString prefix = m_list.GetItemText(current, 1);
				CString number = m_list.GetItemText(current, 2);
				int number_of_pieces = atoi(LPCTSTR(number));

				if(!description.IsEmpty())
				{
					if(!CSettings::AddCategory(description, prefix, number_of_pieces))
					{
						CString errstr = "Could not add category '" + description + "' to the settings.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return;
					}
				}

				current++;
			}
		}
		break;
	case DiamondDetail:
	case StoneDetail:
		{
			if(!m_item)
			{
				SHOW_INTERNAL_ERROR;
				return;
			}

			if(m_editCtrl.IsWindowVisible())
			{
				EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
				m_editCtrl.ShowWindow(SW_HIDE);
				m_selected_item = -1;
				m_selected_subitem = 0;
			}

			switch(m_type)
			{
			case DiamondDetail:
				if(!m_item->DeleteSubItemList(tDiamond))
				{
					CString errstr = "Could not delete all diamond subitems in the item '" + CString(m_item->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return;
				}
				break;
			case StoneDetail:
				if(!m_item->DeleteSubItemList(tStone))
				{
					CString errstr = "Could not delete all stone subitems in the item '" + CString(m_item->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return;
				}
				break;
			default:
				{
					CString errstr;
					errstr.Format("%d", m_type);
					errstr = "Invalid dialog type '" + errstr + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return;
				}
			}

			int total = m_list.GetItemCount();
			int current = 0;
			while(current < total)
			{
				CString name = m_list.GetItemText(current, 0);
				CString weight = m_list.GetItemText(current, 1);
				CString rate = m_list.GetItemText(current, 2);
				CString count = m_list.GetItemText(current, 3);
				
				if(!name.IsEmpty())
				{
					CProductSubItem type;
				
					strcpy(type.m_type, LPCTSTR(name));
					type.m_weight = RoundOff(atof(LPCTSTR(weight)), 2);
					type.m_rate = atoi(LPCTSTR(rate));
					type.m_count = atoi(LPCTSTR(count));

					switch(m_type)
					{
					case DiamondDetail:
						if(!m_item->AddSubItem(tDiamond, &type))
						{
							CString errstr = "Could not add diamond subitem '" + CString(type.m_type) + "' to the item '" + CString(m_item->m_product_ID) + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);
							return;
						}
						break;
					case StoneDetail:
						if(!m_item->AddSubItem(tStone, &type))
						{
							CString errstr = "Could not add stone subitem '" + CString(type.m_type) + "' to the item '" + CString(m_item->m_product_ID) + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);
							return;
						}
						break;
					default:
						{
							CString errstr;
							errstr.Format("%d", m_type);
							errstr = "Invalid dialog type '" + errstr + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);
							return;
						}
					}
				}

				current++;
			}
		}
		break;
	case ProductList:
		{
			POSITION pos = m_list.GetFirstSelectedItemPosition();
			while(pos)
			{
				int index = m_list.GetNextSelectedItem(pos);
				SStringPairs details;
				details.m_string1 = m_list.GetItemText(index, 0);
				details.m_string2 = m_list.GetItemText(index, 1);
				m_itemlist->AddTail(details);
			}
		}
		break;
	case TagnamesList:
		{
			if(!m_itemlist)
			{
				SHOW_INTERNAL_ERROR;
				return;
			}

			if(m_editCtrl.IsWindowVisible())
			{
				EditEntryAt(m_selected_item, 1, END_EDIT);
				m_editCtrl.ShowWindow(SW_HIDE);
				m_selected_item = -1;
				m_selected_subitem = 0;
			}

			m_itemlist->RemoveAll();
			int total = m_list.GetItemCount();
			int current = 0;
			while(current < total)
			{
				SStringPairs details;
				details.m_string1 = m_list.GetItemText(current, 0);
				details.m_string2 = m_list.GetItemText(current, 1);
				m_itemlist->AddTail(details);
				current++;
			}
		}
		break;
	case SerialDetail:
		{
			if(!m_item)
			{
				SHOW_INTERNAL_ERROR;
				return;
			}

			if(m_editCtrl.IsWindowVisible())
			{
				EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
				m_editCtrl.ShowWindow(SW_HIDE);
				m_selected_item = -1;
				m_selected_subitem = 0;
			}

			if(!m_item->DeleteOldSerialList())
			{
				CString errstr = "Could not delete all old serials in the item '" + CString(m_item->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return;
			}

			int total = m_list.GetItemCount();
			int current = 0;
			while(current < total)
			{
				CString tagname = m_list.GetItemText(current, 0);
				CString serial = m_list.GetItemText(current, 1);
				
				if(!tagname.IsEmpty())
				{
					CProductOldSerial entry;
					entry.m_serial = atoi(LPCTSTR(serial));
					strcpy(entry.m_tagname, tagname);
					if(!m_item->AddOldSerial(&entry))
					{
						CString errstr = "Could not add old serial with tagname '" + CString(entry.m_tagname) + "' to the item '" + CString(m_item->m_product_ID) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return;
					}
				}

				current++;
			}
		}
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid dialog type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
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
	*pResult = 1;

	switch(m_type)
	{
	case DescriptionModify:
	case DiamondDetail:
	case StoneDetail:
	case SerialDetail:
	case TagnamesList:
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
					switch(m_type)
					{
					case DescriptionModify:
						{
							CString desc;
							CString prefix;
							int count = 0;

							if(!InsertEntry(desc, prefix, count, 0))
							{
								SHOW_INTERNAL_ERROR_REASON("Could not insert empty description.");
								return;
							}

							m_selected_item = 0;
							m_selected_subitem = 0;
						}
						break;
					case DiamondDetail:
					case StoneDetail:
						{
							CProductSubItem emptytype;
							if(!InsertEntry(&emptytype, 0))
							{
								SHOW_INTERNAL_ERROR_REASON("Could not insert empty subitem.");
								return;
							}

							m_selected_item = 0;
							m_selected_subitem = 0;
						}
						break;
					case SerialDetail:
						{
							CProductOldSerial entry;
							if(!InsertEntry(&entry, 0))
							{
								SHOW_INTERNAL_ERROR_REASON("Could not insert empty old serial info.");
								return;
							}

							m_selected_item = 0;
							m_selected_subitem = 0;
						}
						break;
					case TagnamesList:
						m_selected_item = -1;
						m_selected_subitem = 0;
						break;
					default:
						{
							CString errstr;
							errstr.Format("%d", m_type);
							errstr = "Invalid dialog type '" + errstr + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);
							return;
						}
					}
					
					EditEntryAt(m_selected_item, m_selected_subitem, BEGIN_EDIT);
				}
				else
				{
					m_editCtrl.ShowWindow(SW_HIDE);
				}
			}
		}
		break;
	case DescriptionList:
	case ProductList:
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid dialog type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}
	
	*pResult = 0;
}


void CGenericListBoxDialog::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE *data = (NMITEMACTIVATE *)pNMHDR;
	*pResult = 1;

	switch(m_type)
	{
	case DescriptionList:
		{
			if(data->iItem >= 0)
			{
				m_selection = m_list.GetItemText(data->iItem,0);
				*pResult = 0;
				CDialog::OnOK();
			}
		}
		break;
	case DescriptionModify:
	case DiamondDetail:
	case StoneDetail:
	case SerialDetail:
	case TagnamesList:
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
		break;
	case ProductList:
		{
			if(data->iItem >= 0)
			{
				SStringPairs details;
				details.m_string1 = m_list.GetItemText(data->iItem, 0);
				details.m_string2 = m_list.GetItemText(data->iItem, 1);
				m_itemlist->AddTail(details);
				CDialog::OnOK();
			}
		}
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid dialog type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}

	*pResult = 0;
}


void CGenericListBoxDialog::OnSetfocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 1;

	switch(m_type)
	{
	case DescriptionModify:
	case DiamondDetail:
	case StoneDetail:
	case SerialDetail:
	case TagnamesList:
		{
			if(m_editCtrl.IsWindowVisible())
			{
				EditEntryAt(m_selected_item, m_selected_subitem, END_EDIT);
				m_editCtrl.ShowWindow(SW_HIDE);
				m_selected_item = -1;
				m_selected_subitem = 0;
			}
		}
		break;
	case DescriptionList:
	case ProductList:
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid dialog type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}
	
	*pResult = 0;
}


BOOL CGenericListBoxDialog::PreTranslateMessage(MSG* pMsg)
{
	switch(m_type)
	{
	case DescriptionModify:
	case DiamondDetail:
	case StoneDetail:
	case SerialDetail:
	case TagnamesList:
		{
			CWnd *pCont = GetFocus();
			if(pCont == &m_editCtrl)
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
								if(m_type == TagnamesList)
								{
									if((m_selected_item + 1) == m_list.GetItemCount())
										item = 0;
									else
										item++;

									EditEntryAt(item, 1);
								}
								else
								{
									pos++;
									if(pos == m_number_of_columns)//end of item
									{
										if((m_selected_item + 1) == m_list.GetItemCount())
										{
											int position = m_list.GetItemCount();
											switch(m_type)
											{
											case DescriptionModify:
												{
													CString desc;
													CString prefix;
													int count = 0;
													if(!InsertEntry(desc, prefix, count, position))
													{
														SHOW_INTERNAL_ERROR_REASON("Could not add empty description.");
														return TRUE;
													}
												}
												break;
											case DiamondDetail:
											case StoneDetail:
												{
													CProductSubItem emptytype;
													if(!InsertEntry(&emptytype, position))
													{
														SHOW_INTERNAL_ERROR_REASON("Could not add empty subitem.");
														return TRUE;
													}
												}
												break;
											case SerialDetail:
												{
													CProductOldSerial entry;
													if(!InsertEntry(&entry, position))
													{
														SHOW_INTERNAL_ERROR_REASON("Could not add empty old serial info.");
														return TRUE;
													}
												}
												break;
											default:
												{
													CString errstr;
													errstr.Format("%d", m_type);
													errstr = "Invalid dialog type '" + errstr + "'.";
													SHOW_INTERNAL_ERROR_REASON(errstr);
													return TRUE;
												}
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
							}

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
		break;
	case DescriptionList:
	case ProductList:
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid dialog type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

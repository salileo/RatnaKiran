#include "stdafx.h"
#include "ratnakiran.h"
#include "MainGlobal.h"
#include "AddDialog.h"
#include "MarkedValueFormulaDialog.h"
#include "GenericListBoxDialog.h"
#include "GenericOneEditDialog.h"
#include "DescriptionAddDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAddDialog::CAddDialog(DatabaseType type, CProductSet *set, double gold_rate, CWnd* pParent /*=NULL*/)
: CDialog(((type == tStock) ? IDD_ADD_DIALOG_STOCK : ((type == tKundan) ? IDD_ADD_DIALOG_KUNDAN : ((type == tExport) ? IDD_ADD_DIALOG_EXPORT : 0))), pParent)
{
	//{{AFX_DATA_INIT(CAddDialogStock)
	//}}AFX_DATA_INIT

	ClearSet();
	ClearItem();

	m_type = type;
	m_set = set;
	m_item = NULL;
	m_gold_rate = gold_rate;
	m_disable_killmessage = FALSE;
	m_conversion = 1.0;
	m_currency_name = "Rs.";
	m_desired_serial = 1;

	if(set)
		m_original_set = *set;
}


CAddDialog::CAddDialog(DatabaseType type, CProductSet *set, double gold_rate, double conversion, CString currency_name, int desired_serial, CWnd* pParent /*=NULL*/)
: CDialog(((type == tStock) ? IDD_ADD_DIALOG_STOCK : ((type == tKundan) ? IDD_ADD_DIALOG_KUNDAN : ((type == tExport) ? IDD_ADD_DIALOG_EXPORT : 0))), pParent)
{
	//{{AFX_DATA_INIT(CAddDialogStock)
	//}}AFX_DATA_INIT

	ClearSet();
	ClearItem();

	m_type = type;
	m_set = set;
	m_item = NULL;
	m_gold_rate = gold_rate;
	m_disable_killmessage = FALSE;
	m_conversion = conversion;
	m_currency_name = currency_name;
	m_desired_serial = desired_serial;

	if(set)
		m_original_set = *set;
}


void CAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddDialog)
	DDX_Control(pDX, IDC_ITEM_TABS, m_tabs);
	DDX_Control(pDX, IDC_IMAGE_FRAME, m_image_frame);

	switch(m_type)
	{
	case tStock:
		DDX_Text(pDX, IDC_SET_DESCRIPTION, m_set_description);
		DDX_Text(pDX, IDC_SET_PRODUCTID, m_set_productID);
		DDX_Text(pDX, IDC_UNSOLD_PIECE_COUNT, m_set_piece_count);
		DDX_Text(pDX, IDC_ITEM_DESCRIPTION, m_item_description);
		DDX_Text(pDX, IDC_ITEM_PRODUCTID, m_item_productID);
		DDX_Text(pDX, IDC_ITEM_PHOTO, m_item_photo);
		DDX_Text(pDX, IDC_ITEM_PIECE_COUNT, m_item_piece_count);
		DDX_Text(pDX, IDC_GROSS_WEIGHT, m_gross_weight);
		DDX_Text(pDX, IDC_NET_WEIGHT, m_net_weight);
		DDX_Text(pDX, IDC_GOLD_VALUE, m_gold_value);
		DDX_Text(pDX, IDC_DIAMOND_RATE, m_diamond_rate);
		DDX_Text(pDX, IDC_DIAMOND_WEIGHT, m_diamond_weight);
		DDX_Text(pDX, IDC_DIAMOND_VALUE, m_diamond_value);
		DDX_Text(pDX, IDC_DIAMOND_MARKED_RATE, m_diamond_marked_rate);
		DDX_Text(pDX, IDC_DIAMOND_MARKED_VALUE, m_diamond_marked_value);
		DDX_Text(pDX, IDC_DIAMOND_CLARITY, m_diamond_clarity);
		DDX_Text(pDX, IDC_STONE_WEIGHT, m_precious_stone_weight);
		DDX_Text(pDX, IDC_STONE_RATE, m_precious_stone_rate);
		DDX_Text(pDX, IDC_STONE_VALUE, m_precious_stone_value);
		DDX_Text(pDX, IDC_STONE_MARKED_RATE, m_precious_stone_marked_rate);
		DDX_Text(pDX, IDC_STONE_MARKED_VALUE, m_precious_stone_marked_value);
		DDX_Text(pDX, IDC_PEARL_WEIGHT, m_pearl_weight);
		DDX_Text(pDX, IDC_PEARL_RATE, m_pearl_rate);
		DDX_Text(pDX, IDC_PEARL_VALUE, m_pearl_value);
		DDX_Text(pDX, IDC_PEARL_MARKED_RATE, m_pearl_marked_rate);
		DDX_Text(pDX, IDC_PEARL_MARKED_VALUE, m_pearl_marked_value);
		DDX_Text(pDX, IDC_LABOUR_CHARGE, m_labour_charge);
		DDX_Text(pDX, IDC_MARKED_LABOUR_CHARGE, m_marked_labour_charge);
		DDX_Text(pDX, IDC_OTHER_WEIGHT, m_other_weight);
		DDX_Text(pDX, IDC_TOTAL_PRICE, m_total_price);
		DDX_Text(pDX, IDC_TOTAL_MARKED_PRICE, m_total_marked_price);
		DDX_Check(pDX, IDC_ITEM_SOLD, m_item_sold);
		DDX_Text(pDX, IDC_SOLD_COUNTRY, m_sold_country);
		DDX_Check(pDX, IDC_ITEM_USE_NET_WEIGHT, m_use_net_weight);
		break;
	case tKundan:
		DDX_Text(pDX, IDC_SET_DESCRIPTION, m_set_description);
		DDX_Text(pDX, IDC_SET_PRODUCTID, m_set_productID);
		DDX_Text(pDX, IDC_UNSOLD_PIECE_COUNT, m_set_piece_count);
		DDX_Text(pDX, IDC_ITEM_DESCRIPTION, m_item_description);
		DDX_Text(pDX, IDC_ITEM_PRODUCTID, m_item_productID);
		DDX_Text(pDX, IDC_ITEM_PHOTO, m_item_photo);
		DDX_Text(pDX, IDC_ITEM_PIECE_COUNT, m_item_piece_count);
		DDX_Text(pDX, IDC_GROSS_WEIGHT, m_gross_weight);
		DDX_Text(pDX, IDC_DIAMOND_RATE, m_diamond_rate);
		DDX_Text(pDX, IDC_DIAMOND_WEIGHT, m_diamond_weight);
		DDX_Text(pDX, IDC_DIAMOND_VALUE, m_diamond_value);
		DDX_Text(pDX, IDC_DIAMOND_CLARITY, m_diamond_clarity);
		DDX_Text(pDX, IDC_STONE_WEIGHT, m_precious_stone_weight);
		DDX_Text(pDX, IDC_STONE_RATE, m_precious_stone_rate);
		DDX_Text(pDX, IDC_STONE_VALUE, m_precious_stone_value);
		DDX_Text(pDX, IDC_PEARL_WEIGHT, m_pearl_weight);
		DDX_Text(pDX, IDC_PEARL_RATE, m_pearl_rate);
		DDX_Text(pDX, IDC_PEARL_VALUE, m_pearl_value);
		DDX_Text(pDX, IDC_COSTING, m_costing);
		DDX_Text(pDX, IDC_MARKED_COSTING, m_marked_costing);
		DDX_Text(pDX, IDC_OTHER_WEIGHT, m_other_weight);
		DDX_Text(pDX, IDC_TOTAL_PRICE, m_total_price);
		DDX_Text(pDX, IDC_TOTAL_MARKED_PRICE, m_total_marked_price);
		DDX_Check(pDX, IDC_ITEM_SOLD, m_item_sold);
		DDX_Text(pDX, IDC_SOLD_COUNTRY, m_sold_country);
		break;
	case tExport:
		DDX_Text(pDX, IDC_ITEM_DESCRIPTION, m_item_description);
		DDX_Text(pDX, IDC_ITEM_PRODUCTID, m_item_productID);
		DDX_Text(pDX, IDC_ITEM_PHOTO, m_item_photo);
		DDX_Text(pDX, IDC_ITEM_PIECE_COUNT, m_item_piece_count);
		DDX_Text(pDX, IDC_GROSS_WEIGHT, m_gross_weight);
		DDX_Text(pDX, IDC_NET_WEIGHT, m_net_weight);
		DDX_Text(pDX, IDC_GOLD_VALUE, m_gold_value);
		DDX_Text(pDX, IDC_DIAMOND_WEIGHT, m_diamond_weight);
		DDX_Text(pDX, IDC_DIAMOND_RATE, m_diamond_rate);
		DDX_Text(pDX, IDC_DIAMOND_VALUE, m_diamond_value);
		DDX_Text(pDX, IDC_DIAMOND_CLARITY, m_diamond_clarity);
		DDX_Text(pDX, IDC_STONE_WEIGHT, m_precious_stone_weight);
		DDX_Text(pDX, IDC_STONE_RATE, m_precious_stone_rate);
		DDX_Text(pDX, IDC_STONE_VALUE, m_precious_stone_value);
		DDX_Text(pDX, IDC_PEARL_WEIGHT, m_pearl_weight);
		DDX_Text(pDX, IDC_PEARL_RATE, m_pearl_rate);
		DDX_Text(pDX, IDC_PEARL_VALUE, m_pearl_value);
		DDX_Text(pDX, IDC_LABOUR_CHARGE, m_labour_charge);
		DDX_Text(pDX, IDC_OTHER_WEIGHT, m_other_weight);
		DDX_Text(pDX, IDC_TOTAL_PRICE, m_total_price);
		DDX_Check(pDX, IDC_ITEM_SOLD, m_item_sold);
		DDX_Text(pDX, IDC_SOLD_COUNTRY, m_sold_country);
		DDX_Text(pDX, IDC_SERIAL_NUMBER, m_serial_number);
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddDialog, CDialog)
	//{{AFX_MSG_MAP(CAddDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ITEM_TABS, OnSelchangeItemTabs)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_ITEM_ADD, OnItemAdd)
	ON_BN_CLICKED(ID_ITEM_DELETE, OnItemDelete)
	ON_BN_CLICKED(ID_SET_DESCRIPTION_LIST, OnSetDescriptionList)
	ON_BN_CLICKED(ID_ITEM_DESCRIPTION_LIST, OnItemDescriptionList)
	ON_BN_CLICKED(ID_ITEM_CHANGE_IMAGE, OnItemChangeImage)
	ON_BN_CLICKED(ID_ITEM_REMOVE_IMAGE, OnItemRemoveImage)
	ON_BN_CLICKED(IDC_ITEM_SOLD, OnItemSold)
	ON_BN_CLICKED(IDC_ITEM_USE_NET_WEIGHT, OnItemUseNetWeight)
	ON_BN_CLICKED(ID_DIAMOND_DETAILS, OnDiamondDetails)
	ON_BN_CLICKED(ID_STONE_DETAILS, OnStoneDetails)
	ON_BN_CLICKED(ID_MARKED_VALUES, OnMarkedValues)
	ON_BN_CLICKED(ID_OLD_SERIALS, OnOldSerials)
	ON_BN_CLICKED(ID_SET_SAVE, OnSetSave)
	ON_BN_CLICKED(ID_SET_CANCEL, OnSetCancel)
	ON_CBN_KILLFOCUS(IDC_SET_DESCRIPTION, OnKillfocusSetDescription)
	ON_CBN_KILLFOCUS(IDC_ITEM_DESCRIPTION, OnKillfocusItemDescription)
	ON_EN_KILLFOCUS(IDC_ITEM_PIECE_COUNT, OnKillfocusItemPieceCount)
	ON_EN_KILLFOCUS(IDC_GROSS_WEIGHT, OnKillfocusGrossWeight)
	ON_EN_KILLFOCUS(IDC_COSTING, OnKillfocusCosting)
	ON_EN_KILLFOCUS(IDC_LABOUR_CHARGE, OnKillfocusLabourCharge)
	ON_EN_KILLFOCUS(IDC_OTHER_WEIGHT, OnKillfocusOtherWeight)
	ON_EN_KILLFOCUS(IDC_PEARL_RATE, OnKillfocusPearlRate)
	ON_EN_KILLFOCUS(IDC_PEARL_WEIGHT, OnKillfocusPearlWeight)
	ON_EN_KILLFOCUS(IDC_SOLD_COUNTRY, OnKillfocusSoldCountry)
	ON_EN_KILLFOCUS(IDC_SERIAL_NUMBER, OnKillfocusSerialNumber)
	ON_EN_KILLFOCUS(IDC_DIAMOND_CLARITY, OnKillfocusDiamondClarity)
	ON_CBN_EDITCHANGE(IDC_SET_DESCRIPTION, OnEditchangeSetDescription)
	ON_CBN_EDITCHANGE(IDC_ITEM_DESCRIPTION, OnEditchangeItemDescription)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CAddDialog::ClearSet()
{
	m_set_description = _T("");
	m_set_productID = _T("");
	m_set_piece_count = 0;
}

void CAddDialog::ClearItem()
{
	m_item_description = _T("");
	m_item_productID = _T("");
	m_item_photo = _T("");
	m_item_piece_count = 0;
	m_gross_weight = 0.0;
	m_net_weight = 0.0;
	m_gold_value = 0;
	m_diamond_rate = 0;
	m_diamond_weight = 0.0;
	m_diamond_value = 0;
	m_diamond_marked_rate = 0;
	m_diamond_marked_value = 0;
	m_diamond_clarity = _T("");
	m_precious_stone_weight = 0.0;
	m_precious_stone_rate = 0;
	m_precious_stone_value = 0;
	m_precious_stone_marked_rate = 0;
	m_precious_stone_marked_value = 0;
	m_pearl_weight = 0.0;
	m_pearl_rate = 0;
	m_pearl_value = 0;
	m_pearl_marked_rate = 0;
	m_pearl_marked_value = 0;
	m_labour_charge = 0;
	m_marked_labour_charge = 0;
	m_other_weight = 0.0;
	m_costing = 0.0;
	m_marked_costing = 0.0;
	m_total_price = 0;
	m_total_marked_price = 0;
	m_item_sold = FALSE;
	m_sold_country = _T("");
	m_use_net_weight = FALSE;
	m_serial_number = 0;
}


BOOL CAddDialog::UpdateSetValues(BOOL redraw, int item_index)
{
	ClearSet();
	ClearItem();

	if(!m_set)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if((m_type == tStock) || (m_type == tKundan))
	{
		CComboBox *cb = (CComboBox *)GetDlgItem(IDC_SET_DESCRIPTION);
		if(cb)
		{
			cb->ResetContent();

			int index = 0;
			while(index < CSettings::GetCategoryCount())
			{
				CString desc;
				if(CSettings::GetCategory(index, &desc, NULL, NULL))
					cb->AddString(desc);
				else
				{
					CString errstr;
					errstr.Format("%d", index);
					errstr = "Could not get category number '" + errstr + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}

				index++;
			}
		}
		else
			SHOW_INTERNAL_ERROR_REASON("Could not find set description control.");

		m_set_productID = m_set->m_product_ID;
		m_set_description = m_set->m_description;
		m_set_piece_count = m_set->GetTotalUnsoldItemCount();
	}

	if(!UpdateTabs())
	{
		SHOW_INTERNAL_ERROR_REASON("Could not update tabs.");
		return FALSE;
	}

	if((item_index < 0) ||
	   (item_index >= m_tabs.GetItemCount()))
	{
		item_index = 0;
	}

	if(!GotoTab(item_index, redraw))
	{
		CString errstr;
		errstr.Format("%d", item_index);
		errstr = "Could not select tab number '" + errstr + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}

BOOL CAddDialog::UpdateItemValues(BOOL redraw)
{
	ClearItem();

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_ITEM_DESCRIPTION);
	if(cb)
	{
		cb->ResetContent();

		int index = 0;
		while(index < CSettings::GetCategoryCount())
		{
			CString desc;
			if(CSettings::GetCategory(index, &desc, NULL, NULL))
				cb->AddString(desc);
			else
			{
				CString errstr;
				errstr.Format("%d", index);
				errstr = "Could not get category number '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}

			index++;
		}
	}
	else
		SHOW_INTERNAL_ERROR_REASON("Could not find item description control");

	switch(m_type)
	{
	case tStock:
		m_item_productID = m_item->m_product_ID;
		m_item_description = m_item->m_description;
		m_item_photo = m_item->m_photo_filename;
		m_item_piece_count = m_item->m_piece_count;
		m_gross_weight = m_item->GetGrossWeight();
		m_net_weight = m_item->GetNetWeight();
		m_gold_value = m_item->GetGoldValue(m_item->m_use_net_weight_for_gold, TRUE, m_gold_rate, 1.0);
		m_diamond_weight = m_item->GetSubItemWeight(tDiamond);
		m_diamond_rate = m_item->GetSubItemRate(tDiamond, 1.0);
		m_diamond_value = m_item->GetSubItemValue(tDiamond, 1.0);
		m_diamond_marked_rate = m_item->GetMarkedSubItemRate(tDiamond, m_gold_rate, 1.0);
		m_diamond_marked_value = m_item->GetMarkedSubItemValue(tDiamond, m_gold_rate, 1.0);
		m_diamond_clarity = m_item->m_diamond_clarity;
		m_precious_stone_weight = m_item->GetSubItemWeight(tStone);
		m_precious_stone_rate = m_item->GetSubItemRate(tStone, 1.0);
		m_precious_stone_value = m_item->GetSubItemValue(tStone, 1.0);
		m_precious_stone_marked_rate = m_item->GetMarkedSubItemRate(tStone, m_gold_rate, 1.0);
		m_precious_stone_marked_value = m_item->GetMarkedSubItemValue(tStone, m_gold_rate, 1.0);
		m_pearl_weight = m_item->GetSubItemWeight(tPearl);
		m_pearl_rate = m_item->GetSubItemRate(tPearl, 1.0);
		m_pearl_value = m_item->GetSubItemValue(tPearl, 1.0);
		m_pearl_marked_rate = m_item->GetMarkedSubItemRate(tPearl, m_gold_rate, 1.0);
		m_pearl_marked_value = m_item->GetMarkedSubItemValue(tPearl, m_gold_rate, 1.0);
		m_labour_charge = m_item->GetLabourCost(1.0);
		m_marked_labour_charge = m_item->GetMarkedLabourCost(m_gold_rate, 1.0);
		m_other_weight = m_item->GetOtherWeight();
		m_total_price = m_item->GetTotalValue(m_item->m_use_net_weight_for_gold, TRUE, m_gold_rate, 1.0);
		m_total_marked_price = m_item->GetTotalMarkedValue(m_item->m_use_net_weight_for_gold, TRUE, m_gold_rate, 1.0);
		m_use_net_weight = m_item->m_use_net_weight_for_gold;
		m_item_sold = m_item->m_sold;
		m_sold_country = m_item->m_sold_country;
		break;
	case tExport:
		m_item_productID = m_item->m_product_ID;
		m_item_description = m_item->m_description;
		m_item_photo = m_item->m_photo_filename;
		m_item_piece_count = m_item->m_piece_count;
		m_serial_number = m_item->m_serial_number;
		m_gross_weight = m_item->GetGrossWeight();
		m_net_weight = m_item->GetNetWeight();
		m_gold_value = m_item->GetGoldValue(TRUE, FALSE, m_gold_rate, m_conversion);
		m_diamond_weight = m_item->GetSubItemWeight(tDiamond);
		m_diamond_rate = m_item->GetSubItemRate(tDiamond, m_conversion);
		m_diamond_value = m_item->GetSubItemValue(tDiamond, m_conversion);
		m_diamond_clarity = m_item->m_diamond_clarity;
		m_precious_stone_weight = m_item->GetSubItemWeight(tStone);
		m_precious_stone_rate = m_item->GetSubItemRate(tStone, m_conversion);
		m_precious_stone_value = m_item->GetSubItemValue(tStone, m_conversion);
		m_pearl_weight = m_item->GetSubItemWeight(tPearl);
		m_pearl_rate = m_item->GetSubItemRate(tPearl, m_conversion);
		m_pearl_value = m_item->GetSubItemValue(tPearl, m_conversion);
		m_labour_charge = m_item->GetLabourCost(m_conversion);
		m_other_weight = m_item->GetOtherWeight();
		m_total_price = m_item->GetTotalValue(TRUE, FALSE, m_gold_rate, m_conversion);
		m_item_sold = m_item->m_sold;
		m_sold_country = m_item->m_sold_country;
		break;
	case tKundan:
		m_item_productID = m_item->m_product_ID;
		m_item_description = m_item->m_description;
		m_item_photo = m_item->m_photo_filename;
		m_item_piece_count = m_item->m_piece_count;
		m_gross_weight = m_item->GetGrossWeight();
		m_diamond_weight = m_item->GetSubItemWeight(tDiamond);
		m_diamond_rate = m_item->GetSubItemRate(tDiamond, 1.0);
		m_diamond_value = m_item->GetSubItemValue(tDiamond, 1.0);
		m_diamond_clarity = m_item->m_diamond_clarity;
		m_precious_stone_weight = m_item->GetSubItemWeight(tStone);
		m_precious_stone_rate = m_item->GetSubItemRate(tStone, 1.0);
		m_precious_stone_value = m_item->GetSubItemValue(tStone, 1.0);
		m_pearl_weight = m_item->GetSubItemWeight(tPearl);
		m_pearl_rate = m_item->GetSubItemRate(tPearl, 1.0);
		m_pearl_value = m_item->GetSubItemValue(tPearl, 1.0);
		m_other_weight = m_item->GetOtherWeight();
		m_costing = m_item->GetCostingDouble(1.0);
		m_marked_costing = m_item->GetMarkedCostingDouble(1.0);
		m_total_price = m_item->GetTotalCosting(1.0);
		m_total_marked_price = m_item->GetTotalMarkedCosting(1.0);
		m_item_sold = m_item->m_sold;
		m_sold_country = m_item->m_sold_country;
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", m_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	if(redraw)
	{
		if(!DrawImage(m_item_photo))
		{
			CString errstr = "Could not draw image '" + m_item_photo + "' of item '" + CString(m_item->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}

	return TRUE;
}


BOOL CAddDialog::DrawImage(CString filename)
{
	//Get Window details
	if(!IsWindow(m_image_frame.m_hWnd))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find image frame");
		return FALSE;
	}

	CClientDC dc(&m_image_frame);
	int mode = dc.GetStretchBltMode();
	//dc.SetStretchBltMode(COLORONCOLOR);
	dc.SetStretchBltMode(HALFTONE);

	CRect rect;
	m_image_frame.GetClientRect(rect);
	CSize window_size(rect.Width(), rect.Height());

	//Empty the pane
	CBitmap empty_img;
	if(!empty_img.LoadBitmap(IDB_BLACK_IMAGE))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not load empty black image.");
		return FALSE;
	}

	CDC dcMem;
	if(!dcMem.CreateCompatibleDC(NULL))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not setup drawing environment");
		return FALSE;
	}

	if(!dcMem.SelectObject(&empty_img))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not setup drawing parameters.");
		return FALSE;
	}

	if(!dc.StretchBlt(0, 0, window_size.cx, window_size.cy, &dcMem , 0, 0, 10, 10, SRCCOPY))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not draw empty black image.");
		return FALSE;
	}
	
	if(filename.IsEmpty())
		return TRUE;

	BYTE *image_data;
	DWORD image_size;
	if(!OpenImage(filename, &image_data, &image_size))
	{
		CString errstr = "Could not open image file '" + filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	SPicture pic;
	if(!pic.LoadFromBuffer(image_data, image_size))
	{
		CString errstr = "Could not load image '" + filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	CSize original_size = pic.GetSize(&dc);
	CSize final_size;
	CPoint position = CProductItem::ScaleImage(original_size, window_size, &final_size);
	if(!pic.Draw(&dc, position, final_size))
	{
		CString errstr = "Could not draw image '" + filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}


BOOL CAddDialog::GotoTab(int index, BOOL redraw)
{
	ClearItem();

	if(!m_set)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(index < 0)
		m_item = NULL;
	else if(m_tabs.GetCurSel() == index)
		m_item = m_set->GetItemAt(index);
	else
	{
		if(m_tabs.SetCurSel(index) == -1)
			m_item = NULL;
		else
			m_item = m_set->GetItemAt(index);
	}

	if(!UpdateItemValues(redraw))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}


BOOL CAddDialog::UpdateTabs()
{
	ClearItem();

	if(!m_set)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!m_tabs.DeleteAllItems())
	{
		SHOW_INTERNAL_ERROR_REASON("Could not delete existing tabs.");
		return FALSE;
	}

	int counter = 0;
	while(counter < m_set->GetItemCount())
	{
		CProductItem *item = m_set->GetItemAt(counter);
		CString str;
		if(!item)
		{
			CString errstr;
			errstr.Format("%d", counter);
			errstr = "Could not get item at position '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}

		str = item->m_description;
		if(str.IsEmpty())
			str = "Untitled";
	
		if(m_tabs.InsertItem(counter, str) == -1)
		{
			CString errstr = "Could not add item '" + str + "' to tabs.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}

		counter++;
	}

	return TRUE;
}


BOOL CAddDialog::OnInitDialog() 
{
	m_disable_killmessage = TRUE;

	if(!CDialog::OnInitDialog())
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return FALSE;
	}

	if(!m_set)
	{
		SHOW_INTERNAL_ERROR;
		CDialog::OnCancel();
		return FALSE;
	}

	if(m_type == tExport)
	{
		if(CSettings::GetUseNativeCurrency())
		{
			if(!m_currency_name.IsEmpty())
			{
				CString str;
				GetWindowText(str);

				str = str + " (in " + m_currency_name + ")";
				SetWindowText(LPCTSTR(str));
			}
		}
		else
		{
			CString str;
			GetWindowText(str);

			str = str + " (in USD)";
			SetWindowText(LPCTSTR(str));
		}
	}

	if(!UpdateSetValues(TRUE, 0))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not update set values.");
		m_disable_killmessage = FALSE;
		return FALSE;
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
	return TRUE;
}


void CAddDialog::OnSelchangeItemTabs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_disable_killmessage = TRUE;
	*pResult = 1;

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	int index = m_tabs.GetCurSel();
	if(GotoTab(index, TRUE))
		*pResult = 0;
	else
	{
		CString errstr;
		errstr.Format("%d", index);
		errstr = "Could not select tab number '" + errstr + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnPaint() 
{
	m_disable_killmessage = TRUE;
	CPaintDC dc(this);

	if(m_item)
	{
		CString filename = m_item->m_photo_filename;
		DrawImage(filename);
	}

	m_disable_killmessage = FALSE;
}


void CAddDialog::OnSetDescriptionList() 
{
	m_disable_killmessage = TRUE;

	if(!m_set)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CGenericListBoxDialog dlg(DescriptionList);
	if(dlg.DoModal() == IDCANCEL)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	if(m_set_description == dlg.m_selection)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	strcpy(m_set->m_description, LPCTSTR(m_set_description));

	CString productID = CProductItem::ExtractActualID(m_set->m_product_ID);
	CString prefix = CSettings::GetPrefixFor(m_set->m_description);

	if(prefix == "_DOES_NOT_EXIST_") //Description is not present in list
	{
		prefix = "";
		int retval = 0;
		CString warnstr = "You have chosen a description which is not already in the database. Do you want to feed it in the database and assign the appropriate ID prefix for this description ? If you chose NO, then no description specific prefix will be added for the product ID's, and you would be prompted again with this same message the next time you use this description.";
		SHOW_WARNING_YESNO(warnstr, retval);
		if(retval == IDYES)
		{
			CDescriptionAddDialog dlg;
			dlg.m_name = m_set->m_description;
			if(dlg.DoModal() == IDOK)
			{
				prefix = dlg.m_prefix;
			}
		}
	}

	productID = prefix + productID;
	if(!m_set->UpdateProductID(productID))
	{
		CString errstr = "Could not update productID of the set to '" + productID + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	int index = m_tabs.GetCurSel();
	if(index == CB_ERR)
		index = 0;

	if(!UpdateSetValues(TRUE, index))
		SHOW_INTERNAL_ERROR_REASON("Could not update set values.");

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnItemDescriptionList() 
{
	m_disable_killmessage = TRUE;

	if(!m_set || !m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CGenericListBoxDialog dlg(DescriptionList);
	if(dlg.DoModal() == IDCANCEL)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	if(m_item_description == dlg.m_selection)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_item_description = dlg.m_selection;
	strcpy(m_item->m_description, LPCTSTR(m_item_description));

	CString productID = CProductItem::ExtractActualID(m_set->m_product_ID);
	CString set_prefix = CProductItem::ExtractPrefix(m_set->m_product_ID);
	CString prefix = CSettings::GetPrefixFor(m_item->m_description);
	int value = CSettings::GetPieceCountFor(m_item->m_description);

	if(prefix == "_DOES_NOT_EXIST_")
	{
		prefix = "";
		int retval = 0;
		CString warnstr = "You have chosen a description which is not already in the database. Do you want to feed it in the database and assign the appropriate ID prefix for this description ? If you chose NO, then no description specific prefix will be added for the product ID's, and you would be prompted again with this same message the next time you use this description.";
		SHOW_WARNING_YESNO(warnstr, retval);
		if(retval == IDYES)
		{
			CDescriptionAddDialog dlg;
			dlg.m_name = m_item->m_description;
			if(dlg.DoModal() == IDOK)
			{
				prefix = dlg.m_prefix;
				value = dlg.m_number_of_pieces;
			}
		}
	}

	if(prefix != set_prefix)
		prefix = prefix + set_prefix;

	m_item->m_piece_count = value;
	productID = prefix + productID;
	if(!m_item->UpdateProductID(productID))
	{
		CString errstr = "Could not update productID of item '" + CString(m_item->m_product_ID) + "' to '" + productID + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	int index = m_tabs.GetCurSel();
	if(index == CB_ERR)
		index = 0;
	
	if(!UpdateSetValues(TRUE, index))
		SHOW_INTERNAL_ERROR_REASON("Could not update set values.");

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnItemAdd() 
{
	m_disable_killmessage = TRUE;

	if(!m_set)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CProductItem emptyitem;

	if(m_type == tExport)
	{
		emptyitem.m_serial_number = m_desired_serial;
		m_desired_serial++;
	}

	if(!m_set->AddItem(&emptyitem, TRUE))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not add empty item to set.");
		m_disable_killmessage = FALSE;
		return;
	}

	int index = m_set->GetItemCount() - 1; //subtract to make it a 0 based index.
	if(!UpdateSetValues(TRUE, index))
		SHOW_INTERNAL_ERROR_REASON("Could not update set values.");

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnItemDelete() 
{
	m_disable_killmessage = TRUE;

	if(!m_set)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	int index = m_tabs.GetCurSel();
	if(index == -1)
	{
		SHOW_ERROR("Please select a tab for deleting the corresponding item.");
		m_disable_killmessage = FALSE;
		return;
	}

	int retval = 0;
	SHOW_WARNING_YESNO("Are you sure you want to delete the currently open item?", retval);
	if(retval == IDNO)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	if(!m_set->DeleteItemAt(index))
	{
		CString errstr;
		errstr.Format("%d", index);
		errstr = "Could not delete item at position '" + errstr + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		m_disable_killmessage = FALSE;
		return;
	}

	if(m_set->GetItemCount() == 0)
	{
		CProductItem emptyitem;

		if(m_type == tExport)
		{
			emptyitem.m_serial_number = m_desired_serial;
			m_desired_serial++;
		}

		if(!m_set->AddItem(&emptyitem, TRUE))
			SHOW_INTERNAL_ERROR_REASON("Could not add empty item to set.");
	}

	if(!UpdateSetValues(TRUE, 0))
		SHOW_INTERNAL_ERROR_REASON("Could not update set values.");

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnSetSave() 
{
	m_disable_killmessage = TRUE;

	if(!m_set)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	int pos = 0;
	if(m_set->HasEmptyItems(&pos))
	{
		SHOW_ERROR("Cannot save a set which has empty items. Please remove the empty items to proceed.");

		if(pos != -1)
		{
			if(!GotoTab(pos, TRUE))
			{
				CString errstr;
				errstr.Format("%d", pos);
				errstr = "Could not select tab number '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}

		m_disable_killmessage = FALSE;
		return;
	}

	if(m_original_set == (*m_set))
		CDialog::OnCancel();
	else
		CDialog::OnOK();
}


void CAddDialog::OnSetCancel() 
{
	m_disable_killmessage = TRUE;
	CDialog::OnCancel();
}


void CAddDialog::OnItemChangeImage() 
{
	m_disable_killmessage = TRUE;
	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CString filename = GetFileName("All Files (*.*)", "*.*", NULL, TRUE);
	if(filename.IsEmpty())
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CFile imageFile;
	CFileException e;
	if(!imageFile.Open(LPCTSTR(filename), CFile::modeRead | CFile::typeBinary, &e))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", e.m_cause);
		errstr = "Could not open image file '" + filename + "' for associating with item '" + CString(m_item->m_product_ID) + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		m_disable_killmessage = FALSE;
		return;
	}

	BYTE* pBuff = NULL;
	DWORD size = (DWORD)imageFile.GetLength();
	SALIL_NEW(pBuff, BYTE[size]);
	DWORD readcount = 0;

	try
	{
		readcount = imageFile.Read(pBuff, size);
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", readerror->m_cause);
		errstr = "Could not read image file '" + filename + "' for associating with item '" + CString(m_item->m_product_ID) + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		imageFile.Close();
		m_disable_killmessage = FALSE;
		return;
	}

	imageFile.Close();

	if(readcount < size)
	{
		CString errstr = "Could not read entire image file '" + filename + "' for associating with item '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		m_disable_killmessage = FALSE;
		return;
	}

	BOOL overwrite = TRUE;
	CString photo_filename = m_item->m_photo_filename;
	if(photo_filename.IsEmpty())
	{
		photo_filename = m_item->m_product_ID;
		overwrite = FALSE;
	}

	BOOL saved_successfully = SaveImage(&photo_filename, pBuff, size, overwrite);
	delete pBuff;

	if(saved_successfully)
	{
		strcpy(m_item->m_photo_filename, LPCTSTR(photo_filename));
		if(!UpdateItemValues(TRUE))
		{
			CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}
	else
	{
		CString errstr = "Could not save image data to file '" + photo_filename + "' for item '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnItemRemoveImage() 
{
	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CString filename = m_item->m_photo_filename;
	if(filename.IsEmpty())
	{
		m_disable_killmessage = FALSE;
		return;
	}

	if(!DeleteImage(filename))
	{
		CString errstr = "Could not delete image file '" + filename + "' of item '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}

	strcpy(m_item->m_photo_filename, "");
	if(!UpdateItemValues(TRUE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnItemSold() 
{
	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_item->m_sold = m_item_sold;

	if((m_type == tStock) || (m_type == tKundan))
		m_set_piece_count = m_set->GetTotalUnsoldItemCount();

	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnDiamondDetails() 
{
	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CGenericListBoxDialog dlg(DiamondDetail);
	dlg.m_item = m_item;
	if(dlg.DoModal() == IDCANCEL)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}

void CAddDialog::OnStoneDetails() 
{
	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CGenericListBoxDialog dlg(StoneDetail);
	dlg.m_item = m_item;
	if(dlg.DoModal() == IDCANCEL)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}

void CAddDialog::OnOldSerials() 
{
	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CGenericListBoxDialog dlg(SerialDetail);
	dlg.m_item = m_item;
	if(dlg.DoModal() == IDCANCEL)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnMarkedValues() 
{
	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CProductItem tempitem = *m_item;
	CMarkedValueFormulaDialog dlg(m_type, &tempitem, m_gold_rate);

	if(dlg.DoModal() == IDCANCEL)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	*m_item = tempitem;

	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnItemUseNetWeight() 
{
	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_item->m_use_net_weight_for_gold = m_use_net_weight;
	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusSetDescription() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_set)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CString current = m_set->m_description;
	if(current == m_set_description)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	strcpy(m_set->m_description, LPCTSTR(m_set_description));

	CString productID = CProductItem::ExtractActualID(m_set->m_product_ID);
	CString prefix = CSettings::GetPrefixFor(m_set->m_description);

	if(prefix == "_DOES_NOT_EXIST_") //Description is not present in list
	{
		prefix = "";
		int retval = 0;
		SHOW_WARNING_YESNO("You have chosen a description which is not already in the database. Do you want to feed it in the database and assign the appropriate ID prefix for this description ? If you chose NO, then no description specific prefix will be added for the product ID's, and you would be prompted again with this same message the next time you use this description.", retval);
		if(retval == IDYES)
		{
			CDescriptionAddDialog dlg;
			dlg.m_name = m_set->m_description;
			if(dlg.DoModal() == IDOK)
			{
				prefix = dlg.m_prefix;
			}
		}
	}

	productID = prefix + productID;
	if(!m_set->UpdateProductID(productID))
	{
		CString errstr = "Could not update productID of the set to '" + productID + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}
	
	int index = m_tabs.GetCurSel();
	if(!UpdateSetValues(TRUE, index))
		SHOW_INTERNAL_ERROR_REASON("Could not update set values.");

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusItemDescription() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_set || !m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CString current = m_item->m_description;
	if(current == m_item_description)
	{
		m_disable_killmessage = FALSE;
		return;
	}

	strcpy(m_item->m_description, LPCTSTR(m_item_description));

	CString productID = CProductItem::ExtractActualID(m_set->m_product_ID);
	CString set_prefix = CProductItem::ExtractPrefix(m_set->m_product_ID);
	CString prefix = CSettings::GetPrefixFor(m_item->m_description);
	int value = CSettings::GetPieceCountFor(m_item->m_description);

	if(prefix == "_DOES_NOT_EXIST_") //Description is not present in list
	{
		prefix = "";
		int retval = 0;
		CString warnstr = "You have chosen a description which is not already in the database. Do you want to feed it in the database and assign the appropriate ID prefix for this description ? If you chose NO, then no description specific prefix will be added for the product ID's, and you would be prompted again with this same message the next time you use this description.";
		SHOW_WARNING_YESNO(warnstr, retval);
		if(retval == IDYES)
		{
			CDescriptionAddDialog dlg;
			dlg.m_name = m_item->m_description;
			if(dlg.DoModal() == IDOK)
			{
				prefix = dlg.m_prefix;
				value = dlg.m_number_of_pieces;
			}
		}
	}

	if(prefix != set_prefix)
		prefix = prefix + set_prefix;

	m_item->m_piece_count = value;
	productID = prefix + productID;
	
	if(!m_item->UpdateProductID(productID))
	{
		CString errstr = "Could not update productID of item '" + CString(m_item->m_product_ID) + "' to '" + productID + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	int index = m_tabs.GetCurSel();
	if(!UpdateSetValues(TRUE, index))
		SHOW_INTERNAL_ERROR_REASON("Could not update set values.");

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusItemPieceCount()
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_item->m_piece_count= m_item_piece_count;
	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusGrossWeight() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_item->m_gross_weight= RoundOff(m_gross_weight, 3);
	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusLabourCharge() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_item->m_labour_cost= m_labour_charge;
	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusOtherWeight() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_item->m_other_weight= RoundOff(m_other_weight, 3);
	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusPearlRate() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	if(m_item->GetSubItemCount(tPearl) > 0)
	{
		if((m_pearl_weight == 0.0) &&
		   (m_pearl_rate == 0))
		{
			if(!m_item->DeleteSubItemAt(tPearl, 0))
			{
				CString errstr = "Could not delete the first pearl subitem entry of item '" + CString(m_item->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}
		else
		{
			CProductSubItem *node = m_item->GetSubItemAt(tPearl, 0);
			if(node)
				node->m_rate = m_pearl_rate;
			else
			{
				CString errstr = "Could not get the first pearl subitem in the item '" + CString(m_item->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}
	}
	else if(m_pearl_rate != 0)
	{
		CProductSubItem newnode;
		newnode.m_rate = m_pearl_rate;
		strcpy(newnode.m_type, "P");
		if(!m_item->AddSubItem(tPearl, &newnode))
		{
			CString errstr = "Could not add pearl subitem to the item '" + CString(m_item->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}

	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusPearlWeight() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	if(m_item->GetSubItemCount(tPearl) > 0)
	{
		if((m_pearl_weight == 0.0) &&
		   (m_pearl_rate == 0))
		{
			if(!m_item->DeleteSubItemAt(tPearl, 0))
			{
				CString errstr = "Could not delete the first pearl subitem of the item '" + CString(m_item->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}
		else
		{
			CProductSubItem *node = m_item->GetSubItemAt(tPearl, 0);
			if(node)
				node->m_weight = RoundOff(m_pearl_weight, 2);
			else
			{
				CString errstr = "Could not get the first pearl subitem in the item '" + CString(m_item->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}
	}
	else if(m_pearl_weight != 0.0)
	{
		CProductSubItem newnode;
		newnode.m_weight = RoundOff(m_pearl_weight, 2);
		strcpy(newnode.m_type, "P");
		if(!m_item->AddSubItem(tPearl, &newnode))
		{
			CString errstr = "Could not add pearl subitem to the item '" + CString(m_item->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}

	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusSoldCountry() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	strcpy(m_item->m_sold_country, LPCTSTR(m_sold_country));
	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusSerialNumber() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_item->m_serial_number = m_serial_number;
	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusCosting() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_item->m_costing = m_costing;
	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnKillfocusDiamondClarity() 
{
	if(m_disable_killmessage)
		return;

	m_disable_killmessage = TRUE;

	if(!m_item)
	{
		SHOW_INTERNAL_ERROR;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	strcpy(m_item->m_diamond_clarity, LPCTSTR(m_diamond_clarity));
	if(!UpdateItemValues(FALSE))
	{
		CString errstr = "Could not update item values of '" + CString(m_item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	UpdateData(FALSE);
	m_disable_killmessage = FALSE;
}


void CAddDialog::OnEditchangeSetDescription() 
{
	m_disable_killmessage = TRUE;

	static BOOL fromClipboard = FALSE;

	if(fromClipboard)
	{
		fromClipboard = FALSE;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_SET_DESCRIPTION);
	if(!cb)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find set description control");
		m_disable_killmessage = FALSE;
		return;
	}

	int count = cb->GetCount();
	while(count > 0)
	{
		if(cb->DeleteString(count - 1) == CB_ERR)
			SHOW_INTERNAL_ERROR;

		count--;
	}

	int index = 0;
	while(index < CSettings::GetCategoryCount())
	{
		CString desc;
		if(CSettings::GetCategory(index, &desc, NULL, NULL))
		{
			CString set_description = m_set_description;
			if(desc.GetLength() > set_description.GetLength())
			{
				CString subsection = desc.Left(set_description.GetLength());
				if(subsection.CompareNoCase(set_description) == 0)
					cb->AddString(desc);
			}
			else
			{
				CString subsection = set_description.Left(desc.GetLength());
				if(subsection.CompareNoCase(desc) == 0)
					cb->AddString(desc);
			}
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get category number '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		index++;
	}

	cb->ShowDropDown();
	UpdateData(FALSE);
	if(cb->SetEditSel(0, -1))
	{
		fromClipboard = TRUE;
		cb->Copy();
		cb->Paste();
	}

	m_disable_killmessage = FALSE;
}


void CAddDialog::OnEditchangeItemDescription() 
{
	m_disable_killmessage = TRUE;

	static BOOL fromClipboard = FALSE;

	if(fromClipboard)
	{
		fromClipboard = FALSE;
		m_disable_killmessage = FALSE;
		return;
	}

	if(!UpdateData(TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_ITEM_DESCRIPTION);
	if(!cb)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find item description control");
		m_disable_killmessage = FALSE;
		return;
	}

	int count = cb->GetCount();
	while(count > 0)
	{
		if(cb->DeleteString(count - 1) == CB_ERR)
			SHOW_INTERNAL_ERROR;

		count--;
	}

	int index = 0;
	while(index < CSettings::GetCategoryCount())
	{
		CString desc;
		if(CSettings::GetCategory(index, &desc, NULL, NULL))
		{
			CString item_description = m_item_description;
			if(desc.GetLength() > item_description.GetLength())
			{
				CString subsection = desc.Left(item_description.GetLength());
				if(subsection.CompareNoCase(item_description) == 0)
					cb->AddString(desc);
			}
			else
			{
				CString subsection = item_description.Left(desc.GetLength());
				if(subsection.CompareNoCase(desc) == 0)
					cb->AddString(desc);
			}
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get category number '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		index++;
	}

	cb->ShowDropDown();
	UpdateData(FALSE);
	if(cb->SetEditSel(0, -1))
	{
		fromClipboard = TRUE;
		cb->Copy();
		cb->Paste();
	}

	m_disable_killmessage = FALSE;
}

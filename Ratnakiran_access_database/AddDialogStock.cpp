#include "stdafx.h"
#include "RatnaKiran.h"
#include "AddDialogStock.h"
#include "MarkedValueFormulaDialog.h"
#include "GenericListBoxDialog.h"
#include "DescriptionAddDialog.h"
#include "Picture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CAddDialogStock::ShowMessage(CString mesg, CString caption, int type)
{
	m_disable_killmessage = TRUE;
	int retval = MessageBox(mesg, caption, type);
	m_disable_killmessage = FALSE;
	return retval;
}


CAddDialogStock::CAddDialogStock(CWnd* pParent /*=NULL*/)
	: CDialog(CAddDialogStock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddDialogStock)
	m_set_description = _T("");
	m_set_productID = _T("---- Automatic ----");
	m_set_piece_count = 0;
	m_item_description = _T("");
	m_item_productID = _T("---- Automatic ----");
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
	m_stone_weight = 0.0;
	m_stone_rate = 0;
	m_stone_value = 0;
	m_stone_marked_rate = 0;
	m_stone_marked_value = 0;
	m_pearl_weight = 0.0;
	m_pearl_rate = 0;
	m_pearl_value = 0;
	m_pearl_marked_rate = 0;
	m_pearl_marked_value = 0;
	m_labour_charge = 0;
	m_marked_labour_charge = 0;
	m_other_weight = 0.0;
	m_total_price = 0;
	m_total_marked_price = 0;
	m_item_sold = FALSE;
	m_sold_country = _T("");
	m_use_net_weight = FALSE;
	//}}AFX_DATA_INIT

	m_disable_killmessage = FALSE;
}


void CAddDialogStock::UpdateSetValues(BOOL redraw)
{
	ClearSet();
	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_SET_DESCRIPTION);
	if(cb)
	{
		cb->ResetContent();

		DatabaseState state = gDesc.GotoFirstEntry();

		if(state == DBInvalid)
			return;

		while(state == DBOK)
		{
			cb->AddString(gDesc.m_Description);
			state = gDesc.GotoNextEntry();
		}

		if(!gDesc.IsEOF())
			return;
	}

	m_set_productID = m_set->m_record.m_Set_ID;
	if(m_set_productID.IsEmpty())
		m_set_productID = _T("---- Automatic ----");

	m_set_description = m_set->m_record.m_Description;
	m_set_piece_count = m_set->GetTotalItemCount(UnSold);

	UpdateTabs();
	m_tabs.SetCurSel(0);

	if(m_item->GotoFirstItem() != DBOK)
		return;

	UpdateItemValues(redraw);
}


void CAddDialogStock::UpdateItemValues(BOOL redraw)
{
	ClearItem();
	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_ITEM_DESCRIPTION);
	if(cb)
	{
		cb->ResetContent();

		DatabaseState state = gDesc.GotoFirstEntry();

		if(state == DBInvalid)
			return;

		while(state == DBOK)
		{
			cb->AddString(gDesc.m_Description);
			state = gDesc.GotoNextEntry();
		}

		if(!gDesc.IsEOF())
			return;
	}

	m_item_productID = m_item->m_record.m_Item_ID;
	if(m_item_productID.IsEmpty())
		m_item_productID = _T("---- Automatic ----");

	m_item_description = m_item->m_record.m_Description;
	m_item_photo = m_item->m_record.m_Photo_filename;
	m_item_piece_count = m_item->m_record.m_Piece_count;

	m_gross_weight = m_item->GetGrossWeight();
	m_net_weight = m_item->GetNetWeight();
	m_gold_value = ROUNDUP(m_item->GetGoldValueDouble(m_item->m_record.m_Use_net_weight_for_gold, TRUE, gDoc->m_vouchers.GetGoldRate(), 1.0));

	m_diamond_weight = m_item->GetSubItemWeight(DIAMOND);
	m_diamond_rate = ROUNDUP(m_item->GetSubItemRate(DIAMOND, 1.0));
	m_diamond_value = ROUNDUP(m_item->GetSubItemValueDouble(DIAMOND, 1.0));
	m_diamond_marked_rate = ROUNDUP(m_item->GetMarkedSubItemRate(DIAMOND, gDoc->m_vouchers.GetGoldRate(), 1.0));
	m_diamond_marked_value = ROUNDUP(m_item->GetMarkedSubItemValueDouble(DIAMOND, gDoc->m_vouchers.GetGoldRate(), 1.0));

	m_stone_weight = m_item->GetSubItemWeight(STONE);
	m_stone_rate = ROUNDUP(m_item->GetSubItemRate(STONE, 1.0));
	m_stone_value = ROUNDUP(m_item->GetSubItemValueDouble(STONE, 1.0));
	m_stone_marked_rate = ROUNDUP(m_item->GetMarkedSubItemRate(STONE, gDoc->m_vouchers.GetGoldRate(), 1.0));
	m_stone_marked_value = ROUNDUP(m_item->GetMarkedSubItemValueDouble(STONE, gDoc->m_vouchers.GetGoldRate(), 1.0));

	m_pearl_weight = m_item->GetSubItemWeight(PEARL);
	m_pearl_rate = ROUNDUP(m_item->GetSubItemRate(PEARL, 1.0));
	m_pearl_value = ROUNDUP(m_item->GetSubItemValueDouble(PEARL, 1.0));
	m_pearl_marked_rate = ROUNDUP(m_item->GetMarkedSubItemRate(PEARL, gDoc->m_vouchers.GetGoldRate(), 1.0));
	m_pearl_marked_value = ROUNDUP(m_item->GetMarkedSubItemValueDouble(PEARL, gDoc->m_vouchers.GetGoldRate(), 1.0));

	m_labour_charge = ROUNDUP(m_item->GetLabourCostDouble(1.0));
	m_marked_labour_charge = ROUNDUP(m_item->GetMarkedLabourCostDouble(gDoc->m_vouchers.GetGoldRate(), 1.0));

	m_other_weight = m_item->GetOtherWeight();
	m_total_price = ROUNDUP(m_item->GetTotalValueDouble(m_item->m_record.m_Use_net_weight_for_gold, TRUE, gDoc->m_vouchers.GetGoldRate(), 1.0));
	m_total_marked_price = ROUNDUP(m_item->GetTotalMarkedValueDouble(m_item->m_record.m_Use_net_weight_for_gold, TRUE, gDoc->m_vouchers.GetGoldRate(), 1.0));

	m_use_net_weight = m_item->m_record.m_Use_net_weight_for_gold;

	m_item_sold = m_item->m_record.m_Sold;
	m_sold_country = m_item->m_record.m_Sold_in_country;

	if(redraw)
		DrawImage(m_item_photo);
}


void CAddDialogStock::DrawImage(CString filename)
{
	//Get Window details
	if(!IsWindow(m_image_frame.m_hWnd))
		return;

	CClientDC dc(&m_image_frame);
	int mode = dc.GetStretchBltMode();
	//dc.SetStretchBltMode(COLORONCOLOR);
	dc.SetStretchBltMode(HALFTONE);

	CRect rect;
	m_image_frame.GetClientRect(rect);
	CSize window_size(rect.Width(), rect.Height());

	//Empty the pane
	CBitmap empty_img;
	empty_img.LoadBitmap(IDB_BLACK_IMAGE);

	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(NULL);
		dcMem.SelectObject(&empty_img);
		dc.StretchBlt(0, 0, window_size.cx, window_size.cy, &dcMem , 0, 0, 10, 10, SRCCOPY);
	}
	
	if(m_item->m_record.m_Photo.m_hData && m_item->m_record.m_Photo.m_dwDataLength != 0)
	{
		CRecordPicture pic;
		if(!pic.LoadFromBuffer((unsigned char *)m_item->m_record.m_Photo.m_hData, m_item->m_record.m_Photo.m_dwDataLength))
		{
			ShowMessage("Could not load the image from the buffer.", "ERROR");
			return;
		}

		CSize original_size = pic.GetSize(&dc);
		CSize final_size;
		CPoint position = ScaleImage(original_size, window_size, &final_size);
		if(!pic.Draw(&dc, position, final_size))
		{
			ShowMessage("Could not draw the image.", "ERROR");
			return;
		}
	}
}


CPoint CAddDialogStock::ScaleImage(CSize original, CSize window, CSize *final)
{
	CPoint pos(0,0);

	if(!final)
		return pos;

	double dx = (double)original.cx/window.cx;
	double dy = (double)original.cy/window.cy;

	if((dx <= 1.0) && (dy <= 1.0))
	{
		*final = original;
		
		pos.x = (window.cx - original.cx)/2;
		pos.y = (window.cy - original.cy)/2;
	}
	else if(dx >= dy)
	{
		final->cx = window.cx;
		final->cy = (long)(original.cy / dx);

		pos.x = 0;
		pos.y = (window.cy - final->cy)/2;
	}
	else
	{
		final->cy = window.cy;
		final->cx = (long)(original.cx / dy);

		pos.y = 0;
		pos.x = (window.cx - final->cx)/2;
	}

	return pos;
}


void CAddDialogStock::UpdateTabs()
{
	m_tabs.DeleteAllItems();

	int counter = 0;

	DatabaseState state = m_item->GotoFirstItem();

	if(state == DBInvalid)
		return;

	while(state == DBOK)
	{
		CString str = m_item->m_record.m_Description;

		if(str.IsEmpty())
			str = "Untitled";

		m_tabs.InsertItem(counter, str);

		TCITEM tcItem;
		tcItem.mask = TCIF_PARAM;
		tcItem.lParam = m_item->m_record.m_Identifier;

		m_tabs.SetItem(counter, &tcItem);
		counter++;
		state = m_item->GotoNextItem();
	}
}

void CAddDialogStock::ClearSet()
{
	m_set_description = _T("");
	m_set_productID = _T("---- Automatic ----");
	m_set_piece_count = 0;
}

void CAddDialogStock::ClearItem()
{
	m_item_description = _T("");
	m_item_productID = _T("---- Automatic ----");
	m_item_photo = _T("");
	m_item_piece_count = 0;
	m_gross_weight = 0.0;
	m_net_weight = 0.0;
	m_gold_value = 0;
	m_diamond_rate = 0;
	m_diamond_weight = 0.0;
	m_diamond_value = 0;
	m_diamond_marked_value = 0;
	m_stone_weight = 0.0;
	m_stone_rate = 0;
	m_stone_value = 0;
	m_stone_marked_value = 0;
	m_pearl_weight = 0.0;
	m_pearl_rate = 0;
	m_pearl_value = 0;
	m_pearl_marked_value = 0;
	m_labour_charge = 0;
	m_marked_labour_charge = 0;
	m_other_weight = 0.0;
	m_total_price = 0;
	m_use_net_weight = FALSE;
	m_item_sold = FALSE;
	m_sold_country = _T("");
}


void CAddDialogStock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddDialogStock)
	DDX_Control(pDX, IDC_ITEM_TABS, m_tabs);
	DDX_Control(pDX, IDC_IMAGE_FRAME, m_image_frame);
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
	DDX_Text(pDX, IDC_STONE_WEIGHT, m_stone_weight);
	DDX_Text(pDX, IDC_STONE_RATE, m_stone_rate);
	DDX_Text(pDX, IDC_STONE_VALUE, m_stone_value);
	DDX_Text(pDX, IDC_STONE_MARKED_RATE, m_stone_marked_rate);
	DDX_Text(pDX, IDC_STONE_MARKED_VALUE, m_stone_marked_value);
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
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddDialogStock, CDialog)
	//{{AFX_MSG_MAP(CAddDialogStock)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ITEM_TABS, OnSelchangeItemTabs)
	ON_NOTIFY(TCN_SELCHANGING, IDC_ITEM_TABS, OnSelchangingItemTabs)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_ITEM_ADD, OnItemAdd)
	ON_BN_CLICKED(ID_ITEM_DELETE, OnItemDelete)
	ON_BN_CLICKED(ID_SET_DESCRIPTION_LIST, OnSetDescriptionList)
	ON_BN_CLICKED(ID_ITEM_DESCRIPTION_LIST, OnItemDescriptionList)
	ON_BN_CLICKED(ID_ITEM_CHANGE_IMAGE, OnItemChangeImage)
	ON_BN_CLICKED(ID_ITEM_REMOVE_IMAGE, OnItemRemoveImage)
	ON_BN_CLICKED(ID_ITEM_DUMP_IMAGE, OnItemDumpImage)
	ON_BN_CLICKED(IDC_ITEM_SOLD, OnItemSold)
	ON_BN_CLICKED(IDC_ITEM_USE_NET_WEIGHT, OnItemUseNetWeight)
	ON_BN_CLICKED(ID_DIAMOND_DETAILS, OnDiamondDetails)
	ON_BN_CLICKED(ID_STONE_DETAILS, OnStoneDetails)
	ON_BN_CLICKED(ID_MARKED_VALUES, OnMarkedValues)
	ON_BN_CLICKED(ID_SET_SAVE, OnSetSave)
	ON_BN_CLICKED(ID_SET_CANCEL, OnSetCancel)
	ON_CBN_KILLFOCUS(IDC_SET_DESCRIPTION, OnKillfocusSetDescription)
	ON_CBN_KILLFOCUS(IDC_ITEM_DESCRIPTION, OnKillfocusItemDescription)
	ON_EN_KILLFOCUS(IDC_ITEM_PIECE_COUNT, OnKillfocusItemPieceCount)
	ON_EN_KILLFOCUS(IDC_GROSS_WEIGHT, OnKillfocusGrossWeight)
	ON_EN_KILLFOCUS(IDC_LABOUR_CHARGE, OnKillfocusLabourCharge)
	ON_EN_KILLFOCUS(IDC_OTHER_WEIGHT, OnKillfocusOtherWeight)
	ON_EN_KILLFOCUS(IDC_PEARL_RATE, OnKillfocusPearlRate)
	ON_EN_KILLFOCUS(IDC_PEARL_WEIGHT, OnKillfocusPearlWeight)
	ON_EN_KILLFOCUS(IDC_SOLD_COUNTRY, OnKillfocusSoldCountry)
	ON_BN_CLICKED(ID_OLD_SERIALS, OnOldSerials)
	ON_CBN_EDITCHANGE(IDC_SET_DESCRIPTION, OnEditchangeSetDescription)
	ON_CBN_EDITCHANGE(IDC_ITEM_DESCRIPTION, OnEditchangeItemDescription)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CAddDialogStock::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(!MakeGlobalsValid())
	{
		UnHandledError(__FILE__, __LINE__);
		CDialog::OnCancel();
		return FALSE;
	}

	m_set = &(gDoc->m_vouchers.m_sets);
	m_item = &(m_set->m_items);

	UpdateSetValues(TRUE);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddDialogStock::OnSelchangeItemTabs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	int index = m_tabs.GetCurSel();
	if(index == -1)
		return;

	TCITEM tcItem;
	tcItem.mask = TCIF_PARAM;

	if(!m_tabs.GetItem(index, &tcItem))
		return;

	if(m_item->GotoItem(tcItem.lParam, FALSE) != DBOK)
		return;

	UpdateItemValues(TRUE);
	UpdateData(FALSE);
}

void CAddDialogStock::OnSelchangingItemTabs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!UpdateData(TRUE))
	{
		*pResult = TRUE;
	}
	else
	{
		*pResult = FALSE;
	}
}


void CAddDialogStock::OnPaint() 
{
	CPaintDC dc(this);
	DrawImage(m_item_photo);
}


void CAddDialogStock::OnSetDescriptionList() 
{
	if(!UpdateData(TRUE))
		return;

	CGenericListBoxDialog dlg(DESCRIPTION_LIST_DIALOG);
	if(dlg.DoModal() == IDCANCEL)
		return;

	if(!m_set->DBEditEntry())
		return;
	
	m_set->m_record.m_Description = dlg.m_selection;

	if(!m_set->DBUpdateEntry("OnSetDescriptionList"))
		return;

	m_disable_killmessage = TRUE;
	if(!m_set->UpdateProductID(m_set->m_record.m_Set_ID, TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_disable_killmessage = FALSE;
	m_set_productID = m_set->m_record.m_Set_ID;
	m_set_description = m_set->m_record.m_Description;

	UpdateItemValues(TRUE);
	UpdateData(FALSE);
}


void CAddDialogStock::OnItemDescriptionList() 
{
	if(!UpdateData(TRUE))
		return;

	CGenericListBoxDialog dlg(DESCRIPTION_LIST_DIALOG);
	if(dlg.DoModal() == IDCANCEL)
		return;

	if(!m_item->DBEditEntry())
		return;

	m_item->m_record.m_Description = dlg.m_selection;

	if(!m_item->DBUpdateEntry("OnItemDescriptionList"))
		return;

	m_disable_killmessage = TRUE;
	if(!m_item->UpdateProductID(m_set->m_record.m_Set_ID, TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_disable_killmessage = FALSE;
	m_item_productID = m_item->m_record.m_Item_ID;
	m_item_description = m_item->m_record.m_Description;
	m_item_piece_count = m_item->m_record.m_Piece_count;

	int index = m_tabs.GetCurSel();
	UpdateTabs();
	m_tabs.SetCurSel(index);

	UpdateItemValues(TRUE);
	m_set_piece_count = m_set->GetTotalItemCount(UnSold);
	UpdateData(FALSE);
}


void CAddDialogStock::OnItemAdd() 
{
	if(!UpdateData(TRUE))
		return;

	CRecordItem emptyitem;
	if(!m_set->AddItem(emptyitem, TRUE))
	{
		m_disable_killmessage = TRUE;
		UnHandledError(__FILE__, __LINE__);
		m_disable_killmessage = FALSE;
		return;
	}

	int index = m_set->GetItemCount(Both, FALSE) - 1; //subtract to make it a 0 based index.

	UpdateTabs();
	m_tabs.SetCurSel(index);

	if(m_item->GotoPosItem(index) != DBOK)
		return;

	UpdateItemValues(TRUE);
	m_set_piece_count = m_set->GetTotalItemCount(UnSold);
	UpdateData(FALSE);
}


void CAddDialogStock::OnItemDelete() 
{
	if(!UpdateData(TRUE))
		return;

	int index = m_tabs.GetCurSel();
	if(index == -1)
		return;

	TCITEM tcItem;
	tcItem.mask = TCIF_PARAM;

	if(!m_tabs.GetItem(index, &tcItem))
		return;

	int retval = ShowMessage("Are you sure you want to delete the currently open item?", "Warning", MB_YESNO | MB_ICONWARNING);
	if(retval == IDNO)
		return;

	if(!m_set->DeleteItem(tcItem.lParam))
	{
		ShowMessage("Could not delete the currently open item.", "ERROR");
		return;
	}

	if(m_set->GetItemCount(Both, FALSE) == 0)
	{
		CRecordItem emptyitem;
		if(!m_set->AddItem(emptyitem, TRUE))
			return;
	}

	UpdateTabs();
	m_tabs.SetCurSel(0);

	if(m_item->GotoFirstItem() != DBOK)
		return;

	UpdateItemValues(TRUE);
	m_set_piece_count = m_set->GetTotalItemCount(UnSold);
	UpdateData(FALSE);
}


void CAddDialogStock::OnSetSave() 
{
	if(m_set->HasEmptyItems())
	{
		ShowMessage("Cannot save a set which has empty items. Please remove the empty items to proceed.", "Error");
		return;
	}

	CDialog::OnOK();
}


void CAddDialogStock::OnSetCancel() 
{
	m_disable_killmessage = TRUE;
	CDialog::OnCancel();
}


void CAddDialogStock::OnItemChangeImage() 
{
	if(!UpdateData(TRUE))
		return;

	//CString filename = GetFileName("Bitmap Files (*.bmp)", "*.bmp", NULL, TRUE);
	CString filename = GetFileName("All Files (*.*)", "*.*", NULL, TRUE);

	if(filename.IsEmpty())
		return;

	CFile imageFile;
	CFileException e;
	BYTE* pBuff = NULL;
	DWORD size = 0;
	if(imageFile.Open(LPCTSTR(filename), CFile::modeRead | CFile::typeBinary, &e))
	{
		BYTE* pBuff = NULL;
		SALIL_NEW(pBuff, BYTE[(DWORD)imageFile.GetLength()]);

		if(!(imageFile.Read(pBuff, (DWORD)imageFile.GetLength()) > 0))
			return;
		else
			size = (DWORD)imageFile.GetLength();
	}
	else
	{
		ShowMessage("Could not open image file. Unable to set image correctly.", "ERROR");
		return;
	}

	imageFile.Close();

	if(!m_item->DBEditEntry())
		return;

	m_item->m_record.m_Photo_filename = filename;

	if(!m_item->DBUpdateEntry("OnItemChangeImage"))
		return;

	m_item->AddImage(size, pBuff);
	delete pBuff;

	UpdateItemValues(TRUE);
	UpdateData(FALSE);
}

void CAddDialogStock::OnItemRemoveImage() 
{
	if(!UpdateData(TRUE))
		return;

	if(!m_item->DBEditEntry())
		return;

	m_item->m_record.m_Photo_filename = "";

	if(!m_item->DBUpdateEntry("OnItemRemoveImage"))
		return;

	m_item->DeleteImage();

	UpdateItemValues(TRUE);
	UpdateData(FALSE);
}

void CAddDialogStock::OnItemDumpImage() 
{
	if(!UpdateData(TRUE))
		return;

	if(!m_item->m_record.m_Photo.m_hData)
		return;

	CString filename = GetFileName("All Files (*.*)", "*.*", NULL, FALSE);
	if(filename.IsEmpty())
		return;

	CFile imageFile;
	CFileException e;
	if(imageFile.Open(LPCTSTR(filename), CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary, &e))
	{
		if(m_item->m_record.m_Photo.m_dwDataLength >= 0)
			imageFile.Write(m_item->m_record.m_Photo.m_hData, m_item->m_record.m_Photo.m_dwDataLength);
	}
	else
	{
		ShowMessage("Could not open the new file for writing the image contents.", "ERROR");
	}

	imageFile.Close();
	UpdateData(FALSE);
}

void CAddDialogStock::OnDiamondDetails() 
{
	if(!UpdateData(TRUE))
		return;

	CGenericListBoxDialog dlg(DIAMOND_DETAIL_DIALOG);
	if(dlg.DoModal() == IDCANCEL)
		return;

	UpdateItemValues(TRUE);
	UpdateData(FALSE);
}

void CAddDialogStock::OnStoneDetails() 
{
	if(!UpdateData(TRUE))
		return;

	CGenericListBoxDialog dlg(STONE_DETAIL_DIALOG);
	if(dlg.DoModal() == IDCANCEL)
		return;

	UpdateItemValues(TRUE);
	UpdateData(FALSE);
}

void CAddDialogStock::OnOldSerials() 
{
	if(!UpdateData(TRUE))
		return;

	CGenericListBoxDialog dlg(SERIAL_DETAIL_DIALOG);
	if(dlg.DoModal() == IDCANCEL)
		return;

	UpdateItemValues(TRUE);
	UpdateData(FALSE);
}

void CAddDialogStock::OnMarkedValues() 
{
	if(!UpdateData(TRUE))
		return;

	CMarkedValueFormulaDialog dlg;
	if(dlg.DoModal() == IDCANCEL)
		return;

	UpdateItemValues(TRUE);
	UpdateData(FALSE);
}

void CAddDialogStock::OnKillfocusSetDescription() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_set->m_record.m_Description == m_set_description)
		return;

	if(m_disable_killmessage)
		return;

	if(!m_set->DBEditEntry())
		return;

	m_set->m_record.m_Description = m_set_description;

	if(!m_set->DBUpdateEntry("OnKillfocusSetDescription"))
		return;

	m_disable_killmessage = TRUE;
	if(!m_set->UpdateProductID(m_set->m_record.m_Set_ID, TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_disable_killmessage = FALSE;
	m_set_productID = m_set->m_record.m_Set_ID;
	m_set_description = m_set->m_record.m_Description;

	UpdateItemValues(FALSE);
	UpdateData(FALSE);
}


void CAddDialogStock::OnKillfocusItemDescription() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_item->m_record.m_Description == m_item_description)
		return;

	if(m_disable_killmessage)
		return;

	if(!m_item->DBEditEntry())
		return;

	m_item->m_record.m_Description = m_item_description;

	if(!m_item->DBUpdateEntry("OnKillfocusItemDescription"))
		return;

	m_disable_killmessage = TRUE;
	if(!m_item->UpdateProductID(m_set->m_record.m_Set_ID, TRUE))
	{
		m_disable_killmessage = FALSE;
		return;
	}

	m_disable_killmessage = FALSE;
	m_item_productID = m_item->m_record.m_Item_ID;
	m_item_description = m_item->m_record.m_Description;
	m_item_piece_count = m_item->m_record.m_Piece_count;

	int index = m_tabs.GetCurSel();
	UpdateTabs();
	m_tabs.SetCurSel(index);

	UpdateItemValues(FALSE);
	m_set_piece_count = m_set->GetTotalItemCount(UnSold);
	UpdateData(FALSE);
}


void CAddDialogStock::OnKillfocusItemPieceCount()
{
	if(!UpdateData(TRUE))
		return;

	if(m_item->m_record.m_Piece_count == m_item_piece_count)
		return;
	
	if(!m_item->DBEditEntry())
		return;

	m_item->m_record.m_Piece_count = m_item_piece_count;

	if(!m_item->DBUpdateEntry("OnKillfocusItemPieceCount"))
		return;

	UpdateItemValues(FALSE);
	m_set_piece_count = m_set->GetTotalItemCount(UnSold);
	UpdateData(FALSE);
}


void CAddDialogStock::OnKillfocusGrossWeight() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_item->m_record.m_Gross_weight == RoundOff(m_gross_weight, 3))
		return;

	if(!m_item->DBEditEntry())
		return;

	m_item->m_record.m_Gross_weight = RoundOff(m_gross_weight, 3);

	if(!m_item->DBUpdateEntry("OnKillfocusGrossWeight"))
		return;

	UpdateItemValues(FALSE);
	UpdateData(FALSE);
}


void CAddDialogStock::OnKillfocusLabourCharge() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_item->m_record.m_Labour_cost == m_labour_charge)
		return;

	if(!m_item->DBEditEntry())
		return;

	m_item->m_record.m_Labour_cost = m_labour_charge;

	if(!m_item->DBUpdateEntry("OnKillfocusLabourCharge"))
		return;

	UpdateItemValues(FALSE);
	UpdateData(FALSE);
}


void CAddDialogStock::OnKillfocusOtherWeight() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_item->m_record.m_Other_weight == RoundOff(m_other_weight, 3))
		return;

	if(!m_item->DBEditEntry())
		return;

	m_item->m_record.m_Other_weight = RoundOff(m_other_weight, 3);

	if(!m_item->DBUpdateEntry("OnKillfocusOtherWeight"))
		return;

	UpdateItemValues(FALSE);
	UpdateData(FALSE);
}


void CAddDialogStock::OnKillfocusPearlRate() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_item->GetSubItemRate(PEARL, 1.0) == m_pearl_rate)
		return;

	if(!m_item->DeleteSubItemList(PEARL))
		return;
	
	CRecordSubItem new_sub_item;
	new_sub_item.m_Rate = m_pearl_rate;
	new_sub_item.m_Weight = RoundOff(m_pearl_weight, 2);
	new_sub_item.m_Type = PEARL;

	if(!m_item->AddSubItem(PEARL, new_sub_item))
		return;
	
	UpdateItemValues(FALSE);
	UpdateData(FALSE);
}


void CAddDialogStock::OnKillfocusPearlWeight() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_item->GetSubItemWeight(PEARL) == RoundOff(m_pearl_weight, 2))
		return;

	if(!m_item->DeleteSubItemList(PEARL))
		return;
	
	CRecordSubItem new_sub_item;
	new_sub_item.m_Rate = m_pearl_rate;
	new_sub_item.m_Weight = RoundOff(m_pearl_weight, 2);
	new_sub_item.m_Type = PEARL;

	if(!m_item->AddSubItem(PEARL, new_sub_item))
		return;

	UpdateItemValues(FALSE);
	UpdateData(FALSE);
}


void CAddDialogStock::OnKillfocusSoldCountry() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_item->m_record.m_Sold_in_country == m_sold_country)
		return;

	if(!m_item->DBEditEntry())
		return;

	m_item->m_record.m_Sold_in_country == m_sold_country;

	if(!m_item->DBUpdateEntry("OnKillfocusSoldCountry"))
		return;

	UpdateItemValues(FALSE);
	UpdateData(FALSE);
}


void CAddDialogStock::OnItemSold() 
{
	if(!UpdateData(TRUE))
		return;

	if(m_item_sold)
		m_item->MarkItemAsSold(m_item_sold, m_sold_country);
	else
		m_item->MarkItemAsSold(m_item_sold, "");

	UpdateItemValues(FALSE);
	m_set_piece_count = m_set->GetTotalItemCount(UnSold);
	UpdateData(FALSE);
}

void CAddDialogStock::OnItemUseNetWeight() 
{
	if(!UpdateData(TRUE))
		return;

	if(!m_item->DBEditEntry())
		return;

	m_item->m_record.m_Use_net_weight_for_gold = m_use_net_weight;

	if(!m_item->DBUpdateEntry("OnItemUseNetWeight"))
		return;

	UpdateItemValues(FALSE);
	UpdateData(FALSE);
}

void CAddDialogStock::OnEditchangeSetDescription() 
{
	if(!UpdateData(TRUE))
		return;

	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_SET_DESCRIPTION);
	if(cb)
	{
		int count = cb->GetCount();
		while(count > 0)
		{
			cb->DeleteString(count - 1);
			count--;
		}

		DatabaseState state = gDesc.GotoFirstEntry();

		if(state == DBInvalid)
			return;

		while(state == DBOK)
		{
			if(m_set_description.GetLength() < gDesc.m_Description.GetLength())
			{
				CString subsection = gDesc.m_Description.Left(m_set_description.GetLength());
				if(subsection.CompareNoCase(m_set_description) == 0)
					cb->AddString(gDesc.m_Description);
			}
			else
			{
				CString subsection = m_set_description.Left(gDesc.m_Description.GetLength());
				if(subsection.CompareNoCase(gDesc.m_Description) == 0)
					cb->AddString(gDesc.m_Description);
			}

			state = gDesc.GotoNextEntry();
		}
			
		if(!gDesc.IsEOF())
			return;

		cb->ShowDropDown();
	}
}

void CAddDialogStock::OnEditchangeItemDescription() 
{
	if(!UpdateData(TRUE))
		return;

	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_ITEM_DESCRIPTION);
	if(cb)
	{
		int count = cb->GetCount();
		while(count > 0)
		{
			cb->DeleteString(count - 1);
			count--;
		}

		DatabaseState state = gDesc.GotoFirstEntry();

		if(state == DBInvalid)
			return;

		while(state == DBOK)
		{
			CString subsection = gDesc.m_Description.Left(m_item_description.GetLength());
			if(subsection.CompareNoCase(m_item_description) == 0)
				cb->AddString(gDesc.m_Description);

			state = gDesc.GotoNextEntry();
		}
			
		if(!gDesc.IsEOF())
			return;

		cb->ShowDropDown();
	}
}

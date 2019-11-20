#include "stdafx.h"
#include "Ratnakiran.h"
#include "TableItem.h"
#include "DescriptionAddDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTableItem, CRecordset)

CTableItem::CTableItem(CDatabase* pdb)
	: CRecordset(pdb), m_sub_items(pdb)
{
	//{{AFX_FIELD_INIT(CTableItem)
	//}}AFX_FIELD_INIT
	m_nFields = 20;
	m_nDefaultType = snapshot;
	m_strSort = "Item_ID";
}


CString CTableItem::GetDefaultConnect()
{
	return _T("ODBC;DSN=Ratnakiran");
}

CString CTableItem::GetDefaultSQL()
{
	return _T("[Items]");
}

void CTableItem::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTableItem)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Identifier]"), m_record.m_Identifier);
	RFX_Text(pFX, _T("[Item_ID]"), m_record.m_Item_ID);
	RFX_Text(pFX, _T("[Set_ID]"), m_record.m_Set_ID);
	RFX_Text(pFX, _T("[Description]"), m_record.m_Description);
	RFX_Int(pFX, _T("[Piece_count]"), m_record.m_Piece_count);
	RFX_Int(pFX, _T("[Serial_number]"), m_record.m_Serial_number);
	RFX_Text(pFX, _T("[Old_serials]"), m_record.m_Old_serials);
	RFX_Text(pFX, _T("[Photo_filename]"), m_record.m_Photo_filename);
	RFX_LongBinary(pFX, _T("[Photo]"), m_record.m_Photo);
	RFX_Double(pFX, _T("[Gold_rate]"), m_record.m_Gold_rate);
	RFX_Bool(pFX, _T("[Use_net_weight_for_gold]"), m_record.m_Use_net_weight_for_gold);
	RFX_Double(pFX, _T("[Gross_weight]"), m_record.m_Gross_weight);
	RFX_Double(pFX, _T("[Other_weight]"), m_record.m_Other_weight);
	RFX_Long(pFX, _T("[Labour_cost]"), m_record.m_Labour_cost);
	RFX_Text(pFX, _T("[Diamond_marked_value]"), m_record.m_Diamond_marked_value);
	RFX_Text(pFX, _T("[Stone_marked_value]"), m_record.m_Stone_marked_value);
	RFX_Text(pFX, _T("[Pearl_marked_value]"), m_record.m_Pearl_marked_value);
	RFX_Text(pFX, _T("[Labour_marked_value]"), m_record.m_Labour_marked_value);
	RFX_Bool(pFX, _T("[Sold]"), m_record.m_Sold);
	RFX_Text(pFX, _T("[Sold_in_country]"), m_record.m_Sold_in_country);
	//}}AFX_FIELD_MAP
}

#ifdef _DEBUG
void CTableItem::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTableItem::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

BOOL CTableItem::DBOpen(CString in_func, BOOL open_children)
{
	if(open_children && !m_sub_items.DBOpen("", TRUE))
		return FALSE;

	if(!IsOpen())
	{
		CString errstr = "Error while Open of items table";
		if(!in_func.IsEmpty())
			errstr = errstr + " in " + in_func;

		DB_PERFORM_RET(
		(Open()),
		errstr,
		{return FALSE;},
		{});
	}

	return TRUE;
}


BOOL CTableItem::DBClose(BOOL close_children)
{
	if(close_children && !m_sub_items.DBClose(TRUE))
		return FALSE;

	if(IsOpen())
		Close();

	return TRUE;
}


BOOL CTableItem::DBBeginTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	long current = m_record.m_Identifier;

	if(!m_pDatabase || !m_pDatabase->CanTransact())
		return FALSE;

	if(!DBClose(FALSE))
		return FALSE;

	BOOL status=TRUE;
	if(!m_pDatabase->BeginTrans())
		status=FALSE;

	if(!DBOpen("BeginTransaction",FALSE))
		status=FALSE;

	if(status)
	{
		m_strFilter = queryString;
		if(DBRequeryEntries("BeginTransaction", FALSE) == DBInvalid)
			status=FALSE;
	}

	if(status && !GotoItem(current, FALSE, FALSE))
		status=FALSE;

	if(status && include_children)
		status = m_sub_items.DBBeginTransaction(TRUE);

	return status;
}


BOOL CTableItem::DBRollbackTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	long current = m_record.m_Identifier;
	m_pDatabase->Rollback();

	BOOL status=TRUE;
	m_strFilter = queryString;

	if(DBRequeryEntries("DBRollbackTransaction", FALSE) == DBInvalid)
		status=FALSE;

	if(status && !GotoItem(current, FALSE, FALSE))
		status=FALSE;

	if(include_children && !m_sub_items.DBRollbackTransaction(TRUE))
		status=FALSE;

	return status;
}


BOOL CTableItem::DBCommitTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	long current = m_record.m_Identifier;
	m_pDatabase->CommitTrans();

	BOOL status=TRUE;
	m_strFilter = queryString;

	if(DBRequeryEntries("DBCommitTransaction", FALSE) == DBInvalid)
		status=FALSE;

	if(status && !GotoItem(current, FALSE, FALSE))
		status=FALSE;

	if(include_children && !m_sub_items.DBCommitTransaction(TRUE))
		status=FALSE;

	return status;
}

	
BOOL CTableItem::DBAddEntry()
{
	if(!IsOpen())
		return FALSE;

	if(!CanUpdate())
		return FALSE;

	DB_PERFORM(
	(AddNew()),
	{CancelUpdate(); return FALSE;},
	{});

	return TRUE;
}


BOOL CTableItem::DBEditEntry()
{
	if(!IsOpen())
		return FALSE;

	if(!CanUpdate())
		return FALSE;

	DB_PERFORM(
	(Edit()),
	{CancelUpdate(); return FALSE;},
	{});

	return TRUE;
}


BOOL CTableItem::DBUpdateEntry(CString in_func)
{
	CString errstr = "Error while Update of items table";
	if(!in_func.IsEmpty())
		errstr = errstr + " in " + in_func;

	if(!IsFieldDirty(NULL))
	{
		CancelUpdate();
		return TRUE;
	}

	long current_identifier = m_record.m_Identifier;

	DB_PERFORM_RET(
	(Update()),
	errstr,
	{return FALSE;},
	{});

	return TRUE;
}


BOOL CTableItem::DBDeleteEntry()
{
	if(!IsOpen())
		return FALSE;

	if(!CanUpdate())
		return FALSE;

	DB_PERFORM(
	(Delete()),
	{return FALSE;},
	{});

	return TRUE;
}


DatabaseState CTableItem::DBRequeryEntries(CString in_func, BOOL preserve_state)
{
	if(!IsOpen())
		return DBInvalid;

	CString errstr = "Error while Requery of items table";
	if(!in_func.IsEmpty())
		errstr = errstr + " in " + in_func;

	long current_item = m_record.m_Identifier;

	DB_PERFORM_RET(
	(Requery()),
	errstr,
	{return DBInvalid;},
	{});

	if(preserve_state && (current_item != 0) && (current_item != MAX_LONG))
		return GotoItem(current_item, FALSE);
	else
		return GotoFirstItem();
}


DatabaseState CTableItem::RequerySubItemsFor(long item_identifier)
{
	char query_string[1001];
	sprintf(query_string, "Item_identifier = %d", item_identifier);
	m_sub_items.m_strFilter = query_string;

	return ((m_sub_items.DBRequeryEntries("RequerySubItemsFor", FALSE) != DBInvalid) ? DBOK : DBInvalid);
}


DatabaseState CTableItem::GotoFirstItem(BOOL requery)
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	DB_PERFORM(
	(MoveFirst()),
	{return DBInvalid;},
	{});

	if(requery)
		return RequerySubItemsFor(m_record.m_Identifier);
	else
		return DBOK;
}


DatabaseState CTableItem::GotoNextItem(BOOL requery)
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	if(!IsEOF())
	{
		DB_PERFORM(
		(MoveNext()),
		{return DBInvalid;},
		{});

		if(IsEOF())
			return DBInvalid;
	}
	else
		return DBInvalid;

	if(requery)
		return RequerySubItemsFor(m_record.m_Identifier);
	else
		return DBOK;
}


DatabaseState CTableItem::GotoPrevItem(BOOL requery)
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	if(!IsBOF())
	{
		DB_PERFORM(
		(MovePrev()),
		{return DBInvalid;},
		{});

		if(IsBOF())
			return DBInvalid;
	}
	else
		return DBInvalid;

	if(requery)
		return RequerySubItemsFor(m_record.m_Identifier);
	else
		return DBOK;
}


DatabaseState CTableItem::GotoLastItem(BOOL requery)
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	DB_PERFORM(
	(MoveLast()),
	{return DBInvalid;},
	{});

	if(requery)
		return RequerySubItemsFor(m_record.m_Identifier);
	else
		return DBOK;
}


DatabaseState CTableItem::GotoPosItem(int pos, BOOL requery)
{
	DatabaseState state = GotoFirstItem(FALSE);
	if(state != DBOK)
		return state;

	while(pos > 0)
	{
		state = GotoNextItem(FALSE);
		if(state != DBOK)
			return state;

		pos--;
	}

	if(requery)
		return RequerySubItemsFor(m_record.m_Identifier);
	else
		return DBOK;
}


DatabaseState CTableItem::GotoItem(CString ID, BOOL requery)
{
	DatabaseState state = GotoFirstItem(FALSE);
	if(state != DBOK)
		return state;

	while(m_record.m_Item_ID != ID)
	{
		state = GotoNextItem(FALSE);
		if(state != DBOK)
			return state;
	}

	if(requery)
		return RequerySubItemsFor(m_record.m_Identifier);
	else
		return DBOK;
}


DatabaseState CTableItem::GotoItem(int val, BOOL use_serial, BOOL requery)
{
	DatabaseState state = GotoFirstItem(FALSE);
	if(state != DBOK)
		return state;

	while((use_serial && (m_record.m_Serial_number != val)) ||
		  (!use_serial && (m_record.m_Identifier != val)))
	{
		state = GotoNextItem(FALSE);
		if(state != DBOK)
			return state;
	}

	if(requery)
		return RequerySubItemsFor(m_record.m_Identifier);
	else
		return DBOK;
}


BOOL CTableItem::AddSubItem(CString category, CRecordSubItem new_sub_item)
{
	if(!m_sub_items.DBAddEntry())
		return FALSE;

	long orig_id = m_sub_items.m_record.m_Identifier;
	m_sub_items.m_record = new_sub_item;
	m_sub_items.m_record.m_Item_identifier = m_record.m_Identifier;
	m_sub_items.m_record.m_Item_ID = m_record.m_Item_ID;
	m_sub_items.m_record.m_Category = category;
	m_sub_items.m_record.m_Identifier = orig_id;

	if(!m_sub_items.DBUpdateEntry("AddSubItem"))
		return FALSE;

	return (m_sub_items.DBRequeryEntries("AddSubItem", TRUE) == DBOK);
}


BOOL CTableItem::DeleteSubItem(long sub_item_identifier)
{
	if(DB_IS_EMPTY(m_sub_items))
		return FALSE;

	long current_identifier = m_sub_items.m_record.m_Identifier;
	if(current_identifier == sub_item_identifier)
		current_identifier = 0;

	if(m_sub_items.GotoSubItem(sub_item_identifier) != DBOK)
		return FALSE;

	CString category = m_sub_items.m_record.m_Category;
	
	if(!m_sub_items.DBDeleteEntry())
		return FALSE;

	DatabaseState state = m_sub_items.DBRequeryEntries("DeleteSubItem", FALSE);
	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	if(current_identifier == 0)
		return (m_sub_items.GotoFirstSubItem() == DBOK);
	else
		return (m_sub_items.GotoSubItem(current_identifier) == DBOK);
}


BOOL CTableItem::DeleteSubItemList(CString category)
{
	if(DB_IS_EMPTY(m_sub_items))
		return TRUE;

	long current_identifier = m_sub_items.m_record.m_Identifier;

	DatabaseState state = m_sub_items.GotoFirstSubItem();
	if(state == DBInvalid)
		return FALSE;

	while(state == DBOK)
	{
		if(m_sub_items.m_record.m_Category == category)
		{
			int deleted_identifier = m_sub_items.m_record.m_Identifier;

			if(!m_sub_items.DBDeleteEntry())
				return FALSE;

			if(current_identifier == deleted_identifier)
				current_identifier = 0;
		}

		state = m_sub_items.GotoNextSubItem();
	}

	if(!m_sub_items.IsEOF())
		return FALSE;

	state = m_sub_items.DBRequeryEntries("DeleteSubItemList", FALSE);
	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	if(current_identifier == 0)
		return (m_sub_items.GotoFirstSubItem() == DBOK);
	else
		return (m_sub_items.GotoSubItem(current_identifier) == DBOK);
}


BOOL CTableItem::DeleteAllSubItem()
{
	if(DB_IS_EMPTY(m_sub_items))
		return TRUE;

	DatabaseState state = m_sub_items.GotoFirstSubItem();
	if(state == DBInvalid)
		return FALSE;

	while(state == DBOK)
	{
		if(!m_sub_items.DBDeleteEntry())
			return FALSE;

		state = m_sub_items.GotoNextSubItem();
	}

	if(!m_sub_items.IsEOF())
		return FALSE;

	if(m_sub_items.DBRequeryEntries("DeleteSubItemList", FALSE) == DBInvalid)
		return FALSE;

	return TRUE;
}


int CTableItem::GetSubItemCount(CString category)
{
	int count = 0;

	long current_identifier = m_sub_items.m_record.m_Identifier;
	DatabaseState state = m_sub_items.GotoFirstSubItem(FALSE);

	if(state != DBOK)
		return 0;

	while(state == DBOK)
	{
		if(m_sub_items.m_record.m_Category == category)
			count++;

		state = m_sub_items.GotoNextSubItem(FALSE);
	}

	BOOL error=FALSE;
	if(!m_sub_items.IsEOF())
		error=TRUE;

	if(current_identifier == 0)
	{
		if((m_sub_items.GotoFirstSubItem(FALSE) != DBOK) || error)
			return 0;
	}
	else
	{
		if((m_sub_items.GotoSubItem(current_identifier, FALSE) != DBOK) || error)
			return 0;
	}

	return count;
}


BOOL CTableItem::UpdateProductID(CString productID, BOOL showmessage)
{
	CString setID = productID;

	productID = CTableDescription::ExtractActualID(setID);
	CString prefix = gDesc.GetPrefix(m_record.m_Description);
	int pieces = gDesc.GetPieceCount(m_record.m_Description);

	if(prefix == "_EMPTY_") //Description is not present in list
	{
		prefix = "";

		if(showmessage)
		{
			int retval = MessageBox(NULL, "You have chosen a description which is not already in the database. Do you want to feed it in the database and assign the appropriate ID prefix for this description ? If you chose NO, then no description specific prefix will be added for the product ID's, and you would be prompted again with this same message the next time you use this description.", "Warning", MB_YESNO);
			if(retval == IDYES)
			{
				CDescriptionAddDialog dlg;
				dlg.m_name = m_record.m_Description;
				if(dlg.DoModal() == IDOK)
				{
					prefix = dlg.m_prefix;
					pieces = dlg.m_number_of_pieces;
				}
			}
		}
	}

	if(!prefix.IsEmpty())
	{
		CString setprefix = CTableDescription::ExtractPrefix(setID);
		if(prefix != setprefix)
			prefix = prefix + setprefix;
	}

	if(!DBEditEntry())
		return FALSE;

	productID = prefix + productID;
	m_record.m_Item_ID = productID;
	m_record.m_Piece_count = pieces;

	if(!DBUpdateEntry("UpdateProductID"))
		return FALSE;

	if(m_sub_items.DBRequeryEntries("UpdateProductID", TRUE) == DBInvalid)
		return FALSE;

	return TRUE;
}


BOOL CTableItem::AddImage(DWORD size, BYTE *data)
{
	DeleteImage();

	if((size == 0) && !data)
		return TRUE;
	else if(size == 0)
		return FALSE;
	else if(!data)
		return FALSE;

	HGLOBAL tempPtr = (HGLOBAL)malloc(size);
	if(!tempPtr)
		return FALSE;

	if(!DBEditEntry())
		return FALSE;

	m_record.m_Photo.m_hData = tempPtr;
	m_record.m_Photo.m_dwDataLength = size;
	memcpy(m_record.m_Photo.m_hData, data, size);

	if(!DBUpdateEntry("AddImage"))
		return FALSE;

	return TRUE;
}


BOOL CTableItem::DeleteImage()
{
	if(!DBEditEntry())
		return FALSE;

	if(m_record.m_Photo.m_hData)
		free(m_record.m_Photo.m_hData);

	m_record.m_Photo.m_dwDataLength = 0;
	m_record.m_Photo.m_hData = NULL;

	if(!DBUpdateEntry("DeleteImage"))
		return FALSE;

	return TRUE;
}


void CTableItem::ScaleImage(CSize original, CSize desired, CSize *final)
{
	if(!final)
		return;

	double dx = (double)original.cx / desired.cx;
	double dy = (double)original.cy / desired.cy;

	if(dx >= dy)
	{
		final->cx = desired.cx;
		final->cy = (int)(original.cy / dx);
	}
	else
	{
		final->cx = (int)(original.cx / dy);
		final->cy = desired.cy;
	}
}


void CTableItem::GetSerialList(CList<CRecordOldSerial, CRecordOldSerial> *list)
{
	if(!list)
		return;

	list->RemoveAll();

	CString oldSerials = m_record.m_Old_serials;
	while(oldSerials.GetLength() > 0)
	{
		int pos = oldSerials.Find("|");
		if(pos != -1)
		{
			if(pos == (oldSerials.GetLength() - 1))
				break;

			CString name, number;
			if(oldSerials[pos + 1] == '|')
			{
				oldSerials = oldSerials.Right(oldSerials.GetLength() - pos - 2);
				continue;
			}

			name = oldSerials.Left(pos);
			oldSerials = oldSerials.Right(oldSerials.GetLength() - pos - 1);

			pos = oldSerials.Find("||");
			if(pos != -1)
			{
				number = oldSerials.Left(pos);
				oldSerials = oldSerials.Right(oldSerials.GetLength() - pos - 2);
			}
			else 
				break;

			CRecordOldSerial entry;
			entry.m_tag = name;
			entry.m_serial = atoi(LPCTSTR(number));

			list->AddTail(entry);
		}
		else
		{
			break;
		}
	}
}


BOOL CTableItem::PutSerialList(CList<CRecordOldSerial, CRecordOldSerial> *list)
{
	if(!list)
		return FALSE;

	CString oldSerials;
	POSITION pos = list->GetHeadPosition();
	while(pos)
	{
		CRecordOldSerial entry = list->GetAt(pos);

		CString str;
		str.Format("%d",entry.m_serial);
		str = entry.m_tag + "|" + str + "||";

		oldSerials = oldSerials + str;
		list->GetNext(pos);
	}

	m_record.m_Old_serials = oldSerials;
	return TRUE;
}


int CTableItem::GetOldSerial(CString tagname)
{
	CList<CRecordOldSerial, CRecordOldSerial> list;
	GetSerialList(&list);

	POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		CRecordOldSerial entry = list.GetAt(pos);
		if(entry.m_tag == tagname)
			return entry.m_serial;
		else if(tagname.IsEmpty())
			return entry.m_serial;

		list.GetNext(pos);
	}

	return -1;
}


BOOL CTableItem::StoreOldSerialNumber(CString tagname)
{
	CList<CRecordOldSerial, CRecordOldSerial> list;
	GetSerialList(&list);

	CRecordOldSerial entry;
	entry.m_tag = tagname;
	entry.m_serial = m_record.m_Serial_number;
	list.AddHead(entry);

	if(!DBEditEntry())
		return FALSE;

	PutSerialList(&list);

	return DBUpdateEntry("StoreOldSerialNumber");
}


BOOL CTableItem::ResetOldSerialNumber()
{
	if(!DBEditEntry())
		return FALSE;

	CList<CRecordOldSerial, CRecordOldSerial> list;
	PutSerialList(&list);

	return DBUpdateEntry("StoreOldSerialNumber");
}


BOOL CTableItem::MarkItemAsSold(BOOL sold, CString country)
{
	if(!DBEditEntry())
		return FALSE;

	m_record.m_Sold = sold;
	m_record.m_Sold_in_country = country;

	return DBUpdateEntry("MarkItemAsSold");
}


BOOL CTableItem::IsItemSold()
{
	return m_record.m_Sold;
}


BOOL CTableItem::IsEmpty()
{
	return (m_record.m_Gross_weight == 0.0 || 
			m_record.m_Labour_cost == 0 ||
			(GetSubItemCount(DIAMOND) == 0 && 
			 GetSubItemCount(STONE) == 0 && 
			 GetSubItemCount(PEARL) == 0.0 && 
			 m_record.m_Other_weight == 0.0));
}


double CTableItem::GetGrossWeight()
{
	return m_record.m_Gross_weight;
}


double CTableItem::GetNetWeight()
{
	return (m_record.m_Gross_weight 
			- GramsFromCaret(GetSubItemWeight(DIAMOND) + 
							 GetSubItemWeight(STONE) + 
							 GetSubItemWeight(PEARL)) 
			- m_record.m_Other_weight);
}


double CTableItem::GetGoldValueDouble(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion)
{
	double value = 0.0;
	double weight = 0.0;

	if(use_net)
		weight = GetNetWeight();
	else
		weight = m_record.m_Gross_weight;

	value = weight * gold_rate;

	if(include_wastage)
		value = (value * 6.0) / 5.0;

	value = value / conversion;
	return value;
}


double CTableItem::GetSubItemWeight(CString category)
{
	double weight = 0.0;

	DatabaseState state = m_sub_items.GotoFirstSubItem();

	if(state != DBOK)
		return 0.0;

	while(state == DBOK)
	{
		if(category == m_sub_items.m_record.m_Category)
			weight += m_sub_items.m_record.m_Weight;

		state = m_sub_items.GotoNextSubItem();
	}
	
	if(!m_sub_items.IsEOF())
		return 0.0;

	return weight;
}


double CTableItem::GetSubItemValueDouble(CString category, double conversion)
{
	double value = 0.0;

	DatabaseState state = m_sub_items.GotoFirstSubItem();

	if(state != DBOK)
		return 0.0;

	while(state == DBOK)
	{
		if(category == m_sub_items.m_record.m_Category)
			value += (m_sub_items.m_record.m_Weight * m_sub_items.m_record.m_Rate);

		state = m_sub_items.GotoNextSubItem();
	}
	
	if(!m_sub_items.IsEOF())
		return 0.0;

	value = value / conversion;
	return value;
}


double CTableItem::GetSubItemValueUsingDouble(CString category, int rate, double conversion)
{
	double value = 0.0;
	value = (GetSubItemWeight(category) * (double)rate) / conversion;
	return value;
}


int CTableItem::GetSubItemRate(CString category, double conversion)
{
	double rate = 0.0;

	if(GetSubItemWeight(category) != 0.0)
	{
		rate = GetSubItemValueDouble(category, 1.0) / GetSubItemWeight(category);
		rate = rate / conversion;
	}

	return (int)(rate + 0.5);
}


double CTableItem::GetMarkedSubItemValueDouble(CString category, double gold_rate, double conversion)
{
	CString formula;
	if(category == DIAMOND)
		formula = m_record.m_Diamond_marked_value;
	else if(category == STONE)
		formula = m_record.m_Stone_marked_value;
	else if(category == PEARL)
		formula = m_record.m_Pearl_marked_value;
	else
		return 0.0;

	double val = ProcessFormula(formula, gold_rate, 1.0, this);
	val = val / conversion;
	return val;
}


int CTableItem::GetMarkedSubItemRate(CString category, double gold_rate, double conversion)
{
	double rate = 0.0;

	if(GetSubItemWeight(category) != 0.0)
	{
		rate = GetMarkedSubItemValueDouble(category, gold_rate, 1.0) / GetSubItemWeight(category);
		rate = rate / conversion;
	}

	return (int)(rate + 0.5);
}


double CTableItem::GetLabourCostDouble(double conversion)
{
	double cost = m_record.m_Labour_cost / conversion;
	return cost;
}


double CTableItem::GetMarkedLabourCostDouble(double gold_rate, double conversion)
{
	CString formula = m_record.m_Labour_marked_value;
	double val = ProcessFormula(formula, gold_rate, 1.0, this);
	val = val / conversion;
	return val;
}


double CTableItem::GetOtherWeight()
{
	return m_record.m_Other_weight;
}


double CTableItem::GetTotalValueDouble(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion)
{
	double total = GetSubItemValueDouble(DIAMOND, 1.0) +
				   GetSubItemValueDouble(STONE, 1.0) +
				   GetSubItemValueDouble(PEARL, 1.0) +
				   GetLabourCostDouble(1.0) +
				   GetGoldValueDouble(use_net, include_wastage, gold_rate, 1.0);

	total = total / conversion;
	return total;
}


double CTableItem::GetTotalMarkedValueDouble(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion)
{
	double total = GetMarkedSubItemValueDouble(DIAMOND, gold_rate, 1.0) +
				   GetMarkedSubItemValueDouble(STONE, gold_rate, 1.0) +
				   GetMarkedSubItemValueDouble(PEARL, gold_rate, 1.0) +
				   GetMarkedLabourCostDouble(gold_rate, 1.0) +
				   GetGoldValueDouble(use_net, include_wastage, gold_rate, 1.0);
	
	total = total / conversion;
	return total;
}

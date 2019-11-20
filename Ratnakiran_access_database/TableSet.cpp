#include "stdafx.h"
#include "Ratnakiran.h"
#include "TableSet.h"
#include "DescriptionAddDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTableSet, CRecordset)

CTableSet::CTableSet(CDatabase* pdb)
	: CRecordset(pdb), m_items(pdb)
{
	//{{AFX_FIELD_INIT(CTableSet)
	//}}AFX_FIELD_INIT
	m_nFields = 5;
	m_nDefaultType = snapshot;
	m_strSort = "Set_ID";
}


CString CTableSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=Ratnakiran");
}

CString CTableSet::GetDefaultSQL()
{
	return _T("[Sets]");
}

void CTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTableSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[Set_ID]"), m_record.m_Set_ID);
	RFX_Text(pFX, _T("[Voucher_ID]"), m_record.m_Voucher_ID);
	RFX_Text(pFX, _T("[Description]"), m_record.m_Description);
	RFX_Long(pFX, _T("[Item_count_sold]"), m_record.m_Item_count_sold);
	RFX_Long(pFX, _T("[Item_count_unsold]"), m_record.m_Item_count_unsold);
	//}}AFX_FIELD_MAP
}

#ifdef _DEBUG
void CTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


BOOL CTableSet::DBOpen(CString in_func, BOOL open_children)
{
	if(open_children && !m_items.DBOpen("", TRUE))
		return FALSE;

	if(!IsOpen())
	{
		CString errstr = "Error while Open of sets table";
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


BOOL CTableSet::DBClose(BOOL close_children)
{
	if(close_children && !m_items.DBClose(TRUE))
		return FALSE;

	if(IsOpen())
		Close();

	return TRUE;
}


BOOL CTableSet::DBBeginTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	CString current = m_record.m_Set_ID;

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

	if(status && !GotoSet(current, FALSE))
		status=FALSE;

	if(status && include_children)
		status = m_items.DBBeginTransaction(TRUE);

	return status;
}


BOOL CTableSet::DBRollbackTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	CString current = m_record.m_Set_ID;
	m_pDatabase->Rollback();

	BOOL status=TRUE;
	m_strFilter = queryString;

	if(DBRequeryEntries("DBRollbackTransaction", FALSE) == DBInvalid)
		status=FALSE;

	if(status && !GotoSet(current, FALSE))
		status=FALSE;

	if(include_children && !m_items.DBRollbackTransaction(TRUE))
		status=FALSE;

	return status;
}


BOOL CTableSet::DBCommitTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	CString current = m_record.m_Set_ID;
	m_pDatabase->CommitTrans();

	BOOL status=TRUE;
	m_strFilter = queryString;

	if(DBRequeryEntries("DBCommitTransaction", FALSE) == DBInvalid)
		status=FALSE;

	if(status && !GotoSet(current, FALSE))
		status=FALSE;

	if(include_children && !m_items.DBCommitTransaction(TRUE))
		status=FALSE;

	return status;
}


BOOL CTableSet::DBAddEntry()
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


BOOL CTableSet::DBEditEntry()
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


BOOL CTableSet::DBUpdateEntry(CString in_func)
{
	CString errstr = "Error while Update of sets table";
	if(!in_func.IsEmpty())
		errstr = errstr + " in " + in_func;

	if(!IsFieldDirty(NULL))
	{
		CancelUpdate();
		return TRUE;
	}
	
	DB_PERFORM_RET(
	(Update()),
	errstr,
	{return FALSE;},
	{});

	return TRUE;
}


BOOL CTableSet::DBDeleteEntry()
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


DatabaseState CTableSet::DBRequeryEntries(CString in_func, BOOL preserve_state)
{
	if(!IsOpen())
		return DBInvalid;

	CString errstr = "Error while Requery of sets table";
	if(!in_func.IsEmpty())
		errstr = errstr + " in " + in_func;

	CString current_set = m_record.m_Set_ID;

	DB_PERFORM_RET(
	(Requery()),
	errstr,
	{return DBInvalid;},
	{});

	if(preserve_state && !current_set.IsEmpty())
		return GotoSet(current_set);
	else
		return GotoFirstSet();
}


DatabaseState CTableSet::RequeryItemsFor(CString set_ID)
{
	char query_string[1001];
	sprintf(query_string, "Set_ID = '%s'", LPCTSTR(set_ID));
	m_items.m_strFilter = query_string;

	return ((m_items.DBRequeryEntries("GotoFirstSet", FALSE) != DBInvalid) ? DBOK : DBInvalid);
}


DatabaseState CTableSet::GotoFirstSet(BOOL requery)
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
		return RequeryItemsFor(m_record.m_Set_ID);
	else
		return DBOK;
}


DatabaseState CTableSet::GotoNextSet(BOOL requery)
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
		return RequeryItemsFor(m_record.m_Set_ID);
	else
		return DBOK;
}


DatabaseState CTableSet::GotoPrevSet(BOOL requery)
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
		return RequeryItemsFor(m_record.m_Set_ID);
	else
		return DBOK;
}


DatabaseState CTableSet::GotoLastSet(BOOL requery)
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
		return RequeryItemsFor(m_record.m_Set_ID);
	else
		return DBOK;
}


DatabaseState CTableSet::GotoPosSet(int pos, BOOL requery)
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	DB_PERFORM(
	(MoveFirst()),
	{return DBInvalid;},
	{});

	while(pos > 0)
	{
		DB_PERFORM(
		(MoveNext()),
		{return DBInvalid;},
		{});

		if(IsEOF())
			return DBInvalid;

		pos--;
	}

	if(requery)
		return RequeryItemsFor(m_record.m_Set_ID);
	else
		return DBOK;
}


DatabaseState CTableSet::GotoSet(CString ID, BOOL requery)
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	DB_PERFORM(
	(MoveFirst()),
	{return DBInvalid;},
	{});

	while(m_record.m_Set_ID != ID)
	{
		DB_PERFORM(
		(MoveNext()),
		{return DBInvalid;},
		{});
		
		if(IsEOF())
			return DBInvalid;
	}

	if(requery)
		return RequeryItemsFor(m_record.m_Set_ID);
	else
		return DBOK;
}


BOOL CTableSet::AddItem(CRecordItem new_item, BOOL update_ID)
{
	if(update_ID)
	{
		CString productID = CTableDescription::ExtractActualID(m_record.m_Set_ID);
		CString prefix = gDesc.GetPrefix(new_item.m_Description);
		int pieces = gDesc.GetPieceCount(new_item.m_Description);

		if(!prefix.IsEmpty())
		{
			if(prefix == "_EMPTY_")
				prefix = "";
			else
			{
				CString setprefix = CTableDescription::ExtractPrefix(m_record.m_Set_ID);
				if(prefix != setprefix)
					prefix = prefix + setprefix;
			}
		}

		productID = prefix + productID;
		new_item.m_Item_ID = productID;
		new_item.m_Piece_count = pieces;
	}

	if(!m_items.DBAddEntry())
		return FALSE;

	long orig_id = m_items.m_record.m_Identifier;
	m_items.m_record = new_item;
	m_items.m_record.m_Set_ID = m_record.m_Set_ID;
	m_items.m_record.m_Identifier = orig_id;

	if(!m_items.DBUpdateEntry("AddItem"))
		return FALSE;

	if(!DBEditEntry())
		return FALSE;

	if(m_items.IsItemSold())
		m_record.m_Item_count_sold++;
	else
		m_record.m_Item_count_unsold++;

	if(!DBUpdateEntry("AddItem"))
		return FALSE;

	return (m_items.DBRequeryEntries("AddItem", TRUE) == DBOK);
}


BOOL CTableSet::DeleteItem(long item_identifier)
{
	if(DB_IS_EMPTY(m_items))
		return FALSE;

	long current_identifier = m_items.m_record.m_Identifier;
	if(current_identifier == item_identifier)
		current_identifier = 0;

	if(m_items.GotoItem(item_identifier, FALSE) != DBOK)
		return FALSE;

	if(!m_items.DBDeleteEntry())
		return FALSE;

	if(!DBEditEntry())
		return FALSE;

	if(m_items.IsItemSold())
		m_record.m_Item_count_sold--;
	else
		m_record.m_Item_count_unsold--;

	if(!DBUpdateEntry("DeleteItem"))
		return FALSE;

	DatabaseState state = m_items.DBRequeryEntries("DeleteItem", FALSE);
	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	if(current_identifier == 0)
		return (m_items.GotoFirstItem() == DBOK);
	else
		return (m_items.GotoItem(current_identifier, FALSE) == DBOK);
}


BOOL CTableSet::DeleteAllItems()
{
	DatabaseState state = m_items.GotoFirstItem();

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		if(!m_items.DBDeleteEntry())
			return FALSE;

		state = m_items.GotoNextItem();
	}
	
	if(!m_items.IsEOF())
		return FALSE;

	if(!DBEditEntry())
		return FALSE;

	m_record.m_Item_count_sold = 0;
	m_record.m_Item_count_unsold = 0;

	if(!DBUpdateEntry("DeleteAllItems"))
		return FALSE;

	if(m_items.DBRequeryEntries("DeleteItem", FALSE) == DBInvalid)
		return FALSE;

	return TRUE;
}


BOOL CTableSet::DeleteSoldItems()
{
	long current_identifier = m_items.m_record.m_Identifier;

	DatabaseState state = m_items.GotoFirstItem();

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		if(m_items.IsItemSold())
		{
			long deleted_identifier = m_items.m_record.m_Identifier;
			if(!m_items.DBDeleteEntry())
				return FALSE;

			if(current_identifier == deleted_identifier)
				current_identifier = 0;
		}

		state = m_items.GotoNextItem();
	}
	
	if(!m_items.IsEOF())
		return FALSE;

	GetItemCount(Both, TRUE);

	state = m_items.DBRequeryEntries("DeleteSoldItems", FALSE);
	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	if(current_identifier == 0)
		return (m_items.GotoFirstItem() == DBOK);
	else
		return (m_items.GotoItem(current_identifier, FALSE) == DBOK);
}


int CTableSet::GetItemCount(Sold_Status type, BOOL force_requery)
{
	if(!force_requery)
	{
		if((type == Both) && 
		   ((m_record.m_Item_count_sold > 0) || (m_record.m_Item_count_unsold > 0)) &&
		   ((m_record.m_Item_count_sold < MAX_LONG) || (m_record.m_Item_count_unsold < MAX_LONG)))
		{
			return (m_record.m_Item_count_sold + m_record.m_Item_count_unsold);
		}
		else if((type == Sold) && (m_record.m_Item_count_sold > 0))
			return m_record.m_Item_count_sold;
		else if((type == UnSold) && (m_record.m_Item_count_unsold > 0))
			return m_record.m_Item_count_unsold;
	}

	if(DB_IS_EMPTY(m_items))
	{
		if(force_requery)
		{
			if(!DBEditEntry())
				return 0;

			m_record.m_Item_count_sold = 0;
			m_record.m_Item_count_unsold = 0;

			DBUpdateEntry("GetItemCount");
		}

		return 0;
	}

	int sold_count = 0;
	int unsold_count = 0;
	long current_identifier = m_items.m_record.m_Identifier;

	DatabaseState state = m_items.GotoFirstItem();
	if(state != DBOK)
		return 0;

	while(state == DBOK)
	{
		if(m_items.IsItemSold())
		{
			sold_count++;
		}
		else
		{
			unsold_count++;
		}


		state = m_items.GotoNextItem();
	}

	if(current_identifier == 0)
	{
		if(m_items.GotoFirstItem() != DBOK)
			return 0;
	}
	else
	{
		if(m_items.GotoItem(current_identifier, FALSE) != DBOK)
			return 0;
	}

	int count = 0;
	if(type == Both)
	{
		count = sold_count + unsold_count;

		if(force_requery)
		{
			if(!DBEditEntry())
				return 0;

			m_record.m_Item_count_sold = sold_count;
			m_record.m_Item_count_unsold = unsold_count;

			DBUpdateEntry("GetItemCount");
		}
	}
	else if(type == Sold)
	{
		count = sold_count;

		if(force_requery)
		{
			if(!DBEditEntry())
				return 0;

			m_record.m_Item_count_sold = count;

			DBUpdateEntry("GetItemCount");
		}
	}
	else if(type == UnSold)
	{
		count = unsold_count;

		if(force_requery)
		{
			if(!DBEditEntry())
				return 0;

			m_record.m_Item_count_unsold = count;

			DBUpdateEntry("GetItemCount");
		}
	}

	return count;
}


int CTableSet::GetTotalItemCount(Sold_Status type)
{
	int count = 0;
	long current_identifier = m_items.m_record.m_Identifier;

	DatabaseState state = m_items.GotoFirstItem();

	if(state != DBOK)
		return 0;

	while(state == DBOK)
	{
		if(((type == Sold) && m_items.IsItemSold()) ||
		   ((type == UnSold) && !m_items.IsItemSold()) ||
		   (type == Both))
		{
			count += m_items.m_record.m_Piece_count;
		}

		state = m_items.GotoNextItem();
	}

	if(current_identifier == 0)
	{
		if(m_items.GotoFirstItem() != DBOK)
			return 0;
	}
	else
	{
		if(m_items.GotoItem(current_identifier, FALSE) != DBOK)
			return 0;
	}

	return count;
}


BOOL CTableSet::HasEmptyItems()
{
	return FALSE;
	
	BOOL result = FALSE;
	long current_identifier = m_items.m_record.m_Identifier;

	DatabaseState state = m_items.GotoFirstItem();

	if(state != DBOK)
		return FALSE;

	while(state == DBOK)
	{
		if(m_items.IsEmpty())
		{
			result = TRUE;
			break;
		}

		state = m_items.GotoNextItem();
	}

	if(current_identifier == 0)
	{
		if(m_items.GotoFirstItem() != DBOK)
			return FALSE;
	}
	else
	{
		if(m_items.GotoItem(current_identifier, FALSE) != DBOK)
			return FALSE;
	}

	return result;
}


BOOL CTableSet::UpdateProductID(CString productID, BOOL showmessage)
{
	productID = CTableDescription::ExtractActualID(productID);
	CString prefix = gDesc.GetPrefix(m_record.m_Description);

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
				}
			}
		}
	}

	if(!DBEditEntry())
		return FALSE;

	productID = prefix + productID;
	m_record.m_Set_ID = productID;

	if(!DBUpdateEntry("UpdateProductID"))
		return FALSE;

	if(GotoSet(productID) != DBOK)
		return FALSE;

	return UpdateItemIDs();
}


BOOL CTableSet::UpdateItemIDs()
{
	long current_identifier = m_items.m_record.m_Identifier;

	DatabaseState state = m_items.GotoFirstItem();

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		if(!m_items.UpdateProductID(m_record.m_Set_ID, FALSE))
			return FALSE;

		state = m_items.GotoNextItem();
	}
	
	if(!m_items.IsEOF())
		return FALSE;

	if(current_identifier == 0)
		return (m_items.GotoFirstItem() == DBOK);
	else
		return (m_items.GotoItem(current_identifier, FALSE) == DBOK);
}


BOOL CTableSet::UpdateItemSerials(int *starting)
{
	long current_identifier = m_items.m_record.m_Identifier;

	DatabaseState state = m_items.GotoFirstItem();

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		if(!m_items.DBEditEntry())
			return FALSE;

		m_items.m_record.m_Serial_number = *starting;

		if(!m_items.DBUpdateEntry("UpdateItemSerial"))
			return FALSE;

		state = m_items.GotoNextItem();
	}
	
	if(!m_items.IsEOF())
		return FALSE;

	*starting = *starting + 1;

	if(current_identifier == 0)
		return (m_items.GotoFirstItem() == DBOK);
	else
		return (m_items.GotoItem(current_identifier, FALSE) == DBOK);
}


BOOL CTableSet::MarkSetAsSold(BOOL sold, CString country)
{
	long current_identifier = m_items.m_record.m_Identifier;

	DatabaseState state = m_items.GotoFirstItem();

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		if(!m_items.MarkItemAsSold(sold, country))
			return FALSE;

		state = m_items.GotoNextItem();
	}
	
	if(!m_items.IsEOF())
		return FALSE;

	GetItemCount(Both, TRUE);

	if(current_identifier == 0)
		return (m_items.GotoFirstItem() == DBOK);
	else
		return (m_items.GotoItem(current_identifier, FALSE) == DBOK);
}


BOOL CTableSet::StoreOldSerialNumbers(CString tagname)
{
	long current_identifier = m_items.m_record.m_Identifier;

	DatabaseState state = m_items.GotoFirstItem();

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		if(!m_items.StoreOldSerialNumber(tagname))
			return FALSE;

		state = m_items.GotoNextItem();
	}
	
	if(!m_items.IsEOF())
		return FALSE;

	if(current_identifier == 0)
		return (m_items.GotoFirstItem() == DBOK);
	else
		return (m_items.GotoItem(current_identifier, FALSE) == DBOK);
}


BOOL CTableSet::ResetOldSerialNumbers()
{
	long current_identifier = m_items.m_record.m_Identifier;

	DatabaseState state = m_items.GotoFirstItem();

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		if(!m_items.ResetOldSerialNumber())
			return FALSE;

		state = m_items.GotoNextItem();
	}
	
	if(!m_items.IsEOF())
		return FALSE;

	if(current_identifier == 0)
		return (m_items.GotoFirstItem() == DBOK);
	else
		return (m_items.GotoItem(current_identifier, FALSE) == DBOK);
}


CString CTableSet::GenerateSerialString(int *lowest_serial)
{
	CString ret;

	CArray<int,int> serial_array;
	serial_array.SetSize(GetItemCount(Both, FALSE));

	int array_elements = 0;
	long current_identifier = m_items.m_record.m_Identifier;

	DatabaseState state = m_items.GotoFirstItem();

	if(state != DBOK)
		return ret;

	while(state == DBOK)
	{
		int serial = m_items.m_record.m_Serial_number;

		if(array_elements == 0)
		{
			serial_array[array_elements] = serial;
			array_elements++;
		}
		else
		{
			int cur_index = 0;
			while(cur_index < array_elements)
			{
				if(serial < serial_array[cur_index])
				{
					serial_array.InsertAt(cur_index, serial);
					array_elements++;
					break;
				}

				cur_index++;
			}

			if(cur_index == array_elements)//reached end
			{
				serial_array[cur_index] = serial;
				array_elements++;
			}
		}

		state = m_items.GotoNextItem();
	}
	
	if(array_elements <= 1)
	{
		int cur_index = 0;
		while(cur_index < array_elements)
		{
			int serial = serial_array[cur_index];
			CString tempstring;
			tempstring.Format("%d",serial);
			ret = ret + tempstring;
			cur_index++;

			if(cur_index < array_elements)
				ret = ret + ",";
		}
	}
	else
	{
		int cur_index = 0;
		while(cur_index < array_elements)
		{
			if(cur_index == (array_elements - 1))
			{
				CString tempstring;
				tempstring.Format("%d",serial_array[cur_index]);
				ret = ret + tempstring;
				break;
			}

			int first_index = cur_index;
			int sec_index = cur_index + 1;
			BOOL cont = TRUE;

			while(cont && (sec_index < array_elements))
			{
				if((serial_array[first_index] + 1) == serial_array[sec_index])
				{
					first_index++;
					sec_index++;
				}
				else
				{
					if(cur_index == first_index)
					{
						CString tempstring;
						tempstring.Format("%d,",serial_array[first_index]);
						ret = ret + tempstring;
						cur_index++;
						cont = FALSE;
					}
					else
					{
						CString tempstring;
						tempstring.Format("%d..%d,",serial_array[cur_index],serial_array[first_index]);
						ret = ret + tempstring;
						cur_index = sec_index;
						cont = FALSE;
					}
				}
			}

			if(cont)//combine all elements till end
			{
				CString tempstring;
				tempstring.Format("%d..%d",serial_array[cur_index],serial_array[first_index]);
				ret = ret + tempstring;
				cur_index = sec_index;
			}
		}
	}

	if(lowest_serial)
		*lowest_serial = serial_array[0];

	if(current_identifier == 0)
	{
		if(m_items.GotoFirstItem() != DBOK)
			ret.Empty();
	}
	else
	{
		if(m_items.GotoItem(current_identifier, FALSE) != DBOK)
			ret.Empty();
	}

	return ret;
}

#include "stdafx.h"
#include "Ratnakiran.h"
#include "TableVoucher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTableVoucher, CRecordset)

CTableVoucher::CTableVoucher(CDatabase* pdb)
	: CRecordset(pdb), m_sets(pdb)
{
	//{{AFX_FIELD_INIT(CTableVoucher)
	//}}AFX_FIELD_INIT
	m_nFields = 16;
	m_nDefaultType = snapshot;
}


CString CTableVoucher::GetDefaultConnect()
{
	return _T("ODBC;DSN=Ratnakiran");
}

CString CTableVoucher::GetDefaultSQL()
{
	return _T("[Vouchers]");
}

void CTableVoucher::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTableVoucher)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[Voucher_ID]"), m_record.m_Voucher_ID);
	RFX_Text(pFX, _T("[Type]"), m_record.m_Type);
	RFX_Long(pFX, _T("[Set_count]"), m_record.m_Set_count);
	RFX_Double(pFX, _T("[Gold_rate]"), m_record.m_Gold_rate);
	RFX_Text(pFX, _T("[Invoice_number]"), m_record.m_Invoice_number);
	RFX_Text(pFX, _T("[Consignee]"), m_record.m_Consignee);
	RFX_Text(pFX, _T("[Exporter]"), m_record.m_Exporter);
	RFX_Text(pFX, _T("[Country]"), m_record.m_Country);
	RFX_Text(pFX, _T("[Currency_name]"), m_record.m_Currency_name);
	RFX_Double(pFX, _T("[Currency_conversion_rate]"), m_record.m_Currency_conversion_rate);
	RFX_Double(pFX, _T("[Dollar_rate]"), m_record.m_Dollar_rate);
	RFX_Text(pFX, _T("[Exports_labour_cost_formula]"), m_record.m_Exports_labour_cost_formula);
	RFX_Text(pFX, _T("[Exports_diamond_rate_formula]"), m_record.m_Exports_diamond_rate_formula);
	RFX_Text(pFX, _T("[Exports_pearl_rate_formula]"), m_record.m_Exports_pearl_rate_formula);
	RFX_Text(pFX, _T("[Exports_stone_rate_formula]"), m_record.m_Exports_stone_rate_formula);
	RFX_Text(pFX, _T("[Price_quotation_formula]"), m_record.m_Price_quotation_formula);
	//}}AFX_FIELD_MAP
}

#ifdef _DEBUG
void CTableVoucher::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTableVoucher::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


BOOL CTableVoucher::DBOpen(CString in_func, BOOL open_children)
{
	if(open_children && !m_sets.DBOpen("", TRUE))
		return FALSE;

	if(!IsOpen())
	{
		CString errstr = "Error while Open of vouchers table";
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


BOOL CTableVoucher::DBClose(BOOL close_children)
{
	if(close_children && !m_sets.DBClose(TRUE))
		return FALSE;

	if(IsOpen())
		Close();

	return TRUE;
}


BOOL CTableVoucher::DBBeginTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	CString current = m_record.m_Voucher_ID;

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
		if(DBRequeryEntries("DBBeginTransaction", FALSE) == DBInvalid)
			status=FALSE;
	}

	if(status && !GotoVoucher(current, FALSE))
		status=FALSE;

	if(status && include_children)
		status = m_sets.DBBeginTransaction(TRUE);

	return status;
}


BOOL CTableVoucher::DBRollbackTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	CString current = m_record.m_Voucher_ID;
	m_pDatabase->Rollback();

	BOOL status=TRUE;
	m_strFilter = queryString;

	if(DBRequeryEntries("DBRollbackTransaction", FALSE) == DBInvalid)
		status=FALSE;

	if(status && !GotoVoucher(current, FALSE))
		status=FALSE;

	if(include_children && !m_sets.DBRollbackTransaction(TRUE))
		status=FALSE;

	return status;
}


BOOL CTableVoucher::DBCommitTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	CString current = m_record.m_Voucher_ID;
	m_pDatabase->CommitTrans();

	BOOL status=TRUE;
	m_strFilter = queryString;

	if(DBRequeryEntries("DBCommitTransaction", FALSE) == DBInvalid)
		status=FALSE;

	if(status && !GotoVoucher(current, FALSE))
		status=FALSE;

	if(include_children && !m_sets.DBCommitTransaction(TRUE))
		status=FALSE;

	return status;
}


BOOL CTableVoucher::DBAddEntry()
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


BOOL CTableVoucher::DBEditEntry()
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


BOOL CTableVoucher::DBUpdateEntry(CString in_func)
{
	CString errstr = "Error while Update of vouchers table";
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


BOOL CTableVoucher::DBDeleteEntry()
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


DatabaseState CTableVoucher::DBRequeryEntries(CString in_func, BOOL preserve_state)
{
	if(!IsOpen())
		return DBInvalid;

	CString errstr = "Error while Requery of vouchers table";
	if(!in_func.IsEmpty())
		errstr = errstr + " in " + in_func;

	CString current_voucher = m_record.m_Voucher_ID;

	DB_PERFORM_RET(
	(Requery()),
	errstr,
	{return DBInvalid;},
	{});

	if(preserve_state && !current_voucher.IsEmpty())
		return GotoVoucher(current_voucher);
	else
		return GotoFirstVoucher();
}


DatabaseState CTableVoucher::RequerySetsFor(CString voucher)
{
	if(voucher == STOCK_MAIN)
	{
		CList<CString, CString> list;
		GetList(STOCK_TYPE, &list);

		if(list.IsEmpty())
		{
			m_sets.m_strFilter = "";
			return m_sets.DBRequeryEntries("RequerySetsFor", FALSE);
		}

		
		CString queryStr = "(Voucher_ID=";
		queryStr = queryStr + "'" + list.RemoveHead() + "'";
		
		while(!list.IsEmpty())
		{
			queryStr = queryStr + " OR Voucher_ID='" + list.RemoveHead() + "'";
		}

		queryStr = queryStr + ")";
		m_sets.m_strFilter = LPCTSTR(queryStr);

		return m_sets.DBRequeryEntries("RequerySetsFor", FALSE);
	}
	else if(voucher == EXPORT_MAIN)
	{
		CList<CString, CString> list;
		GetList(EXPORT_TYPE, &list);

		if(list.IsEmpty())
		{
			m_sets.m_strFilter = "";
			return m_sets.DBRequeryEntries("RequerySetsFor", FALSE);
		}

		
		CString queryStr = "(Voucher_ID=";
		queryStr = queryStr + "'" + list.RemoveHead() + "'";
		
		while(!list.IsEmpty())
		{
			queryStr = queryStr + " OR Voucher_ID='" + list.RemoveHead() + "'";
		}

		queryStr = queryStr + ")";
		m_sets.m_strFilter = LPCTSTR(queryStr);

		return m_sets.DBRequeryEntries("RequerySetsFor", FALSE);
	}
	else if(voucher == KUNDAN_MAIN)
	{
		CList<CString, CString> list;
		GetList(KUNDAN_TYPE, &list);

		if(list.IsEmpty())
		{
			m_sets.m_strFilter = "";
			return m_sets.DBRequeryEntries("RequerySetsFor", FALSE);
		}

		
		CString queryStr = "(Voucher_ID=";
		queryStr = queryStr + "'" + list.RemoveHead() + "'";
		
		while(!list.IsEmpty())
		{
			queryStr = queryStr + " OR Voucher_ID='" + list.RemoveHead() + "'";
		}

		queryStr = queryStr + ")";
		m_sets.m_strFilter = LPCTSTR(queryStr);

		return m_sets.DBRequeryEntries("RequerySetsFor", FALSE);
	}
	else
	{
		CString queryStr;
		queryStr.Format("Voucher_ID = '%s'", LPCTSTR(voucher));
		m_sets.m_strFilter = LPCTSTR(queryStr);

		return m_sets.DBRequeryEntries("RequerySetsFor", FALSE);
	}
}


DatabaseState CTableVoucher::GotoFirstVoucher(BOOL requery)
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
		return ((RequerySetsFor(m_record.m_Voucher_ID) != DBInvalid) ? DBOK : DBInvalid);
	else
		return DBOK;
}


DatabaseState CTableVoucher::GotoNextVoucher(BOOL requery)
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
		return ((RequerySetsFor(m_record.m_Voucher_ID) != DBInvalid) ? DBOK : DBInvalid);
	else
		return DBOK;
}


DatabaseState CTableVoucher::GotoPrevVoucher(BOOL requery)
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
		return ((RequerySetsFor(m_record.m_Voucher_ID) != DBInvalid) ? DBOK : DBInvalid);
	else
		return DBOK;
}


DatabaseState CTableVoucher::GotoLastVoucher(BOOL requery)
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
		return ((RequerySetsFor(m_record.m_Voucher_ID) != DBInvalid) ? DBOK : DBInvalid);
	else
		return DBOK;
}


DatabaseState CTableVoucher::GotoPosVoucher(int pos, BOOL requery)
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
		return ((RequerySetsFor(m_record.m_Voucher_ID) != DBInvalid) ? DBOK : DBInvalid);
	else
		return DBOK;
}


DatabaseState CTableVoucher::GotoVoucher(CString ID, BOOL requery)
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	DB_PERFORM(
	(MoveFirst()),
	{return DBInvalid;},
	{});

	while(m_record.m_Voucher_ID != ID)
	{
		DB_PERFORM(
		(MoveNext()),
		{return DBInvalid;},
		{});
		
		if(IsEOF())
			return DBInvalid;
	}

	if(requery)
		return ((RequerySetsFor(m_record.m_Voucher_ID) != DBInvalid) ? DBOK : DBInvalid);
	else
		return DBOK;
}


BOOL CTableVoucher::GetList(CString type, CList<CString, CString> *list)
{
	if(!IsOpen())
		return FALSE;

	CString current_voucher = m_record.m_Voucher_ID;

	DatabaseState state = GotoFirstVoucher(FALSE);

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		if(m_record.m_Type == type)
			list->AddTail(m_record.m_Voucher_ID);

		state = GotoNextVoucher(FALSE);
	}
	
	if(!IsEOF())
		return FALSE;

	return (GotoVoucher(current_voucher, FALSE) == DBOK);
}


double CTableVoucher::GetGoldRate()
{
	double val = 0.0;

	if(!IsOpen() && gSettings.IsOpen())
		val = gSettings.m_Gold_rate;
	else if(IsOpen() && !gSettings.IsOpen() && (m_record.m_Gold_rate != -1))
		val = m_record.m_Gold_rate;
	else if(IsOpen() && gSettings.IsOpen())
	{
		if(m_record.m_Gold_rate == -1)
			val = gSettings.m_Gold_rate;
		else
			val = m_record.m_Gold_rate;
	}

	return val;
}


BOOL CTableVoucher::AddSet(CRecordSet new_set)
{
	if(!m_sets.DBAddEntry())
		return FALSE;

	m_sets.m_record = new_set;
	m_sets.m_record.m_Voucher_ID = m_record.m_Voucher_ID;

	if(!m_sets.DBUpdateEntry("AddSet"))
		return FALSE;

	if(!DBEditEntry())
		return FALSE;

	m_record.m_Set_count++;

	if(!DBUpdateEntry("AddSet"))
		return FALSE;

	return (m_sets.DBRequeryEntries("AddSet", TRUE) == DBOK);
}


BOOL CTableVoucher::DeleteSet(CString set_ID)
{
	if(DB_IS_EMPTY(m_sets))
		return FALSE;

	CString current_ID = m_sets.m_record.m_Set_ID;
	if(current_ID == set_ID)
		current_ID = "";

	if(m_sets.GotoSet(set_ID) != DBOK)
		return FALSE;

	if(!m_sets.DBDeleteEntry())
		return FALSE;

	if(!DBEditEntry())
		return FALSE;

	m_record.m_Set_count--;

	if(!DBUpdateEntry("DeleteSet"))
		return FALSE;

	DatabaseState state = m_sets.DBRequeryEntries("DeleteSet", FALSE);

	if(state == DBInvalid)
		return FALSE;
	else if(state = DBEmpty)
		return TRUE;

	if(current_ID.IsEmpty())
		return (m_sets.GotoFirstSet() == DBOK);
	else
		return (m_sets.GotoSet(current_ID) == DBOK);
}


BOOL CTableVoucher::DeleteEmptySets()
{
	DatabaseState state = m_sets.GotoFirstSet();
	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		if(m_sets.GetItemCount(Both, FALSE) == 0)
		{
			if(!m_sets.DBDeleteEntry())
				return FALSE;
		}

		state = m_sets.GotoNextSet();
	}

	GetSetCount(TRUE);

	return TRUE;
}


BOOL CTableVoucher::GetSetList(CString productID, CList<CString, CString> *setlist, CList<CString, CString> *voucherlist)
{
	if(!setlist || productID.IsEmpty())
		return FALSE;
	
	CString current_ID = m_sets.m_record.m_Set_ID;

	DatabaseState state = m_sets.GotoFirstSet();

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		CString curID = m_sets.m_record.m_Set_ID;
		if(productID.CompareNoCase(CTableDescription::ExtractActualID(curID)) == 0)
		{
			setlist->AddTail(curID);
			if(voucherlist)
				voucherlist->AddTail(m_sets.m_record.m_Voucher_ID);
		}

		state = m_sets.GotoNextSet();
	}

	if(!m_sets.IsEOF())
		return FALSE;

	return (m_sets.GotoSet(current_ID) == DBOK);
}


int CTableVoucher::GetSetCount(BOOL force_requery)
{
	if(!force_requery && (m_record.m_Set_count > 0) && (m_record.m_Set_count < MAX_LONG))
		return m_record.m_Set_count;

	if(DB_IS_EMPTY(m_sets))
	{
		if(!DBEditEntry())
			return 0;

		m_record.m_Set_count = 0;

		DBUpdateEntry("GetSetCount");
		return 0;
	}

	int count = 0;
	CString current_ID = m_sets.m_record.m_Set_ID;

	DatabaseState state = m_sets.GotoFirstSet(FALSE);
	if(state != DBOK)
		return count;

	while(state == DBOK)
	{
		count ++;
		state = m_sets.GotoNextSet(FALSE);
	}

	if(current_ID.IsEmpty())
	{
		if(m_sets.GotoFirstSet(FALSE) != DBOK)
			return 0;
	}
	else
	{
		if(m_sets.GotoSet(current_ID, FALSE) != DBOK)
			return 0;
	}

	if(!DBEditEntry())
		return count;

	m_record.m_Set_count = count;

	DBUpdateEntry("GetSetCount");
	return count;
}


BOOL CTableVoucher::IsRootVoucher()
{
	return ((m_record.m_Voucher_ID == STOCK_MAIN) ||
			(m_record.m_Voucher_ID == EXPORT_MAIN) ||
			(m_record.m_Voucher_ID == KUNDAN_MAIN));
}


BOOL CTableVoucher::AssignSerialNumbers(int *starting)
{
	CString current_set = m_sets.m_record.m_Set_ID;

	CList<CString, CString> sorted_sets;
	CList<int, int> sorted_serials;

	DatabaseState state = m_sets.GotoFirstSet();

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		//We will only check for the serial number of the first item.
		CTableItem *item = &(m_sets.m_items);
		int serial = item->GetOldSerial("");
		if(serial == -1)
		{
			sorted_sets.AddTail(m_sets.m_record.m_Set_ID);
			sorted_serials.AddTail(serial);
		}
		else
		{
			CString setID = m_sets.m_record.m_Set_ID;

			POSITION pos1 = sorted_sets.GetHeadPosition();
			POSITION pos2 = sorted_serials.GetHeadPosition();

			while(pos1 && pos2)
			{
				int tempserial = sorted_serials.GetAt(pos2);
				if(tempserial < serial)
					break;

				sorted_sets.GetNext(pos1);
				sorted_serials.GetNext(pos2);
			}

			if(pos1 && pos2)
			{
				sorted_sets.InsertBefore(pos1, setID);
				sorted_serials.InsertBefore(pos2, serial);
			}
			else
			{
				sorted_sets.AddTail(setID);
				sorted_serials.AddTail(serial);
			}
		}

		state = m_sets.GotoNextSet();
	}

	if(!m_sets.IsEOF())
		return FALSE;

	POSITION pos1 = sorted_sets.GetHeadPosition();
	POSITION pos2 = sorted_serials.GetHeadPosition();

	while(pos1 && pos2)
	{
		CString setID = sorted_sets.GetAt(pos1);

		if(m_sets.GotoSet(setID) != DBOK)
			return FALSE;

		if(!m_sets.UpdateItemSerials(starting))
			return FALSE;

		sorted_sets.GetNext(pos1);
		sorted_serials.GetNext(pos2);
	}

	return (m_sets.GotoSet(current_set) == DBOK);
}

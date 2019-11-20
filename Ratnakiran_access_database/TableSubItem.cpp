#include "stdafx.h"
#include "Ratnakiran.h"
#include "TableSubItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTableSubItem, CRecordset)

CTableSubItem::CTableSubItem(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CTableSubItem)
	//}}AFX_FIELD_INIT
	m_nFields = 7;
	m_nDefaultType = snapshot;
	m_strSort = "Category";
}


CString CTableSubItem::GetDefaultConnect()
{
	return _T("ODBC;DSN=Ratnakiran");
}

CString CTableSubItem::GetDefaultSQL()
{
	return _T("[SubItems]");
}

void CTableSubItem::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTableSubItem)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Identifier]"), m_record.m_Identifier);
	RFX_Long(pFX, _T("[Item_identifier]"), m_record.m_Item_identifier);
	RFX_Text(pFX, _T("[Item_ID]"), m_record.m_Item_ID);
	RFX_Text(pFX, _T("[Category]"), m_record.m_Category);
	RFX_Text(pFX, _T("[Type]"), m_record.m_Type);
	RFX_Double(pFX, _T("[Weight]"), m_record.m_Weight);
	RFX_Long(pFX, _T("[Rate]"), m_record.m_Rate);
	//}}AFX_FIELD_MAP
}

#ifdef _DEBUG
void CTableSubItem::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTableSubItem::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

BOOL CTableSubItem::DBOpen(CString in_func, BOOL open_children)
{
	if(!IsOpen())
	{
		CString errstr = "Error while Open of sub-items table";
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


BOOL CTableSubItem::DBClose(BOOL close_children)
{
	if(IsOpen())
		Close();

	return TRUE;
}


BOOL CTableSubItem::DBBeginTransaction(BOOL include_children)
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

	if(status && !GotoSubItem(current, FALSE))
		status=FALSE;

	return status;
}


BOOL CTableSubItem::DBRollbackTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	long current = m_record.m_Identifier;
	m_pDatabase->Rollback();

	BOOL status=TRUE;
	m_strFilter = queryString;

	if(DBRequeryEntries("DBRollbackTransaction", FALSE) == DBInvalid)
		status=FALSE;

	if(status && !GotoSubItem(current, FALSE))
		status=FALSE;

	return status;
}


BOOL CTableSubItem::DBCommitTransaction(BOOL include_children)
{
	CString queryString = m_strFilter;
	long current = m_record.m_Identifier;
	m_pDatabase->CommitTrans();

	BOOL status=TRUE;
	m_strFilter = queryString;

	if(DBRequeryEntries("DBCommitTransaction", FALSE) == DBInvalid)
		status=FALSE;

	if(status && !GotoSubItem(current, FALSE))
		status=FALSE;

	return status;
}

	
BOOL CTableSubItem::DBAddEntry()
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


BOOL CTableSubItem::DBEditEntry()
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


BOOL CTableSubItem::DBUpdateEntry(CString in_func)
{
	CString errstr = "Error while Update of sub-items table";
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


BOOL CTableSubItem::DBDeleteEntry()
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


DatabaseState CTableSubItem::DBRequeryEntries(CString in_func, BOOL preserve_state)
{
	if(!IsOpen())
		return DBInvalid;

	CString errstr = "Error while Requery of sub-items table";
	if(!in_func.IsEmpty())
		errstr = errstr + " in " + in_func;

	long current_sub_item = m_record.m_Identifier;
		
	DB_PERFORM_RET(
	(Requery()),
	errstr,
	{return DBInvalid;},
	{});

	if(preserve_state && (current_sub_item != 0) && (current_sub_item != MAX_LONG))
		return GotoSubItem(current_sub_item);
	else
		return GotoFirstSubItem();
}


DatabaseState CTableSubItem::GotoFirstSubItem(BOOL requery)
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	DB_PERFORM(
	(MoveFirst()),
	{return DBInvalid;},
	{});

	return DBOK;
}


DatabaseState CTableSubItem::GotoNextSubItem(BOOL requery)
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

	return DBOK;
}


DatabaseState CTableSubItem::GotoPrevSubItem(BOOL requery)
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

	return DBOK;
}


DatabaseState CTableSubItem::GotoLastSubItem(BOOL requery)
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	DB_PERFORM(
	(MoveLast()),
	{return DBInvalid;},
	{});

	return DBOK;
}


DatabaseState CTableSubItem::GotoPosSubItem(int pos, BOOL requery)
{
	DatabaseState state = GotoFirstSubItem(FALSE);
	if(state != DBOK)
		return state;

	while(pos > 0)
	{
		state = GotoNextSubItem(FALSE);
		if(state != DBOK)
			return state;

		pos--;
	}

	return DBOK;
}


DatabaseState CTableSubItem::GotoSubItem(long identifier, BOOL requery)
{
	DatabaseState state = GotoFirstSubItem(FALSE);
	if(state != DBOK)
		return state;

	while(m_record.m_Identifier != identifier)
	{
		state = GotoNextSubItem(FALSE);
		if(state != DBOK)
			return state;
	}

	return DBOK;
}

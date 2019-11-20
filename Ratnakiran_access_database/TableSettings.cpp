#include "stdafx.h"
#include "Ratnakiran.h"
#include "TableSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTableSettings, CRecordset)

CTableSettings::CTableSettings(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CTableSettings)
	m_ID = 0;
	m_Gold_rate = 0.0;
	m_Use_native_currency = FALSE;
	m_nFields = 3;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CTableSettings::GetDefaultConnect()
{
	return _T("ODBC;DSN=Ratnakiran");
}

CString CTableSettings::GetDefaultSQL()
{
	return _T("[Settings]");
}

void CTableSettings::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTableSettings)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Double(pFX, _T("[Gold_rate]"), m_Gold_rate);
	RFX_Bool(pFX, _T("[Use_native_currency]"), m_Use_native_currency);
	//}}AFX_FIELD_MAP
}

#ifdef _DEBUG
void CTableSettings::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTableSettings::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

BOOL CTableSettings::EditEntry()
{
	if(!IsOpen())
		return FALSE;

	if(!CanUpdate())
		return FALSE;

	DB_PERFORM(
	(Edit()),
	{CancelUpdate(); return FALSE;},
	{return TRUE;});
}


BOOL CTableSettings::UpdateEntry(CString in_func)
{
	CString errstr = "Error while Update of settings table";
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
	{return TRUE;});
}

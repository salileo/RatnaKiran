#include "stdafx.h"
#include "Ratnakiran.h"
#include "TableDescription.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTableDescription, CRecordset)

CTableDescription::CTableDescription(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CTableDescription)
	m_Description = _T("");
	m_Prefix = _T("");
	m_Piece_count = 0;
	m_nFields = 3;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CTableDescription::GetDefaultConnect()
{
	return _T("ODBC;DSN=Ratnakiran");
}

CString CTableDescription::GetDefaultSQL()
{
	return _T("[Description]");
}

void CTableDescription::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTableDescription)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Text(pFX, _T("[Prefix]"), m_Prefix);
	RFX_Int(pFX, _T("[Piece_count]"), m_Piece_count);
	//}}AFX_FIELD_MAP
}


#ifdef _DEBUG
void CTableDescription::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTableDescription::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


BOOL CTableDescription::AddEntry()
{
	if(!IsOpen())
		return FALSE;

	if(!CanUpdate())
		return FALSE;

	DB_PERFORM(
	(AddNew()),
	{CancelUpdate(); return FALSE;},
	{return TRUE;});
}


BOOL CTableDescription::EditEntry()
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


BOOL CTableDescription::UpdateEntry(CString in_func)
{
	CString errstr = "Error while Update of description table";
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


BOOL CTableDescription::DeleteEntry()
{
	if(!IsOpen())
		return FALSE;

	if(!CanUpdate())
		return FALSE;

	DB_PERFORM(
	(Delete()),
	{return FALSE;},
	{return TRUE;});
}


DatabaseState CTableDescription::RequeryEntry(CString in_func, BOOL preserve_state)
{
	if(!IsOpen())
		return DBInvalid;

	CString errstr = "Error while Requery of description table";
	if(!in_func.IsEmpty())
		errstr = errstr + " in " + in_func;

	CString current_desc = m_Description;

	DB_PERFORM_RET(
	(Requery()),
	errstr,
	{return DBInvalid;},
	{});

	if(preserve_state)
		return GotoEntry(current_desc);
	else
		return GotoFirstEntry();
}


DatabaseState CTableDescription::GotoFirstEntry()
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	DB_PERFORM(
	(MoveFirst()),
	{return DBInvalid;},
	{return DBOK;});
}


DatabaseState CTableDescription::GotoNextEntry()
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


DatabaseState CTableDescription::GotoPrevEntry()
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


DatabaseState CTableDescription::GotoLastEntry()
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	DB_PERFORM(
	(MoveLast()),
	{return DBInvalid;},
	{return DBOK;});
}


DatabaseState CTableDescription::GotoEntry(CString desc)
{
	if(!IsOpen())
		return DBInvalid;

	if(DB_EMPTY)
		return DBEmpty;

	DB_PERFORM(
	(MoveFirst()),
	{return DBInvalid;},
	{});

	while(desc.CompareNoCase(m_Description) != 0)
	{
		DB_PERFORM(
		(MoveNext()),
		{return DBInvalid;},
		{});
		
		if(IsEOF())
			return DBInvalid;
	}
	
	return DBOK;
}


DatabaseState CTableDescription::GotoPosEntry(int pos)
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

	return DBOK;
}


BOOL CTableDescription::AddDescription(CString desc, CString prefix, int piece_count)
{
	if(!AddEntry())
		return FALSE;

	m_Description = desc;
	m_Prefix = prefix;
	m_Piece_count = piece_count;

	if(!UpdateEntry("AddDescription"))
		return FALSE;

	if(RequeryEntry("AddDescription") != DBOK)
		return FALSE;

	return TRUE;
}


BOOL CTableDescription::DeleteAll()
{
	DatabaseState state = GotoFirstEntry();

	if(state == DBInvalid)
		return FALSE;
	else if (state == DBEmpty)
		return TRUE;

	while(state == DBOK)
	{
		if(!DeleteEntry())
			return FALSE;

		state = GotoNextEntry();
	}
		
	if(!IsEOF())
		return FALSE;

	return TRUE;
}


CString CTableDescription::GetPrefix(CString description)
{
	CString prefix;
	if(description.IsEmpty())
		return prefix;

	if(GotoEntry(description) == DBOK)
		prefix = m_Prefix;
	else
	{
		for(;;)
		{
			CString specialtype;
			CString temp;

			specialtype = "Necklace Set";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "NS";
					break;
				}
			}

			specialtype = "NecklaceSet";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "NS";
					break;
				}
			}

			specialtype = "Necklace";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "N";
					break;
				}
			}

			specialtype = "Bangles";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "B";
					break;
				}
			}

			specialtype = "Bangle";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "Bn";
					break;
				}
			}

			specialtype = "Bracelet";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "Br";
					break;
				}
			}

			specialtype = "Button";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "Bt";
					break;
				}
			}

			specialtype = "Cufflink";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "Cl";
					break;
				}
			}

			specialtype = "Earring";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "Er";
					break;
				}
			}

			specialtype = "Pendant Set";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "PS";
					break;
				}
			}

			specialtype = "PendantSet";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "PS";
					break;
				}
			}

			specialtype = "Pendant";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "P";
					break;
				}
			}

			specialtype = "Ring";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "R";
					break;
				}
			}

			specialtype = "Nose Pin";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "Np";
					break;
				}
			}

			specialtype = "NosePin";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "Np";
					break;
				}
			}

			specialtype = "Brooch";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					prefix = "Bh";
					break;
				}
			}

			break;
		}
	}

	if(!prefix.IsEmpty())
		prefix = prefix + "_";

	return prefix;
}


int CTableDescription::GetPieceCount(CString description)
{
	int value = 0;
	if(description.IsEmpty())
		return value;

	if(GotoEntry(description) == DBOK)
		value = m_Piece_count;
	else
	{
		for(;;)
		{
			CString specialtype;
			CString temp;

			specialtype = "Necklace Set";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 0;
					break;
				}
			}

			specialtype = "NecklaceSet";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 0;
					break;
				}
			}

			specialtype = "Necklace";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 1;
					break;
				}
			}

			specialtype = "Bangles";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 2;
					break;
				}
			}

			specialtype = "Bangle";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 1;
					break;
				}
			}

			specialtype = "Bracelet";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 1;
					break;
				}
			}

			specialtype = "Button";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 1;
					break;
				}
			}

			specialtype = "Cufflink";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 2;
					break;
				}
			}

			specialtype = "Earring";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 2;
					break;
				}
			}

			specialtype = "Pendant Set";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 0;
					break;
				}
			}

			specialtype = "PendantSet";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 0;
					break;
				}
			}

			specialtype = "Pendant";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 1;
					break;
				}
			}

			specialtype = "Ring";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 1;
					break;
				}
			}

			specialtype = "Nose Pin";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 1;
					break;
				}
			}

			specialtype = "NosePin";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 1;
					break;
				}
			}

			specialtype = "Brooch";
			if(description.GetLength() >= specialtype.GetLength())
			{
				temp = description.Left(specialtype.GetLength());
				if(temp.CompareNoCase(specialtype) == 0)
				{
					value = 1;
					break;
				}
			}

			break;
		}
	}

	return value;
}


CString CTableDescription::ExtractActualID(CString ID)
{
	CString finalid = ID;

	int index = ID.ReverseFind('_');
	if(index >= 0)
		finalid = ID.Right(ID.GetLength() - index - 1);

	return finalid;
}


CString CTableDescription::ExtractPrefix(CString ID)
{
	CString finalid;

	int index = ID.ReverseFind('_');
	if(index >= 0)
		finalid = ID.Left(index + 1);

	return finalid;
}

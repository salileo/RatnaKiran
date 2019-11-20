#include "DatabaseGlobals.h"
#include "Settings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static CSettings gSettings;

CSettings::CSettings()
{
	m_GoldRate = 0.0;
	m_UseNativeCurrency = TRUE;
	SetDefaultCategories();
}

CSettings::~CSettings()
{
}

void CSettings::SetDefaultCategories()
{
	m_CategoriesString = "";
	m_CategoriesString += "Necklace Set|NS|0||";
	m_CategoriesString += "NecklaceSet|NS|0||";
	m_CategoriesString += "Necklace|N|1||";
	m_CategoriesString += "Bangles|B|2||";
	m_CategoriesString += "Bangle|Bn|1||";
	m_CategoriesString += "Bracelet|Br|1||";
	m_CategoriesString += "Button|Bt|1||";
	m_CategoriesString += "Cufflink|Cl|2||";
	m_CategoriesString += "Earring|Er|2||";
	m_CategoriesString += "Pendant Set|PS|0||";
	m_CategoriesString += "PendantSet|PS|0||";
	m_CategoriesString += "Pendant|P|1||";
	m_CategoriesString += "Ring|R|1||";
	m_CategoriesString += "Nose Pin|Np|1||";
	m_CategoriesString += "NosePin|Np|1||";
	m_CategoriesString += "Brooch|Bh|1||";

	m_Categories.RemoveAll();
	ConvertStringToList();
}

BOOL CSettings::WriteSettings()
{
	CString filename = gFilePath + "\\settings.rsf";
	FILE *outfile = fopen(LPCTSTR(filename), "w");
	if(outfile)
	{
		ConvertListToString();
		fprintf(outfile, "UseNativeCurrency=%d\n", m_UseNativeCurrency);
		fprintf(outfile, "GoldRate=%.3f\n", m_GoldRate);
		fprintf(outfile, "Cat_len=%d\n", m_CategoriesString.GetLength());
		fprintf(outfile, "Categories=%s\n", LPCTSTR(m_CategoriesString));
		fclose(outfile);
		return TRUE;
	}
	else
	{
		CString errstr = "Could not open settings file '" + filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}
}

BOOL CSettings::ReadSettings()
{
	BOOL problem_in_reading = FALSE;

	CString filename = gFilePath + "\\settings.rsf";
	FILE *infile = fopen(LPCTSTR(filename), "r");
	if(!infile)
	{
		if(IsFileExisting(filename))
		{
			CString errstr = "Could not open settings file '" + filename + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
		else
		{
			//create new settings file
			m_GoldRate = 0.0;
			m_UseNativeCurrency = TRUE;
			SetDefaultCategories();

			if(WriteSettings())
			{
				m_SettingInitialized = TRUE;
				return TRUE;
			}
			else
			{
				SHOW_INTERNAL_ERROR_REASON("Unable to update stored settings.");
				return FALSE;
			}
		}
	}

	int length=0;
	if(fscanf(infile, "UseNativeCurrency=%d\n", &m_UseNativeCurrency) != 1)
		problem_in_reading = TRUE;

	{
		char temp[1024];
		if(fscanf(infile, "GoldRate=%s\n", temp) != 1)
			problem_in_reading = TRUE;
		else
			m_GoldRate = atof(temp);
	}

	if(fscanf(infile, "Cat_len=%d\n", &length) != 1)
		problem_in_reading = TRUE;
	else
	{
		char *temp = NULL;
		SALIL_NEW(temp, char[length + 1]);
		if(fscanf(infile, "Categories=%[^\n]", temp) != 1)
			problem_in_reading = TRUE;
		else
		{
			m_CategoriesString = temp;

			if(m_CategoriesString.GetLength() != length)
				problem_in_reading = TRUE;
			else
				ConvertStringToList();
		}

		delete temp;
	}

	fclose(infile);

	if(problem_in_reading)
	{
		if(!WriteSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to update stored settings.");
			return FALSE;
		}
		else
		{
			SHOW_ERROR("There was some problem in the values read from settings file. This was fixed, but some of the settings might be affected.");
			m_SettingInitialized = TRUE;
			return TRUE;
		}
	}
	else
	{
		m_SettingInitialized = TRUE;
		return TRUE;
	}
}

void CSettings::ConvertListToString()
{
	CString value;
	POSITION pos = m_Categories.GetHeadPosition();
	while(pos)
	{
		CDescriptionName desc = m_Categories.GetAt(pos);

		CString number;
		number.Format("%d",desc.m_number_of_pieces);
		value = value + desc.m_description + "|" + desc.m_prefix + "|" + number + "||";
		m_Categories.GetNext(pos);
	}

	m_CategoriesString = value;
}

void CSettings::ConvertStringToList()
{
	CString value = m_CategoriesString;
	m_Categories.RemoveAll();

	while(value.GetLength() > 0)
	{
		int pos = value.Find("|");
		if(pos != -1)
		{
			if(pos == (value.GetLength() - 1))
				break;

			if(value[pos + 1] == '|')
			{
				value = value.Right(value.GetLength() - pos - 2);
				continue;
			}

			CString name = value.Left(pos);
			value = value.Right(value.GetLength() - pos - 1);

			pos = value.Find("|");
			if(pos != -1)
			{
				if(pos == (value.GetLength() - 1))
					break;

				if(value[pos + 1] == '|')
				{
					value = value.Right(value.GetLength() - pos - 2);
					continue;
				}

				CString prefix = value.Left(pos);
				CString number = value.Right(value.GetLength() - pos - 1);

				pos = number.Find("||");
				if(pos == -1)
				{
					value = "";
				}
				else
				{
					value = number.Right(number.GetLength() - pos - 2);
					number = number.Left(pos);
				}

				CDescriptionName desc;
				desc.m_description = name;
				desc.m_prefix = prefix;
				desc.m_number_of_pieces = atoi(LPCTSTR(number));

				m_Categories.AddTail(desc);
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}

BOOL CSettings::AddCategory(CString description, CString prefix, int number_of_pieces)
{
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return FALSE;
		}
	}

	if(description.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CDescriptionName newnode;
	newnode.m_description = description;
	newnode.m_prefix = prefix;
	newnode.m_number_of_pieces = number_of_pieces;

	gSettings.m_Categories.AddTail(newnode);

	if(!gSettings.WriteSettings())
	{
		//restore state
		gSettings.m_Categories.RemoveTail();
		SHOW_INTERNAL_ERROR_REASON("Unable to update stored settings.");
		return FALSE;
	}

	return TRUE;
}

BOOL CSettings::RemoveCategory(CString description)
{
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return FALSE;
		}
	}

	if(description.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	POSITION pos = gSettings.m_Categories.GetHeadPosition();
	CDescriptionName entry;
	while(pos)
	{
		entry = gSettings.m_Categories.GetAt(pos);
		if(description.CompareNoCase(entry.m_description) == 0)
		{
			gSettings.m_Categories.RemoveAt(pos);
			break;
		}

		gSettings.m_Categories.GetNext(pos);
	}

	if(!pos)
	{
		CString errstr = "Could not find category '" + description + "' in the list.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!gSettings.WriteSettings())
	{
		//restore state
		gSettings.m_Categories.AddTail(entry);
		SHOW_INTERNAL_ERROR_REASON("Unable to update stored settings.");
		return FALSE;
	}
	else
		return TRUE;

	return FALSE;
}

BOOL CSettings::RemoveAllCategories()
{
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return FALSE;
		}
	}

	CList<CDescriptionName, CDescriptionName> temp_list;
	while(!gSettings.m_Categories.IsEmpty())
	{
		CDescriptionName entry = gSettings.m_Categories.RemoveHead();
		temp_list.AddTail(entry);
	}

	if(!gSettings.WriteSettings())
	{
		//restore state
		while(!temp_list.IsEmpty())
		{
			CDescriptionName entry = temp_list.RemoveHead();
			gSettings.m_Categories.AddTail(entry);
		}

		SHOW_INTERNAL_ERROR_REASON("Unable to update stored settings.");
		return FALSE;
	}

	return TRUE;
}

int CSettings::GetCategoryCount()
{
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return 0;
		}
	}

	return gSettings.m_Categories.GetCount();
}


BOOL CSettings::GetCategory(int index, CString *description, CString *prefix, int *piece_count)
{
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return FALSE;
		}
	}

	POSITION pos = gSettings.m_Categories.FindIndex(index);
	if(pos)
	{
		CDescriptionName curnode = gSettings.m_Categories.GetAt(pos);
		if(description)
			*description = curnode.m_description;

		if(prefix)
			*prefix = curnode.m_prefix;

		if(piece_count)
			*piece_count = curnode.m_number_of_pieces;

		return TRUE;
	}

	return FALSE;
}

#if 0
void CSettings::EnumCategories(EnumCategoriesFunc proc, void *clientData)
{
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return;
		}
	}

	POSITION pos = gSettings.m_Categories.GetHeadPosition();
	while(pos)
	{
		CDescriptionName curnode = gSettings.m_Categories.GetAt(pos);
		BOOL cont = proc(curnode.m_description, curnode.m_prefix, curnode.m_number_of_pieces, clientData);
		if(!cont)
			return;
		gSettings.m_Categories.GetNext(pos);
	}
}
#endif

int CSettings::GetPieceCountFor(CString description)
{
	int value = 0;
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return value;
		}
	}

	if(description.IsEmpty())
		return value;

	POSITION pos = gSettings.m_Categories.GetHeadPosition();
	while(pos)
	{
		CDescriptionName desc = gSettings.m_Categories.GetAt(pos);
		if(description.CompareNoCase(desc.m_description) == 0)
		{
			value = desc.m_number_of_pieces;
			break;
		}

		gSettings.m_Categories.GetNext(pos);
	}

	if(!pos)
	{
		pos = gSettings.m_Categories.GetHeadPosition();
		while(pos)
		{
			CDescriptionName desc = gSettings.m_Categories.GetAt(pos);
			if(description.GetLength() >= desc.m_description.GetLength())
			{
				CString temp = description.Left(desc.m_description.GetLength());
				if(temp.CompareNoCase(desc.m_description) == 0)
				{
					value = desc.m_number_of_pieces;
					break;
				}
			}

			gSettings.m_Categories.GetNext(pos);
		}
	}

	return value;
}

CString CSettings::GetPrefixFor(CString description)
{
	CString prefix;
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return prefix;
		}
	}

	if(description.IsEmpty())
		return prefix;

	prefix = "_DOES_NOT_EXIST_";
	POSITION pos = gSettings.m_Categories.GetHeadPosition();
	while(pos)
	{
		CDescriptionName desc = gSettings.m_Categories.GetAt(pos);
		if(description.CompareNoCase(desc.m_description) == 0)
		{
			prefix = desc.m_prefix;
			break;
		}

		gSettings.m_Categories.GetNext(pos);
	}

	if(!pos)
	{
		pos = gSettings.m_Categories.GetHeadPosition();
		while(pos)
		{
			CDescriptionName desc = gSettings.m_Categories.GetAt(pos);
			if(description.GetLength() >= desc.m_description.GetLength())
			{
				CString temp = description.Left(desc.m_description.GetLength());
				if(temp.CompareNoCase(desc.m_description) == 0)
				{
					prefix = desc.m_prefix;
					break;
				}
			}

			gSettings.m_Categories.GetNext(pos);
		}
	}

	if(!prefix.IsEmpty() && (prefix != CString("_DOES_NOT_EXIST_")))
		prefix = prefix + "_";

	return prefix;
}

double CSettings::GetGoldRate()
{
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return 0.0;
		}
	}

	return gSettings.m_GoldRate;
}

BOOL CSettings::SetGoldRate(double val)
{
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return FALSE;
		}
	}

	double original_value = gSettings.m_GoldRate;
	gSettings.m_GoldRate = val;

	if(!gSettings.WriteSettings())
	{
		//restore state
		gSettings.m_GoldRate = original_value;
		SHOW_INTERNAL_ERROR_REASON("Unable to update stored settings.");
		return FALSE;
	}

	return TRUE;
}

BOOL CSettings::GetUseNativeCurrency()
{
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return TRUE;
		}
	}

	return gSettings.m_UseNativeCurrency;
}

BOOL CSettings::SetUseNativeCurrency(BOOL val)
{
	if(!gSettings.m_SettingInitialized)
	{
		if(!gSettings.ReadSettings())
		{
			SHOW_INTERNAL_ERROR_REASON("Unable to read stored settings.");
			return FALSE;
		}
	}

	BOOL original_value = gSettings.m_UseNativeCurrency;
	gSettings.m_UseNativeCurrency = val;

	if(!gSettings.WriteSettings())
	{
		//restore state
		gSettings.m_UseNativeCurrency = original_value;
		SHOW_INTERNAL_ERROR_REASON("Unable to update stored settings.");
		return FALSE;
	}

	return TRUE;
}



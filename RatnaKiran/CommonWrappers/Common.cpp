#include "Common.h"

BOOL gShowErrors;

CString GetBaseName(CString file)
{
	int slashpos;
	int length = file.GetLength();
	slashpos = file.ReverseFind('\\');
	if(slashpos != -1)
	{
		length = length - slashpos - 1;
		file = file.Right(length);
	}

	return file;
}

CString GetDirName(CString file)
{
	int slashpos;
	slashpos = file.ReverseFind('\\');
	if(slashpos != -1)
		file = file.Left(slashpos);
	else
		file = "";

	return file;
}

BOOL IsFileExisting(CString filename)
{
	BOOL retval = FALSE;

	if(filename.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return retval;
	}

	HANDLE listhandle;
	LPWIN32_FIND_DATA infostruct = NULL;
	SALIL_NEW(infostruct, WIN32_FIND_DATA);

	listhandle = FindFirstFile(filename,infostruct);
	if(listhandle == INVALID_HANDLE_VALUE)
		retval = FALSE;
	else
	{
		retval = TRUE;

		if(!FindClose(listhandle))
			SHOW_INTERNAL_ERROR_LAST("Problem in closing search context.");
	}

	delete infostruct;
	return retval;
}

BOOL MoveDatabaseFile(CString from, CString to)
{
	if(from.IsEmpty() || to.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!IsFileExisting(from))
	{
		CString errstr = "Could not move file '" + from + "', as it does not exist.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!MoveFileEx(from, to, MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
	{
		CString errstr = "Could not move file '" + from + "' to '" + to + "'.";
		SHOW_INTERNAL_ERROR_LAST(errstr);
		return FALSE;
	}

	return TRUE;
}


#if 0
BOOL MoveDatabaseFile(CString from, CString to) //both need to be fullpaths
{
	if(from.IsEmpty() || to.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!IsFileExisting(from))
	{
		CString errstr = "Could not move file '" + from + "', as it does not exist.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(IsFileExisting(to))
	{
		CString temp_filename = gFilePath + "\\_temp_converter_file_2.tmp";
		BOOL error = FALSE;

		try
		{
			CFile::Rename(LPCTSTR(to), LPCTSTR(temp_filename));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.",readerror->m_cause);
			errstr = "Could not rename file '" + to + "' to '" + temp_filename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}

		try
		{
			CFile::Rename(LPCTSTR(from), LPCTSTR(to));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.",readerror->m_cause);
			errstr = "Could not rename file '" + from + "' to '" + to + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			error = TRUE;
		}

		if(error)
		{
			try
			{
				CFile::Rename(LPCTSTR(temp_filename), LPCTSTR(to));
			}
			catch(CFileException *readerror)
			{
				CString errstr;
				errstr.Format("ERROR : #%d.",readerror->m_cause);
				errstr = "Could not rename file '" + temp_filename + "' to '" + to + "'.\n" + errstr;
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}

			return FALSE;
		}

		try
		{
			CFile::Remove(LPCTSTR(temp_filename));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.",readerror->m_cause);
			errstr = "Could not delete file '" + temp_filename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}
	else
	{
		try
		{
			CFile::Rename(LPCTSTR(from), LPCTSTR(to));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.",readerror->m_cause);
			errstr = "Could not rename file '" + from + "' to '" + to + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	return TRUE;
}
#endif


CString GetFileName(char *filtername, char *filtervalue, char *defaultextension, BOOL forOpen)
{
	OPENFILENAME OpenFileName;

	//buffer for storing the file name
	char szFile[MAX_PATH] = "\0";
    strcpy(szFile,"");

	char *filter = NULL;

	if(!filtername || !filtervalue)
	{
		filtername = "All Files (*.*)";
		filtervalue = "*.*";
	}

	//create a NULL character seperated strings for defining the filters
	int size = strlen(filtername) + strlen(filtervalue) + 3;
	SALIL_NEW(filter, char[size]);

	strcpy(&(filter[0]),filtername);
	filter[strlen(filtername)] = '\0';

	strcpy(&(filter[strlen(filtername) + 1]),filtervalue);
	filter[size - 2] = '\0';
	filter[size - 1] = '\0';

	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = NULL;
    OpenFileName.hInstance         = NULL;
    OpenFileName.lpstrFilter       = filter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 1;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = MAX_PATH;
    OpenFileName.lpstrFileTitle    = NULL;
    OpenFileName.nMaxFileTitle     = 0;
    OpenFileName.lpstrInitialDir   = NULL;
	OpenFileName.lpstrTitle        = "Select File";
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = defaultextension;
    OpenFileName.lCustData         = NULL;
	OpenFileName.lpfnHook 		   = NULL;
	OpenFileName.lpTemplateName    = NULL;
    OpenFileName.Flags             = OFN_LONGNAMES | OFN_SHOWHELP | OFN_EXPLORER;
		

	CString filename = "";
	if(forOpen)
	{
		OpenFileName.Flags |=  OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetOpenFileName(&OpenFileName))
			filename = OpenFileName.lpstrFile;
	}
	else
	{
		OpenFileName.Flags |=  OFN_OVERWRITEPROMPT;

		if(GetSaveFileName(&OpenFileName))
			filename = OpenFileName.lpstrFile;
	}

	if(filter)
		delete filter;
	
	return filename;
}


BOOL IsNumber(CString str)
{
	str.Remove('0');
	str.Remove('1');
	str.Remove('2');
	str.Remove('3');
	str.Remove('4');
	str.Remove('5');
	str.Remove('6');
	str.Remove('7');
	str.Remove('8');
	str.Remove('9');

	if(str.GetLength() > 0)
		return FALSE;

	return TRUE;
}


BOOL IsGreaterString(CString str1, CString str2)
{
	int val = strcmp(LPCTSTR(str1), LPCTSTR(str2));
	if(val > 0)
		return TRUE;

	return FALSE;
}

//This functions checks whether a given character is an alphabet.
//If the character is an alphabet, the function returns the numeric value of the alphabet.
//If the character is not an alphabet, it returns -1.
static int GetCharacterNumber(char ch)
{
	int val;

	if(ch >= 'a' && ch <= 'z')
		val = ch - 'a';
	else if(ch >= 'A' && ch <= 'Z')
		val = ch - 'A';
	else
		val = -1;

	return val;
}


int FindMenuItem(CMenu* menu, CString menustring)
{
	if(menu == NULL)
	{
		SHOW_INTERNAL_ERROR;
		return -1;
	}

   int count = menu->GetMenuItemCount();
   for (int i = 0; i < count; i++)
   {
      CString str;
      if(menu->GetMenuString(i, str, MF_BYPOSITION))
	  {
		  if(str == menustring)
			  return i;
	  }
   }

   return -1;
}


double RoundOff(double val, int decimal_places)
{
	int tempval = (int)((val * pow(10.0, (double)decimal_places)) + 0.5);
	val = ((double)tempval)/ pow(10.0, (double)decimal_places);
	return val;
}

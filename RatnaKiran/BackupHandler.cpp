#include "stdafx.h"
#include "ratnakiran.h"
#include "DatabaseGlobals.h"
#include "BackupHandler.h"
#if 0
#include "../ZipArchive/ZipArchive/ZipArchive.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBackupHandler::CBackupHandler()
{

}


CBackupHandler::~CBackupHandler()
{

}

#if 0
static BOOL AddMainFolder(CZipArchive *archive, CProgressDialog *progress)
{
	if(!archive || !progress)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CString prefix, suffix, path;
	prefix = gFilePath + "\\";
	path = prefix + "*.*";
	
	HANDLE listhandle;
	LPWIN32_FIND_DATA infostruct = NULL;
	SALIL_NEW(infostruct, WIN32_FIND_DATA);

	//Look for this directory name
	listhandle = FindFirstFile(path,infostruct);
	if(listhandle == INVALID_HANDLE_VALUE)
	{
		delete infostruct;
		return TRUE;
	}

	BOOL success = TRUE;

	do
	{
		DWORD attributes = infostruct->dwFileAttributes;
		if((attributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			CString completepath = prefix + infostruct->cFileName;
			//don't add zip files. They could be previous or this backup files.
			if(completepath.Find(".zip") == -1)
			{
				if(!archive->AddNewFile(LPCTSTR(completepath)))
				{
					CString errstr = "Could not add file '" + completepath + "' to the archive.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					success = FALSE;
					break;
				}
			}
		}

		progress->ProgressForward();
	}
	while(FindNextFile(listhandle,infostruct));

	if(!FindClose(listhandle))
		SHOW_INTERNAL_ERROR_LAST("Problem in closing search context.");

	delete infostruct;		
	return success;
}


static BOOL AddImageFolder(CZipArchive *archive, CProgressDialog *progress)
{
	if(!archive || !progress)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CString prefix, suffix, path;
	prefix = gFilePath + "\\images\\";
	path = prefix + "*.*";
	
	HANDLE listhandle;
	LPWIN32_FIND_DATA infostruct = NULL;
	SALIL_NEW(infostruct, WIN32_FIND_DATA);

	//Look for this directory name
	listhandle = FindFirstFile(path,infostruct);
	if(listhandle == INVALID_HANDLE_VALUE)
	{
		delete infostruct;
		return TRUE;
	}

	BOOL success = TRUE;

	do
	{
		DWORD attributes = infostruct->dwFileAttributes;
		if((attributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			CString completepath = prefix + infostruct->cFileName; 
			if(!archive->AddNewFile(LPCTSTR(completepath)))
			{
				CString errstr = "Could not add file '" + completepath + "' to the archive.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				success = FALSE;
				break;
			}
		}

		progress->ProgressForward();
	}
	while(FindNextFile(listhandle,infostruct));

	if(!FindClose(listhandle))
		SHOW_INTERNAL_ERROR_LAST("Problem in closing search context.");

	delete infostruct;		
	return success;
}
#endif

BOOL CBackupHandler::Create()
{
#if 0
	CString backupfilename;

#if 1
	backupfilename = GetFileName("Archive File (*.zip)", "*.zip", "*.zip", FALSE);
#else
	CTime curtime = CTime::GetCurrentTime();

	
	backupfilename.Format("%d%d%d_%d%d%d", curtime.GetYear(), curtime.GetMonth(), curtime.GetDay(),
										   curtime.GetHour(), curtime.GetMinute(), curtime.GetSecond());
	backupfilename = "c:\\" + backupfilename + ".zip";
#endif

	if(backupfilename.IsEmpty())
		return TRUE;

	if(IsFileExisting(backupfilename))
	{
		try
		{
			CFile::Remove(backupfilename);
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.", readerror->m_cause);
			errstr = "Could not delete old archive file '" + backupfilename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	CProgressDialog progress("Adding to archive", 20);
	progress.StartDialog();

	try
	{
		CZipArchive archive;
		archive.Open(LPCTSTR(backupfilename), CZipArchive::zipCreate);
	
		if(!AddMainFolder(&archive, &progress))
			SHOW_INTERNAL_ERROR("Could not properly add main database folder to archive.");

		if(!AddImageFolder(&archive, &progress))
			SHOW_INTERNAL_ERROR("Could not properly add image folder to archive.");

		archive.Close();
	}
	catch(...)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	progress.FinishDialog();
#endif

	return TRUE;
}

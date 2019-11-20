#include "stdafx.h"
#include "RatnaKiran.h"
#include "MainGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRatnaKiranDoc, CDocument)

BEGIN_MESSAGE_MAP(CRatnaKiranDoc, CDocument)
	//{{AFX_MSG_MAP(CRatnaKiranDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define RETURN_AFTER_ERROR \
{\
	while(!vlist.IsEmpty())\
	{\
		CProductVoucher *node = vlist.RemoveHead();\
		if(node)\
			delete node;\
		else\
			SHOW_INTERNAL_ERROR;\
	}\
	infile.Close();\
	return FALSE;\
}

CRatnaKiranDoc::CRatnaKiranDoc()
{
	m_selected_voucher = NULL;
}

CRatnaKiranDoc::~CRatnaKiranDoc()
{
}

void CRatnaKiranDoc::OnCloseDocument() 
{
	if(!WriteVouchers())
		SHOW_INTERNAL_ERROR_REASON("Could not write vouchers onto files.");

	CDocument::OnCloseDocument();
}

BOOL CRatnaKiranDoc::OnNewDocument()
{
	if(!CDocument::OnNewDocument())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!WriteVouchers())
		SHOW_INTERNAL_ERROR_REASON("Could not write vouchers onto files.");

	gDoc = this;
	gStockView = NULL;
	gTreeView = NULL;

	if(!ReadVouchers())
	{
		SHOW_INTERNAL_ERROR_REASON("Could not read vouchers into memory.");
		return FALSE;
	}

	return TRUE;
}

BOOL CRatnaKiranDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	//don't do anything here
	return TRUE;
}

void CRatnaKiranDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

#ifdef _DEBUG
void CRatnaKiranDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRatnaKiranDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



BOOL CRatnaKiranDoc::ReadVouchers()
{
	ClearVouchers();

	CString prefix, suffix, path;
	prefix = gFilePath + "\\";
	if(!suffix.LoadString(IDS_FILEPATH_SUFFIX))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find suffix string.");
		return FALSE;
	}

	path = prefix + "*" + suffix;
	
	HANDLE listhandle;
	LPWIN32_FIND_DATA infostruct = NULL;
	SALIL_NEW(infostruct, WIN32_FIND_DATA);

	//Look for this directory name
	listhandle = FindFirstFile(path,infostruct);
	if(listhandle == INVALID_HANDLE_VALUE)
	{
		//There may not be any voucher file in the folder. That is ok.
		//So we create a new voucher file with just the global vouchers.

		{
			CProductVoucher new_voucher;
			strcpy(new_voucher.m_voucher_name, STOCK_ROOT_NAME);
			new_voucher.m_type = tStock;
			if(!AddVoucher(&new_voucher))
				SHOW_INTERNAL_ERROR_REASON("Could not add a new global stock voucher.");
		}

		{
			CProductVoucher new_voucher;
			strcpy(new_voucher.m_voucher_name, EXPORT_ROOT_NAME);
			new_voucher.m_type = tExport;
			if(!AddVoucher(&new_voucher))
				SHOW_INTERNAL_ERROR_REASON("Could not add a new global export voucher.");
		}

		{
			CProductVoucher new_voucher;
			strcpy(new_voucher.m_voucher_name, KUNDAN_ROOT_NAME);
			new_voucher.m_type = tKundan;
			if(!AddVoucher(&new_voucher))
				SHOW_INTERNAL_ERROR_REASON("Could not add a new global kundan voucher.");
		}

		delete infostruct;
		return TRUE;
	}

	CList<CString, CString> files;
	do
	{
		DWORD attributes = infostruct->dwFileAttributes;
		if((attributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			CString completepath = prefix + infostruct->cFileName; 
			files.AddTail(completepath);
		}
	}
	while(FindNextFile(listhandle,infostruct));

	if(!FindClose(listhandle))
		SHOW_INTERNAL_ERROR_LAST("Problem in closing search context.");

	delete infostruct;

	POSITION pos = files.GetHeadPosition();
	while(pos)
	{
		CString completepath = files.GetAt(pos);
		CDatabaseConverter converter(completepath);
		if(!converter.Convert())
		{
			CString errstr = "Could not convert database file '" + completepath + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			files.GetNext(pos);
			continue;
		}

		if(!ReadFile(completepath))
		{
			CString errstr = "Could not read database file '" + completepath + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		files.GetNext(pos);
	}

	//search for root voucher in stock
	pos = m_stock_vouchers.GetHeadPosition();
	while(pos)
	{
		CProductVoucher *voucher = m_stock_vouchers.GetAt(pos);
		if(voucher)
		{
			if(voucher->IsRootVoucher())
				break;
		}
		else
			SHOW_INTERNAL_ERROR;

		m_stock_vouchers.GetNext(pos);
	}

	if(!pos)
	{
		CProductVoucher new_voucher;
		strcpy(new_voucher.m_voucher_name, STOCK_ROOT_NAME);
		new_voucher.m_type = tStock;
		if(!AddVoucher(&new_voucher))
			SHOW_INTERNAL_ERROR_REASON("Could not add a new global stock voucher.");
	}

	//search for root voucher in export
	pos = m_export_vouchers.GetHeadPosition();
	while(pos)
	{
		CProductVoucher *voucher = m_export_vouchers.GetAt(pos);
		if(voucher)
		{
			if(voucher->IsRootVoucher())
				break;
		}
		else
			SHOW_INTERNAL_ERROR;

		m_export_vouchers.GetNext(pos);
	}

	if(!pos)
	{
		CProductVoucher new_voucher;
		strcpy(new_voucher.m_voucher_name, EXPORT_ROOT_NAME);
		new_voucher.m_type = tExport;
		if(!AddVoucher(&new_voucher))
			SHOW_INTERNAL_ERROR_REASON("Could not add a new global export voucher.");
	}

	//search for root voucher in kundan
	pos = m_kundan_vouchers.GetHeadPosition();
	while(pos)
	{
		CProductVoucher *voucher = m_kundan_vouchers.GetAt(pos);
		if(voucher)
		{
			if(voucher->IsRootVoucher())
				break;
		}
		else
			SHOW_INTERNAL_ERROR;

		m_kundan_vouchers.GetNext(pos);
	}

	if(!pos)
	{
		CProductVoucher new_voucher;
		strcpy(new_voucher.m_voucher_name, KUNDAN_ROOT_NAME);
		new_voucher.m_type = tKundan;
		if(!AddVoucher(&new_voucher))
			SHOW_INTERNAL_ERROR_REASON("Could not add a new global kundan voucher.");
	}

	CString main_file = gFilePath + "\\_main" + suffix;
	try
	{
		CFile::Remove(LPCTSTR(main_file));
	}
	catch(...)
	{
	}

	return TRUE;
}


BOOL CRatnaKiranDoc::ReadFile(CString filename)
{
	if(filename.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CFile infile;
	CFileException openerror;
	if(!infile.Open(filename, CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", openerror.m_cause);
		errstr = "Could not open file '" + filename + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	CList<CProductVoucher *, CProductVoucher *> vlist;
	DWORD readcount = 0;

	//start by reading the main header
	{
		CProductFileHeader buf;
		
		try
		{
			readcount = infile.Read(&buf,sizeof(CProductFileHeader));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.", readerror->m_cause);
			errstr = "Could not read header info from file '" + filename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			RETURN_AFTER_ERROR;
		}

		if(readcount < sizeof(CProductFileHeader))
		{
			CString errstr = "Invalid file size. Could not read the file header for '" + filename + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			RETURN_AFTER_ERROR;
		}
	}

	//start reading vouchers till end of file
	while(TRUE)
	{
		CProductVoucher *voucher_buf;
		SALIL_NEW(voucher_buf, CProductVoucher);

		try
		{
			readcount = infile.Read(voucher_buf,sizeof(CProductVoucherData));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.", readerror->m_cause);
			errstr = "Could not read voucher info from file '" + filename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			delete voucher_buf;
			RETURN_AFTER_ERROR;
		}

		if(readcount < sizeof(CProductVoucherData))
		{
			delete voucher_buf;
			break;
		}

		DECODE(voucher_buf, CProductVoucherData);

		DWORD set_count = voucher_buf->m_set_count;
		voucher_buf->m_set_count = 0;
		while(set_count > 0)
		{
			CProductSet set_buf;

			try
			{
				readcount = infile.Read(&set_buf,sizeof(CProductSetData));
			}
			catch(CFileException *readerror)
			{
				CString errstr;
				errstr.Format("ERROR : #%d.", readerror->m_cause);
				errstr = "Could not read set info from file '" + filename + "' for voucher '" + CString(voucher_buf->m_voucher_name) + "'.\n" + errstr;
				SHOW_INTERNAL_ERROR_REASON(errstr);
				delete voucher_buf;
				RETURN_AFTER_ERROR;
			}

			if(readcount < sizeof(CProductSetData))
			{
				delete voucher_buf;
				CString errstr = "Could not read all sets from file '" + filename + "' for voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				RETURN_AFTER_ERROR;
			}

			DECODE(&set_buf, CProductSetData);

			int item_count = set_buf.m_item_count;
			set_buf.m_item_count = 0;
			while(item_count > 0)
			{
				CProductItem item_buf;

				try
				{
					readcount = infile.Read(&item_buf,sizeof(CProductItemData));
				}
				catch(CFileException *readerror)
				{
					CString errstr;
					errstr.Format("ERROR : #%d.", readerror->m_cause);
					errstr = "Could not read item info from file '" + filename + "' for set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.\n" + errstr;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete voucher_buf;
					RETURN_AFTER_ERROR;
				}

				if(readcount < sizeof(CProductItemData))
				{
					delete voucher_buf;
					CString errstr = "Could not read all items from file '" + filename + "' for set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					RETURN_AFTER_ERROR;
				}

				DECODE(&item_buf, CProductItemData);

				int pearl_count = item_buf.m_pearl_count;
				item_buf.m_pearl_count = 0;
				while(pearl_count > 0)
				{
					CProductSubItem pearl_buf;

					try
					{
						readcount = infile.Read(&pearl_buf,sizeof(CProductSubItemData));
					}
					catch(CFileException *readerror)
					{
						CString errstr;
						errstr.Format("ERROR : #%d.", readerror->m_cause);
						errstr = "Could not read pearl info from file '" + filename + "' for item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.\n" + errstr;
						SHOW_INTERNAL_ERROR_REASON(errstr);
						delete voucher_buf;
						RETURN_AFTER_ERROR;
					}

					if(readcount < sizeof(CProductSubItemData))
					{
						delete voucher_buf;
						CString errstr = "Could not read all pearl subitems from file '" + filename + "' for item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						RETURN_AFTER_ERROR;
					}

					DECODE(&pearl_buf, CProductSubItemData);
					if(!item_buf.AddSubItem(tPearl, &pearl_buf))
					{
						delete voucher_buf;
						CString errstr = "Could not add pearl subitem '" + CString(pearl_buf.m_type) + "' to item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						RETURN_AFTER_ERROR;
					}

					pearl_count--;
				}

				int diamond_count = item_buf.m_diamond_count;
				item_buf.m_diamond_count = 0;
				while(diamond_count > 0)
				{
					CProductSubItem diamond_buf;

					try
					{
						readcount = infile.Read(&diamond_buf,sizeof(CProductSubItemData));
					}
					catch(CFileException *readerror)
					{
						CString errstr;
						errstr.Format("ERROR : #%d.", readerror->m_cause);
						errstr = "Could not read diamond info from file '" + filename + "' for item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.\n" + errstr;
						SHOW_INTERNAL_ERROR_REASON(errstr);
						delete voucher_buf;
						RETURN_AFTER_ERROR;
					}

					if(readcount < sizeof(CProductSubItemData))
					{
						delete voucher_buf;
						CString errstr = "Could not read all diamond subitems from file '" + filename + "' for item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						RETURN_AFTER_ERROR;
					}

					DECODE(&diamond_buf, CProductSubItemData);
					if(!item_buf.AddSubItem(tDiamond, &diamond_buf))
					{
						delete voucher_buf;
						CString errstr = "Could not add diamond subitem '" + CString(diamond_buf.m_type) + "' to item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						RETURN_AFTER_ERROR;
					}

					diamond_count--;
				}

				int stone_count = item_buf.m_stone_count;
				item_buf.m_stone_count = 0;
				while(stone_count > 0)
				{
					CProductSubItem stone_buf;

					try
					{
						readcount = infile.Read(&stone_buf,sizeof(CProductSubItemData));
					}
					catch(CFileException *readerror)
					{
						CString errstr;
						errstr.Format("ERROR : #%d.", readerror->m_cause);
						errstr = "Could not read stone info from file '" + filename + "' for item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.\n" + errstr;
						SHOW_INTERNAL_ERROR_REASON(errstr);
						delete voucher_buf;
						RETURN_AFTER_ERROR;
					}

					if(readcount < sizeof(CProductSubItemData))
					{
						delete voucher_buf;
						CString errstr = "Could not read all stone subitems from file '" + filename + "' for item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						RETURN_AFTER_ERROR;
					}

					DECODE(&stone_buf, CProductSubItemData);
					if(!item_buf.AddSubItem(tStone, &stone_buf))
					{
						delete voucher_buf;
						CString errstr = "Could not add stone subitem '" + CString(stone_buf.m_type) + "' to item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						RETURN_AFTER_ERROR;
					}

					stone_count--;
				}

				int old_serial_count = item_buf.m_old_serial_count;
				item_buf.m_old_serial_count = 0;
				while(old_serial_count > 0)
				{
					CProductOldSerial serial_buf;

					try
					{
						readcount = infile.Read(&serial_buf,sizeof(CProductOldSerialData));
					}
					catch(CFileException *readerror)
					{
						CString errstr;
						errstr.Format("ERROR : #%d.", readerror->m_cause);
						errstr = "Could not read old serial info from file '" + filename + "' for item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.\n" + errstr;
						SHOW_INTERNAL_ERROR_REASON(errstr);
						delete voucher_buf;
						RETURN_AFTER_ERROR;
					}

					if(readcount < sizeof(CProductOldSerialData))
					{
						delete voucher_buf;
						CString errstr = "Could not read all old serial numbers from file '" + filename + "' for item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						RETURN_AFTER_ERROR;
					}

					DECODE(&serial_buf, CProductOldSerialData);
					if(!item_buf.AddOldSerial(&serial_buf))
					{
						delete voucher_buf;
						CString errstr = "Could not add old serial number with tagname '" + CString(serial_buf.m_tagname) + "' to item '" + CString(item_buf.m_product_ID) + "' in set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						RETURN_AFTER_ERROR;
					}

					old_serial_count--;
				}

				if(!set_buf.AddItem(&item_buf, FALSE))
				{
					delete voucher_buf;
					CString errstr = "Could not add item '" + CString(item_buf.m_product_ID) + "' to set '" + CString(set_buf.m_product_ID) + "' in voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					RETURN_AFTER_ERROR;
				}

				item_count--;
			}

			if(!voucher_buf->AddSet(&set_buf, TRUE, TRUE))
			{
				delete voucher_buf;
				CString errstr = "Could not add set '" + CString(set_buf.m_product_ID) + "' to voucher '" + CString(voucher_buf->m_voucher_name) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				RETURN_AFTER_ERROR;
			}

			set_count--;
		}

		vlist.AddTail(voucher_buf);
	}

	infile.Close();
	while(!vlist.IsEmpty())
	{
		CProductVoucher *node = vlist.RemoveHead();
		if(node)
		{
			if(!AddVoucher(node))
			{
				CString errstr = "Could not store the voucher '" + CString(node->m_voucher_name) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}

			delete node;
		}
		else
			SHOW_INTERNAL_ERROR;
	}

	return TRUE;
}


BOOL CRatnaKiranDoc::WriteVouchers()
{
	POSITION pos = m_stock_vouchers.GetHeadPosition();
	while(pos)
	{
		CProductVoucher *curvoucher = m_stock_vouchers.GetAt(pos);
		if(curvoucher)
		{
			if(!WriteFile(curvoucher))
			{
				CString errstr = "Could not write voucher '" + CString(curvoucher->m_voucher_name) + "' to file.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		m_stock_vouchers.GetNext(pos);
	}

	pos = m_export_vouchers.GetHeadPosition();
	while(pos)
	{
		CProductVoucher *curvoucher = m_export_vouchers.GetAt(pos);
		if(curvoucher)
		{
			if(!WriteFile(curvoucher))
			{
				CString errstr = "Could not write voucher '" + CString(curvoucher->m_voucher_name) + "' to file.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		m_export_vouchers.GetNext(pos);
	}

	pos = m_kundan_vouchers.GetHeadPosition();
	while(pos)
	{
		CProductVoucher *curvoucher = m_kundan_vouchers.GetAt(pos);
		if(curvoucher)
		{
			if(!WriteFile(curvoucher))
			{
				CString errstr = "Could not write voucher '" + CString(curvoucher->m_voucher_name) + "' to file.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		m_kundan_vouchers.GetNext(pos);
	}

	return TRUE;
}


BOOL CRatnaKiranDoc::WriteFile(CProductVoucher *cur_voucher)
{
	if(!cur_voucher)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CString prefix, suffix;
	prefix = gFilePath + "\\";
	if(!suffix.LoadString(IDS_FILEPATH_SUFFIX))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find suffix string.");
		return FALSE;
	}

	CString filename = prefix + cur_voucher->m_voucher_name + suffix;

	CString output_filename = gFilePath + "\\_temp_converter_file.tmp";
	CFileException openerror;
	CFile outfile;
	if(!outfile.Open(output_filename, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", openerror.m_cause);
		errstr = "Could not open file '" + output_filename + "' while trying to write voucher file '" + filename + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	//start by first writing the file header
	{
		CProductFileHeader header_buf;

		ENCODE(&header_buf, CProductFileHeader);

		try
		{
			outfile.Write(&header_buf, sizeof(CProductFileHeader));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.", readerror->m_cause);
			errstr = "Could not write to file '" + output_filename + "' while trying to write voucher file '" + filename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			outfile.Close();
			return FALSE;
		}
	}

	CProductVoucherData voucher_buf = (CProductVoucherData)(*cur_voucher);
	ENCODE(&voucher_buf, CProductVoucherData);

	try
	{
		outfile.Write(&voucher_buf, sizeof(CProductVoucherData));
	}
	catch(CFileException *readerror)
	{
		DECODE(&voucher_buf, CProductVoucherData);
		CString errstr;
		errstr.Format("ERROR : #%d.", readerror->m_cause);
		errstr = "Could not write voucher '" + CString(cur_voucher->m_voucher_name) + "' to file '" + output_filename + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		outfile.Close();
		return FALSE;
	}

	DECODE(&voucher_buf, CProductVoucherData);
	if(cur_voucher->m_set_count != (DWORD)cur_voucher->m_sets.GetCount())
	{
		CString errstr = "Number of sets in voucher '" + CString(cur_voucher->m_voucher_name) + "' does not match the list size.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		outfile.Close();
		return FALSE;
	}

	POSITION set_pos = cur_voucher->m_sets.GetHeadPosition();
	while(set_pos)
	{
		CProductSet *cur_set = cur_voucher->m_sets.GetAt(set_pos);
		if(!cur_set)
		{
			SHOW_INTERNAL_ERROR;
			outfile.Close();
			return FALSE;
		}

		CProductSetData set_buf = (CProductSetData)(*cur_set);
		ENCODE(&set_buf, CProductSetData);

		try
		{
			outfile.Write(&set_buf, sizeof(CProductSetData));
		}
		catch(CFileException *readerror)
		{
			DECODE(&set_buf, CProductSetData);
			CString errstr;
			errstr.Format("ERROR : #%d.", readerror->m_cause);
			errstr = "Could not write set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' to file '" + output_filename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			outfile.Close();
			return FALSE;
		}

		DECODE(&set_buf, CProductSetData);
		if(cur_set->m_item_count != cur_set->m_items.GetCount())
		{
			CString errstr = "Number of items in set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' does not match the list size.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			outfile.Close();
			return FALSE;
		}

		POSITION item_pos = cur_set->m_items.GetHeadPosition();
		while(item_pos)
		{
			CProductItem *cur_item = cur_set->m_items.GetAt(item_pos);
			if(!cur_item)
			{
				SHOW_INTERNAL_ERROR;
				outfile.Close();
				return FALSE;
			}

			CProductItemData item_buf = (CProductItemData)(*cur_item);
			ENCODE(&item_buf, CProductItemData);

			try
			{
				outfile.Write(&item_buf, sizeof(CProductItemData));
			}
			catch(CFileException *readerror)
			{
				DECODE(&item_buf, CProductItemData);
				CString errstr;
				errstr.Format("ERROR : #%d.", readerror->m_cause);
				errstr = "Could not write item '" + CString(cur_item->m_product_ID) + "' in set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' to file '" + output_filename + "'.\n" + errstr;
				SHOW_INTERNAL_ERROR_REASON(errstr);
				outfile.Close();
				return FALSE;
			}

			DECODE(&item_buf, CProductItemData);
			if(cur_item->m_pearl_count != cur_item->m_pearls.GetCount())
			{
				CString errstr;
				errstr.Format(" (%d,%d) ", cur_item->m_pearl_count, cur_item->m_pearls.GetCount());
				errstr = "Number of pearl subitems" + errstr + "in item '" + CString(cur_item->m_product_ID) + "' in set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' does not match the list size.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				outfile.Close();
				return FALSE;
			}

			POSITION pearl_pos = cur_item->m_pearls.GetHeadPosition();
			while(pearl_pos)
			{
				CProductSubItem *cur_pearl = cur_item->m_pearls.GetAt(pearl_pos);
				if(!cur_pearl)
				{
					SHOW_INTERNAL_ERROR;
					outfile.Close();
					return FALSE;
				}

				CProductSubItemData pearl_buf = (CProductSubItemData)(*cur_pearl);
				ENCODE(&pearl_buf, CProductSubItemData);

				try
				{
					outfile.Write(&pearl_buf, sizeof(CProductSubItemData));
				}
				catch(CFileException *readerror)
				{
					DECODE(&pearl_buf, CProductSubItemData);
					CString errstr;
					errstr.Format("ERROR : #%d.", readerror->m_cause);
					errstr = "Could not write pearl '" + CString(cur_pearl->m_type) + "' in item '" + CString(cur_item->m_product_ID) + "' in set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' to file '" + output_filename + "'.\n" + errstr;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					outfile.Close();
					return FALSE;
				}

				DECODE(&pearl_buf, CProductSubItemData);
				cur_item->m_pearls.GetNext(pearl_pos);
			}

			if(cur_item->m_diamond_count != cur_item->m_diamonds.GetCount())
			{
				CString errstr;
				errstr.Format(" (%d,%d) ", cur_item->m_diamond_count, cur_item->m_diamonds.GetCount());
				errstr = "Number of diamond subitems" + errstr + "in item '" + CString(cur_item->m_product_ID) + "' in set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' does not match the list size.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				outfile.Close();
				return FALSE;
			}

			POSITION diamond_pos = cur_item->m_diamonds.GetHeadPosition();
			while(diamond_pos)
			{
				CProductSubItem *cur_diamond = cur_item->m_diamonds.GetAt(diamond_pos);
				if(!cur_diamond)
				{
					SHOW_INTERNAL_ERROR;
					outfile.Close();
					return FALSE;
				}

				CProductSubItemData diamond_buf = (CProductSubItemData)(*cur_diamond);
				ENCODE(&diamond_buf, CProductSubItemData);

				try
				{
					outfile.Write(&diamond_buf, sizeof(CProductSubItemData));
				}
				catch(CFileException *readerror)
				{
					DECODE(&diamond_buf, CProductSubItemData);
					CString errstr;
					errstr.Format("ERROR : #%d.", readerror->m_cause);
					errstr = "Could not write diamond '" + CString(cur_diamond->m_type) + "' in item '" + CString(cur_item->m_product_ID) + "' in set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' to file '" + output_filename + "'.\n" + errstr;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					outfile.Close();
					return FALSE;
				}

				DECODE(&diamond_buf, CProductSubItemData);
				cur_item->m_diamonds.GetNext(diamond_pos);
			}

			if(cur_item->m_stone_count != cur_item->m_stones.GetCount())
			{
				CString errstr;
				errstr.Format(" (%d,%d) ", cur_item->m_stone_count, cur_item->m_stones.GetCount());
				errstr = "Number of stone subitems" + errstr + "in item '" + CString(cur_item->m_product_ID) + "' in set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' does not match the list size.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				outfile.Close();
				return FALSE;
			}

			POSITION stone_pos = cur_item->m_stones.GetHeadPosition();
			while(stone_pos)
			{
				CProductSubItem *cur_stone = cur_item->m_stones.GetAt(stone_pos);
				if(!cur_stone)
				{
					SHOW_INTERNAL_ERROR;
					outfile.Close();
					return FALSE;
				}

				CProductSubItemData stone_buf = (CProductSubItemData)(*cur_stone);
				ENCODE(&stone_buf, CProductSubItemData);

				try
				{
					outfile.Write(&stone_buf, sizeof(CProductSubItemData));
				}
				catch(CFileException *readerror)
				{
					DECODE(&stone_buf, CProductSubItemData);
					CString errstr;
					errstr.Format("ERROR : #%d.", readerror->m_cause);
					errstr = "Could not write stone '" + CString(cur_stone->m_type) + "' in item '" + CString(cur_item->m_product_ID) + "' in set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' to file '" + output_filename + "'.\n" + errstr;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					outfile.Close();
					return FALSE;
				}

				DECODE(&stone_buf, CProductSubItemData);
				cur_item->m_stones.GetNext(stone_pos);
			}

			if(cur_item->m_old_serial_count != cur_item->m_old_serials.GetCount())
			{
				CString errstr;
				errstr.Format(" (%d,%d) ", cur_item->m_old_serial_count, cur_item->m_old_serials.GetCount());
				errstr = "Number of old serial numbers" + errstr + "in item '" + CString(cur_item->m_product_ID) + "' in set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' does not match the list size.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				outfile.Close();
				return FALSE;
			}

			POSITION serial_pos = cur_item->m_old_serials.GetHeadPosition();
			while(serial_pos)
			{
				CProductOldSerial *cur_serial = cur_item->m_old_serials.GetAt(serial_pos);
				if(!cur_serial)
				{
					SHOW_INTERNAL_ERROR;
					outfile.Close();
					return FALSE;
				}

				CProductOldSerialData serial_buf = (CProductOldSerialData)(*cur_serial);
				ENCODE(&serial_buf, CProductOldSerialData);

				try
				{
					outfile.Write(&serial_buf, sizeof(CProductOldSerialData));
				}
				catch(CFileException *readerror)
				{
					DECODE(&serial_buf, CProductOldSerialData);
					CString errstr;
					errstr.Format("ERROR : #%d.", readerror->m_cause);
					errstr = "Could not write old serial with tagname '" + CString(cur_serial->m_tagname) + "' in item '" + CString(cur_item->m_product_ID) + "' in set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(cur_voucher->m_voucher_name) + "' to file '" + output_filename + "'.\n" + errstr;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					outfile.Close();
					return FALSE;
				}

				DECODE(&serial_buf, CProductOldSerialData);
				cur_item->m_old_serials.GetNext(serial_pos);
			}

			cur_set->m_items.GetNext(item_pos);
		}

		cur_voucher->m_sets.GetNext(set_pos);
	}

	outfile.Close();

	if(!MoveDatabaseFile(output_filename, filename))
	{
		CString errstr = "Could not move file '" + output_filename + "' to '" + filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}


void CRatnaKiranDoc::ClearVouchers()
{
	while(!m_stock_vouchers.IsEmpty())
	{
		CProductVoucher *voucher = m_stock_vouchers.RemoveHead();
		if(voucher)
			delete voucher;
		else
			SHOW_INTERNAL_ERROR;
	}

	while(!m_export_vouchers.IsEmpty())
	{
		CProductVoucher *voucher = m_export_vouchers.RemoveHead();
		if(voucher)
			delete voucher;
		else
			SHOW_INTERNAL_ERROR;
	}

	while(!m_kundan_vouchers.IsEmpty())
	{
		CProductVoucher *voucher = m_kundan_vouchers.RemoveHead();
		if(voucher)
			delete voucher;
		else
			SHOW_INTERNAL_ERROR;
	}

	m_selected_voucher = NULL;
}


BOOL CRatnaKiranDoc::AddVoucher(CProductVoucher *voucher)
{
	if(!voucher)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CProductVoucher *new_voucher;
	SALIL_NEW(new_voucher, CProductVoucher);
	*new_voucher = *voucher;

	switch(new_voucher->m_type)
	{
	case tStock:
		m_stock_vouchers.AddTail(new_voucher);
		break;
	case tExport:
		m_export_vouchers.AddTail(new_voucher);
		break;
	case tKundan:
		m_kundan_vouchers.AddTail(new_voucher);
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", new_voucher->m_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CRatnaKiranDoc::DeleteVoucher(CString voucher_name)
{
	if(voucher_name.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CString prefix, suffix;
	prefix = gFilePath + "\\";
	if(!suffix.LoadString(IDS_FILEPATH_SUFFIX))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not find suffix string.");
		return FALSE;
	}

	CProductVoucher *found_voucher = NULL;

	if(!found_voucher)
	{
		POSITION pos = m_stock_vouchers.GetHeadPosition();
		while(pos)
		{
			CProductVoucher *cur_voucher = m_stock_vouchers.GetAt(pos);
			if(cur_voucher)
			{
				if(voucher_name == CString(cur_voucher->m_voucher_name))
				{
					m_stock_vouchers.RemoveAt(pos);
					found_voucher = cur_voucher;

					if(m_selected_voucher == cur_voucher)
						m_selected_voucher = NULL;

					break;
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_stock_vouchers.GetNext(pos);
		}
	}

	if(!found_voucher)
	{
		POSITION pos = m_export_vouchers.GetHeadPosition();
		while(pos)
		{
			CProductVoucher *cur_voucher = m_export_vouchers.GetAt(pos);
			if(cur_voucher)
			{
				if(voucher_name == CString(cur_voucher->m_voucher_name))
				{
					m_export_vouchers.RemoveAt(pos);
					found_voucher = cur_voucher;

					if(m_selected_voucher == cur_voucher)
						m_selected_voucher = NULL;

					break;
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_export_vouchers.GetNext(pos);
		}
	}

	if(!found_voucher)
	{
		POSITION pos = m_kundan_vouchers.GetHeadPosition();
		while(pos)
		{
			CProductVoucher *cur_voucher = m_kundan_vouchers.GetAt(pos);
			if(cur_voucher)
			{
				if(voucher_name == CString(cur_voucher->m_voucher_name))
				{
					m_kundan_vouchers.RemoveAt(pos);
					found_voucher = cur_voucher;

					if(m_selected_voucher == cur_voucher)
						m_selected_voucher = NULL;

					break;
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_kundan_vouchers.GetNext(pos);
		}
	}
	
	if(!found_voucher)
	{
		CString errstr = "Could not find voucher '" + voucher_name + "' to delete.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	CString filename = prefix + found_voucher->m_voucher_name + suffix;

	try
	{
		CFile::Remove(LPCTSTR(filename));
	}
	catch(CFileException *readerror)
	{
		switch(found_voucher->m_type)
		{
		case tStock:
			m_stock_vouchers.AddTail(found_voucher);
			break;
		case tExport:
			m_export_vouchers.AddTail(found_voucher);
			break;
		case tKundan:
			m_kundan_vouchers.AddTail(found_voucher);
			break;
		default:
			SHOW_INTERNAL_ERROR;
			break;
		}

		CString errstr;
		errstr.Format("ERROR : #%d.", readerror->m_cause);
		errstr = "Could not delete voucher file '" + filename + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	delete found_voucher;
	return TRUE;
}

CProductVoucher *CRatnaKiranDoc::GetVoucher(CString voucher_name)
{
	if(voucher_name.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	POSITION pos = m_stock_vouchers.GetHeadPosition();
	while(pos)
	{
		CProductVoucher *cur_voucher = m_stock_vouchers.GetAt(pos);
		if(cur_voucher)
		{
			if(voucher_name == CString(cur_voucher->m_voucher_name))
				return cur_voucher;
		}
		else
			SHOW_INTERNAL_ERROR;

		m_stock_vouchers.GetNext(pos);
	}

	pos = m_export_vouchers.GetHeadPosition();
	while(pos)
	{
		CProductVoucher *cur_voucher = m_export_vouchers.GetAt(pos);
		if(cur_voucher)
		{
			if(voucher_name == CString(cur_voucher->m_voucher_name))
				return cur_voucher;
		}
		else
			SHOW_INTERNAL_ERROR;

		m_export_vouchers.GetNext(pos);
	}

	pos = m_kundan_vouchers.GetHeadPosition();
	while(pos)
	{
		CProductVoucher *cur_voucher = m_kundan_vouchers.GetAt(pos);
		if(cur_voucher)
		{
			if(voucher_name == CString(cur_voucher->m_voucher_name))
				return cur_voucher;
		}
		else
			SHOW_INTERNAL_ERROR;

		m_kundan_vouchers.GetNext(pos);
	}
	
	return NULL;
}


BOOL CRatnaKiranDoc::SetCurrentVoucher(CString name)
{
	if(name.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(name == CString(STOCK_ROOT))
		name = STOCK_ROOT_NAME;
	else if(name == CString(EXPORT_ROOT))
		name = EXPORT_ROOT_NAME;
	else if(name == CString(KUNDAN_ROOT))
		name = KUNDAN_ROOT_NAME;

	CProductVoucher *voucher = GetVoucher(name);
	if(voucher)
	{
		m_selected_voucher = voucher;
		if(gStockView->RefreshEntries(TRUE))
			return TRUE;
		else
			SHOW_INTERNAL_ERROR_REASON("Could not refresh the list view.");
	}
	else
	{
		CString errstr = "Could not find voucher '" + name + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	return FALSE;
}

BOOL CRatnaKiranDoc::IsIgnored(CString name)
{
	if(name.IsEmpty())
		SHOW_INTERNAL_ERROR;

	POSITION pos = m_ignore_list.Find(name);
	return (pos != NULL);
}

void CRatnaKiranDoc::AddToIgnoreEntry(CString name)
{
	if(name.IsEmpty())
		SHOW_INTERNAL_ERROR;

	if(!IsIgnored(name))
		m_ignore_list.AddTail(name);
}

void CRatnaKiranDoc::DeleteFromIgnoreEntry(CString name)
{
	if(name.IsEmpty())
		SHOW_INTERNAL_ERROR;

	if(IsIgnored(name))
	{
		POSITION pos = m_ignore_list.Find(name);
		if(pos)
			m_ignore_list.RemoveAt(pos);
		else
		{
			CString errstr = "Could not find voucher name '" + name + "' in the ignore list.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}
}

CProductSet *CRatnaKiranDoc::GetSet(CString productID, CProductVoucher **voucher)
{
	if(!m_selected_voucher || productID.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return NULL;
	}

	if(m_selected_voucher->IsRootVoucher())
	{
		CList<CProductVoucher *, CProductVoucher *> *list;

		switch(m_selected_voucher->m_type)
		{
		case tStock:
			list = &m_stock_vouchers;
			break;
		case tExport:
			list = &m_export_vouchers;
			break;
		case tKundan:
			list = &m_kundan_vouchers;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", m_selected_voucher->m_type);
				errstr = "Invalid voucher type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return NULL;
			}
		}

		if(!list)
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		POSITION pos = list->GetHeadPosition();
		while(pos)
		{
			CProductVoucher *cur_voucher = list->GetAt(pos);
			if(cur_voucher)
			{
				if(!IsIgnored(cur_voucher->m_voucher_name))
				{
					CProductSet *cur_set = cur_voucher->GetSet(productID);
					if(cur_set)
					{
						if(voucher)
							*voucher = cur_voucher;

						return cur_set;
					}
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			list->GetNext(pos);
		}

		return NULL;
	}
	else
	{
		CProductSet *set = m_selected_voucher->GetSet(productID);
		if(set)
		{
			if(voucher)
				*voucher = m_selected_voucher;

			return set;
		}

		return NULL;
	}
}

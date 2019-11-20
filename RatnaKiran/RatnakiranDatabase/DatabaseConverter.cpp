#include "DatabaseGlobals.h"

#define RETURN_AFTER_ERROR {\
inputfile.Close();\
outputfile.Close();\
return FALSE;\
}

BOOL CDatabaseConverter::CheckEquality()
{
	static int firstrun = 1;
	if(firstrun == 0)
		return TRUE;
	else
		firstrun = 0;

	{
		CProductFileHeader o1;
		CProductFileHeaderV3 o2;

		if(sizeof(o1) != sizeof(o2))
			return FALSE;

		int offset1 = 0;
		int offset2 = 0;

		offset1 = (int)(&(o1.m_version)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_version)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;
	}

	{
		CProductVoucherData o1;
		CProductVoucherDataV3 o2;

		if(sizeof(o1) != sizeof(o2))
			return FALSE;

		int offset1 = 0;
		int offset2 = 0;

		offset1 = (int)(&(o1.m_consignee)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_consignee)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_country)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_country)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_currency_conversion)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_currency_conversion)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_currency_name)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_currency_name)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_diamond_rate)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_diamond_rate)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_dollar_rate)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_dollar_rate)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_exporter)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_exporter)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_gold_rate)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_gold_rate)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_invoice_number)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_invoice_number)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_labour_cost)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_labour_cost)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_pearl_rate)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_pearl_rate)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_price_quote)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_price_quote)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_set_count)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_set_count)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_stone_rate)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_stone_rate)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_type)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_type)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_voucher_name)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_voucher_name)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;
	}

	{
		CProductSetData o1;
		CProductSetDataV3 o2;

		if(sizeof(o1) != sizeof(o2))
			return FALSE;

		int offset1 = 0;
		int offset2 = 0;

		offset1 = (int)(&(o1.m_description)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_description)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_item_count)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_item_count)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_product_ID)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_product_ID)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;
	}

	{
		CProductItemData o1;
		CProductItemDataV3 o2;

		if(sizeof(o1) != sizeof(o2))
			return FALSE;

		int offset1 = 0;
		int offset2 = 0;

		offset1 = (int)(&(o1.m_costing)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_costing)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_description)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_description)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_diamond_clarity)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_diamond_clarity)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_diamond_count)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_diamond_count)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_diamond_marked_value)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_diamond_marked_value)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_gold_rate)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_gold_rate)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_gross_weight)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_gross_weight)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_labour_cost)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_labour_cost)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_labour_marked_cost)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_labour_marked_cost)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_marked_costing)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_marked_costing)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_old_serial_count)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_old_serial_count)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_other_weight)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_other_weight)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_pearl_count)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_pearl_count)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_pearl_marked_value)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_pearl_marked_value)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_photo_filename)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_photo_filename)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_piece_count)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_piece_count)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_product_ID)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_product_ID)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_serial_number)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_serial_number)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_sold)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_sold)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_sold_country)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_sold_country)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_stone_count)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_stone_count)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_stone_marked_value)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_stone_marked_value)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_use_net_weight_for_gold)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_use_net_weight_for_gold)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;
	}

	{
		CProductSubItemData o1;
		CProductSubItemDataV3 o2;

		if(sizeof(o1) != sizeof(o2))
			return FALSE;

		int offset1 = 0;
		int offset2 = 0;

		offset1 = (int)(&(o1.m_count)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_count)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_rate)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_rate)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_type)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_type)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_weight)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_weight)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;
	}

	{
		CProductOldSerialData o1;
		CProductOldSerialDataV3 o2;

		if(sizeof(o1) != sizeof(o2))
			return FALSE;

		int offset1 = 0;
		int offset2 = 0;

		offset1 = (int)(&(o1.m_serial)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_serial)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;

		offset1 = (int)(&(o1.m_tagname)) - (int)(&(o1));
		offset2 = (int)(&(o2.m_tagname)) - (int)(&(o2));
		if(offset1 != offset2)
			return FALSE;
	}

	return TRUE;
}

CDatabaseConverter::CDatabaseConverter(CString filename)
{
	m_filename = filename;

	if(!CheckEquality())
	{
		SHOW_INTERNAL_ERROR_REASON("Inconsistent database sizes. Probably an incorrect compilation.");
		exit(1);
	}
}

CDatabaseConverter::~CDatabaseConverter()
{
}

BOOL CDatabaseConverter::Convert()
{
	int version = GetDatabaseVersion();
	BOOL success = TRUE;

	if(m_filename.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	switch(version)
	{
	case 0:
		if(!ConvertV0toV1())
		{
			CString errstr = "Conversion from V0 to V1 failed for '" + m_filename + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			success = FALSE;
		}
		break;
	case 1:
		if(!ConvertV1toV2())
		{
			CString errstr = "Conversion from V1 to V2 failed for '" + m_filename + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			success = FALSE;
		}
		break;
	case 2:
		if(!ConvertV2toV3())
		{
			CString errstr = "Conversion from V2 to V3 failed for '" + m_filename + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			success = FALSE;
		}
		break;
	case 3:
		//latest version
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", version);
			errstr = "Invalid file version (Ver. = " + errstr + ") detected for '" + m_filename + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			success = FALSE;
		}
		break;
	}

	return success;
}

int CDatabaseConverter::GetDatabaseVersion()
{
	CFile inputfile;
	CFileException openerror;
	CString input_filename = m_filename;

	if(input_filename.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return -1;
	}

	if(!inputfile.Open(LPCTSTR(input_filename), CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",openerror.m_cause);
		errstr = "Could not open file '" + input_filename + "' for fetching the version info.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return -1;
	}

	char version[5];
	int ver = -1;
	DWORD readcount = 0;

	try
	{
		readcount = inputfile.Read(version, 5);
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		errstr = "Could not read version info from the file '" + input_filename + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		inputfile.Close();
		return -1;
	}

	if(readcount > 0)
	{
		DECODE(version, version);
		version[4] = '\0';

		if(version[0] == 'V')
		{
			if(sscanf(version, "V%d", &ver) != 1)
			{
				CString errstr = "Garbage version info in the file '" + input_filename + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				ver = -1;
			}
		}
		else
			ver = 0;
	}
	else
	{
		CString errstr = "Could not read the version info from the file '" + input_filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		ver = -1;
	}

	inputfile.Close();
	return ver;
}

BOOL CDatabaseConverter::ConvertV0toV1()
{
	CFile inputfile;
	CFile outputfile;
	CFileException openerror;
	CString input_filename = m_filename;
	CString output_filename = gFilePath + "\\_temp_converter_file.tmp";

	if(input_filename.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!inputfile.Open(LPCTSTR(input_filename), CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",openerror.m_cause);
		errstr = "Could not open file '" + input_filename + "' for converting from V0 to V1.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!outputfile.Open(LPCTSTR(output_filename), CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",openerror.m_cause);
		errstr = "Could not open temporary file '" + output_filename + "' while converting file '" + input_filename + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		inputfile.Close();
		return FALSE;
	}

	try
	{
		inputfile.SeekToBegin();
		outputfile.SeekToBegin();
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		SHOW_INTERNAL_ERROR_REASON(errstr);
		RETURN_AFTER_ERROR;
	}

	CDbV0 original;
	CDbV1 final;

	FileError err = ReadV0(&original, &inputfile);
	if(err == Error)
	{
		CString errstr = "Could not read V0 voucher info from file '" + input_filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		RETURN_AFTER_ERROR;
	}

	CString work = "Converting database '" + m_filename + "' from V0 to V1";
	CProgressDialog progress(work, original.sets.GetCount());
	progress.StartDialog();

	//convert here
	CFileHeaderDataV0 *old_header = original.header;
	CFileHeaderDataV1 *new_header = NULL;
	SALIL_NEW(new_header, CFileHeaderDataV1);

	strcpy(new_header->m_consignee, old_header->m_consignee);
	strcpy(new_header->m_country, old_header->m_country);
	new_header->m_currency_conversion = old_header->m_currency_conversion;
	strcpy(new_header->m_currency_name, old_header->m_currency_name);
	new_header->m_dollar_rate = old_header->m_dollar_rate;
	strcpy(new_header->m_exporter, old_header->m_exporter);
	strcpy(new_header->m_exports_diamond_rate, old_header->m_exports_diamond_rate);
	strcpy(new_header->m_exports_labour_cost, old_header->m_exports_labour_cost);
	strcpy(new_header->m_exports_pearl_rate, old_header->m_exports_pearl_rate);
	strcpy(new_header->m_exports_stone_rate, old_header->m_exports_stone_rate);
	new_header->m_gold_rate = old_header->m_gold_rate;
	strcpy(new_header->m_invoice_number, old_header->m_invoice_number);
	strcpy(new_header->m_price_quote, old_header->m_price_quote);
	new_header->m_type = old_header->m_type;
	strcpy(new_header->m_version, "V001");

	final.header = new_header;

	POSITION setPos = original.sets.GetHeadPosition();
	while(setPos)
	{
		CProductSetNodeV0 *old_set = original.sets.GetAt(setPos);
		if(old_set)
		{
			CProductSetNodeV1 *new_set = NULL;
			SALIL_NEW(new_set, CProductSetNodeV1);

			strcpy(new_set->m_description, old_set->m_description);
			new_set->m_piece_count = old_set->m_piece_count;
			strcpy(new_set->m_productID, old_set->m_productID);

			final.sets.AddTail(new_set);

			POSITION itemPos = old_set->items.GetHeadPosition();
			while(itemPos)
			{
				CProductItemNodeV0 *old_item = old_set->items.GetAt(itemPos);
				if(old_item)
				{
					CProductItemNodeV1 *new_item = NULL;
					SALIL_NEW(new_item, CProductItemNodeV1);

					strcpy(new_item->m_description, old_item->m_description);
					strcpy(new_item->m_diamond_marked_value, old_item->m_diamond_marked_value);
					new_item->m_diamond_type_count = old_item->m_diamond_type_count;
					new_item->m_gold_rate = old_item->m_gold_rate;
					new_item->m_gross_weight = old_item->m_gross_weight;
					new_item->m_image_size = old_item->m_image_size;
					new_item->m_labour_cost = old_item->m_labour_cost;
					strcpy(new_item->m_labour_marked_cost, old_item->m_labour_marked_cost);
					strcpy(new_item->m_old_serials, old_item->m_old_serials);
					new_item->m_other_weight = old_item->m_other_weight;
					strcpy(new_item->m_pearl_marked_value, old_item->m_pearl_marked_value);
					new_item->m_pearl_rate = old_item->m_pearl_rate;
					new_item->m_pearl_weight = old_item->m_pearl_weight;
					strcpy(new_item->m_photo_filename, old_item->m_photo_filename);
					new_item->m_piece_count = old_item->m_piece_count;
					strcpy(new_item->m_productID, old_item->m_productID);
					new_item->m_serial_number = old_item->m_serial_number;
					new_item->m_sold = old_item->m_sold;
					strcpy(new_item->m_sold_country, old_item->m_sold_country);
					strcpy(new_item->m_stone_marked_value, old_item->m_stone_marked_value);
					new_item->m_stone_type_count = old_item->m_stone_type_count;
					new_item->m_use_net_weight_for_gold = old_item->m_use_net_weight_for_gold;

					new_set->items.AddTail(new_item);

					POSITION stonePos = old_item->stones.GetHeadPosition();
					while(stonePos)
					{
						CStoneTypeDataV0 *old_stone = old_item->stones.GetAt(stonePos);
						if(old_stone)
						{
							CStoneTypeDataV1 *new_stone = NULL;
							SALIL_NEW(new_stone, CStoneTypeDataV1);

							new_stone->m_rate = old_stone->m_rate;
							strcpy(new_stone->m_type, old_stone->m_type);
							new_stone->m_weight = old_stone->m_weight;

							new_item->stones.AddTail(new_stone);
						}
						else
							SHOW_INTERNAL_ERROR;

						old_item->stones.GetNext(stonePos);
					}

					if(old_item->image)
					{
						BYTE *new_image = NULL;
						SALIL_NEW(new_image, BYTE[new_item->m_image_size]);
						memcpy(new_image, old_item->image, new_item->m_image_size);
						new_item->image = new_image;
					}
				}
				else
					SHOW_INTERNAL_ERROR;

				old_set->items.GetNext(itemPos);
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		progress.ProgressForward();
		original.sets.GetNext(setPos);
	}

	progress.FinishDialog();

	err = WriteV1(&final, &outputfile);
	if(err == Error)
	{
		CString errstr = "Could not write V1 voucher info to file '" + output_filename + "' while converting file '" + input_filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		RETURN_AFTER_ERROR;
	}
	
	inputfile.Close();
	outputfile.Close();
	
	if(!MoveDatabaseFile(output_filename, input_filename))
	{
		CString errstr = "Could not move voucher file '" + output_filename + "' to '" + input_filename + "' after conversion.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}

BOOL CDatabaseConverter::ConvertV1toV2()
{
	CFile inputfile;
	CFile outputfile;
	CFileException openerror;
	CString input_filename = m_filename;
	CString output_filename = gFilePath + "\\_temp_converter_file.tmp";

	if(input_filename.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!inputfile.Open(LPCTSTR(input_filename), CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",openerror.m_cause);
		errstr = "Could not open file '" + input_filename + "' for converting from V1 to V2.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!outputfile.Open(LPCTSTR(output_filename), CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",openerror.m_cause);
		errstr = "Could not open temporary file '" + output_filename + "' while converting file '" + input_filename + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		inputfile.Close();
		return FALSE;
	}

	try
	{
		inputfile.SeekToBegin();
		outputfile.SeekToBegin();
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		SHOW_INTERNAL_ERROR_REASON(errstr);
		RETURN_AFTER_ERROR;
	}

	CDbV1 original;
	CDbV2 final;

	FileError err = ReadV1(&original, &inputfile);
	if(err == Error)
	{
		CString errstr = "Could not read V1 voucher info from file '" + input_filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		RETURN_AFTER_ERROR;
	}

	CString work = "Converting database '" + m_filename + "' from V1 to V2";
	CProgressDialog progress(work, original.sets.GetCount());
	progress.StartDialog();

	//convert here
	CProductFileHeaderV2 *new_header = NULL;
	SALIL_NEW(new_header, CProductFileHeaderV2);
	strcpy(new_header->m_version, "V002");
	final.header = new_header;

	CFileHeaderDataV1 *old_header = original.header;
	CProductVoucherNodeV2 *new_voucher = NULL;
	SALIL_NEW(new_voucher, CProductVoucherNodeV2);

	strcpy(new_voucher->m_consignee, old_header->m_consignee);
	strcpy(new_voucher->m_country, old_header->m_country);
	new_voucher->m_currency_conversion = old_header->m_currency_conversion;
	strcpy(new_voucher->m_currency_name, old_header->m_currency_name);
	new_voucher->m_dollar_rate = old_header->m_dollar_rate;
	strcpy(new_voucher->m_exporter, old_header->m_exporter);
	strcpy(new_voucher->m_diamond_rate, old_header->m_exports_diamond_rate);
	strcpy(new_voucher->m_labour_cost, old_header->m_exports_labour_cost);
	strcpy(new_voucher->m_pearl_rate, old_header->m_exports_pearl_rate);
	strcpy(new_voucher->m_stone_rate, old_header->m_exports_stone_rate);
	new_voucher->m_gold_rate = old_header->m_gold_rate;
	strcpy(new_voucher->m_invoice_number, old_header->m_invoice_number);
	strcpy(new_voucher->m_price_quote, old_header->m_price_quote);
	new_voucher->m_type = old_header->m_type;
	new_voucher->m_set_count = original.sets.GetCount();

	{
		//Till V1 we had one file per voucher
		CString vouchername = GetBaseName(inputfile.GetFileName());
		int pos = vouchername.ReverseFind('.');
		if(pos >= 0)
			vouchername = vouchername.Left(pos);

		strcpy(new_voucher->m_voucher_name, LPCTSTR(vouchername));
	}

	final.vouchers.AddTail(new_voucher);

	POSITION setPos = original.sets.GetHeadPosition();
	while(setPos)
	{
		CProductSetNodeV1 *old_set = original.sets.GetAt(setPos);
		if(old_set)
		{
			CProductSetNodeV2 *new_set = NULL;
			SALIL_NEW(new_set, CProductSetNodeV2);

			strcpy(new_set->m_description, old_set->m_description);
			new_set->m_item_count = old_set->m_piece_count;
			strcpy(new_set->m_product_ID, old_set->m_productID);

			new_voucher->sets.AddTail(new_set);

			POSITION itemPos = old_set->items.GetHeadPosition();
			while(itemPos)
			{
				CProductItemNodeV1 *old_item = old_set->items.GetAt(itemPos);
				if(old_item)
				{
					CProductItemNodeV2 *new_item = NULL;
					SALIL_NEW(new_item, CProductItemNodeV2);

					strcpy(new_item->m_description, old_item->m_description);
					strcpy(new_item->m_diamond_marked_value, old_item->m_diamond_marked_value);
					new_item->m_diamond_count = old_item->m_diamond_type_count;
					new_item->m_gold_rate = old_item->m_gold_rate;
					new_item->m_gross_weight_final = old_item->m_gross_weight;
					new_item->m_labour_cost = old_item->m_labour_cost;
					strcpy(new_item->m_labour_marked_cost, old_item->m_labour_marked_cost);
					new_item->m_other_weight = old_item->m_other_weight;
					strcpy(new_item->m_pearl_marked_value, old_item->m_pearl_marked_value);
					new_item->m_piece_count = old_item->m_piece_count;
					strcpy(new_item->m_product_ID, old_item->m_productID);
					new_item->m_serial_number = old_item->m_serial_number;
					new_item->m_sold = old_item->m_sold;
					strcpy(new_item->m_sold_country, old_item->m_sold_country);
					strcpy(new_item->m_stone_marked_value, old_item->m_stone_marked_value);
					new_item->m_stone_count = old_item->m_stone_type_count;
					new_item->m_use_net_weight_for_gold = old_item->m_use_net_weight_for_gold;
					strcpy(new_item->m_approval_number, "");
					new_item->m_costing = 0;
					new_item->m_gross_weight_polished_after_setting = 0.0;
					new_item->m_gross_weight_polished_before_setting = 0.0;
					new_item->m_gross_weight_unpolished = 0.0;
					strcpy(new_item->m_marked_costing,"=COS");
					strcpy(new_item->m_worker, "");

					//add the new pearl into the stone list
					if((old_item->m_pearl_rate != 0) || (old_item->m_pearl_weight != 0.0))
					{
						new_item->m_pearl_count = 1;
						
						CProductSubItemDataV2 *new_pearl = NULL;
						SALIL_NEW(new_pearl, CProductSubItemDataV2);

						new_pearl->m_count = 0;
						new_pearl->m_rate = old_item->m_pearl_rate;
						strcpy(new_pearl->m_type, "P");
						new_pearl->m_weight = old_item->m_pearl_weight;

						new_item->stones.AddTail(new_pearl);
					}

					//change the old serials stored from string to a proper list
					{
						CString oldSerials = old_item->m_old_serials;
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

								CProductOldSerialDataV2 *new_serial = NULL;
								SALIL_NEW(new_serial, CProductOldSerialDataV2);

								new_serial->m_serial = atoi(LPCTSTR(number));
								strcpy(new_serial->m_tagname, LPCTSTR(name));
								new_item->serials.AddTail(new_serial);
								new_item->m_old_serial_count++;
							}
							else
							{
								break;
							}
						}
					}

					new_set->items.AddTail(new_item);

					POSITION stonePos = old_item->stones.GetHeadPosition();
					while(stonePos)
					{
						CStoneTypeDataV1 *old_stone = old_item->stones.GetAt(stonePos);
						if(old_stone)
						{
							CProductSubItemDataV2 *new_stone = NULL;
							SALIL_NEW(new_stone, CProductSubItemDataV2);

							new_stone->m_count = 1;
							new_stone->m_rate = old_stone->m_rate;
							strcpy(new_stone->m_type, old_stone->m_type);
							new_stone->m_weight = old_stone->m_weight;

							new_item->stones.AddTail(new_stone);
						}
						else
							SHOW_INTERNAL_ERROR;

						old_item->stones.GetNext(stonePos);
					}

					if(old_item->image)
					{
						CString extension = old_item->m_photo_filename;
						int pos = extension.ReverseFind('.');
						if(pos == -1)
							extension = "";
						else
							extension = extension.Right(extension.GetLength() - pos);

						if(extension.IsEmpty())
							extension = ".jpg";

						CString filename = new_item->m_product_ID + extension;
						if(SaveImage(&filename, old_item->image, old_item->m_image_size, FALSE))
						{
							filename = GetBaseName(filename);
							strcpy(new_item->m_photo_filename, LPCTSTR(filename));
						}
						else
						{
							CString errstr = "Could not save image file '" + filename + "' for item '" + CString(new_item->m_product_ID) + "' in set '" + CString(new_set->m_product_ID) + "' in voucher '" + CString(new_voucher->m_voucher_name) + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);
							RETURN_AFTER_ERROR;
						}
					}
					else
					{
						strcpy(new_item->m_photo_filename, "");
					}
				}
				else
					SHOW_INTERNAL_ERROR;

				old_set->items.GetNext(itemPos);
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		progress.ProgressForward();
		original.sets.GetNext(setPos);
	}

	progress.FinishDialog();

	err = WriteV2(&final, &outputfile);
	if(err == Error)
	{
		CString errstr = "Could not write V2 voucher info to file '" + output_filename + "' while converting file '" + input_filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		RETURN_AFTER_ERROR;
	}
	
	inputfile.Close();
	outputfile.Close();

	if(!MoveDatabaseFile(output_filename, input_filename))
	{
		CString errstr = "Could not move voucher file '" + output_filename + "' to '" + input_filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}


BOOL CDatabaseConverter::ConvertV2toV3()
{
	CFile inputfile;
	CFile outputfile;
	CFileException openerror;
	CString input_filename = m_filename;
	CString output_filename = gFilePath + "\\_temp_converter_file.tmp";

	if(input_filename.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!inputfile.Open(LPCTSTR(input_filename), CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",openerror.m_cause);
		errstr = "Could not open file '" + input_filename + "' for converting from V2 to V3.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!outputfile.Open(LPCTSTR(output_filename), CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary, &openerror))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",openerror.m_cause);
		errstr = "Could not open temporary file '" + output_filename + "' while converting file '" + input_filename + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		inputfile.Close();
		return FALSE;
	}

	try
	{
		inputfile.SeekToBegin();
		outputfile.SeekToBegin();
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		SHOW_INTERNAL_ERROR_REASON(errstr);
		RETURN_AFTER_ERROR;
	}

	CDbV2 original;
	CDbV3 final;

	FileError err = ReadV2(&original, &inputfile);
	if(err == Error)
	{
		CString errstr = "Could not read V2 voucher info from file '" + input_filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		RETURN_AFTER_ERROR;
	}

	CString work = "Converting database '" + m_filename + "' from V2 to V3";
	CProgressDialog progress(work, original.vouchers.GetCount());
	progress.StartDialog();

	//convert here
	CProductFileHeaderV3 *new_header = NULL;
	SALIL_NEW(new_header, CProductFileHeaderV3);
	strcpy(new_header->m_version, "V003");
	final.header = new_header;

	POSITION voucherPos = original.vouchers.GetHeadPosition();
	while(voucherPos)
	{
		CProductVoucherNodeV2 *old_voucher = original.vouchers.GetAt(voucherPos);
		if(old_voucher)
		{
			CProductVoucherNodeV3 *new_voucher = NULL;
			SALIL_NEW(new_voucher, CProductVoucherNodeV3);

			strcpy(new_voucher->m_consignee, old_voucher->m_consignee);
			strcpy(new_voucher->m_country, old_voucher->m_country);
			new_voucher->m_currency_conversion = old_voucher->m_currency_conversion;
			strcpy(new_voucher->m_currency_name, old_voucher->m_currency_name);
			strcpy(new_voucher->m_diamond_rate, old_voucher->m_diamond_rate);
			new_voucher->m_dollar_rate = old_voucher->m_dollar_rate;
			strcpy(new_voucher->m_exporter, old_voucher->m_exporter);
			new_voucher->m_gold_rate = old_voucher->m_gold_rate;
			strcpy(new_voucher->m_invoice_number, old_voucher->m_invoice_number);
			strcpy(new_voucher->m_labour_cost, old_voucher->m_labour_cost);
			strcpy(new_voucher->m_pearl_rate, old_voucher->m_pearl_rate);
			strcpy(new_voucher->m_price_quote, old_voucher->m_price_quote);
			new_voucher->m_set_count = old_voucher->m_set_count;
			strcpy(new_voucher->m_stone_rate, old_voucher->m_stone_rate);
			new_voucher->m_type = old_voucher->m_type;
			strcpy(new_voucher->m_voucher_name, old_voucher->m_voucher_name);

			final.vouchers.AddTail(new_voucher);

			POSITION setPos = old_voucher->sets.GetHeadPosition();
			while(setPos)
			{
				CProductSetNodeV2 *old_set = old_voucher->sets.GetAt(setPos);
				if(old_set)
				{
					CProductSetNodeV3 *new_set = NULL;
					SALIL_NEW(new_set, CProductSetNodeV3);

					strcpy(new_set->m_description, old_set->m_description);
					new_set->m_item_count = old_set->m_item_count;
					strcpy(new_set->m_product_ID, old_set->m_product_ID);

					new_voucher->sets.AddTail(new_set);

					POSITION itemPos = old_set->items.GetHeadPosition();
					while(itemPos)
					{
						CProductItemNodeV2 *old_item = old_set->items.GetAt(itemPos);
						if(old_item)
						{
							CProductItemNodeV3 *new_item = NULL;
							SALIL_NEW(new_item, CProductItemNodeV3);

							new_item->m_costing = old_item->m_costing;
							strcpy(new_item->m_description, old_item->m_description);
							new_item->m_diamond_count = old_item->m_diamond_count;
							strcpy(new_item->m_diamond_marked_value, old_item->m_diamond_marked_value);
							new_item->m_gold_rate = old_item->m_gold_rate;
							new_item->m_gross_weight = old_item->m_gross_weight_final;
							new_item->m_labour_cost = old_item->m_labour_cost;
							strcpy(new_item->m_labour_marked_cost, old_item->m_labour_marked_cost);
							strcpy(new_item->m_marked_costing, old_item->m_marked_costing);
							new_item->m_old_serial_count = old_item->m_old_serial_count;
							new_item->m_other_weight = old_item->m_other_weight;
							new_item->m_pearl_count = old_item->m_pearl_count;
							strcpy(new_item->m_pearl_marked_value, old_item->m_pearl_marked_value);
							strcpy(new_item->m_photo_filename, old_item->m_photo_filename);
							new_item->m_piece_count = old_item->m_piece_count;
							strcpy(new_item->m_product_ID, old_item->m_product_ID);
							new_item->m_serial_number = old_item->m_serial_number;
							new_item->m_sold = old_item->m_sold;
							strcpy(new_item->m_sold_country, old_item->m_sold_country);
							strcpy(new_item->m_stone_marked_value, old_item->m_stone_marked_value);
							new_item->m_stone_count = old_item->m_stone_count;
							new_item->m_use_net_weight_for_gold = old_item->m_use_net_weight_for_gold;
							strcpy(new_item->m_diamond_clarity, "");

							new_set->items.AddTail(new_item);

							POSITION serialPos = old_item->serials.GetHeadPosition();
							while(serialPos)
							{
								CProductOldSerialDataV2 *old_serial = old_item->serials.GetAt(serialPos);
								if(old_serial)
								{
									CProductOldSerialDataV3 *new_serial = NULL;
									SALIL_NEW(new_serial, CProductOldSerialDataV3);

									new_serial->m_serial = old_serial->m_serial;
									strcpy(new_serial->m_tagname, old_serial->m_tagname);

									new_item->serials.AddTail(new_serial);
								}
								else
									SHOW_INTERNAL_ERROR;

								old_item->serials.GetNext(serialPos);
							}

							POSITION stonePos = old_item->stones.GetHeadPosition();
							while(stonePos)
							{
								CProductSubItemDataV2 *old_stone = old_item->stones.GetAt(stonePos);
								if(old_stone)
								{
									CProductSubItemDataV3 *new_stone = NULL;
									SALIL_NEW(new_stone, CProductSubItemDataV3);

									new_stone->m_count = old_stone->m_count;
									new_stone->m_rate = old_stone->m_rate;
									strcpy(new_stone->m_type, old_stone->m_type);
									new_stone->m_weight = old_stone->m_weight;

									new_item->stones.AddTail(new_stone);
								}
								else
									SHOW_INTERNAL_ERROR;

								old_item->stones.GetNext(stonePos);
							}
						}
						else
							SHOW_INTERNAL_ERROR;

						old_set->items.GetNext(itemPos);
					}
				}
				else
					SHOW_INTERNAL_ERROR;

				old_voucher->sets.GetNext(setPos);
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		original.vouchers.GetNext(voucherPos);
	}

	progress.FinishDialog();

	err = WriteV3(&final, &outputfile);
	if(err == Error)
	{
		CString errstr = "Could not write V3 voucher info to file '" + output_filename + "' while converting file '" + input_filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		RETURN_AFTER_ERROR;
	}
	
	inputfile.Close();
	outputfile.Close();

	if(!MoveDatabaseFile(output_filename, input_filename))
	{
		CString errstr = "Could not move voucher file '" + output_filename + "' to '" + input_filename + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}


FileError CDatabaseConverter::ReadV0(CDbV0 *db, CFile *infile)
{
	if(!db || !infile)
	{
		SHOW_INTERNAL_ERROR;
		return Error;
	}

	CString work = "Reading V0 database '" + infile->GetFileName() + "'";
	CProgressDialog progress(work, infile->GetLength());
	progress.StartDialog();

	DWORD readcount;

	//read file header
	CFileHeaderDataV0 *header_buf = NULL;
	SALIL_NEW(header_buf, CFileHeaderDataV0);

	try
	{
		readcount = infile->Read(header_buf,sizeof(CFileHeaderDataV0));
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		errstr = "Could not read header from file '" + infile->GetFileName() + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		delete header_buf;
		return Error;
	}

	if(readcount < sizeof(CFileHeaderDataV0))
	{
		delete header_buf;
		return EndOfFile;
	}

	//DECODE(&buffer, CFileHeaderDataV0);
	db->header = header_buf;
	progress.ProgressForward(sizeof(CFileHeaderDataV0));

	//read sets
	while(TRUE)
	{
		CProductSetNodeV0 *set_buf = NULL;
		SALIL_NEW(set_buf, CProductSetNodeV0);

		try
		{
			readcount = infile->Read(set_buf,sizeof(CProductSetDataV0));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.",readerror->m_cause);
			errstr = "Could not read set info from file '" + infile->GetFileName() + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			delete set_buf;
			return Error;
		}

		if(readcount < sizeof(CProductSetDataV0))
		{
			delete set_buf;
			return EndOfFile;
		}

		//DECODE(&buffer, CProductSetDataV0);
		db->sets.AddTail(set_buf);
		progress.ProgressForward(sizeof(CProductSetDataV0));

		//read items
		int item_count = set_buf->m_piece_count;
		while(item_count > 0)
		{
			CProductItemNodeV0 *item_buf = NULL;
			SALIL_NEW(item_buf, CProductItemNodeV0);

			try
			{
				readcount = infile->Read(item_buf, sizeof(CProductItemDataV0));
			}
			catch(CFileException *readerror)
			{
				CString errstr;
				errstr.Format("ERROR : #%d.",readerror->m_cause);
				errstr = "Could not read item info from file '" + infile->GetFileName() + "' for set '" + CString(set_buf->m_productID) + "'.\n" + errstr;
				SHOW_INTERNAL_ERROR_REASON(errstr);
				delete item_buf;
				return Error;
			}

			if(readcount < sizeof(CProductItemDataV0))
			{
				CString errstr = "Could not read all items from file '" + infile->GetFileName() + "' for set '" + CString(set_buf->m_productID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				delete item_buf;
				return Error;
			}

			//DECODE(&buffer, CProductItemDataV0);
			set_buf->items.AddTail(item_buf);
			progress.ProgressForward(sizeof(CProductItemDataV0));

			//read stones
			int stone_count = item_buf->m_diamond_type_count + item_buf->m_stone_type_count;
			while(stone_count > 0)
			{
				CStoneTypeDataV0 *stone_buf = NULL;
				SALIL_NEW(stone_buf, CStoneTypeDataV0);

				try
				{
					readcount = infile->Read(stone_buf, sizeof(CStoneTypeDataV0));
				}
				catch(CFileException *readerror)
				{
					CString errstr;
					errstr.Format("ERROR : #%d.",readerror->m_cause);
					errstr = "Could not read subitem info from file '" + infile->GetFileName() + "' for item '" + CString(item_buf->m_productID) + "' in set '" + CString(set_buf->m_productID) + "'.\n" + errstr;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete stone_buf;
					return Error;
				}

				if(readcount < sizeof(CStoneTypeDataV0))
				{
					CString errstr = "Could not read all subitems from file '" + infile->GetFileName() + "' for item '" + CString(item_buf->m_productID) + "' in set '" + CString(set_buf->m_productID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete stone_buf;
					return Error;
				}

				//DECODE(&buffer, CStoneTypeDataV0);
				item_buf->stones.AddTail(stone_buf);
				progress.ProgressForward(sizeof(CStoneTypeDataV0));

				stone_count--;
			}
			
			if(item_buf->m_image_size > 0)
			{
				BYTE *imageData = NULL;
				SALIL_NEW(imageData, BYTE[item_buf->m_image_size]);

				try
				{
					readcount = infile->Read(imageData, item_buf->m_image_size);
				}
				catch(CFileException *readerror)
				{
					CString errstr;
					errstr.Format("ERROR : #%d.",readerror->m_cause);
					errstr = "Could not read imagedata from file '" + infile->GetFileName() + "' for item '" + CString(item_buf->m_productID) + "' in set '" + CString(set_buf->m_productID) + "'.\n" + errstr;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete imageData;
					return Error;
				}

				if(readcount < item_buf->m_image_size)
				{
					CString errstr = "Could not read complete image from file '" + infile->GetFileName() + "' for item '" + CString(item_buf->m_productID) + "' in set '" + CString(set_buf->m_productID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete imageData;
					return Error;
				}

				item_buf->image = imageData;
				progress.ProgressForward(item_buf->m_image_size);
			}

			item_count--;
		}
	}

	return NoError;
}


FileError CDatabaseConverter::ReadV1(CDbV1 *db, CFile *infile)
{
	if(!db || !infile)
	{
		SHOW_INTERNAL_ERROR;
		return Error;
	}

	CString work = "Reading V1 database '" + infile->GetFileName() + "'";
	CProgressDialog progress(work, infile->GetLength());
	progress.StartDialog();

	DWORD readcount;

	//read file header
	CFileHeaderDataV1 *header_buf = NULL;
	SALIL_NEW(header_buf, CFileHeaderDataV1);

	try
	{
		readcount = infile->Read(header_buf,sizeof(CFileHeaderDataV1));
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		errstr = "Could not read header from file '" + infile->GetFileName() + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		delete header_buf;
		return Error;
	}

	if(readcount < sizeof(CFileHeaderDataV1))
	{
		delete header_buf;
		return EndOfFile;
	}

	DECODE(header_buf, CFileHeaderDataV1);
	db->header = header_buf;
	progress.ProgressForward(sizeof(CFileHeaderDataV1));

	//read sets
	while(TRUE)
	{
		CProductSetNodeV1 *set_buf = NULL;
		SALIL_NEW(set_buf, CProductSetNodeV1);

		try
		{
			readcount = infile->Read(set_buf,sizeof(CProductSetDataV1));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.",readerror->m_cause);
			errstr = "Could not read set info from file '" + infile->GetFileName() + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			delete set_buf;
			return Error;
		}

		if(readcount < sizeof(CProductSetDataV1))
		{
			delete set_buf;
			return EndOfFile;
		}

		DECODE(set_buf, CProductSetDataV1);
		db->sets.AddTail(set_buf);
		progress.ProgressForward(sizeof(CProductSetDataV1));

		//read items
		int item_count = set_buf->m_piece_count;
		while(item_count > 0)
		{
			CProductItemNodeV1 *item_buf = NULL;
			SALIL_NEW(item_buf, CProductItemNodeV1);

			try
			{
				readcount = infile->Read(item_buf, sizeof(CProductItemDataV1));
			}
			catch(CFileException *readerror)
			{
				CString errstr;
				errstr.Format("ERROR : #%d.",readerror->m_cause);
				errstr = "Could not read item info from file '" + infile->GetFileName() + "' for set '" + CString(set_buf->m_productID) + "'.\n" + errstr;
				SHOW_INTERNAL_ERROR_REASON(errstr);
				delete item_buf;
				return Error;
			}

			if(readcount < sizeof(CProductItemDataV1))
			{
				CString errstr = "Could not read all items from file '" + infile->GetFileName() + "' for set '" + CString(set_buf->m_productID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				delete item_buf;
				return Error;
			}

			DECODE(item_buf, CProductItemDataV1);
			set_buf->items.AddTail(item_buf);
			progress.ProgressForward(sizeof(CProductItemDataV1));

			//read stones
			int stone_count = item_buf->m_diamond_type_count + item_buf->m_stone_type_count;
			while(stone_count > 0)
			{
				CStoneTypeDataV1 *stone_buf = NULL;
				SALIL_NEW(stone_buf, CStoneTypeDataV1);

				try
				{
					readcount = infile->Read(stone_buf, sizeof(CStoneTypeDataV1));
				}
				catch(CFileException *readerror)
				{
					CString errstr;
					errstr.Format("ERROR : #%d.",readerror->m_cause);
					errstr = "Could not read subitem info from file '" + infile->GetFileName() + "'. for item '" + CString(item_buf->m_productID) + "' in set '" + CString(set_buf->m_productID) + "'\n" + errstr;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete stone_buf;
					return Error;
				}

				if(readcount < sizeof(CStoneTypeDataV1))
				{
					CString errstr = "Could not read all subitems from file '" + infile->GetFileName() + "' for item '" + CString(item_buf->m_productID) + "' in set '" + CString(set_buf->m_productID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete stone_buf;
					return Error;
				}

				DECODE(stone_buf, CStoneTypeDataV1);
				item_buf->stones.AddTail(stone_buf);
				progress.ProgressForward(sizeof(CStoneTypeDataV1));

				stone_count--;
			}
			
			if(item_buf->m_image_size > 0)
			{
				BYTE *imageData = NULL;
				SALIL_NEW(imageData, BYTE[item_buf->m_image_size]);

				try
				{
					readcount = infile->Read(imageData, item_buf->m_image_size);
				}
				catch(CFileException *readerror)
				{
					CString errstr;
					errstr.Format("ERROR : #%d.",readerror->m_cause);
					errstr = "Could not read image data from file '" + infile->GetFileName() + "' for item '" + CString(item_buf->m_productID) + "' in set '" + CString(set_buf->m_productID) + "'.\n" + errstr;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete imageData;
					return Error;
				}

				if(readcount < item_buf->m_image_size)
				{
					CString errstr = "Could not read complete image from file '" + infile->GetFileName() + "' for item '" + CString(item_buf->m_productID) + "' in set '" + CString(set_buf->m_productID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete imageData;
					return Error;
				}

				item_buf->image = imageData;
				progress.ProgressForward(item_buf->m_image_size);
			}

			item_count--;
		}
	}

	return NoError;
}


FileError CDatabaseConverter::ReadV2(CDbV2 *db, CFile *infile)
{
	if(!db || !infile)
	{
		SHOW_INTERNAL_ERROR;
		return Error;
	}

	CString work = "Reading V2 database '" + infile->GetFileName() + "'";
	CProgressDialog progress(work, infile->GetLength());
	progress.StartDialog();

	DWORD readcount;

	//read file header
	CProductFileHeaderV2 *header_buf = NULL;
	SALIL_NEW(header_buf, CProductFileHeaderV2);

	try
	{
		readcount = infile->Read(header_buf,sizeof(CProductFileHeaderV2));
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		errstr = "Could not read header from file '" + infile->GetFileName() + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		delete header_buf;
		return Error;
	}

	if(readcount < sizeof(CProductFileHeaderV2))
	{
		delete header_buf;
		return EndOfFile;
	}

	DECODE(header_buf, CProductFileHeaderV2);
	db->header = header_buf;
	progress.ProgressForward(sizeof(CProductFileHeaderV2));

	//read vouchers
	while(TRUE)
	{
		CProductVoucherNodeV2 *voucher_buf = NULL;
		SALIL_NEW(voucher_buf, CProductVoucherNodeV2);

		try
		{
			readcount = infile->Read(voucher_buf,sizeof(CProductVoucherDataV2));
		}
		catch(CFileException *readerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.",readerror->m_cause);
			errstr = "Could not read voucher info from file '" + infile->GetFileName() + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			delete voucher_buf;
			return Error;
		}

		if(readcount < sizeof(CProductVoucherDataV2))
		{
			delete voucher_buf;
			return EndOfFile;
		}

		DECODE(voucher_buf, CProductVoucherDataV2);
		db->vouchers.AddTail(voucher_buf);
		progress.ProgressForward(sizeof(CProductVoucherDataV2));

		//read sets
		int set_count = voucher_buf->m_set_count;
		while(set_count > 0)
		{
			CProductSetNodeV2 *set_buf = NULL;
			SALIL_NEW(set_buf, CProductSetNodeV2);

			try
			{
				readcount = infile->Read(set_buf,sizeof(CProductSetDataV2));
			}
			catch(CFileException *readerror)
			{
				CString errstr;
				errstr.Format("ERROR : #%d.",readerror->m_cause);
				errstr = "Could not read set info from file '" + infile->GetFileName() + "'.\n" + errstr;
				SHOW_INTERNAL_ERROR_REASON(errstr);
				delete set_buf;
				return Error;
			}

			if(readcount < sizeof(CProductSetDataV2))
			{
				CString errstr = "Could not read all sets from file '" + infile->GetFileName() + "' for set '" + CString(set_buf->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				delete set_buf;
				return EndOfFile;
			}

			DECODE(set_buf, CProductSetDataV2);
			voucher_buf->sets.AddTail(set_buf);
			progress.ProgressForward(sizeof(CProductSetDataV2));

			//read items
			int item_count = set_buf->m_item_count;
			while(item_count > 0)
			{
				CProductItemNodeV2 *item_buf = NULL;
				SALIL_NEW(item_buf, CProductItemNodeV2);

				try
				{
					readcount = infile->Read(item_buf, sizeof(CProductItemDataV2));
				}
				catch(CFileException *readerror)
				{
					CString errstr;
					errstr.Format("ERROR : #%d.",readerror->m_cause);
					errstr = "Could not read item info from file '" + infile->GetFileName() + "' for set '" + CString(set_buf->m_product_ID) + "'.\n" + errstr;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete item_buf;
					return Error;
				}

				if(readcount < sizeof(CProductItemDataV2))
				{
					CString errstr = "Could not read all items from file '" + infile->GetFileName() + "' for set '" + CString(set_buf->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					delete item_buf;
					return Error;
				}

				DECODE(item_buf, CProductItemDataV2);
				set_buf->items.AddTail(item_buf);
				progress.ProgressForward(sizeof(CProductItemDataV2));

				//read stones
				int stone_count = item_buf->m_pearl_count + item_buf->m_diamond_count + item_buf->m_stone_count;
				while(stone_count > 0)
				{
					CProductSubItemDataV2 *stone_buf = NULL;
					SALIL_NEW(stone_buf, CProductSubItemDataV2);

					try
					{
						readcount = infile->Read(stone_buf, sizeof(CProductSubItemDataV2));
					}
					catch(CFileException *readerror)
					{
						CString errstr;
						errstr.Format("ERROR : #%d.",readerror->m_cause);
						errstr = "Could not read subitem info from file '" + infile->GetFileName() + "'. for item '" + CString(item_buf->m_product_ID) + "' in set '" + CString(set_buf->m_product_ID) + "'\n" + errstr;
						SHOW_INTERNAL_ERROR_REASON(errstr);
						delete stone_buf;
						return Error;
					}

					if(readcount < sizeof(CProductSubItemDataV2))
					{
						CString errstr = "Could not read all subitems from file '" + infile->GetFileName() + "' for item '" + CString(item_buf->m_product_ID) + "' in set '" + CString(set_buf->m_product_ID) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						delete stone_buf;
						return Error;
					}

					DECODE(stone_buf, CProductSubItemDataV2);
					item_buf->stones.AddTail(stone_buf);
					progress.ProgressForward(sizeof(CProductSubItemDataV2));

					stone_count--;
				}

				//read serials
				int serial_count = item_buf->m_old_serial_count;
				while(serial_count > 0)
				{
					CProductOldSerialDataV2 *serial_buf = NULL;
					SALIL_NEW(serial_buf, CProductOldSerialDataV2);

					try
					{
						readcount = infile->Read(serial_buf, sizeof(CProductOldSerialDataV2));
					}
					catch(CFileException *readerror)
					{
						CString errstr;
						errstr.Format("ERROR : #%d.",readerror->m_cause);
						errstr = "Could not read subitem info from file '" + infile->GetFileName() + "'. for item '" + CString(item_buf->m_product_ID) + "' in set '" + CString(set_buf->m_product_ID) + "'\n" + errstr;
						SHOW_INTERNAL_ERROR_REASON(errstr);
						delete serial_buf;
						return Error;
					}

					if(readcount < sizeof(CProductOldSerialDataV2))
					{
						CString errstr = "Could not read all subitems from file '" + infile->GetFileName() + "' for item '" + CString(item_buf->m_product_ID) + "' in set '" + CString(set_buf->m_product_ID) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						delete serial_buf;
						return Error;
					}

					DECODE(serial_buf, CProductOldSerialDataV2);
					item_buf->serials.AddTail(serial_buf);
					progress.ProgressForward(sizeof(CProductOldSerialDataV2));

					serial_count--;
				}

				item_count--;
			}

			set_count--;
		}
	}

	return NoError;
}


FileError CDatabaseConverter::WriteV1(CDbV1 *db, CFile *outfile)
{
	if(!db || !outfile)
	{
		SHOW_INTERNAL_ERROR;
		return Error;
	}

	CString work = "Writing V1 database '" + outfile->GetFileName() + "'";
	CProgressDialog progress(work, (db->sets.GetCount() + 1));
	progress.StartDialog();

	ENCODE(db->header, CFileHeaderDataV1);

	try
	{
		outfile->Write(db->header, sizeof(CFileHeaderDataV1));
	}
	catch(CFileException *readerror)
	{
		DECODE(db->header, CFileHeaderDataV1);
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		errstr = "Could not write header to file '" + outfile->GetFileName() + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return Error;
	}

	DECODE(db->header, CFileHeaderDataV1);
	progress.ProgressForward();
	
	POSITION setPos = db->sets.GetHeadPosition();
	while(setPos)
	{
		CProductSetNodeV1 *set = db->sets.GetAt(setPos);
		if(set)
		{
			ENCODE(set, CProductSetDataV1);

			try
			{
				outfile->Write(set, sizeof(CProductSetDataV1));
			}
			catch(CFileException *readerror)
			{
				DECODE(set, CProductSetDataV1);
				CString errstr;
				errstr.Format("ERROR : #%d.",readerror->m_cause);
				errstr = "Could not write set info of '" + CString(set->m_productID) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return Error;
			}

			DECODE(set, CProductSetDataV1);

			if(set->m_piece_count != set->items.GetCount())
			{
				CString errstr = "Mismatch in the number of items and the count record for set '" + CString(set->m_productID) + "' while writing file '" + outfile->GetFileName() + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return Error;
			}

			POSITION itemPos = set->items.GetHeadPosition();
			while(itemPos)
			{
				CProductItemNodeV1 *item = set->items.GetAt(itemPos);
				if(item)
				{
					ENCODE(item, CProductItemDataV1);

					try
					{
						outfile->Write(item, sizeof(CProductItemDataV1));
					}
					catch(CFileException *readerror)
					{
						DECODE(item, CProductItemDataV1);
						CString errstr;
						errstr.Format("ERROR : #%d.",readerror->m_cause);
						errstr = "Could not write item info of '" + CString(item->m_productID) + "' in set '" + CString(set->m_productID) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return Error;
					}

					DECODE(item, CProductItemDataV1);

					if((item->m_diamond_type_count + item->m_stone_type_count) != item->stones.GetCount())
					{
						CString errstr = "Mismatch in the number of subitems and the count record for item '" + CString(item->m_productID) + "' in set '" + CString(set->m_productID) + "' while writing file '" + outfile->GetFileName() + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return Error;
					}

					POSITION stonePos = item->stones.GetHeadPosition();
					while(stonePos)
					{
						CStoneTypeDataV1 *stone = item->stones.GetAt(stonePos);
						if(stone)
						{
							ENCODE(stone, CStoneTypeDataV1);

							try
							{
								outfile->Write(stone, sizeof(CStoneTypeDataV1));
							}
							catch(CFileException *readerror)
							{
								DECODE(stone, CStoneTypeDataV1);
								CString errstr;
								errstr.Format("ERROR : #%d.",readerror->m_cause);
								errstr = "Could not write subitem info in item '" + CString(item->m_productID) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
								SHOW_INTERNAL_ERROR_REASON(errstr);
								return Error;
							}

							DECODE(stone, CStoneTypeDataV1);
						}
						else
							SHOW_INTERNAL_ERROR;

						item->stones.GetNext(stonePos);
					}

					if(item->image)
					{
						try
						{
							outfile->Write(item->image, item->m_image_size);
						}
						catch(CFileException *readerror)
						{
							CString errstr;
							errstr.Format("ERROR : #%d.",readerror->m_cause);
							errstr = "Could not write image data of item '" + CString(item->m_productID) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
							SHOW_INTERNAL_ERROR_REASON(errstr);
							return Error;
						}
					}
				}
				else
					SHOW_INTERNAL_ERROR;

				set->items.GetNext(itemPos);
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		progress.ProgressForward();
		db->sets.GetNext(setPos);
	}

	progress.FinishDialog();
	return NoError;
}


FileError CDatabaseConverter::WriteV2(CDbV2 *db, CFile *outfile)
{
	if(!db || !outfile)
	{
		SHOW_INTERNAL_ERROR;
		return Error;
	}

	CString work = "Writing V2 database '" + outfile->GetFileName() + "'";
	CProgressDialog progress(work, (db->vouchers.GetCount() + 1));
	progress.StartDialog();

	ENCODE(db->header, CProductFileHeaderV2);

	try
	{
		outfile->Write(db->header, sizeof(CProductFileHeaderV2));
	}
	catch(CFileException *readerror)
	{
		DECODE(db->header, CProductFileHeaderV2);
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		errstr = "Could not write header to file '" + outfile->GetFileName() + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return Error;
	}
	
	DECODE(db->header, CProductFileHeaderV2);
	progress.ProgressForward();

	POSITION voucherPos = db->vouchers.GetHeadPosition();
	while(voucherPos)
	{
		CProductVoucherNodeV2 *voucher = db->vouchers.GetAt(voucherPos);
		if(voucher)
		{
			progress.SetupDialog((voucher->sets.GetCount() + 1), FALSE);
			ENCODE(voucher, CProductVoucherDataV2);

			try
			{
				outfile->Write(voucher, sizeof(CProductVoucherDataV2));
			}
			catch(CFileException *readerror)
			{
				DECODE(voucher, CProductVoucherDataV2);
				CString errstr;
				errstr.Format("ERROR : #%d.",readerror->m_cause);
				errstr = "Could not write voucher info of '" + CString(voucher->m_voucher_name) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return Error;
			}

			DECODE(voucher, CProductVoucherDataV2);
			progress.ProgressForward();

			if(voucher->m_set_count != (DWORD)voucher->sets.GetCount())
			{
				CString errstr = "Mismatch in the number of sets and the count record for voucher '" + CString(voucher->m_voucher_name) + "' while writing file '" + outfile->GetFileName() + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return Error;
			}

			POSITION setPos = voucher->sets.GetHeadPosition();
			while(setPos)
			{
				CProductSetNodeV2 *set = voucher->sets.GetAt(setPos);
				if(set)
				{
					ENCODE(set, CProductSetDataV2);

					try
					{
						outfile->Write(set, sizeof(CProductSetDataV2));
					}
					catch(CFileException *readerror)
					{
						DECODE(set, CProductSetDataV2);
						CString errstr;
						errstr.Format("ERROR : #%d.",readerror->m_cause);
						errstr = "Could not write set info of '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return Error;
					}

					DECODE(set, CProductSetDataV2);

					if(set->m_item_count != set->items.GetCount())
					{
						CString errstr = "Mismatch in the number of items and the count record for set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' while writing file '" + outfile->GetFileName() + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return Error;
					}

					POSITION itemPos = set->items.GetHeadPosition();
					while(itemPos)
					{
						CProductItemNodeV2 *item = set->items.GetAt(itemPos);
						if(item)
						{
							ENCODE(item, CProductItemDataV2);

							try
							{
								outfile->Write(item, sizeof(CProductItemDataV2));
							}
							catch(CFileException *readerror)
							{
								DECODE(item, CProductItemDataV2);
								CString errstr;
								errstr.Format("ERROR : #%d.",readerror->m_cause);
								errstr = "Could not write item info of '" + CString(item->m_product_ID) + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
								SHOW_INTERNAL_ERROR_REASON(errstr);
								return Error;
							}

							DECODE(item, CProductItemDataV2);

							if((item->m_diamond_count + item->m_stone_count + item->m_pearl_count) != item->stones.GetCount())
							{
								CString errstr = "Mismatch in the number of subitems and the count record for item '" + CString(item->m_product_ID) + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' while writing file '" + outfile->GetFileName() + "'.";
								SHOW_INTERNAL_ERROR_REASON(errstr);
								return Error;
							}

							POSITION stonePos = item->stones.GetHeadPosition();
							while(stonePos)
							{
								CProductSubItemDataV2 *stone = item->stones.GetAt(stonePos);
								if(stone)
								{
									ENCODE(stone, CProductSubItemDataV2);

									try
									{
										outfile->Write(stone, sizeof(CProductSubItemDataV2));
									}
									catch(CFileException *readerror)
									{
										DECODE(stone, CProductSubItemDataV2);
										CString errstr;
										errstr.Format("ERROR : #%d.",readerror->m_cause);
										errstr = "Could not write subitem info if item '" + CString(item->m_product_ID) + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
										SHOW_INTERNAL_ERROR_REASON(errstr);
										return Error;
									}

									DECODE(stone, CProductSubItemDataV2);
								}
								else
									SHOW_INTERNAL_ERROR;

								item->stones.GetNext(stonePos);
							}

							if(item->m_old_serial_count != item->serials.GetCount())
							{
								CString errstr = "Mismatch in the number of old serials and the count record for item '" + CString(item->m_product_ID) + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' while writing file '" + outfile->GetFileName() + "'.";
								SHOW_INTERNAL_ERROR_REASON(errstr);
								return Error;
							}

							POSITION serialPos = item->serials.GetHeadPosition();
							while(serialPos)
							{
								CProductOldSerialDataV2 *serial = item->serials.GetAt(serialPos);
								if(serial)
								{
									ENCODE(serial, CProductOldSerialDataV2);

									try
									{
										outfile->Write(serial, sizeof(CProductOldSerialDataV2));
									}
									catch(CFileException *readerror)
									{
										DECODE(serial, CProductOldSerialDataV2);
										CString errstr;
										errstr.Format("ERROR : #%d.",readerror->m_cause);
										errstr = "Could not write old serial info of item '" + CString(item->m_product_ID) + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
										SHOW_INTERNAL_ERROR_REASON(errstr);
										return Error;
									}

									DECODE(serial, CProductOldSerialDataV2);
								}
								else
									SHOW_INTERNAL_ERROR;

								item->serials.GetNext(serialPos);
							}
						}
						else
							SHOW_INTERNAL_ERROR;

						set->items.GetNext(itemPos);
					}
				}
				else
					SHOW_INTERNAL_ERROR;

				progress.ProgressForward();
				voucher->sets.GetNext(setPos);
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		progress.ProgressForward();
		db->vouchers.GetNext(voucherPos);
	}

	return NoError;
}


FileError CDatabaseConverter::WriteV3(CDbV3 *db, CFile *outfile)
{
	if(!db || !outfile)
	{
		SHOW_INTERNAL_ERROR;
		return Error;
	}

	CString work = "Writing V3 database '" + outfile->GetFileName() + "'";
	CProgressDialog progress(work, (db->vouchers.GetCount() + 1));
	progress.StartDialog();

	ENCODE(db->header, CProductFileHeaderV3);

	try
	{
		outfile->Write(db->header, sizeof(CProductFileHeaderV3));
	}
	catch(CFileException *readerror)
	{
		DECODE(db->header, CProductFileHeaderV3);
		CString errstr;
		errstr.Format("ERROR : #%d.",readerror->m_cause);
		errstr = "Could not write header to file '" + outfile->GetFileName() + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return Error;
	}
	
	DECODE(db->header, CProductFileHeaderV3);
	progress.ProgressForward();

	POSITION voucherPos = db->vouchers.GetHeadPosition();
	while(voucherPos)
	{
		CProductVoucherNodeV3 *voucher = db->vouchers.GetAt(voucherPos);
		if(voucher)
		{
			progress.SetupDialog((voucher->sets.GetCount() + 1), FALSE);
			ENCODE(voucher, CProductVoucherDataV3);

			try
			{
				outfile->Write(voucher, sizeof(CProductVoucherDataV3));
			}
			catch(CFileException *readerror)
			{
				DECODE(voucher, CProductVoucherDataV3);
				CString errstr;
				errstr.Format("ERROR : #%d.",readerror->m_cause);
				errstr = "Could not write voucher info of '" + CString(voucher->m_voucher_name) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return Error;
			}

			DECODE(voucher, CProductVoucherDataV3);
			progress.ProgressForward();

			if(voucher->m_set_count != (DWORD)voucher->sets.GetCount())
			{
				CString errstr = "Mismatch in the number of sets and the count record for voucher '" + CString(voucher->m_voucher_name) + "' while writing file '" + outfile->GetFileName() + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return Error;
			}

			POSITION setPos = voucher->sets.GetHeadPosition();
			while(setPos)
			{
				CProductSetNodeV3 *set = voucher->sets.GetAt(setPos);
				if(set)
				{
					ENCODE(set, CProductSetDataV3);

					try
					{
						outfile->Write(set, sizeof(CProductSetDataV3));
					}
					catch(CFileException *readerror)
					{
						DECODE(set, CProductSetDataV3);
						CString errstr;
						errstr.Format("ERROR : #%d.",readerror->m_cause);
						errstr = "Could not write set info of '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return Error;
					}

					DECODE(set, CProductSetDataV3);

					if(set->m_item_count != set->items.GetCount())
					{
						CString errstr = "Mismatch in the number of items and the count record for set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' while writing file '" + outfile->GetFileName() + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return Error;
					}

					POSITION itemPos = set->items.GetHeadPosition();
					while(itemPos)
					{
						CProductItemNodeV3 *item = set->items.GetAt(itemPos);
						if(item)
						{
							ENCODE(item, CProductItemDataV3);

							try
							{
								outfile->Write(item, sizeof(CProductItemDataV3));
							}
							catch(CFileException *readerror)
							{
								DECODE(item, CProductItemDataV3);
								CString errstr;
								errstr.Format("ERROR : #%d.",readerror->m_cause);
								errstr = "Could not write item info of '" + CString(item->m_product_ID) + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
								SHOW_INTERNAL_ERROR_REASON(errstr);
								return Error;
							}

							DECODE(item, CProductItemDataV3);

							if((item->m_diamond_count + item->m_stone_count + item->m_pearl_count) != item->stones.GetCount())
							{
								CString errstr = "Mismatch in the number of subitems and the count record for item '" + CString(item->m_product_ID) + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' while writing file '" + outfile->GetFileName() + "'.";
								SHOW_INTERNAL_ERROR_REASON(errstr);
								return Error;
							}

							POSITION stonePos = item->stones.GetHeadPosition();
							while(stonePos)
							{
								CProductSubItemDataV3 *stone = item->stones.GetAt(stonePos);
								if(stone)
								{
									ENCODE(stone, CProductSubItemDataV3);

									try
									{
										outfile->Write(stone, sizeof(CProductSubItemDataV3));
									}
									catch(CFileException *readerror)
									{
										DECODE(stone, CProductSubItemDataV3);
										CString errstr;
										errstr.Format("ERROR : #%d.",readerror->m_cause);
										errstr = "Could not write subitem info if item '" + CString(item->m_product_ID) + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
										SHOW_INTERNAL_ERROR_REASON(errstr);
										return Error;
									}

									DECODE(stone, CProductSubItemDataV3);
								}
								else
									SHOW_INTERNAL_ERROR;

								item->stones.GetNext(stonePos);
							}

							if(item->m_old_serial_count != item->serials.GetCount())
							{
								CString errstr = "Mismatch in the number of old serials and the count record for item '" + CString(item->m_product_ID) + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' while writing file '" + outfile->GetFileName() + "'.";
								SHOW_INTERNAL_ERROR_REASON(errstr);
								return Error;
							}

							POSITION serialPos = item->serials.GetHeadPosition();
							while(serialPos)
							{
								CProductOldSerialDataV3 *serial = item->serials.GetAt(serialPos);
								if(serial)
								{
									ENCODE(serial, CProductOldSerialDataV3);

									try
									{
										outfile->Write(serial, sizeof(CProductOldSerialDataV3));
									}
									catch(CFileException *readerror)
									{
										DECODE(serial, CProductOldSerialDataV3);
										CString errstr;
										errstr.Format("ERROR : #%d.",readerror->m_cause);
										errstr = "Could not write old serial info of item '" + CString(item->m_product_ID) + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "' to file '" + outfile->GetFileName() + "'.\n" + errstr;
										SHOW_INTERNAL_ERROR_REASON(errstr);
										return Error;
									}

									DECODE(serial, CProductOldSerialDataV3);
								}
								else
									SHOW_INTERNAL_ERROR;

								item->serials.GetNext(serialPos);
							}
						}
						else
							SHOW_INTERNAL_ERROR;

						set->items.GetNext(itemPos);
					}
				}
				else
					SHOW_INTERNAL_ERROR;

				progress.ProgressForward();
				voucher->sets.GetNext(setPos);
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		progress.ProgressForward();
		db->vouchers.GetNext(voucherPos);
	}

	return NoError;
}

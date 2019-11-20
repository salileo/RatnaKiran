#if !defined(AFX_DATABASECONVERTER_H__76DFD6E7_672C_452F_804B_A6F34C7A9BA2__INCLUDED_)
#define AFX_DATABASECONVERTER_H__76DFD6E7_672C_452F_804B_A6F34C7A9BA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum FileError {NoError, EndOfFile, Error};

class CDatabaseConverter  
{
public:
	CDatabaseConverter(CString filename);
	virtual ~CDatabaseConverter();
	BOOL Convert();

public:

	//NO VERSION
	class CStoneTypeDataV0
	{
	public:
		CStoneTypeDataV0()
		{
			m_rate = 0;
			m_weight = 0.0;
			strcpy(m_type,"");
		}

		char m_type[SHORT_STRING];
		double m_weight; // in carets
		int m_rate;
	};

	class CProductItemDataV0
	{
	public:
		CProductItemDataV0()
		{
			m_serial_number = 0;

			strcpy(m_old_serials, "");
			strcpy(m_productID,"");
			strcpy(m_description,"");
			strcpy(m_photo_filename,"");
			m_piece_count = 0;

			m_gold_rate = -1;
			m_use_net_weight_for_gold = FALSE;

			m_gross_weight = 0.0;
			m_diamond_type_count = 0;
			m_stone_type_count = 0;
			m_pearl_weight = 0.0;
			m_pearl_rate = 0;
			m_other_weight = 0.0;
			m_labour_cost = 0;

			strcpy(m_diamond_marked_value,"=DV");
			strcpy(m_stone_marked_value,"=PRV");
			strcpy(m_pearl_marked_value,"=PV");
			strcpy(m_labour_marked_cost,"=LC");

			m_sold = FALSE;
			strcpy(m_sold_country, "");

			m_image_size = 0;
		}

		int m_serial_number;
		char m_old_serials[LONG_STRING];
		char m_productID[SHORT_STRING];
		char m_description[MEDIUM_STRING];
		char m_photo_filename[MEDIUM_STRING];
		int m_piece_count;

		int m_gold_rate;
		BOOL m_use_net_weight_for_gold;

		double m_gross_weight; // in grams
		int m_diamond_type_count;
		int m_stone_type_count;
		double m_pearl_weight; // in carets
		int m_pearl_rate;
		double m_other_weight; // in grams
		int m_labour_cost;

		char m_diamond_marked_value[MEDIUM_STRING];
		char m_stone_marked_value[MEDIUM_STRING];
		char m_pearl_marked_value[MEDIUM_STRING];
		char m_labour_marked_cost[MEDIUM_STRING];

		BOOL m_sold;
		char m_sold_country[MEDIUM_STRING];

		DWORD m_image_size;
	};

	class CProductItemNodeV0 : public CProductItemDataV0
	{
	public:
		CList<CStoneTypeDataV0 *, CStoneTypeDataV0 *> stones;
		BYTE *image;

		CProductItemNodeV0()
		{
			image = NULL;
		}

		~CProductItemNodeV0()
		{
			if(image)
				delete image;

			while(!stones.IsEmpty())
			{
				CStoneTypeDataV0 *data = stones.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	class CProductSetDataV0
	{
	public:
		CProductSetDataV0()
		{
			strcpy(m_productID,"");
			strcpy(m_description,"");
			m_piece_count = 0;
		}

		char m_productID[SHORT_STRING];
		char m_description[MEDIUM_STRING];
		int m_piece_count;
	};

	class CProductSetNodeV0 : public CProductSetDataV0
	{
	public:
		CList<CProductItemNodeV0 *, CProductItemNodeV0 *> items;

		~CProductSetNodeV0()
		{
			while(!items.IsEmpty())
			{
				CProductItemNodeV0 *data = items.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	class CFileHeaderDataV0
	{
	public:
		CFileHeaderDataV0()
		{
			m_type = tNone;

			strcpy(m_country,"");
			strcpy(m_consignee,"");
			strcpy(m_exporter,"Santosh Jewellers,\nE-21 Nanag House, NDSE-1,\nNew Delhi, India");
			strcpy(m_invoice_number,"");

			strcpy(m_currency_name,"");
			m_currency_conversion = 1.0;
			m_dollar_rate = 1.0;

			m_gold_rate = -1;

			strcpy(m_exports_labour_cost, "");
			strcpy(m_exports_diamond_rate, "");
			strcpy(m_exports_pearl_rate, "");
			strcpy(m_exports_stone_rate, "");
			strcpy(m_price_quote, "");
		}

		DatabaseType m_type;
		double m_gold_rate; //in Rs. per Gm. for 18K of gold

		//For EXPORT databases
		char m_country[MEDIUM_STRING];
		char m_consignee[MEDIUM_STRING];
		char m_exporter[MEDIUM_STRING];
		char m_invoice_number[MEDIUM_STRING];
		char m_currency_name[MEDIUM_STRING];
		double m_currency_conversion; //amount of Rs. per unit of currency
		double m_dollar_rate; //amount of Rs. per unit 
		char m_exports_labour_cost[MEDIUM_STRING];
		char m_exports_diamond_rate[MEDIUM_STRING];
		char m_exports_pearl_rate[MEDIUM_STRING];
		char m_exports_stone_rate[MEDIUM_STRING];
		char m_price_quote[MEDIUM_STRING];
	};

	class CDbV0
	{
	public:
		CFileHeaderDataV0 *header;
		CList<CProductSetNodeV0 *, CProductSetNodeV0 *> sets;

		CDbV0()
		{
			header = NULL;
		}

		~CDbV0()
		{
			if(header)
				delete header;

			while(!sets.IsEmpty())
			{
				CProductSetNodeV0 *data = sets.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};




	//VERSION 1
	class CStoneTypeDataV1
	{
	public:
		CStoneTypeDataV1()
		{
			m_rate = 0;
			m_weight = 0.0;
			strcpy(m_type,"");
		}

		char m_type[SHORT_STRING];
		double m_weight; // in carets
		int m_rate;
	};

	class CProductItemDataV1
	{
	public:
		CProductItemDataV1()
		{
			m_serial_number = 0;

			strcpy(m_old_serials, "");
			strcpy(m_productID,"");
			strcpy(m_description,"");
			strcpy(m_photo_filename,"");
			m_piece_count = 0;

			m_gold_rate = -1;
			m_use_net_weight_for_gold = FALSE;

			m_gross_weight = 0.0;
			m_diamond_type_count = 0;
			m_stone_type_count = 0;
			m_pearl_weight = 0.0;
			m_pearl_rate = 0;
			m_other_weight = 0.0;
			m_labour_cost = 0;

			strcpy(m_diamond_marked_value,"=DV");
			strcpy(m_stone_marked_value,"=PRV");
			strcpy(m_pearl_marked_value,"=PV");
			strcpy(m_labour_marked_cost,"=LC");

			m_sold = FALSE;
			strcpy(m_sold_country, "");

			m_image_size = 0;
		}

		int m_serial_number;
		char m_old_serials[LONG_STRING];
		char m_productID[SHORT_STRING];
		char m_description[MEDIUM_STRING];
		char m_photo_filename[MEDIUM_STRING];
		int m_piece_count;

		int m_gold_rate;
		BOOL m_use_net_weight_for_gold;

		double m_gross_weight; // in grams
		int m_diamond_type_count;
		int m_stone_type_count;
		double m_pearl_weight; // in carets
		int m_pearl_rate;
		double m_other_weight; // in grams
		int m_labour_cost;

		char m_diamond_marked_value[MEDIUM_STRING];
		char m_stone_marked_value[MEDIUM_STRING];
		char m_pearl_marked_value[MEDIUM_STRING];
		char m_labour_marked_cost[MEDIUM_STRING];

		BOOL m_sold;
		char m_sold_country[MEDIUM_STRING];

		DWORD m_image_size;
	};

	class CProductItemNodeV1 : public CProductItemDataV1
	{
	public:
		CList<CStoneTypeDataV1 *, CStoneTypeDataV1 *> stones;
		BYTE *image;

		CProductItemNodeV1()
		{
			image = NULL;
		}

		~CProductItemNodeV1()
		{
			if(image)
				delete image;

			while(!stones.IsEmpty())
			{
				CStoneTypeDataV1 *data = stones.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	class CProductSetDataV1
	{
	public:
		CProductSetDataV1()
		{
			strcpy(m_productID,"");
			strcpy(m_description,"");
			m_piece_count = 0;
		}

		char m_productID[SHORT_STRING];
		char m_description[MEDIUM_STRING];
		int m_piece_count;
	};

	class CProductSetNodeV1 : public CProductSetDataV1
	{
	public:
		CList<CProductItemNodeV1 *, CProductItemNodeV1 *> items;

		~CProductSetNodeV1()
		{
			while(!items.IsEmpty())
			{
				CProductItemNodeV1 *data = items.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	class CFileHeaderDataV1
	{
	public:
		CFileHeaderDataV1()
		{
			strcpy(m_version,"V001");
			m_type = tNone;

			strcpy(m_country,"");
			strcpy(m_consignee,"");
			strcpy(m_exporter,"Santosh Jewellers,\nE-21 Nanag House, NDSE-1,\nNew Delhi, India");
			strcpy(m_invoice_number,"");

			strcpy(m_currency_name,"");
			m_currency_conversion = 1.0;
			m_dollar_rate = 1.0;

			m_gold_rate = -1;

			strcpy(m_exports_labour_cost, "");
			strcpy(m_exports_diamond_rate, "");
			strcpy(m_exports_pearl_rate, "");
			strcpy(m_exports_stone_rate, "");
			strcpy(m_price_quote, "");
		}

		char m_version[5];
		DatabaseType m_type;
		double m_gold_rate; //in Rs. per Gm. for 18K of gold

		//For EXPORT databases
		char m_country[MEDIUM_STRING];
		char m_consignee[MEDIUM_STRING];
		char m_exporter[MEDIUM_STRING];
		char m_invoice_number[MEDIUM_STRING];
		char m_currency_name[MEDIUM_STRING];
		double m_currency_conversion; //amount of Rs. per unit of currency
		double m_dollar_rate; //amount of Rs. per unit 
		char m_exports_labour_cost[MEDIUM_STRING];
		char m_exports_diamond_rate[MEDIUM_STRING];
		char m_exports_pearl_rate[MEDIUM_STRING];
		char m_exports_stone_rate[MEDIUM_STRING];
		char m_price_quote[MEDIUM_STRING];
	};

	class CDbV1
	{
	public:
		CFileHeaderDataV1 *header;
		CList<CProductSetNodeV1 *, CProductSetNodeV1 *> sets;

		CDbV1()
		{
			header = NULL;
		}

		~CDbV1()
		{
			if(header)
				delete header;

			while(!sets.IsEmpty())
			{
				CProductSetNodeV1 *data = sets.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};




	//VERSION 2
	class CProductSubItemDataV2
	{
	public:
		char m_type[SHORT_STRING];
		double m_weight; // in carets
		int m_rate;
		int m_count;

	public:
		CProductSubItemDataV2()
		{
			strcpy(m_type,"");
			m_weight = 0.0;
			m_rate = 0;
			m_count = 0;
		}
	};

	class CProductOldSerialDataV2
	{
	public:
		char m_tagname[MEDIUM_STRING];
		int m_serial;

	public:
		CProductOldSerialDataV2()
		{
			strcpy(m_tagname,"");
			m_serial = 0;
		}
	};

	class CProductItemDataV2
	{
	public:
		char m_product_ID[SHORT_STRING];
		char m_description[MEDIUM_STRING];
		int m_piece_count;
		char m_photo_filename[MEDIUM_STRING];
		int m_date_of_feeding; // stored as ddmmyy
		char m_worker[MEDIUM_STRING];
		char m_approval_number[MEDIUM_STRING];

		int m_serial_number;
		int m_old_serial_count;

		int m_gold_rate; //in Rs. per Gm. for 18K of gold
		BOOL m_use_net_weight_for_gold;

		double m_gross_weight_unpolished; // in grams
		double m_gross_weight_polished_before_setting; // in grams
		double m_gross_weight_polished_after_setting; // in grams
		double m_gross_weight_final; // in grams

		double m_other_weight; // in grams
		
		int m_diamond_count;
		int m_stone_count;
		int m_pearl_count;
		int m_labour_cost;

		char m_diamond_marked_value[MEDIUM_STRING];
		char m_stone_marked_value[MEDIUM_STRING];
		char m_pearl_marked_value[MEDIUM_STRING];
		char m_labour_marked_cost[MEDIUM_STRING];

		BOOL m_sold;
		char m_sold_country[MEDIUM_STRING];

		//for kundan pieces
		double m_costing; //in Rs. per Gm.
		char m_marked_costing[MEDIUM_STRING];

	public:
		CProductItemDataV2()
		{
			strcpy(m_product_ID,"");
			strcpy(m_description,"");
			strcpy(m_photo_filename,"");
			m_piece_count = 0;
			strcpy(m_worker, "");
			strcpy(m_approval_number, "");

			CTime system_time = CTime::GetCurrentTime();
			m_date_of_feeding = (system_time.GetDay() * 10000) + (system_time.GetMonth() * 100) + (system_time.GetYear() % 100);

			m_serial_number = 0;
			m_old_serial_count = 0;

			m_gold_rate = -1;
			m_use_net_weight_for_gold = FALSE;

			m_gross_weight_unpolished = 0.0;
			m_gross_weight_polished_before_setting = 0.0;
			m_gross_weight_polished_after_setting = 0.0;
			m_gross_weight_final = 0.0;

			m_other_weight = 0.0;

			m_diamond_count = 0;
			m_stone_count = 0;
			m_pearl_count = 0;
			m_labour_cost = 0;

			strcpy(m_diamond_marked_value,"=DV");
			strcpy(m_stone_marked_value,"=PRV");
			strcpy(m_pearl_marked_value,"=PV");
			strcpy(m_labour_marked_cost,"=LC");

			m_sold = FALSE;
			strcpy(m_sold_country, "");

			m_costing = 0.0;
			strcpy(m_marked_costing,"=COS");
		}
	};

	class CProductItemNodeV2 : public CProductItemDataV2
	{
	public:
		CList<CProductSubItemDataV2 *, CProductSubItemDataV2 *> stones;
		CList<CProductOldSerialDataV2 *, CProductOldSerialDataV2 *> serials;

		~CProductItemNodeV2()
		{
			while(!stones.IsEmpty())
			{
				CProductSubItemDataV2 *data = stones.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}

			while(!serials.IsEmpty())
			{
				CProductOldSerialDataV2 *data = serials.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	class CProductSetDataV2
	{
	public:
		char m_product_ID[SHORT_STRING];
		char m_description[MEDIUM_STRING];
		int m_item_count;

	public:
		CProductSetDataV2()
		{
			strcpy(m_product_ID,"");
			strcpy(m_description,"");
			m_item_count = 0;
		}
	};

	class CProductSetNodeV2 : public CProductSetDataV2
	{
	public:
		CList<CProductItemNodeV2 *, CProductItemNodeV2 *> items;

		~CProductSetNodeV2()
		{
			while(!items.IsEmpty())
			{
				CProductItemNodeV2 *data = items.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	class CProductVoucherDataV2
	{
	public:
		CProductVoucherDataV2()
		{
			strcpy(m_voucher_name,"");
			m_type = tNone;
			m_set_count = 0;
			m_gold_rate = -1;

			strcpy(m_country,"");
			strcpy(m_consignee,"");
			strcpy(m_exporter,"Santosh Jewellers,\nE-21 Nanag House, NDSE-1,\nNew Delhi, India");
			strcpy(m_invoice_number,"");

			strcpy(m_currency_name,"");
			m_currency_conversion = 1.0;
			m_dollar_rate = 1.0;

			strcpy(m_labour_cost, "");
			strcpy(m_diamond_rate, "");
			strcpy(m_pearl_rate, "");
			strcpy(m_stone_rate, "");
			strcpy(m_price_quote, "");
		}

	public:
		char m_voucher_name[MEDIUM_STRING];
		DatabaseType m_type;
		DWORD m_set_count;
		double m_gold_rate; //in Rs. per Gm. for 18K of gold

		char m_price_quote[MEDIUM_STRING];

		//used for exports only
		char m_country[MEDIUM_STRING];
		char m_consignee[MEDIUM_STRING];
		char m_exporter[MEDIUM_STRING];
		char m_invoice_number[MEDIUM_STRING];

		char m_currency_name[MEDIUM_STRING];
		double m_currency_conversion; //amount of Rs. per unit of currency
		double m_dollar_rate; //amount of Rs. per unit

		char m_labour_cost[MEDIUM_STRING];
		char m_diamond_rate[MEDIUM_STRING];
		char m_pearl_rate[MEDIUM_STRING];
		char m_stone_rate[MEDIUM_STRING];
	};
	
	class CProductVoucherNodeV2 : public CProductVoucherDataV2
	{
	public:
		CList<CProductSetNodeV2 *, CProductSetNodeV2 *> sets;

		~CProductVoucherNodeV2()
		{
			while(!sets.IsEmpty())
			{
				CProductSetNodeV2 *data = sets.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	class CProductFileHeaderV2
	{
	public:
		CProductFileHeaderV2()
		{
			strcpy(m_version, "V002");
		}

		char m_version[5];
	};

	class CDbV2
	{
	public:
		CProductFileHeaderV2 *header;
		CList<CProductVoucherNodeV2 *, CProductVoucherNodeV2*> vouchers;

		CDbV2()
		{
			header = NULL;
		}

		~CDbV2()
		{
			if(header)
				delete header;

			while(!vouchers.IsEmpty())
			{
				CProductVoucherNodeV2 *data = vouchers.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	//VERSION 3
	class CProductSubItemDataV3
	{
	public:
		char m_type[SHORT_STRING];
		double m_weight; // in carets
		int m_rate;
		int m_count;

	public:
		CProductSubItemDataV3()
		{
			strcpy(m_type,"");
			m_weight = 0.0;
			m_rate = 0;
			m_count = 0;
		}
	};

	class CProductOldSerialDataV3
	{
	public:
		char m_tagname[MEDIUM_STRING];
		int m_serial;

	public:
		CProductOldSerialDataV3()
		{
			strcpy(m_tagname,"");
			m_serial = 0;
		}
	};

	class CProductItemDataV3
	{
	public:
		char m_product_ID[SHORT_STRING];
		char m_description[MEDIUM_STRING];
		int m_piece_count;
		char m_photo_filename[MEDIUM_STRING];

		int m_serial_number;
		int m_old_serial_count;

		int m_gold_rate; //in Rs. per Gm. for 18K of gold
		BOOL m_use_net_weight_for_gold;

		double m_gross_weight; // in grams
		double m_other_weight; // in grams
		
		int m_diamond_count;
		int m_stone_count;
		int m_pearl_count;
		int m_labour_cost;
		char m_diamond_clarity[SHORT_STRING];

		char m_diamond_marked_value[MEDIUM_STRING];
		char m_stone_marked_value[MEDIUM_STRING];
		char m_pearl_marked_value[MEDIUM_STRING];
		char m_labour_marked_cost[MEDIUM_STRING];

		BOOL m_sold;
		char m_sold_country[MEDIUM_STRING];

		//for kundan pieces
		double m_costing; //in Rs. per Gm.
		char m_marked_costing[MEDIUM_STRING];

	public:
		CProductItemDataV3()
		{
			strcpy(m_product_ID,"");
			strcpy(m_description,"");
			strcpy(m_photo_filename,"");
			m_piece_count = 0;

			m_serial_number = 0;
			m_old_serial_count = 0;

			m_gold_rate = -1;
			m_use_net_weight_for_gold = FALSE;

			m_gross_weight = 0.0;
			m_other_weight = 0.0;

			m_diamond_count = 0;
			m_stone_count = 0;
			m_pearl_count = 0;
			m_labour_cost = 0;
			strcpy(m_diamond_clarity, "");

			strcpy(m_diamond_marked_value,"=DV");
			strcpy(m_stone_marked_value,"=PRV");
			strcpy(m_pearl_marked_value,"=PV");
			strcpy(m_labour_marked_cost,"=LC");

			m_sold = FALSE;
			strcpy(m_sold_country, "");

			m_costing = 0.0;
			strcpy(m_marked_costing,"=COS");
		}
	};

	class CProductItemNodeV3 : public CProductItemDataV3
	{
	public:
		CList<CProductSubItemDataV3 *, CProductSubItemDataV3 *> stones;
		CList<CProductOldSerialDataV3 *, CProductOldSerialDataV3 *> serials;

		~CProductItemNodeV3()
		{
			while(!stones.IsEmpty())
			{
				CProductSubItemDataV3 *data = stones.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}

			while(!serials.IsEmpty())
			{
				CProductOldSerialDataV3 *data = serials.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	class CProductSetDataV3
	{
	public:
		char m_product_ID[SHORT_STRING];
		char m_description[MEDIUM_STRING];
		int m_item_count;

	public:
		CProductSetDataV3()
		{
			strcpy(m_product_ID,"");
			strcpy(m_description,"");
			m_item_count = 0;
		}
	};

	class CProductSetNodeV3 : public CProductSetDataV3
	{
	public:
		CList<CProductItemNodeV3 *, CProductItemNodeV3 *> items;

		~CProductSetNodeV3()
		{
			while(!items.IsEmpty())
			{
				CProductItemNodeV3 *data = items.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	class CProductVoucherDataV3
	{
	public:
		CProductVoucherDataV3()
		{
			strcpy(m_voucher_name,"");
			m_type = tNone;
			m_set_count = 0;
			m_gold_rate = -1;

			strcpy(m_country,"");
			strcpy(m_consignee,"");
			strcpy(m_exporter,"Santosh Jewellers,\nE-21 Nanag House, NDSE-1,\nNew Delhi, India");
			strcpy(m_invoice_number,"");

			strcpy(m_currency_name,"");
			m_currency_conversion = 1.0;
			m_dollar_rate = 1.0;

			strcpy(m_labour_cost, "");
			strcpy(m_diamond_rate, "");
			strcpy(m_pearl_rate, "");
			strcpy(m_stone_rate, "");
			strcpy(m_price_quote, "");
		}

	public:
		char m_voucher_name[MEDIUM_STRING];
		DatabaseType m_type;
		DWORD m_set_count;
		double m_gold_rate; //in Rs. per Gm. for 18K of gold

		char m_price_quote[MEDIUM_STRING];

		//used for exports only
		char m_country[MEDIUM_STRING];
		char m_consignee[MEDIUM_STRING];
		char m_exporter[MEDIUM_STRING];
		char m_invoice_number[MEDIUM_STRING];

		char m_currency_name[MEDIUM_STRING];
		double m_currency_conversion; //amount of Rs. per unit of currency
		double m_dollar_rate; //amount of Rs. per unit

		char m_labour_cost[MEDIUM_STRING];
		char m_diamond_rate[MEDIUM_STRING];
		char m_pearl_rate[MEDIUM_STRING];
		char m_stone_rate[MEDIUM_STRING];
	};
	
	class CProductVoucherNodeV3 : public CProductVoucherDataV3
	{
	public:
		CList<CProductSetNodeV3 *, CProductSetNodeV3 *> sets;

		~CProductVoucherNodeV3()
		{
			while(!sets.IsEmpty())
			{
				CProductSetNodeV3 *data = sets.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

	class CProductFileHeaderV3
	{
	public:
		CProductFileHeaderV3()
		{
			strcpy(m_version, "V003");
		}

		char m_version[5];
	};

	class CDbV3
	{
	public:
		CProductFileHeaderV3 *header;
		CList<CProductVoucherNodeV3 *, CProductVoucherNodeV3*> vouchers;

		CDbV3()
		{
			header = NULL;
		}

		~CDbV3()
		{
			if(header)
				delete header;

			while(!vouchers.IsEmpty())
			{
				CProductVoucherNodeV3 *data = vouchers.RemoveHead();
				if(data)
					delete data;
				else
					SHOW_INTERNAL_ERROR;
			}
		}
	};

private:
	static BOOL CheckEquality();
	int GetDatabaseVersion();

	BOOL ConvertV2toV3();
	BOOL ConvertV1toV2();
	BOOL ConvertV0toV1();

	FileError ReadV0(CDbV0 *db, CFile *infile);
	FileError ReadV1(CDbV1 *db, CFile *infile);
	FileError ReadV2(CDbV2 *db, CFile *infile);

	FileError WriteV1(CDbV1 *db, CFile *outfile);
	FileError WriteV2(CDbV2 *db, CFile *outfile);
	FileError WriteV3(CDbV3 *db, CFile *outfile);

	CString m_filename; //full path of file
};

#endif // !defined(AFX_DATABASECONVERTER_H__76DFD6E7_672C_452F_804B_A6F34C7A9BA2__INCLUDED_)

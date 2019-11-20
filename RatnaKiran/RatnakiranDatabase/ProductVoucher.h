#if !defined(AFX_PRODUCTVOUCHER_H__E4680C1C_F5AD_453B_A664_7146006779F7__INCLUDED_)
#define AFX_PRODUCTVOUCHER_H__E4680C1C_F5AD_453B_A664_7146006779F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProductSet.h"
enum DatabaseType {tNone, tStock, tExport, tKundan};

#define STOCK_ROOT "Stock Vouchers"
#define EXPORT_ROOT "Export Vouchers"
#define KUNDAN_ROOT "Kundan Vouchers"

#define STOCK_ROOT_NAME "__stock_main"
#define EXPORT_ROOT_NAME "__export_main"
#define KUNDAN_ROOT_NAME "__kundan_main"

class CProductVoucherData
{
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

public:
	CProductVoucherData()
	{
		Empty();
	}

	CProductVoucherData(const CProductVoucherData &arg)
	{
		Empty();
		Copy(arg);
	}

	~CProductVoucherData()
	{
		Empty();
	}

	void operator=(const CProductVoucherData &arg)
	{
		Copy(arg);
	}

	BOOL operator==(CProductVoucherData &arg)
	{
		return Equal(arg);
	}

protected:
	void Copy(const CProductVoucherData &arg)
	{
		strcpy(m_voucher_name, arg.m_voucher_name);
		m_type = arg.m_type;
		m_set_count = arg.m_set_count;
		m_gold_rate = arg.m_gold_rate;

		strcpy(m_price_quote, arg.m_price_quote);

		strcpy(m_country, arg.m_country);
		strcpy(m_consignee, arg.m_consignee);
		strcpy(m_exporter, arg.m_exporter);
		strcpy(m_invoice_number, arg.m_invoice_number);

		strcpy(m_currency_name, arg.m_currency_name);
		m_currency_conversion = arg.m_currency_conversion;
		m_dollar_rate = arg.m_dollar_rate;

		strcpy(m_labour_cost, arg.m_labour_cost);
		strcpy(m_diamond_rate, arg.m_diamond_rate);
		strcpy(m_pearl_rate, arg.m_pearl_rate);
		strcpy(m_stone_rate, arg.m_stone_rate);
	}

	BOOL Equal(CProductVoucherData &arg)
	{
		return ((m_type == arg.m_type) &&
			(m_gold_rate == arg.m_gold_rate) &&
			(strcmp(m_voucher_name, arg.m_voucher_name) == 0) &&
			(m_set_count == arg.m_set_count) &&
			(strcmp(m_country, arg.m_country) == 0) &&
			(strcmp(m_consignee, arg.m_consignee) == 0) &&
			(strcmp(m_exporter, arg.m_exporter) == 0) &&
			(strcmp(m_invoice_number, arg.m_invoice_number) == 0) &&
			(strcmp(m_currency_name, arg.m_currency_name) == 0) &&
			(strcmp(m_labour_cost, arg.m_labour_cost) == 0) &&
			(strcmp(m_diamond_rate, arg.m_diamond_rate) == 0) &&
			(strcmp(m_pearl_rate, arg.m_pearl_rate) == 0) &&
			(strcmp(m_stone_rate, arg.m_stone_rate) == 0) &&
			(strcmp(m_price_quote, arg.m_price_quote) == 0) &&
			(m_currency_conversion == arg.m_currency_conversion) &&
			(m_dollar_rate == arg.m_dollar_rate));
	}

	void Empty()
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
};

class CProductVoucher : public CProductVoucherData
{
public:
	CList<CProductSet*, CProductSet*> m_sets;

public:
	CProductVoucher()
	{
	}

	CProductVoucher(const CProductVoucher &arg)
	{
		Empty();
		Copy((CProductVoucherData)arg);

		m_set_count = 0;
		POSITION pos = arg.m_sets.GetHeadPosition();
		while(pos)
		{
			CProductSet *curnode = arg.m_sets.GetAt(pos);
			if(curnode)
			{
				if(!AddSet(curnode, FALSE, FALSE))
				{
					CString errstr = "Could not add set '" + CString(curnode->m_product_ID) + "' to voucher '" + CString(m_voucher_name) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_sets.GetNext(pos);
		}
	}

	~CProductVoucher()
	{
		DeleteSetList();
	}

	void operator=(const CProductVoucher &arg)
	{
		DeleteSetList();

		Copy((CProductVoucherData)arg);

		m_set_count = 0;
		POSITION pos = arg.m_sets.GetHeadPosition();
		while(pos)
		{
			CProductSet *curnode = arg.m_sets.GetAt(pos);
			if(curnode)
			{
				if(!AddSet(curnode, FALSE, FALSE))
				{
					CString errstr = "Could not add set '" + CString(curnode->m_product_ID) + "' to voucher '" + CString(m_voucher_name) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_sets.GetNext(pos);
		}
	}

	BOOL operator==(CProductVoucher &arg)
	{
		BOOL retval = Equal((CProductVoucherData)arg);

		if(retval)
		{
			POSITION pos1 = m_sets.GetHeadPosition();
			POSITION pos2 = arg.m_sets.GetHeadPosition();

			while(retval && pos1 && pos2)
			{
				CProductSet *set1 = m_sets.GetAt(pos1);
				CProductSet *set2 = arg.m_sets.GetAt(pos2);

				if(set1 && set2)
				{
					if((*(set1)) == (*(set2)))
					{
						m_sets.GetNext(pos1);
						arg.m_sets.GetNext(pos2);
					}
					else
						retval = FALSE;
				}
				else
				{
					SHOW_INTERNAL_ERROR;
					retval = FALSE;
				}
			}

			if(pos1 || pos2)
				retval = FALSE;
		}

		return retval;
	}

	BOOL IsRootVoucher()
	{
		BOOL retval = FALSE;
		switch(m_type)
		{
		case tStock:
			retval = (strcmp(m_voucher_name, STOCK_ROOT_NAME) == 0);
			break;
		case tExport:
			retval = (strcmp(m_voucher_name, EXPORT_ROOT_NAME) == 0);
			break;
		case tKundan:
			retval = (strcmp(m_voucher_name, KUNDAN_ROOT_NAME) == 0);
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", m_type);
				errstr = "Invalid voucher type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}

		return retval;
	}

	BOOL AddSet(CProductSet *set, BOOL check_duplicate, BOOL perform_sorting)
	{
		if(!set)
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CProductSet *newlistnode = NULL;
		SALIL_NEW(newlistnode, CProductSet);
		*newlistnode = *set;

		if(check_duplicate)
		{
			if(!ResolveDuplicate(newlistnode))
			{
				CString errstr = "Could not resolve duplicates for the set '" + CString(newlistnode->m_product_ID) + "' in voucher '" + CString(m_voucher_name) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}

		if(!perform_sorting)
		{
			m_sets.AddTail(newlistnode);
		}
		else
		{
			POSITION pos = m_sets.GetHeadPosition();
			while(pos)
			{
				CProductSet *curnode = m_sets.GetAt(pos);
				if(curnode)
				{
					if(IsGreaterString(curnode->m_product_ID, newlistnode->m_product_ID))
					{
						m_sets.InsertBefore(pos, newlistnode);
						break;
					}
				}
				else
					SHOW_INTERNAL_ERROR;

				m_sets.GetNext(pos);
			}

			if(!pos)
			{
				m_sets.AddTail(newlistnode);
			}
		}

		m_set_count++;
		gIDList.AddTail(CProductItem::ExtractActualID(set->m_product_ID));
		return TRUE;
	}

	BOOL ResolveDuplicate(CProductSet *set)
	{
		if(!set)
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CString originalID = set->m_product_ID;
		CString newID = CProductItem::ExtractActualID(set->m_product_ID);
		CString prefix = CProductItem::ExtractPrefix(set->m_product_ID);

		POSITION pos = gIDList.Find(newID);
		while(pos)
		{
			newID = CProductItem::GetProductID();
			pos = gIDList.Find(newID);
		}

		newID = prefix + newID;
		if(newID != originalID)
		{
			if(!set->UpdateProductID(newID))
			{
				CString errstr = "Could not update productID of set '" + CString(set->m_product_ID) + "' to '" + newID + "' in voucher '" + CString(m_voucher_name) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
			else
			{
				CString errstr = "ProductID '" + originalID + "' already existed in the database. Replaced it with '" + newID + "' in voucher '" + CString(m_voucher_name) + "'.";
				SHOW_WARNING(errstr);
			}
		}

		return TRUE;
	}

	BOOL DeleteSet(CString productID)
	{
		if(productID.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		POSITION pos = m_sets.GetHeadPosition();
		while(pos)
		{
			CProductSet *curnode = m_sets.GetAt(pos);
			if(curnode)
			{
				CString curID = curnode->m_product_ID;
				if(productID.CompareNoCase(curID) == 0)
				{
					m_sets.RemoveAt(pos);
					m_set_count--;

					POSITION IDPos = gIDList.Find(CProductItem::ExtractActualID(curnode->m_product_ID));
					if(IDPos)
						gIDList.RemoveAt(IDPos);

					delete curnode;
					return TRUE;
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_sets.GetNext(pos);
		}

		return FALSE;
	}

	BOOL DeleteSetAt(int position)
	{
		if((position < 0) ||
		   (position >= GetSetCount()))
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		POSITION pos = m_sets.FindIndex(position);
		if(pos)
		{
			CProductSet *curnode = m_sets.GetAt(pos);
			if(curnode)
			{
				m_sets.RemoveAt(pos);
				m_set_count--;

				POSITION IDPos = gIDList.Find(CProductItem::ExtractActualID(curnode->m_product_ID));
				if(IDPos)
					gIDList.RemoveAt(IDPos);

				delete curnode;
				return TRUE;
			}
			else
				SHOW_INTERNAL_ERROR;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", position);
			errstr = "Could not delete set at position '" + errstr + "' in voucher '" + CString(m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return FALSE;
	}

	BOOL DeleteEmptySets(BOOL *changed)
	{
		BOOL haschanged = FALSE;
		POSITION pos = m_sets.GetHeadPosition();
		while(pos)
		{
			CProductSet *curnode = m_sets.GetAt(pos);
			if(curnode)
			{
				if(curnode->GetItemCount() <= 0)
				{
					POSITION deletePos = pos;
					m_sets.GetNext(pos);
					m_sets.RemoveAt(deletePos);
					m_set_count--;

					POSITION IDPos = gIDList.Find(CProductItem::ExtractActualID(curnode->m_product_ID));
					if(IDPos)
						gIDList.RemoveAt(IDPos);

					delete curnode;
					haschanged = TRUE;
					continue;
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_sets.GetNext(pos);
		}

		if(changed)
			*changed = haschanged;

		return TRUE;
	}

	BOOL DeleteSoldSets(BOOL *changed)
	{
		BOOL haschanged = FALSE;
		POSITION pos = m_sets.GetHeadPosition();
		while(pos)
		{
			CProductSet *curnode = m_sets.GetAt(pos);
			if(curnode)
			{
				BOOL set_changed = FALSE;
				if(!curnode->DeleteSoldItems(&set_changed))
				{
					CString errstr = "Could not delete sold items for set '" + CString(curnode->m_product_ID) + "' in voucher '" + CString(m_voucher_name) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
				else if(set_changed)
				{
					haschanged = TRUE;

					//delete empty set
					if(curnode->GetItemCount() <= 0)
					{
						POSITION deletePos = pos;
						m_sets.GetNext(pos);
						m_sets.RemoveAt(deletePos);
						m_set_count--;

						POSITION IDPos = gIDList.Find(CProductItem::ExtractActualID(curnode->m_product_ID));
						if(IDPos)
							gIDList.RemoveAt(IDPos);

						delete curnode;
						continue;
					}
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_sets.GetNext(pos);
		}

		if(changed)
			*changed = haschanged;

		return TRUE;
	}

	BOOL DeleteSetList()
	{
		while(!m_sets.IsEmpty())
		{
			CProductSet *temp = m_sets.RemoveHead();
			if(temp)
			{
				POSITION IDPos = gIDList.Find(CProductItem::ExtractActualID(temp->m_product_ID));
				if(IDPos)
					gIDList.RemoveAt(IDPos);

				delete temp;
			}
			else
				SHOW_INTERNAL_ERROR;
		}

		m_set_count = 0;
		return TRUE;
	}

	CProductSet *GetSet(CString productID, int *position = NULL)
	{
		int return_pos = -1;
		CProductSet *return_node = NULL;

		if(productID.IsEmpty())
		{
			if(position)
				*position = return_pos;

			SHOW_INTERNAL_ERROR;
			return return_node;
		}

		POSITION pos = m_sets.GetHeadPosition();
		while(pos)
		{
			CProductSet *curnode = m_sets.GetAt(pos);
			if(curnode)
			{
				return_pos++;

				CString curID = curnode->m_product_ID;
				if(productID.CompareNoCase(curID) == 0)
				{
					return_node = curnode;
					break;
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_sets.GetNext(pos);
		}

		if(!return_node)
			return_pos = -1;

		if(position)
			*position = return_pos;

		return return_node;
	}

	CProductSet *GetSetAt(int position)
	{
		if((position < 0) ||
		   (position >= GetSetCount()))
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		POSITION pos = m_sets.FindIndex(position);
		if(pos)
		{
			CProductSet *curnode = m_sets.GetAt(pos);
			if(curnode)
				return curnode;
			else
				SHOW_INTERNAL_ERROR;
		}

		return NULL;
	}

	BOOL GetSetList(CString productID, CList<CString, CString> *list)//The productID here is partial
	{
		if(!list || productID.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		POSITION pos = m_sets.GetHeadPosition();
		while(pos)
		{
			CProductSet *curnode = m_sets.GetAt(pos);
			if(curnode)
			{
				CString curID = curnode->m_product_ID;
				if(productID.CompareNoCase(CProductItem::ExtractActualID(curID)) == 0)
					list->AddTail(curID);
			}
			else
				SHOW_INTERNAL_ERROR;

			m_sets.GetNext(pos);
		}

		return TRUE;
	}

	int GetSetCount()
	{
		return m_set_count;
	}

	BOOL UpdateSet(CString productID, const CProductSet *updatedSet)
	{
		if(!updatedSet || productID.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CProductSet *curnode = GetSet(productID);
		if(!curnode)
		{
			CString errstr = "Could not get set '" + productID + "' in voucher '" + CString(m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}

		*curnode = *updatedSet;
		return TRUE;
	}

	BOOL UpdateSetAt(int position, const CProductSet *updatedSet)
	{
		if(!updatedSet || (position < 0) ||
		   (position >= GetSetCount()))
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CProductSet *curnode = GetSetAt(position);
		if(!curnode)
		{
			CString errstr;
			errstr.Format("%d", position);
			errstr = "Could not get set at position '" + errstr + "' in voucher '" + CString(m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}

		*curnode = *updatedSet;
		return TRUE;
	}

	CProductItem *GetItem(CString productID)
	{
		if(productID.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		POSITION pos = m_sets.GetHeadPosition();
		while(pos)
		{
			CProductSet *curnode = m_sets.GetAt(pos);
			if(curnode)
			{
				CProductItem *item = curnode->GetItem(productID);
				if(item)
					return item;
			}
			else
				SHOW_INTERNAL_ERROR;

			m_sets.GetNext(pos);
		}
		
		return NULL;
	}

	CProductItem *GetItem(int serial_number)
	{
		if(serial_number <= 0)
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		POSITION pos = m_sets.GetHeadPosition();
		while(pos)
		{
			CProductSet *curnode = m_sets.GetAt(pos);
			if(curnode)
			{
				CProductItem *item = curnode->GetItem(serial_number);
				if(item)
					return item;
			}
			else
				SHOW_INTERNAL_ERROR;

			m_sets.GetNext(pos);
		}
		
		return NULL;
	}

	BOOL MarkAsSold(CString productID, BOOL sold, CString sold_in_country, BOOL asSet)
	{
		if(productID.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		if(asSet)
		{
			CProductSet *curnode = GetSet(productID);
			if(curnode)
			{
				if(curnode->MarkSetAsSold(sold, sold_in_country))
					return TRUE;
				else
				{
					CString errstr = "Could not update sold status of set '" + CString(curnode->m_product_ID) + "' in voucher '" + CString(m_voucher_name) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
			{
				CString errstr = "Could not get set '" + productID + "' in voucher '" + CString(m_voucher_name) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}

			return FALSE;
		}
		else
		{
			CProductItem *item = GetItem(productID);
			if(item)
			{
				item->m_sold = sold;

				if(sold)
					strcpy(item->m_sold_country, LPCTSTR(sold_in_country));
				else
					strcpy(item->m_sold_country, "");

				return TRUE;
			}
			else
			{
				CString errstr = "Could not get item '" + productID + "' in voucher '" + CString(m_voucher_name) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}

			return FALSE;
		}
	}

	BOOL MarkAsSold(int serial_number, BOOL sold, CString sold_in_country)
	{
		if(serial_number <= 0)
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CProductItem *item = GetItem(serial_number);
		if(item)
		{
			item->m_sold = sold;

			if(sold)
				strcpy(item->m_sold_country, LPCTSTR(sold_in_country));
			else
				strcpy(item->m_sold_country, "");

			return TRUE;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", serial_number);
			errstr = "Could not get item with serial number '" + errstr + "' in voucher '" + CString(m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return FALSE;
	}

	void SortDatabase()
	{
		CList<CProductSet*, CProductSet*> temp_set_list;
		while(!m_sets.IsEmpty())
		{
			CProductSet *curnode = m_sets.RemoveHead();
			if(curnode)
				temp_set_list.AddTail(curnode);
			else
				SHOW_INTERNAL_ERROR;
		}

		while(!temp_set_list.IsEmpty())
		{
			CProductSet *curnode = temp_set_list.RemoveHead();
			if(curnode)
			{
				POSITION pos = m_sets.GetHeadPosition();
				while(pos)
				{
					CProductSet *current = m_sets.GetAt(pos);
					if(IsGreaterString(current->m_product_ID, curnode->m_product_ID))
					{
						m_sets.InsertBefore(pos, curnode);
						break;
					}

					m_sets.GetNext(pos);
				}

				if(!pos)
					m_sets.AddTail(curnode);
			}
			else
				SHOW_INTERNAL_ERROR;
		}

		return;
	}

	int GetDesiredSerialNumber()
	{
		int serial = 0;
		POSITION pos = m_sets.GetHeadPosition();
		while(pos)
		{
			CProductSet *curnode = m_sets.GetAt(pos);
			if(curnode)
			{
				POSITION item_pos = curnode->m_items.GetHeadPosition();
				while(item_pos)
				{
					CProductItem *item = curnode->m_items.GetAt(item_pos);
					if(item)
						serial = max(serial, item->m_serial_number);
					else
						SHOW_INTERNAL_ERROR;

					curnode->m_items.GetNext(item_pos);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_sets.GetNext(pos);
		}

		return (serial + 1);
	}

	BOOL AssignSerialNumbers(int startSerial)
	{
		if(startSerial <= 0)
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		int serial = startSerial;
		CList<CProductSet *, CProductSet *> nodelist_with_serials;
		CList<CProductSet *, CProductSet *> nodelist_without_serials;

		POSITION pos1 = m_sets.GetHeadPosition();
		while(pos1)
		{
			CProductSet *cur_set = m_sets.GetAt(pos1);
			if(cur_set)
			{
				CProductItem *cur_item = cur_set->GetItemAt(0);
				if(cur_item)
				{
					if(cur_item->m_old_serials.IsEmpty())
						nodelist_without_serials.AddTail(cur_set);
					else
					{
						CProductOldSerial *cur_serial = cur_item->m_old_serials.GetHead();
						if(cur_serial)
						{
							int oldserial = cur_serial->m_serial;
							POSITION pos2 = nodelist_with_serials.GetHeadPosition();
							while(pos2)
							{
								CProductSet *temp_set = nodelist_with_serials.GetAt(pos2);
								if(temp_set)
								{
									CProductItem *temp_item = temp_set->GetItemAt(0);
									if(temp_item)
									{
										CProductOldSerial *temp_serial = temp_item->m_old_serials.GetHead();
										int tempoldserial = temp_serial->m_serial;
										if(tempoldserial > oldserial)
											break;
									}
									else
									{
										CString errstr = "Could not get first item for set '" + CString(temp_set->m_product_ID) + "' in voucher '" + CString(m_voucher_name) + "'.";
										SHOW_INTERNAL_ERROR_REASON(errstr);
									}
								}
								else
									SHOW_INTERNAL_ERROR;

								nodelist_with_serials.GetNext(pos2);
							}
					
							if(!pos2)
								nodelist_with_serials.AddTail(cur_set);
							else
								nodelist_with_serials.InsertBefore(pos2, cur_set);
						}
						else
							SHOW_INTERNAL_ERROR;
					}
				}
				else
				{
					CString errstr = "Could not get first item for set '" + CString(cur_set->m_product_ID) + "' in voucher '" + CString(m_voucher_name) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_sets.GetNext(pos1);
		}

		pos1 = nodelist_with_serials.GetHeadPosition();
		while(pos1)
		{
			CProductSet *current_set = nodelist_with_serials.GetAt(pos1);
			if(current_set)
				serial = current_set->UpdateSerialNumbers(serial);
			else
				SHOW_INTERNAL_ERROR;

			nodelist_with_serials.GetNext(pos1);
		}

		pos1 = nodelist_without_serials.GetHeadPosition();
		while(pos1)
		{
			CProductSet *current_set = nodelist_without_serials.GetAt(pos1);
			if(current_set)
				serial = current_set->UpdateSerialNumbers(serial);
			else
				SHOW_INTERNAL_ERROR;

			nodelist_without_serials.GetNext(pos1);
		}

		return TRUE;
	}

	BOOL DeleteOldSerialNumbers(CString productID, CString tagname)
	{
		if(productID.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CProductSet *curnode = GetSet(productID);
		if(curnode)
		{
			if(!curnode->DeleteOldSerialNumbers(tagname))
			{
				CString errstr = "Could not delete old serial numbers with tagname '" + tagname + "' for set '" + productID + "' in voucher '" + CString(m_voucher_name) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}

			return TRUE;
		}
		else
		{
			CString errstr = "Could not get set '" + productID + "' in voucher '" + CString(m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return FALSE;
	}

	BOOL ChangeProductID(CString productID)
	{
		if(productID.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CProductSet *curnode = GetSet(productID);
		if(!curnode)
		{
			CString errstr = "Could not get set '" + productID + "' in voucher '" + CString(m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}

		CString originalID = curnode->m_product_ID;
		CString newID = CProductItem::GetProductID();
		CString prefix = CProductItem::ExtractPrefix(curnode->m_product_ID);

		POSITION pos = gIDList.Find(newID);
		while(pos)
		{
			newID = CProductItem::GetProductID();
			pos = gIDList.Find(newID);
		}

		newID = prefix + newID;
		if(!curnode->UpdateProductID(newID))
		{
			CString errstr = "Could not update productID of set '" + CString(curnode->m_product_ID) + "' to '" + newID + "' in voucher '" + CString(m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
		else
		{
			CString errstr = "ProductID '" + originalID + "' replaced with '" + newID + "' in voucher '" + CString(m_voucher_name) + "'.";
			SHOW_WARNING(errstr);
			return TRUE;
		}
	}
};

#endif // !defined(AFX_PRODUCTVOUCHER_H__E4680C1C_F5AD_453B_A664_7146006779F7__INCLUDED_)

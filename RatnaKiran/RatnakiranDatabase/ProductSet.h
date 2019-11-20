#if !defined(AFX_PRODUCTSET_H__2378F0B1_647D_4FDA_ADA1_E8237ECB5D80__INCLUDED_)
#define AFX_PRODUCTSET_H__2378F0B1_647D_4FDA_ADA1_E8237ECB5D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProductItem.h"
#include "Settings.h"

class CProductSetData
{
public:
	char m_product_ID[SHORT_STRING];
	char m_description[MEDIUM_STRING];
	int m_item_count;

public:
	CProductSetData()
	{
		Empty();
	}

	CProductSetData(const CProductSetData &arg)
	{
		Empty();
		Copy(arg);
	}

	~CProductSetData()
	{
		Empty();
	}

	void operator=(const CProductSetData &arg)
	{
		Copy(arg);
	}

	BOOL operator==(CProductSetData &arg)
	{
		return Equal(arg);
	}

protected:
	void Copy(const CProductSetData &arg)
	{
		strcpy(m_product_ID,arg.m_product_ID);
		strcpy(m_description,arg.m_description);
		m_item_count = arg.m_item_count;
	}

	BOOL Equal(CProductSetData &arg)
	{
		return((strcmp(m_product_ID, arg.m_product_ID) == 0) &&
				(strcmp(m_description, arg.m_description) == 0) &&
				(m_item_count == arg.m_item_count));
	}

	void Empty()
	{
		strcpy(m_product_ID,"");
		strcpy(m_description,"");
		m_item_count = 0;
	}
};

class CProductSet : public CProductSetData
{
public:
	CList<CProductItem*, CProductItem*> m_items;

public:
	CProductSet()
	{
	}

	CProductSet(const CProductSet &arg)
	{
		Empty();
		Copy(arg);

		m_item_count = 0;
		POSITION pos = arg.m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = arg.m_items.GetAt(pos);
			if(curnode)
			{
				if(!AddItem(curnode, FALSE))
				{
					CString errstr = "Could not add item '" + CString(curnode->m_product_ID) + "' to set '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_items.GetNext(pos);
		}
	}

	~CProductSet()
	{
		DeleteItemList();
	}

	void operator=(const CProductSet &arg)
	{
		DeleteItemList();

		Copy((CProductSetData)arg);

		m_item_count = 0;
		POSITION pos = arg.m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = arg.m_items.GetAt(pos);
			if(curnode)
			{
				if(!AddItem(curnode, FALSE))
				{
					CString errstr = "Could not add item '" + CString(curnode->m_product_ID) + "' to set '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_items.GetNext(pos);
		}
	}

	BOOL operator==(CProductSet &arg)
	{
		BOOL retval = Equal((CProductSetData)arg);

		if(retval)
		{
			POSITION pos1 = m_items.GetHeadPosition();
			POSITION pos2 = arg.m_items.GetHeadPosition();

			while(retval && pos1 && pos2)
			{
				CProductItem *item1 = m_items.GetAt(pos1);
				CProductItem *item2 = arg.m_items.GetAt(pos2);

				if(item1 && item2)
				{
					if((*(item1)) == (*(item2)))
					{
						m_items.GetNext(pos1);
						arg.m_items.GetNext(pos2);
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


	BOOL AddItem(CProductItem *item, BOOL updateID)
	{
		if(!item)
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		if(updateID)
		{
			CString productID = CProductItem::ExtractActualID(m_product_ID);
			CString setprefix = CProductItem::ExtractPrefix(m_product_ID);
			CString prefix = CSettings::GetPrefixFor(item->m_description);
			int pieces = CSettings::GetPieceCountFor(item->m_description);

			if(prefix != setprefix)
				prefix = prefix + setprefix;

			productID = prefix + productID;
			if(!item->UpdateProductID(productID))
			{
				CString errstr = "Could not update productID of item '" + CString(item->m_product_ID) + "' to '" + productID + "' in set '" + CString(m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}

			item->m_piece_count = pieces;
		}

		CProductItem *newnode = NULL;
		SALIL_NEW(newnode, CProductItem);

		*newnode = *item;
		m_items.AddTail(newnode);
		m_item_count++;
		return TRUE;
	}

	BOOL DeleteItem(CString productID)
	{
		if(productID.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				if(productID.CompareNoCase(curnode->m_product_ID) == 0)
				{
					m_items.RemoveAt(pos);
					delete curnode;
					m_item_count--;
					return TRUE;
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		return FALSE;
	}

	BOOL DeleteItemAt(int position)
	{
		if((position < 0) ||
		   (position >= GetItemCount()))
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		POSITION pos = m_items.FindIndex(position);
		if(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				m_items.RemoveAt(pos);
				delete curnode;
				m_item_count--;
				return TRUE;
			}
			else
				SHOW_INTERNAL_ERROR;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", position);
			errstr = "Could not delete item at position '" + errstr + "' in set '" + CString(m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return FALSE;
	}

	BOOL DeleteItemList()
	{
		while(!m_items.IsEmpty())
		{
			CProductItem *temp = m_items.RemoveHead();
			if(temp)
				delete temp;
			else
				SHOW_INTERNAL_ERROR;
		}

		m_item_count = 0;
		return TRUE;
	}

	BOOL DeleteSoldItems(BOOL *changed)
	{
		BOOL haschanged = FALSE;
		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				if(curnode->IsItemSold())
				{
					POSITION deletePos = pos;
					m_items.GetNext(pos);
					m_items.RemoveAt(deletePos);
					delete curnode;
					m_item_count--;
					haschanged = TRUE;
					continue;
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		if(changed)
			*changed = haschanged;

		return TRUE;
	}

	CProductItem *GetItem(CString productID)
	{
		if(productID.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				if(productID.CompareNoCase(curnode->m_product_ID) == 0)
					return curnode;
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
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

		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				if(curnode->m_serial_number == serial_number)
					return curnode;
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		return NULL;
	}

	CProductItem *GetItemAt(int position) //0 based index
	{
		if((position < 0) ||
		   (position >= GetItemCount()))
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		POSITION pos = m_items.FindIndex(position);
		if(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(!curnode)
				SHOW_INTERNAL_ERROR;

			return curnode;
		}

		return NULL;
	}

	int GetItemCount()
	{
		return m_item_count;
	}

	int GetSoldItemCount()
	{
		int count = 0;
		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				if(curnode->IsItemSold())
					count++;
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}
		
		return count;
	}

	int GetUnsoldItemCount()
	{
		return (GetItemCount() - GetSoldItemCount());
	}

	int GetTotalItemCount()
	{
		int count = 0;
		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
				count += curnode->m_piece_count;
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		return count;
	}

	int GetTotalSoldItemCount()
	{
		int count = 0;
		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				if(curnode->IsItemSold())
					count += curnode->m_piece_count;
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		return count;
	}

	int GetTotalUnsoldItemCount()
	{
		return (GetTotalItemCount() - GetTotalSoldItemCount());
	}

	BOOL UpdateProductID(CString productID)
	{
		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				CString item_description = curnode->m_description;
				CString old_productID = curnode->m_product_ID;
				if(item_description.IsEmpty())
				{
					if(productID.CompareNoCase(old_productID) != 0)
					{
						if(!curnode->UpdateProductID(productID))
						{
							CString errstr = "Could not update productID of item '" + CString(curnode->m_product_ID) + "' to '" + productID + "' in set '" + CString(m_product_ID) + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);
						}
					}
				}
				else
				{
					CString actualID = CProductItem::ExtractActualID(productID);
					CString setprefix = CProductItem::ExtractPrefix(productID);
					CString prefix = CSettings::GetPrefixFor(curnode->m_description);
					int pieces = CSettings::GetPieceCountFor(curnode->m_description);

					if(prefix != setprefix)
						prefix = prefix + setprefix;

					CString new_productID = prefix + actualID;
					if(new_productID.CompareNoCase(old_productID) != 0)
					{
						if(!curnode->UpdateProductID(new_productID))
						{
							CString errstr = "Could not update productID of item '" + CString(curnode->m_product_ID) + "' to '" + productID + "' in set '" + CString(m_product_ID) + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);
						}

						curnode->m_piece_count = pieces;
					}
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		strcpy(m_product_ID, LPCTSTR(productID));
		return TRUE;
	}

	BOOL HasEmptyItems(int *position)
	{
		if(position)
			*position = -1;

		if(m_items.GetCount() == 0)
			return TRUE;

		if(position)
			*position = 0;

		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				if(curnode->IsEmpty())
					return TRUE;
			}
			else
			{
				SHOW_INTERNAL_ERROR;
				return TRUE;
			}

			if(position)
				(*position)++;

			m_items.GetNext(pos);
		}

		if(position)
			*position = -1;

		return FALSE;
	}

	int UpdateSerialNumbers(int starting)
	{
		if(starting <= 0)
		{
			SHOW_INTERNAL_ERROR;
			return starting;
		}

		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				curnode->m_serial_number = starting;
				starting++;
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		return starting;
	}

	BOOL MarkSetAsSold(BOOL val, CString country)
	{
		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				curnode->m_sold = val;

				if(val)
					strcpy(curnode->m_sold_country, LPCTSTR(country));
				else
					strcpy(curnode->m_sold_country, "");
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		return TRUE;
	}

	BOOL HasOldSerialNumbers(CString tagname)
	{
		if(tagname.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				if(curnode->GetOldSerial(tagname))
					return TRUE;
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		return FALSE;
	}

	BOOL AddOldSerialNumbers(CString tagname)
	{
		if(tagname.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		BOOL retval = TRUE;
		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				CProductOldSerial newnode;
				newnode.m_serial = curnode->m_serial_number;
				strcpy(newnode.m_tagname, LPCTSTR(tagname));
				if(!curnode->AddOldSerial(&newnode))
				{
					CString errstr = "Could not add old serial with tagname '" + tagname + "' to item '" + CString(curnode->m_product_ID) + "' in set '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					retval = FALSE;
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		return retval;
	}

	BOOL DeleteOldSerialNumbers(CString tagname)
	{
		BOOL retval = TRUE;
		BOOL deleteAll = FALSE;
		if(tagname.IsEmpty())
			deleteAll = TRUE;

		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *curnode = m_items.GetAt(pos);
			if(curnode)
			{
				if(deleteAll)
				{
					if(!curnode->DeleteOldSerialList())
					{
						CString errstr = "Could not delete old serial list for item '" + CString(curnode->m_product_ID) + "' in set '" + CString(m_product_ID) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						retval = FALSE;
					}
				}
				else
				{
					if(!curnode->DeleteOldSerial(tagname, NULL))
					{
						CString errstr = "Could not delete old serial with tagname '" + tagname + "' for item '" + CString(curnode->m_product_ID) + "' in set '" + CString(m_product_ID) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						retval = FALSE;
					}
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		return retval;
	}

	CString GenerateSerialString(int *lowest_serial = NULL)
	{
		CString ret;

		CArray<int,int> serial_array;
		serial_array.SetSize(GetItemCount());

		int array_elements = 0;

		//This while loop will pass through all the items and create a sorted array of all serial numbers in the set.
		POSITION pos = m_items.GetHeadPosition();
		while(pos)
		{
			CProductItem *item = m_items.GetAt(pos);
			if(item)
			{
				int serial = item->m_serial_number;

				if(array_elements == 0)
				{
					serial_array[array_elements] = serial;
					array_elements++;
				}
				else
				{
					int cur_index = 0;
					//This will find the correct position of the serial number so the sorting is maintained
					while(cur_index < array_elements)
					{
						if(serial < serial_array[cur_index])
						{
							serial_array.InsertAt(cur_index, serial);
							array_elements++;
							break;
						}

						cur_index++;
					}

					if(cur_index == array_elements)//reached end
					{
						serial_array[cur_index] = serial;
						array_elements++;
					}
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_items.GetNext(pos);
		}

		//If the number of element is 0 or 1, just display it directly.
		if(array_elements <= 1)
		{
			int cur_index = 0;
			while(cur_index < array_elements)
			{
				int serial = serial_array[cur_index];
				CString tempstring;
				tempstring.Format("%d",serial);
				ret = ret + tempstring;
				cur_index++;

				if(cur_index < array_elements)
					ret = ret + ",";
			}
		}
		else
		{
			int cur_index = 0;
			while(cur_index < array_elements)
			{
				//just add the last serial number directly to the string.
				if(cur_index == (array_elements - 1))
				{
					CString tempstring;
					tempstring.Format("%d",serial_array[cur_index]);
					ret = ret + tempstring;
					break;
				}

				int first_index = cur_index;
				int sec_index = cur_index + 1;
				BOOL cont = TRUE;

				while(cont && (sec_index < array_elements))
				{
					if((serial_array[first_index] + 1) == serial_array[sec_index])
					{
						first_index++;
						sec_index++;
					}
					else
					{
						if(cur_index == first_index)
						{
							CString tempstring;
							tempstring.Format("%d,",serial_array[first_index]);
							ret = ret + tempstring;
							cur_index++;
							cont = FALSE;
						}
						else
						{
							CString tempstring;
							tempstring.Format("%d..%d,",serial_array[cur_index],serial_array[first_index]);
							ret = ret + tempstring;
							cur_index = sec_index;
							cont = FALSE;
						}
					}
				}

				if(cont)//combine all elements till end
				{
					CString tempstring;
					tempstring.Format("%d..%d",serial_array[cur_index],serial_array[first_index]);
					ret = ret + tempstring;
					cur_index = sec_index;
				}
			}
		}

		if(lowest_serial)
			*lowest_serial = serial_array[0];

		return ret;
	}
};

#endif // !defined(AFX_PRODUCTSET_H__2378F0B1_647D_4FDA_ADA1_E8237ECB5D80__INCLUDED_)

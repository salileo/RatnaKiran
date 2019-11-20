#if !defined(AFX_PRODUCTITEM_H__D40F259B_3D04_402F_921A_1A821440A567__INCLUDED_)
#define AFX_PRODUCTITEM_H__D40F259B_3D04_402F_921A_1A821440A567__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProductSubItem.h"
#include "ProductOldSerial.h"

class CProductItemData
{
public:
	char m_product_ID[SHORT_STRING];
	char m_description[MEDIUM_STRING];
	int m_piece_count;
	char m_photo_filename[MEDIUM_STRING];  // just the name not full path

	int m_serial_number;
	int m_old_serial_count;

	int m_gold_rate; // in Rs. per Gm. for 18K of gold
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
	double m_costing; // in Rs. per Gm.
	char m_marked_costing[MEDIUM_STRING];

public:
	CProductItemData()
	{ 
		Empty();
	}

	CProductItemData(const CProductItemData &arg)
	{
		Empty();
		Copy(arg);
	}

	~CProductItemData()
	{
		Empty();
	}

	void operator=(const CProductItemData &arg)
	{
		Copy(arg);
	}

	BOOL operator==(CProductItemData &arg)
	{
		return Equal(arg);
	}

protected:
	void Copy(const CProductItemData &arg)
	{
		strcpy(m_product_ID,arg.m_product_ID);
		strcpy(m_description,arg.m_description);
		m_piece_count = arg.m_piece_count;
		strcpy(m_photo_filename,arg.m_photo_filename);

		m_serial_number = arg.m_serial_number;
		m_old_serial_count = arg.m_old_serial_count;

		m_gold_rate = arg.m_gold_rate;
		m_use_net_weight_for_gold = arg.m_use_net_weight_for_gold;

		m_gross_weight = arg.m_gross_weight;
		m_other_weight = arg.m_other_weight;
		
		m_diamond_count = arg.m_diamond_count;
		m_stone_count = arg.m_stone_count;
		m_pearl_count = arg.m_pearl_count;
		m_labour_cost = arg.m_labour_cost;
		strcpy(m_diamond_clarity, arg.m_diamond_clarity);

		strcpy(m_diamond_marked_value,arg.m_diamond_marked_value);
		strcpy(m_stone_marked_value,arg.m_stone_marked_value);
		strcpy(m_pearl_marked_value,arg.m_pearl_marked_value);
		strcpy(m_labour_marked_cost,arg.m_labour_marked_cost);

		m_sold = arg.m_sold;
		strcpy(m_sold_country, arg.m_sold_country);

		m_costing = arg.m_costing;
		strcpy(m_marked_costing, arg.m_marked_costing);
	}

	BOOL Equal(CProductItemData &arg)
	{
		return ((strcmp(m_product_ID, arg.m_product_ID) == 0) &&
				(strcmp(m_description, arg.m_description) == 0) &&
				(m_piece_count == arg.m_piece_count) &&
				(strcmp(m_photo_filename, arg.m_photo_filename) == 0) &&
				(m_serial_number == arg.m_serial_number) &&
				(m_old_serial_count == arg.m_old_serial_count) &&
				(m_gold_rate == arg.m_gold_rate) &&
				(m_use_net_weight_for_gold == arg.m_use_net_weight_for_gold) &&
				(m_gross_weight == arg.m_gross_weight) &&
				(m_other_weight == arg.m_other_weight) &&
				(m_diamond_count == arg.m_diamond_count) &&
				(m_stone_count == arg.m_stone_count) &&
				(m_pearl_count == arg.m_pearl_count) &&
				(m_labour_cost == arg.m_labour_cost) &&
				(strcmp(m_diamond_clarity, arg.m_diamond_clarity) == 0) &&
				(strcmp(m_diamond_marked_value, arg.m_diamond_marked_value) == 0) &&
				(strcmp(m_stone_marked_value, arg.m_stone_marked_value) == 0) &&
				(strcmp(m_pearl_marked_value, arg.m_pearl_marked_value) == 0) &&
				(strcmp(m_labour_marked_cost, arg.m_labour_marked_cost) == 0) &&
				(strcmp(m_sold_country, arg.m_sold_country) == 0) &&
				(m_sold == arg.m_sold) &&
				(m_costing == arg.m_costing) &&
				(strcmp(m_marked_costing, arg.m_marked_costing) == 0));
	}

	void Empty()
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


class CProductItem : public CProductItemData
{
public:
	CList<CProductSubItem*, CProductSubItem*> m_diamonds;
	CList<CProductSubItem*, CProductSubItem*> m_stones;
	CList<CProductSubItem*, CProductSubItem*> m_pearls;
	CList<CProductOldSerial*, CProductOldSerial*> m_old_serials;

public:
	CProductItem()
	{
	}

	CProductItem(const CProductItem &arg)
	{
		Empty();
		Copy((CProductItemData)arg);

		m_diamond_count = 0;
		m_stone_count = 0;
		m_pearl_count = 0;
		m_old_serial_count = 0;

		POSITION pos = NULL;
		
		pos = arg.m_diamonds.GetHeadPosition();
		while(pos)
		{
			CProductSubItem *curnode = arg.m_diamonds.GetAt(pos);
			if(curnode)
			{
				if(!AddSubItem(tDiamond, curnode))
				{
					CString errstr = "Could not add diamond subitem '" + CString(curnode->m_type) + "' to item '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_diamonds.GetNext(pos);
		}

		pos = arg.m_stones.GetHeadPosition();
		while(pos)
		{
			CProductSubItem *curnode = arg.m_stones.GetAt(pos);
			if(curnode)
			{
				if(!AddSubItem(tStone, curnode))
				{
					CString errstr = "Could not add stone subitem '" + CString(curnode->m_type) + "' to item '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_stones.GetNext(pos);
		}

		pos = arg.m_pearls.GetHeadPosition();
		while(pos)
		{
			CProductSubItem *curnode = arg.m_pearls.GetAt(pos);
			if(curnode)
			{
				if(!AddSubItem(tPearl, curnode))
				{
					CString errstr = "Could not add pearl subitem '" + CString(curnode->m_type) + "' to item '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_pearls.GetNext(pos);
		}

		pos = arg.m_old_serials.GetHeadPosition();
		while(pos)
		{
			CProductOldSerial *curnode = arg.m_old_serials.GetAt(pos);
			if(curnode)
			{
				if(!AddOldSerial(curnode))
				{
					CString errstr = curnode->m_tagname;
					errstr = "Could not add old serial with tagname '" + errstr + "' to item '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_old_serials.GetNext(pos);
		}
	}

	~CProductItem()
	{
		DeleteSubItemList(tDiamond);
		DeleteSubItemList(tStone);
		DeleteSubItemList(tPearl);
		DeleteOldSerialList();
	}

	void operator=(const CProductItem &arg)
	{
		DeleteSubItemList(tDiamond);
		DeleteSubItemList(tStone);
		DeleteSubItemList(tPearl);
		DeleteOldSerialList();

		Copy((CProductItemData)arg);

		m_diamond_count = 0;
		m_stone_count = 0;
		m_pearl_count = 0;
		m_old_serial_count = 0;

		POSITION pos = NULL;
		
		pos = arg.m_diamonds.GetHeadPosition();
		while(pos)
		{
			CProductSubItem *curnode = arg.m_diamonds.GetAt(pos);
			if(curnode)
			{
				if(!AddSubItem(tDiamond, curnode))
				{
					CString errstr = "Could not add diamond subitem '" + CString(curnode->m_type) + "' to item '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_diamonds.GetNext(pos);
		}

		pos = arg.m_stones.GetHeadPosition();
		while(pos)
		{
			CProductSubItem *curnode = arg.m_stones.GetAt(pos);
			if(curnode)
			{
				if(!AddSubItem(tStone, curnode))
				{
					CString errstr = "Could not add stone subitem '" + CString(curnode->m_type) + "' to item '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_stones.GetNext(pos);
		}

		pos = arg.m_pearls.GetHeadPosition();
		while(pos)
		{
			CProductSubItem *curnode = arg.m_pearls.GetAt(pos);
			if(curnode)
			{
				if(!AddSubItem(tPearl, curnode))
				{
					CString errstr = "Could not add pearl subitem '" + CString(curnode->m_type) + "' to item '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_pearls.GetNext(pos);
		}

		pos = arg.m_old_serials.GetHeadPosition();
		while(pos)
		{
			CProductOldSerial *curnode = arg.m_old_serials.GetAt(pos);
			if(curnode)
			{
				if(!AddOldSerial(curnode))
				{
					CString errstr = curnode->m_tagname;
					errstr = "Could not add old serial with tagname '" + errstr + "' to item '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			arg.m_old_serials.GetNext(pos);
		}
	}

	BOOL operator==(CProductItem &arg)
	{
		BOOL retval = Equal((CProductItemData)arg);

		POSITION pos1 = m_diamonds.GetHeadPosition();
		POSITION pos2 = arg.m_diamonds.GetHeadPosition();

		while(retval && pos1 && pos2)
		{
			CProductSubItem *type1 = m_diamonds.GetAt(pos1);
			CProductSubItem *type2 = arg.m_diamonds.GetAt(pos2);

			if(type1 && type2)
			{
				if((*(type1)) == (*(type2)))
				{
					m_diamonds.GetNext(pos1);
					arg.m_diamonds.GetNext(pos2);
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

		//One of the list is over, whereas the second still exists.
		if(pos1 || pos2)
			retval = FALSE;

		pos1 = m_stones.GetHeadPosition();
		pos2 = arg.m_stones.GetHeadPosition();

		while(retval && pos1 && pos2)
		{
			CProductSubItem *type1 = m_stones.GetAt(pos1);
			CProductSubItem *type2 = arg.m_stones.GetAt(pos2);

			if(type1 && type2)
			{
				if((*(type1)) == (*(type2)))
				{
					m_stones.GetNext(pos1);
					arg.m_stones.GetNext(pos2);
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

		//One of the list is over, whereas the second still exists.
		if(pos1 || pos2)
			retval = FALSE;

		pos1 = m_pearls.GetHeadPosition();
		pos2 = arg.m_pearls.GetHeadPosition();

		while(retval && pos1 && pos2)
		{
			CProductSubItem *type1 = m_pearls.GetAt(pos1);
			CProductSubItem *type2 = arg.m_pearls.GetAt(pos2);

			if(type1 && type2)
			{
				if((*(type1)) == (*(type2)))
				{
					m_pearls.GetNext(pos1);
					arg.m_pearls.GetNext(pos2);
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

		//One of the list is over, whereas the second still exists.
		if(pos1 || pos2)
			retval = FALSE;

		pos1 = m_old_serials.GetHeadPosition();
		pos2 = arg.m_old_serials.GetHeadPosition();

		while(retval && pos1 && pos2)
		{
			CProductOldSerial *type1 = m_old_serials.GetAt(pos1);
			CProductOldSerial *type2 = arg.m_old_serials.GetAt(pos2);

			if(type1 && type2)
			{
				if((*(type1)) == (*(type2)))
				{
					m_old_serials.GetNext(pos1);
					arg.m_old_serials.GetNext(pos2);
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

		//One of the list is over, whereas the second still exists.
		if(pos1 || pos2)
			retval = FALSE;

		return retval;
	}

	BOOL UpdateProductID(CString new_productID)
	{
		if(new_productID.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CString old_productID = m_product_ID;
		if(new_productID == old_productID)
			return TRUE;

		BOOL success = FALSE;
		BYTE *image_data;
		DWORD image_size = 0;
		CString old_image = m_photo_filename; 
		if(!old_image.IsEmpty())
		{
			if(OpenImage(old_image, &image_data, &image_size))
			{
				CString filename = new_productID;
				if(SaveImage(&filename, image_data, image_size, FALSE))
				{
					if(filename != old_image)
					{
						if(DeleteImage(old_image))
						{
							strcpy(m_photo_filename, LPCTSTR(filename));
							success = TRUE;
						}
						else
						{
							CString errstr = "Could not delete old image file '" + old_image + "' in item '" + CString(m_product_ID) + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);

							if(!DeleteImage(filename))
							{
								CString errstr = "Could not delete newly created image file '" + filename + "' in item '" + CString(m_product_ID) + "'.";
								SHOW_INTERNAL_ERROR_REASON(errstr);
							}
						}
					}
					else
					{
						strcpy(m_photo_filename, LPCTSTR(filename));
						success = TRUE;
					}
				}
				else
				{
					CString errstr = "Could not write image data to file '" + filename + "' in item '" + CString(m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}

				delete image_data;
			}
			else
			{
				CString errstr = "Could not fetch image data from file '" + old_image + "' in item '" + CString(m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}
		else
		{
			success = TRUE;
		}

		if(success)
		{
			strcpy(m_product_ID, LPCTSTR(new_productID));
			return TRUE;
		}

		return FALSE;
	}

	BOOL AddSubItem(SubItemType type, CProductSubItem *item)
	{
		if(!item)
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;
		int *tempCount = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			tempCount = &m_diamond_count;
			break;
		case tStone:
			tempList = &m_stones;
			tempCount = &m_stone_count;
			break;
		case tPearl:
			tempList = &m_pearls;
			tempCount = &m_pearl_count;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CProductSubItem *newnode = NULL;
		SALIL_NEW(newnode, CProductSubItem);

		*newnode = *item;
		tempList->AddTail(newnode);
		(*tempCount)++;
		return TRUE;
	}

	BOOL DeleteSubItemAt(SubItemType type, int position)
	{
		if((position < 0) ||
		   (position >= GetSubItemCount(type)))
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;
		int *tempCount = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			tempCount = &m_diamond_count;
			break;
		case tStone:
			tempList = &m_stones;
			tempCount = &m_stone_count;
			break;
		case tPearl:
			tempList = &m_pearls;
			tempCount = &m_pearl_count;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		POSITION pos = tempList->FindIndex(position);
		if(pos)
		{
			CProductSubItem *curnode = tempList->GetAt(pos);
			if(curnode)
			{
				tempList->RemoveAt(pos);
				delete curnode;
				(*tempCount)--;
				return TRUE;
			}
			else
				SHOW_INTERNAL_ERROR;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", position);
			errstr = "Could not delete subitem at position '" + errstr + "' in item '" + CString(m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return FALSE;
	}

	BOOL DeleteSubItemList(SubItemType type)
	{
		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;
		int *tempCount = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			tempCount = &m_diamond_count;
			break;
		case tStone:
			tempList = &m_stones;
			tempCount = &m_stone_count;
			break;
		case tPearl:
			tempList = &m_pearls;
			tempCount = &m_pearl_count;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		while(!tempList->IsEmpty())
		{
			CProductSubItem *temp = tempList->RemoveHead();
			if(temp)
				delete temp;
			else
				SHOW_INTERNAL_ERROR;
		}

		*tempCount = 0;
		return TRUE;
	}

	CProductSubItem *GetSubItemAt(SubItemType type, int position)
	{
		if((position < 0) ||
		   (position >= GetSubItemCount(type)))
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			break;
		case tStone:
			tempList = &m_stones;
			break;
		case tPearl:
			tempList = &m_pearls;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return NULL;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		POSITION pos = tempList->FindIndex(position);
		if(pos)
		{
			CProductSubItem *curnode = tempList->GetAt(pos);
			if(!curnode)
				SHOW_INTERNAL_ERROR;

			return curnode;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", position);
			errstr = "Could not get subitem at position '" + errstr + "' in item '" + CString(m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return NULL;
	}

	int GetSubItemCount(SubItemType type)
	{
		int *tempCount = NULL;

		switch(type)
		{
		case tDiamond:
			tempCount = &m_diamond_count;
			break;
		case tStone:
			tempCount = &m_stone_count;
			break;
		case tPearl:
			tempCount = &m_pearl_count;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return 0;
			}
		}

		if(!tempCount)
		{
			SHOW_INTERNAL_ERROR;
			return 0;
		}

		return (*tempCount);
	}

	BOOL AddOldSerial(CProductOldSerial *item)
	{
		if(!item)
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		CProductOldSerial *old_item = GetOldSerial(item->m_tagname);
		if(old_item != NULL)
		{
			old_item->m_serial = item->m_serial;
			return TRUE;
		}
		else
		{
			CProductOldSerial *newnode = NULL;
			SALIL_NEW(newnode, CProductOldSerial);

			*newnode = *item;
			m_old_serials.AddTail(newnode);
			m_old_serial_count++;
			return TRUE;
		}
	}

	BOOL DeleteOldSerial(CString tagname, BOOL *changed)
	{
		BOOL haschanged = FALSE;
		if(tagname.IsEmpty())
		{
			if(changed)
				*changed = haschanged;

			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		POSITION pos = m_old_serials.GetHeadPosition();
		while(pos)
		{
			CProductOldSerial *curnode = m_old_serials.GetAt(pos);
			if(curnode)
			{
				if(tagname.CompareNoCase(curnode->m_tagname) == 0)
				{
					POSITION deletePos = pos;
					m_old_serials.GetNext(pos);
					m_old_serials.RemoveAt(deletePos);
					delete curnode;
					m_old_serial_count--;
					haschanged = TRUE;
					continue;
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			m_old_serials.GetNext(pos);
		}

		if(changed)
			*changed = haschanged;

		return TRUE;
	}

	BOOL DeleteOldSerialAt(int position)
	{
		if((position < 0) ||
		   (position >= GetOldSerialCount()))
		{
			SHOW_INTERNAL_ERROR;
			return FALSE;
		}

		POSITION pos = m_old_serials.FindIndex(position);
		if(pos)
		{
			CProductOldSerial *curnode = m_old_serials.GetAt(pos);
			if(curnode)
			{
				m_old_serials.RemoveAt(pos);
				delete curnode;
				m_old_serial_count--;
				return TRUE;
			}
			else
				SHOW_INTERNAL_ERROR;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", position);
			errstr = "Could not delete old serial at position '" + errstr + "' in item '" + CString(m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return FALSE;
	}

	BOOL DeleteOldSerialList()
	{
		while(!m_old_serials.IsEmpty())
		{
			CProductOldSerial *temp = m_old_serials.RemoveHead();
			if(temp)
				delete temp;
			else
				SHOW_INTERNAL_ERROR;
		}

		m_old_serial_count = 0;
		return TRUE;
	}

	CProductOldSerial *GetOldSerial(CString tagname)
	{
		if(tagname.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		POSITION pos = m_old_serials.GetHeadPosition();
		while(pos)
		{
			CProductOldSerial *curnode = m_old_serials.GetAt(pos);
			if(curnode)
			{
				if(tagname.CompareNoCase(curnode->m_tagname) == 0)
					return curnode;
			}
			else
				SHOW_INTERNAL_ERROR;

			m_old_serials.GetNext(pos);
		}

		return NULL;
	}

	CProductOldSerial *GetOldSerialAt(int position)
	{
		if((position < 0) ||
		   (position >= GetOldSerialCount()))
		{
			SHOW_INTERNAL_ERROR;
			return NULL;
		}

		POSITION pos = m_old_serials.FindIndex(position);
		if(pos)
		{
			CProductOldSerial *curnode = m_old_serials.GetAt(pos);
			if(!curnode)
				SHOW_INTERNAL_ERROR;

			return curnode;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", position);
			errstr = "Could not get old serial at position '" + errstr + "' in item '" + CString(m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return NULL;
	}

	int GetOldSerialCount()
	{
		return m_old_serial_count;
	}
	
	BOOL IsItemSold()
	{
		return m_sold;
	}

	BOOL IsEmpty()
	{
		return ((m_gross_weight == 0.0) ||
				(((m_labour_cost == 0) || 
				  ((m_diamond_count == 0) && 
				   (m_stone_count == 0) && 
				   (m_pearl_count == 0) && 
				   (m_other_weight == 0.0))) &&
				 (m_costing == 0.0)));
	}

	double GetGrossWeight()
	{
		return m_gross_weight;
	}

	double GetNetWeight()
	{
		return (GetGrossWeight() 
			    - CProductSubItem::GramsFromCaret(GetSubItemWeight(tDiamond) + 
												GetSubItemWeight(tStone) + 
												GetSubItemWeight(tPearl)) 
				- m_other_weight);
	}

	int GetGoldValue(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion)
	{
		double value = GetGoldValueDouble(use_net, include_wastage, gold_rate, conversion);
		return (int)(value + 0.5);
	}

	double GetGoldValueDouble(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion)
	{
		double value = 0.0;
		double weight = 0.0;

		if(use_net)
			weight = GetNetWeight();
		else
			weight = GetGrossWeight();

		value = weight * gold_rate;

		if(include_wastage)
			value = (value * 6.0) / 5.0;

		value = value / conversion;
		return value;
	}

	double GetSubItemWeight(SubItemType type)
	{
		double weight = 0.0;
		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			break;
		case tStone:
			tempList = &m_stones;

			break;
		case tPearl:
			tempList = &m_pearls;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return weight;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return weight;
		}

		POSITION pos = tempList->GetHeadPosition();
		while(pos)
		{
			CProductSubItem *curnode = tempList->GetAt(pos);
			if(curnode)
				weight += curnode->m_weight;
			else
				SHOW_INTERNAL_ERROR;

			tempList->GetNext(pos);
		}

		return weight;
	}

	int GetSubItemValue(SubItemType type, double conversion)
	{
		double value = GetSubItemValueDouble(type, conversion);
		return (int)(value + 0.5);
	}

	double GetSubItemValueDouble(SubItemType type, double conversion)
	{
		double value = 0.0;
		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			break;
		case tStone:
			tempList = &m_stones;
			break;
		case tPearl:
			tempList = &m_pearls;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return value;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return value;
		}

		POSITION pos = tempList->GetHeadPosition();
		while(pos)
		{
			CProductSubItem *curnode = tempList->GetAt(pos);
			if(curnode)
				value += (curnode->m_weight * curnode->m_rate);
			else
				SHOW_INTERNAL_ERROR;

			tempList->GetNext(pos);
		}

		value = value / conversion;
		return value;
	}

	int GetSubItemValueUsing(SubItemType type, int rate, double conversion)
	{
		double value = GetSubItemValueUsingDouble(type, rate, conversion);
		return (int)(value + 0.5);
	}

	double GetSubItemValueUsingDouble(SubItemType type, int rate, double conversion)
	{
		double value = 0.0;
		value = (GetSubItemWeight(type) * (double)rate) / conversion;
		return value;
	}

	int GetSubItemRate(SubItemType type, double conversion)
	{
		double rate = 0.0;

		if(GetSubItemWeight(type) != 0.0)
		{
			rate = GetSubItemValueDouble(type, 1.0) / GetSubItemWeight(type);
			rate = rate / conversion;
		}

		return (int)(rate + 0.5);
	}

	int GetSubItemPieceCount(SubItemType type)
	{
		int count = 0;
		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			break;
		case tStone:
			tempList = &m_stones;
			break;
		case tPearl:
			tempList = &m_pearls;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return count;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return count;
		}

		POSITION pos = tempList->GetHeadPosition();
		while(pos)
		{
			CProductSubItem *curnode = tempList->GetAt(pos);
			if(curnode)
				count += curnode->m_count;
			else
				SHOW_INTERNAL_ERROR;

			tempList->GetNext(pos);
		}

		return count;
	}

	int GetMarkedSubItemValue(SubItemType type, double gold_rate, double conversion)
	{
		double val = GetMarkedSubItemValueDouble(type, gold_rate, conversion);
		return (int)(val + 0.5);
	}

	double GetMarkedSubItemValueDouble(SubItemType type, double gold_rate, double conversion)
	{
		CString formula;
		
		switch(type)
		{
		case tDiamond:
			formula = m_diamond_marked_value;
			break;
		case tStone:
			formula = m_stone_marked_value;
			break;
		case tPearl:
			formula = m_pearl_marked_value;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return 0.0;
			}
		}
		
		double val = ProcessFormula(formula, gold_rate, 1.0, this);
		val = val / conversion;
		return val;
	}

	int GetMarkedSubItemRate(SubItemType type, double gold_rate, double conversion)
	{
		double rate = 0.0;

		if(GetSubItemWeight(type) != 0.0)
		{
			rate = GetMarkedSubItemValueDouble(type, gold_rate, 1.0) / GetSubItemWeight(type);
			rate = rate / conversion;
		}

		return (int)(rate + 0.5);
	}

	char *GetSubItemTypeOf(SubItemType type, int index)
	{
		if((index < 0) ||
		   (index >= GetSubItemCount(type)))
			return "";

		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			break;
		case tStone:
			tempList = &m_stones;
			break;
		case tPearl:
			tempList = &m_pearls;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return "";
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return "";
		}

		POSITION pos = tempList->FindIndex(index);
		if(pos)
		{
			CProductSubItem *curnode = tempList->GetAt(pos);
			if(curnode)
				return curnode->m_type;
			else
				SHOW_INTERNAL_ERROR;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get subitem at position '" + errstr + "' in item '" + CString(m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return "";
	}

	double GetSubItemWeightOf(SubItemType type, int index)
	{
		if((index < 0) ||
		   (index >= GetSubItemCount(type)))
		{
			SHOW_INTERNAL_ERROR;
			return 0.0;
		}

		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			break;
		case tStone:
			tempList = &m_stones;
			break;
		case tPearl:
			tempList = &m_pearls;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return 0.0;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return 0.0;
		}

		POSITION pos = tempList->FindIndex(index);
		if(pos)
		{
			CProductSubItem *curnode = tempList->GetAt(pos);
			if(curnode)
				return curnode->m_weight;
			else
				SHOW_INTERNAL_ERROR;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get subitem at position '" + errstr + "' in item '" + CString(m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return 0.0;
	}

	int GetSubItemValueOf(SubItemType type, int index, double conversion)
	{
		double value = GetSubItemValueOfDouble(type, index, conversion);
		return (int)(value + 0.5);
	}

	double GetSubItemValueOfDouble(SubItemType type, int index, double conversion)
	{
		if((index < 0) ||
		   (index >= GetSubItemCount(type)))
		{
			SHOW_INTERNAL_ERROR;
			return 0.0;
		}

		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			break;
		case tStone:
			tempList = &m_stones;
			break;
		case tPearl:
			tempList = &m_pearls;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return 0.0;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return 0.0;
		}

		POSITION pos = tempList->FindIndex(index);
		if(pos)
		{
			CProductSubItem *curnode = tempList->GetAt(pos);
			if(curnode)
			{
				double value = curnode->m_weight * curnode->m_rate;
				value = value / conversion;
				return value;
			}
			else
				SHOW_INTERNAL_ERROR;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get subitem at position '" + errstr + "' in item '" + CString(m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return 0.0;
	}

	int GetSubItemValueOfUsing(SubItemType type, int index, int rate, double conversion)
	{
		double value = GetSubItemValueOfUsingDouble(type, index, rate, conversion);
		return (int)(value + 0.5);
	}

	double GetSubItemValueOfUsingDouble(SubItemType type, int index, int rate, double conversion)
	{
		double value = GetSubItemWeightOf(type, index) * rate;
		value = value / conversion;
		return value;
	}

	int GetSubItemRateOf(SubItemType type, int index, double conversion)
	{
		if((index < 0) ||
		   (index >= GetSubItemCount(type)))
		{
			SHOW_INTERNAL_ERROR;
			return 0;
		}

		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			break;
		case tStone:
			tempList = &m_stones;
			break;
		case tPearl:
			tempList = &m_pearls;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return 0;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return 0;
		}

		POSITION pos = tempList->FindIndex(index);
		if(pos)
		{
			CProductSubItem *curnode = tempList->GetAt(pos);
			if(curnode)
			{
				double rate = curnode->m_rate / conversion;
				return (int)(rate + 0.5);
			}
			else
				SHOW_INTERNAL_ERROR;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get subitem at position '" + errstr + "' in item '" + CString(m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return 0;
	}

	int GetSubItemPieceCountOf(SubItemType type, int index)
	{
		if((index < 0) ||
		   (index >= GetSubItemCount(type)))
		{
			SHOW_INTERNAL_ERROR;
			return 0;
		}

		CList<CProductSubItem*, CProductSubItem*> *tempList = NULL;

		switch(type)
		{
		case tDiamond:
			tempList = &m_diamonds;
			break;
		case tStone:
			tempList = &m_stones;
			break;
		case tPearl:
			tempList = &m_pearls;
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", type);
				errstr = "Invalid subitem type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return 0;
			}
		}

		if(!tempList)
		{
			SHOW_INTERNAL_ERROR;
			return 0;
		}

		POSITION pos = tempList->FindIndex(index);
		if(pos)
		{
			CProductSubItem *curnode = tempList->GetAt(pos);
			if(curnode)
				return curnode->m_count;
			else
				SHOW_INTERNAL_ERROR;
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get subitem at position '" + errstr + "' in item '" + CString(m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		return 0;
	}

	int GetMarkedSubItemValueOf(SubItemType type, int index, double gold_rate, double conversion)
	{
		double value = GetMarkedSubItemValueOfDouble(type, index, gold_rate, conversion);
		return (int)(value + 0.5);
	}

	double GetMarkedSubItemValueOfDouble(SubItemType type, int index, double gold_rate, double conversion)
	{
		double ratio = GetMarkedSubItemValueDouble(type, gold_rate, 1.0) / GetSubItemValueDouble(type, 1.0);
		double value = GetSubItemValueOfDouble(type, index, 1.0) * ratio;
		value = value / conversion;
		return value;
	}

	int GetMarkedSubItemRateOf(SubItemType type, int index, double gold_rate, double conversion)
	{
		double weight = GetSubItemWeightOf(type, index);
		if(weight == 0.0)
			return 0;

		double rate = GetMarkedSubItemValueOfDouble(type, index, gold_rate, 1.0) / weight;
		rate = rate / conversion;
		return (int)(rate + 0.5);
	}

	int GetLabourCost(double conversion)
	{
		double cost = GetLabourCostDouble(conversion);
		return (int)(cost + 0.5);
	}

	double GetLabourCostDouble(double conversion)
	{
		double cost = m_labour_cost / conversion;
		return cost;
	}

	int GetMarkedLabourCost(double gold_rate, double conversion)
	{
		double val = GetMarkedLabourCostDouble(gold_rate, conversion);
		return (int)(val + 0.5);
	}

	double GetMarkedLabourCostDouble(double gold_rate, double conversion)
	{
		CString formula = m_labour_marked_cost;
		double val = ProcessFormula(formula, gold_rate, 1.0, this);
		val = val / conversion;
		return val;
	}

	double GetOtherWeight()
	{
		return m_other_weight;
	}

	int GetCosting(double conversion)
	{
		double val = GetCostingDouble(conversion);
		return (int)(val + 0.5);
	}

	double GetCostingDouble(double conversion)
	{
		double value = m_costing / conversion;
		return value;
	}

	int GetMarkedCosting(double conversion)
	{
		double val = GetMarkedCostingDouble(conversion);
		return (int)(val + 0.5);
	}

	double GetMarkedCostingDouble(double conversion)
	{
		CString formula = m_marked_costing;
		double val = ProcessFormula(formula, 0.0, 1.0, this);
		val = val / conversion;
		return val;
	}

	int GetTotalCosting(double conversion)
	{
		double val = GetTotalCostingDouble(conversion);
		return (int)(val + 0.5);
	}

	double GetTotalCostingDouble(double conversion)
	{
		double val = GetGrossWeight() * m_costing;
		val = val / conversion;
		return val;
	}

	int GetTotalMarkedCosting(double conversion)
	{
		double val = GetTotalMarkedCostingDouble(conversion);
		return (int)(val + 0.5);
	}

	double GetTotalMarkedCostingDouble(double conversion)
	{
		double val =  GetGrossWeight() * GetMarkedCostingDouble(1.0);
		val = val / conversion;
		return val;
	}

	int GetTotalValue(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion)
	{
		double total = GetTotalValueDouble(use_net, include_wastage, gold_rate, conversion);
		return (int)(total + 0.5);
	}

	double GetTotalValueDouble(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion)
	{
		double total = GetSubItemValueDouble(tDiamond, 1.0) +
					   GetSubItemValueDouble(tStone, 1.0) +
					   GetSubItemValueDouble(tPearl, 1.0) +
					   GetLabourCostDouble(1.0) +
					   GetGoldValueDouble(use_net, include_wastage, gold_rate, 1.0);

		total = total / conversion;
		return total;
	}

	int GetTotalMarkedValue(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion)
	{
		double total = GetTotalMarkedValueDouble(use_net, include_wastage, gold_rate, conversion);
		return (int)(total + 0.5);
	}

	double GetTotalMarkedValueDouble(BOOL use_net, BOOL include_wastage, double gold_rate, double conversion)
	{
		double total = GetMarkedSubItemValueDouble(tDiamond, gold_rate, 1.0) +
					   GetMarkedSubItemValueDouble(tStone, gold_rate, 1.0) +
					   GetMarkedSubItemValueDouble(tPearl, gold_rate, 1.0) +
					   GetMarkedLabourCostDouble(gold_rate, 1.0) +
					   GetGoldValueDouble(use_net, include_wastage, gold_rate, 1.0);
		
		total = total / conversion;
		return total;
	}

	CString ExtractActualID()
	{
		return CProductItem::ExtractActualID(CString(m_product_ID));
	}

	CString ExtractPrefix()
	{
		return CProductItem::ExtractPrefix(CString(m_product_ID));
	}

	static CString GetProductID()
	{
		static BOOL firstrun = TRUE;
		if(firstrun)
		{
			firstrun = FALSE;
			srand((unsigned)time(NULL));
		}

		int number = rand()%1000;
		int firstchar = (rand()%26) + 65;

		CString id;
		id.Format("%c%03d",firstchar, number);
		return id;
	}

	static CString ExtractActualID(CString original)
	{
		CString finalid = original;

		int index = original.ReverseFind('_');
		if(index >= 0)
			finalid = original.Right(original.GetLength() - index - 1);

		return finalid;
	}

	static CString ExtractPrefix(CString original)
	{
		CString finalid;

		int index = original.ReverseFind('_');
		if(index >= 0)
			finalid = original.Left(index + 1);

		return finalid;
	}

	static CPoint ScaleImage(CSize original, CSize desired, CSize *final)
	{
		CPoint pos(0,0);

		if(!final)
		{
			SHOW_INTERNAL_ERROR;
			return pos;
		}

		double dx = (double)original.cx/desired.cx;
		double dy = (double)original.cy/desired.cy;

		if((dx <= 1.0) && (dy <= 1.0))
		{
			*final = original;
			
			pos.x = (desired.cx - original.cx)/2;
			pos.y = (desired.cy - original.cy)/2;
		}
		else if(dx >= dy)
		{
			final->cx = desired.cx;
			final->cy = (long)(original.cy / dx);

			pos.x = 0;
			pos.y = (desired.cy - final->cy)/2;
		}
		else
		{
			final->cy = desired.cy;
			final->cx = (long)(original.cx / dy);

			pos.y = 0;
			pos.x = (desired.cx - final->cx)/2;
		}

		return pos;
	}
};

#endif // !defined(AFX_PRODUCTITEM_H__D40F259B_3D04_402F_921A_1A821440A567__INCLUDED_)

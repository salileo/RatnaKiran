#if !defined(AFX_PRODUCTSUBITEM_H__F5A9C31E_1428_41A9_B3BC_1F96CE8114CA__INCLUDED_)
#define AFX_PRODUCTSUBITEM_H__F5A9C31E_1428_41A9_B3BC_1F96CE8114CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SHORT_STRING 50
#define MEDIUM_STRING 256
#define LONG_STRING 1024

enum SubItemType {tDiamond, tStone, tPearl};

class CProductSubItemData
{
public:
	char m_type[SHORT_STRING];
	double m_weight; // in carets
	int m_rate;
	int m_count;

public:
	CProductSubItemData()
	{
		Empty();
	}

	CProductSubItemData(const CProductSubItemData &arg)
	{
		Empty();
		Copy(arg);
	}

	~CProductSubItemData()
	{
		Empty();
	}

	void operator=(const CProductSubItemData &arg)
	{
		Copy(arg);
	}

	BOOL operator==(CProductSubItemData &arg)
	{
		return Equal(arg);
	}

protected:
	void Copy(const CProductSubItemData &arg)
	{
		strcpy(m_type, arg.m_type);
		m_weight = arg.m_weight;
		m_rate = arg.m_rate;
		m_count = arg.m_count;
	}

	BOOL Equal(CProductSubItemData &arg)
	{
		return ((strcmp(m_type, arg.m_type) == 0) &&
				(m_weight == arg.m_weight) &&
				(m_count == arg.m_count) &&
				(m_rate == arg.m_rate));
	}

	void Empty()
	{
		strcpy(m_type,"");
		m_weight = 0.0;
		m_rate = 0;
		m_count = 0;
	}
};


class CProductSubItem  : public CProductSubItemData
{
public:
	CProductSubItem()
	{
	}

	CProductSubItem(const CProductSubItem &arg)
	{
		Empty();
		Copy((CProductSubItemData)arg);
	}

	~CProductSubItem()
	{
	}

	void operator=(const CProductSubItem &arg)
	{
		Copy((CProductSubItemData)arg);
	}

	BOOL operator==(CProductSubItem &arg)
	{
		return Equal((CProductSubItemData)arg);
	}

	static double CaretFromGrams(double grams)
	{
		return (grams * 5.0);
	}

	static double GramsFromCaret(double carets)
	{
		return (carets / 5.0);
	}
};

#endif // !defined(AFX_PRODUCTSUBITEM_H__F5A9C31E_1428_41A9_B3BC_1F96CE8114CA__INCLUDED_)

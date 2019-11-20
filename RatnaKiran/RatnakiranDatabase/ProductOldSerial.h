#if !defined(AFX_PRODUCTOLDSERIAL_H__F5A9C31E_1428_41A9_B3BC_1F96CE8114CA__INCLUDED_)
#define AFX_PRODUCTOLDSERIAL_H__F5A9C31E_1428_41A9_B3BC_1F96CE8114CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProductOldSerialData
{
public:
	char m_tagname[MEDIUM_STRING];
	int m_serial;

public:
	CProductOldSerialData()
	{
		Empty();
	}

	CProductOldSerialData(const CProductOldSerialData &arg)
	{
		Empty();
		Copy(arg);
	}

	~CProductOldSerialData()
	{
		Empty();
	}

	void operator=(const CProductOldSerialData &arg)
	{
		Copy(arg);
	}

	BOOL operator==(CProductOldSerialData &arg)
	{
		return Equal(arg);
	}

protected:
	void Copy(const CProductOldSerialData &arg)
	{
		strcpy(m_tagname, arg.m_tagname);
		m_serial = arg.m_serial;
	}

	BOOL Equal(CProductOldSerialData &arg)
	{
		return ((strcmp(m_tagname, arg.m_tagname) == 0) &&
				(m_serial == arg.m_serial));
	}

	void Empty()
	{
		strcpy(m_tagname,"");
		m_serial = 0;
	}
};


class CProductOldSerial  : public CProductOldSerialData
{
public:
	CProductOldSerial()
	{
	}

	CProductOldSerial(const CProductOldSerial &arg)
	{
		Empty();
		Copy((CProductOldSerialData)arg);
	}

	~CProductOldSerial()
	{
	}

	void operator=(const CProductOldSerial &arg)
	{
		Copy((CProductOldSerialData)arg);
	}

	BOOL operator==(CProductOldSerial &arg)
	{
		return Equal((CProductOldSerialData)arg);
	}
};

#endif // !defined(AFX_PRODUCTOLDSERIAL_H__F5A9C31E_1428_41A9_B3BC_1F96CE8114CA__INCLUDED_)

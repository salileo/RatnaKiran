#ifndef _database_globals_h_
#define _database_globals_h_

#include "Common.h"

class CProductFileHeader
{
public:
	CProductFileHeader()
	{
		strcpy(m_version, "V003");
	}

	char m_version[5];
};

extern CString gFilePath;
extern CList<CString, CString> gIDList;

class CProductVoucher;
class CProductSet;
class CProductItem;
class CProductSubItem;
class CProductOldASerial;

double ProcessFormula(CString formula, double gold_rate, double conversion, CProductItem *item);
void ReplaceDollarWithValue(CString *formula, double value);
double GetGoldRate(CProductVoucher *voucher);
BOOL SaveImage(CString *filename, BYTE *data, DWORD size, BOOL overwrite);
BOOL OpenImage(CString filename, BYTE **data, DWORD *size);
BOOL DeleteImage(CString filename);

#include "ProductVoucher.h"
#include "Settings.h"
#include "Permissions.h"
#include "DatabaseConverter.h"

#endif
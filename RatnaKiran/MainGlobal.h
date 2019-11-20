#ifndef _main_globals_h_
#define _main_globals_h_

struct SStringPairs
{
public:
	SStringPairs()
	{
		m_string1 = "";
		m_string2 = "";
	}

	~SStringPairs()
	{
	}

	CString m_string1;
	CString m_string2;
};

enum ReportDatabase {Stock_Report, Export_Report, Kundan_Report};
enum ReportType {Original_Report, Sold_Report, UnSold_Report};

#include "DatabaseGlobals.h"
#include "Mainfrm.h"
#include "RatnaKiranDoc.h"
#include "RatnaKiranStockView.h"
#include "RatnaKiranTreeView.h"

int ShowErrorMessage(CString message, CList<CString, CString> *IDList, BOOL try_again);
BOOL MakeGlobalsValid(BOOL check_selection = TRUE);

extern CRatnaKiranDoc *gDoc;
extern CRatnaKiranStockView *gStockView;
extern CRatnaKiranTreeView *gTreeView;
extern CMainFrame *gFrame;


#endif
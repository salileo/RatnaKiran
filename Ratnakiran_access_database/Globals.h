#ifndef _globals_h_
#define _globals_h_

#include "TableVoucher.h"
#include "TableDescription.h"
#include "TableSettings.h"
#include "MainFrm.h"
#include "RatnakiranDoc.h"
#include "RatnakiranStockView.h"
#include "RatnakiranTreeView.h"

extern CRatnakiranStockView *gStockView;
extern CRatnakiranTreeView *gTreeView;
extern CMainFrame *gFrame;
extern CRatnakiranDoc *gDoc;
extern CTableDescription gDesc;
extern CTableSettings gSettings;

enum Dialog_Types { TYPE_INT, TYPE_DOUBLE, TYPE_STRING };

struct CProductIDList
{
public:
	CProductIDList()
	{
		m_productID = "";
		m_voucherName = "";
	}

	~CProductIDList()
	{
	}

	CString m_productID;
	CString m_voucherName;
};

void UnHandledError(CString file, int line);
int ShowErrorMessage(CString message, CList<CString, CString> *IDList, BOOL try_again);
BOOL MakeGlobalsValid();
int FindMenuItem(CMenu* menu, CString menustring);
CString GetFileName(char *filtername, char *filtervalue, char *defaultextension, BOOL forOpen);
void ReplaceDollarWithValue(CString *formula, double value);
double ProcessFormula(CString formula, double gold_rate, double conversion, CTableItem *item);
double RoundOff(double val, int decimal_places);
BOOL IsNumber(CString str);
double GramsFromCaret(double carets);
double CaretFromGrams(double grams);
CString GetProductID();

#define BEGIN_WAIT \
{\
	CRatnakiranApp *app = (CRatnakiranApp *)AfxGetApp();\
	app->BeginWaitCursor();\
}

#define END_WAIT \
{\
	CRatnakiranApp *app = (CRatnakiranApp *)AfxGetApp();\
	app->EndWaitCursor();\
}

#define DB_PERFORM_RET(a, b, c, d) \
{\
BOOL perform_retval = TRUE;\
BOOL perform_error = FALSE;\
try { perform_retval = a; }\
catch(CDBException *e)\
{\
::MessageBox(NULL,e->m_strError,_T("Error"),MB_ICONERROR | MB_OK);\
e->Delete();\
perform_error = TRUE;\
}\
if(!perform_retval && !perform_error)\
{\
::MessageBox(NULL,_T(b),_T("Error"),MB_ICONERROR | MB_OK);\
perform_error = TRUE;\
}\
if(perform_error)\
{\
c;\
}\
else\
{\
d;\
}\
}

#define DB_PERFORM(a, b, c) \
{\
BOOL perform_error = FALSE;\
try { a; }\
catch(CDBException *e)\
{\
::MessageBox(NULL,e->m_strError,_T("Error"),MB_ICONERROR | MB_OK);\
e->Delete();\
perform_error = TRUE;\
}\
if(perform_error)\
{\
b;\
}\
else\
{\
c;\
}\
}

#define DB_IS_EMPTY(a) (a.IsBOF() && a.IsEOF())
#define DB_EMPTY (IsBOF() && IsEOF())

#define SALIL_NEW(var_ptr,type) \
{\
try\
{\
var_ptr = new type;\
if(!var_ptr)\
{\
::MessageBox(NULL, "Out of memory", "ERROR", MB_OK);\
exit(1);\
}\
}\
catch(CMemoryException *memerror)\
{\
memerror = NULL;\
::MessageBox(NULL, "Out of memory", "ERROR", MB_OK);\
exit(1);\
}\
}

#define ROUNDUP(x) ((int)(x + 0.5))

#endif
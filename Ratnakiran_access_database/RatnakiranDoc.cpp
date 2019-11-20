#include "stdafx.h"
#include "Ratnakiran.h"
#include "RatnakiranDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRatnakiranDoc, CDocument)

BEGIN_MESSAGE_MAP(CRatnakiranDoc, CDocument)
	//{{AFX_MSG_MAP(CRatnakiranDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CRatnakiranDoc::CRatnakiranDoc()
{
}

CRatnakiranDoc::~CRatnakiranDoc()
{
}

BOOL CRatnakiranDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	gDoc = this;
	gTreeView = NULL;
	gStockView = NULL;

	if(!m_vouchers.DBOpen("OnNewDocument", TRUE))
		return FALSE;

	m_vouchers.GetSetCount(TRUE);
	
	DB_PERFORM_RET(
	(gDesc.Open()),
	"Unable to access description table. Please set database location in ODBC client and retry.",
	{return FALSE;},
	{});

	DB_PERFORM_RET(
	(gSettings.Open()),
	"Unable to access settings table. Please set database location in ODBC client and retry.",
	{return FALSE;},
	{});

	return TRUE;
}


#ifdef _DEBUG
void CRatnakiranDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRatnakiranDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


BOOL CRatnakiranDoc::GetVoucherList(CString type, CList<CString, CString> *list)
{
	if(!list)
	{
		UnHandledError(__FILE__, __LINE__);
		return FALSE;
	}

	list->RemoveAll();

	if(!MakeGlobalsValid())
		return FALSE;

	if(DB_IS_EMPTY(m_vouchers))
		return TRUE;

	return m_vouchers.GetList(type, list);
}


BOOL CRatnakiranDoc::CreateNewVoucher(CString type, CString ID)
{
	if(!MakeGlobalsValid())
		return FALSE;

	if((ID == STOCK_MAIN) ||
	   (ID == EXPORT_MAIN) ||
	   (ID == KUNDAN_MAIN))
		return FALSE;

	CString current_ID = m_vouchers.m_record.m_Voucher_ID;

	if(!m_vouchers.DBAddEntry())
		return FALSE;

	m_vouchers.m_record.m_Voucher_ID = ID;
	m_vouchers.m_record.m_Type = type;

	if(!m_vouchers.DBUpdateEntry("CreateNewVoucher"))
		return FALSE;

	if(m_vouchers.DBRequeryEntries("CreateNewVoucher", FALSE) != DBOK)
		return FALSE;

	if(m_vouchers.GotoVoucher(current_ID) != DBOK)
		return FALSE;

	return TRUE;
}


BOOL CRatnakiranDoc::DeleteVoucher(CString ID)
{
	if(!MakeGlobalsValid())
		return FALSE;

	if((ID == STOCK_MAIN) ||
	   (ID == EXPORT_MAIN) ||
	   (ID == KUNDAN_MAIN))
		return FALSE;

	CString current_ID = m_vouchers.m_record.m_Voucher_ID;
	if(current_ID == ID)
		current_ID = "";
	
	if(m_vouchers.GotoVoucher(ID) != DBOK)
		return FALSE;

	if(!m_vouchers.DBDeleteEntry())
		return FALSE;

	DatabaseState state = m_vouchers.DBRequeryEntries("DeleteVoucher", FALSE);

	if(state == DBInvalid)
		return FALSE;
	else if(state == DBEmpty)
		return TRUE;

	if(current_ID.IsEmpty())
		return (m_vouchers.GotoFirstVoucher() == DBOK);
	else
		return (m_vouchers.GotoVoucher(current_ID) == DBOK);
}

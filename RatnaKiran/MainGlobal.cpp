#include "StdAfx.h"
#include "RatnaKiran.h"
#include "Resource.h"
#include "MainGlobal.h"

CRatnaKiranDoc *gDoc;
CRatnaKiranStockView *gStockView;
CRatnaKiranTreeView *gTreeView;
CMainFrame *gFrame;


int ShowErrorMessage(CString message, CList<CString, CString> *IDList, BOOL try_again)
{
	if(!IDList || IDList->IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return IDCANCEL;
	}

	CString final = "Could not " + message + " the following sets/items -\n";
	POSITION pos = IDList->GetHeadPosition();
	while(pos)
	{
		CString id = IDList->GetAt(pos);
		final = final + id + "\n";
		IDList->GetNext(pos);
	}

	if(try_again)
	{
		final = final + "\nDo you want to try again ?";
		int retval = 0;
		SHOW_ERROR_YESNO(final, retval);
		return retval;
	}
	else
	{
		SHOW_INTERNAL_ERROR_REASON(final);
		return IDOK;
	}
}


BOOL MakeGlobalsValid(BOOL check_selection)
{
	if(!gFrame)
	{
		gFrame = (CMainFrame *)AfxGetMainWnd();
		if(!gFrame)
			return FALSE;
	}

	if(!gDoc)
	{
		gDoc = (CRatnaKiranDoc *)gFrame->GetActiveDocument();
		if(!gDoc)
			return FALSE;
	}

	if(!gTreeView)
	{
		if(!gFrame->m_wndSplitter)
			return FALSE;

		gTreeView = STATIC_DOWNCAST(CRatnaKiranTreeView, (gFrame->m_wndSplitter)->GetPane(0,0));
		if(!gTreeView)
			return FALSE;
	}

	if(!gStockView)
	{
		if(!gFrame->m_wndSplitter)
			return FALSE;

		gStockView = STATIC_DOWNCAST(CRatnaKiranStockView, (gFrame->m_wndSplitter)->GetPane(0,1));
		if(!gStockView)
			return FALSE;
	}

	if(check_selection && !gDoc->m_selected_voucher)
		return FALSE;

	return TRUE;
}



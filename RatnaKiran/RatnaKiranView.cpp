#include "stdafx.h"
#include "RatnaKiran.h"
#include "MainGlobal.h"
#include "RatnaKiranView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRatnaKiranView, CView)

BEGIN_MESSAGE_MAP(CRatnaKiranView, CView)
	//{{AFX_MSG_MAP(CRatnaKiranView)
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CRatnaKiranView::CRatnaKiranView()
{
}

CRatnaKiranView::~CRatnaKiranView()
{
}

void CRatnaKiranView::OnDraw(CDC* pDC) 
{
	//Nothing to do	
}

#define CHANGE_VIEW_TIMER 12345

void CRatnaKiranView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	SetTimer(CHANGE_VIEW_TIMER,1,NULL);
}


void CRatnaKiranView::OnTimer(UINT nIDEvent) 
{
	CView::OnTimer(nIDEvent);

	if(nIDEvent == CHANGE_VIEW_TIMER)
	{
		CMainFrame *mainframe = (CMainFrame *)AfxGetMainWnd();
		if(mainframe != NULL)
		{
			KillTimer(CHANGE_VIEW_TIMER);
			if(!mainframe->ChangeView())
			{
				SHOW_INTERNAL_ERROR_REASON("Could not correctly setup views.");
				PostQuitMessage(1);
			}
		}
		else
			SHOW_INTERNAL_ERROR;
	}
}

#ifdef _DEBUG
void CRatnaKiranView::AssertValid() const
{
	CView::AssertValid();
}

void CRatnaKiranView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


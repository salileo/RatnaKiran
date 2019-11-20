#include "stdafx.h"
#include "Ratnakiran.h"
#include "RatnakiranDoc.h"
#include "RatnakiranView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRatnakiranView, CView)

BEGIN_MESSAGE_MAP(CRatnakiranView, CView)
	//{{AFX_MSG_MAP(CRatnakiranView)
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CRatnakiranView::CRatnakiranView()
{
}

CRatnakiranView::~CRatnakiranView()
{
}

void CRatnakiranView::OnDraw(CDC* pDC) 
{
	//Nothing to do	
}

#define CHANGE_VIEW_TIMER 12345

void CRatnakiranView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	SetTimer(CHANGE_VIEW_TIMER,1,NULL);
}


void CRatnakiranView::OnTimer(UINT nIDEvent) 
{
	CView::OnTimer(nIDEvent);

	if(nIDEvent == CHANGE_VIEW_TIMER)
	{
		CMainFrame *mainframe = (CMainFrame *)AfxGetMainWnd();
		if(mainframe != NULL)
		{
			KillTimer(CHANGE_VIEW_TIMER);
			if(!mainframe->ChangeView())
				PostQuitMessage(1);
		}
	}
}

#ifdef _DEBUG
void CRatnakiranView::AssertValid() const
{
	CView::AssertValid();
}

void CRatnakiranView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


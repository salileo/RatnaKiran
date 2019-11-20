#if !defined(AFX_RATNAKIRANVIEW_H__B71EEC62_577A_4DEA_842F_5AAB68D909FC__INCLUDED_)
#define AFX_RATNAKIRANVIEW_H__B71EEC62_577A_4DEA_842F_5AAB68D909FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRatnakiranView : public CView
{
protected:
	CRatnakiranView();
	DECLARE_DYNCREATE(CRatnakiranView)

public:
	//{{AFX_VIRTUAL(CRatnakiranView)
	protected:
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

public:
	virtual ~CRatnakiranView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CRatnakiranView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_RATNAKIRANVIEW_H__B71EEC62_577A_4DEA_842F_5AAB68D909FC__INCLUDED_)

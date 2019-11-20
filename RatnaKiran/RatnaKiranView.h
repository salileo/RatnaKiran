#if !defined(AFX_RATNAKIRANVIEW_H__B71EEC62_577A_4DEA_842F_5AAB68D909FC__INCLUDED_)
#define AFX_RATNAKIRANVIEW_H__B71EEC62_577A_4DEA_842F_5AAB68D909FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRatnaKiranView : public CView
{
protected:
	DECLARE_DYNCREATE(CRatnaKiranView)

public:
	CRatnaKiranView();
	virtual ~CRatnaKiranView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	//{{AFX_VIRTUAL(CRatnaKiranView)
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CRatnaKiranView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_RATNAKIRANVIEW_H__B71EEC62_577A_4DEA_842F_5AAB68D909FC__INCLUDED_)

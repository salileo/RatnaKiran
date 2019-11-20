#if !defined(AFX_MAINFRM_H__A2A6EA48_6FC0_4BB6_B18C_508C6872345D__INCLUDED_)
#define AFX_MAINFRM_H__A2A6EA48_6FC0_4BB6_B18C_508C6872345D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMainFrame : public CFrameWnd
{
	struct SSoldProductID
	{
		CString ID;
		CString voucher_name;
		CString country;
		BOOL set;
	};

	struct SSoldSerial
	{
		int serial;
		CString ID;
		CString voucher_name;
		CString country;
	};
	
protected:
	DECLARE_DYNCREATE(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CSplitterWnd  *m_wndSplitter;

	BOOL ChangeView();
	void SetStatusBarString(CString str);

	BOOL OpenForEdit(CString productID);
	BOOL PerformSetAddNew();
	BOOL PerformSetDeleteCurrent(BOOL force);
	BOOL PerformSetDeleteChecked(BOOL force);
	BOOL PerformSetDeleteHighlighted(BOOL force);
	BOOL PerformSetDeleteCheckedHighlighted(BOOL force);

private:
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	CStatusBar  m_wndStatusBar;
	//CToolBar    m_wndToolBar;

	BOOL DeleteSets(CList<CString, CString> *list, BOOL force);
	BOOL MoveSetsTo(CList<CString, CString> *pendinglist, CList<CString, CString> *successlist, CString voucher_name, DatabaseType type, BOOL createnew);

	void GenerateStockReport(int mode, ReportType type);
	void GenerateKundanReport(int mode, ReportType type);
	void GenerateExportReport(int mode, ReportType type);

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTreeCreatenewstockvoucher();
	afx_msg void OnTreeCreatenewexportvoucher();
	afx_msg void OnTreeCreatenewkundanvoucher();
	afx_msg void OnTreeDeleteselected();
	afx_msg void OnTreeIgnoreselected();
	afx_msg void OnTreeDeletesolditems();
	afx_msg void OnTreeQuerySetID();
	afx_msg void OnTreeQuerySetGrossWeight();
	afx_msg void OnTreeUpdateinformation();
	afx_msg void OnTreeReallocateserialnumbers();
	afx_msg void OnTreeMarkitemassoldOnproductid();
	afx_msg void OnTreeMarkitemassoldOnserialnumber();
	afx_msg void OnSetAddnew();
	afx_msg void OnSetDeleteCurrent();
	afx_msg void OnSetDeleteChecked();
	afx_msg void OnSetDeleteHighlighted();
	afx_msg void OnSetDeletecheckedHighlighted();
	afx_msg void OnSetEdit();
	afx_msg void OnSetMoveAddtoexisting();
	afx_msg void OnSetMoveCreatenew();
	afx_msg void OnSetSelectall();
	afx_msg void OnSetSelecthighlighted();
	afx_msg void OnSetUnselectall();
	afx_msg void OnSetUnselecthighlighted();
	afx_msg void OnSetChangeproductid();
	afx_msg void OnSetResetoldserials();
	afx_msg void OnReportsStock(UINT nID);
	afx_msg void OnReportsKundan(UINT nID);
	afx_msg void OnReportsExport(UINT nID);
	afx_msg void OnReportsCatalogue();
	afx_msg void OnSettingsUpdategoldrate();
	afx_msg void OnSettingsManagedescriptiondatabase();
	afx_msg void OnSettingsChangefilepath();
	afx_msg void OnSettingsBackup();
	afx_msg void OnUsingforexportsDollarusd();
	afx_msg void OnUsingforexportsNativecurrency();
	afx_msg void OnUserAdd();
	afx_msg void OnUserDelete();
	afx_msg void OnUserEditPerm();
	afx_msg void OnUserChangePassword();
	afx_msg void OnUpdateGeneral(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_MAINFRM_H__A2A6EA48_6FC0_4BB6_B18C_508C6872345D__INCLUDED_)

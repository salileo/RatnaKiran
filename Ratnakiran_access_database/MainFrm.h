#if !defined(AFX_MAINFRM_H__A2A6EA48_6FC0_4BB6_B18C_508C6872345D__INCLUDED_)
#define AFX_MAINFRM_H__A2A6EA48_6FC0_4BB6_B18C_508C6872345D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CStatusBar  m_wndStatusBar;
	//CToolBar    m_wndToolBar;
	CSplitterWnd  *m_wndSplitter;

	BOOL ChangeView();
	BOOL OpenForEdit(CString productID);
	BOOL DeleteSets(CList<CString, CString> *list, BOOL force);
	BOOL MoveSetsTo(CList<CString, CString> *pendinglist, CList<CString, CString> *successlist, CString dest_voucher_name, CString dest_voucher_type, BOOL newly_created);

	BOOL PerformSetDeleteCurrent(BOOL force);
	BOOL PerformSetDeleteChecked(BOOL force);
	BOOL PerformSetDeleteHighlighted(BOOL force);
	BOOL PerformSetDeleteCheckedHighlighted(BOOL force);

	void GenerateStockReport(int mode, Sold_Status type);
	void GenerateKundanReport(int mode, Sold_Status type);
	void GenerateExportReport(int mode, Sold_Status type);
public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMainUpdategoldrate();
	afx_msg void OnMainManagedescriptiondatabase();
	afx_msg void OnMainChangefilepath();
	afx_msg void OnMainExportsettings();
	afx_msg void OnMainImportsettings();
	afx_msg void OnUsingforexportsDollarusd();
	afx_msg void OnUsingforexportsNativecurrency();
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
	afx_msg void OnTreeCreatenewstockvoucher();
	afx_msg void OnTreeCreatenewexportvoucher();
	afx_msg void OnTreeCreatenewkundanvoucher();
	afx_msg void OnTreeDeleteselected();
	afx_msg void OnTreeDeletesolditems();
	afx_msg void OnTreeQuerySetID();
	afx_msg void OnTreeQuerySetGrossWeight();
	afx_msg void OnTreeUpdateinformation();
	afx_msg void OnTreeReallocateserialnumbers();
	afx_msg void OnTreeMarkitemassoldOnproductid();
	afx_msg void OnTreeMarkitemassoldOnserialnumber();
	afx_msg void OnReportsStockStocksreportActualsOriginal();
	afx_msg void OnReportsStockStocksreportActualsSold();
	afx_msg void OnReportsStockStocksreportActualsUnsold();
	afx_msg void OnReportsStockStocksreportMarkedOriginal();
	afx_msg void OnReportsStockStocksreportMarkedSold();
	afx_msg void OnReportsStockStocksreportMarkedUnsold();
	afx_msg void OnReportsStockPricequotation();
	afx_msg void OnReportsStockChecklist();
	afx_msg void OnReportsStockTags();
	afx_msg void OnReportsKundanStocksreportActualsOriginal();
	afx_msg void OnReportsKundanStocksreportActualsSold();
	afx_msg void OnReportsKundanStocksreportActualsUnsold();
	afx_msg void OnReportsKundanStocksreportMarkedOriginal();
	afx_msg void OnReportsKundanStocksreportMarkedSold();
	afx_msg void OnReportsKundanStocksreportMarkedUnsold();
	afx_msg void OnReportsKundanPricequotation();
	afx_msg void OnReportsKundanChecklist();
	afx_msg void OnReportsKundanTags();
	afx_msg void OnReportsExportPackagelistOriginal();
	afx_msg void OnReportsExportPackagelistSold();
	afx_msg void OnReportsExportPackagelistUnsold();
	afx_msg void OnReportsExportShortinvoiceOriginal();
	afx_msg void OnReportsExportShortinvoiceSold();
	afx_msg void OnReportsExportShortinvoiceUnsold();
	afx_msg void OnReportsExportPhotoinvoiceOriginal();
	afx_msg void OnReportsExportPhotoinvoiceSold();
	afx_msg void OnReportsExportPhotoinvoiceUnsold();
	afx_msg void OnReportsExportGrform();
	afx_msg void OnReportsExportPricequotation();
	afx_msg void OnReportsExportChecklist();
	afx_msg void OnReportsExportTags();
	afx_msg void OnReportsCatalogue();
	afx_msg void OnUpdateUsingforexportsDollarusd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUsingforexportsNativecurrency(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetAddnew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetDeleteCurrent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetDeleteChecked(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetDeleteHighlighted(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetDeletecheckedHighlighted(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetMoveCreatenew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetMoveAddtoexisting(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetSelectall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetSelecthighlighted(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetUnselectall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetUnselecthighlighted(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetChangeproductid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetResetoldserials(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTreeCreatenewstockvoucher(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTreeCreatenewexportvoucher(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTreeDeleteselected(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTreeDeletesolditems(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTreeQuerySet(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTreeUpdateinformation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTreeReallocateserialnumbers(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTreeMarkitemassoldOnproductid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTreeMarkitemassoldOnserialnumber(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsStock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsKundan(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsExport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsCatalogue(CCmdUI* pCmdUI);
	afx_msg void OnSettingsCreatebackup();
	afx_msg void OnSettingsRestorefrombackup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_MAINFRM_H__A2A6EA48_6FC0_4BB6_B18C_508C6872345D__INCLUDED_)

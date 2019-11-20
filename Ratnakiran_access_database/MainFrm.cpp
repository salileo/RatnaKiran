#include "stdafx.h"
#include "RatnaKiran.h"
#include "MainFrm.h"

#include "DetailsDialogExport.h"
#include "DetailsDialogKundan.h"
#include "DetailsDialogStock.h"
#include "GenericListBoxDialog.h"
#include "GenericOneEditDialog.h"
#include "GenericTwoEditDialog.h"
#include "ProgressDialog.h"
#include "AppendListDialog.h"
#include "ReportOptionDialog.h"
#include "ExportSerialsDialog.h"
#include "MoveOptionDialog.h"
#include "AddDialogExport.h"
#include "AddDialogStock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_UPDATEGOLDRATE, OnMainUpdategoldrate)
	ON_COMMAND(ID_MANAGEDESCRIPTIONDATABASE, OnMainManagedescriptiondatabase)
	ON_COMMAND(ID_USINGFOREXPORTS_DOLLARUSD, OnUsingforexportsDollarusd)
	ON_COMMAND(ID_USINGFOREXPORTS_NATIVECURRENCY, OnUsingforexportsNativecurrency)
	ON_COMMAND(ID_SET_ADDNEW, OnSetAddnew)
	ON_COMMAND(ID_SET_DELETE_CURRENT, OnSetDeleteCurrent)
	ON_COMMAND(ID_SET_DELETE_CHECKED, OnSetDeleteChecked)
	ON_COMMAND(ID_SET_DELETE_HIGHLIGHTED, OnSetDeleteHighlighted)
	ON_COMMAND(ID_SET_DELETECHECKED_HIGHLIGHTED, OnSetDeletecheckedHighlighted)
	ON_COMMAND(ID_SET_EDIT, OnSetEdit)
	ON_COMMAND(ID_SET_MOVE_ADDTOEXISTING, OnSetMoveAddtoexisting)
	ON_COMMAND(ID_SET_MOVE_CREATENEW, OnSetMoveCreatenew)
	ON_COMMAND(ID_SET_SELECTALL, OnSetSelectall)
	ON_COMMAND(ID_SET_SELECTHIGHLIGHTED, OnSetSelecthighlighted)
	ON_COMMAND(ID_SET_UNSELECTALL, OnSetUnselectall)
	ON_COMMAND(ID_SET_UNSELECTHIGHLIGHTED, OnSetUnselecthighlighted)
	ON_COMMAND(ID_SET_CHANGEPRODUCTID, OnSetChangeproductid)
	ON_COMMAND(ID_SET_RESETOLDSERIALS, OnSetResetoldserials)
	ON_COMMAND(ID_TREE_CREATENEWSTOCKVOUCHER, OnTreeCreatenewstockvoucher)
	ON_COMMAND(ID_TREE_CREATENEWEXPORTVOUCHER, OnTreeCreatenewexportvoucher)
	ON_COMMAND(ID_TREE_CREATENEWKUNDANVOUCHER, OnTreeCreatenewkundanvoucher)
	ON_COMMAND(ID_TREE_DELETESELECTED, OnTreeDeleteselected)
	ON_COMMAND(ID_TREE_DELETESOLDITEMS, OnTreeDeletesolditems)
	ON_COMMAND(ID_TREE_QUERYSET_ID, OnTreeQuerySetID)
	ON_COMMAND(ID_TREE_QUERYSET_GROSS_WEIGHT, OnTreeQuerySetGrossWeight)
	ON_COMMAND(ID_TREE_UPDATEINFORMATION, OnTreeUpdateinformation)
	ON_COMMAND(ID_TREE_REALLOCATESERIALNUMBERS, OnTreeReallocateserialnumbers)
	ON_COMMAND(ID_TREE_MARKITEMASSOLD_ONPRODUCTID, OnTreeMarkitemassoldOnproductid)
	ON_COMMAND(ID_TREE_MARKITEMASSOLD_ONSERIALNUMBER, OnTreeMarkitemassoldOnserialnumber)
	ON_COMMAND(ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_ORIGINAL, OnReportsStockStocksreportActualsOriginal)
	ON_COMMAND(ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_SOLD, OnReportsStockStocksreportActualsSold)
	ON_COMMAND(ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_UNSOLD, OnReportsStockStocksreportActualsUnsold)
	ON_COMMAND(ID_REPORTS_STOCK_STOCKSREPORT_MARKED_ORIGINAL, OnReportsStockStocksreportMarkedOriginal)
	ON_COMMAND(ID_REPORTS_STOCK_STOCKSREPORT_MARKED_SOLD, OnReportsStockStocksreportMarkedSold)
	ON_COMMAND(ID_REPORTS_STOCK_STOCKSREPORT_MARKED_UNSOLD, OnReportsStockStocksreportMarkedUnsold)
	ON_COMMAND(ID_REPORTS_STOCK_PRICEQUOTATION, OnReportsStockPricequotation)
	ON_COMMAND(ID_REPORTS_STOCK_CHECKLIST, OnReportsStockChecklist)
	ON_COMMAND(ID_REPORTS_STOCK_TAGS, OnReportsStockTags)
	ON_COMMAND(ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_ORIGINAL, OnReportsKundanStocksreportActualsOriginal)
	ON_COMMAND(ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_SOLD, OnReportsKundanStocksreportActualsSold)
	ON_COMMAND(ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_UNSOLD, OnReportsKundanStocksreportActualsUnsold)
	ON_COMMAND(ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_ORIGINAL, OnReportsKundanStocksreportMarkedOriginal)
	ON_COMMAND(ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_SOLD, OnReportsKundanStocksreportMarkedSold)
	ON_COMMAND(ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_UNSOLD, OnReportsKundanStocksreportMarkedUnsold)
	ON_COMMAND(ID_REPORTS_KUNDAN_PRICEQUOTATION, OnReportsKundanPricequotation)
	ON_COMMAND(ID_REPORTS_KUNDAN_CHECKLIST, OnReportsKundanChecklist)
	ON_COMMAND(ID_REPORTS_KUNDAN_TAGS, OnReportsKundanTags)
	ON_COMMAND(ID_REPORTS_EXPORT_PACKAGELIST_ORIGINAL, OnReportsExportPackagelistOriginal)
	ON_COMMAND(ID_REPORTS_EXPORT_PACKAGELIST_SOLD, OnReportsExportPackagelistSold)
	ON_COMMAND(ID_REPORTS_EXPORT_PACKAGELIST_UNSOLD, OnReportsExportPackagelistUnsold)
	ON_COMMAND(ID_REPORTS_EXPORT_SHORTINVOICE_ORIGINAL, OnReportsExportShortinvoiceOriginal)
	ON_COMMAND(ID_REPORTS_EXPORT_SHORTINVOICE_SOLD, OnReportsExportShortinvoiceSold)
	ON_COMMAND(ID_REPORTS_EXPORT_SHORTINVOICE_UNSOLD, OnReportsExportShortinvoiceUnsold)
	ON_COMMAND(ID_REPORTS_EXPORT_PHOTOINVOICE_ORIGINAL, OnReportsExportPhotoinvoiceOriginal)
	ON_COMMAND(ID_REPORTS_EXPORT_PHOTOINVOICE_SOLD, OnReportsExportPhotoinvoiceSold)
	ON_COMMAND(ID_REPORTS_EXPORT_PHOTOINVOICE_UNSOLD, OnReportsExportPhotoinvoiceUnsold)
	ON_COMMAND(ID_REPORTS_EXPORT_GRFORM, OnReportsExportGrform)
	ON_COMMAND(ID_REPORTS_EXPORT_PRICEQUOTATION, OnReportsExportPricequotation)
	ON_COMMAND(ID_REPORTS_EXPORT_CHECKLIST, OnReportsExportChecklist)
	ON_COMMAND(ID_REPORTS_EXPORT_TAGS, OnReportsExportTags)
	ON_COMMAND(ID_REPORTS_CATALOGUE, OnReportsCatalogue)
	ON_UPDATE_COMMAND_UI(ID_USINGFOREXPORTS_DOLLARUSD, OnUpdateUsingforexportsDollarusd)
	ON_UPDATE_COMMAND_UI(ID_USINGFOREXPORTS_NATIVECURRENCY, OnUpdateUsingforexportsNativecurrency)
	ON_UPDATE_COMMAND_UI(ID_SET_ADDNEW, OnUpdateSetAddnew)
	ON_UPDATE_COMMAND_UI(ID_SET_DELETE_CURRENT, OnUpdateSetDeleteCurrent)
	ON_UPDATE_COMMAND_UI(ID_SET_DELETE_CHECKED, OnUpdateSetDeleteChecked)
	ON_UPDATE_COMMAND_UI(ID_SET_DELETE_HIGHLIGHTED, OnUpdateSetDeleteHighlighted)
	ON_UPDATE_COMMAND_UI(ID_SET_DELETECHECKED_HIGHLIGHTED, OnUpdateSetDeletecheckedHighlighted)
	ON_UPDATE_COMMAND_UI(ID_SET_EDIT, OnUpdateSetEdit)
	ON_UPDATE_COMMAND_UI(ID_SET_MOVE_CREATENEW, OnUpdateSetMoveCreatenew)
	ON_UPDATE_COMMAND_UI(ID_SET_MOVE_ADDTOEXISTING, OnUpdateSetMoveAddtoexisting)
	ON_UPDATE_COMMAND_UI(ID_SET_SELECTALL, OnUpdateSetSelectall)
	ON_UPDATE_COMMAND_UI(ID_SET_SELECTHIGHLIGHTED, OnUpdateSetSelecthighlighted)
	ON_UPDATE_COMMAND_UI(ID_SET_UNSELECTALL, OnUpdateSetUnselectall)
	ON_UPDATE_COMMAND_UI(ID_SET_UNSELECTHIGHLIGHTED, OnUpdateSetUnselecthighlighted)
	ON_UPDATE_COMMAND_UI(ID_SET_CHANGEPRODUCTID, OnUpdateSetChangeproductid)
	ON_UPDATE_COMMAND_UI(ID_SET_RESETOLDSERIALS, OnUpdateSetResetoldserials)
	ON_UPDATE_COMMAND_UI(ID_TREE_CREATENEWSTOCKVOUCHER, OnUpdateTreeCreatenewstockvoucher)
	ON_UPDATE_COMMAND_UI(ID_TREE_CREATENEWEXPORTVOUCHER, OnUpdateTreeCreatenewexportvoucher)
	ON_UPDATE_COMMAND_UI(ID_TREE_DELETESELECTED, OnUpdateTreeDeleteselected)
	ON_UPDATE_COMMAND_UI(ID_TREE_DELETESOLDITEMS, OnUpdateTreeDeletesolditems)
	ON_UPDATE_COMMAND_UI(ID_TREE_QUERYSET_ID, OnUpdateTreeQuerySet)
	ON_UPDATE_COMMAND_UI(ID_TREE_UPDATEINFORMATION, OnUpdateTreeUpdateinformation)
	ON_UPDATE_COMMAND_UI(ID_TREE_REALLOCATESERIALNUMBERS, OnUpdateTreeReallocateserialnumbers)
	ON_UPDATE_COMMAND_UI(ID_TREE_MARKITEMASSOLD_ONPRODUCTID, OnUpdateTreeMarkitemassoldOnproductid)
	ON_UPDATE_COMMAND_UI(ID_TREE_MARKITEMASSOLD_ONSERIALNUMBER, OnUpdateTreeMarkitemassoldOnserialnumber)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_ORIGINAL, OnUpdateReportsStock)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_ORIGINAL, OnUpdateReportsKundan)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_PACKAGELIST_ORIGINAL, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_CATALOGUE, OnUpdateReportsCatalogue)
	ON_UPDATE_COMMAND_UI(ID_TREE_QUERYSET_GROSS_WEIGHT, OnUpdateTreeQuerySet)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_SOLD, OnUpdateReportsStock)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_UNSOLD, OnUpdateReportsStock)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_STOCK_STOCKSREPORT_MARKED_ORIGINAL, OnUpdateReportsStock)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_STOCK_STOCKSREPORT_MARKED_SOLD, OnUpdateReportsStock)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_STOCK_STOCKSREPORT_MARKED_UNSOLD, OnUpdateReportsStock)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_STOCK_PRICEQUOTATION, OnUpdateReportsStock)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_STOCK_CHECKLIST, OnUpdateReportsStock)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_STOCK_TAGS, OnUpdateReportsStock)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_SOLD, OnUpdateReportsKundan)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_UNSOLD, OnUpdateReportsKundan)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_ORIGINAL, OnUpdateReportsKundan)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_SOLD, OnUpdateReportsKundan)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_UNSOLD, OnUpdateReportsKundan)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_KUNDAN_PRICEQUOTATION, OnUpdateReportsKundan)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_KUNDAN_CHECKLIST, OnUpdateReportsKundan)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_KUNDAN_TAGS, OnUpdateReportsKundan)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_PACKAGELIST_SOLD, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_PACKAGELIST_UNSOLD, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_SHORTINVOICE_ORIGINAL, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_SHORTINVOICE_SOLD, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_SHORTINVOICE_UNSOLD, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_PHOTOINVOICE_ORIGINAL, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_PHOTOINVOICE_SOLD, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_PHOTOINVOICE_UNSOLD, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_GRFORM, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_PRICEQUOTATION, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_CHECKLIST, OnUpdateReportsExport)
	ON_UPDATE_COMMAND_UI(ID_REPORTS_EXPORT_TAGS, OnUpdateReportsExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CMainFrame::CMainFrame()
{
	m_wndSplitter = NULL;
}

CMainFrame::~CMainFrame()
{
	if(m_wndSplitter)
		delete m_wndSplitter;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		::MessageBox(NULL, "CMainFrame::OnCreate - Could not create the main window.", "ERROR", MB_OK);
		return -1;
	}
	
	/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		::MessageBox(NULL, "CMainFrame::OnCreate - Could not create the toolbar.", "ERROR", MB_OK);
		return -1;
	}
	*/

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		::MessageBox(NULL, "CMainFrame::OnCreate - Could not create the status bar.", "ERROR", MB_OK);
		return -1;
	}

	/*
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	*/

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
	{
		::MessageBox(NULL, "CMainFrame::PreCreateWindow - Could not create the window.", "ERROR", MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::ChangeView()
{
	if(m_wndSplitter)
	{
		//Somebody called this by mistake
		return FALSE;
	}

	CRect rect;
	SIZE sz1, sz2;

	GetClientRect(&rect);

	sz1.cx = 200;
	sz1.cy = rect.Height();
	sz2.cx = rect.Width() - sz1.cx;
	sz2.cy = rect.Height();

	CView *removedview = GetActiveView();
	if(removedview == NULL)
	{
		MessageBox("CMainFrame::ChangeView - no view to be removed.", "ERROR");
		return FALSE;
	}

	SALIL_NEW(m_wndSplitter, CSplitterWnd);

	if(!m_wndSplitter->CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE))
	{
		MessageBox("CMainFrame::ChangeView - splitter view creation error.", "ERROR");
		delete m_wndSplitter;
		m_wndSplitter = NULL;
		return FALSE;
	}

	CCreateContext context;
	context.m_pCurrentFrame = this;
	context.m_pCurrentDoc = GetActiveDocument();

	if(!(m_wndSplitter->CreateView(0, 0, RUNTIME_CLASS(CRatnakiranTreeView), sz1, &context)))
	{
		MessageBox("CMainFrame::ChangeView - could not create left panel.", "ERROR");
		delete m_wndSplitter;
		m_wndSplitter = NULL;
		return FALSE;
	}

	if(!(m_wndSplitter->CreateView(0, 1, RUNTIME_CLASS(CRatnakiranStockView), sz2, &context)))
	{
		MessageBox("CMainFrame::ChangeView - could not create right panel.", "ERROR");
		delete m_wndSplitter;
		m_wndSplitter = NULL;
		return FALSE;
	}
	
	CView *addedview1, *addedview2;
	addedview1 = STATIC_DOWNCAST(CView, m_wndSplitter->GetPane(0, 0));
	addedview2 = STATIC_DOWNCAST(CView, m_wndSplitter->GetPane(0, 1));
	if(addedview1 == NULL || addedview2 == NULL)
	{
		MessageBox("CMainFrame::ChangeView - could not get pointers to new views.", "ERROR");
		delete m_wndSplitter;
		m_wndSplitter = NULL;
		return FALSE;
	}

	if(!removedview->DestroyWindow())
	{
		MessageBox("CMainFrame::ChangeView - could not destroy old view.", "ERROR");
		delete m_wndSplitter;
		m_wndSplitter = NULL;
		return FALSE;
	}
	
	MakeGlobalsValid();

	addedview2->ShowWindow(SW_SHOW);
	addedview2->OnInitialUpdate();

	addedview1->ShowWindow(SW_SHOW);
	addedview1->OnInitialUpdate();

	SetActiveView(addedview1);
	RecalcLayout();

	SetWindowText("Stock Management System - RatnaKiran");
	return TRUE;
}


#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


void CMainFrame::OnMainUpdategoldrate() 
{
	if(!MakeGlobalsValid())
		return;

	CGenericTwoEditDialog<double, double> dlg(
		gSettings.m_Gold_rate, 0.0, 0.0, "Old gold rate", FALSE, "",
		0.0, 0.0, 0.0, "New gold rate", FALSE, "", "Gold Rate", NO_READONLY);

	if(dlg.DoModal() == IDCANCEL)
		return;

	if(!gSettings.EditEntry())
		return;

	gSettings.m_Gold_rate = dlg.m_val_2;

	if(!gSettings.UpdateEntry("OnMainUpdategoldrate"))
		return;
}


void CMainFrame::OnMainManagedescriptiondatabase() 
{
	CGenericListBoxDialog dlg(DESCRIPTION_MODIFY_DIALOG);
	dlg.DoModal();
}



void CMainFrame::OnUsingforexportsDollarusd() 
{
	if(!MakeGlobalsValid())
		return;

	if(!gSettings.EditEntry())
		return;

	gSettings.m_Use_native_currency = FALSE;

	if(!gSettings.UpdateEntry("OnUsingforexportsdollarusd"))
		return;
}


void CMainFrame::OnUsingforexportsNativecurrency() 
{
	if(!MakeGlobalsValid())
		return;

	if(!gSettings.EditEntry())
		return;

	gSettings.m_Use_native_currency = TRUE;

	if(!gSettings.UpdateEntry("OnUsingforexportsnativecurrency"))
		return;
}

void CMainFrame::OnSetAddnew() 
{
	if(!MakeGlobalsValid())
		return;

	if(gDoc->m_vouchers.m_record.m_Type != STOCK_TYPE)
	{
		MessageBox("You can only add a set to a stock voucher.", "ERROR");
		return;
	}

	BOOL root_voucher = FALSE;
	if(gDoc->m_vouchers.IsRootVoucher())
		root_voucher = TRUE;

	while(TRUE)
	{
		CString voucher;

		if(root_voucher)
		{
			CAppendListDialog dlg(STOCK_TYPE);

			if(dlg.DoModal() == IDCANCEL)
				break;

			voucher = dlg.m_selection;
		}
		else
			voucher = gDoc->m_vouchers.m_record.m_Voucher_ID;

		if(gDoc->m_vouchers.GotoVoucher(voucher) != DBOK)
			break;

		if(!gDoc->m_vouchers.DBBeginTransaction(TRUE))
			break;

		CRecordSet set;
		set.m_Set_ID = GetProductID();
		set.m_Voucher_ID = voucher;

		if(!gDoc->m_vouchers.AddSet(set))
		{
			CList<CString, CString> list;
			list.AddHead(set.m_Set_ID);
			ShowErrorMessage("add", &list, FALSE);
			
			gDoc->m_vouchers.DBRollbackTransaction(TRUE);
			continue;
		}

		if(!gDoc->m_vouchers.m_sets.GotoSet(set.m_Set_ID))
			break;

		CRecordItem item;
		item.m_Item_ID = set.m_Set_ID;
		item.m_Set_ID = set.m_Set_ID;

		if(!gDoc->m_vouchers.m_sets.AddItem(item, TRUE))
		{
			CList<CString, CString> list;
			list.AddHead(set.m_Set_ID);
			ShowErrorMessage("add", &list, FALSE);
			
			gDoc->m_vouchers.DBRollbackTransaction(TRUE);
			continue;
		}

		CAddDialogStock dlg;
		if(dlg.DoModal() == IDCANCEL)
		{
			gDoc->m_vouchers.DBRollbackTransaction(TRUE);
			break;
		}
		else
		{
			gDoc->m_vouchers.DBCommitTransaction(TRUE);
		}

		gStockView->RefreshEntries(FALSE);
	}

	if(root_voucher)
		gDoc->m_vouchers.GotoVoucher(STOCK_MAIN);
}


void CMainFrame::OnSetDeleteCurrent() 
{
	PerformSetDeleteCurrent(FALSE);
}


BOOL CMainFrame::PerformSetDeleteCurrent(BOOL force)
{
	if(!MakeGlobalsValid())
		return FALSE;

	BEGIN_WAIT;

	CList<CString, CString> list;
	CString productID;
	int index;
	productID = gStockView->GetSelectedProduct(&index);
	if(index < 0)
	{
		MessageBox("Please select a set to delete.", "ERROR");
		return FALSE;
	}
	
	list.AddHead(productID);
	if(!DeleteSets(&list, force))
	{
		END_WAIT;
		ShowErrorMessage("delete", &list, FALSE);
		return FALSE;
	}

	END_WAIT;
	return TRUE;
}


void CMainFrame::OnSetDeleteChecked()
{
	PerformSetDeleteChecked(FALSE);
}


BOOL CMainFrame::PerformSetDeleteChecked(BOOL force) 
{
	if(!MakeGlobalsValid())
		return FALSE;

	BEGIN_WAIT;

	CList<CString, CString> list;
	gStockView->GetCheckedList(&list);
	if(!DeleteSets(&list, force))
	{
		END_WAIT;
		ShowErrorMessage("delete", &list, FALSE);
		return FALSE;
	}

	END_WAIT;
	return TRUE;
}


void CMainFrame::OnSetDeleteHighlighted() 
{
	PerformSetDeleteHighlighted(FALSE);
}


BOOL CMainFrame::PerformSetDeleteHighlighted(BOOL force) 
{
	if(!MakeGlobalsValid())
		return FALSE;

	BEGIN_WAIT;

	CList<CString, CString> list;
	gStockView->GetHighlightedList(&list);
	if(!DeleteSets(&list, force))
	{
		END_WAIT;
		ShowErrorMessage("delete", &list, FALSE);
		return FALSE;
	}

	END_WAIT;
	return TRUE;
}


void CMainFrame::OnSetDeletecheckedHighlighted() 
{
	PerformSetDeleteCheckedHighlighted(FALSE);
}


BOOL CMainFrame::PerformSetDeleteCheckedHighlighted(BOOL force) 
{
	if(!MakeGlobalsValid())
		return FALSE;

	BEGIN_WAIT;

	CList<CString, CString> list;
	gStockView->GetCheckedList(&list);
	gStockView->GetHighlightedList(&list);
	if(!DeleteSets(&list, force))
	{
		END_WAIT;
		ShowErrorMessage("delete", &list, FALSE);
		return FALSE;
	}

	END_WAIT;

	return TRUE;
}

void CMainFrame::OnSetEdit() 
{
	if(!MakeGlobalsValid())
		return;

	CString productID;
	int index;
	productID = gStockView->GetSelectedProduct(&index);
	if(index < 0)
	{
		MessageBox("Please select a set for editing.", "ERROR");
		return;
	}

	if(!OpenForEdit(productID))
	{
		CList<CString, CString> list;
		list.AddHead(productID);
		ShowErrorMessage("edit", &list, FALSE);
		return;
	}
}


void CMainFrame::OnSetMoveAddtoexisting() 
{
	if(!MakeGlobalsValid())
		return;

	CList<CString, CString> pendingMoveList;
	gStockView->GetCheckedList(&pendingMoveList);
	if(pendingMoveList.IsEmpty())
		return;

	CMoveOptionDialog typedlg;
	if(typedlg.DoModal() == IDCANCEL)
		return;

	CString dest_type = typedlg.m_type;
	CAppendListDialog dlg(dest_type);
	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CString voucher_name = gDoc->m_vouchers.m_record.m_Voucher_ID;
		if(dlg.m_selection.CompareNoCase(voucher_name) == 0)
		{
			MessageBox("You cannot move/copy to the same folder.", "ERROR");
			continue;
		}
		else
		{
			break;
		}
	}

	BEGIN_WAIT;

	CList<CString, CString> pendingDeleteList;
	if(!gFrame->MoveSetsTo(&pendingMoveList, &pendingDeleteList, dlg.m_selection, dest_type, FALSE))
	{
		END_WAIT;
		ShowErrorMessage("move", &pendingMoveList, FALSE);
		return;
	}

	if(!pendingMoveList.IsEmpty())
	{
		END_WAIT;
		ShowErrorMessage("move", &pendingMoveList, FALSE);
		BEGIN_WAIT;
	}

	if(!DeleteSets(&pendingDeleteList, TRUE))
	{
		END_WAIT;
		ShowErrorMessage("delete", &pendingDeleteList, FALSE);
		return;
	}

	if(!pendingDeleteList.IsEmpty())
	{
		END_WAIT;
		ShowErrorMessage("delete", &pendingDeleteList, FALSE);
		BEGIN_WAIT;
	}

	if(gDoc->m_vouchers.IsRootVoucher() &&
	   (dest_type == gDoc->m_vouchers.m_record.m_Type))
	{
		gDoc->m_vouchers.GotoVoucher(gDoc->m_vouchers.m_record.m_Voucher_ID);
	}

	END_WAIT;
}


void CMainFrame::OnSetMoveCreatenew() 
{
	if(!MakeGlobalsValid())
		return;
	
	CList<CString, CString> pendingMoveList;
	gStockView->GetCheckedList(&pendingMoveList);
	if(pendingMoveList.IsEmpty())
		return;

	CMoveOptionDialog typedlg;
	if(typedlg.DoModal() == IDCANCEL)
		return;

	CString dest_type = typedlg.m_type;
	CGenericOneEditDialog<CString> dlg("", "", "", "New Voucher Name", FALSE, "", "Create New Voucher");
	if(dlg.DoModal() == IDCANCEL)
		return;

	if(!gDoc->CreateNewVoucher(dest_type, dlg.m_val))
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	BEGIN_WAIT;

	CList<CString, CString> pendingDeleteList;
	if(!gFrame->MoveSetsTo(&pendingMoveList, &pendingDeleteList, dlg.m_val, dest_type, TRUE))
	{
		END_WAIT;
		ShowErrorMessage("move", &pendingMoveList, FALSE);
		return;
	}

	if(!pendingMoveList.IsEmpty())
	{
		END_WAIT;
		ShowErrorMessage("move", &pendingMoveList, FALSE);
		BEGIN_WAIT;
	}

	if(!DeleteSets(&pendingDeleteList, TRUE))
	{
		END_WAIT;
		ShowErrorMessage("delete", &pendingDeleteList, FALSE);
		return;
	}

	if(!pendingDeleteList.IsEmpty())
	{
		END_WAIT;
		ShowErrorMessage("delete", &pendingDeleteList, FALSE);
		BEGIN_WAIT;
	}

	END_WAIT;

	gTreeView->OnInitialUpdate();
}


void CMainFrame::OnSetSelectall() 
{
	if(!gStockView)
		return;

	gStockView->SetCheckedList(NULL, ON);
}


void CMainFrame::OnSetSelecthighlighted() 
{
	if(!gStockView)
		return;

	CList<CString, CString> list;
	gStockView->GetHighlightedList(&list);
	gStockView->SetCheckedList(&list, ON);
}


void CMainFrame::OnSetUnselectall() 
{
	if(!gStockView)
		return;

	gStockView->SetCheckedList(NULL, OFF);
}


void CMainFrame::OnSetUnselecthighlighted() 
{
	if(!gStockView)
		return;

	CList<CString, CString> list;
	gStockView->GetHighlightedList(&list);
	gStockView->SetCheckedList(&list, OFF);
}


void CMainFrame::OnSetChangeproductid() 
{
	if(!MakeGlobalsValid())
		return;

	CString productID;
	int index;
	productID = gStockView->GetSelectedProduct(&index);
	if(index < 0)
	{
		MessageBox("Please select a set for changing the productID.", "ERROR");
		return;
	}

	CTableSet *set = &(gDoc->m_vouchers.m_sets);
	if(set->GotoSet(productID) != DBOK)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	//TODO 
	if(!set->UpdateProductID(set->m_record.m_Set_ID, FALSE))
		return;

	gStockView->RefreshEntries(FALSE);
}


void CMainFrame::OnSetResetoldserials() 
{
	if(!MakeGlobalsValid())
		return;

	CString productID;
	int index;
	productID = gStockView->GetSelectedProduct(&index);
	if(index < 0)
	{
		MessageBox("Please select a set for resetting the serial numbers.", "ERROR");
		return;
	}

	CTableSet *set = &(gDoc->m_vouchers.m_sets);
	if(set->GotoSet(productID) != DBOK)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	if(!set->ResetOldSerialNumbers())
	{
		CList<CString, CString> list;
		list.AddHead(productID);
		ShowErrorMessage("reset old serial numbers of", &list, FALSE);
		return;
	}
}


void CMainFrame::OnTreeCreatenewstockvoucher() 
{
	if(!MakeGlobalsValid())
		return;
	
	CGenericOneEditDialog<CString> dlg("", "", "", "New Voucher Name", FALSE, "", "Create New Voucher");
	if(dlg.DoModal() == IDCANCEL)
		return;

	CString vouchername = dlg.m_val;

	if(!gDoc->CreateNewVoucher(STOCK_TYPE, vouchername))
	{
		CString errstr = "Could not create the new stock voucher \'" + vouchername + "\'.";
		MessageBox(errstr, "ERROR");
		return;
	}

	gTreeView->OnInitialUpdate();
}


void CMainFrame::OnTreeCreatenewexportvoucher() 
{
	if(!MakeGlobalsValid())
		return;
	
	CGenericOneEditDialog<CString> dlg("", "", "", "New Voucher Name", FALSE, "", "Create New Voucher");
	if(dlg.DoModal() == IDCANCEL)
		return;

	CString vouchername = dlg.m_val;

	if(!gDoc->CreateNewVoucher(EXPORT_TYPE, vouchername))
	{
		CString errstr = "Could not create the new stock voucher \'" + vouchername + "\'.";
		MessageBox(errstr, "ERROR");
		return;
	}

	if(gDoc->m_vouchers.GotoVoucher(vouchername) != DBOK)
		return;

	CDetailsDialogExport detail_dlg;
	detail_dlg.DoModal();

	gTreeView->OnInitialUpdate();
}


void CMainFrame::OnTreeCreatenewkundanvoucher() 
{
	if(!MakeGlobalsValid())
		return;
	
	CGenericOneEditDialog<CString> dlg("", "", "", "New Voucher Name", FALSE, "", "Create New Voucher");
	if(dlg.DoModal() == IDCANCEL)
		return;

	CString vouchername = dlg.m_val;

	if(!gDoc->CreateNewVoucher(KUNDAN_TYPE, vouchername))
	{
		CString errstr = "Could not create the new stock voucher \'" + vouchername + "\'.";
		MessageBox(errstr, "ERROR");
		return;
	}

	gTreeView->OnInitialUpdate();
}


void CMainFrame::OnTreeDeleteselected() 
{
	if(!MakeGlobalsValid())
		return;

	if(gDoc->m_vouchers.IsRootVoucher())
	{
		MessageBox("You cannot delete root folders.", "ERROR");
		return;
	}

	CString name = gDoc->m_vouchers.m_record.m_Voucher_ID;
	CString warn_msg = "Are you sure you want to delete \"" + name + "\" ?";
	int retval = MessageBox(warn_msg, "Warning", MB_YESNO | MB_ICONWARNING);
	if(retval == IDNO)
		return;

	gDoc->DeleteVoucher(name);
	gTreeView->OnInitialUpdate();
}


void CMainFrame::OnTreeDeletesolditems()
{
	if(!MakeGlobalsValid())
		return;

	BEGIN_WAIT;
	
	CTableSet *set = &(gDoc->m_vouchers.m_sets);
	DatabaseState state = set->GotoFirstSet();

	if(state == DBInvalid)
		return;

	while(state == DBOK)
	{
		set->DeleteSoldItems();
		state = set->GotoNextSet();
	}

	gDoc->m_vouchers.DeleteEmptySets();
	gStockView->RefreshEntries(FALSE);
	END_WAIT;
}


void CMainFrame::OnTreeQuerySetID() 
{
#if 0
	if(!MakeGlobalsValid())
	{
		MessageBox("CMainFrame::OnTreeQuerySet - No defaults.", "ERROR");
		return;
	}

	if(!gDatabase)
	{
		MessageBox("CMainFrame::OnTreeQuerySet - No database.", "ERROR");
		return;
	}

	CGenericOneEditDialog dlg(PRODUCTID_OF_ITEM_TO_QUERY);

	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CList<SProductDetails, SProductDetails> productlist;
		CString productID = dlg.m_string;
		dlg.m_string = "";
		BOOL actualID = FALSE;

		if(productID.CompareNoCase(SProductItemNode::ExtractActualID(productID)) == 0)
			actualID = TRUE;

		if(actualID)
		{
			CList<CString, CString> setlist;
			gDatabase->GetSetList(productID, &setlist);

			while(!setlist.IsEmpty())
			{
				CString set = setlist.RemoveHead();
				SProductDetails details;
				details.m_productID = set;

				CString filename;
				if(gDatabase->GetDatabaseNameForSet(details.m_productID, &filename) &&
				   !filename.IsEmpty())
				{
					details.m_databaseName = CRatnaKiranDoc::FileToTitle(filename);
				}

				productlist.AddTail(details);
			}
		}
		else
		{
			SProductSetNode *set = gDatabase->GetSet(productID);
			if(set)
			{
				SProductDetails details;
				details.m_productID = set->m_productID;

				if(!(set->m_filename).IsEmpty())
					details.m_databaseName = CRatnaKiranDoc::FileToTitle(set->m_filename);

				productlist.AddTail(details);
			}
		}

		if(productlist.GetCount() >= 2)
		{
			CGenericListBoxDialog querydlg(PRODUCT_LIST_DIALOG);
			querydlg.m_itemlist = &productlist;
			querydlg.DoModal();
		}

		if(productlist.GetCount() == 0)
		{
			MessageBox("No product chosen to display.");
			continue;
		}

		while(!productlist.IsEmpty())
		{
			SProductDetails details = productlist.RemoveHead();
			if(!OpenForEdit(details.m_productID))
			{
				CList<CString, CString> list;
				list.AddHead(details.m_productID);
				ShowErrorMessage("edit", &list, FALSE);
				continue;
			}
		}

		break;
	}
#endif
}


void CMainFrame::OnTreeQuerySetGrossWeight() 
{
#if 0
	if(!MakeGlobalsValid())
	{
		MessageBox("CMainFrame::OnTreeQuerySet - No defaults.", "ERROR");
		return;
	}

	if(!gDatabase)
	{
		MessageBox("CMainFrame::OnTreeQuerySet - No database.", "ERROR");
		return;
	}

	CGenericOneEditDialog dlg(GROSSWEIGHT_OF_ITEM_TO_QUERY);

	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CList<SProductDetails, SProductDetails> productlist;
		double weight = dlg.m_double;
		dlg.m_double = 0.0;

		int count = gDatabase->GetSetCount();
		while(count > 0)
		{
			SProductSetNode *set = gDatabase->GetSetAt(count - 1);
			if(set)
			{
				int itemcount = set->GetItemCount();
				while(itemcount > 0)
				{
					SProductItemNode *item = set->GetItemAt(itemcount - 1);
					if(item)
					{
						if(weight == item->m_gross_weight)
						{
							SProductDetails details;
							details.m_productID = set->m_productID;

							if(!(set->m_filename).IsEmpty())
								details.m_databaseName = CRatnaKiranDoc::FileToTitle(set->m_filename);

							productlist.AddTail(details);
						}
					}

					itemcount--;
				}
			}

			count--;
		}

		if(productlist.GetCount() >= 2)
		{
			CGenericListBoxDialog querydlg(PRODUCT_LIST_DIALOG);
			querydlg.m_itemlist = &productlist;
			querydlg.DoModal();
		}

		if(productlist.GetCount() == 0)
		{
			MessageBox("No product chosen to display.");
			continue;
		}

		while(!productlist.IsEmpty())
		{
			SProductDetails details = productlist.RemoveHead();
			if(!OpenForEdit(details.m_productID))
			{
				CList<CString, CString> list;
				list.AddHead(details.m_productID);
				ShowErrorMessage("edit", &list, FALSE);
				continue;
			}
		}

		break;
	}
#endif
}


void CMainFrame::OnTreeUpdateinformation() 
{
	if(gDoc->m_vouchers.IsRootVoucher())
	{
		MessageBox("You cannot change information of the root folder.", "ERROR");
		return;
	}

	if(gDoc->m_vouchers.m_record.m_Type == STOCK_TYPE)
	{
		CDetailsDialogStock dlg;
		if(dlg.DoModal() == IDCANCEL)
			return;
	}
	else if(gDoc->m_vouchers.m_record.m_Type == EXPORT_TYPE)
	{
		CDetailsDialogExport dlg;
		if(dlg.DoModal() == IDCANCEL)
			return;
	}
	else if(gDoc->m_vouchers.m_record.m_Type == KUNDAN_TYPE)
	{
		CDetailsDialogKundan dlg;
		if(dlg.DoModal() == IDCANCEL)
			return;
	}
}


void CMainFrame::OnTreeReallocateserialnumbers() 
{
	if(!MakeGlobalsValid())
		return;

	if(gDoc->m_vouchers.m_record.m_Type != EXPORT_TYPE)
	{
		MessageBox("Cannot allocate serail numbers to stock vouchers", "ERROR");
		return;
	}

	if(gDoc->m_vouchers.IsRootVoucher())
	{
		MessageBox("You cannot perform this operation on the root folder.", "ERROR");
		return;
	}

	CGenericOneEditDialog<int> dlg(1, 1, 1, "Starting serial number", FALSE, "", "Serial Number");
	if(dlg.DoModal() == IDCANCEL)
		return;

	CString msg = "Are you sure you want to reset the serial numbers of \"" + gDoc->m_vouchers.m_record.m_Voucher_ID + "\" ?";
	if(MessageBox(msg, NULL, MB_YESNO) == IDNO)
		return;

	BEGIN_WAIT;

	int serial = dlg.m_val;

	if(!gDoc->m_vouchers.AssignSerialNumbers(&serial))
	{
		END_WAIT;
		MessageBox("Could not assign serial numbers properly. Please try again.", "ERROR");
		return;
	}

	gStockView->RefreshEntries(FALSE);
	END_WAIT;
}


void CMainFrame::OnTreeMarkitemassoldOnproductid() 
{
	if(!MakeGlobalsValid())
		return;

	CString country;
	CString country_validation;
	int dlgprop;

	if(gDoc->m_vouchers.m_record.m_Type == STOCK_TYPE)
	{
		country = "India";
		dlgprop = SECOND_READONLY;
	}
	else if(gDoc->m_vouchers.m_record.m_Type == KUNDAN_TYPE)
	{
		country = "India";
		dlgprop = SECOND_READONLY;
	}
	else if(gDoc->m_vouchers.m_record.m_Type == EXPORT_TYPE)
	{
		country_validation = "Please entry a country name.";
		dlgprop = NO_READONLY;
	}

	CGenericTwoEditDialog<CString, CString> dlg("", "", "", "Sold ProductID", FALSE, "Please enter an ID.", 
		country, "", "", "Sold in country", FALSE, country_validation, 
		"Mark Sold ProductID", dlgprop);
	
	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CList<CProductIDList, CProductIDList> productlist;
		BOOL asSet = FALSE;
		BOOL actualID = FALSE;
		CString productID = dlg.m_val_1;
		CString countryname = dlg.m_val_2;
		(dlg.m_val_1).Empty();

		if(productID.CompareNoCase(CTableDescription::ExtractActualID(productID)) == 0)
			actualID = TRUE;

		if(actualID)
		{
			CList<CString, CString> setlist;
			CList<CString, CString> voucherlist;

			gDoc->m_vouchers.GetSetList(productID, &setlist, &voucherlist);

			while(!setlist.IsEmpty() && !voucherlist.IsEmpty())
			{
				CProductIDList details;
				details.m_productID = setlist.RemoveHead();
				details.m_voucherName = voucherlist.RemoveHead();

				productlist.AddTail(details);
			}

			asSet = TRUE;
		}
		else
		{
			if(gDoc->m_vouchers.m_sets.GotoSet(productID) == DBOK)
			{
				asSet = TRUE;

				CProductIDList details;
				details.m_productID = gDoc->m_vouchers.m_sets.m_record.m_Set_ID;
				details.m_voucherName = gDoc->m_vouchers.m_sets.m_record.m_Voucher_ID;
				productlist.AddTail(details);
			}
			else
			{
				DatabaseState state = gDoc->m_vouchers.m_sets.GotoFirstSet();

				while(state == DBOK)
				{
					if(gDoc->m_vouchers.m_sets.m_items.GotoItem(productID) == DBOK)
						break;

					state = gDoc->m_vouchers.m_sets.GotoNextSet();
				}

				if(!gDoc->m_vouchers.m_sets.IsEOF() && (gDoc->m_vouchers.m_sets.m_items.m_record.m_Item_ID == productID))
				{
					asSet = FALSE;

					CProductIDList details;
					details.m_productID = gDoc->m_vouchers.m_sets.m_items.m_record.m_Item_ID;
					details.m_voucherName = gDoc->m_vouchers.m_sets.m_record.m_Voucher_ID;
					productlist.AddTail(details);
				}
			}
		}

		if(productlist.GetCount() >= 2)
		{
			CGenericListBoxDialog solddlg(PRODUCT_LIST_DIALOG);
			solddlg.m_item_list = &productlist;
			solddlg.DoModal();
		}

		if(productlist.GetCount() == 0)
		{
			MessageBox("No product chosen to mark as sold.");
			continue;
		}

		BOOL err = FALSE;
		CList<CString, CString> errlist;
		while(!productlist.IsEmpty())
		{
			CProductIDList details = productlist.RemoveHead();
			if(asSet)
			{
				if(gDoc->m_vouchers.m_sets.GotoSet(details.m_productID) != DBOK)
				{
					errlist.AddTail(details.m_productID);
					err = TRUE;
				}

				if(!gDoc->m_vouchers.m_sets.MarkSetAsSold(TRUE, countryname))
				{
					errlist.AddTail(details.m_productID);
					err = TRUE;
				}
			}
			else
			{
				//there should be only one entry in the list, and it should be of the
				//currently selected item.

				if(!gDoc->m_vouchers.m_sets.m_items.MarkItemAsSold(TRUE, countryname))
				{
					errlist.AddTail(details.m_productID);
					err = TRUE;
				}
			}
		}

		if(err)
			ShowErrorMessage("mark as sold", &errlist, FALSE);

		gStockView->RefreshEntries(FALSE);
	}
}


void CMainFrame::OnTreeMarkitemassoldOnserialnumber() 
{
	if(!MakeGlobalsValid())
		return;

	if(gDoc->m_vouchers.m_record.m_Type != EXPORT_TYPE)
		return;

	if(gDoc->m_vouchers.IsRootVoucher())
	{
		MessageBox("You cannot perform this operation on the root folder.", "ERROR");
		return;
	}

	CGenericTwoEditDialog<int, CString> dlg(1, 1, 1, "Sold Serial Number", FALSE, "Please enter a serial number.", 
		"", "", "", "Sold in country", FALSE, "Please enter a country.", 
		"Mark Sold Serial", NO_READONLY);

	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		int serial_number = dlg.m_val_1;
		CString countryname = dlg.m_val_2;
		dlg.m_val_1 = 1;
		BOOL error = TRUE;

		DatabaseState state = gDoc->m_vouchers.m_sets.GotoFirstSet();

		while(state == DBOK)
		{
			if(gDoc->m_vouchers.m_sets.m_items.GotoItem(serial_number, TRUE) == DBOK)
				break;

			state = gDoc->m_vouchers.m_sets.GotoNextSet();
		}

		if(!gDoc->m_vouchers.m_sets.IsEOF() && (gDoc->m_vouchers.m_sets.m_items.m_record.m_Serial_number == serial_number))
		{
			if(gDoc->m_vouchers.m_sets.m_items.MarkItemAsSold(TRUE, countryname))
			{
				error = FALSE;
			}
		}

		if(error)
		{
			MessageBox("No product chosen to mark as sold.");
			continue;
		}

		gStockView->RefreshEntries(FALSE);
	}
}


#define REPORT_STOCKACTUAL 1
#define REPORT_STOCKMARKED 2
#define REPORT_STOCKPRICEQUOTATION 3
#define REPORT_STOCKCHECKLIST 4
#define REPORT_STOCKTAGS 5
#define REPORT_CATALOGUE 6


void CMainFrame::OnReportsStockStocksreportActualsOriginal() 
{
	GenerateStockReport(REPORT_STOCKACTUAL, Both);
}


void CMainFrame::OnReportsStockStocksreportActualsSold() 
{
	GenerateStockReport(REPORT_STOCKACTUAL, Sold);
}


void CMainFrame::OnReportsStockStocksreportActualsUnsold() 
{
	GenerateStockReport(REPORT_STOCKACTUAL, UnSold);
}


void CMainFrame::OnReportsStockStocksreportMarkedOriginal() 
{
	GenerateStockReport(REPORT_STOCKMARKED, Both);
}


void CMainFrame::OnReportsStockStocksreportMarkedSold() 
{
	GenerateStockReport(REPORT_STOCKMARKED, Sold);
}


void CMainFrame::OnReportsStockStocksreportMarkedUnsold() 
{
	GenerateStockReport(REPORT_STOCKMARKED, UnSold);
}


void CMainFrame::OnReportsStockPricequotation() 
{
	GenerateStockReport(REPORT_STOCKPRICEQUOTATION, Both);
}


void CMainFrame::OnReportsStockChecklist() 
{
	GenerateStockReport(REPORT_STOCKCHECKLIST, Both);
}


void CMainFrame::OnReportsStockTags() 
{
	GenerateStockReport(REPORT_STOCKTAGS, Both);
}


void CMainFrame::OnReportsCatalogue() 
{
	GenerateStockReport(REPORT_CATALOGUE, Both);
}


void CMainFrame::GenerateStockReport(int mode, Sold_Status type)
{
#if 0
	if(!MakeGlobalsValid())
	{
		MessageBox("CMainFrame::GenerateStockReport - No defaults.","ERROR");
		return;
	}

	if(!gDatabase || (gDatabase->GetType() != tStock))
	{
		MessageBox("CMainFrame::GenerateStockReport - No valid database.", "ERROR");
		return;
	}

	COptionsDialog option_dlg;
	CGenericTwoEditDialog tag_dlg(STARTING_ROW_AND_COLUMN_FOR_TAGS);

	if(mode == REPORT_STOCKTAGS)
	{
		if(tag_dlg.DoModal() == IDCANCEL)
			return;
	}

	if(mode == REPORT_CATALOGUE)
	{
		//do nothing
	}
	else
	{
		if(option_dlg.DoModal() == IDCANCEL)
			return;
	}

	CGenericOneEditDialog rows_in_a_page_dlg(ROWS_IN_A_PAGE);
	int rows_in_a_page = 63;

	if(mode == REPORT_STOCKPRICEQUOTATION)
	{
		rows_in_a_page_dlg.m_int = 63;
		if(rows_in_a_page_dlg.DoModal() == IDCANCEL)
			return;

		rows_in_a_page = rows_in_a_page_dlg.m_int;
	}

	BEGIN_WAIT;

	CRatnaKiranReports report;

	{
		CList<CString, CString> names;
		gStockView->GetCheckedList(&names);
		if(names.IsEmpty())
		{
			gStockView->SetCheckedList(NULL, ON);
			gStockView->GetCheckedList(&names);
			gStockView->SetCheckedList(NULL, OFF);
		}

		report.m_list = &names;
		report.m_database = gDatabase;
		report.m_filename = option_dlg.m_filename;
		report.m_rows_in_a_page = rows_in_a_page;

		if(mode == REPORT_STOCKACTUAL)
			report.m_status = report.CreateStocksReportActual(type);
		else if(mode == REPORT_STOCKMARKED)
			report.m_status = report.CreateStocksReportMarked(type);
		else if(mode == REPORT_STOCKPRICEQUOTATION)
			report.m_status = report.CreateStockPriceQuotation();
		else if(mode == REPORT_STOCKCHECKLIST)
			report.m_status = report.CreateStockChecklist();
		else if(mode == REPORT_STOCKTAGS)
			report.m_status = report.CreateStockTags(tag_dlg.m_int_1, tag_dlg.m_int_2);
		else if(mode == REPORT_CATALOGUE)
			report.m_status = report.CreateCatalogue();
	}

	END_WAIT;

	if(report.m_status == TRUE)
		MessageBox("Done processing report. Please wait while it is displayed/printed.");
#endif
}


#define REPORT_KUNDANACTUAL 1
#define REPORT_KUNDANMARKED 2
#define REPORT_KUNDANPRICEQUOTATION 3
#define REPORT_KUNDANCHECKLIST 4
#define REPORT_KUNDANTAGS 5


void CMainFrame::OnReportsKundanStocksreportActualsOriginal() 
{
	GenerateKundanReport(REPORT_KUNDANACTUAL, Both);
}


void CMainFrame::OnReportsKundanStocksreportActualsSold() 
{
	GenerateKundanReport(REPORT_KUNDANACTUAL, Sold);
}


void CMainFrame::OnReportsKundanStocksreportActualsUnsold() 
{
	GenerateKundanReport(REPORT_KUNDANACTUAL, UnSold);
}


void CMainFrame::OnReportsKundanStocksreportMarkedOriginal() 
{
	GenerateKundanReport(REPORT_KUNDANMARKED, Both);
}


void CMainFrame::OnReportsKundanStocksreportMarkedSold() 
{
	GenerateKundanReport(REPORT_KUNDANMARKED, Sold);
}


void CMainFrame::OnReportsKundanStocksreportMarkedUnsold() 
{
	GenerateKundanReport(REPORT_KUNDANMARKED, UnSold);
}


void CMainFrame::OnReportsKundanPricequotation() 
{
	GenerateKundanReport(REPORT_KUNDANPRICEQUOTATION, Both);
}


void CMainFrame::OnReportsKundanChecklist() 
{
	GenerateKundanReport(REPORT_KUNDANCHECKLIST, Both);
}


void CMainFrame::OnReportsKundanTags() 
{
	GenerateKundanReport(REPORT_KUNDANTAGS, Both);
}


void CMainFrame::GenerateKundanReport(int mode, Sold_Status type)
{
#if 0
	if(!MakeGlobalsValid())
	{
		MessageBox("CMainFrame::GenerateStockReport - No defaults.","ERROR");
		return;
	}

	if(!gDatabase || (gDatabase->GetType() != tKundan))
	{
		MessageBox("CMainFrame::GenerateStockReport - No valid database.", "ERROR");
		return;
	}

	COptionsDialog option_dlg;
	CGenericTwoEditDialog tag_dlg(STARTING_ROW_AND_COLUMN_FOR_TAGS);

	if(mode == REPORT_KUNDANTAGS)
	{
		if(tag_dlg.DoModal() == IDCANCEL)
			return;
	}

	if(option_dlg.DoModal() == IDCANCEL)
		return;

	CGenericOneEditDialog rows_in_a_page_dlg(ROWS_IN_A_PAGE);
	int rows_in_a_page = 64;

	if(mode == REPORT_KUNDANPRICEQUOTATION)
	{
		rows_in_a_page_dlg.m_int = 64;
		if(rows_in_a_page_dlg.DoModal() == IDCANCEL)
			return;

		rows_in_a_page = rows_in_a_page_dlg.m_int;
	}

	BEGIN_WAIT;

	CRatnaKiranReports report;

	{
		CList<CString, CString> names;
		gStockView->GetCheckedList(&names);
		if(names.IsEmpty())
		{
			gStockView->SetCheckedList(NULL, ON);
			gStockView->GetCheckedList(&names);
			gStockView->SetCheckedList(NULL, OFF);
		}

		report.m_list = &names;
		report.m_database = gDatabase;
		report.m_filename = option_dlg.m_filename;
		report.m_rows_in_a_page = rows_in_a_page;

		if(mode == REPORT_KUNDANACTUAL)
			report.m_status = report.CreateKundanReportActual(type);
		else if(mode == REPORT_KUNDANMARKED)
			report.m_status = report.CreateKundanReportMarked(type);
		else if(mode == REPORT_KUNDANPRICEQUOTATION)
			report.m_status = report.CreateKundanPriceQuotation();
		else if(mode == REPORT_KUNDANCHECKLIST)
			report.m_status = report.CreateKundanChecklist();
		else if(mode == REPORT_KUNDANTAGS)
			report.m_status = report.CreateKundanTags(tag_dlg.m_int_1, tag_dlg.m_int_2);
	}

	END_WAIT;

	if(report.m_status == TRUE)
		MessageBox("Done processing report. Please wait while it is displayed/printed.");
#endif
}


#define REPORT_PACKAGELIST 1
#define REPORT_SHORTINVOICE 2
#define REPORT_PHOTOINVOICE 3
#define REPORT_GRFORM 4
#define REPORT_PRICEQUOTATION 5
#define REPORT_EXPORTCHECKLIST 6
#define REPORT_EXPORTTAGS 7


void CMainFrame::OnReportsExportPackagelistOriginal() 
{
	GenerateExportReport(REPORT_PACKAGELIST, Both);
}


void CMainFrame::OnReportsExportPackagelistSold() 
{
	GenerateExportReport(REPORT_PACKAGELIST, Sold);
}


void CMainFrame::OnReportsExportPackagelistUnsold() 
{
	GenerateExportReport(REPORT_PACKAGELIST, UnSold);
}


void CMainFrame::OnReportsExportShortinvoiceOriginal() 
{
	GenerateExportReport(REPORT_SHORTINVOICE, Both);
}


void CMainFrame::OnReportsExportShortinvoiceSold() 
{
	GenerateExportReport(REPORT_SHORTINVOICE, Sold);
}


void CMainFrame::OnReportsExportShortinvoiceUnsold() 
{
	GenerateExportReport(REPORT_SHORTINVOICE, UnSold);
}


void CMainFrame::OnReportsExportPhotoinvoiceOriginal() 
{
	GenerateExportReport(REPORT_PHOTOINVOICE, Both);
}


void CMainFrame::OnReportsExportPhotoinvoiceSold() 
{
	GenerateExportReport(REPORT_PHOTOINVOICE, Sold);
}


void CMainFrame::OnReportsExportPhotoinvoiceUnsold() 
{
	GenerateExportReport(REPORT_PHOTOINVOICE, UnSold);
}


void CMainFrame::OnReportsExportGrform() 
{
	GenerateExportReport(REPORT_GRFORM, Both);
}


void CMainFrame::OnReportsExportPricequotation() 
{
	GenerateExportReport(REPORT_PRICEQUOTATION, Both);
}


void CMainFrame::OnReportsExportChecklist() 
{
	GenerateExportReport(REPORT_EXPORTCHECKLIST, Both);
}


void CMainFrame::OnReportsExportTags() 
{
	GenerateExportReport(REPORT_EXPORTTAGS, Both);
}


void CMainFrame::GenerateExportReport(int mode, Sold_Status type)
{
#if 0
	if(!MakeGlobalsValid())
	{
		MessageBox("CMainFrame::GenerateExportReport - No defaults.", "ERROR");
		return;
	}

	if(!gDatabase || (gDatabase->GetType() != tExport))
	{
		MessageBox("CMainFrame::GenerateExportReport - No valid database.", "ERROR");
		return;
	}

	SFileHeaderNode common_header;
	if(gDatabase->IsRootDatabase())
	{
		CDetailsDialogExport dlg(&common_header, TRUE);
		if(dlg.DoModal() == IDCANCEL)
		{
			MessageBox("Cannot complete processing as no valid database information was found to complete the report.");
			return;
		}
	}

	CExportSerialsDialog serials_dlg;
	COptionsDialog option_dlg;
	CGenericTwoEditDialog tag_dlg(STARTING_ROW_AND_COLUMN_FOR_TAGS);

	if(serials_dlg.DoModal() == IDCANCEL)
		return;

	if(mode == REPORT_EXPORTTAGS)
	{
		if(tag_dlg.DoModal() == IDCANCEL)
			return;
	}

	if(option_dlg.DoModal() == IDCANCEL)
		return;

	CGenericOneEditDialog image_count_dlg(NUMBER_OF_IMAGES);
	int image_count = 1;
	
	if(mode == REPORT_PHOTOINVOICE)
	{
		if(image_count_dlg.DoModal() == IDCANCEL)
			return;

		image_count = image_count_dlg.m_int;
	}

	CGenericOneEditDialog rows_in_a_page_dlg(ROWS_IN_A_PAGE);
	int rows_in_a_page = 55;

	if(mode == REPORT_PRICEQUOTATION)
	{
		rows_in_a_page_dlg.m_int = 55;
		if(rows_in_a_page_dlg.DoModal() == IDCANCEL)
			return;

		rows_in_a_page = rows_in_a_page_dlg.m_int;
	}

	BEGIN_WAIT;

	CRatnaKiranReports report;

	{
		CList<CString, CString> names;
		gStockView->GetCheckedList(&names);
		if(names.IsEmpty())
		{
			gStockView->SetCheckedList(NULL, ON);
			gStockView->GetCheckedList(&names);
			gStockView->SetCheckedList(NULL, OFF);
		}

		report.m_list = &names;
		report.m_database = gDatabase;
		report.m_serials_from = serials_dlg.m_from;
		report.m_serials_to = serials_dlg.m_to;
		report.m_use_new_serials = serials_dlg.m_use_new;
		report.m_use_old_serials = serials_dlg.m_use_old;
		report.m_old_serial_tagname = serials_dlg.m_tagname;
		report.m_filename = option_dlg.m_filename;
		report.m_image_count = image_count;
		report.m_rows_in_a_page = rows_in_a_page;

		if(gDatabase->IsRootDatabase())
			report.m_header = &common_header;

		if(mode == REPORT_PACKAGELIST)
			report.m_status = report.CreatePackageList(type);
		else if(mode == REPORT_SHORTINVOICE)
			report.m_status = report.CreateShortInvoice(type);
		else if(mode == REPORT_PHOTOINVOICE)
			report.m_status = report.CreatePhotoInvoice(type);
		else if(mode == REPORT_GRFORM)
			report.m_status = report.CreateGRForm();
		else if(mode == REPORT_PRICEQUOTATION)
			report.m_status = report.CreateExportPriceQuotation();
		else if(mode == REPORT_EXPORTCHECKLIST)
			report.m_status = report.CreateExportChecklist();
		else if(mode == REPORT_EXPORTTAGS)
			report.m_status = report.CreateExportTags(tag_dlg.m_int_1, tag_dlg.m_int_2);
	}

	END_WAIT;

	if(report.m_status == TRUE)
		MessageBox("Done processing report. Please wait while it is displayed/printed.");
#endif
}


void CMainFrame::OnUpdateUsingforexportsDollarusd(CCmdUI* pCmdUI) 
{
	if(gSettings.m_Use_native_currency)
		pCmdUI->SetCheck(FALSE);
	else
		pCmdUI->SetCheck(TRUE);
}


void CMainFrame::OnUpdateUsingforexportsNativecurrency(CCmdUI* pCmdUI) 
{
	if(gSettings.m_Use_native_currency)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}


void CMainFrame::OnUpdateSetAddnew(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetDeleteCurrent(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	int index;
	gStockView->GetSelectedProduct(&index);
	if(index < 0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetDeleteChecked(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CList<CString, CString> list;
	gStockView->GetCheckedList(&list);
	if(list.IsEmpty())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetDeleteHighlighted(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CList<CString, CString> list;
	gStockView->GetHighlightedList(&list);
	if(list.IsEmpty())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetDeletecheckedHighlighted(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CList<CString, CString> list;
	gStockView->GetHighlightedList(&list);
	gStockView->GetCheckedList(&list);
	if(list.IsEmpty())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetEdit(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	int index;
	gStockView->GetSelectedProduct(&index);
	if(index < 0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetMoveCreatenew(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CList<CString, CString> list;
	gStockView->GetCheckedList(&list);
	if(list.IsEmpty())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetMoveAddtoexisting(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CList<CString, CString> list;
	gStockView->GetCheckedList(&list);
	if(list.IsEmpty())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetSelectall(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetSelecthighlighted(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CList<CString, CString> list;
	gStockView->GetHighlightedList(&list);
	if(list.IsEmpty())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetUnselectall(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetUnselecthighlighted(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CList<CString, CString> list;
	gStockView->GetHighlightedList(&list);
	if(list.IsEmpty())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetChangeproductid(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSetResetoldserials(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateTreeCreatenewstockvoucher(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateTreeCreatenewexportvoucher(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateTreeDeleteselected(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateTreeDeletesolditems(CCmdUI* pCmdUI)
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateTreeQuerySet(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateTreeUpdateinformation(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateTreeReallocateserialnumbers(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateTreeMarkitemassoldOnproductid(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateTreeMarkitemassoldOnserialnumber(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateReportsStock(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateReportsKundan(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateReportsExport(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateReportsCatalogue(CCmdUI* pCmdUI) 
{
	if(!MakeGlobalsValid())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

BOOL CMainFrame::OpenForEdit(CString productID)
{
	if(!MakeGlobalsValid())
		return FALSE;

	if(productID.IsEmpty())
	{
		MessageBox("CMainFrame::OpenForEdit - No productID.", "ERROR");
		return FALSE;
	}

	if(gDoc->m_vouchers.m_sets.GotoSet(productID) != DBOK)
	{
		CString errstr = "Could not get the set \"" + productID + "\" for editing.";
		MessageBox(errstr, "ERROR");
		return FALSE;
	}

	//if(!gDoc->m_main_database.CanTransact())
		//return FALSE;

	//if(!gDoc->m_main_database.BeginTrans())
		//return FALSE;
	
	int dialog_retval = IDCANCEL;
	if(gDoc->m_vouchers.m_record.m_Type == STOCK_TYPE)
	{
		CAddDialogStock dlg;
		dialog_retval = dlg.DoModal();
	}
	else if(gDoc->m_vouchers.m_record.m_Type == KUNDAN_TYPE)
	{
		CAddDialogStock dlg;
		dialog_retval = dlg.DoModal();
	}
	else if(gDoc->m_vouchers.m_record.m_Type == EXPORT_TYPE)
	{
		//TODO ... getdesiredserialnumber
		CAddDialogExport dlg(0);
		dialog_retval = dlg.DoModal();
	}

	if(dialog_retval == IDOK)
	{
		//if(!gDoc->m_main_database.CommitTrans())
			//return FALSE;

		gStockView->RefreshEntries(FALSE);
	}
	else
	{
		//if(!gDoc->m_main_database.Rollback())
			//return FALSE;
	}

	return TRUE;
}


BOOL CMainFrame::DeleteSets(CList<CString, CString> *list, BOOL force)
{
	if(!MakeGlobalsValid())
	{
		MessageBox("CMainFrame::DeleteSets - No defaults.", "ERROR");
		return FALSE;
	}

	if(!list)
	{
		MessageBox("CMainFrame::DeleteSets - No list.", "ERROR");
		return FALSE;
	}

	if(list->IsEmpty())
		return TRUE;

	if(!force)
	{
		int retval = MessageBox("Are you sure you want to delete the sets ?", "Warning", MB_YESNO | MB_ICONWARNING);
		if(retval == IDNO)
			return TRUE;
	}

	CProgressDialog progress("Deleting sets", list->GetCount());
	progress.StartDialog();

	BOOL err = FALSE;
	int count = list->GetCount();
	while(count > 0)
	{
		CString productID = list->RemoveHead();
		if(!gDoc->m_vouchers.DeleteSet(productID))
		{
			list->AddTail(productID);
			err = TRUE;
		}

		progress.ProgressForward();
		count--;
	}

	progress.FinishDialog();
	gStockView->RefreshEntries(FALSE);
	return TRUE;
}


BOOL CMainFrame::MoveSetsTo(CList<CString, CString> *pendinglist, CList<CString, CString> *successlist, CString dest_voucher_name, CString dest_voucher_type, BOOL newly_created)
{
	if(!MakeGlobalsValid())
	{
		MessageBox("CMainFrame::MoveSetsTo - No defaults.", "ERROR");
		return FALSE;
	}

	if(dest_voucher_name.IsEmpty())
	{
		MessageBox("CMainFrame::MoveSetsTo - No filename provided.", "ERROR");
		return FALSE;
	}

	if(!pendinglist || !successlist)
	{
		MessageBox("CMainFrame::MoveSetsTo - No list.", "ERROR");
		return FALSE;
	}

	if(pendinglist->IsEmpty())
		return TRUE;

	if(newly_created && (dest_voucher_type == EXPORT_TYPE))
	{	
		CString current_voucher = gDoc->m_vouchers.m_record.m_Voucher_ID;

		if(gDoc->m_vouchers.GotoVoucher(dest_voucher_name) != DBOK)
			return FALSE;

		CDetailsDialogExport dlg;
		dlg.DoModal();

		if(gDoc->m_vouchers.GotoVoucher(current_voucher) != DBOK)
			return FALSE;
	}

	BOOL doSerialAssignment = FALSE;
	int serial = 0;
	int start_serial = 0;
	if(dest_voucher_type == EXPORT_TYPE)
	{
		//TODO ... getdesiredserialnumber
		start_serial = 1;
		if(start_serial == 1)
		{
			doSerialAssignment = TRUE;

			CGenericOneEditDialog<int> newSerialDlg(1, 1, 1, "Starting serial number", FALSE, "", "Serial Number");
			newSerialDlg.DoModal();
			start_serial = newSerialDlg.m_val;
		}

		serial = start_serial;
	}

	BOOL storeOldSerial = FALSE;
	CString oldSerialTag = "";
	if((dest_voucher_type == STOCK_TYPE) && (gDoc->m_vouchers.m_record.m_Type == EXPORT_TYPE))
	{
		storeOldSerial = (MessageBox("Do you want to store the old serial number for future use ?", "", MB_YESNO) == IDYES);
		if(storeOldSerial)
		{
			CGenericOneEditDialog<CString> dlg("", "", "", "Tag name for reference", FALSE, "Please enter a valid tagname.", "Tag Name");
			while(TRUE)
			{
				if(dlg.DoModal() == IDOK)
				{
					oldSerialTag = dlg.m_val;
					break;
				}
				else
				{
					if(MessageBox("Are you sure you don't want to store the old serials ?", "", MB_YESNO) == IDYES)
					{
						storeOldSerial = FALSE;
						break;
					}
				}
			}
		}
	}

	CProgressDialog progress("Moving sets", pendinglist->GetCount());
	progress.StartDialog();

	BOOL err = FALSE;
	int count = pendinglist->GetCount();
	while(count > 0)
	{
		CString productID = pendinglist->RemoveHead();

		if(gDoc->m_vouchers.m_sets.GotoSet(productID) != DBOK)
		{
			pendinglist->AddTail(productID);
			err = TRUE;
			count--;
			progress.ProgressForward();
			continue;
		}

		if(storeOldSerial)
		{
			gDoc->m_vouchers.m_sets.StoreOldSerialNumbers(oldSerialTag);
		}

		gDoc->m_vouchers.m_sets.UpdateItemSerials(&serial);

		BOOL add_success = FALSE;
		if(gDoc->m_vouchers.m_sets.DBEditEntry())
		{
			gDoc->m_vouchers.m_sets.m_record.m_Voucher_ID = dest_voucher_name;
			if(gDoc->m_vouchers.m_sets.DBUpdateEntry("MoveSetsTo"))
			{
				if(gDoc->m_vouchers.DBEditEntry())
				{
					gDoc->m_vouchers.m_record.m_Set_count--;
					if(gDoc->m_vouchers.DBUpdateEntry("MoveSetsTo"))
					{
						CString current_voucher = gDoc->m_vouchers.m_record.m_Voucher_ID;
						if(gDoc->m_vouchers.GotoVoucher(dest_voucher_name,FALSE))
						{
							if(gDoc->m_vouchers.DBEditEntry())
							{
								gDoc->m_vouchers.m_record.m_Set_count++;
								if(gDoc->m_vouchers.DBUpdateEntry("MoveSetsTo"))
								{
									if(gDoc->m_vouchers.GotoVoucher(current_voucher, FALSE))
										add_success = TRUE;
								}
							}
						}
					}
				}
			}
		}

		if(!add_success)
		{
			pendinglist->AddTail(productID);
			err = TRUE;
		}
		else
		{
			successlist->AddTail(productID);
		}

		progress.ProgressForward();
		count--;
	}

	if(doSerialAssignment)
		gDoc->m_vouchers.AssignSerialNumbers(&start_serial);

	progress.FinishDialog();
	gStockView->RefreshEntries(FALSE);
	return TRUE;
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case VK_TAB:
			CWnd *current = CWnd::GetFocus();
			if(current == gTreeView)
				gStockView->SetFocus();
			else
				gTreeView->SetFocus();

			break;
		}
		break;
	case WM_KEYUP:
		break;
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

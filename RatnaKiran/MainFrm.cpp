#include "stdafx.h"
#include "RatnaKiran.h"
#include "MainGlobal.h"
#include "MainFrm.h"
#include "RatnaKiranReports.h"

#include "AppendListDialog.h"
#include "OptionsDialog.h"
#include "AddDialog.h"
#include "DetailsDialog.h"
#include "GenericListBoxDialog.h"
#include "GenericOneEditDialog.h"
#include "GenericTwoEditDialog.h"
#include "ExportSerialsDialog.h"
#include "MoveOptionDialog.h"
#include "BackupHandler.h"
#include "EditPermissions.h"
#include "AddUserDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TREE_CREATENEWSTOCKVOUCHER, OnTreeCreatenewstockvoucher)
	ON_COMMAND(ID_TREE_CREATENEWEXPORTVOUCHER, OnTreeCreatenewexportvoucher)
	ON_COMMAND(ID_TREE_CREATENEWKUNDANVOUCHER, OnTreeCreatenewkundanvoucher)
	ON_COMMAND(ID_TREE_DELETESELECTED, OnTreeDeleteselected)
	ON_COMMAND(ID_TREE_IGNORESELECTED, OnTreeIgnoreselected)
	ON_COMMAND(ID_TREE_DELETESOLDITEMS, OnTreeDeletesolditems)
	ON_COMMAND(ID_TREE_QUERYSET_ID, OnTreeQuerySetID)
	ON_COMMAND(ID_TREE_QUERYSET_GROSS_WEIGHT, OnTreeQuerySetGrossWeight)
	ON_COMMAND(ID_TREE_UPDATEINFORMATION, OnTreeUpdateinformation)
	ON_COMMAND(ID_TREE_REALLOCATESERIALNUMBERS, OnTreeReallocateserialnumbers)
	ON_COMMAND(ID_TREE_MARKITEMASSOLD_ONPRODUCTID, OnTreeMarkitemassoldOnproductid)
	ON_COMMAND(ID_TREE_MARKITEMASSOLD_ONSERIALNUMBER, OnTreeMarkitemassoldOnserialnumber)
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
	ON_COMMAND_RANGE(ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_ORIGINAL, ID_REPORTS_STOCK_PHOTOINVOICE_UNSOLD, OnReportsStock)
	ON_COMMAND_RANGE(ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_ORIGINAL, ID_REPORTS_KUNDAN_TAGS, OnReportsKundan)
	ON_COMMAND_RANGE(ID_REPORTS_EXPORT_PACKAGELIST_ORIGINAL, ID_REPORTS_EXPORT_GRFORM, OnReportsExport)
	ON_COMMAND(ID_REPORTS_CATALOGUE, OnReportsCatalogue)
	ON_COMMAND(ID_SETTINGS_UPDATEGOLDRATE, OnSettingsUpdategoldrate)
	ON_COMMAND(ID_SETTINGS_MANAGEDESCRIPTIONDATABASE, OnSettingsManagedescriptiondatabase)
	ON_COMMAND(ID_SETTINGS_CHANGEFILEPATH, OnSettingsChangefilepath)
	ON_COMMAND(ID_SETTINGS_BACKUP, OnSettingsBackup)
	ON_COMMAND(ID_USINGFOREXPORTS_DOLLARUSD, OnUsingforexportsDollarusd)
	ON_COMMAND(ID_USINGFOREXPORTS_NATIVECURRENCY, OnUsingforexportsNativecurrency)
	ON_COMMAND(ID_USER_ADD, OnUserAdd)
	ON_COMMAND(ID_USER_DELETE, OnUserDelete)
	ON_COMMAND(ID_USER_EDIT_PERM, OnUserEditPerm)
	ON_COMMAND(ID_USER_CHANGE_PASSWORD, OnUserChangePassword)
	ON_UPDATE_COMMAND_UI_RANGE(1, 99999, OnUpdateGeneral)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_TREE_CREATENEWSTOCKVOUCHER, ID_USER_CHANGE_PASSWORD, OnUpdateGeneral)
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
	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not create main window.");
		return -1;
	}

	/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		SHOW_INTERNAL_ERROR;
		return -1;
	}
	*/

	if(!m_wndStatusBar.Create(this) ||
	   !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not create the status bar.");
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
	if(!CFrameWnd::PreCreateWindow(cs))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not create the main window.");
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::ChangeView()
{
	if(m_wndSplitter)
	{
		//Somebody called this by mistake
		SHOW_INTERNAL_ERROR;
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
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	SALIL_NEW(m_wndSplitter, CSplitterWnd);

	if(!m_wndSplitter->CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not create the splitter window.");
		delete m_wndSplitter;
		m_wndSplitter = NULL;
		return FALSE;
	}

	CCreateContext context;
	context.m_pCurrentFrame = this;
	context.m_pCurrentDoc = GetActiveDocument();

	if(!(m_wndSplitter->CreateView(0, 0, RUNTIME_CLASS(CRatnaKiranTreeView), sz1, &context)))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not create the tree view.");
		delete m_wndSplitter;
		m_wndSplitter = NULL;
		return FALSE;
	}

	if(!(m_wndSplitter->CreateView(0, 1, RUNTIME_CLASS(CRatnaKiranStockView), sz2, &context)))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not create the list view.");
		delete m_wndSplitter;
		m_wndSplitter = NULL;
		return FALSE;
	}
	
	CView *addedview1, *addedview2;
	addedview1 = STATIC_DOWNCAST(CView, m_wndSplitter->GetPane(0, 0));
	addedview2 = STATIC_DOWNCAST(CView, m_wndSplitter->GetPane(0, 1));
	if(addedview1 == NULL || addedview2 == NULL)
	{
		SHOW_INTERNAL_ERROR;
		delete m_wndSplitter;
		m_wndSplitter = NULL;
		return FALSE;
	}

	if(!removedview->DestroyWindow())
	{
		SHOW_INTERNAL_ERROR;
		delete m_wndSplitter;
		m_wndSplitter = NULL;
		return FALSE;
	}
	
	if(!MakeGlobalsValid(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");

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


void CMainFrame::OnSettingsUpdategoldrate() 
{
	CGenericTwoEditDialog<double, double> dlg(CSettings::GetGoldRate(), "Old Value", 0.0, "New Value", "Change Gold Value");
	dlg.SetLimits2(0.0, 0.0, FALSE, "Please enter a positive value.");
	dlg.SetProperties(ReadOnly_Prop, Simple_Prop);

	if(dlg.DoModal() == IDCANCEL)
		return;

	if(!CSettings::SetGoldRate(dlg.m_val_2))
		SHOW_INTERNAL_ERROR_REASON("Could not update the gold rate.");
}


void CMainFrame::OnSettingsManagedescriptiondatabase() 
{
	CGenericListBoxDialog dlg(DescriptionModify);
	dlg.DoModal();
}


void CMainFrame::OnSettingsChangefilepath() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CString original = gFilePath;
	CRatnaKiranApp *app = (CRatnaKiranApp *)AfxGetApp();
	if(!app || !app->SetDatabasePath())
	{
		SHOW_INTERNAL_ERROR_REASON("Could not change the database path.");
		return;
	}

	if(original == gFilePath)
		return;

	CString newpath = gFilePath;
	gFilePath = original;

	if(!gDoc->WriteVouchers())
	{
		//not changing database path
		SHOW_INTERNAL_ERROR_REASON("Could not write the voucher files.");
		return;
	}

	gFilePath = newpath;
	if(!gDoc->ReadVouchers())
		SHOW_INTERNAL_ERROR_REASON("Could not read the voucher files.");

	gTreeView->OnInitialUpdate();
}


void CMainFrame::OnUsingforexportsDollarusd() 
{
	if(!CSettings::SetUseNativeCurrency(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not set the use of native currency.");
}


void CMainFrame::OnUsingforexportsNativecurrency() 
{
	if(!CSettings::SetUseNativeCurrency(TRUE))
		SHOW_INTERNAL_ERROR_REASON("Could not set the use of native currency.");
}


void CMainFrame::OnSettingsBackup() 
{
	CBackupHandler bh;
	bh.Create();
}


void CMainFrame::OnUserAdd() 
{
	CAddUserDialog dlg(ADD_USER);
	dlg.DoModal();
}


void CMainFrame::OnUserDelete() 
{
	CGenericOneEditDialog<CString> dlg("", "Name of user to delete", "Delete User");
	dlg.SetLimits("", "", FALSE, "Please enter a username to delete.");
	if(dlg.DoModal() == IDCANCEL)
		return;

	if(dlg.m_val == CPermissions::GetAdminUsername())
	{
		SHOW_ERROR("Cannot delete the 'admin' user.");
		return;
	}

	if(!CPermissions::IsUserExisting(dlg.m_val))
	{
		CString errstr = "User '" + dlg.m_val + "' does not exist.";
		SHOW_ERROR(errstr);
		return;
	}

	CString current_user;
	if(!CPermissions::GetCurrentUser(&current_user))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not get current username.");
		return;
	}

	if(dlg.m_val == current_user)
	{
		SHOW_ERROR("Cannot delete the self username.");
		return;
	}

	if(CPermissions::DeleteUser(dlg.m_val))
	{
		CString errstr = "Could not deleted user '" + dlg.m_val + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}
}


void CMainFrame::OnUserEditPerm() 
{
	CEditPermissions dlg;
	dlg.DoModal();
}


void CMainFrame::OnUserChangePassword() 
{
	CAddUserDialog dlg(CHANGE_PASSWORD);
	dlg.DoModal();
}


void CMainFrame::OnSetAddnew() 
{
	if(!PerformSetAddNew())
		SHOW_INTERNAL_ERROR_REASON("Could not add a new user.");
}

BOOL CMainFrame::PerformSetAddNew()
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return FALSE;
	}

	if(gDoc->m_selected_voucher->m_type == tExport)
	{
		SHOW_ERROR("You cannot perform this operation on an export voucher.");
		return FALSE;
	}

	while(TRUE)
	{
		CString voucher_name;
		if(gDoc->m_selected_voucher->IsRootVoucher())
		{
			CList<CProductVoucher *, CProductVoucher *> vlist;
			POSITION pos = gDoc->m_stock_vouchers.GetHeadPosition();
			while(pos)
			{
				CProductVoucher *curvoucher = gDoc->m_stock_vouchers.GetAt(pos);
				if(curvoucher)
				{
					if(!gDoc->IsIgnored(curvoucher->m_voucher_name))
						vlist.AddTail(curvoucher);
				}
				else
				{
					SHOW_INTERNAL_ERROR;
					return FALSE;
				}

				gDoc->m_stock_vouchers.GetNext(pos);
			}

			CAppendListDialog dlg(&vlist);
			if(dlg.DoModal() == IDCANCEL)
				break;

			voucher_name = dlg.m_selection;
		}
		else
			voucher_name = gDoc->m_selected_voucher->m_voucher_name;

		if(voucher_name.IsEmpty())
		{
			SHOW_INTERNAL_ERROR;
			continue;
		}

		CProductVoucher *voucher = gDoc->GetVoucher(voucher_name);
		if(!voucher)
		{
			CString errstr = "Could not find voucher with name '" + voucher_name + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}

		CProductSet set;
		CString productID = CProductItem::GetProductID();
		strcpy(set.m_product_ID, LPCTSTR(productID));

		CProductItem emptyitem;
		if(!set.AddItem(&emptyitem, TRUE))
		{
			CString errstr = "Could not add empty item to the set '" + CString(set.m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name);
			SHOW_INTERNAL_ERROR_REASON(errstr);
			continue;
		}

		double gold_rate = GetGoldRate(voucher);
		CAddDialog dlg(voucher->m_type, &set, gold_rate);
		if(dlg.DoModal() == IDCANCEL)
			break;

		if(!voucher->AddSet(&set, TRUE, TRUE))
		{
			CList<CString, CString> list;
			CString id = set.m_product_ID;
			list.AddHead(productID);
			ShowErrorMessage("add", &list, FALSE);
			continue;
		}
	}

	if(!gStockView->RefreshEntries(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not refresh the list view.");

	return TRUE;
}

void CMainFrame::OnSetDeleteCurrent() 
{
	if(!PerformSetDeleteCurrent(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not delete the currently selected set.");
}


BOOL CMainFrame::PerformSetDeleteCurrent(BOOL force)
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return FALSE;
	}

	BEGIN_WAIT;

	CList<CString, CString> list;
	CString productID;
	int index;
	productID = gStockView->GetSelectedProduct(&index);
	if(index < 0)
	{
		SHOW_ERROR("Please select a set to delete.");
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
	if(!PerformSetDeleteChecked(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not delete checked sets.");
}


BOOL CMainFrame::PerformSetDeleteChecked(BOOL force) 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return FALSE;
	}

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
	if(!PerformSetDeleteHighlighted(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not delete highlighted sets.");
}


BOOL CMainFrame::PerformSetDeleteHighlighted(BOOL force) 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return FALSE;
	}

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
	if(!PerformSetDeleteCheckedHighlighted(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not delete checked/highlighted sets.");
}


BOOL CMainFrame::PerformSetDeleteCheckedHighlighted(BOOL force) 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return FALSE;
	}

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
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CString productID;
	int index;
	productID = gStockView->GetSelectedProduct(&index);
	if(index < 0)
	{
		SHOW_ERROR("Please select a set for editing.");
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
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CList<CString, CString> pendingMoveList;
	gStockView->GetCheckedList(&pendingMoveList);
	if(pendingMoveList.IsEmpty())
	{
		CString productID;
		int index;
		productID = gStockView->GetSelectedProduct(&index);
		if(index < 0)
		{
			SHOW_ERROR("Please select a set for moving.");
			return;
		}

		int retval;
		CString warnstr = "Are you sure you want to move set '" + productID + "'?";
		SHOW_WARNING_YESNO(warnstr, retval);
		if(retval == IDNO)
			return;

		pendingMoveList.AddTail(productID);
	}
	else
	{
		int retval;
		CString warnstr = "Are you sure you want to move all checked sets?";
		SHOW_WARNING_YESNO(warnstr, retval);
		if(retval == IDNO)
			return;
	}

	CMoveOptionDialog typedlg;
	if(typedlg.DoModal() == IDCANCEL)
		return;

	DatabaseType dest_type = typedlg.m_type;
	CList<CProductVoucher *, CProductVoucher *> *voucherlist = NULL;

	switch(dest_type)
	{
	case tStock:
		voucherlist = &(gDoc->m_stock_vouchers);
		break;
	case tExport:
		voucherlist = &(gDoc->m_export_vouchers);
		break;
	case tKundan:
		voucherlist = &(gDoc->m_kundan_vouchers);
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", dest_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}

	if(!voucherlist)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	CAppendListDialog dlg(voucherlist);
	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CString voucher_name = gDoc->m_selected_voucher->m_voucher_name;
		if(dlg.m_selection.CompareNoCase(voucher_name) == 0)
		{
			SHOW_ERROR("You cannot move/copy to the same folder.");
			continue;
		}
		else
		{
			break;
		}
	}

	BEGIN_WAIT;

	CList<CString, CString> pendingDeleteList;
	if(!MoveSetsTo(&pendingMoveList, &pendingDeleteList, dlg.m_selection, dest_type, FALSE))
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

	if(!gStockView->RefreshEntries(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not refresh the list view.");

	END_WAIT;
}


void CMainFrame::OnSetMoveCreatenew() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CList<CString, CString> pendingMoveList;
	gStockView->GetCheckedList(&pendingMoveList);
	if(pendingMoveList.IsEmpty())
	{
		CString productID;
		int index;
		productID = gStockView->GetSelectedProduct(&index);
		if(index < 0)
		{
			SHOW_ERROR("Please select a set for moving.");
			return;
		}

		int retval;
		CString warnstr = "Are you sure you want to move set '" + productID + "'?";
		SHOW_WARNING_YESNO(warnstr, retval);
		if(retval == IDNO)
			return;

		pendingMoveList.AddTail(productID);
	}
	else
	{
		int retval;
		CString warnstr = "Are you sure you want to move all checked sets?";
		SHOW_WARNING_YESNO(warnstr, retval);
		if(retval == IDNO)
			return;
	}

	CMoveOptionDialog typedlg;
	if(typedlg.DoModal() == IDCANCEL)
		return;

	DatabaseType dest_type = typedlg.m_type;
	CGenericOneEditDialog<CString> dlg("", "New Voucher Name", "Create New Voucher");
	dlg.SetLimits("", "", FALSE, "Please enter a name of the voucher to create.");

	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CString voucher_name = dlg.m_val;
		if(gDoc->GetVoucher(voucher_name))
		{
			CString str = "A voucher/list by the name '" + voucher_name + "' already exists. Do you want to overwrite ?";
			int retval = MessageBox(str, "Warning", MB_YESNO | MB_ICONWARNING);
			if(retval == IDNO)
				continue;
		}

		break;
	}

	BEGIN_WAIT;

	CList<CString, CString> pendingDeleteList;
	if(!MoveSetsTo(&pendingMoveList, &pendingDeleteList, dlg.m_val, dest_type, TRUE))
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
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	gStockView->SetCheckedList(NULL, ON);
}


void CMainFrame::OnSetSelecthighlighted() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CList<CString, CString> list;
	gStockView->GetHighlightedList(&list);
	gStockView->SetCheckedList(&list, ON);
}


void CMainFrame::OnSetUnselectall() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	gStockView->SetCheckedList(NULL, OFF);
}


void CMainFrame::OnSetUnselecthighlighted() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CList<CString, CString> list;
	gStockView->GetHighlightedList(&list);
	gStockView->SetCheckedList(&list, OFF);
}


void CMainFrame::OnSetChangeproductid() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CString productID;
	int index;
	productID = gStockView->GetSelectedProduct(&index);
	if(index < 0)
	{
		SHOW_ERROR("Please select a set for changing the productID.");
		return;
	}

	CProductVoucher *voucher = NULL;
	CProductSet *set = gDoc->GetSet(productID, &voucher);
	if(!set || !voucher)
	{
		CList<CString, CString> list;
		list.AddHead(productID);
		ShowErrorMessage("change productID of", &list, FALSE);
		return;
	}

	if(!voucher->ChangeProductID(productID))
	{
		CList<CString, CString> list;
		list.AddHead(productID);
		ShowErrorMessage("change productID of", &list, FALSE);
		return;
	}

	if(!gStockView->RefreshEntries(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not refresh the list view.");
}


void CMainFrame::OnSetResetoldserials() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CList<CString, CString> list;
	gStockView->GetCheckedList(&list);
	if(list.IsEmpty())
	{
		CString productID;
		int index;
		productID = gStockView->GetSelectedProduct(&index);
		if(index < 0)
		{
			SHOW_ERROR("Please select a set for resetting the serial numbers.");
			return;
		}

		int retval;
		CString warnstr = "Are you sure you want to delete all old serial numbers in set '" + productID + "'?";
		SHOW_WARNING_YESNO(warnstr, retval);
		if(retval == IDNO)
			return;

		list.AddTail(productID);
	}
	else
	{
		int retval;
		CString warnstr = "Are you sure you want to delete all old serial numbers of all checked sets?";
		SHOW_WARNING_YESNO(warnstr, retval);
		if(retval == IDNO)
			return;
	}

	CList<CString, CString> err_list;
	POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		CString productID = list.GetAt(pos);
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(!set || !voucher)
			err_list.AddTail(productID);
		else if(!set->DeleteOldSerialNumbers(""))
			err_list.AddTail(productID);
		list.GetNext(pos);
	}

	if(!err_list.IsEmpty())
	{
		ShowErrorMessage("reset old serial numbers of", &err_list, FALSE);
		return;
	}
}


void CMainFrame::OnTreeCreatenewstockvoucher() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CGenericOneEditDialog<CString> dlg("", "New Voucher Name", "Create New Voucher");
	dlg.SetLimits("", "", FALSE, "Please enter a name of the voucher to create.");
	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CString voucher_name = dlg.m_val;
		if(gDoc->GetVoucher(voucher_name))
		{
			CString str = "A voucher/list by the name '" + voucher_name + "' already exists. Do you want to overwrite ?";
			int retval = MessageBox(str, "Warning", MB_YESNO | MB_ICONWARNING);
			if(retval == IDNO)
				continue;
		}

		break;
	}

	CString voucher_name = dlg.m_val;
	if(voucher_name.IsEmpty())
		return;

	if(gDoc->GetVoucher(voucher_name))
	{
		if(!gDoc->DeleteVoucher(voucher_name))
		{
			CString errstr = "Could not delete existing voucher '" + voucher_name + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}

	CProductVoucher new_voucher;
	strcpy(new_voucher.m_voucher_name, LPCTSTR(voucher_name));
	new_voucher.m_type = tStock;
	if(!gDoc->AddVoucher(&new_voucher))
	{
		CString errstr = "Could not create stock voucher with name '" + voucher_name + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}

	gTreeView->OnInitialUpdate();
}


void CMainFrame::OnTreeCreatenewexportvoucher() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CGenericOneEditDialog<CString> dlg("", "New Voucher Name", "Create New Voucher");
	dlg.SetLimits("", "", FALSE, "Please enter a name of the voucher to create.");
	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CString voucher_name = dlg.m_val;
		if(gDoc->GetVoucher(voucher_name))
		{
			CString str = "A voucher/list by the name '" + voucher_name + "' already exists. Do you want to overwrite ?";
			int retval = MessageBox(str, "Warning", MB_YESNO | MB_ICONWARNING);
			if(retval == IDNO)
				continue;
		}

		break;
	}

	CString voucher_name = dlg.m_val;
	if(voucher_name.IsEmpty())
		return;

	if(gDoc->GetVoucher(voucher_name))
	{
		if(!gDoc->DeleteVoucher(voucher_name))
		{
			CString errstr = "Could not delete existing voucher '" + voucher_name + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}

	CProductVoucher new_voucher;
	strcpy(new_voucher.m_voucher_name, LPCTSTR(voucher_name));
	new_voucher.m_type = tExport;

	if(CPermissions::IsAllowed(updatevoucherinfo))
	{
		CDetailsDialog detail_dlg(tExport, &new_voucher);
		detail_dlg.DoModal();
	}

	if(!gDoc->AddVoucher(&new_voucher))
	{
		CString errstr = "Could not create export voucher with name '" + voucher_name + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}

	gTreeView->OnInitialUpdate();
}


void CMainFrame::OnTreeCreatenewkundanvoucher() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CGenericOneEditDialog<CString> dlg("", "New Voucher Name", "Create New Voucher");
	dlg.SetLimits("", "", FALSE, "Please enter a name of the voucher to create.");
	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CString voucher_name = dlg.m_val;
		if(gDoc->GetVoucher(voucher_name))
		{
			CString str = "A voucher/list by the name '" + voucher_name + "' already exists. Do you want to overwrite ?";
			int retval = MessageBox(str, "Warning", MB_YESNO | MB_ICONWARNING);
			if(retval == IDNO)
				continue;
		}

		break;
	}

	CString voucher_name = dlg.m_val;
	if(voucher_name.IsEmpty())
		return;

	if(gDoc->GetVoucher(voucher_name))
	{
		if(!gDoc->DeleteVoucher(voucher_name))
		{
			CString errstr = "Could not delete existing voucher '" + voucher_name + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}

	CProductVoucher new_voucher;
	strcpy(new_voucher.m_voucher_name, LPCTSTR(voucher_name));
	new_voucher.m_type = tKundan;
	if(!gDoc->AddVoucher(&new_voucher))
	{
		CString errstr = "Could not create kundan voucher with name '" + voucher_name + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}
	
	gTreeView->OnInitialUpdate();
}


void CMainFrame::OnTreeDeleteselected() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	if(gDoc->m_selected_voucher->IsRootVoucher())
	{
		MessageBox("You cannot perform this operation on the root folder.", "ERROR");
		return;
	}

	CString name = gDoc->m_selected_voucher->m_voucher_name;
	CString warn_msg = "Are you sure you want to delete '" + name + "' ?";
	int retval = MessageBox(warn_msg, "Warning", MB_YESNO | MB_ICONWARNING);
	if(retval == IDNO)
		return;

	if(!gDoc->DeleteVoucher(name))
	{
		CString errstr = "Could not delete voucher with name '" + name + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}

	gTreeView->OnInitialUpdate();
}


void CMainFrame::OnTreeIgnoreselected() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	if(gDoc->m_selected_voucher->IsRootVoucher())
	{
		MessageBox("You cannot perform this operation on the root folder.", "ERROR");
		return;
	}

	CString name = gDoc->m_selected_voucher->m_voucher_name;
	if(gDoc->IsIgnored(name))
		gDoc->DeleteFromIgnoreEntry(name);
	else
		gDoc->AddToIgnoreEntry(name);

	gTreeView->OnInitialUpdate();
}

void CMainFrame::OnTreeDeletesolditems()
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	BEGIN_WAIT;
	
	if(gDoc->m_selected_voucher->IsRootVoucher())
	{
		CList<CProductVoucher *, CProductVoucher *> *vlist = NULL;
		switch(gDoc->m_selected_voucher->m_type)
		{
		case tStock:
			vlist = &(gDoc->m_stock_vouchers);
			break;
		case tExport:
			vlist = &(gDoc->m_export_vouchers);
			break;
		case tKundan:
			vlist = &(gDoc->m_kundan_vouchers);
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", gDoc->m_selected_voucher->m_type);
				errstr = "Invalid voucher type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR;
				return;
			}
		}

		if(!vlist)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		POSITION pos = vlist->GetHeadPosition();
		while(pos)
		{
			CProductVoucher *voucher = vlist->GetAt(pos);
			if(voucher)
			{
				if(!gDoc->IsIgnored(voucher->m_voucher_name))
				{
					if(!voucher->DeleteSoldSets(NULL))
					{
						CString errstr = "Could not delete sold sets in the voucher '" + CString(voucher->m_voucher_name) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
					}

					if(!voucher->DeleteEmptySets(NULL))
					{
						CString errstr = "Could not delete empty sets in the voucher '" + CString(voucher->m_voucher_name) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
					}
				}
			}
			else
				SHOW_INTERNAL_ERROR;

			vlist->GetNext(pos);
		}
	}
	else
	{
		if(!gDoc->m_selected_voucher->DeleteSoldSets(NULL))
		{
			CString errstr = "Could not delete sold sets in the voucher '" + CString(gDoc->m_selected_voucher->m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		if(!gDoc->m_selected_voucher->DeleteEmptySets(NULL))
		{
			CString errstr = "Could not delete sold sets in the voucher '" + CString(gDoc->m_selected_voucher->m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}

	if(!gStockView->RefreshEntries(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not refresh the list view.");

	END_WAIT;
}


void CMainFrame::OnTreeQuerySetID() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CGenericOneEditDialog<CString> dlg("", "ProductID", "Query for ID");
	dlg.SetLimits("", "", FALSE, "Please choose a productID to search for.");

	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CList<SStringPairs, SStringPairs> productlist;
		CString productID = dlg.m_val;
		dlg.m_val = "";
		BOOL actualID = FALSE;

		if(productID.CompareNoCase(CProductItem::ExtractActualID(productID)) == 0)
			actualID = TRUE;

		if(actualID)
		{
			CList<CString, CString> setlist;
			if(gDoc->m_selected_voucher->IsRootVoucher())
			{
				CList<CProductVoucher *, CProductVoucher *> *vlist = NULL;
				switch(gDoc->m_selected_voucher->m_type)
				{
				case tStock:
					vlist = &(gDoc->m_stock_vouchers);
					break;
				case tExport:
					vlist = &(gDoc->m_export_vouchers);
					break;
				case tKundan:
					vlist = &(gDoc->m_kundan_vouchers);
					break;
				default:
					{
						CString errstr;
						errstr.Format("%d", gDoc->m_selected_voucher->m_type);
						errstr = "Invalid voucher type '" + errstr + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return;
					}
				}

				if(!vlist)
				{
					SHOW_INTERNAL_ERROR;
					return;
				}

				POSITION pos = vlist->GetHeadPosition();
				while(pos)
				{
					CProductVoucher *voucher = vlist->GetAt(pos);
					if(voucher)
					{
						if(!gDoc->IsIgnored(voucher->m_voucher_name))
							voucher->GetSetList(productID, &setlist);
					}
					else
						SHOW_INTERNAL_ERROR;

					vlist->GetNext(pos);
				}
			}
			else
			{
				gDoc->m_selected_voucher->GetSetList(productID, &setlist);
			}
			
			while(!setlist.IsEmpty())
			{
				CString set = setlist.RemoveHead();
				CProductVoucher *voucher = NULL;
				CProductSet *actual_set = gDoc->GetSet(set, &voucher);
				if(!actual_set || !voucher)
				{
					CString errstr = "Could not find the set '" + set + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return;
				}

				SStringPairs details;
				details.m_string1 = actual_set->m_product_ID;
				details.m_string2 = voucher->m_voucher_name;
				productlist.AddTail(details);
			}
		}
		else
		{
			CProductVoucher *voucher = NULL;
			CProductSet *set = gDoc->GetSet(productID, &voucher);
			if(set && voucher)
			{
				SStringPairs details;
				details.m_string1 = set->m_product_ID;
				details.m_string2 = voucher->m_voucher_name;
				productlist.AddTail(details);
			}
		}

		if(productlist.GetCount() >= 2)
		{
			CGenericListBoxDialog querydlg(ProductList);
			querydlg.m_itemlist = &productlist;
			querydlg.DoModal();
		}

		if(productlist.GetCount() == 0)
		{
			SHOW_INFO("No product found or chosen to display.");
			continue;
		}

		while(!productlist.IsEmpty())
		{
			SStringPairs details = productlist.RemoveHead();
			if(!OpenForEdit(details.m_string1))
			{
				CList<CString, CString> list;
				list.AddHead(details.m_string1);
				ShowErrorMessage("edit", &list, FALSE);
				continue;
			}
		}

		break;
	}
}


void CMainFrame::OnTreeQuerySetGrossWeight() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	CGenericOneEditDialog<double> dlg(0.0, "ProductID", "Query for Gross Weight");
	dlg.SetLimits(0.0, 0.0, TRUE, "Please choose a value to search for.");

	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			return;

		CList<SStringPairs, SStringPairs> productlist;
		double weight = dlg.m_val;
		dlg.m_val = 0.0;

		if(gDoc->m_selected_voucher->IsRootVoucher())
		{
			CList<CProductVoucher *, CProductVoucher *> *vlist = NULL;
			switch(gDoc->m_selected_voucher->m_type)
			{
			case tStock:
				vlist = &(gDoc->m_stock_vouchers);
				break;
			case tExport:
				vlist = &(gDoc->m_export_vouchers);
				break;
			case tKundan:
				vlist = &(gDoc->m_kundan_vouchers);
				break;
			default:
				{
					CString errstr;
					errstr.Format("%d", gDoc->m_selected_voucher->m_type);
					errstr = "Invalid voucher type '" + errstr + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return;
				}
			}

			if(!vlist)
			{
				SHOW_INTERNAL_ERROR;
				return;
			}

			POSITION pos = vlist->GetHeadPosition();
			while(pos)
			{
				CProductVoucher *voucher = vlist->GetAt(pos);
				if(voucher)
				{
					if(!gDoc->IsIgnored(voucher->m_voucher_name))
					{
						int count = voucher->GetSetCount();
						while(count > 0)
						{
							CProductSet *set = voucher->GetSetAt(count - 1);
							if(set)
							{
								int itemcount = set->GetItemCount();
								while(itemcount > 0)
								{
									CProductItem *item = set->GetItemAt(itemcount - 1);
									if(item)
									{
										if(weight == item->m_gross_weight)
										{
											SStringPairs details;
											details.m_string1 = set->m_product_ID;
											details.m_string2 = voucher->m_voucher_name;
											productlist.AddTail(details);
										}
									}
									else
									{
										CString errstr;
										errstr.Format("%d", itemcount - 1);
										errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(voucher->m_voucher_name) + "'.";
										SHOW_INTERNAL_ERROR_REASON(errstr);
									}

									itemcount--;
								}
							}
							else
							{
								CString errstr;
								errstr.Format("%d", count - 1);
								errstr = "Could not get set at position '" + errstr + "' in voucher '" + CString(voucher->m_voucher_name) + "'.";
								SHOW_INTERNAL_ERROR_REASON(errstr);
							}

							count--;
						}
					}
				}
				else
					SHOW_INTERNAL_ERROR;

				vlist->GetNext(pos);
			}
		}
		else
		{
			int count = gDoc->m_selected_voucher->GetSetCount();
			while(count > 0)
			{
				CProductSet *set = gDoc->m_selected_voucher->GetSetAt(count - 1);
				if(set)
				{
					int itemcount = set->GetItemCount();
					while(itemcount > 0)
					{
						CProductItem *item = set->GetItemAt(itemcount - 1);
						if(item)
						{
							if(weight == item->m_gross_weight)
							{
								SStringPairs details;
								details.m_string1 = set->m_product_ID;
								details.m_string2 = gDoc->m_selected_voucher->m_voucher_name;
								productlist.AddTail(details);
							}
						}
						else
						{
							CString errstr;
							errstr.Format("%d", itemcount -1);
							errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "' in voucher '" + CString(gDoc->m_selected_voucher->m_voucher_name) + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);
							return;
						}

						itemcount--;
					}
				}
				else
				{
					CString errstr;
					errstr.Format("%d", count -1);
					errstr = "Could not get set at position '" + errstr + "' in voucher '" + CString(gDoc->m_selected_voucher->m_voucher_name) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return;
				}

				count--;
			}
		}

		if(productlist.GetCount() >= 2)
		{
			CGenericListBoxDialog querydlg(ProductList);
			querydlg.m_itemlist = &productlist;
			querydlg.DoModal();
		}

		if(productlist.GetCount() == 0)
		{
			MessageBox("No product found or chosen to display.");
			continue;
		}

		while(!productlist.IsEmpty())
		{
			SStringPairs details = productlist.RemoveHead();
			if(!OpenForEdit(details.m_string1))
			{
				CList<CString, CString> list;
				list.AddHead(details.m_string1);
				ShowErrorMessage("edit", &list, FALSE);
				continue;
			}
		}

		break;
	}
}


void CMainFrame::OnTreeUpdateinformation() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	switch(gDoc->m_selected_voucher->m_type)
	{
	case tStock:
		{
			CDetailsDialog dlg(tStock, gDoc->m_selected_voucher);
			if(dlg.DoModal() == IDCANCEL)
				return;
		}
		break;
	case tExport:
		{
			CDetailsDialog dlg(tExport, gDoc->m_selected_voucher);
			if(dlg.DoModal() == IDCANCEL)
				return;
		}
		break;
	case tKundan:
		{
			CDetailsDialog dlg(tKundan, gDoc->m_selected_voucher);
			if(dlg.DoModal() == IDCANCEL)
				return;
		}
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", gDoc->m_selected_voucher->m_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}
}


void CMainFrame::OnTreeReallocateserialnumbers() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	if(gDoc->m_selected_voucher->IsRootVoucher())
	{
		SHOW_ERROR("You cannot perform this operation on the root folder.");
		return;
	}

	if(gDoc->m_selected_voucher->m_type != tExport)
	{
		SHOW_ERROR("You can perform this operation on export vouchers only.");
		return;
	}

	CGenericOneEditDialog<int> dlg(1, "Enter starting serial number", "Serial Number");
	dlg.SetLimits(1, 1, FALSE, "Please enter a serial number greater than 0.");
	if(dlg.DoModal() == IDCANCEL)
		return;

	int serial = dlg.m_val;
	CString voucher_name = gDoc->m_selected_voucher->m_voucher_name;
	CString msg = "Are you sure you want to reset the serial numbers of '" + voucher_name + "' ?";
	int retval = 0;
	SHOW_WARNING_YESNO(msg, retval);
	if(retval == IDNO)
		return;

	BEGIN_WAIT;

	if(!gDoc->m_selected_voucher->AssignSerialNumbers(serial))
	{
		CString errstr = "Could not assign serial number to the voucher '" + CString(gDoc->m_selected_voucher->m_voucher_name) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}
	else if(!gStockView->RefreshEntries(FALSE))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not refresh the list view.");
	}

	END_WAIT;
}


void CMainFrame::OnTreeMarkitemassoldOnproductid() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	GenericTwoEditProperties prop;
	CString dlg_countryname;
	switch(gDoc->m_selected_voucher->m_type)
	{
	case tStock:
		prop = ReadOnly_Prop;
		dlg_countryname = "India";
		break;
	case tKundan:
		prop = ReadOnly_Prop;
		dlg_countryname = "India";
		break;
	case tExport:
		prop = Simple_Prop;
		dlg_countryname = "";
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", gDoc->m_selected_voucher->m_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return;
		}
	}

	CList<SSoldProductID, SSoldProductID> finallist;
	CList<CString, CString> errlist;

	CGenericTwoEditDialog<CString, CString> dlg("", "Sold ProductID", dlg_countryname, "Sold in Country", "Mark Sold ProductID");
	dlg.SetProperties(Simple_Prop, prop);
	dlg.SetLimits1("", "", FALSE, "Please choose a productID to mark as sold.");
	dlg.SetLimits2("", "", FALSE, "Please choose a country name.");

	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			break;

		CList<SStringPairs, SStringPairs> productlist;
		BOOL asSet = FALSE;
		BOOL actualID = FALSE;
		CString productID = dlg.m_val_1;
		CString countryname = dlg.m_val_2;
		(dlg.m_val_1).Empty();

		if(productID.CompareNoCase(CProductItem::ExtractActualID(productID)) == 0)
			actualID = TRUE;

		if(actualID)
		{
			CList<CString, CString> setlist;
			if(gDoc->m_selected_voucher->IsRootVoucher())
			{
				CList<CProductVoucher *, CProductVoucher *> *vlist = NULL;
				switch(gDoc->m_selected_voucher->m_type)
				{
				case tStock:
					vlist = &(gDoc->m_stock_vouchers);
					break;
				case tExport:
					vlist = &(gDoc->m_export_vouchers);
					break;
				case tKundan:
					vlist = &(gDoc->m_kundan_vouchers);
					break;
				default:
					{
						CString errstr;
						errstr.Format("%d", gDoc->m_selected_voucher->m_type);
						errstr = "Invalid voucher type '" + errstr + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
						return;
					}
				}

				if(!vlist)
				{
					SHOW_INTERNAL_ERROR;
					return;
				}

				POSITION pos = vlist->GetHeadPosition();
				while(pos)
				{
					CProductVoucher *voucher = vlist->GetAt(pos);
					if(voucher)
					{
						if(!gDoc->IsIgnored(voucher->m_voucher_name))
							voucher->GetSetList(productID, &setlist);
					}
					else
						SHOW_INTERNAL_ERROR;

					vlist->GetNext(pos);
				}
			}
			else
			{
				gDoc->m_selected_voucher->GetSetList(productID, &setlist);
			}

			while(!setlist.IsEmpty())
			{
				CString set = setlist.RemoveHead();
				CProductVoucher *voucher = NULL;
				CProductSet *actual_set = gDoc->GetSet(set, &voucher);
				if(!actual_set || !voucher)
				{
					CString errstr = "Could not find set '" + set + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return;
				}

				SStringPairs details;
				details.m_string1 = actual_set->m_product_ID;
				details.m_string2 = voucher->m_voucher_name;
				productlist.AddTail(details);
			}

			asSet = TRUE;
		}
		else
		{
			CProductVoucher *voucher = NULL;
			CProductSet *set = gDoc->GetSet(productID, &voucher);
			if(set && voucher)
			{
				asSet = TRUE;
				SStringPairs details;
				details.m_string1 = set->m_product_ID;
				details.m_string2 = voucher->m_voucher_name;
				productlist.AddTail(details);
			}
			else
			{
				CProductItem *item = NULL;
				if(gDoc->m_selected_voucher->IsRootVoucher())
				{
					CList<CProductVoucher *, CProductVoucher *> *vlist = NULL;
					switch(gDoc->m_selected_voucher->m_type)
					{
					case tStock:
						vlist = &(gDoc->m_stock_vouchers);
						break;
					case tExport:
						vlist = &(gDoc->m_export_vouchers);
						break;
					case tKundan:
						vlist = &(gDoc->m_kundan_vouchers);
						break;
					default:
						{
							CString errstr;
							errstr.Format("%d", gDoc->m_selected_voucher->m_type);
							errstr = "Invalid voucher type '" + errstr + "'.";
							SHOW_INTERNAL_ERROR_REASON(errstr);
							return;
						}
					}

					if(!vlist)
					{
						SHOW_INTERNAL_ERROR;
						return;
					}

					POSITION pos = vlist->GetHeadPosition();
					while(pos)
					{
						CProductVoucher *voucher = vlist->GetAt(pos);
						if(voucher)
						{
							if(!gDoc->IsIgnored(voucher->m_voucher_name))
							{
								item = voucher->GetItem(productID);
								if(item)
								{
									asSet = FALSE;
									SStringPairs details;
									details.m_string1 = item->m_product_ID;
									details.m_string2 = voucher->m_voucher_name;
									productlist.AddTail(details);
									break;
								}
							}
						}
						else
							SHOW_INTERNAL_ERROR;

						vlist->GetNext(pos);
					}
				}
				else
				{
					item = gDoc->m_selected_voucher->GetItem(productID);
					if(item)
					{
						asSet = FALSE;
						SStringPairs details;
						details.m_string1 = item->m_product_ID;
						details.m_string2 = gDoc->m_selected_voucher->m_voucher_name;
						productlist.AddTail(details);
					}
				}
			}
		}

		if(productlist.GetCount() >= 2)
		{
			CGenericListBoxDialog solddlg(ProductList);
			solddlg.m_itemlist = &productlist;
			solddlg.DoModal();
		}

		if(productlist.GetCount() == 0)
		{
			MessageBox("No product found or chosen to mark as sold.");
			continue;
		}

		while(!productlist.IsEmpty())
		{
			SStringPairs details = productlist.RemoveHead();
			SSoldProductID node;
			node.ID = details.m_string1;
			node.voucher_name = details.m_string2;
			node.country = countryname;
			node.set = asSet;

			finallist.AddTail(node);
		}
	}

	while(!finallist.IsEmpty())
	{
		SSoldProductID node = finallist.RemoveHead();
		CProductVoucher *voucher = gDoc->GetVoucher(node.voucher_name);
		if(voucher)
		{
			if(!voucher->MarkAsSold(node.ID, TRUE, node.country, node.set))
				errlist.AddTail(node.ID);
		}
		else
			errlist.AddTail(node.ID);
	}

	if(!errlist.IsEmpty())
		ShowErrorMessage("mark as sold", &errlist, FALSE);

	if(!gStockView->RefreshEntries(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not refresh the list view.");
}

void CMainFrame::OnTreeMarkitemassoldOnserialnumber() 
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	if(gDoc->m_selected_voucher->IsRootVoucher())
	{
		SHOW_ERROR("You cannot perform this operation on the root folder.");
		return;
	}

	if(gDoc->m_selected_voucher->m_type != tExport)
	{
		SHOW_ERROR("You can perform this operation on export vouchers only.");
		return;
	}

	CList<SSoldSerial, SSoldSerial> finallist;
	CList<CString, CString> errlist;

	CGenericTwoEditDialog<int, CString> dlg(0, "Sold Serial Number", "", "Sold in Country", "Mark Sold Serial");
	dlg.SetLimits1(0, 0, TRUE, "Please choose a serial number to mark as sold.");
	dlg.SetLimits2("", "", FALSE, "Please choose a country name.");

	while(TRUE)
	{
		if(dlg.DoModal() == IDCANCEL)
			break;

		int serial_number = dlg.m_val_1;
		CString countryname = dlg.m_val_2;
		dlg.m_val_1 = 0;

		CProductItem *item = NULL;
		if(gDoc->m_selected_voucher->IsRootVoucher())
		{
			CList<CProductVoucher *, CProductVoucher *> *vlist = NULL;
			switch(gDoc->m_selected_voucher->m_type)
			{
			case tStock:
				vlist = &(gDoc->m_stock_vouchers);
				break;
			case tExport:
				vlist = &(gDoc->m_export_vouchers);
				break;
			case tKundan:
				vlist = &(gDoc->m_kundan_vouchers);
				break;
			default:
				{
					CString errstr;
					errstr.Format("%d", gDoc->m_selected_voucher->m_type);
					errstr = "Invalid voucher type '" + errstr + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return;
				}
			}

			if(!vlist)
			{
				SHOW_INTERNAL_ERROR;
				return;
			}

			POSITION pos = vlist->GetHeadPosition();
			while(pos)
			{
				CProductVoucher *voucher = vlist->GetAt(pos);
				if(voucher)
				{
					if(!gDoc->IsIgnored(voucher->m_voucher_name))
					{
						item = voucher->GetItem(serial_number);
						if(item)
						{
							SSoldSerial node;
							node.serial = serial_number;
							node.ID = item->m_product_ID;
							node.country = countryname;
							node.voucher_name = voucher->m_voucher_name;
							finallist.AddTail(node);
						}
					}
				}
				else
					SHOW_INTERNAL_ERROR;

				vlist->GetNext(pos);
			}
		}
		else
		{
			item = gDoc->m_selected_voucher->GetItem(serial_number);
			if(item)
			{
				SSoldSerial node;
				node.serial = serial_number;
				node.ID = item->m_product_ID;
				node.country = countryname;
				node.voucher_name = gDoc->m_selected_voucher->m_voucher_name;
				finallist.AddTail(node);
			}
		}

		if(!item)
		{
			CString errstr;
			errstr.Format("%d", serial_number);
			errstr = "Could not find item with serial number '" + errstr + "'.";
			SHOW_ERROR(errstr);
			continue;
		}
	}

	while(!finallist.IsEmpty())
	{
		SSoldSerial node = finallist.RemoveHead();
		CProductVoucher *voucher = gDoc->GetVoucher(node.voucher_name);
		if(voucher)
		{
			if(!voucher->MarkAsSold(node.serial, TRUE, node.country))
				errlist.AddTail(node.ID);
		}
		else
			errlist.AddTail(node.ID);
	}

	if(!errlist.IsEmpty())
		ShowErrorMessage("mark as sold", &errlist, FALSE);

	if(!gStockView->RefreshEntries(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not refresh the list view.");
}


void CMainFrame::OnReportsCatalogue() 
{
}


void CMainFrame::OnReportsStock(UINT nID)
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	if(gDoc->m_selected_voucher->m_type != tStock)
	{
		SHOW_ERROR("You can perform this operation on stock vouchers only.");
		return;
	}

	COptionsDialog option_dlg;
	CGenericTwoEditDialog<int, int> tag_dlg(1, "Start Row", 1, "Start Column", "Tag Position");
	tag_dlg.SetLimits1(1, 18, FALSE, "Incorrect Row Value");
	tag_dlg.SetLimits2(1, 6, FALSE, "Incorrect Column Value");

	if(nID == ID_REPORTS_STOCK_TAGS)
	{
		if(tag_dlg.DoModal() == IDCANCEL)
			return;

		if(tag_dlg.m_val_2 == 2)
			tag_dlg.m_val_2 = 3;
		else if(tag_dlg.m_val_2 == 4)
			tag_dlg.m_val_2 = 5;
		else if(tag_dlg.m_val_2 == 6)
		{
			tag_dlg.m_val_2 = 1;
			tag_dlg.m_val_1++;

			if(tag_dlg.m_val_1 > 18)
				tag_dlg.m_val_1 = 1;
		}
	}

	if(option_dlg.DoModal() == IDCANCEL)
		return;

	CGenericTwoEditDialog<int, int> image_size_dlg(30, "Image Width", 150, "Image Height", "Image Size for Photo Invoice");
	tag_dlg.SetLimits1(1, 1000, FALSE, "Width should be between 1 and 1000");
	tag_dlg.SetLimits2(1, 1000, FALSE, "Height should be between 1 and 1000");

	int image_width = 30;
	int image_height = 150;
	
	if((nID == ID_REPORTS_STOCK_PHOTOINVOICE_ORIGINAL) ||
	   (nID == ID_REPORTS_STOCK_PHOTOINVOICE_SOLD) ||
	   (nID == ID_REPORTS_STOCK_PHOTOINVOICE_UNSOLD))
	{
		if(image_size_dlg.DoModal() == IDCANCEL)
			return;

		image_width = image_size_dlg.m_val_1;
		image_height = image_size_dlg.m_val_2;
	}

	CGenericOneEditDialog<int> rows_in_a_page_dlg(55, "Rows per page in Price Quotation", "Rows per page");
	rows_in_a_page_dlg.SetLimits(1, 1, FALSE, "Please enter the number of rows greater than 0.");
	int rows_in_a_page = 55;

	if((nID == ID_REPORTS_STOCK_PRICEQUOTATION_ORIGINAL) ||
	   (nID == ID_REPORTS_STOCK_PRICEQUOTATION_SOLD) ||
	   (nID == ID_REPORTS_STOCK_PRICEQUOTATION_UNSOLD))
	{
		if(rows_in_a_page_dlg.DoModal() == IDCANCEL)
			return;

		rows_in_a_page = rows_in_a_page_dlg.m_val;
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
		report.m_image_width = image_width;
		report.m_image_height = image_height;
		report.m_filename = option_dlg.m_filename;
		report.m_rows_in_a_page = rows_in_a_page;
		report.m_show_diamond_clarity = (MessageBox("Do you want 'Diamond Clarity' to be displayed?", "", MB_YESNO) == IDYES);

		switch(nID)
		{
		case ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_ORIGINAL:
			report.m_status = report.CreateStocksReportActual(Original_Report);
			break;
		case ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_SOLD:
			report.m_status = report.CreateStocksReportActual(Sold_Report);
			break;
		case ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_UNSOLD:
			report.m_status = report.CreateStocksReportActual(UnSold_Report);
			break;
		case ID_REPORTS_STOCK_STOCKSREPORT_MARKED_ORIGINAL:
			report.m_status = report.CreateStocksReportMarked(Original_Report);
			break;
		case ID_REPORTS_STOCK_STOCKSREPORT_MARKED_SOLD:
			report.m_status = report.CreateStocksReportMarked(Sold_Report);
			break;
		case ID_REPORTS_STOCK_STOCKSREPORT_MARKED_UNSOLD:
			report.m_status = report.CreateStocksReportMarked(UnSold_Report);
			break;
		case ID_REPORTS_STOCK_PHOTOINVOICE_ORIGINAL:
			report.m_status = report.CreateStockPhotoInvoice(Original_Report);
			break;
		case ID_REPORTS_STOCK_PHOTOINVOICE_SOLD:
			report.m_status = report.CreateStockPhotoInvoice(Sold_Report);
			break;
		case ID_REPORTS_STOCK_PHOTOINVOICE_UNSOLD:
			report.m_status = report.CreateStockPhotoInvoice(UnSold_Report);
			break;
		case ID_REPORTS_STOCK_PRICEQUOTATION_ORIGINAL:
			report.m_status = report.CreateStockPriceQuotation(Original_Report);
			break;
		case ID_REPORTS_STOCK_PRICEQUOTATION_SOLD:
			report.m_status = report.CreateStockPriceQuotation(Sold_Report);
			break;
		case ID_REPORTS_STOCK_PRICEQUOTATION_UNSOLD:
			report.m_status = report.CreateStockPriceQuotation(UnSold_Report);
			break;
		case ID_REPORTS_STOCK_CHECKLIST_ORIGINAL:
			report.m_status = report.CreateStockChecklist(Original_Report);
			break;
		case ID_REPORTS_STOCK_CHECKLIST_SOLD:
			report.m_status = report.CreateStockChecklist(Sold_Report);
			break;
		case ID_REPORTS_STOCK_CHECKLIST_UNSOLD:
			report.m_status = report.CreateStockChecklist(UnSold_Report);
			break;
		case ID_REPORTS_STOCK_TAGS:
			report.m_status = report.CreateStockTags(tag_dlg.m_val_1, tag_dlg.m_val_2);
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", nID);
				errstr = "Invalid stock report type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return;
			}
		}
	}

	END_WAIT;

	if(report.m_status == TRUE)
		SHOW_INFO("Done processing report. Press OK to display/print the result.");
}


void CMainFrame::OnReportsKundan(UINT nID)
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	if(gDoc->m_selected_voucher->m_type != tKundan)
	{
		SHOW_ERROR("You can perform this operation on kundan vouchers only.");
		return;
	}

	COptionsDialog option_dlg;
	CGenericTwoEditDialog<int, int> tag_dlg(1, "Start Row", 1, "Start Column", "Tag Position");
	tag_dlg.SetLimits1(1, 18, FALSE, "Incorrect Row Value");
	tag_dlg.SetLimits2(1, 6, FALSE, "Incorrect Column Value");

	if(nID == ID_REPORTS_KUNDAN_TAGS)
	{
		if(tag_dlg.DoModal() == IDCANCEL)
			return;

		if(tag_dlg.m_val_2 == 2)
			tag_dlg.m_val_2 = 3;
		else if(tag_dlg.m_val_2 == 4)
			tag_dlg.m_val_2 = 5;
		else if(tag_dlg.m_val_2 == 6)
		{
			tag_dlg.m_val_2 = 1;
			tag_dlg.m_val_1++;

			if(tag_dlg.m_val_1 > 18)
				tag_dlg.m_val_1 = 1;
		}
	}

	if(option_dlg.DoModal() == IDCANCEL)
		return;

	CGenericOneEditDialog<int> rows_in_a_page_dlg(55, "Rows per page in Price Quotation", "Rows per page");
	rows_in_a_page_dlg.SetLimits(1, 1, FALSE, "Please enter the number of rows greater than 0.");
	int rows_in_a_page = 55;

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
		report.m_filename = option_dlg.m_filename;
		report.m_rows_in_a_page = rows_in_a_page;

		switch(nID)
		{
		case ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_ORIGINAL:
			report.m_status = report.CreateKundanReportActual(Original_Report);
			break;
		case ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_SOLD:
			report.m_status = report.CreateKundanReportActual(Sold_Report);
			break;
		case ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_UNSOLD:
			report.m_status = report.CreateKundanReportActual(UnSold_Report);
			break;
		case ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_ORIGINAL:
			report.m_status = report.CreateKundanReportMarked(Original_Report);
			break;
		case ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_SOLD:
			report.m_status = report.CreateKundanReportMarked(Sold_Report);
			break;
		case ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_UNSOLD:
			report.m_status = report.CreateKundanReportMarked(UnSold_Report);
			break;
		case ID_REPORTS_KUNDAN_CHECKLIST_ORIGINAL:
			report.m_status = report.CreateKundanChecklist(Original_Report);
			break;
		case ID_REPORTS_KUNDAN_CHECKLIST_SOLD:
			report.m_status = report.CreateKundanChecklist(Sold_Report);
			break;
		case ID_REPORTS_KUNDAN_CHECKLIST_UNSOLD:
			report.m_status = report.CreateKundanChecklist(UnSold_Report);
			break;
		case ID_REPORTS_KUNDAN_TAGS:
			report.m_status = report.CreateKundanTags(tag_dlg.m_val_1, tag_dlg.m_val_2);
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", nID);
				errstr = "Invalid kundan report type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return;
			}
		}
	}

	END_WAIT;

	if(report.m_status == TRUE)
		SHOW_INFO("Done processing report. Press OK to display/print the result.");
}


void CMainFrame::OnReportsExport(UINT nID)
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return;
	}

	if(gDoc->m_selected_voucher->m_type != tExport)
	{
		SHOW_ERROR("You can perform this operation on export vouchers only.");
		return;
	}

	CExportSerialsDialog serials_dlg;
	COptionsDialog option_dlg;
	CGenericTwoEditDialog<int, int> tag_dlg(1, "Start Row", 1, "Start Column", "Tag Position");
	tag_dlg.SetLimits1(1, 18, FALSE, "Incorrect Row Value");
	tag_dlg.SetLimits2(1, 6, FALSE, "Incorrect Column Value");

	if(serials_dlg.DoModal() == IDCANCEL)
		return;

	if(nID == ID_REPORTS_EXPORT_TAGS)
	{
		if(tag_dlg.DoModal() == IDCANCEL)
			return;

		if(tag_dlg.m_val_2 == 2)
			tag_dlg.m_val_2 = 3;
		else if(tag_dlg.m_val_2 == 4)
			tag_dlg.m_val_2 = 5;
		else if(tag_dlg.m_val_2 == 6)
		{
			tag_dlg.m_val_2 = 1;
			tag_dlg.m_val_1++;

			if(tag_dlg.m_val_1 > 18)
				tag_dlg.m_val_1 = 1;
		}
	}

	if(option_dlg.DoModal() == IDCANCEL)
		return;

	CGenericTwoEditDialog<int, int> image_size_dlg(30, "Image Width", 150, "Image Height", "Image Size for Photo Invoice");
	tag_dlg.SetLimits1(1, 1000, FALSE, "Width should be between 1 and 1000");
	tag_dlg.SetLimits2(1, 1000, FALSE, "Height should be between 1 and 1000");

	int image_width = 30;
	int image_height = 150;
	
	if((nID == ID_REPORTS_EXPORT_PHOTOINVOICE_ORIGINAL) ||
	   (nID == ID_REPORTS_EXPORT_PHOTOINVOICE_SOLD) ||
	   (nID == ID_REPORTS_EXPORT_PHOTOINVOICE_UNSOLD))
	{
		if(image_size_dlg.DoModal() == IDCANCEL)
			return;

		image_width = image_size_dlg.m_val_1;
		image_height = image_size_dlg.m_val_2;
	}

	CGenericOneEditDialog<int> rows_in_a_page_dlg(55, "Rows per page in Price Quotation", "Rows per page");
	rows_in_a_page_dlg.SetLimits(1, 1, FALSE, "Please enter the number of rows greater than 0.");
	int rows_in_a_page = 55;

	if((nID == ID_REPORTS_EXPORT_PRICEQUOTATION_ORIGINAL) ||
	   (nID == ID_REPORTS_EXPORT_PRICEQUOTATION_SOLD) ||
	   (nID == ID_REPORTS_EXPORT_PRICEQUOTATION_UNSOLD))
	{
		if(rows_in_a_page_dlg.DoModal() == IDCANCEL)
			return;

		rows_in_a_page = rows_in_a_page_dlg.m_val;
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
		report.m_serials_from = serials_dlg.m_from;
		report.m_serials_to = serials_dlg.m_to;
		report.m_use_product_id = serials_dlg.m_use_productID;
		report.m_use_new_serials = serials_dlg.m_use_new;
		report.m_use_old_serials = serials_dlg.m_use_old;
		report.m_old_serial_tagname = serials_dlg.m_tagname;
		report.m_old_serial_tagnames = &(serials_dlg.m_tagnames);
		report.m_filename = option_dlg.m_filename;
		report.m_image_width = image_width;
		report.m_image_height = image_height;
		report.m_rows_in_a_page = rows_in_a_page;
		report.m_show_diamond_clarity = (MessageBox("Do you want 'Diamond Clarity' to be displayed?", "", MB_YESNO) == IDYES);

		switch(nID)
		{
		case ID_REPORTS_EXPORT_PACKAGELIST_ORIGINAL:
			report.m_status = report.CreateExportPackageList(Original_Report);
			break;
		case ID_REPORTS_EXPORT_PACKAGELIST_SOLD:
			report.m_status = report.CreateExportPackageList(Sold_Report);
			break;
		case ID_REPORTS_EXPORT_PACKAGELIST_UNSOLD:
			report.m_status = report.CreateExportPackageList(UnSold_Report);
			break;
		case ID_REPORTS_EXPORT_SHORTINVOICE_ORIGINAL:
			report.m_status = report.CreateExportShortInvoice(Original_Report);
			break;
		case ID_REPORTS_EXPORT_SHORTINVOICE_SOLD:
			report.m_status = report.CreateExportShortInvoice(Sold_Report);
			break;
		case ID_REPORTS_EXPORT_SHORTINVOICE_UNSOLD:
			report.m_status = report.CreateExportShortInvoice(UnSold_Report);
			break;
		case ID_REPORTS_EXPORT_PHOTOINVOICE_ORIGINAL:
			report.m_status = report.CreateExportPhotoInvoice(Original_Report);
			break;
		case ID_REPORTS_EXPORT_PHOTOINVOICE_SOLD:
			report.m_status = report.CreateExportPhotoInvoice(Sold_Report);
			break;
		case ID_REPORTS_EXPORT_PHOTOINVOICE_UNSOLD:
			report.m_status = report.CreateExportPhotoInvoice(UnSold_Report);
			break;
		case ID_REPORTS_EXPORT_PRICEQUOTATION_ORIGINAL:
			report.m_status = report.CreateExportPriceQuotation(Original_Report);
			break;
		case ID_REPORTS_EXPORT_PRICEQUOTATION_SOLD:
			report.m_status = report.CreateExportPriceQuotation(Sold_Report);
			break;
		case ID_REPORTS_EXPORT_PRICEQUOTATION_UNSOLD:
			report.m_status = report.CreateExportPriceQuotation(UnSold_Report);
			break;
		case ID_REPORTS_EXPORT_CHECKLIST_ORIGINAL:
			report.m_status = report.CreateExportChecklist(Original_Report);
			break;
		case ID_REPORTS_EXPORT_CHECKLIST_SOLD:
			report.m_status = report.CreateExportChecklist(Sold_Report);
			break;
		case ID_REPORTS_EXPORT_CHECKLIST_UNSOLD:
			report.m_status = report.CreateExportChecklist(UnSold_Report);
			break;
		case ID_REPORTS_EXPORT_TAGS:
			report.m_status = report.CreateExportTags(tag_dlg.m_val_1, tag_dlg.m_val_2);
			break;
		case ID_REPORTS_EXPORT_GRFORM:
			report.m_status = report.CreateExportGRForm();
			break;
		default:
			{
				CString errstr;
				errstr.Format("%d", nID);
				errstr = "Invalid export report type '" + errstr + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return;
			}
		}
	}

	END_WAIT;

	if(report.m_status == TRUE)
		SHOW_INFO("Done processing report. Press OK to display/print the result.");
}


void CMainFrame::OnUpdateGeneral(CCmdUI *pCmdUI)
{
	BOOL allowed = TRUE;

	switch(pCmdUI->m_nID)
	{
	case ID_TREE_CREATENEWSTOCKVOUCHER:
	case ID_TREE_CREATENEWEXPORTVOUCHER:
	case ID_TREE_CREATENEWKUNDANVOUCHER:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(createvoucher));
		break;

	case ID_TREE_DELETESELECTED:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(deletevoucher));
		allowed = (allowed && (!gDoc->m_selected_voucher->IsRootVoucher()));
		break;

	case ID_TREE_IGNORESELECTED:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && (!gDoc->m_selected_voucher->IsRootVoucher()));
		if(allowed)
		{
			CString name = gDoc->m_selected_voucher->m_voucher_name;
			pCmdUI->SetCheck(gDoc->IsIgnored(name));
		}
		break;

	case ID_TREE_UPDATEINFORMATION:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(updatevoucherinfo));
		break;

	case ID_TREE_MARKITEMASSOLD_ONSERIALNUMBER:
	case ID_TREE_REALLOCATESERIALNUMBERS:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(editset));
		allowed = (allowed && (!gDoc->m_selected_voucher->IsRootVoucher()));
		allowed = (allowed && (gDoc->m_selected_voucher->m_type == tExport));
		allowed = (allowed && (gStockView->GetItemCount() != 0));
		break;

	case ID_TREE_MARKITEMASSOLD_ONPRODUCTID:
	case ID_SET_CHANGEPRODUCTID:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(editset));
		allowed = (allowed && (gStockView->GetItemCount() != 0));
		break;

	case ID_TREE_QUERYSET_GROSS_WEIGHT:
	case ID_TREE_QUERYSET_ID:
	case ID_SET_SELECTALL:
	case ID_SET_UNSELECTALL:
	case ID_REPORTS_CATALOGUE:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && (gStockView->GetItemCount() != 0));
		break;

	case ID_SET_ADDNEW:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(addset));
		allowed = (allowed && (gDoc->m_selected_voucher->m_type != tExport));
		allowed = (allowed && (!gDoc->m_stock_vouchers.IsEmpty()));
		break;

	case ID_SET_EDIT:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && (gStockView->GetItemCount() != 0));
		if(allowed)
		{
			int index;
			gStockView->GetSelectedProduct(&index);
			allowed = (allowed && (index >= 0));
		}
		break;

	case ID_TREE_DELETESOLDITEMS:
	case ID_SET_DELETE_CURRENT:
	case ID_SET_DELETE_CHECKED:
	case ID_SET_DELETE_HIGHLIGHTED:
	case ID_SET_DELETECHECKED_HIGHLIGHTED:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(deleteset));
		allowed = (allowed && (gStockView->GetItemCount() != 0));
		if(allowed)
		{
			switch(pCmdUI->m_nID)
			{
			case ID_SET_DELETE_CURRENT:
				{
					int index;
					gStockView->GetSelectedProduct(&index);
					allowed = (allowed && (index >= 0));
				}
				break;
			case ID_SET_DELETE_CHECKED:
				{
					CList<CString, CString> list;
					gStockView->GetCheckedList(&list);
					allowed = (allowed && (!list.IsEmpty()));
				}
				break;
			case ID_SET_DELETE_HIGHLIGHTED:
				{
					CList<CString, CString> list;
					gStockView->GetHighlightedList(&list);
					allowed = (allowed && (!list.IsEmpty()));
				}
				break;
			case ID_SET_DELETECHECKED_HIGHLIGHTED:
				{
					CList<CString, CString> list;
					gStockView->GetCheckedList(&list);
					gStockView->GetHighlightedList(&list);
					allowed = (allowed && (!list.IsEmpty()));
				}
				break;
			case ID_TREE_DELETESOLDITEMS:
				break;
			}
		}
		break;

	case ID_SET_RESETOLDSERIALS:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(editset));
		if(allowed)
		{
			CList<CString, CString> list;
			gStockView->GetCheckedList(&list);
			allowed = (allowed && ((!list.IsEmpty()) || (gStockView->GetItemCount() != 0)));
		}

	case ID_SET_MOVE_CREATENEW:
		allowed = (allowed && CPermissions::IsAllowed(createvoucher));
	case ID_SET_MOVE_ADDTOEXISTING:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(moveset));
		if(allowed)
		{
			CList<CString, CString> list;
			gStockView->GetCheckedList(&list);
			allowed = (allowed && ((!list.IsEmpty()) || (gStockView->GetItemCount() != 0)));
		}
		break;

	case ID_SET_SELECTHIGHLIGHTED:
	case ID_SET_UNSELECTHIGHLIGHTED:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && (gStockView->GetItemCount() != 0));
		if(allowed)
		{
			CList<CString, CString> list;
			gStockView->GetHighlightedList(&list);
			allowed = (allowed && (!list.IsEmpty()));
		}
		break;

	case ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_ORIGINAL:
	case ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_SOLD:
	case ID_REPORTS_STOCK_STOCKSREPORT_ACTUALS_UNSOLD:
	case ID_REPORTS_STOCK_STOCKSREPORT_MARKED_ORIGINAL:
	case ID_REPORTS_STOCK_STOCKSREPORT_MARKED_SOLD:
	case ID_REPORTS_STOCK_STOCKSREPORT_MARKED_UNSOLD:
	case ID_REPORTS_STOCK_PHOTOINVOICE_ORIGINAL:
	case ID_REPORTS_STOCK_PHOTOINVOICE_SOLD:
	case ID_REPORTS_STOCK_PHOTOINVOICE_UNSOLD:
	case ID_REPORTS_STOCK_PRICEQUOTATION_ORIGINAL:
	case ID_REPORTS_STOCK_PRICEQUOTATION_SOLD:
	case ID_REPORTS_STOCK_PRICEQUOTATION_UNSOLD:
	case ID_REPORTS_STOCK_CHECKLIST_ORIGINAL:
	case ID_REPORTS_STOCK_CHECKLIST_SOLD:
	case ID_REPORTS_STOCK_CHECKLIST_UNSOLD:
	case ID_REPORTS_STOCK_TAGS:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(stockreports));
		allowed = (allowed && (gDoc->m_selected_voucher->m_type == tStock));
		allowed = (allowed && (gStockView->GetItemCount() != 0));
		break;

	case ID_REPORTS_EXPORT_PACKAGELIST_ORIGINAL:
	case ID_REPORTS_EXPORT_PACKAGELIST_SOLD:
	case ID_REPORTS_EXPORT_PACKAGELIST_UNSOLD:
	case ID_REPORTS_EXPORT_SHORTINVOICE_ORIGINAL:
	case ID_REPORTS_EXPORT_SHORTINVOICE_SOLD:
	case ID_REPORTS_EXPORT_SHORTINVOICE_UNSOLD:
	case ID_REPORTS_EXPORT_PHOTOINVOICE_ORIGINAL:
	case ID_REPORTS_EXPORT_PHOTOINVOICE_SOLD:
	case ID_REPORTS_EXPORT_PHOTOINVOICE_UNSOLD:
	case ID_REPORTS_EXPORT_PRICEQUOTATION_ORIGINAL:
	case ID_REPORTS_EXPORT_PRICEQUOTATION_SOLD:
	case ID_REPORTS_EXPORT_PRICEQUOTATION_UNSOLD:
	case ID_REPORTS_EXPORT_CHECKLIST_ORIGINAL:
	case ID_REPORTS_EXPORT_CHECKLIST_SOLD:
	case ID_REPORTS_EXPORT_CHECKLIST_UNSOLD:
	case ID_REPORTS_EXPORT_TAGS:
	case ID_REPORTS_EXPORT_GRFORM:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(exportreports));
		allowed = (allowed && (gDoc->m_selected_voucher->m_type == tExport));
		allowed = (allowed && (gStockView->GetItemCount() != 0));
		break;

	case ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_ORIGINAL:
	case ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_SOLD:
	case ID_REPORTS_KUNDAN_STOCKSREPORT_ACTUALS_UNSOLD:
	case ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_ORIGINAL:
	case ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_SOLD:
	case ID_REPORTS_KUNDAN_STOCKSREPORT_MARKED_UNSOLD:
	case ID_REPORTS_KUNDAN_CHECKLIST_ORIGINAL:
	case ID_REPORTS_KUNDAN_CHECKLIST_SOLD:
	case ID_REPORTS_KUNDAN_CHECKLIST_UNSOLD:
	case ID_REPORTS_KUNDAN_TAGS:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(kundanreports));
		allowed = (allowed && (gDoc->m_selected_voucher->m_type == tKundan));
		allowed = (allowed && (gStockView->GetItemCount() != 0));
		break;

	case ID_USINGFOREXPORTS_NATIVECURRENCY:
		{
			if(CSettings::GetUseNativeCurrency())
				pCmdUI->SetCheck(TRUE);
			else
				pCmdUI->SetCheck(FALSE);
		}

		allowed = (allowed && CPermissions::IsAllowed(updateusingcurrency));
		break;

	case ID_USINGFOREXPORTS_DOLLARUSD:
		{
			if(CSettings::GetUseNativeCurrency())
				pCmdUI->SetCheck(FALSE);
			else
				pCmdUI->SetCheck(TRUE);
		}

		allowed = (allowed && CPermissions::IsAllowed(updateusingcurrency));
		break;

	case ID_SETTINGS_CHANGEFILEPATH:
		allowed = (allowed && MakeGlobalsValid());
		allowed = (allowed && CPermissions::IsAllowed(changefilepath));
		break;

	case ID_SETTINGS_UPDATEGOLDRATE:
		allowed = (allowed && CPermissions::IsAllowed(updategoldrate));
		break;

	case ID_SETTINGS_MANAGEDESCRIPTIONDATABASE:
		allowed = (allowed && CPermissions::IsAllowed(updatedescriptiondatabase));
		break;

	case ID_USER_ADD:
		allowed = (allowed && CPermissions::IsAllowed(adduser));
		break;

	case ID_USER_DELETE:
		allowed = (allowed && CPermissions::IsAllowed(deleteuser));
		break;

	case ID_USER_EDIT_PERM:
		allowed = (allowed && CPermissions::IsAllowed(edituser));
		break;

	case ID_USER_CHANGE_PASSWORD:
		//allowed = (allowed && (!CPermissions::IsAdmin()));
		break;
	
	case ID_SETTINGS_BACKUP:
		allowed = TRUE;
		break;

	default:
		allowed = FALSE;
		break;
	}

	pCmdUI->Enable(allowed);
}


BOOL CMainFrame::OpenForEdit(CString productID)
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return FALSE;
	}

	if(productID.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CProductVoucher *voucher = NULL;
	CProductSet *set = gDoc->GetSet(productID, &voucher);
	if(!set)
	{
		CString errstr = "Could not find set '" + productID + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	CProductSet newset = *set;
	static BOOL shownonce = FALSE;
	if(!CPermissions::IsAllowed(editset) && !shownonce)
	{
		SHOW_ERROR("You do not have permissions to edit sets. The changes you make will not be saved.");
		shownonce = TRUE;
	}

	double gold_rate = GetGoldRate(voucher);
	int dialog_retval;

	switch(gDoc->m_selected_voucher->m_type)
	{
	case tStock:
		{
			CAddDialog dlg(tStock, &newset, gold_rate);
			dialog_retval = dlg.DoModal();
		}
		break;
	case tKundan:
		{
			CAddDialog dlg(tKundan, &newset, gold_rate);
			dialog_retval = dlg.DoModal();
		}
		break;
	case tExport:
		{
			int serial = voucher->GetDesiredSerialNumber();

			double conversion;
			if(CSettings::GetUseNativeCurrency())
				conversion = voucher->m_currency_conversion;
			else
				conversion = voucher->m_dollar_rate;

			CAddDialog dlg(tExport, &newset, gold_rate, conversion, voucher->m_currency_name, serial);
			dialog_retval = dlg.DoModal();
		}
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", gDoc->m_selected_voucher->m_type);
			errstr = "Invalid voucher type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	if(!CPermissions::IsAllowed(editset))
		dialog_retval = IDCANCEL;

	if(dialog_retval == IDOK)
	{
		BOOL needSorting = FALSE;
		if(strcmp(LPCTSTR(productID),newset.m_product_ID) != 0)
			needSorting = TRUE;

		if(!voucher->UpdateSet(productID, &newset))
		{
			CString errstr = "Could not update the set '" + productID + "' in voucher '" + CString(voucher->m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}

		if(needSorting)
			voucher->SortDatabase();

		if(!gStockView->RefreshEntries(FALSE))
			SHOW_INTERNAL_ERROR_REASON("Could not refresh the list view.");
	}

	return TRUE;
}


BOOL CMainFrame::DeleteSets(CList<CString, CString> *list, BOOL force)
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return FALSE;
	}

	if(!list)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(list->IsEmpty())
		return TRUE;

	if(!force)
	{
		int retval = 0;
		SHOW_WARNING_YESNO("Are you sure you want to delete the selected set/sets?", retval);
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

		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(!set || !voucher)
		{
			list->AddTail(productID);
			err = TRUE;
		}
		else
		{
			if(!voucher->DeleteSet(productID))
			{
				list->AddTail(productID);
				err = TRUE;
			}
		}

		progress.ProgressForward();
		count--;
	}

	progress.FinishDialog();
	if(!gStockView->RefreshEntries(FALSE))
		SHOW_INTERNAL_ERROR_REASON("Could not refresh the list view.");

	return TRUE;
}


BOOL CMainFrame::MoveSetsTo(CList<CString, CString> *pendinglist, CList<CString, CString> *successlist, CString voucher_name, DatabaseType type, BOOL createnew)
{
	if(!MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR_REASON("Global values invalid");
		return FALSE;
	}

	if(voucher_name.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!pendinglist || !successlist)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(pendinglist->IsEmpty())
		return TRUE;

	if(createnew)
	{
		if(gDoc->GetVoucher(voucher_name))
		{
			if(!gDoc->DeleteVoucher(voucher_name))
			{
				CString errstr = "Could not delete voucher '" + voucher_name + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
				return FALSE;
			}
		}

		CProductVoucher new_voucher;
		strcpy(new_voucher.m_voucher_name, LPCTSTR(voucher_name));
		new_voucher.m_type = type;

		if(!gDoc->AddVoucher(&new_voucher))
		{
			CString errstr = "Could not create voucher '" + voucher_name + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}

	CProductVoucher *dest_voucher = gDoc->GetVoucher(voucher_name);
	if(!dest_voucher)
	{
		CString errstr = "Could not find voucher '" + voucher_name + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(createnew && (type == tExport))
	{	
		if(CPermissions::IsAllowed(updatevoucherinfo))
		{
			CDetailsDialog dlg(tExport, dest_voucher);
			dlg.DoModal();
		}
	}

	BOOL doSerialAssignment = FALSE;
	int serial = 1;
	int start_serial = 1;
	if(type == tExport)
	{
		start_serial = dest_voucher->GetDesiredSerialNumber();
		if(start_serial == 1)
		{
			doSerialAssignment = TRUE;
			CGenericOneEditDialog<int> newSerialDlg(1, "Enter starting serial number", "Serial Number");
			newSerialDlg.SetLimits(1, 1, FALSE, "Please enter a serial number greater than 0.");
			newSerialDlg.DoModal();
			start_serial = newSerialDlg.m_val;
		}

		serial = start_serial;
	}

	BOOL storeOldSerial = FALSE;
	CString oldSerialTag = "";
	if((type == tStock) && (gDoc->m_selected_voucher->m_type == tExport))
	{
		int userchoice = 0;
		SHOW_INFO_YESNO("Do you want to store the old serial number for future use ?", userchoice);
		storeOldSerial = (userchoice == IDYES);
		if(storeOldSerial)
		{
			while(TRUE)
			{
				CGenericOneEditDialog<CString> dlg("", "Tagname for reference", "Tagname");
				dlg.SetLimits("", "", FALSE, "Please choose a tagname.");

				while(TRUE)
				{
					if(dlg.DoModal() == IDOK)
					{
						oldSerialTag = dlg.m_val;
						break;
					}
					else
					{
						SHOW_WARNING_YESNO("Are you sure you don't want to store the old serials ?", userchoice);
						if(userchoice == IDYES)
						{
							storeOldSerial = FALSE;
							break;
						}
					}
				}

				if(!storeOldSerial)
					break;

				BOOL get_another_tagname = FALSE;
				POSITION pos = pendinglist->GetHeadPosition();
				while(pos)
				{
					CString productID = pendinglist->GetAt(pos);
					CProductSet *set = gDoc->GetSet(productID, NULL);
					if(set)
					{
						if(set->HasOldSerialNumbers(oldSerialTag))
						{
							CString str = "Some of the chosen sets already have old serials with the tagname '" + oldSerialTag + "'. Are you sure you want to overwrite them ?";
							SHOW_WARNING_YESNO(str, userchoice);
							if(userchoice != IDYES)
								get_another_tagname = TRUE;

							break;
						}
					}
					else
					{
						CString errstr = "Could not find set '" + productID + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
					}

					pendinglist->GetNext(pos);
				}

				if(!get_another_tagname)
					break;
			}
		}
	}

	CProgressDialog progress("Moving sets", pendinglist->GetCount());
	progress.StartDialog();

	int overwrite_old_tags = -1;
	BOOL err = FALSE;
	int count = pendinglist->GetCount();
	while(count > 0)
	{
		CString productID = pendinglist->RemoveHead();
		CProductVoucher *this_voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &this_voucher);
		if(!set)
		{
			pendinglist->AddTail(productID);
			err = TRUE;
			count--;
			progress.ProgressForward();
			continue;
		}

		CString this_voucher_name = this_voucher->m_voucher_name;
		if(voucher_name.CompareNoCase(this_voucher_name) == 0)
		{
			count--;
			progress.ProgressForward();
			continue;
		}

		if(storeOldSerial)
		{
			if(!set->AddOldSerialNumbers(oldSerialTag))
			{
				CString errstr = "Could not add old serial number to set '" + CString(set->m_product_ID) + "' in voucher '" + CString(this_voucher->m_voucher_name) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}

		serial = set->UpdateSerialNumbers(serial);
		//remove checking duplicates here .................
		if(!dest_voucher->AddSet(set, FALSE, TRUE))
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
	{
		if(!dest_voucher->AssignSerialNumbers(start_serial))
		{
			CString errstr = "Could not assign serial numbers to the voucher '" + CString(dest_voucher->m_voucher_name) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}

	progress.FinishDialog();
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
			if(!MakeGlobalsValid())
			{
				SHOW_INTERNAL_ERROR_REASON("Global values invalid");
			}
			else
			{
				CWnd *current = CWnd::GetFocus();
				if(current == gTreeView)
					gStockView->SetFocus();
				else
					gTreeView->SetFocus();
			}

			break;
		}
		break;
	case WM_KEYUP:
		break;
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::SetStatusBarString(CString str)
{
	if(!m_wndStatusBar.SetPaneText(0, str))
		SHOW_INTERNAL_ERROR_REASON("Could not set status bar text.");
}

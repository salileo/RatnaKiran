#include "stdafx.h"
#include "RatnaKiran.h"
#include "MainGlobal.h"
#include "RatnaKiranReports.h"
#include "ShortInvoiceDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define USE_TWO_COLUMNS 1
static int price_quotation_left = 0;
static int price_quotation_right = 0;
static int price_quotation_number_of_columns = 0;
static int price_quotation_page_size = 0;

static int common_serial = 0;

CRatnaKiranReports::CRatnaKiranReports(BOOL use_excel)
{
	m_list = NULL;
	m_close = FALSE;

	m_serials_from = 0;
	m_serials_to = -1;
	m_use_product_id = FALSE;
	m_use_new_serials = TRUE;
	m_use_old_serials = FALSE;
	m_show_diamond_clarity = TRUE;
	m_old_serial_tagname = "";

	m_image_width = 30;
	m_image_height = 150;

	m_rows_in_a_page = 1;

	m_start_row = 0;
	m_end_row = 0;
	m_last_column = 0;

	if(use_excel)
		m_valid = InitiateReport();
	else
		m_valid = FALSE;
}


CRatnaKiranReports::~CRatnaKiranReports()
{
	if(m_valid)
	{
		if(m_close || !m_status)
			StopExcel();
		else
			DisplayExcel();
	}
	else
		SHOW_INTERNAL_ERROR;
}


int CRatnaKiranReports::MeasureText(CDC *dc, CString str)
{
	CRect rect;
	dc->DrawText(str, &rect, DT_NOPREFIX | DT_CALCRECT);
	return rect.Width();
}


BOOL CRatnaKiranReports::InitiateReport()
{
	return StartExcel();
}


void CRatnaKiranReports::AddAutoFit(int column)
{
	m_autofit.AddTail(column);
}


void CRatnaKiranReports::ProcessAutoFit()
{
	POSITION pos = m_autofit.GetHeadPosition();
	while(pos)
	{
		int column = m_autofit.GetAt(pos);
		SetAutoFit(gAlpha[column]);
		m_autofit.GetNext(pos);
	}
}


void CRatnaKiranReports::AddTotals(int column, BOOL autofit)
{
	m_totals.AddTail(column);

	if(autofit)
		AddAutoFit(column);
}


int CRatnaKiranReports::ProcessTotals(int position)
{
	FillCell(gAlpha[1], position, "GRAND TOTAL");

	POSITION pos = m_totals.GetHeadPosition();
	while(pos)
	{
		int column = m_totals.GetAt(pos);

		if(position == m_start_row)
			FillCell(gAlpha[column], position, 0);
		else
		{
			CString str;
			str.Format("=SUM(%s:%s)", GetAddress(gAlpha[column], m_start_row), GetAddress(gAlpha[column], position - 1));
			FillCellWithFormula(gAlpha[column], position, str);
		}

		m_totals.GetNext(pos);
	}

	return (position + 1);
}


void CRatnaKiranReports::FinalizeLandscapeReport(BOOL add_right_footer)
{
	if(m_filename.IsEmpty())
	{
		ExcelPageSetup pagesetup;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetPageSetup();
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		pagesetup.AttachDispatch(lpDisp);
		pagesetup.SetOrientation(2);
		//1 = Potrait, 2 = Landscape

		CString printarea;
		printarea.Format("%s1:%s%d", gAlpha[0], gAlpha[m_last_column], m_end_row);
		pagesetup.SetPrintArea(printarea);

		pagesetup.SetCenterHorizontally(FALSE);
		pagesetup.SetCenterVertically(FALSE);

		pagesetup.SetFitToPagesWide(COleVariant((short)1));
		pagesetup.SetFitToPagesTall(COleVariant((short)m_end_row));

		pagesetup.SetTopMargin(36);//1 -> 0.013888

		if(add_right_footer)
			pagesetup.SetBottomMargin(72);
		else
			pagesetup.SetBottomMargin(36);

		pagesetup.SetLeftMargin(36);
		pagesetup.SetRightMargin(36);

		CString printtitle;
		printtitle.Format("$%d:$%d", m_start_row - 2, m_start_row - 1);
		pagesetup.SetPrintTitleRows(printtitle);
		pagesetup.SetCenterFooter("Page &p");
		pagesetup.SetFooterMargin(18); //1 -> 0.013888
		pagesetup.SetHeaderMargin(0);

		if(add_right_footer)
			pagesetup.SetRightFooter("For Santosh Jewellers\n\n\n\nPartner");

/*
		m_sheet.PrintOut(covOptional,covOptional,covOptional,covOptional,
						covOptional,covOptional,covOptional,covOptional);
*/
	}
	else
	{
#if SALIL_COMP
		m_book.SaveAs(COleVariant(m_filename),covOptional,covOptional,covOptional,
				covOptional,covOptional,GENERIC_ALL,covOptional,covOptional,
				covOptional,covOptional,covOptional);
#else
		m_book.SaveAs(COleVariant(m_filename),covOptional,covOptional,covOptional,
				covOptional,covOptional,GENERIC_ALL,covOptional,covOptional,
				covOptional,covOptional);
#endif
	}
}


void CRatnaKiranReports::FinalizePotraitReport(BOOL pricequotation)
{
	if(m_filename.IsEmpty())
	{
		ExcelPageSetup pagesetup;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetPageSetup();
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		pagesetup.AttachDispatch(lpDisp);
		pagesetup.SetOrientation(1);
		//1 = Potrait, 2 = Landscape

		CString printarea;
		printarea.Format("%s1:%s%d", gAlpha[0], gAlpha[m_last_column], m_end_row);
		pagesetup.SetPrintArea(printarea);

		if(pricequotation)
		{
			pagesetup.SetCenterHorizontally(FALSE);
			pagesetup.SetCenterVertically(FALSE);

			pagesetup.SetFitToPagesWide(COleVariant((short)1));
			pagesetup.SetFitToPagesTall(COleVariant((short)m_end_row));

			pagesetup.SetLeftMargin(36);//1 -> 0.013888
			pagesetup.SetRightMargin(36);
			pagesetup.SetTopMargin(36);
			pagesetup.SetBottomMargin(72);

			CString printtitle;
			printtitle.Format("$%d:$%d", m_start_row - 2, m_start_row - 1);
			pagesetup.SetPrintTitleRows(printtitle);
			pagesetup.SetCenterFooter("Page &p");
			pagesetup.SetFooterMargin(18);
			pagesetup.SetHeaderMargin(0);
		}
		else
		{
			pagesetup.SetCenterHorizontally(TRUE);
			pagesetup.SetCenterVertically(TRUE);

			pagesetup.SetFitToPagesWide(COleVariant((short)1));
			pagesetup.SetFitToPagesTall(COleVariant((short)1));

			pagesetup.SetLeftMargin(18);//1 -> 0.013888
			pagesetup.SetRightMargin(18);
			pagesetup.SetTopMargin(18);
			pagesetup.SetBottomMargin(18);
			pagesetup.SetFooterMargin(0);
			pagesetup.SetHeaderMargin(0);
		}

/*
		m_sheet.PrintOut(covOptional,covOptional,covOptional,covOptional,
						covOptional,covOptional,covOptional,covOptional);
*/
	}
	else
	{
#if SALIL_COMP
		m_book.SaveAs(COleVariant(m_filename),covOptional,covOptional,covOptional,
				covOptional,covOptional,GENERIC_ALL,covOptional,covOptional,
				covOptional,covOptional,covOptional);
#else
		m_book.SaveAs(COleVariant(m_filename),covOptional,covOptional,covOptional,
				covOptional,covOptional,GENERIC_ALL,covOptional,covOptional,
				covOptional,covOptional);
#endif
	}
}


void CRatnaKiranReports::FinalizeTagReport()
{
	if(m_filename.IsEmpty())
	{
		ExcelPageSetup pagesetup;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetPageSetup();
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		pagesetup.AttachDispatch(lpDisp);
		pagesetup.SetOrientation(1);
		//1 = Potrait, 2 = Landscape

		CString printarea;
		printarea.Format("%s1:%s%d", gAlpha[0], gAlpha[m_last_column], m_end_row);
		pagesetup.SetPrintArea(printarea);

		//pagesetup.SetCenterAlignHorizontally(TRUE);
		//pagesetup.SetCenterAlignVertically(TRUE);

		pagesetup.SetPaperSize(5); //5 = legal
		pagesetup.SetFitToPagesWide(COleVariant((short)1));
		pagesetup.SetFitToPagesTall(COleVariant((short)m_end_row));

		pagesetup.SetLeftMargin(36);//1 -> 0.013888
		pagesetup.SetRightMargin(36);
		pagesetup.SetTopMargin(54);
		pagesetup.SetBottomMargin(180);
		pagesetup.SetFooterMargin(0);
		pagesetup.SetHeaderMargin(0);

/*
		m_sheet.PrintOut(covOptional,covOptional,covOptional,covOptional,
						covOptional,covOptional,covOptional,covOptional);
*/
	}
	else
	{
#if SALIL_COMP
		m_book.SaveAs(COleVariant(m_filename),covOptional,covOptional,covOptional,
				covOptional,covOptional,GENERIC_ALL,covOptional,covOptional,
				covOptional,covOptional,covOptional);
#else
		m_book.SaveAs(COleVariant(m_filename),covOptional,covOptional,covOptional,
				covOptional,covOptional,GENERIC_ALL,covOptional,covOptional,
				covOptional,covOptional);
#endif
	}
}

BOOL CRatnaKiranReports::CreateStocksReportActual(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	common_serial = 1;

	CProgressDialog progress("Processing report", m_list->GetCount() + 3);
	progress.StartDialog();

	m_start_row = 5;
	StockReport_Actual_AddHeaders();
	StockReport_Actual_AddTitle(type);
	progress.ProgressForward();

	int position = m_start_row;
	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				position = StockReport_Actual_AddSet(set, voucher, position, type);
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	position = ProcessTotals(position);
	m_end_row = position;
	progress.ProgressForward();

	ProcessAutoFit();
	progress.ProgressForward();

	FinalizeLandscapeReport();
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::StockReport_Actual_AddTitle(ReportType type)
{
	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 1);
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);

	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "STOCK LIST (Actual rates/values)");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "STOCK LIST OF SOLD ITEMS (Actual rates/values)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "STOCK LIST OF UNSOLD ITEMS (Actual rates/values)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign,CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
}


void CRatnaKiranReports::StockReport_Actual_AddHeaders()
{
	int position = m_start_row - 2;
	int column = 0;

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "S.No.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "ProductID");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Item");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Desc.");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Pcs.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "GWt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "NWt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Gold value (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column + 3], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Diamond");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Type");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Value (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column+ 3], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pr/SPr Stone");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Type");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Value (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column + 2], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pearl");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Value (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Oth. Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Polishing (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Total price (Rs.)");

	m_last_column = column - 1;

	SetBorders(gAlpha[0], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[0], position, gAlpha[m_last_column], position + 1);
}


//Returns the position where the next entry can be made
int CRatnaKiranReports::StockReport_Actual_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type)
{
	if(!set || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	if(type == Sold_Report && set->GetSoldItemCount() == 0)
		return position;
	else if(type == UnSold_Report && set->GetUnsoldItemCount() == 0)
		return position;

	int pos = position;

	FillCell(gAlpha[0], position, common_serial);
	FillCell(gAlpha[1], position, set->m_product_ID);
	common_serial++;

	int index = 0;
	while(index < set->GetItemCount())
	{
		CProductItem *item = set->GetItemAt(index);
		if(item)
		{
			if(((type == Sold_Report) && item->IsItemSold()) ||
			   ((type == UnSold_Report) && !(item->IsItemSold())) ||
				type == Original_Report)
			{
				pos = StockReport_Actual_AddItem(item, voucher, pos);
			} 
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		index++;
	}

	if(pos == position)
		pos++;

	SetBorder(gAlpha[0], position, gAlpha[m_last_column], pos - 1, AllBorders, TRUE, 1);
	return pos;
}

int CRatnaKiranReports::StockReport_Actual_AddItem(CProductItem *item, CProductVoucher *voucher, int position)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int column = 2;

	FillCell(gAlpha[column++], position, item->m_description);
	FillCell(gAlpha[column++], position, item->m_piece_count);
	FillCell(gAlpha[column++], position, item->GetGrossWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetNetWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetGoldValue(item->m_use_net_weight_for_gold, TRUE, GetGoldRate(voucher), 1.0), 2);

	int index = 0;
	while(index < item->GetSubItemCount(tDiamond))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemTypeOf(tDiamond, index));
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemWeightOf(tDiamond, index), 2);
		FillCell(gAlpha[column + 2], position + index, item->GetSubItemRateOf(tDiamond, index, 1.0));
		FillCell(gAlpha[column + 3], position + index, item->GetSubItemValueOf(tDiamond, index, 1.0), 2);

		index++;
	}

	column += 4;
	
	index = 0;
	while(index < item->GetSubItemCount(tStone))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemTypeOf(tStone, index));
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemWeightOf(tStone, index), 2);
		FillCell(gAlpha[column + 2], position + index, item->GetSubItemRateOf(tStone, index, 1.0));
		FillCell(gAlpha[column + 3], position + index, item->GetSubItemValueOf(tStone, index, 1.0), 2);

		index++;
	}

	column += 4;

	index = 0;
	while(index < item->GetSubItemCount(tPearl))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemWeightOf(tPearl, index), 2);
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemRateOf(tPearl, index, 1.0));
		FillCell(gAlpha[column + 2], position + index, item->GetSubItemValueOf(tPearl, index, 1.0), 2);

		index++;
	}

	column += 3;

	FillCell(gAlpha[column++], position, item->GetOtherWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetLabourCost(1.0), 2);
	FillCell(gAlpha[column++], position, item->GetTotalValue(item->m_use_net_weight_for_gold, TRUE, GetGoldRate(voucher), 1.0), 2);

	int newpos;
	int maxval = max(item->GetSubItemCount(tDiamond), item->GetSubItemCount(tStone));
	maxval = max(maxval, item->GetSubItemCount(tPearl));
	maxval = max(1, maxval);
	
	/*
	if(maxval > 1)//this takes care of going to the next empty position to add the next item.
		newpos = position + maxval + 1;
	else
		newpos = position + maxval;
	*/

	newpos = position + maxval;
	return newpos;
}


BOOL CRatnaKiranReports::CreateStocksReportMarked(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	common_serial = 1;

	CProgressDialog progress("Processing report", m_list->GetCount() + 3);
	progress.StartDialog();

	m_start_row = 5;
	StockReport_Marked_AddHeaders();
	StockReport_Marked_AddTitle(type);
	progress.ProgressForward();

	int position = m_start_row;
	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				position = StockReport_Marked_AddSet(set, voucher, position, type);
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	position = ProcessTotals(position);
	m_end_row = position;
	progress.ProgressForward();

	ProcessAutoFit();
	progress.ProgressForward();

	FinalizeLandscapeReport();
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::StockReport_Marked_AddTitle(ReportType type)
{
	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 1);
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);

	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "STOCK LIST (Marked rates/values)");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "STOCK LIST OF SOLD ITEMS (Marked rates/values)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "STOCK LIST OF UNSOLD ITEMS (Marked rates/values)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign,CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
}


void CRatnaKiranReports::StockReport_Marked_AddHeaders()
{
	int position = m_start_row - 2;
	int column = 0;

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "S.No.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "ProductID");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Item");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Desc.");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Pcs.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "GWt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "NWt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Gold value (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column + 3], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Diamond");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Type");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Value (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column+ 3], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pr/SPr Stone");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Type");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Value (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column + 2], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pearl");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Value (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Oth. Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Polishing (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Total price (Rs.)");

	m_last_column = column - 1;

	SetBorders(gAlpha[0], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[0], position, gAlpha[m_last_column], position + 1);
}


//Returns the position where the next entry can be made
int CRatnaKiranReports::StockReport_Marked_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type)
{
	if(!set || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	if(type == Sold_Report && set->GetSoldItemCount() == 0)
		return position;
	else if(type == UnSold_Report && set->GetUnsoldItemCount() == 0)
		return position;

	int pos = position;

	FillCell(gAlpha[0], position, common_serial);
	FillCell(gAlpha[1], position, set->m_product_ID);
	common_serial++;

	int index = 0;
	while(index < set->GetItemCount())
	{
		CProductItem *item = set->GetItemAt(index);
		if(item)
		{
			if(((type == Sold_Report) && item->IsItemSold()) ||
			   ((type == UnSold_Report) && !(item->IsItemSold())) ||
				type == Original_Report)
			{
				pos = StockReport_Marked_AddItem(item, voucher, pos);
			}
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		index++;
	}

	if(pos == position)
		pos++;

	SetBorder(gAlpha[0], position, gAlpha[m_last_column], pos - 1, AllBorders, TRUE, 1);
	return pos;
}


int CRatnaKiranReports::StockReport_Marked_AddItem(CProductItem *item, CProductVoucher *voucher, int position)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int column = 2;

	FillCell(gAlpha[column++], position, item->m_description);
	FillCell(gAlpha[column++], position, item->m_piece_count);
	FillCell(gAlpha[column++], position, item->GetGrossWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetNetWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetGoldValue(item->m_use_net_weight_for_gold, TRUE, GetGoldRate(voucher), 1.0), 2);

	int index = 0;
	while(index < item->GetSubItemCount(tDiamond))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemTypeOf(tDiamond, index));
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemWeightOf(tDiamond, index), 2);
		FillCell(gAlpha[column + 2], position + index, item->GetMarkedSubItemRateOf(tDiamond, index, GetGoldRate(voucher),1.0));
		FillCell(gAlpha[column + 3], position + index, item->GetMarkedSubItemValueOf(tDiamond, index, GetGoldRate(voucher),1.0), 2);

		index++;
	}

	column += 4;

	index = 0;
	while(index < item->GetSubItemCount(tStone))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemTypeOf(tStone, index));
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemWeightOf(tStone, index), 2);
		FillCell(gAlpha[column + 2], position + index, item->GetMarkedSubItemRateOf(tStone, index, GetGoldRate(voucher),1.0));
		FillCell(gAlpha[column + 3], position + index, item->GetMarkedSubItemValueOf(tStone, index, GetGoldRate(voucher),1.0), 2);

		index++;
	}

	column += 4;

	index = 0;
	while(index < item->GetSubItemCount(tPearl))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemWeightOf(tPearl, index), 2);
		FillCell(gAlpha[column + 1], position + index, item->GetMarkedSubItemRateOf(tPearl, index, GetGoldRate(voucher),1.0));
		FillCell(gAlpha[column + 2], position + index, item->GetMarkedSubItemValueOf(tPearl, index, GetGoldRate(voucher),1.0), 2);

		index++;
	}

	column += 3;

	FillCell(gAlpha[column++], position, item->GetOtherWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetMarkedLabourCost(GetGoldRate(voucher),1.0), 2);
	FillCell(gAlpha[column++], position, item->GetTotalMarkedValue(item->m_use_net_weight_for_gold, TRUE, GetGoldRate(voucher),1.0), 2);

	int newpos;
	int maxval = max(item->GetSubItemCount(tDiamond), item->GetSubItemCount(tStone));
	maxval = max(maxval, item->GetSubItemCount(tPearl));
	maxval = max(1, maxval);

	/*
	if(maxval > 1)//this takes care of going to the next empty position to add the next item.
		newpos = position + maxval + 1;
	else
		newpos = position + maxval;
	*/

	newpos = position + maxval;
	return newpos;
}


BOOL CRatnaKiranReports::CreateStockPhotoInvoice(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	common_serial = 1;

	CProgressDialog progress("Processing report", m_list->GetCount() + 3);
	progress.StartDialog();

	m_start_row = 5;
	StockPhotoInvoice_AddHeaders();
	StockPhotoInvoice_AddTitle(type);
	progress.ProgressForward();

	int position = m_start_row;
	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			int index = 0;
			while(index < set->GetItemCount())
			{
				int orig_position = position;
				CProductItem *item = set->GetItemAt(index);
				if(item)
				{
					if(((type == Sold_Report) && item->IsItemSold()) ||
					   ((type == UnSold_Report) && !(item->IsItemSold())) ||
						type == Original_Report)
					{
						position = StockPhotoInvoice_AddItem(item, voucher, position);
					}
				}
				else
				{
					CString errstr;
					errstr.Format("%d", index);
					errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}

				index++;
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	//position = ProcessTotals(position);
	m_end_row = position;
	progress.ProgressForward();

	ProcessAutoFit();
	progress.ProgressForward();

	FinalizeLandscapeReport();
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::StockPhotoInvoice_AddTitle(ReportType type)
{
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);
	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "STOCK PHOTO INVOICE");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "STOCK PHOTO INVOICE (Sold)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "STOCK PHOTO INVOICE (UnSold)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 1);
	SetAlignment(gAlpha[0], 1, CenterAlign, CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
}


void CRatnaKiranReports::StockPhotoInvoice_AddHeaders()
{
	int position = m_start_row - 2;
	int column = 0;

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	SetWidth(gAlpha[column], gAlpha[column], m_image_width);
	FillCell(gAlpha[column++], position, "Picture");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "ID");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	SetTextWrap(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Desc. of product");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Pcs.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Gross Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Net Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Gold fineness");

	if(m_show_diamond_clarity)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Diamond clarity");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Diamond Wt. (Crt.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Stone Wt. (Crt.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Pearl Wt. (Crt.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Value");

	m_last_column = column - 1;

	SetBorders(gAlpha[0], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[0], position, gAlpha[m_last_column], position + 1);
}


int CRatnaKiranReports::StockPhotoInvoice_AddItem(CProductItem *item, CProductVoucher *voucher, int position)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	SetHeight(position, position, m_image_height);
	SetAlignment(gAlpha[1], position, gAlpha[m_last_column], position, CenterAlign, CenterAlign);

	double goldvalue = item->GetGoldValue(item->m_use_net_weight_for_gold, TRUE, GetGoldRate(voucher), 1.0);
	goldvalue = RoundOff(goldvalue, 2);

	double diamondvalue = item->GetSubItemValueDouble(tDiamond, 1.0);
	diamondvalue = RoundOff(diamondvalue, 2);

	double stonevalue = item->GetSubItemValueDouble(tStone, 1.0);
	stonevalue = RoundOff(stonevalue, 2);

	double labourvalue = item->GetLabourCost(1.0);
	labourvalue = RoundOff(labourvalue, 2);

	double pearlvalue = item->GetSubItemValueDouble(tPearl, 1.0);
	pearlvalue = RoundOff(pearlvalue, 2);

	double total =  goldvalue + diamondvalue + stonevalue + labourvalue + pearlvalue;
	int column = 0;

	CString filename = item->m_photo_filename;
	if(!filename.IsEmpty())
	{
		filename = gFilePath + "\\images\\" + filename;
		FillCellWithImage(gAlpha[column++], position, filename);
	}
	else
	{
		column++;
	}

	FillCell(gAlpha[column++], position, item->m_product_ID);
	FillCell(gAlpha[column++], position, item->m_description);
	FillCell(gAlpha[column++], position, item->m_piece_count);
	FillCell(gAlpha[column++], position, item->GetGrossWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetNetWeight(), 3);
	FillCell(gAlpha[column++], position, "18KT");
	if(m_show_diamond_clarity)
		FillCell(gAlpha[column++], position, item->m_diamond_clarity);
	FillCell(gAlpha[column++], position, item->GetSubItemWeight(tDiamond), 2);
	FillCell(gAlpha[column++], position, item->GetSubItemWeight(tStone), 2);
	FillCell(gAlpha[column++], position, item->GetSubItemWeight(tPearl), 2);
	FillCell(gAlpha[column++], position, total, 2);

	SetBorder(gAlpha[0], position, gAlpha[m_last_column], position, AllBorders, TRUE, 1);
	return (position + 1);
}


BOOL CRatnaKiranReports::CreateStockPriceQuotation(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CProgressDialog progress("Processing report", m_list->GetCount() + 3);
	progress.StartDialog();

	m_start_row = 5;
	StockPriceQuotation_AddHeaders();
	StockPriceQuotation_AddTitle(type);
	progress.ProgressForward();

	int position = m_start_row;
	BOOL onfirstpage = TRUE;
	int page = 0;
	int itemcount = 0;
	int orientation = price_quotation_left;

	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
#if USE_TWO_COLUMNS
			if(onfirstpage)
			{
				if(itemcount + set->GetItemCount() > (price_quotation_page_size - 4))
				{
					if(orientation == price_quotation_left)
					{
						itemcount = 0;
						position = m_start_row;
						orientation = price_quotation_right;
					}
					else
					{
						page++;
						onfirstpage = FALSE;
						itemcount = 0;
						position = price_quotation_page_size + 1;
						orientation = price_quotation_left;
						SetPageBreak(gAlpha[m_last_column + 1], price_quotation_page_size + 1);
					}
				}
			}
			else
			{
				if(itemcount + set->GetItemCount() > price_quotation_page_size - 2)
				{
					if(orientation == price_quotation_left)
					{
						itemcount = 0;
						position = price_quotation_page_size + ((page - 1) * (price_quotation_page_size - 2)) + 1; //all pages except first page will have 2 less items than the page size.
						orientation = price_quotation_right;
					}
					else
					{
						page++;
						itemcount = 0;
						position = price_quotation_page_size + ((page - 1) * (price_quotation_page_size - 2)) + 1; //all pages except first page will have 2 less items than the page size.
						orientation = price_quotation_left;
						SetPageBreak(gAlpha[m_last_column + 1], price_quotation_page_size + ((page - 1) * (price_quotation_page_size - 2)) + 1);
					}
				}
			}
#endif

			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				position = StockPriceQuotation_AddSet(set, voucher, position, orientation, type);
			}

#if USE_TWO_COLUMNS
			if(onfirstpage)
			{
				itemcount = position - m_start_row;
			}
			else
			{
				itemcount = position - (price_quotation_page_size + ((page - 1) * (price_quotation_page_size - 2)) + 1);
			}
#endif
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	//position = ProcessTotals(position);
#if USE_TWO_COLUMNS
	m_end_row = (page + 1) * price_quotation_page_size;
#else
	m_end_row = position;
#endif

	progress.ProgressForward();

	ProcessAutoFit();
	progress.ProgressForward();

	FinalizePotraitReport(TRUE);
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::StockPriceQuotation_AddTitle(ReportType type)
{
	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 1);
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);

	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "STOCK PRICE QUOTATION");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "STOCK PRICE QUOTATION (Sold)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "STOCK PRICE QUOTATION (UnSold)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign, CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
}

	
void CRatnaKiranReports::StockPriceQuotation_AddHeaders()
{
	int position = m_start_row - 2;
	int column = price_quotation_left;

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Item ID");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Item Desc.");

	if(m_show_diamond_clarity)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Diamond Clarity");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Price");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Total");

	m_last_column = column - 1;
	price_quotation_number_of_columns = column;

	SetBorders(gAlpha[price_quotation_left], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[price_quotation_left], position, gAlpha[m_last_column], position + 1);

#if USE_TWO_COLUMNS
	SetWidth(gAlpha[price_quotation_number_of_columns], gAlpha[price_quotation_number_of_columns], 4);

	price_quotation_right = price_quotation_number_of_columns + 1;
	column = price_quotation_right;

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Item ID");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Item Desc.");

	if(m_show_diamond_clarity)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Diamond Clarity");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Price");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Total");

	m_last_column = column - 1;

	SetBorders(gAlpha[price_quotation_right], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[price_quotation_right], position, gAlpha[m_last_column], position + 1);

	price_quotation_page_size = m_rows_in_a_page - 2; //needs to be 2 less the actual size to accomodate the heading.
#endif
}


int CRatnaKiranReports::StockPriceQuotation_AddSet(CProductSet *set, CProductVoucher *voucher, int position, int orientation, ReportType type)
{
	if(!set || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int pos = position;
	int index = 0;
	while(index < set->GetItemCount())
	{
		CProductItem *item = set->GetItemAt(index);
		if(item)
		{
			if(((type == Sold_Report) && item->IsItemSold()) ||
			   ((type == UnSold_Report) && !(item->IsItemSold())) ||
				type == Original_Report)
			{
				pos = StockPriceQuotation_AddItem(item, voucher, pos, orientation);
			}
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		index++;
	}

	if(pos == position)
		pos++;

	CString str;
	if(set->GetItemCount() > 1)
	{
#if USE_TWO_COLUMNS
		if(orientation == price_quotation_left)
		{
			str.Format("=SUM(%s:%s)", GetAddress(gAlpha[price_quotation_left + price_quotation_number_of_columns - 2], position), GetAddress(gAlpha[price_quotation_left + price_quotation_number_of_columns - 2], pos - 1));
			FillCellWithFormula(gAlpha[price_quotation_left + price_quotation_number_of_columns - 1], (position + pos - 1) / 2, str);
		}
		else if(orientation == price_quotation_right)
		{
			str.Format("=SUM(%s:%s)", GetAddress(gAlpha[price_quotation_right +  price_quotation_number_of_columns - 2], position), GetAddress(gAlpha[price_quotation_right +  price_quotation_number_of_columns - 2], pos - 1));
			FillCellWithFormula(gAlpha[price_quotation_right +  price_quotation_number_of_columns - 1], (position + pos - 1) / 2, str);
		}
#else
		str.Format("=SUM(%s:%s)", GetAddress(gAlpha[m_last_column - 1], position), GetAddress(gAlpha[m_last_column - 1], pos - 1));
		FillCellWithFormula(gAlpha[m_last_column], (position + pos - 1) / 2, str);
#endif
	}

#if USE_TWO_COLUMNS
	if(orientation == price_quotation_left)
		SetBorder(gAlpha[price_quotation_left], position, gAlpha[price_quotation_left + price_quotation_number_of_columns - 1], pos - 1, AllBorders, TRUE, 1);
	else if(orientation == price_quotation_right)
		SetBorder(gAlpha[price_quotation_right], position, gAlpha[price_quotation_right + price_quotation_number_of_columns - 1], pos - 1, AllBorders, TRUE, 1);
#else
	SetBorder(gAlpha[price_quotation_left], position, gAlpha[m_last_column], pos - 1, AllBorders, TRUE, 1);
#endif

	return pos;
}


int CRatnaKiranReports::StockPriceQuotation_AddItem(CProductItem *item, CProductVoucher *voucher, int position, int orientation)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	double gold = GetGoldRate(voucher);
	double pricequote;
	CString price = voucher->m_price_quote;
	double value = item->GetTotalValue(item->m_use_net_weight_for_gold, TRUE, gold, 1.0);

	if(!price.IsEmpty())
		value = ProcessFormula(price, gold, 1.0, item);

	pricequote = value;

#if USE_TWO_COLUMNS
	int column = 0;
	if(orientation == price_quotation_left)
		column = price_quotation_left;
	else if(orientation == price_quotation_right)
		column = price_quotation_right;
#else
	int column = price_quotation_left;
#endif

	FillCell(gAlpha[column++], position, item->m_product_ID);
	FillCell(gAlpha[column++], position, item->m_description);
	if(m_show_diamond_clarity)
		FillCell(gAlpha[column++], position, item->m_diamond_clarity);

	int final_price = (int)(pricequote + 0.5);
	FillCell(gAlpha[column++], position, final_price);

	return (position + 1);
}


BOOL CRatnaKiranReports::CreateStockChecklist(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	common_serial = 1;

	CProgressDialog progress("Processing report", m_list->GetCount() + 3);
	progress.StartDialog();

	m_start_row = 5;
	StockChecklist_AddHeaders();
	StockChecklist_AddTitle(type);
	progress.ProgressForward();

	int position = m_start_row;
	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				position = StockChecklist_AddSet(set, voucher, position, type);
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	position = ProcessTotals(position);
	m_end_row = position;
	progress.ProgressForward();

	ProcessAutoFit();
	progress.ProgressForward();

	FinalizeLandscapeReport();
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::StockChecklist_AddTitle(ReportType type)
{
	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 1);
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);

	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "STOCK CHECKLIST");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "STOCK CHECKLIST (Sold)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "STOCK CHECKLIST (UnSold)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign,CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
}


void CRatnaKiranReports::StockChecklist_AddHeaders()
{
	int position = m_start_row - 2;
	int column = 0;

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "S.No.");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Set");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "ProductID");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Desc.");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Item");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Desc.");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Pcs.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "GWt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "NWt. (Gms.)");

	if(m_show_diamond_clarity)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddTotals(column);
		FillCell(gAlpha[column++], position, "Diamond Clarity");
	}

	MergeCells(gAlpha[column], position, gAlpha[column + 2], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Diamond");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Type");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column+ 2], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pr/SPr Stone");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Type");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pearl");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Other Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Polishing (Rs.)");

	m_last_column = column - 1;

	SetBorders(gAlpha[0], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[0], position, gAlpha[m_last_column], position + 1);
}


//Returns the position where the next entry can be made
int CRatnaKiranReports::StockChecklist_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type)
{
	if(!set || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int pos = position;

	FillCell(gAlpha[0], position, common_serial);
	FillCell(gAlpha[1], position, set->m_product_ID);
	FillCell(gAlpha[2], position, set->m_description);
	common_serial++;

	int index = 0;
	while(index < set->GetItemCount())
	{
		CProductItem *item = set->GetItemAt(index);
		if(item)
		{
			if(((type == Sold_Report) && item->IsItemSold()) ||
			   ((type == UnSold_Report) && !(item->IsItemSold())) ||
				type == Original_Report)
			{
				pos = StockChecklist_AddItem(item, voucher, pos);
			}
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		index++;
	}

	if(pos == position)
		pos++;

	SetBorder(gAlpha[0], position, gAlpha[m_last_column], pos - 1, AllBorders, TRUE, 1);
	return pos;
}

int CRatnaKiranReports::StockChecklist_AddItem(CProductItem *item, CProductVoucher *voucher, int position)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int column = 3;

	FillCell(gAlpha[column++], position, item->m_description);
	FillCell(gAlpha[column++], position, item->m_piece_count);
	FillCell(gAlpha[column++], position, item->GetGrossWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetNetWeight(), 3);
	if(m_show_diamond_clarity)
		FillCell(gAlpha[column++], position, item->m_diamond_clarity);

	int index = 0;
	while(index < item->GetSubItemCount(tDiamond))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemTypeOf(tDiamond, index));
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemWeightOf(tDiamond, index), 2);
		FillCell(gAlpha[column + 2], position + index, item->GetSubItemRateOf(tDiamond, index, 1.0));

		index++;
	}

	column += 3;

	index = 0;
	while(index < item->GetSubItemCount(tStone))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemTypeOf(tStone, index));
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemWeightOf(tStone, index), 2);
		FillCell(gAlpha[column + 2], position + index, item->GetSubItemRateOf(tStone, index, 1.0));

		index++;
	}

	column += 3;

	index = 0;
	while(index < item->GetSubItemCount(tPearl))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemWeightOf(tPearl, index), 2);
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemRateOf(tPearl, index, 1.0));

		index++;
	}

	column += 2;

	FillCell(gAlpha[column++], position, item->GetOtherWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetLabourCost(1.0), 2);

	int maxval = max(item->GetSubItemCount(tDiamond), item->GetSubItemCount(tStone));
	maxval = max(maxval, item->GetSubItemCount(tPearl));
	maxval = max(1, maxval);

	return (position + maxval);//this takes care of going to the next empty position to add the next item.
}


BOOL CRatnaKiranReports::CreateStockTags(int row, int column)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	bool show_values = (MessageBox(NULL, "Do you want to show price values?", "Tag creation", MB_YESNO) == IDYES);

	m_start_row = 1;
	m_last_column = 5;

	int position = row;
	int itemnumber = (column - 1)/2;

	CProgressDialog progress("Processing report", m_list->GetCount() + 1);
	progress.StartDialog();

	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			int index = 0;
			while(index < set->GetItemCount())
			{
				CProductItem *item = set->GetItemAt(index);
				if(item)
				{
					StockTags_AddItem(item, voucher, (2 * itemnumber), position, show_values);
					itemnumber++;

					if(itemnumber == 3)
					{
						itemnumber = 0;
						position++;
					}
				}
				else
				{
					CString errstr;
					errstr.Format("%d", index);
					errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}

				index++;
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	m_end_row = position;
	SetWidth(gAlpha[0], gAlpha[m_last_column], 13.4);
	SetHeight(1, position, 49.3);
	progress.ProgressForward();

	FinalizeTagReport();
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::StockTags_AddItem(CProductItem *item, CProductVoucher *voucher, int column, int row, bool show_values)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	int firstcol = column;
	int secondcol = column + 1;
	
	CString firstside = "";

	{
		CString temp = "";

		//PRODUCTID
		firstside = item->m_product_ID;

		//NUMBER OF PIECES
		temp.Format("%d",item->m_piece_count);
		firstside = firstside + " - " + temp + "\n";

		//GROSS WEIGHT
		temp.Format("%.3f",item->GetGrossWeight());
		firstside = firstside + "GWt:" + temp + "\n";

		if(show_values)
		{
			//GOLD VALUE
			temp.Format("%d",item->GetGoldValue(item->m_use_net_weight_for_gold, TRUE, GetGoldRate(voucher), 1.0) + item->GetMarkedLabourCost(GetGoldRate(voucher), 1.0));
			firstside = firstside + "MM:" + temp;

			//LABOUR COST
			temp.Format("%d",item->GetMarkedLabourCost(GetGoldRate(voucher), 1.0));
			firstside = firstside + " / " + temp + "\n";

			//TOTAL VALUE
			temp.Format("%d", item->GetTotalMarkedValue(item->m_use_net_weight_for_gold, TRUE, GetGoldRate(voucher), 1.0));
			firstside = firstside + "TV:" + temp + "\n";
		}
	}

	CString secondside;

	{
		CString temp;

		//PRODUCTID
		secondside = item->m_product_ID;

		//NUMBER OF PIECES
		temp.Format("%d",item->m_piece_count);
		secondside = secondside + " " + temp + "\n";

		//DIAMOND WEIGHT
		int count = 0;
		while(count < item->GetSubItemCount(tDiamond))
		{
			CString type, rate, weight;
			type = item->GetSubItemTypeOf(tDiamond, count);
			weight.Format("%.02f", item->GetSubItemWeightOf(tDiamond, count));
			rate.Format("%d", item->GetMarkedSubItemRateOf(tDiamond, count, GetGoldRate(voucher), 1.0));
			secondside = secondside + type + " -" + weight + " -" + rate + "\n";
			count++;
		}
		
		//STONE WEIGHT
		count = 0;
		while(count < item->GetSubItemCount(tStone))
		{
			CString type, rate, weight;
			type = item->GetSubItemTypeOf(tStone, count);
			weight.Format("%.02f", item->GetSubItemWeightOf(tStone, count));
			rate.Format("%d", item->GetMarkedSubItemRateOf(tStone, count, GetGoldRate(voucher), 1.0));
			secondside = secondside + type + " -" + weight + " -" + rate + "\n";
			count++;
		}

		//PEARL WEIGHT
		count = 0;
		while(count < item->GetSubItemCount(tPearl))
		{
			CString type, rate, weight;
			type = "P";
			weight.Format("%.02f", item->GetSubItemWeightOf(tPearl, count));
			rate.Format("%d", item->GetMarkedSubItemRateOf(tPearl, count, GetGoldRate(voucher), 1.0));
			secondside = secondside + type + " -" + weight + " -" + rate + "\n";
			count++;
		}
	}

	FillCell(gAlpha[firstcol], row, firstside);
	FillCell(gAlpha[secondcol], row, secondside);

	SetAlignment(gAlpha[firstcol], row, CenterAlign, CenterAlign);
	SetAlignment(gAlpha[secondcol], row, CenterAlign, CenterAlign);
	SetTextWrap(gAlpha[firstcol], row, gAlpha[secondcol], row, TRUE);
	SetFontSize(gAlpha[firstcol], row, gAlpha[secondcol], row, 6);
}


BOOL CRatnaKiranReports::CreateCatalogue()
{
	//salil
	return TRUE;
}


BOOL CRatnaKiranReports::CreateKundanReportActual(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	common_serial = 1;

	CProgressDialog progress("Processing report", m_list->GetCount() + 3);
	progress.StartDialog();

	m_start_row = 5;
	KundanReport_Actual_AddHeaders();
	KundanReport_Actual_AddTitle(type);
	progress.ProgressForward();

	int position = m_start_row;
	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				position = KundanReport_Actual_AddSet(set, voucher, position, type);
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	position = ProcessTotals(position);
	m_end_row = position;
	progress.ProgressForward();

	ProcessAutoFit();
	progress.ProgressForward();

	FinalizeLandscapeReport();
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::KundanReport_Actual_AddTitle(ReportType type)
{
	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 1);
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);

	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "KUNDAN LIST (Actual rates/values)");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "KUNDAN LIST OF SOLD ITEMS (Actual rates/values)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "KUNDAN LIST OF UNSOLD ITEMS (Actual rates/values)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign,CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
}


void CRatnaKiranReports::KundanReport_Actual_AddHeaders()
{
	int position = m_start_row - 2;
	int column = 0;

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "S.No.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "ProductID");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Item");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Desc.");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Pcs.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "GWt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Costing (per Gm.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Total price (Rs.)");

	m_last_column = column - 1;

	SetBorders(gAlpha[0], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[0], position, gAlpha[m_last_column], position + 1);
}


//Returns the position where the next entry can be made
int CRatnaKiranReports::KundanReport_Actual_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type)
{
	if(!set || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	if(type == Sold_Report && set->GetSoldItemCount() == 0)
		return position;
	else if(type == UnSold_Report && set->GetUnsoldItemCount() == 0)
		return position;

	int pos = position;

	FillCell(gAlpha[0], position, common_serial);
	FillCell(gAlpha[1], position, set->m_product_ID);
	common_serial++;

	int index = 0;
	while(index < set->GetItemCount())
	{
		CProductItem *item = set->GetItemAt(index);
		if(item)
		{
			if(((type == Sold_Report) && item->IsItemSold()) ||
			   ((type == UnSold_Report) && !(item->IsItemSold())) ||
				type == Original_Report)
			{
				pos = KundanReport_Actual_AddItem(item, voucher, pos);
			} 
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		index++;
	}

	if(pos == position)
		pos++;

	SetBorder(gAlpha[0], position, gAlpha[m_last_column], pos - 1, AllBorders, TRUE, 1);
	return pos;
}

int CRatnaKiranReports::KundanReport_Actual_AddItem(CProductItem *item, CProductVoucher *voucher, int position)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int column = 2;

	FillCell(gAlpha[column++], position, item->m_description);
	FillCell(gAlpha[column++], position, item->m_piece_count);
	FillCell(gAlpha[column++], position, item->GetGrossWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetCosting(1.0));
	FillCell(gAlpha[column++], position, item->GetTotalCosting(1.0));

	int newpos = position + 1;
	return newpos;
}

BOOL CRatnaKiranReports::CreateKundanReportMarked(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	common_serial = 1;

	CProgressDialog progress("Processing report", m_list->GetCount() + 3);
	progress.StartDialog();

	m_start_row = 5;
	KundanReport_Marked_AddHeaders();
	KundanReport_Marked_AddTitle(type);
	progress.ProgressForward();

	int position = m_start_row;
	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				position = KundanReport_Marked_AddSet(set, voucher, position, type);
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	position = ProcessTotals(position);
	m_end_row = position;
	progress.ProgressForward();

	ProcessAutoFit();
	progress.ProgressForward();

	FinalizeLandscapeReport();
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::KundanReport_Marked_AddTitle(ReportType type)
{
	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 1);
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);

	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "KUNDAN LIST (Marked rates/values)");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "KUNDAN LIST OF SOLD ITEMS (Marked rates/values)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "KUNDAN LIST OF UNSOLD ITEMS (Marked rates/values)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign,CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
}


void CRatnaKiranReports::KundanReport_Marked_AddHeaders()
{
	int position = m_start_row - 2;
	int column = 0;

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "S.No.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "ProductID");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Item");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Desc.");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Pcs.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "GWt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Costing (per Gm.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Total price (Rs.)");

	m_last_column = column - 1;

	SetBorders(gAlpha[0], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[0], position, gAlpha[m_last_column], position + 1);
}


//Returns the position where the next entry can be made
int CRatnaKiranReports::KundanReport_Marked_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type)
{
	if(!set || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	if(type == Sold_Report && set->GetSoldItemCount() == 0)
		return position;
	else if(type == UnSold_Report && set->GetUnsoldItemCount() == 0)
		return position;

	int pos = position;

	FillCell(gAlpha[0], position, common_serial);
	FillCell(gAlpha[1], position, set->m_product_ID);
	common_serial++;

	int index = 0;
	while(index < set->GetItemCount())
	{
		CProductItem *item = set->GetItemAt(index);
		if(item)
		{
			if(((type == Sold_Report) && item->IsItemSold()) ||
			   ((type == UnSold_Report) && !(item->IsItemSold())) ||
				type == Original_Report)
			{
				pos = KundanReport_Marked_AddItem(item, voucher, pos);
			}
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		index++;
	}

	if(pos == position)
		pos++;

	SetBorder(gAlpha[0], position, gAlpha[m_last_column], pos - 1, AllBorders, TRUE, 1);
	return pos;
}


int CRatnaKiranReports::KundanReport_Marked_AddItem(CProductItem *item, CProductVoucher *voucher, int position)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int column = 2;

	FillCell(gAlpha[column++], position, item->m_description);
	FillCell(gAlpha[column++], position, item->m_piece_count);
	FillCell(gAlpha[column++], position, item->GetGrossWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetMarkedCosting(1.0));
	FillCell(gAlpha[column++], position, item->GetTotalMarkedCosting(1.0));

	int newpos = position + 1;
	return newpos;
}


BOOL CRatnaKiranReports::CreateKundanChecklist(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	common_serial = 1;

	CProgressDialog progress("Processing report", m_list->GetCount() + 3);
	progress.StartDialog();

	m_start_row = 5;
	KundanChecklist_AddHeaders();
	KundanChecklist_AddTitle(type);
	progress.ProgressForward();

	int position = m_start_row;
	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				position = KundanChecklist_AddSet(set, voucher, position, type);
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	position = ProcessTotals(position);
	m_end_row = position;
	progress.ProgressForward();

	ProcessAutoFit();
	progress.ProgressForward();

	FinalizeLandscapeReport();
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::KundanChecklist_AddTitle(ReportType type)
{
	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 1);
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);

	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "KUNDAN CHECKLIST");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "KUNDAN CHECKLIST (Sold)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "KUNDAN CHECKLIST (UnSold)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign,CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
}


void CRatnaKiranReports::KundanChecklist_AddHeaders()
{
	int position = m_start_row - 2;
	int column = 0;

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "S.No.");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Set");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "ProductID");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Desc.");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Item");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Desc.");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Pcs.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "GWt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Costing (per Gm.)");

	MergeCells(gAlpha[column], position, gAlpha[column + 2], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Diamond");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Type");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column+ 2], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pr/SPr Stone");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Type");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pearl");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Other Wt. (Gms.)");

	m_last_column = column - 1;

	SetBorders(gAlpha[0], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[0], position, gAlpha[m_last_column], position + 1);
}


//Returns the position where the next entry can be made
int CRatnaKiranReports::KundanChecklist_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type)
{
	if(!set || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int pos = position;

	FillCell(gAlpha[0], position, common_serial);
	FillCell(gAlpha[1], position, set->m_product_ID);
	FillCell(gAlpha[2], position, set->m_description);
	common_serial++;

	int index = 0;
	while(index < set->GetItemCount())
	{
		CProductItem *item = set->GetItemAt(index);
		if(item)
		{
			if(((type == Sold_Report) && item->IsItemSold()) ||
			   ((type == UnSold_Report) && !(item->IsItemSold())) ||
				type == Original_Report)
			{
				pos = KundanChecklist_AddItem(item, voucher, pos);
			}
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		index++;
	}

	if(pos == position)
		pos++;

	SetBorder(gAlpha[0], position, gAlpha[m_last_column], pos - 1, AllBorders, TRUE, 1);
	return pos;
}

int CRatnaKiranReports::KundanChecklist_AddItem(CProductItem *item, CProductVoucher *voucher, int position)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int column = 3;

	FillCell(gAlpha[column++], position, item->m_description);
	FillCell(gAlpha[column++], position, item->m_piece_count);
	FillCell(gAlpha[column++], position, item->GetGrossWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetCosting(1.0));

	int index = 0;
	while(index < item->GetSubItemCount(tDiamond))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemTypeOf(tDiamond, index));
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemWeightOf(tDiamond, index), 2);
		FillCell(gAlpha[column + 2], position + index, item->GetSubItemRateOf(tDiamond, index, 1.0));

		index++;
	}

	column += 3;

	index = 0;
	while(index < item->GetSubItemCount(tStone))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemTypeOf(tStone, index));
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemWeightOf(tStone, index), 2);
		FillCell(gAlpha[column + 2], position + index, item->GetSubItemRateOf(tStone, index, 1.0));

		index++;
	}

	column += 3;

	index = 0;
	while(index < item->GetSubItemCount(tPearl))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemWeightOf(tPearl, index), 2);
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemRateOf(tPearl, index, 1.0));

		index++;
	}

	column += 2;

	FillCell(gAlpha[column++], position, item->GetOtherWeight(), 3);

	int maxval = max(item->GetSubItemCount(tDiamond), item->GetSubItemCount(tStone));
	maxval = max(maxval, item->GetSubItemCount(tPearl));
	maxval = max(1, maxval);

	return (position + maxval);//this takes care of going to the next empty position to add the next item.
}


BOOL CRatnaKiranReports::CreateKundanTags(int row, int column)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	m_start_row = 1;
	m_last_column = 5;

	int position = row;
	int itemnumber = (column - 1)/2;

	CProgressDialog progress("Processing report", m_list->GetCount() + 1);
	progress.StartDialog();

	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			int index = 0;
			while(index < set->GetItemCount())
			{
				CProductItem *item = set->GetItemAt(index);
				if(item)
				{
					KundanTags_AddItem(item, voucher, (2 * itemnumber), position);
					itemnumber++;

					if(itemnumber == 3)
					{
						itemnumber = 0;
						position++;
					}
				}
				else
				{
					CString errstr;
					errstr.Format("%d", index);
					errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}

				index++;
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	m_end_row = position;
	SetWidth(gAlpha[0], gAlpha[m_last_column], 13.4);
	SetHeight(1, position, 49.3);
	progress.ProgressForward();

	FinalizeTagReport();
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::KundanTags_AddItem(CProductItem *item, CProductVoucher *voucher, int column, int row)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	int firstcol = column;
	int secondcol = column + 1;
	
	CString firstside;

	{
		CString temp;

		//PRODUCTID
		firstside = item->m_product_ID;

		//NUMBER OF PIECES
		temp.Format("%d",item->m_piece_count);
		firstside = firstside + " - " + temp + "\n";

		//GROSS WEIGHT
		temp.Format("%.3f",item->GetGrossWeight());
		firstside = firstside + "GWt:" + temp + "\n";

		//COSTING
		temp.Format("%d", item->GetMarkedCosting(1.0));
		firstside = firstside + "Costing:" + temp;

		//TOTAL VALUE
		temp.Format("%d", item->GetTotalMarkedCosting(1.0));
		firstside = firstside + "TV:" + temp + "\n";
	}

	FillCell(gAlpha[firstcol], row, firstside);
	SetAlignment(gAlpha[firstcol], row, CenterAlign, CenterAlign);
	SetTextWrap(gAlpha[firstcol], row, gAlpha[secondcol], row, TRUE);
	SetFontSize(gAlpha[firstcol], row, gAlpha[secondcol], row, 6);
}


CProductItem *CRatnaKiranReports::GetLowestSerialNumberItem(CList<Item_Voucher *, Item_Voucher *> *list, CProductVoucher **voucher)
{
	if(!list)
	{
		SHOW_INTERNAL_ERROR;
		return NULL;
	}

	Item_Voucher *ret_item = NULL;
	POSITION ret_pos = NULL;
	int min_serial = -1;

	POSITION pos = list->GetHeadPosition();
	while(pos)
	{
		Item_Voucher *tempitem = list->GetAt(pos);
		if(tempitem && tempitem->item)
		{
			if((min_serial == -1) || (tempitem->item->m_serial_number < min_serial))
			{
				ret_item = tempitem;
				ret_pos = pos;
				min_serial = tempitem->item->m_serial_number;
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		list->GetNext(pos);
	}

	if(ret_pos)
		list->RemoveAt(ret_pos);

	if(ret_item && voucher)
		*voucher = ret_item->voucher;

	CProductItem *val = ret_item->item;
	delete ret_item;
	return val;
}


CProductSet *CRatnaKiranReports::GetLowestSerialNumberSet(CList<Set_Voucher *, Set_Voucher *> *list, CProductVoucher **voucher)
{
	if(!list)
	{
		SHOW_INTERNAL_ERROR;
		return NULL;
	}

	Set_Voucher *ret_set = NULL;
	POSITION ret_pos = NULL;
	int min_serial = -1;

	POSITION pos = list->GetHeadPosition();
	while(pos)
	{
		Set_Voucher *tempset = list->GetAt(pos);
		if(tempset && tempset->set)
		{
			CProductItem *tempitem = tempset->set->GetItemAt(0);
			if(tempitem)
			{
				if((min_serial == -1) || (tempitem->m_serial_number < min_serial))
				{
					ret_set = tempset;
					ret_pos = pos;
					min_serial = tempitem->m_serial_number;
				}
			}
			else
			{
				CString errstr = "Could not get first item in set '" + CString(tempset->set->m_product_ID) + "'.";
				SHOW_INTERNAL_ERROR_REASON(errstr);
			}
		}
		else
			SHOW_INTERNAL_ERROR;

		list->GetNext(pos);
	}

	if(ret_pos)
		list->RemoveAt(ret_pos);

	if(ret_set && voucher)
		*voucher = ret_set->voucher;

	CProductSet *val = ret_set->set;
	delete ret_set;
	return val;
}


BOOL CRatnaKiranReports::CalculateExportValues(double *gold_rate, double *currency_conversion, double *labourvalue, double *pearlvalue, 
											   double *diamondvalue, double *stonevalue, double *pricequote, CProductItem *item, CProductVoucher *voucher)
{
	if(!voucher)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	double gold = GetGoldRate(voucher);
	double conversion;
	
	if(CSettings::GetUseNativeCurrency())
		conversion = voucher->m_currency_conversion;
	else
		conversion = voucher->m_dollar_rate;

	if(gold_rate)
	{
        *gold_rate = gold;
	}

	if(currency_conversion)
	{
        *currency_conversion = conversion;
	}

	if(!item)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(labourvalue)
	{
        CString labour = voucher->m_labour_cost;
		double value = item->GetLabourCostDouble(conversion);

		if(!labour.IsEmpty())
		{
			ReplaceDollarWithValue(&labour, item->GetLabourCostDouble(conversion));
			value = ProcessFormula(labour, gold, conversion, item);
		}

		*labourvalue = value;
	}

	if(pearlvalue)
	{
		CString pearl = voucher->m_pearl_rate;
		double value = item->GetSubItemValueDouble(tPearl, conversion);

		if(!pearl.IsEmpty())
		{
			value = 0;
			int count = 0;
			while(count < item->GetSubItemCount(tPearl))
			{
				int rate;
				CString str = pearl;
				ReplaceDollarWithValue(&str, item->GetSubItemRateOf(tPearl, count, 1.0));
				rate = (int)(ProcessFormula(str, gold, 1.0, item) + 0.5);
				value += item->GetSubItemValueOfUsingDouble(tPearl, count, rate, conversion);
				count++;
			}
		}

		*pearlvalue = value;
	}

	if(diamondvalue)
	{
		CString diamond = voucher->m_diamond_rate;
		double value = item->GetSubItemValueDouble(tDiamond, conversion);
		
		if(!diamond.IsEmpty())
		{
			value = 0;
			int count = 0;
			while(count < item->GetSubItemCount(tDiamond))
			{
				int rate;
				CString str = diamond;
				ReplaceDollarWithValue(&str, item->GetSubItemRateOf(tDiamond, count, 1.0));
				rate = (int)(ProcessFormula(str, gold, 1.0, item) + 0.5);
				value += item->GetSubItemValueOfUsingDouble(tDiamond, count, rate, conversion);
				count++;
			}
		}

		*diamondvalue = value;
	}

	if(stonevalue)
	{
		CString stone = voucher->m_stone_rate;
		double value = item->GetSubItemValueDouble(tStone, conversion);

		if(!stone.IsEmpty())
		{
			value = 0;
			int count = 0;
			while(count < item->GetSubItemCount(tStone))
			{
				int rate;
				CString str = stone;
				ReplaceDollarWithValue(&str, item->GetSubItemRateOf(tStone, count, 1.0));
				rate = (int)(ProcessFormula(str, gold, 1.0, item) + 0.5);
				value += item->GetSubItemValueOfUsingDouble(tStone, count, rate, conversion);
				count++;
			}
		}

		*stonevalue = value;
	}

	if(pricequote)
	{
		CString price = voucher->m_price_quote;
		double value = item->GetTotalValueDouble(TRUE, FALSE, gold, conversion);

		if(!price.IsEmpty())
			value = ProcessFormula(price, gold, conversion, item);

		*pricequote = value;
	}

	return TRUE;
}


BOOL CRatnaKiranReports::CreateExportPackageList(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CProgressDialog progress1("Processing report", m_list->GetCount() + 1);
	progress1.StartDialog();

	m_start_row = 17;
	ExportPackagelist_AddHeaders();
	ExportPackagelist_AddTitle(type);
	progress1.ProgressForward();

	int position = m_start_row;
	CList<Item_Voucher *, Item_Voucher *> itemlist;
	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				int index = 0;
				while(index < set->GetItemCount())
				{
					CProductItem *item = set->GetItemAt(index);
					if(item)
					{
						if((type == Sold_Report && item->IsItemSold()) ||
						   (type == UnSold_Report && !(item->IsItemSold())) ||
						   (type == Original_Report))
						{
							if((item->m_serial_number >= m_serials_from) &&
							   ((item->m_serial_number <= m_serials_to) || (m_serials_to == -1)))
							{
								Item_Voucher *newnode;
								SALIL_NEW(newnode, Item_Voucher);
								newnode->item = item;
								newnode->voucher = voucher;

								itemlist.AddHead(newnode);
							}
						}
					}
					else
					{
						CString errstr;
						errstr.Format("%d", index);
						errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
					}

					index++;
				}
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress1.ProgressForward();
	}

	progress1.FinishDialog();
	CProgressDialog progress2("Processing report", itemlist.GetCount() + 2);
	progress2.StartDialog();

	while(!itemlist.IsEmpty())
	{
		CProductVoucher *voucher = NULL;
		CProductItem *item = GetLowestSerialNumberItem(&itemlist, &voucher);
		if(item && voucher)
			position = ExportPackagelist_AddItem(item, voucher, position);
		else
			SHOW_INTERNAL_ERROR;

		progress2.ProgressForward();
	}

	position = ProcessTotals(position);
	m_end_row = position;
	progress2.ProgressForward();

	ProcessAutoFit();
	progress2.ProgressForward();

	FinalizeLandscapeReport(TRUE);
	progress2.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::ExportPackagelist_AddTitle(ReportType type)
{
	CString consignee = gDoc->m_selected_voucher->m_consignee;
	CString exporter = gDoc->m_selected_voucher->m_exporter;
	CString invoice_number = gDoc->m_selected_voucher->m_invoice_number;

	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 12);

	MergeCells(gAlpha[0], 3, gAlpha[3], 6);
	FillCell(gAlpha[0], 3, CString("Exporter: ") + exporter);
	SetAlignment(gAlpha[0], 3, LeftAlign, CenterAlign);

	MergeCells(gAlpha[0], 8, gAlpha[3], 12);
	FillCell(gAlpha[0], 8, CString("Consignee: ") + consignee);
	SetAlignment(gAlpha[0], 8, LeftAlign, CenterAlign);
	SetTextWrap(gAlpha[0], 8, gAlpha[3], 12);
	
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);
	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "PACKING LIST");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "PACKING LIST (Sold)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "PACKING LIST (UnSold)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign, CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);

	MergeCells(gAlpha[m_last_column - 3], 3, gAlpha[m_last_column], 3);

	//CTime systemtime = CTime::GetCurrentTime();
	//FillCell(gAlpha[m_last_column - 3], 3, CString("DATE: ") + systemtime.Format("%d-%m-%Y"));
	
	FillCell(gAlpha[m_last_column - 3], 3, CString("DATE:           "));
	SetAlignment(gAlpha[m_last_column - 3], 3, RightAlign, CenterAlign);

	MergeCells(gAlpha[m_last_column - 3], 5, gAlpha[m_last_column], 5);
	FillCell(gAlpha[m_last_column - 3], 5, CString("INVOICE NUMBER: "));
	SetAlignment(gAlpha[m_last_column - 3], 5, RightAlign, CenterAlign);
	SetTextWrap(gAlpha[m_last_column - 3], 5, gAlpha[m_last_column], 5);

	MergeCells(gAlpha[m_last_column - 3], 6, gAlpha[m_last_column], 6);
	FillCell(gAlpha[m_last_column - 3], 6, invoice_number);
	SetAlignment(gAlpha[m_last_column - 3], 6, RightAlign, CenterAlign);
	SetTextWrap(gAlpha[m_last_column - 3], 6, gAlpha[m_last_column], 6);
}


void CRatnaKiranReports::ExportPackagelist_AddHeaders()
{
	CString currency;
	if(CSettings::GetUseNativeCurrency())
		currency = gDoc->m_selected_voucher->m_currency_name;
	else
		currency = "USD";

	int position = m_start_row - 2;
	int column = 0;

	if(m_use_product_id)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "ProductID");
	}

	if(m_use_new_serials)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Serial no.");
	}

	if(m_use_old_serials)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Old Serial no.");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Desc. of product");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Pcs.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Gross Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Net Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, CString("Gold value (") + currency + CString(")"));

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Diamond");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Weight (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, CString("Value (") + currency + CString(")"));

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pr/SPr Stone");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Weight (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, CString("Value (") + currency + CString(")"));

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pearl");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Weight (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, CString("Value (") + currency + CString(")"));

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Other Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, CString("Polishing (") + currency + CString(")"));

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, CString("Total price (") + currency + CString(")"));

	m_last_column = column - 1;

	SetBorders(gAlpha[0], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[0], position, gAlpha[m_last_column], position + 1);
}


int CRatnaKiranReports::ExportPackagelist_AddItem(CProductItem *item, CProductVoucher *voucher, int position)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	double gold_rate, conversion, labourvalue, pearlvalue, diamondvalue, stonevalue;
	if(!CalculateExportValues(&gold_rate, &conversion, &labourvalue, &pearlvalue, &diamondvalue, &stonevalue, NULL, item, voucher))
	{
		CString errstr = "Could not calculate export values for item " + CString(item->m_product_ID) + ".";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return position;
	}

	double goldvalue = item->GetGoldValueDouble(TRUE, FALSE, gold_rate, conversion);

	goldvalue = RoundOff(goldvalue, 2);
	diamondvalue = RoundOff(diamondvalue, 2);
	stonevalue = RoundOff(stonevalue, 2);
	labourvalue = RoundOff(labourvalue, 2);
	pearlvalue = RoundOff(pearlvalue, 2);

	double total =  goldvalue + diamondvalue + stonevalue + labourvalue + pearlvalue;
	int column = 0;

	if(m_use_product_id)
	{
		FillCell(gAlpha[column++], position, item->m_product_ID);
	}

	if(m_use_new_serials)
	{
		FillCell(gAlpha[column++], position, item->m_serial_number);
	}

	if(m_use_old_serials)
	{
		CString tagname;

		if(gDoc->m_selected_voucher->IsRootVoucher())
		{
			CString voucher_name = voucher->m_voucher_name;
			POSITION pos = m_old_serial_tagnames->GetHeadPosition();
			while(pos)
			{
				SStringPairs details = m_old_serial_tagnames->GetAt(pos);
				if(details.m_string1 == voucher_name)
				{
					tagname = details.m_string2;
					break;
				}

				m_old_serial_tagnames->GetNext(pos);
			}

			if(pos == NULL) //not found
				tagname = "";
		}
		else
		{
			tagname = m_old_serial_tagname;
		}

		CProductOldSerial *serial = NULL;
		if(tagname.IsEmpty())
		{
			int count = item->GetOldSerialCount();
			if(count > 0)
				serial = item->GetOldSerialAt(count - 1); //get last added
		}
		else
			serial = item->GetOldSerial(tagname);

		if(serial)
			FillCell(gAlpha[column++], position, serial->m_serial);
		else
			FillCell(gAlpha[column++], position, "");
	}

	FillCell(gAlpha[column++], position, item->m_description);
	FillCell(gAlpha[column++], position, item->m_piece_count);
	FillCell(gAlpha[column++], position, item->GetGrossWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetNetWeight(), 3);
	FillCell(gAlpha[column++], position, goldvalue, 2);
	FillCell(gAlpha[column++], position, item->GetSubItemWeight(tDiamond), 2);
	FillCell(gAlpha[column++], position, diamondvalue, 2);
	FillCell(gAlpha[column++], position, item->GetSubItemWeight(tStone), 2);
	FillCell(gAlpha[column++], position, stonevalue, 2);
	FillCell(gAlpha[column++], position, item->GetSubItemWeight(tPearl), 2);
	FillCell(gAlpha[column++], position, pearlvalue, 2);
	FillCell(gAlpha[column++], position, item->GetOtherWeight(), 3);
	FillCell(gAlpha[column++], position, labourvalue, 2);
	FillCell(gAlpha[column++], position, total, 2);

	SetBorder(gAlpha[0], position, gAlpha[m_last_column], position, AllBorders, TRUE, 1);
	return (position + 1);
}


BOOL CRatnaKiranReports::CreateExportShortInvoice(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CProgressDialog progress("Processing report", m_list->GetCount() + 1);
	progress.StartDialog();

	m_start_row = 3;
	m_end_row = 58;
	m_last_column = 10;

	int pieces = 0;
	double gross_weight = 0.0;
	double net_weight = 0.0;
	double net_value = 0.0;
	double diamond_weight = 0.0;
	double diamond_value = 0.0;
	double stone_weight = 0.0;
	double stone_value = 0.0;
	double pearl_weight = 0.0;
	double pearl_value = 0.0;
	double other_weight = 0.0;
	double value_addition = 0.0;
	double total_value = 0.0;

	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			int index = 0;
			while(index < set->GetItemCount())
			{
				CProductItem *item = set->GetItemAt(index);
				if(item)
				{
					if((type == Original_Report) ||
					   ((type == Sold_Report) && item->IsItemSold()) ||
					   ((type == UnSold_Report) && !(item->IsItemSold())))
					{
						if((item->m_serial_number >= m_serials_from) &&
						((m_serials_to == -1) || (item->m_serial_number <= m_serials_to)))
						{
							ExportShortInvoice_AddItem(item, voucher, &pieces, &gross_weight, &net_weight, &net_value, &diamond_weight, &diamond_value,
														&stone_weight, &stone_value, &pearl_weight, &pearl_value, &other_weight, &value_addition, &total_value);
						}
					}
				}
				else
				{
					CString errstr;
					errstr.Format("%d", index);
					errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}

				index++;
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	ExportShortInvoice_FillValues(pieces, gross_weight, net_weight, net_value, diamond_weight, diamond_value,
							stone_weight, stone_value, pearl_weight, pearl_value, other_weight, value_addition, total_value, type);
	progress.ProgressForward();

	FinalizePotraitReport(FALSE);
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::ExportShortInvoice_AddItem(CProductItem *item, CProductVoucher *voucher,
												    int *pieces, double *gross_weight,
													double *net_weight, double *net_value,
													double *diamond_weight, double *diamond_value,
													double *stone_weight, double *stone_value,
													double *pearl_weight, double *pearl_value,
													double *other_weight, double *value_addition,
													double *total_value)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	double gold_rate, conversion, labourvalue, pearlvalue, diamondvalue, stonevalue;
	if(!CalculateExportValues(&gold_rate, &conversion, &labourvalue, &pearlvalue, &diamondvalue, &stonevalue, NULL, item, voucher))
	{
		CString errstr = "Could not calculate export values for item " + CString(item->m_product_ID) + ".";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}

	double goldvalue = item->GetGoldValueDouble(TRUE, FALSE, gold_rate, conversion);

	goldvalue = RoundOff(goldvalue, 2);
	diamondvalue = RoundOff(diamondvalue, 2);
	stonevalue = RoundOff(stonevalue, 2);
	labourvalue = RoundOff(labourvalue, 2);
	pearlvalue = RoundOff(pearlvalue, 2);

	double total =  goldvalue + diamondvalue + stonevalue + labourvalue + pearlvalue;

	*pieces = *pieces + item->m_piece_count;
	*gross_weight = *gross_weight + RoundOff(item->GetGrossWeight(), 3);
	*net_weight = *net_weight + RoundOff(item->GetNetWeight(), 3);
	*net_value = *net_value + goldvalue;
	*diamond_weight = *diamond_weight + RoundOff(item->GetSubItemWeight(tDiamond), 2);
	*diamond_value = *diamond_value + diamondvalue;
	*stone_weight = *stone_weight + RoundOff(item->GetSubItemWeight(tStone), 2);
	*stone_value = *stone_value + stonevalue;
	*pearl_weight = *pearl_weight + RoundOff(item->GetSubItemWeight(tPearl), 2);
	*pearl_value = *pearl_value + pearlvalue;
	*other_weight = *other_weight + RoundOff(item->GetOtherWeight(), 3);
	*value_addition = *value_addition + labourvalue;
	*total_value = *total_value + RoundOff(total, 2);
}


void CRatnaKiranReports::ExportShortInvoice_CreateOutline(char *startcolumn, int startrow, char *endcolumn, int endrow, BOOL hasEntry, BOOL drawBorder)
{
	if(hasEntry)
	{
		MergeCells(startcolumn, startrow, endcolumn, startrow);
		SetAlignment(startcolumn, startrow, LeftAlign, TopAlign);
		MergeCells(startcolumn, startrow + 1, endcolumn, endrow);
		SetAlignment(startcolumn, startrow + 1, LeftAlign, TopAlign);
	}
	else
	{
		MergeCells(startcolumn, startrow, endcolumn, endrow);
		SetAlignment(startcolumn, startrow, LeftAlign, TopAlign);
	}

	if(drawBorder)
		SetBorder(startcolumn, startrow, endcolumn, endrow, AllBorders, TRUE, 1);
}


void CRatnaKiranReports::ExportShortInvoice_FillValues(int pieces, double gross_weight, 
													   double net_weight, double net_value,
													   double diamond_weight, double diamond_value,
													   double stone_weight, double stone_value,
													   double pearl_weight, double pearl_value,
													   double other_weight, double value_addition,
													   double total_value, ReportType type)
{
	CString consignee = gDoc->m_selected_voucher->m_consignee;
	CString exporter = gDoc->m_selected_voucher->m_exporter;
	CString invoice_number = gDoc->m_selected_voucher->m_invoice_number;
	CString country = gDoc->m_selected_voucher->m_country;
	CTime systemtime = CTime::GetCurrentTime();
	CString date = "          "; //systemtime.Format("%d-%m-%Y");
	CString str;

	CString currency;
	if(CSettings::GetUseNativeCurrency())
		currency = gDoc->m_selected_voucher->m_currency_name;
	else
		currency = "USD";

	double conversion;
	if(CSettings::GetUseNativeCurrency())
		conversion = gDoc->m_selected_voucher->m_currency_conversion;
	else
		conversion = gDoc->m_selected_voucher->m_dollar_rate;

	double gold_rate = (GetGoldRate(gDoc->m_selected_voucher) * 40.0) / (conversion * 3.0);

	CShortInvoiceDialog dlg;
	dlg.DoModal();

	//HEADING
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);
	SetAlignment(gAlpha[0], 1, CenterAlign, CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
	if(type == Original_Report)
		FillCell(gAlpha[0], 1, "SHORT INVOICE");
	else if(type == Sold_Report)
		FillCell(gAlpha[0], 1, "SHORT INVOICE (SOLD)");
	else if(type == UnSold_Report)
		FillCell(gAlpha[0], 1, "SHORT INVOICE (UNSOLD)");
	else
	{
		CString errstr;
		errstr.Format("%d", type);
		errstr = "Invalid report type '" + errstr + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
	}

	//EXPORTER
	ExportShortInvoice_CreateOutline("A", 3, "D", 9, TRUE, TRUE);
	FillCell("A", 3, "Exporter:");
	FillCell("A", 4, exporter);

	//INVOICE NUMBER	
	ExportShortInvoice_CreateOutline("E", 3, "G", 5, TRUE, TRUE);
	FillCell("E", 3, "Invoice No. & Date");
	FillCell("E", 4, invoice_number + "\nDt. " + date);
	
	//EXPORTER's REF. GR No.
	ExportShortInvoice_CreateOutline("H", 3, "K", 5, TRUE, TRUE);
	FillCell("H", 3, "Exporter's Ref. GR No.");
	FillCell("H", 4, dlg.m_exporter_gr_number);

	//BUYER's ORDER No.
	ExportShortInvoice_CreateOutline("E", 6, "K", 7, TRUE, TRUE);
	FillCell("E", 6, "Buyer's Order No. & Date");
	FillCell("E", 7, dlg.m_order_number);

	//OTHER REFERENCE
	ExportShortInvoice_CreateOutline("E", 8, "K", 9, TRUE, TRUE);
	FillCell("E", 8, "Other Reference(s)");
	FillCell("E", 9, "I.E.C. Code No. 0588053554");

	//CONSIGNEE
	ExportShortInvoice_CreateOutline("A", 10, "D", 17, TRUE, TRUE);
	FillCell("A", 10, "Consignee:");
	FillCell("A", 11, consignee);

	//BUYER
	ExportShortInvoice_CreateOutline("E", 10, "K", 15, TRUE, TRUE);
	FillCell("E", 10, "Buyer (if other than consignee)");
	FillCell("E", 11, dlg.m_buyer);

	//COUNTRY OF ORIGIN
	ExportShortInvoice_CreateOutline("E", 16, "G", 17, TRUE, TRUE);
	FillCell("E", 16, "Country of Origin");
	FillCell("E", 17, "INDIA");

	//COUNTRY OF FINAL DESTINATION
	ExportShortInvoice_CreateOutline("H", 16, "K", 17, TRUE, TRUE);
	FillCell("H", 16, "Country of final destination");
	FillCell("H", 17, country);

	//PRE-CARRIAGE BY
	ExportShortInvoice_CreateOutline("A", 18, "B", 19, TRUE, TRUE);
	FillCell("A", 18, "Pre-Carriage by");
	FillCell("A", 19, dlg.m_precarriage);

	//PLACE OF RECEIPT OF PRE-CARRIER
	ExportShortInvoice_CreateOutline("C", 18, "D", 19, TRUE, TRUE);
	FillCell("C", 18, "Place of Receipt");// by Pre-Carrier");
	FillCell("C", 19, dlg.m_place_receipt);

	//VESSEL/FLIGHT No.
	ExportShortInvoice_CreateOutline("A", 20, "B", 21, TRUE, TRUE);
	FillCell("A", 20, "Vessel/Flight No.");
	FillCell("A", 21, dlg.m_vessel);

	//PORT OF LOADING
	ExportShortInvoice_CreateOutline("C", 20, "D", 21, TRUE, TRUE);
	FillCell("C", 20, "Port of loading");
	FillCell("C", 21, "NEW DELHI");

	//PORT OF DISCHARGE
	ExportShortInvoice_CreateOutline("A", 22, "B", 23, TRUE, TRUE);
	FillCell("A", 22, "Port of Discharge");
	FillCell("A", 23, country);

	//FINAL DESTINATION
	ExportShortInvoice_CreateOutline("C", 22, "D", 23, TRUE, TRUE);
	FillCell("C", 22, "Final Destination");
	FillCell("C", 23, country);

	//TERMS OF DELIVERY AND PAYMENT
	ExportShortInvoice_CreateOutline("E", 18, "K", 23, TRUE, TRUE);
	FillCell("E", 18, "Terms Of Delivery and Payment");
	FillCell("E", 19, dlg.m_terms);

	//MARK AND NOs. CONTAINER
	ExportShortInvoice_CreateOutline("A", 24, "B", 25, FALSE, FALSE);
	ExportShortInvoice_CreateOutline("A", 26, "B", 29, FALSE, FALSE);
	FillCell("A", 24, "Marks & Nos.\nContainer No.");

	//Np. & KIND OF PKGs.
	ExportShortInvoice_CreateOutline("C", 24, "D", 25, FALSE, FALSE);
	ExportShortInvoice_CreateOutline("C", 26, "D", 29, FALSE, FALSE);
	FillCell("C", 24, "No. & kind of Pkgs.");

	//DESCRIPTION OF GOODS
	ExportShortInvoice_CreateOutline("E", 24, "G", 29, FALSE, FALSE);
	FillCell("E", 24, "Description of Goods\nGold jewellery of .750 fineness set with cut & polished diamonds, precious & semi-precious stones and pearls.");

	//GROSS WEIGHT
	ExportShortInvoice_CreateOutline("A", 31, "B", 32, TRUE, FALSE);
	FillCell("A", 31, "Gross Weight in Gms.");
	FillCell("A", 32, gross_weight, 3);

	//DIAMOND QUALITY
	ExportShortInvoice_CreateOutline("A", 33, "B", 34, TRUE, FALSE);
	FillCell("A", 33, "Diamond Quality:");
	FillCell("A", 34, "VS");

	//GR FORM No.
	ExportShortInvoice_CreateOutline("A", 35, "B", 36, TRUE, FALSE);
	FillCell("A", 35, "GR Form No.:");
	FillCell("A", 36, dlg.m_gr_number);

	//No. OF PIECES
	ExportShortInvoice_CreateOutline("A", 37, "B", 38, TRUE, FALSE);
	FillCell("A", 37, "No. Of Pcs.:");
	FillCell("A", 38, pieces);

	//CUSTOM EXCHANGE RATE
	ExportShortInvoice_CreateOutline("A", 40, "C", 41, TRUE, FALSE);
	str.Format("%.02f",conversion);
	str = currency + " 1.00 = Rs. " + str;
	FillCell("A", 40, "Custom exchange rate:");
	FillCell("A", 41, str);

	//VALUE OF GOLD
	ExportShortInvoice_CreateOutline("A", 43, "C", 44, TRUE, FALSE);
	str.Format("%.02f",gold_rate);
	str = currency + " " + str + " per 10 gms.";
	FillCell("A", 43, "Value of Gold:");
	FillCell("A", 44, str);
	ExportShortInvoice_CreateOutline("A", 45, "C", 45, FALSE, FALSE);
	FillCell("A", 45, "of one fineness");

	//VALUES
	MergeCells("H", 24, "H", 25);
	SetBorder("H", 24, "H", 44, AllBorders, TRUE, 1);
	SetAlignment("H", 24, CenterAlign, TopAlign);
	SetTextWrap("H", 24, "H", 25);
	FillCell("H", 24, "Quantity (Cts.)");

	MergeCells("I", 24, "I", 25);
	SetBorder("I", 24, "I", 44, AllBorders, TRUE, 1);
	SetAlignment("I", 24, CenterAlign, TopAlign);
	SetTextWrap("I", 24, "I", 25);
	FillCell("I", 24, "Quantity (Gms.)");

	MergeCells("J", 24, "K", 24);
	SetBorder("J", 24, "K", 44, AllBorders, TRUE, 1);
	SetAlignment("J", 24, CenterAlign, TopAlign);
	FillCell("J", 24, CString("Amount (") + currency + CString(")"));

#if 0
	//GROSS WEIGHT
	ExportShortInvoice_CreateOutline("F", 31, "G", 31, FALSE, FALSE);
	FillCell("F", 31, "Gross Weight");

	MergeCells("H", 31, "I", 31);
	MergeCells("H", 32, "I", 32);
	SetAlignment("H", 31, CenterAlign, TopAlign);
	FillCell("H", 31, gross_weight, 3);
#endif

	//NET WEIGHT
	ExportShortInvoice_CreateOutline("F", 33, "G", 33, FALSE, FALSE);
	FillCell("F", 33, "Net Weight (gold)");

	//MergeCells("H", 33, "I", 33);
	//MergeCells("H", 34, "I", 34);
	SetAlignment("I", 33, CenterAlign, TopAlign);
	FillCell("I",33,net_weight, 3);

	MergeCells("J", 33, "K", 33);
	MergeCells("J", 34, "K", 34);
	SetAlignment("J", 33, CenterAlign, TopAlign);
	FillCell("J", 33, net_value, 3);

	//DIAMOND WEIGHT
	ExportShortInvoice_CreateOutline("F", 35, "G", 35, FALSE, FALSE);
	FillCell("F", 35, "Diamond Weight");

	//MergeCells("H", 35, "I", 35);
	//MergeCells("H", 36, "I", 36);
	SetAlignment("H", 35, CenterAlign, TopAlign);
	FillCell("H", 35, diamond_weight, 2);
	SetAlignment("I", 35, CenterAlign, TopAlign);
	FillCell("I", 35, CProductSubItem::GramsFromCaret(diamond_weight), 3);

	MergeCells("J", 35, "K", 35);
	MergeCells("J", 36, "K", 36);
	SetAlignment("J", 35, CenterAlign, TopAlign);
	FillCell("J", 35, diamond_value, 2);

	//STONE WEIGHT
	ExportShortInvoice_CreateOutline("F", 37, "G", 37, FALSE, FALSE);
	FillCell("F", 37, "Colour Stone Weight");

	//MergeCells("H", 37, "I", 37);
	//MergeCells("H", 38, "I", 38);
	SetAlignment("H", 37, CenterAlign, TopAlign);
	FillCell("H", 37, stone_weight, 2);
	SetAlignment("I", 37, CenterAlign, TopAlign);
	FillCell("I", 37, CProductSubItem::GramsFromCaret(stone_weight), 3);

	MergeCells("J", 37, "K", 37);
	MergeCells("J", 38, "K", 38);
	SetAlignment("J", 37, CenterAlign, TopAlign);
	FillCell("J", 37, stone_value, 2);

	//PEARL WEIGHT
	ExportShortInvoice_CreateOutline("F", 39, "G", 39, FALSE, FALSE);
	FillCell("F", 39, "Pearl Weight");

	//MergeCells("H", 39, "I", 39);
	//MergeCells("H", 40, "I", 40);
	SetAlignment("H", 39, CenterAlign, TopAlign);
	FillCell("H", 39, pearl_weight, 2);
	SetAlignment("I", 39, CenterAlign, TopAlign);
	FillCell("I", 39, CProductSubItem::GramsFromCaret(pearl_weight), 3);

	MergeCells("J", 39, "K", 39);
	MergeCells("J", 40, "K", 40);
	SetAlignment("J", 39, CenterAlign, TopAlign);
	FillCell("J", 39, pearl_value, 2);

	//OTHER WEIGHT
	ExportShortInvoice_CreateOutline("F", 41, "G", 41, FALSE, FALSE);
	FillCell("F", 41, "Other Weight");

	//MergeCells("H", 41, "I", 41);
	//MergeCells("H", 42, "I", 42);
	SetAlignment("I", 41, CenterAlign, TopAlign);
	FillCell("I", 41, other_weight, 3);

	MergeCells("J", 41, "K", 41);
	MergeCells("J", 42, "K", 42);
	SetAlignment("J", 41, CenterAlign, TopAlign);
	FillCell("J", 41, "NIL");

	//VALUE ADDITION
	ExportShortInvoice_CreateOutline("F", 43, "G", 43, FALSE, FALSE);
	FillCell("F", 43, "Value Addition");

	MergeCells("J", 43, "K", 43);
	MergeCells("J", 44, "K", 44);
	SetAlignment("J", 43, CenterAlign, TopAlign);
	FillCell("J", 43, value_addition, 2);

	//FOB VALUE
	ExportShortInvoice_CreateOutline("F", 45, "G", 45, FALSE, FALSE);
	FillCell("F", 45, "FOB Value");

	SetAlignment("I", 45, CenterAlign, TopAlign);
	CString formula_str;
	formula_str.Format("=SUM(%s:%s)", GetAddress("I", 33), GetAddress("I", 43));
	FillCellWithFormula("I", 45, formula_str);

	MergeCells("J", 45, "K", 45);
	SetAlignment("J", 45, CenterAlign, TopAlign);
	FillCell("J", 45, total_value, 2);

	//AMOUNT CHARGEABLE
	ExportShortInvoice_CreateOutline("A", 47, "K", 48, TRUE, TRUE);
	FillCell("A", 47, "Amount Chargeable: ");

	//WASTAGE ON GOLD
	ExportShortInvoice_CreateOutline("A", 50, "E", 52, FALSE, FALSE);
	FillCell("A", 50, "Wastage on gold to be taken as per chapter 4 of export import policy 2002-07.");

	//DECLARATION
	ExportShortInvoice_CreateOutline("A", 56, "F", 58, FALSE, FALSE);
	FillCell("A", 56, "Declaration:\nWe declare that this Invoice shows the actual price of the Goods described and that all particulars are true and correct.");

	//SIGNATURE AND DATE
	ExportShortInvoice_CreateOutline("G", 52, "K", 58, TRUE, TRUE);
	FillCell("G", 52, "Signature & Date");
	FillCell("G", 53, "Date - " + date);

	SetTextWrap(gAlpha[0], 3, gAlpha[m_last_column], m_end_row);
	SetBorder(gAlpha[0], 3, gAlpha[m_last_column], m_end_row, AllBorders, TRUE, 1);
}


BOOL CRatnaKiranReports::CreateExportPhotoInvoice(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CProgressDialog progress1("Processing report", m_list->GetCount() + 1);
	progress1.StartDialog();

	m_start_row = 17;
	ExportPhotoInvoice_AddHeaders();
	ExportPhotoInvoice_AddTitle(type);
	progress1.ProgressForward();

	int position = m_start_row;
	CList<Item_Voucher *, Item_Voucher *> itemlist;
	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				int index = 0;
				while(index < set->GetItemCount())
				{
					CProductItem *item = set->GetItemAt(index);
					if(item)
					{
						if((type == Sold_Report && item->IsItemSold()) ||
						   (type == UnSold_Report && !(item->IsItemSold())) ||
						   (type == Original_Report))
						{
							if((item->m_serial_number >= m_serials_from) &&
							   ((item->m_serial_number <= m_serials_to) || (m_serials_to == -1)))
							{
								Item_Voucher *new_node = NULL;
								SALIL_NEW(new_node, Item_Voucher);
								new_node->item = item;
								new_node->voucher = voucher;

								itemlist.AddHead(new_node);
							}
						}
					}
					else
					{
						CString errstr;
						errstr.Format("%d", index);
						errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
					}

					index++;
				}
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress1.ProgressForward();
	}

	progress1.FinishDialog();
	CProgressDialog progress2("Processing report", itemlist.GetCount() + 2);
	progress2.StartDialog();

	while(!itemlist.IsEmpty())
	{
		int orig_position = position;
		CProductVoucher *voucher = NULL;
		CProductItem *item = GetLowestSerialNumberItem(&itemlist, &voucher);
		if(item && voucher)
			position = ExportPhotoInvoice_AddItem(item, voucher, position);
		else
			SHOW_INTERNAL_ERROR;

		progress2.ProgressForward();
	}

	//position = ProcessTotals(position);
	m_end_row = position;
	progress2.ProgressForward();

	ProcessAutoFit();
	progress2.ProgressForward();

	FinalizeLandscapeReport();
	progress2.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::ExportPhotoInvoice_AddTitle(ReportType type)
{
	CString consignee = gDoc->m_selected_voucher->m_consignee;
	CString exporter = gDoc->m_selected_voucher->m_exporter;
	CString invoice_number = gDoc->m_selected_voucher->m_invoice_number;

	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 12);

	MergeCells(gAlpha[0], 3, gAlpha[3], 6);
	FillCell(gAlpha[0], 3, CString("Exporter: ") + exporter);
	SetAlignment(gAlpha[0], 3, LeftAlign, CenterAlign);

	MergeCells(gAlpha[0], 8, gAlpha[3], 12);
	FillCell(gAlpha[0], 8, CString("Consignee: ") + consignee);
	SetAlignment(gAlpha[0], 8, LeftAlign, CenterAlign);
	SetTextWrap(gAlpha[0], 8, gAlpha[3], 12);
	
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);
	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "PHOTO INVOICE");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "PHOTO INVOICE (Sold)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "PHOTO INVOICE (UnSold)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign, CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);

	MergeCells(gAlpha[m_last_column - 3], 3, gAlpha[m_last_column], 3);

	//CTime systemtime = CTime::GetCurrentTime();
	//FillCell(gAlpha[m_last_column - 3], 3, CString("DATE: ") + systemtime.Format("%d-%m-%Y"));

	FillCell(gAlpha[m_last_column - 3], 3, CString("DATE:           "));
	SetAlignment(gAlpha[m_last_column - 3], 3, RightAlign, CenterAlign);

	MergeCells(gAlpha[m_last_column - 3], 5, gAlpha[m_last_column], 5);
	FillCell(gAlpha[m_last_column - 3], 5, CString("INVOICE NUMBER: "));
	SetAlignment(gAlpha[m_last_column - 3], 5, RightAlign, CenterAlign);
	SetTextWrap(gAlpha[m_last_column - 3], 5, gAlpha[m_last_column], 5);

	MergeCells(gAlpha[m_last_column - 3], 6, gAlpha[m_last_column], 6);
	FillCell(gAlpha[m_last_column - 3], 6, invoice_number);
	SetAlignment(gAlpha[m_last_column - 3], 6, RightAlign, CenterAlign);
	SetTextWrap(gAlpha[m_last_column - 3], 6, gAlpha[m_last_column], 6);
}


void CRatnaKiranReports::ExportPhotoInvoice_AddHeaders()
{
	CString currency;
	if(CSettings::GetUseNativeCurrency())
		currency = gDoc->m_selected_voucher->m_currency_name;
	else
		currency = "USD";

	int position = m_start_row - 2;
	int column = 0;

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	SetWidth(gAlpha[column], gAlpha[column], m_image_width);
	FillCell(gAlpha[column++], position, "Picture");

	if(m_use_product_id)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "ID");
	}

	if(m_use_new_serials)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "SNo.");
	}

	if(m_use_old_serials)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Old SNo.");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	SetTextWrap(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Desc. of product");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Pcs.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Gross Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Net Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Gold fineness");

	if(m_show_diamond_clarity)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Diamond clarity");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Diamond Wt. (Crt.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Stone Wt. (Crt.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Pearl Wt. (Crt.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, CString("Value (") + currency + CString(")"));

	m_last_column = column - 1;

	SetBorders(gAlpha[0], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[0], position, gAlpha[m_last_column], position + 1);
}


int CRatnaKiranReports::ExportPhotoInvoice_AddItem(CProductItem *item, CProductVoucher *voucher, int position)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	SetHeight(position, position, m_image_height);
	SetAlignment(gAlpha[1], position, gAlpha[m_last_column], position, CenterAlign, CenterAlign);

	double gold_rate, conversion, labourvalue, pearlvalue, diamondvalue, stonevalue;
	if(!CalculateExportValues(&gold_rate, &conversion, &labourvalue, &pearlvalue, &diamondvalue, &stonevalue, NULL, item, voucher))
	{
		CString errstr = "Could not calculate export values for item " + CString(item->m_product_ID) + ".";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return position;
	}

	double goldvalue = item->GetGoldValueDouble(TRUE, FALSE, gold_rate, conversion);

	goldvalue = RoundOff(goldvalue, 2);
	diamondvalue = RoundOff(diamondvalue, 2);
	stonevalue = RoundOff(stonevalue, 2);
	labourvalue = RoundOff(labourvalue, 2);
	pearlvalue = RoundOff(pearlvalue, 2);

	double total =  goldvalue + diamondvalue + stonevalue + labourvalue + pearlvalue;
	int column = 0;

	CString filename = item->m_photo_filename;
	if(!filename.IsEmpty())
	{
		filename = gFilePath + "\\images\\" + filename;
		FillCellWithImage(gAlpha[column++], position, filename);
	}
	else
	{
		column++;
	}

	if(m_use_product_id)
	{
		FillCell(gAlpha[column++], position, item->m_product_ID);
	}

	if(m_use_new_serials)
	{
		FillCell(gAlpha[column++], position, item->m_serial_number);
	}

	if(m_use_old_serials)
	{
		CString tagname;

		if(gDoc->m_selected_voucher->IsRootVoucher())
		{
			CString voucher_name = voucher->m_voucher_name;
			POSITION pos = m_old_serial_tagnames->GetHeadPosition();
			while(pos)
			{
				SStringPairs details = m_old_serial_tagnames->GetAt(pos);
				if(details.m_string1 == voucher_name)
				{
					tagname = details.m_string2;
					break;
				}

				m_old_serial_tagnames->GetNext(pos);
			}

			if(pos == NULL) //not found
				tagname = "";
		}
		else
		{
			tagname = m_old_serial_tagname;
		}

		CProductOldSerial *serial = NULL;
		if(tagname.IsEmpty())
		{
			int count = item->GetOldSerialCount();
			if(count > 0)
				serial = item->GetOldSerialAt(count - 1); //get last added
		}
		else
			serial = item->GetOldSerial(tagname);

		if(serial)
			FillCell(gAlpha[column++], position, serial->m_serial);
		else
			FillCell(gAlpha[column++], position, "");
	}

	FillCell(gAlpha[column++], position, item->m_description);
	FillCell(gAlpha[column++], position, item->m_piece_count);
	FillCell(gAlpha[column++], position, item->GetGrossWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetNetWeight(), 3);
	FillCell(gAlpha[column++], position, "18KT");
	if(m_show_diamond_clarity)
		FillCell(gAlpha[column++], position, item->m_diamond_clarity);
	FillCell(gAlpha[column++], position, item->GetSubItemWeight(tDiamond), 2);
	FillCell(gAlpha[column++], position, item->GetSubItemWeight(tStone), 2);
	FillCell(gAlpha[column++], position, item->GetSubItemWeight(tPearl), 2);
	FillCell(gAlpha[column++], position, total, 2);

	SetBorder(gAlpha[0], position, gAlpha[m_last_column], position, AllBorders, TRUE, 1);
	return (position + 1);
}


BOOL CRatnaKiranReports::CreateExportChecklist(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CProgressDialog progress1("Processing report", m_list->GetCount() + 1);
	progress1.StartDialog();

	m_start_row = 5;
	ExportChecklist_AddHeaders();
	ExportChecklist_AddTitle(type);
	progress1.ProgressForward();

	int position = m_start_row;
	CList<Item_Voucher *, Item_Voucher *> itemlist;

	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				int index = 0;
				while(index < set->GetItemCount())
				{
					CProductItem *item = set->GetItemAt(index);
					if(item)
					{
						if((type == Sold_Report && item->IsItemSold()) ||
						   (type == UnSold_Report && !(item->IsItemSold())) ||
						   (type == Original_Report))
							{
								if((item->m_serial_number >= m_serials_from) &&
								   ((item->m_serial_number <= m_serials_to) || (m_serials_to == -1)))
								{
									Item_Voucher *new_node = NULL;
									SALIL_NEW(new_node, Item_Voucher);
									new_node->item = item;
									new_node->voucher = voucher;
									itemlist.AddHead(new_node);
								}
							}
					}
					else
					{
						CString errstr;
						errstr.Format("%d", index);
						errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
						SHOW_INTERNAL_ERROR_REASON(errstr);
					}

					index++;
				}
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress1.ProgressForward();
	}

	progress1.FinishDialog();
	CProgressDialog progress2("Processing report", itemlist.GetCount() + 2);
	progress2.StartDialog();

	while(!itemlist.IsEmpty())
	{
		CProductVoucher *voucher = NULL;
		CProductItem *item = GetLowestSerialNumberItem(&itemlist, &voucher);
		if(item && voucher)
			position = ExportChecklist_AddItem(item, voucher, position);
		else
			SHOW_INTERNAL_ERROR;

		progress2.ProgressForward();
	}

	position = ProcessTotals(position);
	m_end_row = position;
	progress2.ProgressForward();

	ProcessAutoFit();
	progress2.ProgressForward();

	FinalizeLandscapeReport();
	progress2.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::ExportChecklist_AddTitle(ReportType type)
{
	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 1);
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);

	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "EXPORT CHECKLIST");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "EXPORT CHECKLIST (Sold)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "EXPORT CHECKLIST (UnSold)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign,CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
}


void CRatnaKiranReports::ExportChecklist_AddHeaders()
{
	int position = m_start_row - 2;
	int column = 0;

	if(m_use_product_id)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "ProductID");
	}

	if(m_use_new_serials)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Serial no.");
	}

	if(m_use_old_serials)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Old Serial no.");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "ProductID");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Item Desc.");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Pcs.");

	MergeCells(gAlpha[column], position, gAlpha[column],position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "GWt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "NWt. (Gms.)");

	if(m_show_diamond_clarity)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Diamond Clarity");
	}

	MergeCells(gAlpha[column], position, gAlpha[column + 2], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Diamond");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Type");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column + 2], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pr/Spr Stone");
	AddAutoFit(column);
	FillCell(gAlpha[column++], position + 1, "Type");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column + 1], position);
	SetAlignment(gAlpha[column], position, CenterAlign, CenterAlign);
	FillCell(gAlpha[column], position, "Pearl");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Wt. (Cts.)");
	AddTotals(column);
	FillCell(gAlpha[column++], position + 1, "Rate (Rs.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Other Wt. (Gms.)");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Polishing (Rs.)");

	m_last_column = column - 1;

	SetBorders(gAlpha[0], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[0], position, gAlpha[m_last_column], position + 1);
}


int CRatnaKiranReports::ExportChecklist_AddItem(CProductItem *item, CProductVoucher *voucher, int position)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int column = 0;

	if(m_use_product_id)
	{
		FillCell(gAlpha[column++], position, item->m_product_ID);
	}

	if(m_use_new_serials)
	{
		FillCell(gAlpha[column++], position, item->m_serial_number);
	}

	if(m_use_old_serials)
	{
		CString tagname;

		if(gDoc->m_selected_voucher->IsRootVoucher())
		{
			CString voucher_name = voucher->m_voucher_name;
			POSITION pos = m_old_serial_tagnames->GetHeadPosition();
			while(pos)
			{
				SStringPairs details = m_old_serial_tagnames->GetAt(pos);
				if(details.m_string1 == voucher_name)
				{
					tagname = details.m_string2;
					break;
				}

				m_old_serial_tagnames->GetNext(pos);
			}

			if(pos == NULL) //not found
				tagname = "";
		}
		else
		{
			tagname = m_old_serial_tagname;
		}

		CProductOldSerial *serial = NULL;
		if(tagname.IsEmpty())
		{
			int count = item->GetOldSerialCount();
			if(count > 0)
				serial = item->GetOldSerialAt(count - 1); //get last added
		}
		else
			serial = item->GetOldSerial(tagname);

		if(serial)
			FillCell(gAlpha[column++], position, serial->m_serial);
		else
			FillCell(gAlpha[column++], position, "");
	}

	FillCell(gAlpha[column++], position, item->m_product_ID);
	FillCell(gAlpha[column++], position, item->m_description);
	FillCell(gAlpha[column++], position, item->m_piece_count);
	FillCell(gAlpha[column++], position, item->GetGrossWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetNetWeight(), 3);
	if(m_show_diamond_clarity)
		FillCell(gAlpha[column++], position, item->m_diamond_clarity);

	int index = 0;
	while(index < item->GetSubItemCount(tDiamond))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemTypeOf(tDiamond, index));
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemWeightOf(tDiamond, index), 2);
		FillCell(gAlpha[column + 2], position + index, item->GetSubItemRateOf(tDiamond, index, 1.0));

		index++;
	}

	column += 3;

	index = 0;
	while(index < item->GetSubItemCount(tStone))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemTypeOf(tStone, index));
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemWeightOf(tStone, index), 2);
		FillCell(gAlpha[column + 2], position + index, item->GetSubItemRateOf(tStone, index, 1.0));

		index++;
	}

	column += 3;

	index = 0;
	while(index < item->GetSubItemCount(tPearl))
	{
		SetAlignment(gAlpha[column], position + index, RightAlign, CenterAlign);
		FillCell(gAlpha[column], position + index, item->GetSubItemWeightOf(tPearl, index), 2);
		FillCell(gAlpha[column + 1], position + index, item->GetSubItemRateOf(tPearl, index, 1.0));

		index++;
	}

	column += 2;

	FillCell(gAlpha[column++], position, item->GetOtherWeight(), 3);
	FillCell(gAlpha[column++], position, item->GetLabourCost(1.0), 2);

	int maxval = max(item->GetSubItemCount(tDiamond), item->GetSubItemCount(tStone));
	maxval = max(maxval, item->GetSubItemCount(tPearl));

	SetBorder(gAlpha[0], position, gAlpha[m_last_column], position + maxval - 1, AllBorders, TRUE, 1);
	return (position + maxval);//this takes care of going to the next empty position to add the next item.
}


BOOL CRatnaKiranReports::CreateExportTags(int row, int column)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	m_start_row = 1;
	m_last_column = 5;

	int position = row;
	int itemnumber = (column - 1)/2;

	CProgressDialog progress("Processing report", m_list->GetCount() + 1);
	progress.StartDialog();

	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			int index = 0;
			while(index < set->GetItemCount())
			{
				CProductItem *item = set->GetItemAt(index);
				if(item)
				{
					if((item->m_serial_number >= m_serials_from) &&
					   ((m_serials_to == -1) || (item->m_serial_number <= m_serials_to)))
					{
						ExportTags_AddItem(item, voucher, (2 * itemnumber), position);
						itemnumber++;

						if(itemnumber == 3)
						{
							itemnumber = 0;
							position++;
						}
					}
				}
				else
				{
					CString errstr;
					errstr.Format("%d", index);
					errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}

				index++;
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	m_end_row = position;
	SetWidth(gAlpha[0], gAlpha[m_last_column], 13.4);
	SetHeight(1, position, 49.3);
	progress.ProgressForward();

	FinalizeTagReport();
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::ExportTags_AddItem(CProductItem *item, CProductVoucher *voucher, int column, int row)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	int firstcol = column;
	int secondcol = column + 1;
	
	CString firstside = "";

	{
		CString temp = "";
	
		//NEW SERIAL NUMBER
		if(m_use_new_serials)
		{
			temp.Format("%d",item->m_serial_number);
			firstside = firstside + temp + " = ";
		}

		//OLD SERIAL NUMBER
		if(m_use_old_serials)
		{
			CString tagname;

			if(gDoc->m_selected_voucher->IsRootVoucher())
			{
				CString voucher_name = voucher->m_voucher_name;
				POSITION pos = m_old_serial_tagnames->GetHeadPosition();
				while(pos)
				{
					SStringPairs details = m_old_serial_tagnames->GetAt(pos);
					if(details.m_string1 == voucher_name)
					{
						tagname = details.m_string2;
						break;
					}

					m_old_serial_tagnames->GetNext(pos);
				}

				if(pos == NULL) //not found
					tagname = "";
			}
			else
			{
				tagname = m_old_serial_tagname;
			}

			CProductOldSerial *serial = NULL;
			if(tagname.IsEmpty())
			{
				int count = item->GetOldSerialCount();
				if(count > 0)
					serial = item->GetOldSerialAt(count - 1); //get last added
			}
			else
				serial = item->GetOldSerial(tagname);

			if(serial)
			{
				temp.Format("%d",serial->m_serial);
				firstside = firstside + "#" + temp + " = ";
			}
		}
			
		//PRODUCT ID
		firstside = firstside + item->m_product_ID + "\n";

		//GROSS WEIGHT
		temp.Format("%.3f",item->GetGrossWeight());
		firstside = firstside + "GWt:" + temp + "\n";

		//NUMBER OF PIECES
		temp.Format("%d",item->m_piece_count);
		firstside = firstside + "Pcs:" + temp + "\n";
	}

	CString secondside;

	{
		CString temp;

		//NEW SERIAL NUMBER
		if(m_use_new_serials)
		{
			temp.Format("%d",item->m_serial_number);
			secondside = secondside + temp + " = ";
		}

		//OLD SERIAL NUMBER
		if(m_use_old_serials)
		{
			CString tagname;

			if(gDoc->m_selected_voucher->IsRootVoucher())
			{
				CString voucher_name = voucher->m_voucher_name;
				POSITION pos = m_old_serial_tagnames->GetHeadPosition();
				while(pos)
				{
					SStringPairs details = m_old_serial_tagnames->GetAt(pos);
					if(details.m_string1 == voucher_name)
					{
						tagname = details.m_string2;
						break;
					}

					m_old_serial_tagnames->GetNext(pos);
				}

				if(pos == NULL) //not found
					tagname = "";
			}
			else
			{
				tagname = m_old_serial_tagname;
			}

			CProductOldSerial *serial = NULL;
			if(tagname.IsEmpty())
			{
				int count = item->GetOldSerialCount();
				if(count > 0)
					serial = item->GetOldSerialAt(count - 1); //get last added
			}
			else
				serial = item->GetOldSerial(tagname);

			if(serial)
			{
				temp.Format("%d",serial->m_serial);
				secondside = secondside + "#" + temp + " = ";
			}
		}

		//PRODUCT ID
		secondside = secondside + item->m_product_ID + "\n";

		//DIAMOND WEIGHT
		int count = 0;
		while(count < item->GetSubItemCount(tDiamond))
		{
			CString type, weight;
			type = item->GetSubItemTypeOf(tDiamond, count);
			weight.Format("%.02f", item->GetSubItemWeightOf(tDiamond, count));
			secondside = secondside + type + " -" + weight + "\n";
			count++;
		}

		//STONE WEIGHT
		count = 0;
		while(count < item->GetSubItemCount(tStone))
		{
			CString type, weight;
			type = item->GetSubItemTypeOf(tStone, count);
			weight.Format("%.02f", item->GetSubItemWeightOf(tStone, count));
			secondside = secondside + type + " -" + weight + "\n";
			count++;
		}

		//PEARL WEIGHT
		count = 0;
		while(count < item->GetSubItemCount(tPearl))
		{
			CString type, weight;
			type = "P";
			weight.Format("%.02f", item->GetSubItemWeightOf(tPearl, count));
			secondside = secondside + type + " -" + weight + "\n";
			count++;
		}
	}

	FillCell(gAlpha[firstcol], row, firstside);
	FillCell(gAlpha[secondcol], row, secondside);

	SetAlignment(gAlpha[firstcol], row, CenterAlign, CenterAlign);
	SetAlignment(gAlpha[secondcol], row, CenterAlign, CenterAlign);
	SetTextWrap(gAlpha[firstcol], row, gAlpha[secondcol], row, TRUE);
	SetFontSize(gAlpha[firstcol], row, gAlpha[secondcol], row, 6);
}


BOOL CRatnaKiranReports::CreateExportPriceQuotation(ReportType type)
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CProgressDialog progress1("Processing report", m_list->GetCount() + 3);
	progress1.StartDialog();

	m_start_row = 5;
	ExportPriceQuotation_AddHeaders();
	ExportPriceQuotation_AddTitle(type);
	progress1.ProgressForward();

	int position = m_start_row;

	CList<Set_Voucher *, Set_Voucher *> setlist;
	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			Set_Voucher *new_node = NULL;
			SALIL_NEW(new_node, Set_Voucher);
			new_node->set = set;
			new_node->voucher = voucher;

			setlist.AddHead(new_node);
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress1.ProgressForward();
	}

	progress1.FinishDialog();
	CProgressDialog progress2("Processing report", setlist.GetCount() + 2);
	progress2.StartDialog();

	BOOL onfirstpage = TRUE;
	int page = 0;
	int itemcount = 0;
	int orientation = price_quotation_left;

	//first page will have 2 less than the number of items in other pages.
	while(!setlist.IsEmpty())
	{
		CProductVoucher *voucher = NULL;
		CProductSet *set = GetLowestSerialNumberSet(&setlist, &voucher);
		if(set && voucher)
		{
#if USE_TWO_COLUMNS
			if(onfirstpage)
			{
				if(itemcount + set->GetItemCount() > (price_quotation_page_size - 4))
				{
					if(orientation == price_quotation_left)
					{
						itemcount = 0;
						position = m_start_row;
						orientation = price_quotation_right;
					}
					else
					{
						page++;
						onfirstpage = FALSE;
						itemcount = 0;
						position = price_quotation_page_size + 1; //move one down the actual page
						orientation = price_quotation_left;
						SetPageBreak(gAlpha[m_last_column + 1], price_quotation_page_size + 1);
					}
				}
			}
			else
			{
				if(itemcount + set->GetItemCount() > price_quotation_page_size - 2) //2 for the header and 1 because of some excel issue
				{
					if(orientation == price_quotation_left)
					{
						itemcount = 0;
						position = price_quotation_page_size + ((page - 1) * (price_quotation_page_size - 2)) + 1; //all pages except first page will have 2 less items than the page size.
						orientation = price_quotation_right;
					}
					else
					{
						page++;
						itemcount = 0;
						position = price_quotation_page_size + ((page - 1) * (price_quotation_page_size - 2)) + 1; //all pages except first page will have 2 less items than the page size.
						orientation = price_quotation_left;
						SetPageBreak(gAlpha[m_last_column + 1], price_quotation_page_size + ((page - 1) * (price_quotation_page_size - 2)) + 1);
					}
				}
			}
#endif

			if((type == Sold_Report && set->GetSoldItemCount() != 0) ||
			   (type == UnSold_Report && set->GetUnsoldItemCount() != 0) ||
			   (type == Original_Report))
			{
				position = ExportPriceQuotation_AddSet(set, voucher, position, orientation, type);
			}

#if USE_TWO_COLUMNS
			if(onfirstpage)
			{
				itemcount = position - m_start_row;
			}
			else
			{
				itemcount = position - (price_quotation_page_size + ((page - 1) * (price_quotation_page_size - 2)) + 1);
			}
#endif
		}
		else
			SHOW_INTERNAL_ERROR;

		progress2.ProgressForward();
	}

	//position = ProcessTotals(position);
#if USE_TWO_COLUMNS
	m_end_row = (page + 1) * price_quotation_page_size;
#else
	m_end_row = position;
#endif

	progress2.ProgressForward();

	ProcessAutoFit();
	progress2.ProgressForward();

	FinalizePotraitReport(TRUE);
	progress2.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::ExportPriceQuotation_AddTitle(ReportType type)
{
	SetBold(gAlpha[0], 1, gAlpha[m_last_column], 1);
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);

	switch(type)
	{
	case Original_Report:
		FillCell(gAlpha[0], 1, "EXPORT PRICE QUOTATION");
		break;
	case Sold_Report:
		FillCell(gAlpha[0], 1, "EXPORT PRICE QUOTATION (Sold)");
		break;
	case UnSold_Report:
		FillCell(gAlpha[0], 1, "EXPORT PRICE QUOTATION (UnSold)");
		break;
	default:
		{
			CString errstr;
			errstr.Format("%d", type);
			errstr = "Invalid report type '" + errstr + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
		break;
	}

	SetAlignment(gAlpha[0], 1, CenterAlign, CenterAlign);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
}

	
void CRatnaKiranReports::ExportPriceQuotation_AddHeaders()
{
	int position = m_start_row - 2;
	int column = price_quotation_left;

	if(m_use_product_id)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "ProductID");
	}

	if(m_use_new_serials)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Serial");
	}

	if(m_use_old_serials)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Old SNo.");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Item Desc.");

	if(m_show_diamond_clarity)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Diamond Clarity");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Price");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Total");

	m_last_column = column - 1;
	price_quotation_number_of_columns = column;

	SetBorders(gAlpha[price_quotation_left], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[price_quotation_left], position, gAlpha[m_last_column], position + 1);

#if USE_TWO_COLUMNS
	SetWidth(gAlpha[price_quotation_number_of_columns], gAlpha[price_quotation_number_of_columns], 4);

	price_quotation_right = price_quotation_number_of_columns + 1;
	column = price_quotation_right;

	if(m_use_product_id)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "ProductID");
	}

	if(m_use_new_serials)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Serial");
	}

	if(m_use_old_serials)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Old SNo.");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddAutoFit(column);
	FillCell(gAlpha[column++], position, "Item Desc.");

	if(m_show_diamond_clarity)
	{
		MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
		AddAutoFit(column);
		FillCell(gAlpha[column++], position, "Diamond Clarity");
	}

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Price");

	MergeCells(gAlpha[column], position, gAlpha[column], position + 1);
	AddTotals(column);
	FillCell(gAlpha[column++], position, "Total");

	m_last_column = column - 1;

	SetBorders(gAlpha[price_quotation_right], position, gAlpha[m_last_column], position + 1, TRUE);
	SetTextWrap(gAlpha[price_quotation_right], position, gAlpha[m_last_column], position + 1);

	price_quotation_page_size = m_rows_in_a_page - 2;
#endif
}


int CRatnaKiranReports::ExportPriceQuotation_AddSet(CProductSet *set, CProductVoucher *voucher, int position, int orientation, ReportType type)
{
	if(!set || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	int pos = position;
	int index = 0;
	while(index < set->GetItemCount())
	{
		CProductItem *item = set->GetItemAt(index);
		if(item)
		{
			if((type == Sold_Report && item->IsItemSold()) ||
			   (type == UnSold_Report && !(item->IsItemSold())) ||
			   (type == Original_Report))
			{
				if((item->m_serial_number >= m_serials_from) &&
				   ((item->m_serial_number <= m_serials_to) || (m_serials_to == -1)))
				{
					pos = ExportPriceQuotation_AddItem(item, voucher, pos, orientation);
				}
			}
		}
		else
		{
			CString errstr;
			errstr.Format("%d", index);
			errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		index++;
	}

	if(pos == position)
		pos++;

	CString str;
	if(set->GetItemCount() > 1)
	{
#if USE_TWO_COLUMNS
		if(orientation == price_quotation_left)
		{
			str.Format("=SUM(%s:%s)", GetAddress(gAlpha[price_quotation_left + price_quotation_number_of_columns - 2], position), GetAddress(gAlpha[price_quotation_left + price_quotation_number_of_columns - 2], pos - 1));
			FillCellWithFormula(gAlpha[price_quotation_left + price_quotation_number_of_columns - 1], (position + pos - 1) / 2, str);
		}
		else if(orientation == price_quotation_right)
		{
			str.Format("=SUM(%s:%s)", GetAddress(gAlpha[price_quotation_right +  price_quotation_number_of_columns - 2], position), GetAddress(gAlpha[price_quotation_right +  price_quotation_number_of_columns - 2], pos - 1));
			FillCellWithFormula(gAlpha[price_quotation_right +  price_quotation_number_of_columns - 1], (position + pos - 1) / 2, str);
		}
#else
		str.Format("=SUM(%s:%s)", GetAddress(gAlpha[m_last_column - 1], position), GetAddress(gAlpha[m_last_column - 1], pos - 1));
		FillCellWithFormula(gAlpha[m_last_column], (position + pos - 1) / 2, str);
#endif
	}

#if USE_TWO_COLUMNS
	if(orientation == price_quotation_left)
		SetBorder(gAlpha[price_quotation_left], position, gAlpha[price_quotation_left + price_quotation_number_of_columns - 1], pos - 1, AllBorders, TRUE, 1);
	else if(orientation == price_quotation_right)
		SetBorder(gAlpha[price_quotation_right], position, gAlpha[price_quotation_right + price_quotation_number_of_columns - 1], pos - 1, AllBorders, TRUE, 1);
#else
	SetBorder(gAlpha[price_quotation_left], position, gAlpha[m_last_column], pos - 1, AllBorders, TRUE, 1);
#endif

	return pos;
}


int CRatnaKiranReports::ExportPriceQuotation_AddItem(CProductItem *item, CProductVoucher *voucher, int position, int orientation)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return position;
	}

	double gold_rate, conversion, price;
	if(!CalculateExportValues(&gold_rate, &conversion, NULL, NULL, NULL, NULL, &price, item, voucher))
	{
		CString errstr = "Could not calculate export values for item " + CString(item->m_product_ID) + ".";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return position;
	}

#if USE_TWO_COLUMNS
	int column = 0;
	if(orientation == price_quotation_left)
		column = price_quotation_left;
	else if(orientation == price_quotation_right)
		column = price_quotation_right;
#else
	int column = price_quotation_left;
#endif

	if(m_use_product_id)
	{
		FillCell(gAlpha[column++], position, item->m_product_ID);
	}

	if(m_use_new_serials)
	{
		FillCell(gAlpha[column++], position, item->m_serial_number);
	}

	if(m_use_old_serials)
	{
		CString tagname;

		if(gDoc->m_selected_voucher->IsRootVoucher())
		{
			CString voucher_name = voucher->m_voucher_name;
			POSITION pos = m_old_serial_tagnames->GetHeadPosition();
			while(pos)
			{
				SStringPairs details = m_old_serial_tagnames->GetAt(pos);
				if(details.m_string1 == voucher_name)
				{
					tagname = details.m_string2;
					break;
				}

				m_old_serial_tagnames->GetNext(pos);
			}

			if(pos == NULL) //not found
				tagname = "";
		}
		else
		{
			tagname = m_old_serial_tagname;
		}

		CProductOldSerial *serial = NULL;
		if(tagname.IsEmpty())
		{
			int count = item->GetOldSerialCount();
			if(count > 0)
				serial = item->GetOldSerialAt(count - 1); //get last added
		}
		else
			serial = item->GetOldSerial(tagname);

		if(serial)
			FillCell(gAlpha[column++], position, serial->m_serial);
		else
			FillCell(gAlpha[column++], position, "");
	}

	FillCell(gAlpha[column++], position, item->m_description);
	if(m_show_diamond_clarity)
		FillCell(gAlpha[column++], position, item->m_diamond_clarity);

	int final_price = (int)(price + 0.5);
	FillCell(gAlpha[column++], position, final_price);

	return (position + 1);
}


BOOL CRatnaKiranReports::CreateExportGRForm()
{
	if(!m_list || !m_valid || !MakeGlobalsValid())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CProgressDialog progress("Processing report", m_list->GetCount() + 1);
	progress.StartDialog();

	m_start_row = 3;
	m_end_row = 64;
	m_last_column = 10;

	int pieces = 0;
	double gross_weight = 0.0;
	double net_weight = 0.0;
	double total_value = 0.0;

	while(!m_list->IsEmpty())
	{
		CString productID = m_list->RemoveHead();
		CProductVoucher *voucher = NULL;
		CProductSet *set = gDoc->GetSet(productID, &voucher);
		if(set && voucher)
		{
			int index = 0;
			while(index < set->GetItemCount())
			{
				CProductItem *item = set->GetItemAt(index);
				if(item)
				{
					if((item->m_serial_number >= m_serials_from) &&
					((m_serials_to == -1) || (item->m_serial_number <= m_serials_to)))
					{
						ExportGRForm_AddItem(item, voucher, &pieces, &gross_weight, &net_weight, &total_value);
					}
				}
				else
				{
					CString errstr;
					errstr.Format("%d", index);
					errstr = "Could not get item at position '" + errstr + "' in set '" + CString(set->m_product_ID) + "'.";
					SHOW_INTERNAL_ERROR_REASON(errstr);
				}

				index++;
			}
		}
		else
		{
			CString errstr = "Could not get set details for " + productID + ".";
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}

		progress.ProgressForward();
	}

	ExportGRForm_FillValues(pieces, gross_weight, net_weight, total_value);
	progress.ProgressForward();

	FinalizePotraitReport(FALSE);
	progress.FinishDialog();
	return TRUE;
}


void CRatnaKiranReports::ExportGRForm_CreateOutline(char *startcolumn, int startrow, char *endcolumn, int endrow, BOOL hasEntry, BOOL drawBorder)
{
	if(hasEntry)
	{
		MergeCells(startcolumn, startrow, endcolumn, startrow);
		SetAlignment(startcolumn, startrow, LeftAlign, TopAlign);
		MergeCells(startcolumn, startrow + 1, endcolumn, endrow);
		SetAlignment(startcolumn, startrow + 1, LeftAlign, TopAlign);
	}
	else
	{
		MergeCells(startcolumn, startrow, endcolumn, endrow);
		SetAlignment(startcolumn, startrow, LeftAlign, TopAlign);
	}

	if(drawBorder)
		SetBorder(startcolumn, startrow, endcolumn, endrow, AllBorders, TRUE, 1);
}


void CRatnaKiranReports::ExportGRForm_AddItem(CProductItem *item, CProductVoucher *voucher,
										int *pieces, double *gross_weight,
										double *net_weight, double *total_value)
{
	if(!item || !voucher)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	double gold_rate, conversion, labourvalue, pearlvalue, diamondvalue, stonevalue;
	if(!CalculateExportValues(&gold_rate, &conversion, &labourvalue, &pearlvalue, &diamondvalue, &stonevalue, NULL, item, voucher))
	{
		CString errstr = "Could not calculate export values for item " + CString(item->m_product_ID) + ".";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}

	double goldvalue = item->GetGoldValueDouble(TRUE, FALSE, gold_rate, conversion);

	goldvalue = RoundOff(goldvalue, 2);
	diamondvalue = RoundOff(diamondvalue, 2);
	stonevalue = RoundOff(stonevalue, 2);
	labourvalue = RoundOff(labourvalue, 2);
	pearlvalue = RoundOff(pearlvalue, 2);

	double total =  goldvalue + diamondvalue + stonevalue + labourvalue + pearlvalue;

	*pieces = *pieces + item->m_piece_count;
	*gross_weight = *gross_weight + RoundOff(item->GetGrossWeight(), 3);
	*net_weight = *net_weight + RoundOff(item->GetNetWeight(), 3);
	*total_value = *total_value + RoundOff(total, 2);
}


void CRatnaKiranReports::ExportGRForm_FillValues(int pieces, double gross_weight, 
										   double net_weight, double total_value)
{
	CString consignee = gDoc->m_selected_voucher->m_consignee;
	CString exporter = gDoc->m_selected_voucher->m_exporter;
	CString invoice_number = gDoc->m_selected_voucher->m_invoice_number;
	CString country = gDoc->m_selected_voucher->m_country;
	CTime systemtime = CTime::GetCurrentTime();
	CString date = "          "; //systemtime.Format("%d-%m-%Y");
	CString str;

	CString currency;
	if(CSettings::GetUseNativeCurrency())
		currency = gDoc->m_selected_voucher->m_currency_name;
	else
		currency = "USD";

	double conversion;
	if(CSettings::GetUseNativeCurrency())
		conversion = gDoc->m_selected_voucher->m_currency_conversion;
	else
		conversion = gDoc->m_selected_voucher->m_dollar_rate;

	//HEADING
	MergeCells(gAlpha[0], 1, gAlpha[m_last_column], 1);
	SetAlignment(gAlpha[0], 1, CenterAlign, CenterAlign);
	SetFontSize(gAlpha[0], 1, gAlpha[m_last_column], 1, 14);
	SetUnderline(gAlpha[0], 1, gAlpha[m_last_column], 1);
	FillCell(gAlpha[0], 1, "SHIPPING BILL OF DUTY FREE EXPORTS");

	//EXPORTER
	ExportGRForm_CreateOutline("A", 3, "D", 9, TRUE, TRUE);
	FillCell("A", 3, "Exporter:");
	FillCell("A", 4, exporter);

	//INVOICE NUMBER	
	ExportGRForm_CreateOutline("E", 3, "G", 5, TRUE, TRUE);
	FillCell("E", 3, "Invoice No. & Date");
	FillCell("E", 4, invoice_number + "\nDt. " + date);
	
	//SB No. & DATE
	ExportGRForm_CreateOutline("H", 3, "K", 5, TRUE, TRUE);
	FillCell("H", 3, "SB No. & Date");

	//AR4
	ExportGRForm_CreateOutline("E", 6, "G", 7, TRUE, TRUE);
	FillCell("E", 6, "AR4/AR4A No. & Date");

	//IMPORT EXPORT CODE No.
	ExportGRForm_CreateOutline("H", 6, "K", 7, TRUE, TRUE);
	FillCell("H", 6, "Import/Export Code No.");
	FillCell("H", 7, "0588053554");

	//Cert. No. & Date
	ExportGRForm_CreateOutline("E", 8, "G", 9, TRUE, TRUE);
	FillCell("E", 8, "Q/Cert No. & Date");

	//STATE OF ORIGIN OF GOODS
	ExportGRForm_CreateOutline("H", 8, "K", 9, TRUE, TRUE);
	FillCell("H", 8, "State of origin of goods");

	//CONSIGNEE
	ExportGRForm_CreateOutline("A", 10, "D", 16, TRUE, TRUE);
	FillCell("A", 10, "Consignee:");
	FillCell("A", 11, consignee);

	//EXPORT TRADE CONTROL
	ExportGRForm_CreateOutline("E", 10, "G", 18, TRUE, TRUE);
	FillCell("E", 10, "Export Trade Control");

	//IF EXPORT
	ExportGRForm_CreateOutline("H", 10, "K", 14, FALSE, TRUE);
	FillCell("H", 10, "If export under:\nDeffered credit  [   ]       Joint Ventures  [   ]\nRupee Credit  [   ]       Others  [   ]\nRBI Approval/CIR No. & Date");

	//TYPE OF SHIPPMENT
	ExportGRForm_CreateOutline("H", 15, "K", 18, FALSE, TRUE);
	FillCell("H", 15, "Type of shippment\nConsignment export  [   ]\nOughtRightAlign sale  [   ]       Others  [   ]");

	//PRE-CARRIAGE
	ExportGRForm_CreateOutline("A", 17, "B", 19, TRUE, TRUE);
	FillCell("A", 17, "Pre-Carriage");

	//PLACE OF RECEIPT BY PRE-CARRIER
	ExportGRForm_CreateOutline("A", 20, "B", 22, TRUE, TRUE);
	FillCell("A", 20, "Place of Receipt");// by Pre-Carrier");

	//VESSEL/FLIGHT No.
	ExportGRForm_CreateOutline("C", 17, "D", 18, TRUE, TRUE);
	FillCell("C", 17, "Vessel/Flight No.");

	//ROTATION No.
	ExportGRForm_CreateOutline("C", 19, "D", 20, TRUE, TRUE);
	FillCell("C", 19, "Rotation No.");

	//PORT OF LOADING
	ExportGRForm_CreateOutline("C", 21, "D", 22, TRUE, TRUE);
	FillCell("C", 21, "Port of loading");
	FillCell("C", 22, "NEW DELHI");

	//PORT OF LOADING
	ExportGRForm_CreateOutline("E", 19, "K", 22, FALSE, TRUE);
	FillCell("E", 19, "Nature of contact:\nCIF   [   ]   /   CFR [   ]   /   FOB [   ]\nOthers");

	//PORT OF DISCHARGE
	ExportGRForm_CreateOutline("A", 23, "B", 24, TRUE, TRUE);
	FillCell("A", 23, "Port of Discharge");
	FillCell("A", 24, country);

	//PORT OF DESTINATION
	ExportGRForm_CreateOutline("C", 23, "D", 24, TRUE, TRUE);
	FillCell("C", 23, "Port of Destination");
	FillCell("C", 24, country);

	//EXCHANGE RATE
	ExportGRForm_CreateOutline("E", 23, "G", 24, TRUE, TRUE);
	str.Format("%.02f",conversion);
	str = currency + " 1.00 = Rs. " + str;
	FillCell("E", 23, "Exchange Rate U/s 14 of CA:");
	FillCell("E", 24, str);

	//CURRENCY OF INVOICE
	ExportGRForm_CreateOutline("H", 23, "K", 24, TRUE, TRUE);
	FillCell("H", 23, "Currency of Invoice");
	FillCell("H", 24, currency);

	//MARK AND NOs. CONTAINER
	ExportGRForm_CreateOutline("A", 25, "B", 26, FALSE, FALSE);
	ExportGRForm_CreateOutline("A", 27, "B", 30, FALSE, FALSE);
	FillCell("A", 25, "Marks & Nos.");

	//No. & KIND OF PKGs.
	ExportGRForm_CreateOutline("C", 25, "D", 26, FALSE, FALSE);
	ExportGRForm_CreateOutline("C", 27, "D", 30, FALSE, FALSE);
	FillCell("C", 25, "No. & kind of Pkgs.\nContainer No.");

	//GR FORM No.
	ExportGRForm_CreateOutline("A", 32, "B", 33, TRUE, FALSE);
	FillCell("A", 32, "GR Form No.:");

	//No. OF PIECES
	ExportGRForm_CreateOutline("A", 34, "B", 35, TRUE, FALSE);
	FillCell("A", 34, "No. Of Pcs.:");
	FillCell("A", 35, pieces);

	//NET WEIGHT
	ExportGRForm_CreateOutline("A", 37, "B", 38, TRUE, FALSE);
	FillCell("A", 37, "Net Weight:");
	FillCell("A", 38, net_weight,3);

	//GROSS WEIGHT
	ExportGRForm_CreateOutline("A", 39, "B", 40, TRUE, FALSE);
	FillCell("A", 39, "Gross Weight:");
	FillCell("A", 40, gross_weight,3);

	//STATISTICAL CODE
	ExportGRForm_CreateOutline("E", 25, "G", 26, FALSE, FALSE);
	ExportGRForm_CreateOutline("E", 27, "G", 27, FALSE, FALSE);
	FillCell("E", 25, "Statistical code & Desc. of goods & EXIM Scheme code where applicable.");

	//GOLD JEWELLERY
	ExportGRForm_CreateOutline("E", 29, "G", 32, FALSE, FALSE);
	FillCell("E", 29, "Gold jewellery of .750 fineness set with pearls, precious & semi precious stones & diamonds cut & polished.");

	//PERSONAL CARRIAGE
	ExportGRForm_CreateOutline("E", 34, "G", 40, FALSE, FALSE);
	FillCell("E", 34, "Personal carriage of studded gold jewellery by ");

	//VALUE
	MergeCells("H", 25, "I", 25);
	SetBorder("H", 25, "I", 41, AllBorders, TRUE, 1);
	SetAlignment("H", 25, CenterAlign, TopAlign);
	FillCell("H", 25, "Quantity (Gms.)");

	MergeCells("H", 29, "I", 29);
	SetAlignment("H", 29, CenterAlign, TopAlign);
	FillCell("H", 29, gross_weight, 3);

	MergeCells("J", 25, "K", 25);
	SetBorder("J", 25, "K", 41, AllBorders, TRUE, 1);
	SetAlignment("J", 25, CenterAlign, TopAlign);
	FillCell("J", 25, CString("Amount (") + currency + CString(")"));

	MergeCells("J", 29, "K", 29);
	SetAlignment("J", 29, CenterAlign, TopAlign);
	FillCell("J", 29, total_value, 2);

	MergeCells("J", 31, "K", 33);
	str.Format("%.02f", (total_value * conversion));
	str = "Equivalent\n Rs. " + str;
	SetAlignment("J", 31, CenterAlign, TopAlign);
	FillCell("J", 31, str);
	
	//TOTAL FOB IN WORDS
	ExportGRForm_CreateOutline("A", 42, "K", 43, TRUE, TRUE);
	FillCell("A", 42, "Total FOB in Words:");

	//ANALYSIS OF EXPORT VALUE
	ExportGRForm_CreateOutline("A", 44, "B", 44, FALSE, FALSE);
	FillCell("A", 44, "Analysis of export value");

	MergeCells("A", 46, "C", 46);
	FillCell("A", 46, "FOB VALUE");
	MergeCells("A", 47, "C", 47);
	FillCell("A", 47, "INSURANCE");
	MergeCells("A", 48, "C", 48);
	FillCell("A", 48, "COMMISION");
	MergeCells("A", 49, "C", 49);
	FillCell("A", 49, "DISCOUNT");
	MergeCells("A", 50, "C", 50);
	FillCell("A", 50, "OTHER DEDUCTIONS");

	MergeCells("D", 45, "E", 45);
	SetAlignment("D", 45, CenterAlign, TopAlign);
	FillCell("D", 45, "Currency");
	MergeCells("D", 46, "E", 46);
	SetAlignment("D", 46, CenterAlign, TopAlign);
	FillCell("D", 46, currency);
	MergeCells("D", 47, "E", 47);
	MergeCells("D", 48, "E", 48);
	MergeCells("D", 49, "E", 49);
	MergeCells("D", 50, "E", 50);

	MergeCells("F", 45, "G", 45);
	SetAlignment("F", 45, CenterAlign, TopAlign);
	FillCell("F", 45, "Amount");
	MergeCells("F", 46, "G", 46);
	SetAlignment("F", 46, CenterAlign, TopAlign);
	FillCell("F", 46, total_value, 2);
	MergeCells("F", 47, "G", 47);
	SetAlignment("F", 47, CenterAlign, TopAlign);
	FillCell("F", 47, "NIL");
	MergeCells("F", 48, "G", 48);
	SetAlignment("F", 48, CenterAlign, TopAlign);
	FillCell("F", 48, "NIL");
	MergeCells("F", 49, "G", 49);
	SetAlignment("F", 49, CenterAlign, TopAlign);
	FillCell("F", 49, "NIL");
	MergeCells("F", 50, "G", 50);
	SetAlignment("F", 50, CenterAlign, TopAlign);
	FillCell("F", 50, "NIL");

	//FULL EXPORT VALUE
	ExportGRForm_CreateOutline("H", 44, "K", 50, FALSE, TRUE);
	str.Format("%.02f",total_value);
	str = "Full export value or where value is not assertainable, the value to receive on sale of goods\nCurrency       " + currency + "\nAmount         " + str;
	FillCell("H", 44, str);

	//UNDERTAKING
	ExportGRForm_CreateOutline("A", 51, "K", 56, FALSE, TRUE);
	FillCell("A", 51, "We undertake to re-import and to produce to The HDFC Bank Ltd. Documentary evidence such as the bill of entry showing their import within 30 days from the date of the close of the exhibition. We also undertake to repartiate the sale proceeds of the goods, if any, sold abroad through normal bankingchannel and to submit a certificate from the bank in support of the amount repartiated immediately on the return of our representative from abroad.");

	//DECLARATION
	ExportGRForm_CreateOutline("A", 57, "K", 64, FALSE, TRUE);
	FillCell("A", 57, "Declaration:\nI/we declare that all particulars given herein are true and correct.\nI/we also attach the declaration(s) under clause no. .......\nPublic Notice No. .............. Date ................");

	SetTextWrap(gAlpha[0], 3, gAlpha[m_last_column], m_end_row);
	SetBorder(gAlpha[0], 3, gAlpha[m_last_column], m_end_row, AllBorders, TRUE, 1);
}

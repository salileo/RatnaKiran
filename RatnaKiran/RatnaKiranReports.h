#if !defined(AFX_RATNAKIRANREPORTS_H__24151B6D_EA72_41B1_890C_16C4AECE1FD0__INCLUDED_)
#define AFX_RATNAKIRANREPORTS_H__24151B6D_EA72_41B1_890C_16C4AECE1FD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExcelWrapper.h"

class CRatnaKiranReports  : public CExcelWrapper
{
public:
	CRatnaKiranReports(BOOL use_excel = TRUE);
	virtual ~CRatnaKiranReports();

	CList<CString, CString> *m_list;
	CString m_filename;
	BOOL m_status;

	//for photo invoice
	int m_image_width;
	int m_image_height;

	//for price quotation
	int m_rows_in_a_page;

	int m_serials_from;
	int m_serials_to;
	BOOL m_use_product_id;
	BOOL m_use_new_serials;
	BOOL m_use_old_serials;
	BOOL m_show_diamond_clarity;
	CString m_old_serial_tagname;
	CList<SStringPairs, SStringPairs> *m_old_serial_tagnames;

	BOOL CreateStocksReportActual(ReportType type);
	BOOL CreateStocksReportMarked(ReportType type);
	BOOL CreateStockPriceQuotation(ReportType type);
	BOOL CreateStockChecklist(ReportType type);
	BOOL CreateStockTags(int row = 1, int column = 1);
	BOOL CreateStockPhotoInvoice(ReportType type);
	BOOL CreateKundanReportActual(ReportType type);
	BOOL CreateKundanReportMarked(ReportType type);
	BOOL CreateKundanChecklist(ReportType type);
	BOOL CreateKundanTags(int row = 1, int column = 1);
	BOOL CreateExportPackageList(ReportType type);
	BOOL CreateExportShortInvoice(ReportType type);
	BOOL CreateExportChecklist(ReportType type);
	BOOL CreateExportTags(int row = 1, int column = 1);
	BOOL CreateExportPriceQuotation(ReportType type);
	BOOL CreateExportGRForm();
	BOOL CreateExportPhotoInvoice(ReportType type);
	BOOL CreateCatalogue();
	
private:
	BOOL m_valid;
	BOOL m_close;

	int m_start_row;
	int m_end_row;
	int m_last_column;
	CList<int, int> m_autofit;
	CList<int, int> m_totals;
	
	int MeasureText(CDC *dc, CString str);
	BOOL InitiateReport();

	void AddAutoFit(int column);
	void ProcessAutoFit();
	void AddTotals(int column, BOOL autofit = TRUE);
	int ProcessTotals(int position);

	void FinalizeLandscapeReport(BOOL add_right_footer = FALSE);
	void FinalizePotraitReport(BOOL pricequotation);
	void FinalizeTagReport();

	void StockReport_Actual_AddTitle(ReportType type);
	void StockReport_Actual_AddHeaders();
	int StockReport_Actual_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type);
	int StockReport_Actual_AddItem(CProductItem *item, CProductVoucher *voucher, int position);
	
	void StockReport_Marked_AddTitle(ReportType type);
	void StockReport_Marked_AddHeaders();
	int StockReport_Marked_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type);
	int StockReport_Marked_AddItem(CProductItem *item, CProductVoucher *voucher, int position);

	void StockPhotoInvoice_AddTitle(ReportType type);
	void StockPhotoInvoice_AddHeaders();
	int StockPhotoInvoice_AddItem(CProductItem *item, CProductVoucher *voucher, int position);

	void StockPriceQuotation_AddTitle(ReportType type);
	void StockPriceQuotation_AddHeaders();
	int StockPriceQuotation_AddSet(CProductSet *set, CProductVoucher *voucher, int position, int orientation, ReportType type);
	int StockPriceQuotation_AddItem(CProductItem *item, CProductVoucher *voucher, int position, int orientation);

	void StockChecklist_AddTitle(ReportType type);
	void StockChecklist_AddHeaders();
	int StockChecklist_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type);
	int StockChecklist_AddItem(CProductItem *item, CProductVoucher *voucher, int position);

	void StockTags_AddItem(CProductItem *item, CProductVoucher *voucher, int column, int row, bool show_values);
	
	void KundanReport_Actual_AddTitle(ReportType type);
	void KundanReport_Actual_AddHeaders();
	int KundanReport_Actual_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type);
	int KundanReport_Actual_AddItem(CProductItem *item, CProductVoucher *voucher, int position);
	
	void KundanReport_Marked_AddTitle(ReportType type);
	void KundanReport_Marked_AddHeaders();
	int KundanReport_Marked_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type);
	int KundanReport_Marked_AddItem(CProductItem *item, CProductVoucher *voucher, int position);

	void KundanPriceQuotation_AddTitle(ReportType type);
	void KundanPriceQuotation_AddHeaders();
	int KundanPriceQuotation_AddSet(CProductSet *set, CProductVoucher *voucher, int position, int orientation, ReportType type);
	int KundanPriceQuotation_AddItem(CProductItem *item, CProductVoucher *voucher, int position, int orientation);

	void KundanChecklist_AddTitle(ReportType type);
	void KundanChecklist_AddHeaders();
	int KundanChecklist_AddSet(CProductSet *set, CProductVoucher *voucher, int position, ReportType type);
	int KundanChecklist_AddItem(CProductItem *item, CProductVoucher *voucher, int position);

	void KundanTags_AddItem(CProductItem *item, CProductVoucher *voucher, int column, int row);

	class Item_Voucher
	{
	public:
		CProductItem *item;
		CProductVoucher *voucher;

		Item_Voucher()
		{
			item = NULL;
			voucher = NULL;
		}
	};

	class Set_Voucher
	{
	public:
		CProductSet *set;
		CProductVoucher *voucher;

		Set_Voucher()
		{
			set = NULL;
			voucher = NULL;
		}
	};

	CProductItem *GetLowestSerialNumberItem(CList<Item_Voucher *, Item_Voucher *> *list, CProductVoucher **voucher);
	CProductSet *GetLowestSerialNumberSet(CList<Set_Voucher *, Set_Voucher *> *list, CProductVoucher **voucher);
	BOOL CalculateExportValues(double *gold_rate, double *currency_conversion, double *labourvalue, double *pearlvalue, 
							   double *diamondvalue, double *stonevalue, double *pricequote, CProductItem *item, CProductVoucher *voucher);
	void FetchExportDetails(CString *consignee, CString *exporter, CString *invoice_number, CString *currency_name, CString *country,
							double *currency_conversion, double *dollar_rate);

	void ExportPackagelist_AddTitle(ReportType type);
	void ExportPackagelist_AddHeaders();
	int ExportPackagelist_AddItem(CProductItem *item, CProductVoucher *voucher, int position);

	void ExportShortInvoice_CreateOutline(char *startcolumn, int startrow, char *endcolumn, int endrow, BOOL hasEntry, BOOL drawBorder);
	void ExportShortInvoice_AddItem(CProductItem *item, CProductVoucher *voucher,
								    int *pieces, double *gross_weight,
									double *net_weight, double *net_value,
									double *diamond_weight, double *diamond_value,
									double *stone_weight, double *stone_value,
									double *pearl_weight, double *pearl_value,
									double *other_weight, double *value_addition,
									double *total_value);
	void ExportShortInvoice_FillValues(int pieces, double gross_weight, 
									   double net_weight, double net_value,
									   double diamond_weight, double diamond_value,
									   double stone_weight, double stone_value,
									   double pearl_weight, double pearl_value,
									   double other_weight, double value_addition,
									   double total_value, ReportType type);

	void ExportChecklist_AddTitle(ReportType type);
	void ExportChecklist_AddHeaders();
	int ExportChecklist_AddItem(CProductItem *item, CProductVoucher *voucher, int position);

	void ExportTags_AddItem(CProductItem *item, CProductVoucher *voucher, int column, int row);

	void ExportPriceQuotation_AddTitle(ReportType type);
	void ExportPriceQuotation_AddHeaders();
	int ExportPriceQuotation_AddSet(CProductSet *set, CProductVoucher *voucher, int position, int orientation, ReportType type);
	int ExportPriceQuotation_AddItem(CProductItem *item, CProductVoucher *voucher, int position, int orientation);

	void ExportGRForm_CreateOutline(char *startcolumn, int startrow, char *endcolumn, int endrow, BOOL hasEntry, BOOL drawBorder);
	void ExportGRForm_AddItem(CProductItem *item, CProductVoucher *voucher,
							  int *pieces, double *gross_weight,
							  double *net_weight, double *total_value);
	void ExportGRForm_FillValues(int pieces, double gross_weight, 
					  			 double net_weight, double total_value);

	void ExportPhotoInvoice_AddTitle(ReportType type);
	void ExportPhotoInvoice_AddHeaders();
	int ExportPhotoInvoice_AddItem(CProductItem *item, CProductVoucher *voucher, int position);
};

#endif // !defined(AFX_RATNAKIRANREPORTS_H__24151B6D_EA72_41B1_890C_16C4AECE1FD0__INCLUDED_)

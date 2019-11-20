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
	CRatnaKiranDatabase *m_database;
	SFileHeaderNode *m_header;
	BOOL m_valid;
	BOOL m_close;
	CString m_filename;

	BOOL m_status;
	int m_serials_from;
	int m_serials_to;
	BOOL m_use_new_serials;
	BOOL m_use_old_serials;
	CString m_old_serial_tagname;
	
	int m_image_count;
	int m_rows_in_a_page;

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

	BOOL CreateStocksReportActual(ReportType type);
	void StockReport_Actual_AddTitle();
	void StockReport_Actual_AddHeaders();
	int StockReport_Actual_AddSet(SProductSetNode *set, int position, ReportType type);
	int StockReport_Actual_AddItem(SProductItemNode *item, int position);
	
	BOOL CreateStocksReportMarked(ReportType type);
	void StockReport_Marked_AddTitle();
	void StockReport_Marked_AddHeaders();
	int StockReport_Marked_AddSet(SProductSetNode *set, int position, ReportType type);
	int StockReport_Marked_AddItem(SProductItemNode *item, int position);

	BOOL CreateStockPriceQuotation();
	void StockPriceQuotation_AddTitle();
	void StockPriceQuotation_AddHeaders();
	int StockPriceQuotation_AddSet(SProductSetNode *set, int position, int orientation);
	int StockPriceQuotation_AddItem(SProductItemNode *item, int position, int orientation);

	BOOL CreateStockChecklist();
	void StockChecklist_AddTitle();
	void StockChecklist_AddHeaders();
	int StockChecklist_AddSet(SProductSetNode *set, int position);
	int StockChecklist_AddItem(SProductItemNode *item, int position);

	BOOL CreateStockTags(int row = 1, int column = 1);
	void StockTags_AddItem(SProductItemNode *item, int column, int row);
	
	BOOL CreateCatalogue();

	BOOL CreateKundanReportActual(ReportType type);
	void KundanReport_Actual_AddTitle();
	void KundanReport_Actual_AddHeaders();
	int KundanReport_Actual_AddSet(SProductSetNode *set, int position, ReportType type);
	int KundanReport_Actual_AddItem(SProductItemNode *item, int position);
	
	BOOL CreateKundanReportMarked(ReportType type);
	void KundanReport_Marked_AddTitle();
	void KundanReport_Marked_AddHeaders();
	int KundanReport_Marked_AddSet(SProductSetNode *set, int position, ReportType type);
	int KundanReport_Marked_AddItem(SProductItemNode *item, int position);

	BOOL CreateKundanPriceQuotation();
	void KundanPriceQuotation_AddTitle();
	void KundanPriceQuotation_AddHeaders();
	int KundanPriceQuotation_AddSet(SProductSetNode *set, int position, int orientation);
	int KundanPriceQuotation_AddItem(SProductItemNode *item, int position, int orientation);

	BOOL CreateKundanChecklist();
	void KundanChecklist_AddTitle();
	void KundanChecklist_AddHeaders();
	int KundanChecklist_AddSet(SProductSetNode *set, int position);
	int KundanChecklist_AddItem(SProductItemNode *item, int position);

	BOOL CreateKundanTags(int row = 1, int column = 1);
	void KundanTags_AddItem(SProductItemNode *item, int column, int row);

	SProductItemNode *GetLowestSerialNumberItem(CList<SProductItemNode *, SProductItemNode *> *list);
	SProductSetNode *GetLowestSerialNumberSet(CList<SProductSetNode *, SProductSetNode *> *list);
	void CalculateExportValues(double *gold_rate, double *currency_conversion, double *labourvalue, double *pearlvalue, 
							   double *diamondvalue, double *stonevalue, double *pricequote, SProductItemNode *item);
	void FetchExportDetails(CString *consignee, CString *exporter, CString *invoice_number, CString *currency_name, CString *country,
							double *currency_conversion, double *dollar_rate);

	BOOL CreatePackageList(ReportType type);
	void Packagelist_AddTitle(ReportType type);
	void Packagelist_AddHeaders();
	int Packagelist_AddItem(SProductItemNode *item, int position);

	BOOL CreateShortInvoice(ReportType type);
	void ShortInvoice_CreateOutline(char *startcolumn, int startrow, char *endcolumn, int endrow, BOOL hasEntry, BOOL drawBorder);
	void ShortInvoice_AddItem(SProductItemNode *item,
							  int *pieces, double *gross_weight,
							  double *net_weight, double *net_value,
							  double *diamond_weight, double *diamond_value,
							  double *stone_weight, double *stone_value,
							  double *pearl_weight, double *pearl_value,
							  double *other_weight, double *value_addition,
							  double *total_value);
	void ShortInvoice_FillValues(int pieces, double gross_weight, 
								double net_weight, double net_value,
								double diamond_weight, double diamond_value,
								double stone_weight, double stone_value,
								double pearl_weight, double pearl_value,
								double other_weight, double value_addition,
								double total_value, ReportType type);

	BOOL CreateExportChecklist();
	void ExportChecklist_AddTitle();
	void ExportChecklist_AddHeaders();
	int ExportChecklist_AddItem(SProductItemNode *item, int position);

	BOOL CreateExportTags(int row = 1, int column = 1);
	void ExportTags_AddItem(SProductItemNode *item, int column, int row);

	BOOL CreateExportPriceQuotation();
	void ExportPriceQuotation_AddTitle();
	void ExportPriceQuotation_AddHeaders();
	int ExportPriceQuotation_AddSet(SProductSetNode *set, int position, int orientation);
	int ExportPriceQuotation_AddItem(SProductItemNode *item, int position, int orientation);

	BOOL CreateGRForm();
	void GRForm_CreateOutline(char *startcolumn, int startrow, char *endcolumn, int endrow, BOOL hasEntry, BOOL drawBorder);
	void GRForm_AddItem(SProductItemNode *item,
					    int *pieces, double *gross_weight,
					    double *net_weight, double *total_value);
	void GRForm_FillValues(int pieces, double gross_weight, 
					  	   double net_weight, double total_value);

	BOOL CreatePhotoInvoice(ReportType type);
	void PhotoInvoice_AddTitle(ReportType type);
	void PhotoInvoice_AddHeaders();
	int PhotoInvoice_AddItem(SProductItemNode *item, int position);
};

#endif // !defined(AFX_RATNAKIRANREPORTS_H__24151B6D_EA72_41B1_890C_16C4AECE1FD0__INCLUDED_)

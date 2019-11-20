#if !defined(AFX_EXCELWRAPPER_H__24151B6D_EA72_41B1_890C_16C4AECE1FD0__INCLUDED_)
#define AFX_EXCELWRAPPER_H__24151B6D_EA72_41B1_890C_16C4AECE1FD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "excel.h"

#define LEFTALIGN 1
#define TOPALIGN 1
#define CENTERALIGN 2
#define RIGHTALIGN 3
#define BOTTOMALIGN 3

#define LEFTBORDER 1
#define RIGHTBORDER 2
#define TOPBORDER 4
#define BOTTOMBORDER 8
#define ALLBORDERS 15

#define POTRAIT 1
#define LANDSCAPE 2

extern COleVariant covTrue, covFalse, covOptional;

class CExcelWrapper
{
public:
	CExcelWrapper(void);
	~CExcelWrapper(void);

	Excel_Application m_app;
	ExcelWorkbooks m_books;
	Excel_Workbook m_book;
	ExcelWorksheets m_sheets;
	Excel_Worksheet m_sheet;
	BOOL m_excel_started;
	CList <CString, CString> m_file_list;

	BOOL StartExcel();
	BOOL StopExcel();
	BOOL DisplayExcel();
	CString GetAddress(const char *column, int row);
	void FillCell(const char *column, int row, BYTE *image_data, DWORD image_size);
	void FillCell(const char *column, int row, int value);
	void FillCell(const char *column, int row, double value, int rounded);
	void FillCell(const char *column, int row, const char *value);
	void FillCell(const char *column, int row, CString value);
	void FillCellWithFormula(const char *column, int row, const char *formula);
	void MergeCells(const char *startcolumn, int startrow, const char *endcolumn, int endrow);
	void SetBold(const char *column, int row, BOOL enable = TRUE);
	void SetBold(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable = TRUE);
	void SetItalic(const char *column, int row, BOOL enable = TRUE);
	void SetItalic(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable = TRUE);
	void SetUnderline(const char *column, int row, BOOL enable = TRUE);
	void SetUnderline(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable = TRUE);
	void SetTextWrap(const char *column, int row, BOOL enable = TRUE);
	void SetTextWrap(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable = TRUE);
	void SetFontSize(const char *column, int row, int size);
	void SetFontSize(const char *startcolumn, int startrow, const char *endcolumn, int endrow, int size);
	void SetFontColor(const char *column, int row, int color);
	void SetFontColor(const char *startcolumn, int startrow, const char *endcolumn, int endrow, int color);
	void SetCharsBold(const char *column, int row, int from, int length, BOOL enable);
	void SetCharsItalic(const char *column, int row, int from, int length, BOOL enable);
	void SetCharsUnderline(const char *column, int row, int from, int length, BOOL enable);
	void SetCharsSize(const char *column, int row, int from, int length, int size);
	void SetCharsColor(const char *column, int row, int from, int length, int color);
	void SetAlignment(const char *startcolumn, int startrow, const char *endcolumn, int endrow, int horizontal, int vertical);
	void SetAlignment(const char *column, int row, int horizontal, int vertical);
	void SetBorders(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable);
	void SetBorder(const char *startcolumn, int startrow, const char *endcolumn, int endrow, int place, BOOL enable, int weight);
	void SetAutoFit(const char *column);
	void SetWidth(const char *startcolumn, const char *endcolumn, double width);
	void SetHeight(int startrow, int endrow, double height);
};

#endif // !defined(AFX_EXCELWRAPPER_H__24151B6D_EA72_41B1_890C_16C4AECE1FD0__INCLUDED_)

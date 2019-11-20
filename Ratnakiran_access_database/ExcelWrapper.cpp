#include "stdafx.h"
#include "RatnaKiran.h"
#include "ExcelWrapper.h"

COleVariant	covTrue((short)TRUE),
			covFalse((short)FALSE),
			covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);


CExcelWrapper::CExcelWrapper(void)
{
	m_excel_started = FALSE;
}


CExcelWrapper::~CExcelWrapper(void)
{
	StopExcel();

	while(!m_file_list.IsEmpty())
	{
		CString filename = m_file_list.RemoveHead();

		try
		{
			CFile::Remove((LPCTSTR)filename);
		}
		catch(CFileException *removeerror)
		{
			CString errstr;
			errstr.Format("ERROR : Internal error #%d during removing of temporary file ", removeerror->m_cause);
			errstr = errstr + filename + ".";
			MessageBox(NULL, errstr, "ERROR", MB_OK);
		}
	}

	m_excel_started = FALSE;
}


BOOL CExcelWrapper::StartExcel()
{
	if(!m_app.CreateDispatch("Excel.Application"))
	{
		MessageBox(NULL, "Couldn't start Excel and get application object.", "ERROR", MB_OK);
		return FALSE;
	}

	LPDISPATCH lpDisp;

	lpDisp = m_app.GetWorkbooks();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return FALSE;
	}

	m_books.AttachDispatch(lpDisp);
	lpDisp = m_books.Add(covOptional);
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return FALSE;
	}
	
	m_book.AttachDispatch(lpDisp);
	lpDisp = m_book.GetSheets();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return FALSE;
	}

	m_sheets.AttachDispatch(lpDisp);
	lpDisp = m_sheets.GetItem(COleVariant((short)1));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return FALSE;
	}
	
	m_sheet.AttachDispatch(lpDisp);
	m_excel_started = TRUE;
	return TRUE;
}


BOOL CExcelWrapper::StopExcel()
{
	if(m_excel_started)
	{
		m_book.Close(covFalse,covOptional,covOptional);
		m_books.Close();
		m_app.Quit();
		m_excel_started = FALSE;
	}

	return TRUE;
}


BOOL CExcelWrapper::DisplayExcel()
{
	if(m_excel_started)
	{
		m_app.SetVisible(TRUE);
		m_app.SetUserControl(TRUE);
		m_excel_started = FALSE;
	}

	return TRUE;
}


CString CExcelWrapper::GetAddress(const char *column, int row)
{
	CString buf;
	buf.Format("%s%d",column,row);
	return buf;
}


void CExcelWrapper::FillCell(const char *column, int row, BYTE *image_data, DWORD image_size)
{
	if(!image_data || image_size == 0)
		return;

	double cell_width, cell_height;
	double cell_xpos, cell_ypos;
	const int margin = 3;

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	VARIANT var;
	range.AttachDispatch(lpDisp);

	var = range.GetWidth();
	cell_width = var.dblVal;

	var = range.GetHeight();
	cell_height = var.dblVal;

	var = range.GetLeft();
	cell_xpos = var.dblVal;

	var = range.GetTop();
	cell_ypos = var.dblVal;

	static int imagecounter = 0;
	CFile imageFile;
	CFileException e;
	CString filepath;
	filepath.Format("c:\\tmpimage_%d.jpg", imagecounter++);

	try
	{
		if(imageFile.Open(LPCTSTR(filepath), CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary, &e))
		{
			if(image_size >= 0)
				imageFile.Write(image_data, image_size);

			imageFile.Close();
			m_file_list.AddTail(filepath);
		}
		else
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}
	}
	catch(CFileException *writeerror)
	{
		CString errstr;
		errstr.Format("ERROR : Internal error #%d during creation of temporary image files.", writeerror->m_cause);
		MessageBox(NULL, errstr, "ERROR", MB_OK);
		return;
	}

	ExcelShapes shapes;
	lpDisp = m_sheet.GetShapes();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	static int count = 0;
	shapes.AttachDispatch(lpDisp);
	lpDisp = shapes.AddPicture(LPCTSTR(filepath), FALSE, TRUE, (float)(cell_xpos + margin), (float)(cell_ypos + margin), -1, -1);
	count++;

	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	CSize original, desired, final;
	ExcelPicture picture;
	picture.AttachDispatch(lpDisp);

	original.cx = (long)picture.GetWidth();
	original.cy = (long)picture.GetHeight();

	//salil : hardcoded values
	desired.cx = (LONG)(cell_width - (2 * margin));
	desired.cy = (LONG)(cell_height - (2 * margin));

	CTableItem::ScaleImage(original, desired, &final);

	picture.SetWidth(final.cx);
	picture.SetHeight(final.cy);

	double original_x = picture.GetLeft();
	double original_y = picture.GetTop();

	double final_x = original_x + ((desired.cx - final.cx) / 2.0);
	double final_y = original_y + ((desired.cy - final.cy) / 2.0);

	picture.SetLeft(final_x);
	picture.SetTop(final_y);
}


void CExcelWrapper::FillCell(const char *column, int row, int value)
{
	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	CString str;
	str.Format("%d",value);
	range.AttachDispatch(lpDisp);
	range.SetValue2(COleVariant(str));

	str = "#,##0";
	range.SetNumberFormat(COleVariant(str));
}

void CExcelWrapper::FillCell(const char *column, int row, double value, int rounded)
{
	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	value = RoundOff(value, rounded);
	CString format;
	format.Format("%d",rounded);
	format = "%.0" + format + "f";

	CString str;
	str.Format(LPCTSTR(format),value);
	range.AttachDispatch(lpDisp);
	range.SetValue2(COleVariant(str));

	rounded = max(1, rounded);
	str = "#,##0.";

	while(rounded > 0)
	{
		str = str + "0";
		rounded--;
	}
	
	range.SetNumberFormat(COleVariant(str));
}

void CExcelWrapper::FillCell(const char *column, int row, const char *value)
{
	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	range.SetValue2(COleVariant(value));
}

void CExcelWrapper::FillCell(const char *column, int row, CString value)
{
	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	range.SetValue2(COleVariant(value));
}

void CExcelWrapper::FillCellWithFormula(const char *column, int row, const char *formula)
{
	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	range.SetFormula(COleVariant(formula));
}

void CExcelWrapper::MergeCells(const char *startcolumn, int startrow, const char *endcolumn, int endrow)
{
	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn, endrow)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	range.Merge(covOptional);
}


void CExcelWrapper::SetBold(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable)
{
	ExcelRange range;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetFont();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	font.AttachDispatch(lpDisp);

	if(enable)
		font.SetBold(covTrue);
	else
		font.SetBold(covFalse);
}


void CExcelWrapper::SetBold(const char *column, int row, BOOL enable)
{
	SetBold(column, row, column, row, enable);
}


void CExcelWrapper::SetItalic(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable)
{
	ExcelRange range;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetFont();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	font.AttachDispatch(lpDisp);

	if(enable)
		font.SetItalic(covTrue);
	else
		font.SetItalic(covFalse);
}


void CExcelWrapper::SetItalic(const char *column, int row, BOOL enable)
{
	SetItalic(column, row, column, row, enable);
}


void CExcelWrapper::SetUnderline(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable)
{
	ExcelRange range;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetFont();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	font.AttachDispatch(lpDisp);

	if(enable)
		font.SetUnderline(covTrue);
	else
		font.SetUnderline(covFalse);
}


void CExcelWrapper::SetUnderline(const char *column, int row, BOOL enable)
{
	SetUnderline(column, row, column, row, enable);
}


void CExcelWrapper::SetFontSize(const char *startcolumn, int startrow, const char *endcolumn, int endrow, int size)
{
	ExcelRange range;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetFont();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	font.AttachDispatch(lpDisp);
	font.SetSize(COleVariant((short)size));
}


void CExcelWrapper::SetFontSize(const char *column, int row, int size)
{
	SetFontSize(column, row, column, row, size);
}


void CExcelWrapper::SetFontColor(const char *startcolumn, int startrow, const char *endcolumn, int endrow, int color)
{
	ExcelRange range;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetFont();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	font.AttachDispatch(lpDisp);
	font.SetColor(COleVariant((short)color));
}


void CExcelWrapper::SetFontColor(const char *column, int row, int color)
{
	SetFontColor(column, row, column, row, color);
}


void CExcelWrapper::SetCharsSize(const char *column, int row, int from, int length, int size)
{
	ExcelRange range;
	ExcelRange chars;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetCharacters(COleVariant((short)from), COleVariant((short)length));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	chars.AttachDispatch(lpDisp);
	lpDisp = chars.GetFont();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	font.AttachDispatch(lpDisp);
	font.SetSize(COleVariant((short)size));
}


void CExcelWrapper::SetCharsBold(const char *column, int row, int from, int length, BOOL enable)
{
	ExcelRange range;
	ExcelRange chars;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetCharacters(COleVariant((short)from), COleVariant((short)length));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	chars.AttachDispatch(lpDisp);
	lpDisp = chars.GetFont();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	font.AttachDispatch(lpDisp);
	if(enable)
		font.SetBold(covTrue);
	else
		font.SetBold(covFalse);
}


void CExcelWrapper::SetCharsItalic(const char *column, int row, int from, int length, BOOL enable)
{
	ExcelRange range;
	ExcelRange chars;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetCharacters(COleVariant((short)from), COleVariant((short)length));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	chars.AttachDispatch(lpDisp);
	lpDisp = chars.GetFont();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	font.AttachDispatch(lpDisp);
	if(enable)
		font.SetItalic(covTrue);
	else
		font.SetItalic(covFalse);
}


void CExcelWrapper::SetCharsUnderline(const char *column, int row, int from, int length, BOOL enable)
{
	ExcelRange range;
	ExcelRange chars;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetCharacters(COleVariant((short)from), COleVariant((short)length));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	chars.AttachDispatch(lpDisp);
	lpDisp = chars.GetFont();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	font.AttachDispatch(lpDisp);
	if(enable)
		font.SetUnderline(covTrue);
	else
		font.SetUnderline(covFalse);
}


void CExcelWrapper::SetCharsColor(const char *column, int row, int from, int length, int color)
{
	ExcelRange range;
	ExcelRange chars;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetCharacters(COleVariant((short)from), COleVariant((short)length));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	chars.AttachDispatch(lpDisp);
	lpDisp = chars.GetFont();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	font.AttachDispatch(lpDisp);
	font.SetColor(COleVariant((short)color));
}


void CExcelWrapper::SetBorders(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable)
{
	ExcelRange range;
	ExcelBorders borders;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetBorders();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	borders.AttachDispatch(lpDisp);
	if(enable)
		borders.SetWeight(COleVariant((short)3));
	else
		borders.SetWeight(COleVariant((short)0));		
}

void CExcelWrapper::SetBorder(const char *startcolumn, int startrow, const char *endcolumn, int endrow, int place, BOOL enable, int weight)
{
	//1 = Left border
	//2 = Right border
	//3 = Top border
	//4 = Bottom border

	if(place & LEFTBORDER)
	{
		ExcelRange range;
		ExcelBorders borders;
		ExcelBorder border;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(startcolumn,endrow)));
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		range.AttachDispatch(lpDisp);
		lpDisp = range.GetBorders();
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		borders.AttachDispatch(lpDisp);
		lpDisp = borders.GetItem((long)1);
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		border.AttachDispatch(lpDisp);
		if(enable)
			border.SetWeight(COleVariant((short)weight));
		else
			border.SetWeight(COleVariant((short)0));		
	}

	if(place & RIGHTBORDER)
	{
		ExcelRange range;
		ExcelBorders borders;
		ExcelBorder border;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetRange(COleVariant(GetAddress(endcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		range.AttachDispatch(lpDisp);
		lpDisp = range.GetBorders();
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		borders.AttachDispatch(lpDisp);
		lpDisp = borders.GetItem((long)2);
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		border.AttachDispatch(lpDisp);
		if(enable)
			border.SetWeight(COleVariant((short)weight));
		else
			border.SetWeight(COleVariant((short)0));		
	}

	if(place & TOPBORDER)
	{
		ExcelRange range;
		ExcelBorders borders;
		ExcelBorder border;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,startrow)));
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		range.AttachDispatch(lpDisp);
		lpDisp = range.GetBorders();
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		borders.AttachDispatch(lpDisp);
		lpDisp = borders.GetItem((long)3);
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		border.AttachDispatch(lpDisp);
		if(enable)
			border.SetWeight(COleVariant((short)weight));
		else
			border.SetWeight(COleVariant((short)0));		
	}

	if(place & BOTTOMBORDER)
	{
		ExcelRange range;
		ExcelBorders borders;
		ExcelBorder border;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,endrow)),COleVariant(GetAddress(endcolumn,endrow)));
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		range.AttachDispatch(lpDisp);
		lpDisp = range.GetBorders();
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		borders.AttachDispatch(lpDisp);
		lpDisp = borders.GetItem((long)4);
		if(!lpDisp)
		{
			UnHandledError(__FILE__, __LINE__);
			return;
		}

		border.AttachDispatch(lpDisp);
		if(enable)
			border.SetWeight(COleVariant((short)weight));
		else
			border.SetWeight(COleVariant((short)0));		
	}
}


void CExcelWrapper::SetAlignment(const char *startcolumn, int startrow, const char *endcolumn, int endrow, int horizontal, int vertical)
{
	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	//For Horizontal:
	//1 = default, 2 = left, 3 = centre, 4 = right

	//For Vertical:
	//1 = top, 2 = centre, 3 = bottom

	range.AttachDispatch(lpDisp);
	range.SetHorizontalAlignment(COleVariant((short)(horizontal + 1)));
	range.SetVerticalAlignment(COleVariant((short)vertical));
}


void CExcelWrapper::SetAlignment(const char *column, int row, int horizontal, int vertical)
{
	SetAlignment(column, row, column, row, horizontal, vertical);
}


void CExcelWrapper::SetTextWrap(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable)
{
	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	
	if(enable)
		range.SetWrapText(covTrue);
	else
		range.SetWrapText(covFalse);
}


void CExcelWrapper::SetTextWrap(const char *column, int row, BOOL enable)
{
	SetTextWrap(column, row, column, row, enable);
}


void CExcelWrapper::SetAutoFit(const char *column)
{
	ExcelRange range;
	ExcelRange cols;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,1)),COleVariant(GetAddress(column,1)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetEntireColumn();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	cols.AttachDispatch(lpDisp);
	cols.AutoFit();
}


void CExcelWrapper::SetWidth(const char *startcolumn, const char *endcolumn, double width)
{
	ExcelRange range;
	ExcelRange cols;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,1)),COleVariant(GetAddress(endcolumn,1)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetEntireColumn();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	cols.AttachDispatch(lpDisp);
	cols.SetColumnWidth(COleVariant((double)width));
}


void CExcelWrapper::SetHeight(int startrow, int endrow, double height)
{
	ExcelRange range;
	ExcelRange rows;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress("A",startrow)),COleVariant(GetAddress("A",endrow)));
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetEntireRow();
	if(!lpDisp)
	{
		UnHandledError(__FILE__, __LINE__);
		return;
	}

	rows.AttachDispatch(lpDisp);
	rows.SetRowHeight(COleVariant((double)height));
}
#include "ExcelWrapper.h"
#include "Common.h"
#include <math.h>

COleVariant	covTrue((short)TRUE),
			covFalse((short)FALSE),
			covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

static void ScaleImage(CSize original, CSize desired, CSize *final)
{
	if(!final)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	double dx = (double)original.cx / desired.cx;
	double dy = (double)original.cy / desired.cy;

	if(dx >= dy)
	{
		final->cx = desired.cx;
		final->cy = (int)(original.cy / dx);
	}
	else
	{
		final->cx = (int)(original.cx / dy);
		final->cy = desired.cy;
	}
}


static double RoundOff(double val, int decimal_places)
{
	int tempval = (int)((val * pow(10.0, (double)decimal_places)) + 0.5);
	val = ((double)tempval)/ pow(10.0, (double)decimal_places);
	return val;
}


CExcelWrapper::CExcelWrapper(void)
{
	m_excel_started = FALSE;
}


CExcelWrapper::~CExcelWrapper(void)
{
	if(m_excel_started)
	{
		if(!StopExcel())
			SHOW_INTERNAL_ERROR_REASON("Could not quit excel.");
	}

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
			errstr.Format("ERROR : #%d.", removeerror->m_cause);
			errstr = "Could not remove temporary file '" + filename + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
		}
	}

	m_excel_started = FALSE;
}


BOOL CExcelWrapper::StartExcel()
{
	if(!m_app.CreateDispatch("Excel.Application"))
	{
		SHOW_INTERNAL_ERROR_REASON("Could not start Microsoft Office Excel");
		return FALSE;
	}

	LPDISPATCH lpDisp;

	lpDisp = m_app.GetWorkbooks();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not get workbooks in Excel.");
		return FALSE;
	}

	m_books.AttachDispatch(lpDisp);
	lpDisp = m_books.Add(covOptional);
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not add a workbook in Excel.");
		return FALSE;
	}
	
	m_book.AttachDispatch(lpDisp);
	lpDisp = m_book.GetSheets();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not get sheets in Excel.");
		return FALSE;
	}

	m_sheets.AttachDispatch(lpDisp);
	lpDisp = m_sheets.GetItem(COleVariant((short)1));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR_REASON("Could not get first sheet in Excel.");
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
		return TRUE;
	}
	else
	{
		SHOW_INTERNAL_ERROR_REASON("Trying to close excel, when it is not running.");
		return FALSE;
	}
}


BOOL CExcelWrapper::DisplayExcel()
{
	if(m_excel_started)
	{
		m_app.SetVisible(TRUE);
		m_app.SetUserControl(TRUE);
		m_excel_started = FALSE;
		return TRUE;
	}
	else
	{
		SHOW_INTERNAL_ERROR_REASON("Trying to display excel, when it is not running.");
		return FALSE;
	}
}


CString CExcelWrapper::GetAddress(const char *column, int row)
{
	CString buf;

	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return buf;
	}

	buf.Format("%s%d",column,row);
	return buf;
}


void CExcelWrapper::FillCellWithImage(const char *column, int row, CString image_file)
{
	if(!column || image_file.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	if(!IsFileExisting(image_file))
	{
		CString errstr = "Could not add image to cell '" + GetAddress(column,row) + "' as image file '" + image_file + "' does not exist.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}

	double cell_width, cell_height;
	double cell_xpos, cell_ypos;
	const int margin = 1;

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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

	ExcelShapes shapes;
	lpDisp = m_sheet.GetShapes();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	shapes.AttachDispatch(lpDisp);
	lpDisp = shapes.AddPicture(LPCTSTR(image_file), FALSE, TRUE, (float)(cell_xpos + margin), (float)(cell_ypos + margin), -1, -1);
	if(!lpDisp)
	{
		CString errstr = "Could not add image '" + image_file + "' to cell '" + GetAddress(column,row) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
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

	ScaleImage(original, desired, &final);

	picture.SetWidth(final.cx);
	picture.SetHeight(final.cy);

	double original_x = picture.GetLeft();
	double original_y = picture.GetTop();

	double final_x = original_x + ((desired.cx - final.cx) / 2.0);
	double final_y = original_y + ((desired.cy - final.cy) / 2.0);

	picture.SetLeft(final_x);
	picture.SetTop(final_y);
}


void CExcelWrapper::FillCellWithImage(const char *column, int row, BYTE *image_data, DWORD image_size)
{
	if(!column || !image_data || (image_size < 0))
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	double cell_width, cell_height;
	double cell_xpos, cell_ypos;
	const int margin = 1;

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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

	char buf[MAX_PATH];
	if(GetTempPath(MAX_PATH - 1, buf) == 0)
		SHOW_INTERNAL_ERROR_LAST("Could not get a temporary file for storing image info.");

	CString folderpath = buf;
	CString filepath;
	static int imagecounter = 0;
	filepath.Format("tmpimage_%d.jpg", imagecounter++);
	filepath = folderpath + filepath;

	CFile imageFile;
	CFileException e;
	if(imageFile.Open(LPCTSTR(filepath), CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary, &e))
	{
		m_file_list.AddTail(filepath);

		try
		{
			imageFile.Write(image_data, image_size);
		}
		catch(CFileException *writeerror)
		{
			CString errstr;
			errstr.Format("ERROR : #%d.", writeerror->m_cause);
			errstr = "Could not write temporary file '" + filepath + "'.\n" + errstr;
			SHOW_INTERNAL_ERROR_REASON(errstr);
			imageFile.Close();
			return;
		}

		imageFile.Close();
	}
	else
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", e.m_cause);
		errstr = "Could not open image file '" + filepath + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return;
	}

	ExcelShapes shapes;
	lpDisp = m_sheet.GetShapes();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	shapes.AttachDispatch(lpDisp);
	lpDisp = shapes.AddPicture(LPCTSTR(filepath), FALSE, TRUE, (float)(cell_xpos + margin), (float)(cell_ypos + margin), -1, -1);
	if(!lpDisp)
	{
		CString errstr = "Could not add image '" + filepath + "' to cell '" + GetAddress(column,row) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
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

	ScaleImage(original, desired, &final);

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
	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!column || !value)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	range.SetValue2(COleVariant(value));
}


void CExcelWrapper::FillCell(const char *column, int row, CString value)
{
	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	range.SetValue2(COleVariant(value));
}


void CExcelWrapper::FillCellWithFormula(const char *column, int row, const char *formula)
{
	if(!column || !formula)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	range.SetFormula(COleVariant(formula));
}


void CExcelWrapper::MergeCells(const char *startcolumn, int startrow, const char *endcolumn, int endrow)
{
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn, endrow)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	range.Merge(covOptional);
}


void CExcelWrapper::SetBold(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable)
{
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetFont();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetFont();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetFont();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetFont();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetFont();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelRange chars;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetCharacters(COleVariant((short)from), COleVariant((short)length));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	chars.AttachDispatch(lpDisp);
	lpDisp = chars.GetFont();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	font.AttachDispatch(lpDisp);
	font.SetSize(COleVariant((short)size));
}


void CExcelWrapper::SetCharsBold(const char *column, int row, int from, int length, BOOL enable)
{
	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelRange chars;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetCharacters(COleVariant((short)from), COleVariant((short)length));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	chars.AttachDispatch(lpDisp);
	lpDisp = chars.GetFont();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelRange chars;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetCharacters(COleVariant((short)from), COleVariant((short)length));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	chars.AttachDispatch(lpDisp);
	lpDisp = chars.GetFont();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelRange chars;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetCharacters(COleVariant((short)from), COleVariant((short)length));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	chars.AttachDispatch(lpDisp);
	lpDisp = chars.GetFont();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelRange chars;
	ExcelFont font;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetCharacters(COleVariant((short)from), COleVariant((short)length));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	chars.AttachDispatch(lpDisp);
	lpDisp = chars.GetFont();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	font.AttachDispatch(lpDisp);
	font.SetColor(COleVariant((short)color));
}


void CExcelWrapper::SetBorders(const char *startcolumn, int startrow, const char *endcolumn, int endrow, BOOL enable)
{
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelBorders borders;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetBorders();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	//1 = Left border
	//2 = Right border
	//3 = Top border
	//4 = Bottom border

	if(place & LeftBorder)
	{
		ExcelRange range;
		ExcelBorders borders;
		ExcelBorder border;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(startcolumn,endrow)));
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		range.AttachDispatch(lpDisp);
		lpDisp = range.GetBorders();
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		borders.AttachDispatch(lpDisp);
		lpDisp = borders.GetItem((long)1);
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		border.AttachDispatch(lpDisp);
		if(enable)
			border.SetWeight(COleVariant((short)weight));
		else
			border.SetWeight(COleVariant((short)0));		
	}

	if(place & RightBorder)
	{
		ExcelRange range;
		ExcelBorders borders;
		ExcelBorder border;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetRange(COleVariant(GetAddress(endcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		range.AttachDispatch(lpDisp);
		lpDisp = range.GetBorders();
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		borders.AttachDispatch(lpDisp);
		lpDisp = borders.GetItem((long)2);
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		border.AttachDispatch(lpDisp);
		if(enable)
			border.SetWeight(COleVariant((short)weight));
		else
			border.SetWeight(COleVariant((short)0));		
	}

	if(place & TopBorder)
	{
		ExcelRange range;
		ExcelBorders borders;
		ExcelBorder border;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,startrow)));
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		range.AttachDispatch(lpDisp);
		lpDisp = range.GetBorders();
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		borders.AttachDispatch(lpDisp);
		lpDisp = borders.GetItem((long)3);
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		border.AttachDispatch(lpDisp);
		if(enable)
			border.SetWeight(COleVariant((short)weight));
		else
			border.SetWeight(COleVariant((short)0));		
	}

	if(place & BottomBorder)
	{
		ExcelRange range;
		ExcelBorders borders;
		ExcelBorder border;
		LPDISPATCH lpDisp;

		lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,endrow)),COleVariant(GetAddress(endcolumn,endrow)));
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		range.AttachDispatch(lpDisp);
		lpDisp = range.GetBorders();
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
			return;
		}

		borders.AttachDispatch(lpDisp);
		lpDisp = borders.GetItem((long)4);
		if(!lpDisp)
		{
			SHOW_INTERNAL_ERROR;
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
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,startrow)),COleVariant(GetAddress(endcolumn,endrow)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelRange cols;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,1)),COleVariant(GetAddress(column,1)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetEntireColumn();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	cols.AttachDispatch(lpDisp);
	cols.AutoFit();
}


void CExcelWrapper::SetWidth(const char *startcolumn, const char *endcolumn, double width)
{
	if(!startcolumn || !endcolumn)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	ExcelRange cols;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(startcolumn,1)),COleVariant(GetAddress(endcolumn,1)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetEntireColumn();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
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
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	lpDisp = range.GetEntireRow();
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	rows.AttachDispatch(lpDisp);
	rows.SetRowHeight(COleVariant((double)height));
}


void CExcelWrapper::SetPageBreak(const char *column, int row)
{
	if(!column)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	ExcelRange range;
	LPDISPATCH lpDisp;

	lpDisp = m_sheet.GetRange(COleVariant(GetAddress(column,row)),COleVariant(GetAddress(column,row)));
	if(!lpDisp)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	range.AttachDispatch(lpDisp);
	range.SetPageBreak(1);
}


char gAlpha[][3] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", 
					"AA", "AB", "AC", "AD", "AE", "AF", "AG", "AH", "AI", "AJ", "AK", "AL", "AM", "AN", "AO", "AP", "AQ", "AR", "AS", "AT", "AU", "AV", "AW", "AX", "AY", "AZ", 
					"BA", "BB", "BC", "BD", "BE", "BF", "BG", "BH", "BI", "BJ", "BK", "BL", "BM", "BN", "BO", "BP", "BQ", "BR", "BS", "BT", "BU", "BV", "BW", "BX", "BY", "BZ", 
					"CA", "CB", "CC", "CD", "CE", "CF", "CG", "CH", "CI", "CJ", "CK", "CL", "CM", "CN", "CO", "CP", "CQ", "CR", "CS", "CT", "CU", "CV", "CW", "CX", "CY", "CZ", 
					"DA", "DB", "DC", "DD", "DE", "DF", "DG", "DH", "DI", "DJ", "DK", "DL", "DM", "DN", "DO", "DP", "DQ", "DR", "DS", "DT", "DU", "DV", "DW", "DX", "DY", "DZ", 
					"EA", "EB", "EC", "ED", "EE", "EF", "EG", "EH", "EI", "EJ", "EK", "EL", "EM", "EN", "EO", "EP", "EQ", "ER", "ES", "ET", "EU", "EV", "EW", "EX", "EY", "EZ", 
					"FA", "FB", "FC", "FD", "FE", "FF", "FG", "FH", "FI", "FJ", "FK", "FL", "FM", "FN", "FO", "FP", "FQ", "FR", "FS", "FT", "FU", "FV", "FW", "FX", "FY", "FZ", 
					"GA", "GB", "GC", "GD", "GE", "GF", "GG", "GH", "GI", "GJ", "GK", "GL", "GM", "GN", "GO", "GP", "GQ", "GR", "GS", "GT", "GU", "GV", "GW", "GX", "GY", "GZ", 
					"HA", "HB", "HC", "HD", "HE", "HF", "HG", "HH", "HI", "HJ", "HK", "HL", "HM", "HN", "HO", "HP", "HQ", "HR", "HS", "HT", "HU", "HV", "HW", "HX", "HY", "HZ", 
					"IA", "IB", "IC", "ID", "IE", "IF", "IG", "IH", "II", "IJ", "IK", "IL", "IM", "IN", "IO", "IP", "IQ", "IR", "IS", "IT", "IU", "IV", "IW", "IX", "IY", "IZ"} ;

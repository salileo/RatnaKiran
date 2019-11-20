#include "DatabaseGlobals.h"

CString gFilePath;
CList<CString, CString> gIDList;

static void ReplaceStringWithValue(CString *formula, CString string, double value);
static BOOL DoParenthesis(CString *formula);
static BOOL DoParenthesisFor(CString *formula, CString exp);
static double DoCalculation(CString formula);

double GetGoldRate(CProductVoucher *voucher)
{
	if(voucher->m_gold_rate == -1)
		return CSettings::GetGoldRate();
	else
		return voucher->m_gold_rate;
}

double ProcessFormula(CString formula, double gold_rate, double conversion, CProductItem *item)
{
	double val = 0.0;

	/*
	GW = gross weight
	NW = net weight
	PW = pearl weight
	DW = diamond weight
	PRW = precious stone weight
	OW = other weight

	GVG = gold value using gross weight
	GVGW = gold value using gross weight and including wastage
	GVN = gold value using net weight
	GVNW = gold value using net weight and including wastage
	PV = pearl value
	DV = diamond value
	PRV = precious stone value

	LC = labour cost
	COS = Costing for kundan
	*/

	if(formula.IsEmpty())
		return val;

	formula.Remove(' ');

	ReplaceStringWithValue(&formula, "GW" , item->GetGrossWeight());
	ReplaceStringWithValue(&formula, "NW" , item->GetNetWeight());
	ReplaceStringWithValue(&formula, "PW" , item->GetSubItemWeight(tPearl));
	ReplaceStringWithValue(&formula, "DW" , item->GetSubItemWeight(tDiamond));
	ReplaceStringWithValue(&formula, "PRW" , item->GetSubItemWeight(tStone));
	ReplaceStringWithValue(&formula, "OW" , item->GetOtherWeight());

	ReplaceStringWithValue(&formula, "GVG" , item->GetGoldValue(FALSE, FALSE, gold_rate, 1.0));
	ReplaceStringWithValue(&formula, "GVGW" , item->GetGoldValue(FALSE, TRUE, gold_rate, 1.0));
	ReplaceStringWithValue(&formula, "GVN" , item->GetGoldValue(TRUE, FALSE, gold_rate, 1.0));
	ReplaceStringWithValue(&formula, "GVNW" , item->GetGoldValue(TRUE, TRUE, gold_rate, 1.0));
	ReplaceStringWithValue(&formula, "PV" , item->GetSubItemValue(tPearl, 1.0));
	ReplaceStringWithValue(&formula, "DV" , item->GetSubItemValue(tDiamond, 1.0));
	ReplaceStringWithValue(&formula, "PRV" , item->GetSubItemValue(tStone, 1.0));
	ReplaceStringWithValue(&formula, "LC" , item->GetLabourCost(1.0));
	ReplaceStringWithValue(&formula, "COS", item->GetCosting(1.0));

	if(formula.IsEmpty())
		return val;

	if(formula[0] == '=')
	{
		formula = formula.Right(formula.GetLength() - 1);
	}

	if(DoParenthesis(&formula))
		val = DoCalculation(formula);
	else
	{
		CString errstr = "Could not properly parenthesis the formula '" + formula + "' of item '" + CString(item->m_product_ID) + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return val;
	}

	val = val / conversion;
	return val;
}

void ReplaceDollarWithValue(CString *formula, double value)
{
	if(!formula)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	int pos = formula->Find("$");
	if(pos == -1)
		return;

	int remaining = formula->GetLength() - pos - 1;

	CString newstr;
	newstr.Format("%.3f",value);

	newstr = formula->Left(pos) + newstr + formula->Right(remaining);
	*formula = newstr;
}

static void ReplaceStringWithValue(CString *formula, CString string, double value)
{
	if(!formula)
	{
		SHOW_INTERNAL_ERROR;
		return;
	}

	int pos = formula->Find(string);
	if(pos == -1)
		return;

	int remaining = formula->GetLength() - string.GetLength() - pos;

	CString newstr;
	newstr.Format("%.3f",value);

	newstr = formula->Left(pos) + newstr + formula->Right(remaining);
	*formula = newstr;
}

static BOOL DoParenthesis(CString *formula)
{
	if(!formula)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	if(!DoParenthesisFor(formula,"/"))
	{
		CString errstr = "Could not parenthesis the formula for '/' :\n" + *formula;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!DoParenthesisFor(formula,"*"))
	{
		CString errstr = "Could not parenthesis the formula for '*' :\n" + *formula;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!DoParenthesisFor(formula,"+"))
	{
		CString errstr = "Could not parenthesis the formula for '+' :\n" + *formula;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	if(!DoParenthesisFor(formula,"-"))
	{
		CString errstr = "Could not parenthesis the formula for '-' :\n" + *formula;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}

static BOOL DoParenthesisFor(CString *formula, CString exp)
{
	if(!formula)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	int pos = 0;
	while(pos != -1)
	{
		pos = formula->Find(exp, pos);
		if(pos == -1)
			break;

		CString forwardstr = *formula;
		forwardstr = forwardstr.Right(forwardstr.GetLength() - pos -1);

		CString backwardstr = *formula;
		backwardstr = backwardstr.Left(pos);

		{//Looking backward to place the opening brace
			backwardstr.MakeReverse();

			int brace_pos = backwardstr.Find(")");
			int oper_pos = backwardstr.FindOneOf("/*+-");

			if((brace_pos == -1) && (oper_pos == -1))
				backwardstr = backwardstr + "(";
			else if(brace_pos == -1)
				backwardstr = backwardstr.Left(oper_pos) + "(" + backwardstr.Right(backwardstr.GetLength() - oper_pos);
			else if((oper_pos == -1) || (brace_pos < oper_pos))
			{
				brace_pos++;
				int depth = 1;
				while((brace_pos != backwardstr.GetLength()) && (depth != 0))
				{
					if(backwardstr[brace_pos] == ')')
						depth++;
					else if(backwardstr[brace_pos] == '(')
						depth--;

					brace_pos++;
				}

				if((brace_pos == backwardstr.GetLength()) && (depth != 0))
				{
					CString errstr = "Unmatched brackets existing in the formula :\n" + *formula;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return FALSE;
				}

				backwardstr = backwardstr.Left(brace_pos) + "(" + backwardstr.Right(backwardstr.GetLength() - brace_pos);
			}
			else
				backwardstr = backwardstr.Left(oper_pos) + "(" + backwardstr.Right(backwardstr.GetLength() - oper_pos);

			backwardstr.MakeReverse();
		}

		{//Looking forward to place the closing brace
			int brace_pos = forwardstr.Find("(");
			int oper_pos = forwardstr.FindOneOf("/*+-");

			if((brace_pos == -1) && (oper_pos == -1))
				forwardstr = forwardstr + ")";
			else if(brace_pos == -1)
				forwardstr = forwardstr.Left(oper_pos) + ")" + forwardstr.Right(forwardstr.GetLength() - oper_pos);
			else if((oper_pos == -1) || (brace_pos < oper_pos))
			{
				brace_pos++;
				int depth = 1;
				while((brace_pos != forwardstr.GetLength()) && (depth != 0))
				{
					if(forwardstr[brace_pos] == ')')
						depth--;
					else if(forwardstr[brace_pos] == '(')
						depth++;

					brace_pos++;
				}

				if((brace_pos == forwardstr.GetLength()) && (depth != 0))
				{
					CString errstr = "Unmatched brackets existing in the formula :\n" + *formula;
					SHOW_INTERNAL_ERROR_REASON(errstr);
					return FALSE;
				}

				forwardstr = forwardstr.Left(brace_pos) + ")" + forwardstr.Right(forwardstr.GetLength() - brace_pos);
			}
			else
				forwardstr = forwardstr.Left(oper_pos) + ")" + forwardstr.Right(forwardstr.GetLength() - oper_pos);
		}

		*formula = backwardstr + exp + forwardstr;
		pos += 2; //move forward 2 to cover up the exp and the newly added bracket.
	}

	return TRUE;
}

static double DoCalculation(CString formula)
{
	double val = 0.0;
	int brace_pos = formula.Find("(");
	if(brace_pos == -1)
	{
		int oper_pos = formula.FindOneOf("/*+-");
		if(oper_pos == -1)
		{
			val = atof(LPCTSTR(formula));
			return val;
		}
		else
		{
			char operation = formula[oper_pos];
			double leftval, rightval;

			leftval = atof(LPCTSTR(formula.Left(oper_pos)));
			rightval = atof(LPCTSTR(formula.Right(formula.GetLength() - oper_pos - 1)));

			if(operation == '/')
				val = leftval/rightval;
			else if(operation == '*')
				val = leftval*rightval;
			else if(operation == '+')
				val = leftval+rightval;
			else if(operation == '-')
				val = leftval-rightval;

			return val;
		}
	}
	else
	{
		formula = formula.Left(formula.GetLength() - 1);
		formula = formula.Right(formula.GetLength() - 1);

		int pos = 0;
		int depth = 0;
		BOOL processed = FALSE;

		while(pos != formula.GetLength())
		{
			if(formula[pos] == '(')
				depth++;
			else if(formula[pos] == ')')
				depth--;
			else if(formula[pos] == '/')
			{
				if(depth == 0)
				{
					CString leftexp, rightexp;
					leftexp = formula.Left(pos);
					rightexp = formula.Right(formula.GetLength() - pos - 1);
					val = DoCalculation(leftexp)/DoCalculation(rightexp);
					processed = TRUE;
					break;
				}
			}
			else if(formula[pos] == '*')
			{
				if(depth == 0)
				{
					CString leftexp, rightexp;
					leftexp = formula.Left(pos);
					rightexp = formula.Right(formula.GetLength() - pos - 1);
					val = DoCalculation(leftexp)*DoCalculation(rightexp);
					processed = TRUE;
					break;
				}
			}
			else if(formula[pos] == '+')
			{
				if(depth == 0)
				{
					CString leftexp, rightexp;
					leftexp = formula.Left(pos);
					rightexp = formula.Right(formula.GetLength() - pos - 1);
					val = DoCalculation(leftexp)+DoCalculation(rightexp);
					processed = TRUE;
					break;
				}
			}
			else if(formula[pos] == '-')
			{
				if(depth == 0)
				{
					CString leftexp, rightexp;
					leftexp = formula.Left(pos);
					rightexp = formula.Right(formula.GetLength() - pos - 1);
					val = DoCalculation(leftexp)-DoCalculation(rightexp);
					processed = TRUE;
					break;
				}
			}

			pos++;
		}

		if(!processed && formula[0] == '(' && formula[formula.GetLength() - 1] == ')')
		{
			val = DoCalculation(formula);
		}

		return val;
	}
}

BOOL SaveImage(CString *filename, BYTE *data, DWORD size, BOOL overwrite)
{
	if(!filename || filename->IsEmpty() || !data || (size == 0))
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CString imagefolder = gFilePath + "\\images";
	if(!CreateDirectory(imagefolder, NULL))
	{
		DWORD error = GetLastError();
		if(error != ERROR_ALREADY_EXISTS)
		{
			CString errstr = "Could not create the images directory '" + imagefolder + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	CString finalname = GetBaseName(*filename);

	if(!overwrite)
	{
		CString name, extension;
		int index = finalname.ReverseFind('.');
		if(index != -1)
		{
			name = finalname.Left(index);
			extension = finalname.Right(finalname.GetLength() - index);
		}
		else
		{
			name = finalname;
			extension = ".jpg";
		}

		finalname = name + extension;
		CString path = imagefolder + "\\" + finalname;
		if(IsFileExisting(path))
		{
			int i = 1;
			while(TRUE)
			{
				CString temp_nam;
				temp_nam.Format("_%d", i);
				temp_nam = imagefolder + "\\" + name + temp_nam + extension;

				if(!IsFileExisting(temp_nam))
				{
					CString addition;
					addition.Format("_%d", i);
					finalname = name + addition + extension;
					break;
				}

				i++;
			}
		}
	}

	CString fullpath = imagefolder + "\\" + finalname;
	CString tempfilename = imagefolder + "\\tmp_image.tmp";

	CFile outfile;
	CFileException e;
	if(!outfile.Open(LPCTSTR(tempfilename), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, &e))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", e.m_cause);
		errstr = "Could not open temporary image file '" + tempfilename + "' while trying to save file '" + finalname + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	try
	{
		outfile.Write(data, size);
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", readerror->m_cause);
		errstr = "Could not write to temporary image file '" + tempfilename + "' while trying to save file '" + finalname + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		outfile.Close();
		return FALSE;
	}

	outfile.Close();

	if(!MoveDatabaseFile(tempfilename, fullpath))
	{
		CString errstr = "Could not move file '" + tempfilename + "' to '" + fullpath + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	*filename = GetBaseName(finalname);
	return TRUE;
}

BOOL OpenImage(CString filename, BYTE **data, DWORD *size)
{
	if(filename.IsEmpty() || !data || !size)
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CString imagefolder = gFilePath + "\\images";
	if(!CreateDirectory(imagefolder, NULL))
	{
		DWORD error = GetLastError();
		if(error != ERROR_ALREADY_EXISTS)
		{
			CString errstr = "Could not create the images directory '" + imagefolder + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	filename = GetBaseName(filename);

	CString finalfile = imagefolder + "\\" + filename;
	CFile infile;
	CFileException e;
	if(!infile.Open(LPCTSTR(finalfile), CFile::modeRead | CFile::typeBinary, &e))
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", e.m_cause);
		errstr = "Could not open image file '" + finalfile + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	*size = infile.GetLength();
	SALIL_NEW((*data), BYTE[(*size)]);
	DWORD readcount;

	try
	{
		readcount = infile.Read((*data),(*size));
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", readerror->m_cause);
		errstr = "Could not read from image file '" + finalfile + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		delete (*data);
		infile.Close();
		return FALSE;
	}

	if(readcount < (*size))
	{
		CString errstr = "Could not read complete data from the image file '" + finalfile + "'.";
		SHOW_INTERNAL_ERROR_REASON(errstr);
		delete (*data);
		infile.Close();
		return FALSE;
	}

	infile.Close();
	return TRUE;
}

BOOL DeleteImage(CString filename)
{
	if(filename.IsEmpty())
	{
		SHOW_INTERNAL_ERROR;
		return FALSE;
	}

	CString imagefolder = gFilePath + "\\images";
	if(!CreateDirectory(imagefolder, NULL))
	{
		DWORD error = GetLastError();
		if(error != ERROR_ALREADY_EXISTS)
		{
			CString errstr = "Could not create the images directory '" + imagefolder + "'.";
			SHOW_INTERNAL_ERROR_REASON(errstr);
			return FALSE;
		}
	}

	filename = GetBaseName(filename);
	CString finalfile = imagefolder + "\\" + filename;

	try
	{
		CFile::Remove(LPCTSTR(finalfile));
	}
	catch(CFileException *readerror)
	{
		CString errstr;
		errstr.Format("ERROR : #%d.", readerror->m_cause);
		errstr = "Could not delete file '" + finalfile + "'.\n" + errstr;
		SHOW_INTERNAL_ERROR_REASON(errstr);
		return FALSE;
	}

	return TRUE;
}

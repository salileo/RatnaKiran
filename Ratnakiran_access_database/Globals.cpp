#include "stdafx.h"
#include "Ratnakiran.h"
#include "globals.h"
#include <math.h>

CRatnakiranStockView *gStockView;
CRatnakiranTreeView *gTreeView;
CMainFrame *gFrame;
CRatnakiranDoc *gDoc;
CTableDescription gDesc;
CTableSettings gSettings;

void ReplaceStringWithValue(CString *formula, CString string, double value);
BOOL DoParenthesis(CString *formula);
BOOL DoParenthesisFor(CString *formula, CString exp);
double DoCalculation(CString formula);


void UnHandledError(CString file, int line)
{
	CString filename;
	int pos = file.ReverseFind('\\');
	if(pos == -1)
		filename = file;
	else
		filename = file.Right(file.GetLength() - pos - 1);

	CString lineno;
	lineno.Format("%d", line);

	CString errorstr = "Unexpected failure at ";
	errorstr = errorstr + file;
	errorstr = errorstr + ":";
	errorstr = errorstr + lineno;

	::MessageBox(NULL, errorstr, "Error", MB_OK);
}


int ShowErrorMessage(CString message, CList<CString, CString> *IDList, BOOL try_again)
{
	if(!IDList || IDList->IsEmpty())
		return IDCANCEL;

	CString final = "Could not " + message + " the following sets/items -\n";
	POSITION pos = IDList->GetHeadPosition();
	while(pos)
	{
		CString id = IDList->GetAt(pos);
		final = final + id + "\n";
		IDList->GetNext(pos);
	}

	if(try_again)
	{
		final = final + "\nDo you want to try again ?";
		int retval = MessageBox(NULL, final, "ERROR", MB_YESNO);
		return retval;
	}
	else
	{
		int retval = MessageBox(NULL, final, "ERROR", MB_OK);
		return retval;
	}
}


BOOL MakeGlobalsValid()
{
	if(!gFrame)
	{
		gFrame = (CMainFrame *)AfxGetMainWnd();
		if(!gFrame)
			return FALSE;
	}

	if(!gDoc)
	{
		gDoc = (CRatnakiranDoc *)gFrame->GetActiveDocument();
		if(!gDoc)
			return FALSE;
	}

	if(!gTreeView)
	{
		if(!gFrame->m_wndSplitter)
			return FALSE;

		gTreeView = STATIC_DOWNCAST(CRatnakiranTreeView, (gFrame->m_wndSplitter)->GetPane(0,0));
		if(!gTreeView)
			return FALSE;
	}

	if(!gStockView)
	{
		if(!gFrame->m_wndSplitter)
			return FALSE;

		gStockView = STATIC_DOWNCAST(CRatnakiranStockView, (gFrame->m_wndSplitter)->GetPane(0,1));
		if(!gStockView)
			return FALSE;
	}

	if(!gDoc->m_vouchers.IsOpen())
		return FALSE;

	if(!gDoc->m_vouchers.m_sets.IsOpen())
		return FALSE;

	if(!gDoc->m_vouchers.m_sets.m_items.IsOpen())
		return FALSE;

	if(!gDoc->m_vouchers.m_sets.m_items.m_sub_items.IsOpen())
		return FALSE;

	if(!gDesc.IsOpen())
		return FALSE;

	if(!gSettings.IsOpen())
		return FALSE;

	return TRUE;
}


int FindMenuItem(CMenu* menu, CString menustring)
{
	if(menu == NULL)
		return -1;

   int count = menu->GetMenuItemCount();
   for (int i = 0; i < count; i++)
   {
      CString str;
      if(menu->GetMenuString(i, str, MF_BYPOSITION))
	  {
		  if(str == menustring)
			  return i;
	  }
   }

   return -1;
}


CString GetFileName(char *filtername, char *filtervalue, char *defaultextension, BOOL forOpen)
{
	OPENFILENAME OpenFileName;

	//buffer for storing the file name
	char szFile[MAX_PATH] = "\0";
    strcpy(szFile,"");

	char *filter = NULL;

	if(filtername && filtervalue)
	{
		//create a NULL character seperated strings for defining the filters
		int size = strlen(filtername) + strlen(filtervalue) + 3;
		SALIL_NEW(filter, char[size]);

		strcpy(&(filter[0]),filtername);
		filter[strlen(filtername)] = '\0';

		strcpy(&(filter[strlen(filtername) + 1]),filtervalue);
		filter[size - 2] = '\0';
		filter[size - 1] = '\0';
	}

	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = NULL;
    OpenFileName.hInstance         = NULL;
    OpenFileName.lpstrFilter       = filter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 1;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = MAX_PATH;
    OpenFileName.lpstrFileTitle    = NULL;
    OpenFileName.nMaxFileTitle     = 0;
    OpenFileName.lpstrInitialDir   = NULL;
	OpenFileName.lpstrTitle        = "Select File";
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = defaultextension;
    OpenFileName.lCustData         = NULL;
	OpenFileName.lpfnHook 		   = NULL;
	OpenFileName.lpTemplateName    = NULL;
    OpenFileName.Flags             = OFN_LONGNAMES | OFN_SHOWHELP | OFN_EXPLORER;
		

	CString filename = "";
	if(forOpen)
	{
		OpenFileName.Flags |=  OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetOpenFileName(&OpenFileName))
			filename = OpenFileName.lpstrFile;
	}
	else
	{
		OpenFileName.Flags |=  OFN_OVERWRITEPROMPT;

		if(GetSaveFileName(&OpenFileName))
			filename = OpenFileName.lpstrFile;
	}

	if(filter)
		delete filter;

	return filename;
}


double GramsFromCaret(double carets)
{
	return (carets / 5.0);
}


double CaretFromGrams(double grams)
{
	return (grams * 5.0);
}


CString GetProductID()
{
	static BOOL firstrun = TRUE;
	if(firstrun)
	{
		firstrun = FALSE;
		srand((unsigned)time(NULL));
	}

	int number = rand()%1000;
	int firstchar = (rand()%26) + 65;

	CString id;
	id.Format("%c%03d",firstchar, number);
	return id;
}


double ProcessFormula(CString formula, double gold_rate, double conversion, CTableItem *item)
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
	*/

	if(formula.IsEmpty())
		return val;

	formula.Remove(' ');

	ReplaceStringWithValue(&formula, "GW" , item->GetGrossWeight());
	ReplaceStringWithValue(&formula, "NW" , item->GetNetWeight());
	ReplaceStringWithValue(&formula, "PW" , item->GetSubItemWeight(PEARL));
	ReplaceStringWithValue(&formula, "DW" , item->GetSubItemWeight(DIAMOND));
	ReplaceStringWithValue(&formula, "PRW" , item->GetSubItemWeight(STONE));
	ReplaceStringWithValue(&formula, "OW" , item->GetOtherWeight());

	ReplaceStringWithValue(&formula, "GVG" , ROUNDUP(item->GetGoldValueDouble(FALSE, FALSE, gold_rate, 1.0)));
	ReplaceStringWithValue(&formula, "GVGW" , ROUNDUP(item->GetGoldValueDouble(FALSE, TRUE, gold_rate, 1.0)));
	ReplaceStringWithValue(&formula, "GVN" , ROUNDUP(item->GetGoldValueDouble(TRUE, FALSE, gold_rate, 1.0)));
	ReplaceStringWithValue(&formula, "GVNW" , ROUNDUP(item->GetGoldValueDouble(TRUE, TRUE, gold_rate, 1.0)));
	ReplaceStringWithValue(&formula, "PV" , ROUNDUP(item->GetSubItemValueDouble(PEARL, 1.0)));
	ReplaceStringWithValue(&formula, "DV" , ROUNDUP(item->GetSubItemValueDouble(DIAMOND, 1.0)));
	ReplaceStringWithValue(&formula, "PRV" , ROUNDUP(item->GetSubItemValueDouble(STONE, 1.0)));
	ReplaceStringWithValue(&formula, "LC" , ROUNDUP(item->GetLabourCostDouble(1.0)));

	if(formula.IsEmpty())
		return val;

	if(formula[0] == '=')
	{
		formula = formula.Right(formula.GetLength() - 1);
	}

	if(DoParenthesis(&formula))
		val = DoCalculation(formula);

	val = val / conversion;
	return val;
}


void ReplaceDollarWithValue(CString *formula, double value)
{
	if(!formula)
		return;

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
		return;

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
	if(!DoParenthesisFor(formula,"/"))
		return FALSE;

	if(!DoParenthesisFor(formula,"*"))
		return FALSE;

	if(!DoParenthesisFor(formula,"+"))
		return FALSE;

	if(!DoParenthesisFor(formula,"-"))
		return FALSE;

	return TRUE;
}


static BOOL DoParenthesisFor(CString *formula, CString exp)
{
	if(!formula)
		return FALSE;

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
					return FALSE;

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
					return FALSE;

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


BOOL IsNumber(CString str)
{
	str.Remove('0');
	str.Remove('1');
	str.Remove('2');
	str.Remove('3');
	str.Remove('4');
	str.Remove('5');
	str.Remove('6');
	str.Remove('7');
	str.Remove('8');
	str.Remove('9');

	if(str.GetLength() > 0)
		return FALSE;
	else
		return TRUE;
}


double RoundOff(double val, int decimal_places)
{
	int tempval = (int)((val * pow(10.0, (double)decimal_places)) + 0.5);
	val = ((double)tempval)/ pow(10.0, (double)decimal_places);
	return val;
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

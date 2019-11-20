#if !defined(AFX_SETTINGS_H__FD82E213_1994_4C1C_A172_EDF7F59CD4BF__INCLUDED_)
#define AFX_SETTINGS_H__FD82E213_1994_4C1C_A172_EDF7F59CD4BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef BOOL (*EnumCategoriesFunc)(CString description, CString prefix, int number_of_pieces, void *clientData);

class CSettings  
{
public:
	CSettings();
	virtual ~CSettings();

	static BOOL AddCategory(CString description, CString prefix, int number_of_pieces);
	static BOOL RemoveCategory(CString description);
	static BOOL RemoveAllCategories();
	static int GetCategoryCount();
	static BOOL GetCategory(int index, CString *description, CString *prefix, int *piece_count);
	static int GetPieceCountFor(CString description);
	static CString GetPrefixFor(CString description);

	static double GetGoldRate();
	static BOOL SetGoldRate(double val);

	static BOOL GetUseNativeCurrency();
	static BOOL SetUseNativeCurrency(BOOL val);

private:
	struct CDescriptionName
	{
		CDescriptionName()
		{
			m_description = "";
			m_prefix = "";
			m_number_of_pieces = 0;
		}

		CString m_description;
		CString m_prefix;
		int m_number_of_pieces;
	};

	CList<CDescriptionName, CDescriptionName> m_Categories;
	CString m_CategoriesString;
	double m_GoldRate;
	BOOL m_UseNativeCurrency;
	BOOL m_SettingInitialized;

	BOOL WriteSettings();
	BOOL ReadSettings();

	void SetDefaultCategories();
	void ConvertListToString();
	void ConvertStringToList();
};

#endif // !defined(AFX_SETTINGS_H__FD82E213_1994_4C1C_A172_EDF7F59CD4BF__INCLUDED_)

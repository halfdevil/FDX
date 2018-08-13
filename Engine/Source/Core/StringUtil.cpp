
#include "Core/StringUtil.h"

namespace FDX
{
	void CStringUtil::LTrim(String & pstrString)
	{
		pstrString.erase(pstrString.begin(), 
			std::find_if(pstrString.begin(), pstrString.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	void CStringUtil::RTrim(String & pstrString)
	{
		pstrString.erase(
			std::find_if(pstrString.rbegin(), pstrString.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), pstrString.end());
	}

	void CStringUtil::Trim(String & pstrString)
	{
		LTrim(pstrString);
		RTrim(pstrString);
	}
}
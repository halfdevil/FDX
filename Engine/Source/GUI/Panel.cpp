
#include "GUI/Panel.h"

namespace FDX
{
	CPanel::CPanel(const String & pstrName)
		: CControl(pstrName)
	{
	}

	CPanel::~CPanel()
	{
	}

	void CPanel::PreDraw(CCanvas * pCanvas)
	{
		CControl::PreDraw(pCanvas);
	}

	void CPanel::PostDraw(CCanvas * pCanvas)
	{
		CControl::PostDraw(pCanvas);
	}
}
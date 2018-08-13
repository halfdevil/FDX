#pragma once

#include "Game/Win32GameFramework.h"
#include "Core/Font.h"
#include "GUI/Button.h"
#include "GUI/Panel.h"
#include "GUI/Dialog.h"

using namespace FDX;

class CCanvasDialog : public CDialog
{
public:

	CCanvasDialog(const String &pstrName);

protected:

	EResult OnInitialize(CControlContainer *pContainer);
	EResult CreateControls(CControlContainer *pContainer);
};

class CCanvasSample : public CWin32GameFramework
{
public:

	EResult OnLoadContent() override;
	EResult OnUpdate() override;
	EResult OnDraw() override;

private:

	void CreateGUI();
	void CreateCursor();

private:

	IFont * mFont;
	CTexture2D *mMouseTex;
	CDialog *mDialog;
};
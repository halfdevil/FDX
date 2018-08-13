
#include "CanvasSample.h"
#include "Core/FontResource.h"
#include "GUI/Panel.h"
#include "GUI/DefaultUITheme.h"
#include "GUI/RadioButton.h"
#include "GUI/RadioButtonGroup.h"
#include "GUI/CheckBox.h"
#include "GUI/TextBox.h"
#include <Windows.h>

CCanvasDialog::CCanvasDialog(const String & pstrName)
	: CDialog(pstrName)
{
}

EResult CCanvasDialog::OnInitialize(CControlContainer * pContainer)
{
	EResult lResult = EResult::Success;

	lResult = CDialog::OnInitialize(pContainer);
	if (lResult != EResult::Success)
	{
		LOG_DEBUG("CDialog::OnInitialize() failed");
	}

	return lResult;
}

EResult CCanvasDialog::CreateControls(CControlContainer * pContainer)
{
	EResult lResult = EResult::Success;

	lResult = CDialog::CreateControls(pContainer);
	if (lResult != EResult::Success)
	{
		LOG_DEBUG("CDialog::CreateControls() failed");
	}
	else
	{
		CButton *lLeftButton = new CButton(mstrName + "left_button");
		lLeftButton->SetPosition(4, 4);
		lLeftButton->SetSize(50, 28);
		lLeftButton->SetText("ABCD");

		CControlSkin *lLeftSkin = new CControlSkin(mstrName + "left_button_skin");
		lLeftSkin->AddMaterial(IUITheme::Get().GetButtonMaterial(EControlMaterialType::Default));
		lLeftSkin->AddMaterial(IUITheme::Get().GetButtonMaterial(EControlMaterialType::Hover));
		lLeftSkin->AddMaterial(IUITheme::Get().GetButtonMaterial(EControlMaterialType::Pressed));
		lLeftSkin->SetActiveMaterial(EControlMaterialType::Default);

		lLeftButton->SetSkin(CControlSkinPtr(lLeftSkin));
		pContainer->AddControl(CControlPtr(lLeftButton), mContent);

		CRadioButton *lRadioButton1 = new CRadioButton(mstrName + "_radio");
		lRadioButton1->SetPosition(4, 4);
		lRadioButton1->SetSize(100, 28);
		lRadioButton1->SetText("Radio Button 1");
		lRadioButton1->Create(pContainer);

		CRadioButton *lRadioButton2 = new CRadioButton(mstrName + "_radio");
		lRadioButton2->SetPosition(4, 36);
		lRadioButton2->SetSize(100, 28);
		lRadioButton2->SetText("Radio Button 2");
		lRadioButton2->Create(pContainer);

		CRadioButton *lRadioButton3 = new CRadioButton(mstrName + "_radio");
		lRadioButton3->SetPosition(4, 68);
		lRadioButton3->SetSize(100, 28);
		lRadioButton3->SetText("Radio Button 3");
		lRadioButton3->Create(pContainer);

		CRadioButtonGroup *lRadioButtonGroup = new CRadioButtonGroup(mstrName + "radio_group");
		lRadioButtonGroup->SetPosition(56, 4);
		lRadioButtonGroup->SetSize(108, 100);
		lRadioButtonGroup->Create(pContainer);
		lRadioButtonGroup->AddButton(lRadioButton1);
		lRadioButtonGroup->AddButton(lRadioButton2);
		lRadioButtonGroup->AddButton(lRadioButton3);

		CCheckBox *lCheckBox = new CCheckBox(mstrName + "_checkbox");
		lCheckBox->SetPosition(166, 4);
		lCheckBox->SetSize(100, 28);
		lCheckBox->SetText("Check Box");
		lCheckBox->Create(pContainer);

		CTextBox *lTextBox = new CTextBox(mstrName + "_textbox");
		lTextBox->SetPosition(268, 4);
		lTextBox->SetSize(100, 28);
		lTextBox->Create(pContainer);

		CButton *lBigButton = new CButton(mstrName + "left_button");
		lBigButton->SetPosition(370, 4);
		lBigButton->SetSize(200, 200);
		lBigButton->SetText("ABCD");

		CControlSkin *lBigSkin = new CControlSkin(mstrName + "big_button_skin");
		lBigSkin->AddMaterial(IUITheme::Get().GetButtonMaterial(EControlMaterialType::Default));
		lBigSkin->AddMaterial(IUITheme::Get().GetButtonMaterial(EControlMaterialType::Hover));
		lBigSkin->AddMaterial(IUITheme::Get().GetButtonMaterial(EControlMaterialType::Pressed));
		lBigSkin->GetMaterial(EControlMaterialType::Default)->GetFont()->SetFontSize(72);
		lBigSkin->GetMaterial(EControlMaterialType::Hover)->GetFont()->SetFontSize(72);
		lBigSkin->GetMaterial(EControlMaterialType::Pressed)->GetFont()->SetFontSize(72);
		lBigSkin->SetActiveMaterial(EControlMaterialType::Default);
		lBigButton->SetSkin(CControlSkinPtr(lBigSkin));

		pContainer->AddControl(CControlPtr(lRadioButtonGroup), mContent);
		pContainer->AddControl(CControlPtr(lCheckBox), mContent);
		pContainer->AddControl(CControlPtr(lTextBox), mContent);
		pContainer->AddControl(CControlPtr(lRadioButton1), lRadioButtonGroup);
		pContainer->AddControl(CControlPtr(lRadioButton2), lRadioButtonGroup);
		pContainer->AddControl(CControlPtr(lRadioButton3), lRadioButtonGroup);
		pContainer->AddControl(CControlPtr(lBigButton), mContent);
	}

	return lResult;
}

EResult CCanvasSample::OnLoadContent()
{
	EResult lResult = EResult::Success;
	CFontResource *lFontResource = nullptr;
	IResource *lControlResource = nullptr;

	lResult = GetResourceManager()->LoadResource(EResourceType::Font,
		"/Assets/Fonts/consola.ttf", &lFontResource);

	if (lResult != EResult::Success)
	{
		LOG_ERROR("Unable to load font: arial.ttf");
	}
	else
	{
		GetUIManager()->SetTheme(std::make_unique<CDefaultUITheme>());
		lResult = GetResourceManager()->LoadResource(EResourceType::Xml,
			"/Assets/GUI/main.fvc", &lControlResource);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to load view: main.fvc");
		}
		else
		{
			mFont = lFontResource->GetFont();
			mFont->SetMaskMode(false);

			CreateGUI();
			CreateCursor();

			const auto &lWindowParams = GetWindow()->GetParameters();
			CMatrix4F lProjMatrix = CMatrix4F::OrthogonalMatrix(0, 0,
				(float)lWindowParams.GetWidth(), (float)lWindowParams.GetHeight(),
				-1.0f, 1.0f);

			GetCanvas()->SetProjMatrix(lProjMatrix);
			GetCanvas()->SetWorldMatrix(CMatrix4F::IdentityMatrix());
		}
	}

	return lResult;
}

EResult CCanvasSample::OnUpdate()
{
	if (GetKeyboard()->KeyDown(EKeyboardKey::Escape))
		Exit();

	GetUIManager()->Update((float)GetTimer()->GetDeltaTime());

	return EResult::Success;
}

EResult CCanvasSample::OnDraw()
{
	EResult lResult = EResult::Success;

	GetGraphicsDevice()->Clear(EClearOptions::Color | EClearOptions::Depth,
		CColor4F(0.0f, 0.5f, 0.5f), 1.0f, 0);

	GetCanvas()->Begin();
	GetUIManager()->Draw(GetCanvas());
	GetCanvas()->End();

	return lResult;
}

void CCanvasSample::CreateGUI()
{
	const auto &lWindowParams = GetWindow()->GetParameters();
	CControlContainer *lContainer = GetUIManager()->
		CreateContainer("main");

	CControl *lLayout = lContainer->CreateLayout(EControlLayoutType::Default, "main_layout");
	lLayout->SetPosition(1, 1);
	lLayout->SetSize(1020, 600);

	CCanvasDialog *lDialog = new CCanvasDialog("canvas");
	lDialog->SetPosition(10, 10);
	lDialog->SetSize(640, 480);
	lDialog->Create(lContainer);

	CCanvasDialog *lSecondDialog = new CCanvasDialog("second_canvas");
	lSecondDialog->SetPosition(100, 100);
	lSecondDialog->SetSize(640, 480);
	lSecondDialog->Create(lContainer);

	lContainer->AddControl(CControlPtr(lDialog), nullptr);
	lContainer->AddControl(CControlPtr(lSecondDialog), nullptr);
	lContainer->LayoutControls();

	GetUIManager()->SetActiveContainer(0);
}

void CCanvasSample::CreateCursor()
{
	CCursor *lCursor = GetUIManager()->CreateCursor("Cursor");
	CControlSkinPtr lSkin = std::make_unique<CControlSkin>("Cursor");
	CControlMaterialPtr lMaterial = std::make_unique<CControlMaterial>(EControlMaterialType::Default, "Default");

	CTextureProperties lTexProps;
	lTexProps.SetWrapS(ETextureWrap::Repeat);
	lTexProps.SetWrapT(ETextureWrap::Repeat);
	lTexProps.SetFilter(ETextureFilter::Linear);

	CTexture2D *lArrowTexture = nullptr;
	CTextureManager::Get().AddTexture2D("MouseArrow", "/Assets/Textures/icons8-cursor-24.png",
		lTexProps, &lArrowTexture);

	CTexture2D *lHandTexture = nullptr;
	CTextureManager::Get().AddTexture2D("MouseHand", "/Assets/Textures/icons8-hand-cursor-24.png",
		lTexProps, &lHandTexture);

	lMaterial->AddTexture(lArrowTexture);
	lMaterial->AddTexture(nullptr);
	lMaterial->AddTexture(lHandTexture);

	lSkin->AddMaterial(std::move(lMaterial));
	lSkin->SetActiveMaterial(EControlMaterialType::Default);

	lCursor->SetSkin(std::move(lSkin));
	lCursor->SetSize(24, 24);
	lCursor->SetCursorType(ECursorType::Arrow);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
	EResult lResult;
	CCanvasSample lSample;

	lResult = lSample.Initialize();
	if (lResult != EResult::Success)
	{
		return -1;
	}
	else
	{
		lSample.Run();
	}

	return 0;
}
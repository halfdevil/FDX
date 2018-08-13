
#include "ModelViewer.h"
#include "Core/ModelResource.h"
#include <Windows.h>

EResult CModelViewer::OnLoadContent()
{
	EResult lResult = EResult::Success;

	mSceneTraverser = std::make_unique<CBasicSceneTraverser>();
	mRenderQueue = std::make_unique<CRenderQueue>();

	lResult = mRenderQueue->Create(GetGraphicsDevice());
	if (lResult != EResult::Success)
	{
		LOG_ERROR("Render Queue create failed");
	}
	else
	{
		const auto &lWindowParams = GetWindow()->GetParameters();

		mFreeCamera = std::make_unique<CFreeCamera>();
		mFreeCamera->SetPosition(CVector3F(0.0f, 30.0f, -100.0f));
		mFreeCamera->Rotate(ToRadians(0.0f), 0.0f, 0.0f);
		mFreeCamera->SetProjection(ToRadians(45.0f), (float)lWindowParams.GetWidth() /
			(float)lWindowParams.GetHeight(), 0.5f, 20000.0f);

		mViewport = {
			0, 0,
			lWindowParams.GetWidth(),
			lWindowParams.GetHeight()
		};

		mFreeCamera->Update();
		mRenderQueue->SetCamera(mFreeCamera.get());
		GetGraphicsDevice()->SetViewport(mViewport);
		
		mGround = LoadModel("/Assets/Models/Ground/ground.obj");
		mGround->SetScale(CVector3F(20.0f));
		mSceneTraverser->Add(mGround.get());

		mDude = LoadModel("/Assets/Models/Dude/dude.dae");
		mDude->SetScale(CVector3F(2.0f));
		mDude->SetRotation(CQuaternion::FromEulers(ToRadians(180.0f), 0.0f, 0.0f));
		mDude->SetAnimate(true);
		mDude->GetAnimationPlayer()->StartClipNumber(0);
		mSceneTraverser->Add(mDude.get());

		mPointLight1 = std::make_unique<CPointLight>();
		mPointLight1->SetColor(CColor4F(1.0f, 1.0f, 1.0f));
		mPointLight1->SetPosition(CVector3F(0.0f, 100.0f, -10.0f));
		mPointLight1->SetConstantAtt(1.0f);
		mPointLight1->SetLinearAtt(0.5f);
		mPointLight1->SetQuadraticAtt(0.2f);
		
		auto lLightShadowMap = std::make_unique<CShadowMap>(mPointLight1.get());
		lLightShadowMap->Create(GetGraphicsDevice(),
			lWindowParams.GetWidth(), lWindowParams.GetHeight());
		lLightShadowMap->SetTextureUnit(7);

		auto lLightCamera = std::make_unique<CCamera>();
		lLightCamera->SetProjection(ToRadians(50.0f), 1.0f, 1.0f, 2000.0f);
		lLightCamera->SetView(mPointLight1->GetPosition(), {}, CVector3F(0.0f, 1.0f, 0.0f));

		mPointLight1->SetCamera(std::move(lLightCamera));
		//mPointLight1->SetShadowMap(std::move(lLightShadowMap));

		mRenderQueue->AddLight(mPointLight1.get());

		mSkyBox = std::make_unique<CSkyBoxEntity>();
		lResult = mSkyBox->Load({
			"/Assets/Textures/skybox/ocean/posx.png",
			"/Assets/Textures/skybox/ocean/negx.png",
			"/Assets/Textures/skybox/ocean/posy.png",
			"/Assets/Textures/skybox/ocean/negy.png",
			"/Assets/Textures/skybox/ocean/posz.png",
			"/Assets/Textures/skybox/ocean/negz.png"
		});

		if (lResult != EResult::Success)
		{
			LOG_ERROR("SkyBox::Load() failed");
		}
		else
		{
			mSkyBox->SetScale(10000.0f);

			/*mQuadPostProcessor = std::make_unique<CQuadPostProcessor>();
			mQuadPostProcessor->Create();

			GetProgramManager()->GetProgram("QuadPostProcessorProgram", 
				&mPostProcessorProgram);

			mQuadPostProcessor->SetTexture(mPointLight1->
				GetShadowMap()->GetTexture());*/
		}
	}

	return lResult;
}

EResult CModelViewer::OnUpdate()
{
	EResult lResult = EResult::Success;

	float lfYaw = 0.0f;
	float lfPitch = 0.0f;

	if (GetKeyboard()->KeyTriggered(EKeyboardKey::Escape))
		Exit();

	if (GetKeyboard()->KeyDown(EKeyboardKey::S))
		mFreeCamera->Walk(-1.0f);

	if (GetKeyboard()->KeyDown(EKeyboardKey::W))
		mFreeCamera->Walk(1.0f);

	if (GetKeyboard()->KeyDown(EKeyboardKey::A))
		mFreeCamera->Strafe(-1.0f);

	if (GetKeyboard()->KeyDown(EKeyboardKey::D))
		mFreeCamera->Strafe(1.0f);

	if (GetKeyboard()->KeyDown(EKeyboardKey::Left))
		lfYaw = -1.0f;

	if (GetKeyboard()->KeyDown(EKeyboardKey::Right))
		lfYaw = 1.0f;

	if (GetKeyboard()->KeyDown(EKeyboardKey::Up))
		lfPitch = 1.0f;

	if (GetKeyboard()->KeyDown(EKeyboardKey::Down))
		lfPitch = -1.0f;
	
	mFreeCamera->Rotate(ToRadians(lfYaw), ToRadians(lfPitch), 0.0f);
	mFreeCamera->Update();
	mDude->Update(); 

	CVector3F lCamPos = mFreeCamera->GetPosition();
	lCamPos.Y = 0.0f;

	mSkyBox->SetPosition(lCamPos);
	mSkyBox->Update();

	return lResult;
}

EResult CModelViewer::OnDraw()
{
	EResult lResult = EResult::Success;

	GetGraphicsDevice()->Clear(EClearOptions::Color | EClearOptions::Depth,
		CColor4F(0.0f, 0.5f, 0.5f), 1.0f, 0);

	mSceneTraverser->GetVisible(mFreeCamera->GetFrustum(), mRenderQueue.get());
	mSkyBox->Draw(mFreeCamera.get());

	mRenderQueue->Prepare();
	//mRenderQueue->RenderShadow(GetGraphicsDevice());

	GetGraphicsDevice()->SetViewport(mViewport);
	mRenderQueue->RenderLight(GetGraphicsDevice());
	mRenderQueue->Reset();

	//mQuadPostProcessor->Process(mPostProcessorProgram);	

	return lResult;
}

CMeshEntityPtr CModelViewer::LoadModel(const String & pstrModel)
{
	EResult lResult = EResult::Success;	
	CModelResource *lModel = nullptr;

	lResult = GetResourceManager()->LoadResource(EResourceType::Model,
		pstrModel, &lModel);

	if (lResult != EResult::Success)
	{
		LOG_ERROR("Unable to load model: %d", pstrModel.c_str());
	}
	else
	{
		auto lEntity = std::make_unique<CMeshEntity>();
		lResult = lEntity->Load(lModel->GetModel());

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Unable to load entity: %s", pstrModel.c_str());
			return nullptr;
		}
		else
		{
			return lEntity;
		}
	}

	return nullptr;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
	EResult lResult;
	CModelViewer lSample;

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

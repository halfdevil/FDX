
#include "Game/OSXGameFramework.h"
#include "OS/OSX/OSXSystem.h"
#include "OS/OSX/OSXFileSystem.h"
#include "GLFW/GLFWUtil.h"
#include "GLFW/GLFWGameWindow.h"
#include "GLFW/GLFWGraphicsContext.h"
#include "OS/OSX/OSXGameTimer.h"
#include "OGL/GLGraphicsDevice.h"
#include "Core/BinaryResourceFactory.h"
#include "Core/XmlResourceFactory.h"
#include "Core/ImageResourceFactory.h"
#include "Core/FontResourceFactory.h"
#include "Core/ModelResourceFactory.h"
#include "Core/FreeImageLoader.h"
#include "Core/FreeTypeLoader.h"
#include "Core/ObjModelLoader.h"
#include "Core/DaeModelLoader.h"
#include "Core/EzmModelLoader.h"
#include "GUI/XmlControlContainerLoader.h"
#include "GLFW/GLFWKeyboard.h"
#include "GLFW/GLFWMouse.h"

namespace FDX
{
	COSXGameFramework::COSXGameFramework()
		: mAsyncQueue(nullptr),
		mLogger(nullptr),
		mResourceManager(nullptr),
		mWindow(nullptr),
		mTimer(nullptr),
		mFileSystem(nullptr),
		mGraphicsDevice(nullptr),
		mProgramManager(nullptr),
		mTextureManager(nullptr),
		mVertexCacheManager(nullptr),
		mDebugRenderer(nullptr),
		mCanvas(nullptr),
		mSpriteBatch(nullptr),
		mKeyboard(nullptr),
		mMouse(nullptr),
		mInputHandler(nullptr),
		mUIManager(nullptr),
		mbExitGame(false)
	{
	}

	EResult COSXGameFramework::Initialize()
	{
		EResult lResult;
		
		lResult = InitializeSystem();
		if (lResult != EResult::Success)
			return lResult;

		lResult = InitializeLoggers();
		if (lResult != EResult::Success)
			return lResult;

		lResult = InitlalizeResourceManager();
		if (lResult != EResult::Success)
			return lResult;

		lResult = InitializeGraphics();
		if (lResult != EResult::Success)
			return lResult;

		lResult = InitializeInputs();
		if (lResult != EResult::Success)
			return lResult;

		lResult = InitializeResourceFactories();
		if (lResult != EResult::Success)
			return lResult;

		lResult = InitializeManagers();
		if (lResult != EResult::Success)
			return lResult;
		
		lResult = InitializeSpriteBatch();
		if (lResult != EResult::Success)
			return lResult;

		lResult = InitializeCanvas();
		if (lResult != EResult::Success)
			return lResult;

		lResult = InitializeDebugRenderer();
		if (lResult != EResult::Success)
			return lResult;

		lResult = InitializeControl();
		if (lResult != EResult::Success)
			return lResult;
				
		lResult = OnLoadContent();
		if (lResult != EResult::Success)
			return lResult;

		return lResult;
	}

	void COSXGameFramework::Update()
	{
		mAsyncQueue->DemultiplexEvents();
		mEventManager->ProcessEvents();

		mTimer->Update();
		mKeyboard->Update();
		mMouse->Update();
	}

	void COSXGameFramework::Frame()
	{	
		if (!mbExitGame)
		{
			OnUpdate();
			OnFixedUpdate();
			OnDraw();

			mGraphicsDevice->GetGraphicsContext()->SwapBuffers();
		}
	}

	void COSXGameFramework::Exit()
	{
		mbExitGame = true;
		OnUnloadContent();

		if (mWindow)
			mWindow->Close();
	}

	EResult COSXGameFramework::OnLoadContent()
	{
		return EResult::Success;
	}

	EResult COSXGameFramework::OnUnloadContent()
	{
		return EResult::Success;
	}

	EResult COSXGameFramework::OnUpdate()
	{
		return EResult::Success;
	}

	EResult COSXGameFramework::OnFixedUpdate()
	{
		return EResult::Success;
	}

	EResult COSXGameFramework::OnDraw()
	{
		return EResult::Success;
	}

	EResult COSXGameFramework::InitializeSystem()
	{
		EResult lResult;
		IGameTimer *lTimer;

		mSystem = std::make_unique<COSXSystem>();

		lResult = mSystem->CreateGameTimer(&lTimer);
		if (lResult != EResult::Success)
		{
			LOG_ERROR(_T("CreateGameTimer() failed"));
		}
		else
		{
			mTimer = IGameTimerPtr(lTimer);

			lResult = CGLFWUtil::Initialize();
			if (lResult != EResult::Success)
            {
                LOG_DEBUG(_T("CGLFWUtil::Initialize() failed"));
            }
		}

		return lResult;
	}

	EResult COSXGameFramework::InitializeLoggers()
	{
		EResult lResult;
		uint32_t luiLogLevel = 0;
		String lstrWorkingDirectory;

		lResult = mSystem->GetExecutableDirectory(lstrWorkingDirectory);
		if (lResult != EResult::Success)
			return lResult;

#ifdef FDX_DEBUG
		luiLogLevel = static_cast<uint32_t>(ELogLevel::Debug);
#else
		luiLogLevel = static_cast<uint32_t>(ELogLevel::Error);
#endif

		mLogger = std::make_unique<CGameLogger>(luiLogLevel);
		lResult = mLogger->Create(lstrWorkingDirectory, _T("GameFramework.log"));

		return lResult;
	}

	EResult COSXGameFramework::InitlalizeResourceManager()
	{
		EResult lResult;
		String lstrAssetsDirectory;
		String lstrWorkingDirectory;
		CDirectory *lAssetsDirectory;

		lResult = mSystem->GetExecutableDirectory(lstrWorkingDirectory);
		if (lResult != EResult::Success)
		{
			LOG_ERROR(_T("Unable to get current working directory"));			
		}
		else
		{
			mSystem->CombinePath(lstrWorkingDirectory, _T("Assets"), lstrAssetsDirectory);
			mFileSystem = std::make_unique<COSXFileSystem>();

			lResult = mFileSystem->OpenDirectory(EOpenMode::Open, _T("AssetsDirectory"),
				lstrAssetsDirectory, &lAssetsDirectory);

			if (lResult != EResult::Success)
			{
				LOG_ERROR(_T("Unable to open assets directory"));				
			}
			else
			{
				lResult = mFileSystem->AddMountPoint(_T("/Assets"), lAssetsDirectory);
				if (lResult != EResult::Success)
				{
					LOG_ERROR(_T("Unable to add assets directory as mount point"));
					SAFE_DELETE(lAssetsDirectory);
				}
				else
				{
					mAsyncQueue = std::make_unique<CAsyncQueue>();
					mResourceManager = std::make_unique<CResourceManager>(mAsyncQueue.get());
				}
			}
		}

		return lResult;
	}

	EResult COSXGameFramework::InitializeInputs()
	{
		EResult lResult = EResult::Success;

		mKeyboard = std::make_unique<CGLFWKeyboard>(dynamic_cast<CGLFWGameWindow*>(mWindow.get()));
		mMouse = std::make_unique<CGLFWMouse>(dynamic_cast<CGLFWGameWindow*>(mWindow.get()));
        mMouse->SetMouseSensivity(1.0f);
        mMouse->SetDoubleClickLimit(0.5f);
        mMouse->SetAbsolutePositionBounds(1024, 600);
        mMouse->EnableCustomCursor(true);

        mInputHandler = std::make_unique<CInputEventHandler>();

        mKeyboard->AddCallback(mInputHandler.get());
        mMouse->AddCallback(mInputHandler.get());

		return lResult;
	}

	EResult COSXGameFramework::InitializeResourceFactories()
	{
		EResult lResult;

		CBinaryResourceFactory *lBinaryFactory = nullptr;
		CImageResourceFactory *lImageFactory = nullptr;
		CFontResourceFactory *lFontFactory = nullptr;
		CModelResourceFactory *lModelFactory = nullptr;
		CXmlResourceFactory *lXmlFactory = nullptr;

		CFreeImageLoader *lFreeImageLoader = nullptr;
		CFreeTypeLoader *lFontLoader = nullptr;
		CObjModelLoader *lObjModelLoader = nullptr;
		CEzmModelLoader *lEzmModelLoader = nullptr;
		CDaeModelLoader *lDaeModelLoader = nullptr;

		mResourceManager->RegisterFactory(EResourceType::Binary, &lBinaryFactory);
		mResourceManager->RegisterFactory(EResourceType::Image, &lImageFactory);
		mResourceManager->RegisterFactory(EResourceType::Font, &lFontFactory);
		mResourceManager->RegisterFactory(EResourceType::Model, &lModelFactory);
		mResourceManager->RegisterFactory(EResourceType::Xml, &lXmlFactory);

		lImageFactory->RegisterLoader(EImageLoaderType::FreeImage, &lFreeImageLoader);
		lFontFactory->RegisterLoader(EFontLoaderType::FreeType, &lFontLoader);
		lModelFactory->RegisterLoader(EModelLoaderType::Obj, &lObjModelLoader);
		lModelFactory->RegisterLoader(EModelLoaderType::Ezm, &lEzmModelLoader);
		lModelFactory->RegisterLoader(EModelLoaderType::Dae, &lDaeModelLoader);

		lResult = lFontLoader->Create();
		if (lResult != EResult::Success)
		{
			LOG_ERROR(_T("CFreeTypeLoader creation failed"));
		}

		return lResult;
	}

	EResult COSXGameFramework::InitializeGraphics()
	{
		EResult lResult;
		IGameWindow *lWindow;

		uint32_t luiWidth = 1024;
		uint32_t luiHeight = 768;

		CWindowParameters lWinParams(luiWidth, luiHeight, _T("GLFWGameWindow"), false);
		CContextProperties lContextProps(8, 8, 8, 8, 24, 0, 0);

		lResult = mSystem->CreateGameWindow(lWinParams, lContextProps, &lWindow);
		if (lResult != EResult::Success)
		{
			LOG_ERROR(_T("CreateGameWindow() failed"));
		}
		else
		{
			mWindow = IGameWindowPtr(lWindow);

			auto lGraphicsContext = std::make_unique<CGLFWGraphicsContext>(lContextProps);
			lResult = lGraphicsContext->Create(dynamic_cast<CGLFWGameWindow*>(mWindow.get()));

			if (lResult != EResult::Success)
            {
                LOG_DEBUG(_T("CGLFWGraphicsContext::Create() failed"));
            }
			else
            {
                mGraphicsDevice = std::make_unique<CGLGraphicsDevice>();
                lResult = mGraphicsDevice->Create(mWindow.get(), std::move(lGraphicsContext));

                if (lResult != EResult::Success)
                {
                    LOG_DEBUG(_T("CGLGraphicsDevice::Create() failed"));
                }
            }
		}

		return lResult;
	}

	EResult COSXGameFramework::InitializeManagers()
	{
		EResult lResult;

		mEventManager = std::make_unique<CEventManager>();
		mProgramManager = std::make_unique<CProgramManager>(mGraphicsDevice.get());
		mTextureManager = std::make_unique<CTextureManager>(mGraphicsDevice.get());		
		mVertexCacheManager = std::make_unique<CVertexCacheManager>(mGraphicsDevice.get());

		CVertexFormatPtr lModelFormat = std::make_unique<CVertexFormat>();
		lModelFormat->AddElementRange(SModelVertex::Elements, SModelVertex::NumElements);

		CVertexFormatPtr lPFormat = std::make_unique<CVertexFormat>();
		lPFormat->AddElementRange(SVertexP::Elements, SVertexP::NumElements);
		
		CVertexFormatPtr lPTFormat = std::make_unique<CVertexFormat>();
		lPTFormat->AddElementRange(SVertexPT::Elements, SVertexPT::NumElements);

		CVertexFormatPtr lPCTFormat = std::make_unique<CVertexFormat>();
		lPCTFormat->AddElementRange(SVertexPCT::Elements, SVertexPCT::NumElements);

		mVertexCacheManager->AddVertexFormat(_T("ModelVertex"), std::move(lModelFormat));
		mVertexCacheManager->AddVertexFormat(_T("VertexP"), std::move(lPFormat));
		mVertexCacheManager->AddVertexFormat(_T("VertexPT"), std::move(lPTFormat));
		mVertexCacheManager->AddVertexFormat(_T("VertexPCT"), std::move(lPCTFormat));
		
		lResult = mVertexCacheManager->AddVertexCache(_T("ModelVertex"), 
			EIndexType::SixteenBit, 6553500, 6553500);

		if (lResult != EResult::Success)
		{
			LOG_ERROR(_T("Unable to add VertexCache for vertex format ModelVertex"));
		}
		else
		{
			lResult = mVertexCacheManager->AddVertexCache(_T("VertexP"), 
				EIndexType::SixteenBit, 65535, 65535);

			if (lResult != EResult::Success)
			{
				LOG_ERROR(_T("Unable to add VertexCache for vertex format VertexP"));
			}
			else
			{
				lResult = mVertexCacheManager->AddVertexCache(_T("VertexPCT"),
					EIndexType::SixteenBit, 1024, 1024);

				if (lResult != EResult::Success)
				{
					LOG_ERROR(_T("Unable to add VertexCache for vertex format VertexCanvas"));
				}
			}
		}

		return lResult;
	}

	EResult COSXGameFramework::InitializeCanvas()
	{
		EResult lResult;
		
		CVertexFormat *lVertexFormat;
		IProgram *lProgram;

		Map<EShaderType, String> lRectShaders =
		{
			{ EShaderType::Vertex, _T("/Assets/Programs/canvas.vert.glsl") },
			{ EShaderType::Fragment, _T("/Assets/Programs/canvas.frag.glsl") }
		};

		lResult = mVertexCacheManager->GetVertexFormat(_T("VertexPCT"), &lVertexFormat);
        if (lResult != EResult::Success)
        {
            LOG_ERROR(_T("GetVertexFormat() for VertexPCT failed"));
        }
        else
        {
            lResult = GetProgramManager()->AddProgram(_T("CanvasProgram"), lRectShaders,
                                                      lVertexFormat, &lProgram);

            if (lResult != EResult::Success) {
                LOG_ERROR(_T("Unable to load canvas program"));
            } else {
                mCanvas = std::make_unique<CCanvas>();
                lResult = mCanvas->Create(mGraphicsDevice.get(), 65535, 65535);

                if (lResult != EResult::Success) {
                    LOG_DEBUG(_T("Unable to canvas canvas"));
                }
            }
        }
		
		return lResult;
	}

	EResult COSXGameFramework::InitializeControl()
	{
		EResult lResult;
		CXmlControlContainerLoader *lLoader = nullptr;

		mUIManager = std::make_unique<CUIManager>();
		lResult = mUIManager->RegisterLoader(EControlContainerLoaderType::Xml, &lLoader);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG(_T("Unable to register XmlControlContainerLoader"));
		}

		return lResult;
	}

	EResult COSXGameFramework::InitializeSpriteBatch()
	{
		EResult lResult;
		CVertexFormat *lVertexFormat = nullptr;
		IProgram *lProgram = nullptr;

		Map<EShaderType, String> lSpriteShaders =
		{
			{ EShaderType::Vertex, _T("/Assets/Programs/sprite.vert.glsl") },
			{ EShaderType::Fragment, _T("/Assets/Programs/sprite.frag.glsl") }
		};

		lResult = mVertexCacheManager->GetVertexFormat(_T("VertexPCT"), &lVertexFormat);
        if (lResult != EResult::Success)
        {
            LOG_ERROR(_T("GetVertexFormat() for VertexPCT failed"));
        }
        else
        {
            lResult = GetProgramManager()->AddProgram(_T("SpriteBatchProgram"), lSpriteShaders,
                                                      lVertexFormat, &lProgram);

            if (lResult != EResult::Success) {
                LOG_ERROR(_T("Unable to load Sprite batch program"));
            } else {
                mSpriteBatch = std::make_unique<CSpriteBatch>();
                lResult = mSpriteBatch->Create(mGraphicsDevice.get(), 21845, 65355);

                if (lResult != EResult::Success) {
                    LOG_ERROR(_T("Sprite batch creation failed"));
                }
            }
        }

		return lResult;
	}

	EResult COSXGameFramework::InitializeDebugRenderer()
	{
		EResult lResult;
		CVertexFormat *lVertexFormat;
		IProgram *lProgram;

		Map<EShaderType, String> lDebugShaders =
		{
			{ EShaderType::Vertex, _T("/Assets/Programs/debug.vert.glsl") },
			{ EShaderType::Fragment, _T("/Assets/Programs/debug.frag.glsl") }
		};

		lResult = mVertexCacheManager->GetVertexFormat(_T("VertexP"), &lVertexFormat);
        if (lResult != EResult::Success)
        {
            LOG_ERROR(_T("GetVertexFormat() for VertexP failed"));
        }
        else
        {
            lResult = GetProgramManager()->AddProgram(_T("DebugProgram"), lDebugShaders,
                                                      lVertexFormat, &lProgram);

            if (lResult != EResult::Success) {
                LOG_ERROR(_T("Unable to load debug program"));
            } else {
                mDebugRenderer = std::make_unique<CDebugRenderer>();
                lResult = mDebugRenderer->Initialize();

                if (lResult != EResult::Success) {
                    LOG_ERROR(_T("Debug Renderer creation failed"));
                } else {
                    mDebugRenderer->SetDebugColor(CColor4F(0.5f, 0.5f, 0.5f, 1.0f));
                }
            }
        }

		return lResult;
	}
}
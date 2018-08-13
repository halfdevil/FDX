
#include "Game/Win32GameFramework.h"
#include "OS/Win32/Win32System.h"
#include "OS/Win32/Win32FileSystem.h"
#include "OS/Win32/Win32GameTimer.h"
#include "GLFW/GLFWUtil.h"
#include "GLFW/GLFWGameWindow.h"
#include "GLFW/GLFWGraphicsContext.h"
#include "GLFW/GLFWKeyboard.h"
#include "GLFW/GLFWMouse.h"
#include "OGL/GLGraphicsDevice.h"
#include "Core/BinaryResourceFactory.h"
#include "Core/XmlResourceFactory.h"
#include "Core/ImageResourceFactory.h"
#include "Core/FontResourceFactory.h"
#include "Core/ModelResourceFactory.h"
#include "Core/FreeImageLoader.h"
#include "Core/FreeTypeLoader.h"
#include "Core/AssimpModelLoader.h"
#include "GUI/XmlControlContainerLoader.h"

namespace FDX
{
	CWin32GameFramework::CWin32GameFramework()
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

	EResult CWin32GameFramework::Initialize()
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

		lResult = InitializeUIManager();
		if (lResult != EResult::Success)
			return lResult;

		lResult = InitializePrograms();
		if (lResult != EResult::Success)
			return lResult;

		lResult = OnLoadContent();
		if (lResult != EResult::Success)
			return lResult;

		return lResult;
	}

	void CWin32GameFramework::Update()
	{
		mAsyncQueue->DemultiplexEvents();
		mEventManager->ProcessEvents();

		mTimer->Update();
		mKeyboard->Update();
		mMouse->Update();
	}

	void CWin32GameFramework::Frame()
	{
		if (!mbExitGame)
		{
			OnUpdate();
			OnFixedUpdate();
			OnDraw();

			mGraphicsDevice->GetGraphicsContext()->SwapBuffers();
		}
	}

	void CWin32GameFramework::Exit()
	{
		mbExitGame = true;
		OnUnloadContent();

		if (mWindow)
			mWindow->Close();
	}

	void CWin32GameFramework::Run()
	{
		mWindow->Show();
		mTimer->Start();

		while (!mbExitGame && !mWindow->IsClosed())
		{
			Update();
			ISystem::Get().PollEvents();
			Frame();
		}
	}

	EResult CWin32GameFramework::OnLoadContent()
	{
		return EResult::Success;
	}

	EResult CWin32GameFramework::OnUnloadContent()
	{
		return EResult::Success;
	}

	EResult CWin32GameFramework::OnUpdate()
	{
		return EResult::Success;
	}

	EResult CWin32GameFramework::OnFixedUpdate()
	{
		return EResult::Success;
	}

	EResult CWin32GameFramework::OnDraw()
	{
		return EResult::Success;
	}

	EResult CWin32GameFramework::InitializeSystem()
	{
		EResult lResult;
		IGameTimer *lTimer;
		HINSTANCE lInstance = GetModuleHandle(nullptr);

		if (lInstance == nullptr)
		{
			return EResult::CreateFailed;
		}
		else
		{
			mSystem = std::make_unique<CWin32System>(lInstance);

			lResult = mSystem->CreateGameTimer(&lTimer);
			if (lResult != EResult::Success)
			{
				LOG_ERROR("CreateGameTimer() failed");
			}
			else
			{
				mTimer = IGameTimerPtr(lTimer);

				lResult = CGLFWUtil::Initialize();
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CGLFWUtil::Initialize() failed");
				}
			}
		}

		return lResult;
	}

	EResult CWin32GameFramework::InitializeLoggers()
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
		lResult = mLogger->Create(lstrWorkingDirectory, "GameFramework.log");

		return lResult;
	}

	EResult CWin32GameFramework::InitlalizeResourceManager()
	{
		EResult lResult;
		String lstrAssetsDirectory;
		String lstrWorkingDirectory;
		CDirectory *lAssetsDirectory;

		lResult = mSystem->GetExecutableDirectory(lstrWorkingDirectory);
		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to get current working directory");
		}
		else
		{
			mSystem->CombinePath(lstrWorkingDirectory, "Assets", lstrAssetsDirectory);
			mFileSystem = std::make_unique<CWin32FileSystem>();

			lResult = mFileSystem->OpenDirectory(EOpenMode::Open, "AssetsDirectory",
				lstrAssetsDirectory, &lAssetsDirectory);

			if (lResult != EResult::Success)
			{
				LOG_ERROR("Unable to open assets directory");
			}
			else
			{
				lResult = mFileSystem->AddMountPoint("/Assets", lAssetsDirectory);
				if (lResult != EResult::Success)
				{
					LOG_ERROR("Unable to add assets directory as mount point");
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

	EResult CWin32GameFramework::InitializeInputs()
	{
		EResult lResult = EResult::Success;

		mKeyboard = std::make_unique<CGLFWKeyboard>(dynamic_cast<CGLFWGameWindow*>(mWindow.get()));
		mMouse = std::make_unique<CGLFWMouse>(dynamic_cast<CGLFWGameWindow*>(mWindow.get()));
		mMouse->SetMouseSensivity(1.0f);
		mMouse->SetDoubleClickLimit(0.5f);
		mMouse->SetAbsolutePositionBounds(1024, 768);
		mMouse->EnableCustomCursor(true);

		mInputHandler = std::make_unique<CInputEventHandler>();

		mKeyboard->AddCallback(mInputHandler.get());
		mMouse->AddCallback(mInputHandler.get());

		return lResult;
	}

	EResult CWin32GameFramework::InitializeResourceFactories()
	{
		EResult lResult;

		CBinaryResourceFactory *lBinaryFactory = nullptr;
		CImageResourceFactory *lImageFactory = nullptr;
		CFontResourceFactory *lFontFactory = nullptr;
		CModelResourceFactory *lModelFactory = nullptr;
		CXmlResourceFactory *lXmlFactory = nullptr;

		CFreeImageLoader *lFreeImageLoader = nullptr;
		CFreeTypeLoader *lFontLoader = nullptr;
		CAssimpModelLoader *lModelLoader = nullptr;

		mResourceManager->RegisterFactory(EResourceType::Binary, &lBinaryFactory);
		mResourceManager->RegisterFactory(EResourceType::Image, &lImageFactory);
		mResourceManager->RegisterFactory(EResourceType::Font, &lFontFactory);
		mResourceManager->RegisterFactory(EResourceType::Model, &lModelFactory);
		mResourceManager->RegisterFactory(EResourceType::Xml, &lXmlFactory);

		lImageFactory->RegisterLoader(EImageLoaderType::FreeImage, &lFreeImageLoader);
		lFontFactory->RegisterLoader(EFontLoaderType::FreeType, &lFontLoader);
		lModelFactory->RegisterLoader(EModelLoaderType::Assimp, &lModelLoader);

		lResult = lFontLoader->Create();
		if (lResult != EResult::Success)
		{
			LOG_ERROR("CFreeTypeLoader creation failed");
		}

		return lResult;
	}

	EResult CWin32GameFramework::InitializeGraphics()
	{
		EResult lResult;
		IGameWindow *lWindow;

		uint32_t luiWidth = 1024;
		uint32_t luiHeight = 768;

		CWindowParameters lWinParams(luiWidth, luiHeight, "GLFWGameWindow", false);
		CContextProperties lContextProps(8, 8, 8, 8, 24, 0, 2);

		lResult = mSystem->CreateGameWindow(lWinParams, lContextProps, &lWindow);
		if (lResult != EResult::Success)
		{
			LOG_ERROR("CreateGameWindow() failed");
		}
		else
		{
			mWindow = IGameWindowPtr(lWindow);

			auto lGraphicsContext = std::make_unique<CGLFWGraphicsContext>(lContextProps);
			lResult = lGraphicsContext->Create(dynamic_cast<CGLFWGameWindow*>(mWindow.get()));

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CGLFWGraphicsContext::Create() failed");
			}
			else
			{
				mGraphicsDevice = std::make_unique<CGLGraphicsDevice>();
				lResult = mGraphicsDevice->Create(mWindow.get(), std::move(lGraphicsContext));

				if (lResult != EResult::Success)
				{
					LOG_DEBUG("CGLGraphicsDevice::Create() failed");
				}
			}
		}

		return lResult;
	}

	EResult CWin32GameFramework::InitializeManagers()
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

		mVertexCacheManager->AddVertexFormat("ModelVertex", std::move(lModelFormat));
		mVertexCacheManager->AddVertexFormat("VertexP", std::move(lPFormat));
		mVertexCacheManager->AddVertexFormat("VertexPT", std::move(lPTFormat));
		mVertexCacheManager->AddVertexFormat("VertexPCT", std::move(lPCTFormat));

		lResult = mVertexCacheManager->AddVertexCache("ModelVertex",
			EIndexType::SixteenBit, 655350, 655350);

		if (lResult != EResult::Success)
		{
			LOG_ERROR("Unable to add VertexCache for vertex format ModelVertex");
		}
		else
		{
			lResult = mVertexCacheManager->AddVertexCache("VertexP",
				EIndexType::SixteenBit, 65535, 65535);

			if (lResult != EResult::Success)
			{
				LOG_ERROR("Unable to add VertexCache for vertex format VertexP");
			}
			else
			{
				lResult = mVertexCacheManager->AddVertexCache("VertexPCT",
					EIndexType::SixteenBit, 65535, 65535);

				if (lResult != EResult::Success)
				{
					LOG_ERROR("Unable to add VertexCache for vertex format VertexCanvas");
				}
				else
				{
					lResult = mVertexCacheManager->AddVertexCache("VertexPT",
						EIndexType::SixteenBit, 65535, 65535);

					if (lResult != EResult::Success)
					{
						LOG_ERROR("Unable to add VertexCache for vertex format VertexCanvas");
					}
				}
			}
		}

		return lResult;
	}

	EResult CWin32GameFramework::InitializePrograms()
	{
		EResult lResult = EResult::Success;

		UnorderedMap<String, CProgramParameters> lProgramParameters =
		{
			{
				"ModelDiffuseProgram", 
				{
					"ModelVertex", 
					{ 
						{ EShaderType::Vertex, "/Assets/Programs/model_diffuse.vert.glsl" },
						{ EShaderType::Fragment, "/Assets/Programs/model_diffuse.frag.glsl" } 
					}
				}
			},
			{
				"ModelLightProgram",
				{
					"ModelVertex",
					{
						{ EShaderType::Vertex, "/Assets/Programs/model_light.vert.glsl" },
						{ EShaderType::Fragment, "/Assets/Programs/model_light.frag.glsl" }
					}
				}
			},
			{
				"ModelShadowProgram",
				{
					"ModelVertex",
					{
						{ EShaderType::Vertex, "/Assets/Programs/model_shadow.vert.glsl" },
						{ EShaderType::Fragment, "/Assets/Programs/model_shadow.frag.glsl" }
					}
				}
			},
			{
				"SkyBoxProgram",
				{
					"VertexP",
					{
						{ EShaderType::Vertex, "/Assets/Programs/skybox.vert.glsl" },
						{ EShaderType::Fragment, "/Assets/Programs/skybox.frag.glsl" }
					}
				}
			},
			{
				"QuadPostProcessorProgram",
				{
					"VertexP",
					{
						{ EShaderType::Vertex, "/Assets/Programs/quad_postprocessor.vert.glsl" },
						{ EShaderType::Fragment, "/Assets/Programs/quad_postprocessor.frag.glsl" }
					}
				}
			}
		};

		for (const auto &lProgramParam : lProgramParameters)
		{
			IProgram *lProgram = nullptr;

			lResult = mProgramManager->AddProgram(lProgramParam.first, 
				lProgramParam.second, &lProgram);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("AddProgram() failed for: %s", lProgramParam.first.c_str());
				break;
			}
		}

		return lResult;
	}

	EResult CWin32GameFramework::InitializeCanvas()
	{
		EResult lResult;

		CVertexFormat *lVertexFormat;
		IProgram *lProgram;

		ProgramShaders lRectShaders =
		{
			{ EShaderType::Vertex, "/Assets/Programs/canvas.vert.glsl" },
			{ EShaderType::Fragment, "/Assets/Programs/canvas.frag.glsl" }
		};

		lResult = mVertexCacheManager->GetVertexFormat("VertexPCT", &lVertexFormat);
		if (lResult != EResult::Success)
		{
			LOG_ERROR("GetVertexFormat() for VertexPCT failed");
		}
		else
		{
			lResult = GetProgramManager()->AddProgram("CanvasProgram", lRectShaders,
				lVertexFormat, &lProgram);

			if (lResult != EResult::Success) 
			{
				LOG_ERROR("Unable to load canvas program");
			}
			else 
			{
				mCanvas = std::make_unique<CCanvas>();
				lResult = mCanvas->Create(mGraphicsDevice.get(), 65535, 65535);

				if (lResult != EResult::Success) {
					LOG_DEBUG("Unable to canvas canvas");
				}
			}
		}

		return lResult;
	}

	EResult CWin32GameFramework::InitializeUIManager()
	{
		EResult lResult;
		CXmlControlContainerLoader *lLoader = nullptr;

		mUIManager = std::make_unique<CUIManager>();
		lResult = mUIManager->RegisterLoader(EControlContainerLoaderType::Xml, &lLoader);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Unable to register XmlControlContainerLoader");
		}

		return lResult;
	}

	EResult CWin32GameFramework::InitializeSpriteBatch()
	{
		EResult lResult;
		CVertexFormat *lVertexFormat = nullptr;
		IProgram *lProgram = nullptr;

		ProgramShaders lSpriteShaders =
		{
			{ EShaderType::Vertex, "/Assets/Programs/sprite.vert.glsl" },
			{ EShaderType::Fragment, "/Assets/Programs/sprite.frag.glsl" }
		};

		lResult = mVertexCacheManager->GetVertexFormat("VertexPCT", &lVertexFormat);
		if (lResult != EResult::Success)
		{
			LOG_ERROR("GetVertexFormat() for VertexPCT failed");
		}
		else
		{
			lResult = GetProgramManager()->AddProgram("SpriteBatchProgram", lSpriteShaders,
				lVertexFormat, &lProgram);

			if (lResult != EResult::Success) 
			{
				LOG_ERROR("Unable to load Sprite batch program");
			}
			else 
			{
				mSpriteBatch = std::make_unique<CSpriteBatch>();
				lResult = mSpriteBatch->Create(mGraphicsDevice.get(), 21845, 65355);

				if (lResult != EResult::Success) {
					LOG_ERROR("Sprite batch creation failed");
				}
			}
		}

		return lResult;
	}

	EResult CWin32GameFramework::InitializeDebugRenderer()
	{
		EResult lResult;
		CVertexFormat *lVertexFormat;
		IProgram *lProgram;

		ProgramShaders lDebugShaders =
		{
			{ EShaderType::Vertex, "/Assets/Programs/debug.vert.glsl" },
			{ EShaderType::Fragment, "/Assets/Programs/debug.frag.glsl" }
		};

		lResult = mVertexCacheManager->GetVertexFormat("VertexP", &lVertexFormat);
		if (lResult != EResult::Success)
		{
			LOG_ERROR("GetVertexFormat() for VertexP failed");
		}
		else
		{
			lResult = GetProgramManager()->AddProgram("DebugProgram", lDebugShaders,
				lVertexFormat, &lProgram);

			if (lResult != EResult::Success) 
			{
				LOG_ERROR("Unable to load debug program");
			}
			else 
			{
				mDebugRenderer = std::make_unique<CDebugRenderer>();
				lResult = mDebugRenderer->Initialize();

				if (lResult != EResult::Success) {
					LOG_ERROR("Debug Renderer creation failed");
				}
				else {
					mDebugRenderer->SetDebugColor(CColor4F(0.5f, 0.5f, 0.5f, 1.0f));
				}
			}
		}

		return lResult;
	}
}
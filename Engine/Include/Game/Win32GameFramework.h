#pragma once

#include "Core/ResourceManager.h"
#include "Graphics/GraphicsDevice.h"
#include "OS/System.h"
#include "FS/FileSystem.h"
#include "OS/GameTimer.h"
#include "OS/GameWindow.h"
#include "OS/GameLogger.h"
#include "Graphics/ProgramManager.h"
#include "Graphics/TextureManager.h"
#include "Graphics/VertexCacheManager.h"
#include "Scene/DebugRenderer.h"
#include "Graphics/Canvas.h"
#include "Graphics/SpriteBatch.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/InputEventHandler.h"
#include "Core/EventManager.h"
#include "GUI/UIManager.h"

namespace FDX
{
	class FDX_EXPORT CWin32GameFramework
	{
	public:

		CWin32GameFramework();
		virtual ~CWin32GameFramework() = default;

		CGameLogger* GetLogger() const
		{
			return mLogger.get();
		}

		CResourceManager* GetResourceManager() const
		{
			return mResourceManager.get();
		}

		ISystem* GetSystem() const
		{
			return mSystem.get();
		}

		IFileSystem* GetFileSystem() const
		{
			return mFileSystem.get();
		}

		IGraphicsDevice* GetGraphicsDevice() const
		{
			return mGraphicsDevice.get();
		}

		IGameWindow* GetWindow() const
		{
			return mWindow.get();
		}

		IGameTimer* GetTimer() const
		{
			return mTimer.get();
		}

		CProgramManager* GetProgramManager() const
		{
			return mProgramManager.get();
		}

		CTextureManager* GetTextureManager() const
		{
			return mTextureManager.get();
		}

		CDebugRenderer* GetDebugRenderer() const
		{
			return mDebugRenderer.get();
		}

		CCanvas* GetCanvas() const
		{
			return mCanvas.get();
		}

		CSpriteBatch* GetSpriteBatch() const
		{
			return mSpriteBatch.get();
		}

		CKeyboard* GetKeyboard() const
		{
			return mKeyboard.get();
		}

		CMouse* GetMouse() const
		{
			return mMouse.get();
		}

		CInputEventHandler* GetInputEventHandler() const
		{
			return mInputHandler.get();
		}

		CEventManager* GetEventManager() const
		{
			return mEventManager.get();
		}

		CUIManager* GetUIManager() const
		{
			return mUIManager.get();
		}

		CVertexCacheManager* GetVertexCacheManager() const
		{
			return mVertexCacheManager.get();
		}

		bool IsExitGame() const
		{
			return mbExitGame;
		}

		EResult Initialize();

		void Update();
		void Frame();
		void Exit();
		void Run();

	protected:

		virtual EResult OnLoadContent();
		virtual EResult OnUnloadContent();
		virtual EResult OnUpdate();
		virtual EResult OnFixedUpdate();
		virtual EResult OnDraw();

	private:

		EResult InitializeSystem();
		EResult InitializeLoggers();
		EResult InitlalizeResourceManager();
		EResult InitializeInputs();
		EResult InitializeResourceFactories();
		EResult InitializeGraphics();
		EResult InitializeManagers();
		EResult InitializePrograms();
		EResult InitializeCanvas();
		EResult InitializeUIManager();
		EResult InitializeSpriteBatch();
		EResult InitializeDebugRenderer();

	protected:

		bool mbExitGame;
		CAsyncQueuePtr mAsyncQueue;
		CGameLoggerPtr mLogger;
		CResourceManagerPtr mResourceManager;
		IGameWindowPtr mWindow;
		IGameTimerPtr mTimer;
		ISystemPtr mSystem;
		IFileSystemPtr mFileSystem;
		IGraphicsDevicePtr mGraphicsDevice;
		CProgramManagerPtr mProgramManager;
		CTextureManagerPtr mTextureManager;
		CVertexCacheManagerPtr mVertexCacheManager;
		CDebugRendererPtr mDebugRenderer;
		CCanvasPtr mCanvas;
		CSpriteBatchPtr mSpriteBatch;
		CKeyboardPtr mKeyboard;
		CMousePtr mMouse;
		CInputEventHandlerPtr mInputHandler;
		CEventManagerPtr mEventManager;
		CUIManagerPtr mUIManager;
	};
}

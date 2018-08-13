
#include "OGL/GLGraphicsContext.h"
#include "OS/GameLogger.h"
#include "OS/Win32GameWindow.h"
#include "OS/Win32System.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>

namespace FDX
{
	CGLGraphicsContext::CGLGraphicsContext(const CContextProperties & pProperties)
		: mProperties(pProperties)
	{
	}

	CGLGraphicsContext::~CGLGraphicsContext()
	{
	}

	EResult CGLGraphicsContext::Create(IGameWindow *pWindow)
	{
		EResult lResult = EResult::Success;
		CWin32System *lSystem = dynamic_cast<CWin32System*>(ISystem::GetPtr());

		IFileSystem *lFileSystem = IFileSystem::GetPtr();

		CWindowParameters lParameters = CWindowParameters(800, 600, _T("TempWindow"), false);
		CWin32GameWindow *lTempWindow = new CWin32GameWindow(lParameters);
		
		lResult = lTempWindow->Create(lSystem->GetInstance());
		if (lResult != EResult::Success)
		{
			LOG_ERROR(_T("Unable to create temporary window"));
			SAFE_DELETE(lTempWindow);

			return lResult;
		}

		PIXELFORMATDESCRIPTOR lFormatDesc =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			mProperties.GetColorBits(),
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			mProperties.GetDepthBits(),
			mProperties.GetStencilBits(),
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		GLint liGLMajor, liGLMinor;

		HGLRC lTempRC = nullptr;
		HDC lTempDC = GetDC(lTempWindow->GetHWND());
		int32_t liPixelFormat = ChoosePixelFormat(lTempDC, &lFormatDesc);

		if (liPixelFormat == 0)
		{
			LOG_ERROR(_T("Unable to choose pixel format for temporary window"));			
			lResult = EResult::CreateFailed;
		}
		else
		{
			if (!SetPixelFormat(lTempDC, liPixelFormat, &lFormatDesc))
			{
				LOG_ERROR(_T("Unable to set pixel format for temporary window"));
				lResult = EResult::CreateFailed;
			}
			else
			{
				lTempRC = wglCreateContext(lTempDC);
				if (!lTempRC)
				{
					LOG_ERROR(_T("Unable to create temporary WGL context"));
					lResult = EResult::CreateFailed;
				}
				else
				{
					wglMakeCurrent(lTempDC, lTempRC);
				}
			}
		}

		if (lResult == EResult::Success)
		{
			GLenum lError = glewInit();
			
			if (lError != GL_NO_ERROR)
			{
				LOG_ERROR(_T("Unable to initialize glew: %s"), glewGetErrorString(lError));
				lResult = EResult::CreateFailed;
			}
		}

		if (lResult == EResult::Success)
		{
			glGetIntegerv(GL_MAJOR_VERSION, &liGLMajor);
			glGetIntegerv(GL_MINOR_VERSION, &liGLMinor);

			if (liGLMajor == OGL_MAJOR_VERSION)
			{
				if (liGLMinor < OGL_MINOR_VERSION)
				{
					LOG_ERROR(_T("OpenGL version (%d.%d) less than required version (%d.%d): "),
						liGLMajor, liGLMinor, OGL_MAJOR_VERSION, OGL_MINOR_VERSION);
					lResult = EResult::CreateFailed;
				}
			}
			else if (liGLMajor < OGL_MAJOR_VERSION)
			{
				LOG_ERROR(_T("OpenGL version (%d.%d) less than required version (%d.%d): "),
					liGLMajor, liGLMinor, OGL_MAJOR_VERSION, OGL_MINOR_VERSION);
				lResult = EResult::CreateFailed;
			}
		}

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(lTempRC);

		ReleaseDC(lTempWindow->GetHWND(), lTempDC);
		SAFE_DELETE(lTempWindow);

		if (lResult == EResult::Success)
		{
			mWindow = dynamic_cast<CWin32GameWindow*>(pWindow);
			mHDC = GetDC(mWindow->GetHWND());
			
			int32_t liPixelAttribs[] =
			{
				WGL_SUPPORT_OPENGL_ARB, 1,
				WGL_DRAW_TO_WINDOW_ARB, 1,
				WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
				WGL_COLOR_BITS_ARB,     mProperties.GetColorBits(),
				WGL_DEPTH_BITS_ARB,     mProperties.GetDepthBits(),
				WGL_STENCIL_BITS_ARB,   mProperties.GetStencilBits(),
				WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
				WGL_SAMPLE_BUFFERS_ARB, mProperties.GetMSAA() > 0 ? GL_TRUE : GL_FALSE,
				WGL_SAMPLES_ARB,        mProperties.GetMSAA(),
				WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
				0
			};

			uint32_t liPixelCount = 0;
			liPixelFormat = 0;

			if (!wglChoosePixelFormatARB(mHDC, liPixelAttribs, nullptr, 1, &liPixelFormat, &liPixelCount))
			{
				LOG_ERROR(_T("Unable to choose pixel format"));
				lResult = EResult::CreateFailed;
			}
			else
			{
				if (!SetPixelFormat(mHDC, liPixelFormat, &lFormatDesc))
				{
					LOG_ERROR(_T("Unable to set pixel format"));
					lResult = EResult::CreateFailed;
				}
				else
				{
					GLint liContextAttribs[] =
					{
						WGL_CONTEXT_MAJOR_VERSION_ARB, liGLMajor,
						WGL_CONTEXT_MINOR_VERSION_ARB, liGLMinor,
						WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
						WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#ifdef FDX_DEBUG
						| WGL_CONTEXT_DEBUG_BIT_ARB
#endif
						,0
					};

					mRC = wglCreateContextAttribsARB(mHDC, nullptr, liContextAttribs);

					if (!mRC)
					{
						LOG_ERROR(_T("Unable to create context from attributes"));
						lResult = EResult::CreateFailed;
					}
					else
					{
						wglMakeCurrent(mHDC, mRC);
					}
				}
			}

			LOG_DEBUG(_T("OpenGL %d.%d context created"), liGLMajor, liGLMinor);
		}

		return lResult;
	}

	EResult CGLGraphicsContext::Activate()
	{
		EResult lResult = EResult::Success;

		if (!wglMakeCurrent(mHDC, mRC))
		{
			LOG_DEBUG(_T("Unable to make current context active"));
			lResult = EResult::ActivateFailed;
		}

		return lResult;
	}

	EResult CGLGraphicsContext::Deactivate()
	{
		EResult lResult = EResult::Success;

		if (!wglMakeCurrent(nullptr, nullptr))
		{
			LOG_DEBUG(_T("Unable to deactivate current context"));
			lResult = EResult::ActivateFailed;
		}

		return lResult;
	}

	EResult CGLGraphicsContext::SwapBuffers()
	{
		EResult lResult = EResult::Success;

		if (!::SwapBuffers(mHDC))
			lResult = EResult::Error;

		return lResult;
	}

	EResult CGLGraphicsContext::Share(IGameWindow * pWindow, IGraphicsContext ** pContext)
	{
		return EResult::Success;		
	}
}
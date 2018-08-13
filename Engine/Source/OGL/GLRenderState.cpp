
#include "OGL/GLRenderState.h"
#include "OGL/GLUtil.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CGLBlendState::CGLBlendState()
	{
	}

	CGLBlendState::~CGLBlendState()
	{
	}

	void CGLBlendState::SetProperties(const SBlendStateProperties & pProperties)
	{
		mProperties = pProperties;
		mbBlendEnabled = pProperties.BlendEnabled;
		mSrcColorBlend = CGLUtil::GetBlendOption(pProperties.SourceColorBlend);
		mDestColorBlend = CGLUtil::GetBlendOption(pProperties.DestinationColorBlend);
		mSrcAlphaBlend = CGLUtil::GetBlendOption(pProperties.SourceAlphaBlend);
		mDestAlphaBlend = CGLUtil::GetBlendOption(pProperties.DestinationAlphaBlend);		
		mColorBlendEquation = CGLUtil::GetBlendEquation(pProperties.ColorBlendEquation);
		mAlphaBlendEquation = CGLUtil::GetBlendEquation(pProperties.AlphaBlendEquation);
		mBlendColor[0] = pProperties.BlendColor.R;
		mBlendColor[1] = pProperties.BlendColor.G;
		mBlendColor[2] = pProperties.BlendColor.B;
		mBlendColor[3] = pProperties.BlendColor.A;
	}

	EResult CGLBlendState::Apply()
	{
		EResult lResult = EResult::Success;
		EMPTY_GL_ERROR();

		if (mbBlendEnabled)
		{
			glEnable(GL_BLEND);
			
			glBlendFuncSeparate(mSrcColorBlend, mDestColorBlend, 
				mSrcAlphaBlend, mDestAlphaBlend);
			glBlendEquationSeparate(mColorBlendEquation, mAlphaBlendEquation);
			glBlendColor(mBlendColor[0], mBlendColor[1],
				mBlendColor[2], mBlendColor[3]);
		}
		else
		{
			glDisable(GL_BLEND);
		}

		GLenum lGLError = glGetError();
		if (lGLError != GL_NO_ERROR)
		{
			lResult = EResult::ApplyFailed;
		}

		return lResult;
	}

	CGLRasterizerState::CGLRasterizerState()
	{
	}

	CGLRasterizerState::~CGLRasterizerState()
	{
	}

	void CGLRasterizerState::SetProperties(const SRasterizerStateProperties & pProperties)
	{
		mProperties = pProperties;
		mPolygonMode = CGLUtil::GetFillMode(pProperties.FillMode);
		mCullFace = CGLUtil::GetCullMode(pProperties.CullMode);
		mFrontFace = CGLUtil::GetFrontFace(pProperties.CullMode);
	}

	EResult CGLRasterizerState::Apply()
	{
		EResult lResult = EResult::Success;
		EMPTY_GL_ERROR();

		if (mCullFace != GL_NONE)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(mFrontFace);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		EColorMask lColorMask = mProperties.ColorMask;

		if (lColorMask != EColorMask::None)
		{
			GLboolean lbRed = lColorMask & EColorMask::Red;
			GLboolean lbGreen = lColorMask & EColorMask::Green;
			GLboolean lbBlue = lColorMask & EColorMask::Blue;
			GLboolean lbAlpha = lColorMask & EColorMask::Alpha;

			glColorMask(lbRed, lbGreen, lbBlue, lbAlpha);
		}
		else
		{
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		}

		GLenum lGLError = glGetError();
		if (lGLError != GL_NO_ERROR)
		{
			lResult = EResult::ApplyFailed;
		}

		return lResult;
	}

	CGLDepthState::CGLDepthState()
	{
	}

	CGLDepthState::~CGLDepthState()
	{
	}

	void CGLDepthState::SetProperties(const SDepthStateProperties & pProperties)
	{
		mProperties = pProperties;
		mbEnabled = pProperties.Enabled;
		mbWritable = pProperties.Writeable;
		mCompareFunction = CGLUtil::GetCompareFunction(pProperties.CompareFunction);
	}

	EResult CGLDepthState::Apply()
	{
		EResult lResult = EResult::Success;
		EMPTY_GL_ERROR();

		if (mbEnabled)
		{
			glEnable(GL_DEPTH_TEST);			
			glDepthMask(mbWritable);
			glDepthFunc(mCompareFunction);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		GLenum lGLError = glGetError();
		if (lGLError != GL_NO_ERROR)
		{
			lResult = EResult::ApplyFailed;
		}

		return lResult;
	}

	CGLStencilState::CGLStencilState()
	{
	}

	CGLStencilState::~CGLStencilState()
	{
	}

	void CGLStencilState::SetProperties(const SStencilStateProperties & pProperties)
	{
		mProperties = pProperties;
		mbEnabled = pProperties.Enabled;
		mFace = CGLUtil::GetStencilFace(pProperties.Face);
		mCompareFunction = CGLUtil::GetCompareFunction(pProperties.CompareFunction);
		muiReference = pProperties.Reference;
		muiMask = pProperties.Mask;
		muiWriteMask = pProperties.WriteMask;
		mOnFail = CGLUtil::GetStencilOption(pProperties.OnFail);
		mOnZFail = CGLUtil::GetStencilOption(pProperties.OnZFail);
		mOnZPass = CGLUtil::GetStencilOption(pProperties.OnZPass);
	}

	EResult CGLStencilState::Apply()
	{
		EResult lResult = EResult::Success;
		EMPTY_GL_ERROR();

		if (mbEnabled)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilFuncSeparate(mFace, mCompareFunction, muiReference, muiMask);
			glStencilMaskSeparate(mFace, muiWriteMask);
			glStencilOpSeparate(mFace, mOnFail, mOnZFail, mOnZPass);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}

		GLenum lGLError = glGetError();
		if (lGLError != GL_NO_ERROR)
		{
			lResult = EResult::ApplyFailed;
		}

		return lResult;
	}
}
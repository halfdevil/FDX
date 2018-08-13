#pragma once

#include "Graphics/RenderState.h"
#include <GL/gl3w.h>

namespace FDX
{
	class CGLBlendState : public IBlendState
	{
	public:

		CGLBlendState();
		~CGLBlendState();

		const SBlendStateProperties& GetProperties() const override
		{
			return mProperties;
		}

		void SetProperties(const SBlendStateProperties &pProperties) override;
		EResult Apply() override;

	private:

		SBlendStateProperties mProperties;
		GLboolean mbBlendEnabled;
		GLenum mSrcColorBlend;
		GLenum mDestColorBlend;		
		GLenum mSrcAlphaBlend;
		GLenum mDestAlphaBlend;
		GLenum mColorBlendEquation;
		GLenum mAlphaBlendEquation;
		GLfloat mBlendColor[4];
	};

	class CGLRasterizerState : public IRasterizerState
	{
	public:

		CGLRasterizerState();
		~CGLRasterizerState();

		const SRasterizerStateProperties& GetProperties() const override
		{
			return mProperties;
		}

		void SetProperties(const SRasterizerStateProperties &pProperties) override;
		EResult Apply() override;

	private:

		SRasterizerStateProperties mProperties;
		GLenum mCullFace;
		GLenum mFrontFace;
		GLenum mPolygonMode;
	};

	class CGLDepthState : public IDepthState
	{
	public:

		CGLDepthState();
		~CGLDepthState();

		const SDepthStateProperties& GetProperties() const override
		{
			return mProperties;
		}

		void SetProperties(const SDepthStateProperties &pProperties) override;
		EResult Apply() override;

	private:

		SDepthStateProperties mProperties;
		GLboolean mbEnabled;
		GLboolean mbWritable;
		GLenum mCompareFunction;
	};

	class CGLStencilState : public IStencilState
	{
	public:

		CGLStencilState();
		~CGLStencilState();

		const SStencilStateProperties& GetProperties() const override
		{
			return mProperties;
		}

		void SetProperties(const SStencilStateProperties &pProperties) override;
		EResult Apply() override;

	private:

		SStencilStateProperties mProperties;
		GLboolean mbEnabled;
		GLenum mFace;
		GLenum mCompareFunction;
		GLuint muiReference;
		GLuint muiMask;
		GLuint muiWriteMask;
		GLenum mOnFail;
		GLenum mOnZFail;
		GLenum mOnZPass;
	};
}
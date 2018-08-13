#pragma once

#include "Graphics/PostProcessor.h"
#include "Graphics/DrawOperation.h"

namespace FDX
{
	class FDX_EXPORT CQuadPostProcessor : public IPostProcessor
	{
	public:

		CQuadPostProcessor();
		virtual ~CQuadPostProcessor();

		CTexture2D* GetTexture() const override
		{
			return mTexture;
		}

		virtual EResult Create();
		virtual void SetTexture(CTexture2D *pTexture);

		EResult Process(IProgram *pProgram) override;
		
	protected:

		CTexture2D *mTexture;
		IDrawOperationPtr mDrawOperation;
	};

	using CQuadPostProcessorPtr = SmartPtr<CQuadPostProcessor>;
}
#pragma once

#include "Graphics/UniformBlock.h"
#include "Graphics/GraphicsDevice.h"

namespace FDX
{
	struct SPerFrameBlock
	{
		CMatrix4F WorldMatrix;
		CMatrix4F WorldViewMatrix;
		CMatrix4F WorldViewProjMatrix;
		CMatrix4F ViewMatrix;
		CMatrix4F ProjMatrix;
	};

	class FDX_EXPORT CPerFrameUnifromBlock : public IUniformBlock
	{
	public:

		CPerFrameUnifromBlock();
		~CPerFrameUnifromBlock();

		const SPerFrameBlock& GetPerFrameBlock() const
		{
			return mBlock;
		}

		CUniformBuffer* GetUniformBuffer() const override
		{
			return mUniformBuffer.get();
		}

		void SetWorldMatrix(const CMatrix4F &pWorld);
		void SetWorldViewMatrix(const CMatrix4F &pWorldView);
		void SetWorldViewProjMatrix(const CMatrix4F &pWorldViewProj);
		void SetViewMatrix(const CMatrix4F &pView);
		void SetProjMatrix(const CMatrix4F &pProj);

		EResult Update() override;
		EResult Apply(IProgram *pProgram) override;

		EResult Create(IGraphicsDevice *pGraphicsDevice, 
			uint32_t puiBindingIndex);

	private:

		SPerFrameBlock mBlock;
		CUniformBufferPtr mUniformBuffer;
		bool mbUpdated;
	};

	using CPerFrameUnifromBlockPtr = SmartPtr<CPerFrameUnifromBlock>;
}
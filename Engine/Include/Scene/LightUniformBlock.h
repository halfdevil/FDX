#pragma once

#include "Graphics/UniformBlock.h"
#include "Graphics/GraphicsDevice.h"
#include "Scene/Light.h"

namespace FDX
{
	struct SLightBlock
	{
		CVector3F Position;
		float Type;
		CColor4F Color;
		float Intensity;
		float ConstantAtt;
		float LinearAtt;
		float QuadraticAtt;
	};

	class FDX_EXPORT CLightUniformBlock : public IUniformBlock
	{	
	public:

		CLightUniformBlock();
		~CLightUniformBlock();

		CUniformBuffer* GetUniformBuffer() const override
		{
			return mUniformBuffer.get();
		}

		uint32_t GetNumLights() const
		{
			return mLightBlocks.size();
		}

		uint32_t GetMaxNumLights() const
		{
			return muiMaxNumLights;
		}

		void AddLight(ILight *pLight);
		void UpdateLight(uint32_t puiIdx, ILight *pLight);
		void RemoveLight(uint32_t puiIdx);

		EResult Update() override;
		EResult Apply(IProgram *pProgram) override;

		EResult Create(IGraphicsDevice *pGraphicsDevice, 
			uint32_t puiMaxNumLights, uint32_t puiBindingIndex);

	private:

		CUniformBufferPtr mUniformBuffer;
		Array<SLightBlock> mLightBlocks;
		uint32_t muiMaxNumLights;
		bool mbUpdated;
	};

	using CLightUniformBlockPtr = SmartPtr<CLightUniformBlock>;
}
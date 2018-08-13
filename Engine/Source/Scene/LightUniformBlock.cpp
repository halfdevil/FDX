
#include "Scene/LightUniformBlock.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CLightUniformBlock::CLightUniformBlock()
		: mUniformBuffer(nullptr),
		muiMaxNumLights(0),
		mbUpdated(false)
	{
	}

	CLightUniformBlock::~CLightUniformBlock()
	{
	}

	void CLightUniformBlock::AddLight(ILight * pLight)
	{
		if (pLight != nullptr)
		{
			if (mLightBlocks.size() < muiMaxNumLights)
			{
				SLightBlock lBlock;
				pLight->GetLightBlock(&lBlock);

				mLightBlocks.push_back(std::move(lBlock));
				mbUpdated = false;
			}
		}
	}

	void CLightUniformBlock::UpdateLight(uint32_t puiIdx, ILight * pLight)
	{
		if (pLight != nullptr)
		{
			if (puiIdx < mLightBlocks.size())
			{
				SLightBlock lBlock;
				pLight->GetLightBlock(&lBlock);

				mLightBlocks[puiIdx] = std::move(lBlock);
				mbUpdated = false;
			}
		}
	}

	void CLightUniformBlock::RemoveLight(uint32_t puiIdx)
	{
		if (puiIdx < mLightBlocks.size())
		{
			mLightBlocks.erase(mLightBlocks.begin() + puiIdx);
			mbUpdated = false;
		}
	}

	EResult CLightUniformBlock::Update()
	{
		EResult lResult = EResult::Success;

		if (!mbUpdated)
		{
			lResult = mUniformBuffer->UploadData(0, sizeof(SLightBlock) * mLightBlocks.size(),
				mLightBlocks.data());

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CUniformBuffer::UploadData() failed");
			}
			else
			{
				mbUpdated = true;
			}
		}

		return lResult;
	}

	EResult CLightUniformBlock::Apply(IProgram * pProgram)
	{
		EResult lResult = EResult::Success;

		if (pProgram)
		{
			pProgram->GetConstants()->SetUniformBuffer("LightUniformBlock", mUniformBuffer.get());
			pProgram->GetConstants()->SetInt("uNumLights", (int32_t)mLightBlocks.size());			
		}

		return lResult;
	}

	EResult CLightUniformBlock::Create(IGraphicsDevice * pGraphicsDevice, 
		uint32_t puiMaxNumLights, uint32_t puiBindingIndex)
	{
		EResult lResult = EResult::Success;
		CUniformBuffer *lBuffer = nullptr;

		lResult = pGraphicsDevice->CreateUniformBuffer("LightUniformBlock", EBufferUsage::Static,
			sizeof(SLightBlock) * puiMaxNumLights, puiBindingIndex, &lBuffer);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("IGraphicsDevice::CreateUniformBuffer() failed for LightUniformBlock");
		}
		else
		{
			mUniformBuffer = CUniformBufferPtr(lBuffer);
			muiMaxNumLights = puiMaxNumLights;
		}

		return lResult;
	}
}

#include "Scene/PerFrameUniformBlock.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CPerFrameUnifromBlock::CPerFrameUnifromBlock()
		: mUniformBuffer(nullptr),
		mbUpdated(false)
	{
	}

	CPerFrameUnifromBlock::~CPerFrameUnifromBlock()
	{
	}

	void CPerFrameUnifromBlock::SetWorldMatrix(const CMatrix4F & pWorld)
	{
		mBlock.WorldMatrix = pWorld;
		mbUpdated = false;
	}

	void CPerFrameUnifromBlock::SetWorldViewMatrix(const CMatrix4F & pWorldView)
	{
		mBlock.WorldViewMatrix = pWorldView;
		mbUpdated = false;
	}

	void CPerFrameUnifromBlock::SetWorldViewProjMatrix(const CMatrix4F & pWorldViewProj)
	{
		mBlock.WorldViewProjMatrix = pWorldViewProj;
		mbUpdated = false;
	}

	void CPerFrameUnifromBlock::SetViewMatrix(const CMatrix4F & pView)
	{
		mBlock.ViewMatrix = pView;
		mbUpdated = false;
	}

	void CPerFrameUnifromBlock::SetProjMatrix(const CMatrix4F & pProj)
	{
		mBlock.ProjMatrix = pProj;
		mbUpdated = false;
	}

	EResult CPerFrameUnifromBlock::Update()
	{
		EResult lResult = EResult::Success;

		if (!mbUpdated)
		{
			lResult = mUniformBuffer->UploadData(0, sizeof(SPerFrameBlock),	&mBlock);
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

	EResult CPerFrameUnifromBlock::Apply(IProgram * pProgram)
	{
		EResult lResult = EResult::Success;

		if (pProgram)		
			pProgram->GetConstants()->SetUniformBuffer("PerFrameUniformBlock", mUniformBuffer.get());		

		return lResult;
	}

	EResult CPerFrameUnifromBlock::Create(IGraphicsDevice * pGraphicsDevice, uint32_t puiBindingIndex)
	{
		EResult lResult = EResult::Success;
		CUniformBuffer *lBuffer = nullptr;

		lResult = pGraphicsDevice->CreateUniformBuffer("PerFrameUniformBlock", EBufferUsage::Dynamic,
			sizeof(SPerFrameBlock), puiBindingIndex, &lBuffer);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("IGraphicsDevice::CreateUniformBuffer() failed for PerFrameUniformBlock");
		}
		else
		{
			mUniformBuffer = CUniformBufferPtr(lBuffer);
		}

		return lResult;
	}
}
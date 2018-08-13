#pragma once

#include "Core/Singleton.h"
#include "Graphics/BatchRenderer.h"
#include "Graphics/SpriteBatchMaterial.h"

namespace FDX
{
	class FDX_EXPORT CSpriteBatch : public CSingleton<CSpriteBatch>
	{
	public:

		CSpriteBatch();
		virtual ~CSpriteBatch();

		IBlendState* GetBlendState() const
		{
			return mBlendState;
		}

		const CMatrix4F& GetProjMatrix() const
		{
			return mProjMatrix;
		}

		const CMatrix4F& GetWorldMatrix() const
		{
			return mWorldMatrix;
		}

		const CColor4F& GetColor() const
		{
			return mColor;
		}

		bool IsBlendingEnabled() const
		{
			return mBlendState != nullptr;
		}

		void SetBlendState(IBlendState *pBlendState);
		void SetProjMatrix(const CMatrix4F &pProj);
		void SetWorldMatrix(const CMatrix4F &pWorld);
		void SetColor(const CColor4F &pColor);

		virtual EResult Create(IGraphicsDevice *pGraphicsDevice,
			uint32_t puiMaxNumVertices, uint32_t puiMaxNumIndices);

		virtual EResult Begin();
		virtual EResult End();

		virtual EResult Draw(CTexture2D *pTexture, const CVector2F &pOrigin,
			const CVector2F &pPosition, const CVector2F &pSize, 
			const CVector2F &pSrcPosition, const CVector2F &pSrcSize, 
			const CVector2F &pScale, float pfRotation, 
			bool pbFlipX, bool pbFlipY);

		virtual EResult Draw(CTexture2D *pTexture, const CVector2F &pPosition,
			const CVector2F &pSize, const CVector2F &pSrcPosition,
			const CVector2F &pSrcSize, bool pbFlipX, bool pbFlipY);
		
		virtual EResult Draw(CTexture2D *pTexture, const CVector2F &pPosition,
			const CVector2F &pSrcPosition, const CVector2F &pSrcSize);

		virtual EResult Draw(CTexture2D *pTexture, const CVector2F &pPosition,
			const CVector2F &pSize);

		virtual EResult Draw(CTexture2D *pTexture, const CVector2F &pPosition);

		virtual void StartPass(EPassType pPassType, IProgram *pProgram);
		virtual void EndPass(EPassType pPassType);

	protected:

		virtual EResult SwitchTexture(CTexture2D *pTexture);
		virtual EResult SetupStates(IGraphicsDevice *pGraphicsDevice);

	protected:

		CBatchRendererPtr mBatchRenderer;
		CSpriteBatchMaterialPtr mMaterial;
		IBlendState *mBlendState;
		CTexture2D *mCurrentTexture;
		CVector2F mInvTextureSize;
		IDepthStatePtr mEnableDepthState;
		IDepthStatePtr mDisableDepthState;
		IBlendStatePtr mDisableBlendState;
		CMatrix4F mProjMatrix;
		CMatrix4F mWorldMatrix;
		CColor4F mColor;
	};

	using CSpriteBatchPtr = SmartPtr<CSpriteBatch>;
}
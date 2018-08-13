#pragma once

#include "Math/Vector3F.h"
#include "Math/Color4F.h"
#include "Scene/Camera.h"
#include "Graphics/Program.h"
#include "Graphics/ShadowMap.h"

namespace FDX
{
	struct SLightBlock;

	enum class ELightType : uint32_t
	{
		Directional = 0,
		Point = 1
	};

	class FDX_EXPORT ILight
	{
	public:

		virtual ~ILight()
		{
		}

		virtual ELightType GetLightType() const = 0;
		virtual const CVector3F& GetPosition() const = 0;
		virtual const CColor4F& GetColor() const = 0;

		virtual bool IsShadowCaster() const = 0;
		virtual CCamera* GetCamera() const = 0;
		virtual CShadowMap* GetShadowMap() const = 0;

		virtual void SetPosition(const CVector3F &pPosition) = 0;
		virtual void SetColor(const CColor4F &pColor) = 0;
		virtual void SetCamera(CCameraPtr pCamera) = 0;

		virtual void GetLightBlock(SLightBlock *pBlock) = 0;
		virtual void Apply(IProgram *pProgram) = 0;
	};

	class FDX_EXPORT CBaseLight : public ILight
	{
	public:

		CBaseLight(ELightType pLightType);
		~CBaseLight() override;

		ELightType GetLightType() const override
		{
			return mLightType;
		}

		const CVector3F& GetPosition() const override
		{
			return mPosition;
		}

		const CColor4F& GetColor() const override
		{
			return mColor;
		}

		bool IsShadowCaster() const override
		{
			return mShadowMap != nullptr;
		}

		CCamera* GetCamera() const override
		{
			return mCamera.get();
		}

		CShadowMap* GetShadowMap() const override
		{
			return mShadowMap.get();
		}

		void SetPosition(const CVector3F &pPosition) override;
		void SetColor(const CColor4F &pColor) override;
		void SetCamera(CCameraPtr pCamera) override;

		void GetLightBlock(SLightBlock *pBlock) override;
		void Apply(IProgram *pProgram) override;

	protected:

		ELightType mLightType;
		CVector3F mPosition;
		CColor4F mColor;	
		CCameraPtr mCamera;
		CShadowMapPtr mShadowMap;
	};

	class FDX_EXPORT CDirectionalLight : public CBaseLight
	{
	public:

		CDirectionalLight();
		~CDirectionalLight();

		float GetIntensity() const
		{
			return mfIntensity;
		}			
		
		void SetIntensity(float pfIntensity);
		void GetLightBlock(SLightBlock *pBlock) override;
		void Apply(IProgram *pProgram) override;

	private:

		float mfIntensity;		
	};

	class FDX_EXPORT CPointLight : public CBaseLight
	{
	public:

		CPointLight();
		~CPointLight();

		float GetConstantAtt() const
		{
			return mfConstantAtt;
		}

		float GetLinearAtt() const
		{
			return mfLinearAtt;
		}

		float GetQuadraticAtt() const
		{
			return mfQuadraticAtt;
		}

		void SetConstantAtt(float pfAtt);
		void SetLinearAtt(float pfAtt);
		void SetQuadraticAtt(float pfAtt);
		void SetShadowMap(CShadowMapPtr pShadowMap);

		void GetLightBlock(SLightBlock *pBlock) override;
		void Apply(IProgram *pProgram) override;

	private:

		float mfConstantAtt;
		float mfLinearAtt;
		float mfQuadraticAtt;
	};

	using ILightPtr = SmartPtr<ILight>;
	using CBaseLightPtr = SmartPtr<CBaseLight>;
	using CDirectionalLightPtr = SmartPtr<CDirectionalLight>;
	using CPointLightPtr = SmartPtr<CPointLight>;
}
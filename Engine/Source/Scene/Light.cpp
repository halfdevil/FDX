
#include "Scene/Light.h"
#include "Scene/LightUniformBlock.h"

namespace FDX
{
	CBaseLight::CBaseLight(ELightType pLightType)
		: mLightType(pLightType),
		mShadowMap(nullptr),
		mCamera(nullptr)
	{
	}

	CBaseLight::~CBaseLight()
	{
	}

	void CBaseLight::SetPosition(const CVector3F & pPosition)
	{
		mPosition = pPosition;
	}

	void CBaseLight::SetColor(const CColor4F & pColor)
	{
		mColor = pColor;
	}

	void CBaseLight::SetCamera(CCameraPtr pCamera)
	{
		mCamera = std::move(pCamera);
	}

	void CBaseLight::GetLightBlock(SLightBlock * pBlock)
	{
		if (pBlock)
		{
			pBlock->Type = (float)mLightType;
			pBlock->Color = mColor;
			pBlock->Position = mPosition;
		}
	}

	void CBaseLight::Apply(IProgram * pProgram)
	{
		if (pProgram)
		{
			pProgram->GetConstants()->SetInt("uLight.Type", (int32_t)mLightType);
			pProgram->GetConstants()->SetColor4("uLight.Color", mColor);
			pProgram->GetConstants()->SetVector3("uLight.Position", mPosition);
		}
	}

	CDirectionalLight::CDirectionalLight()
		: CBaseLight(ELightType::Directional),
		mfIntensity(0.0f)
	{
	}

	CDirectionalLight::~CDirectionalLight()
	{
	}

	void CDirectionalLight::SetIntensity(float pfIntensity)
	{
		mfIntensity = pfIntensity;
	}

	void CDirectionalLight::GetLightBlock(SLightBlock * pBlock)
	{
		if (pBlock)
		{
			CBaseLight::GetLightBlock(pBlock);
			pBlock->Intensity = mfIntensity;
		}
	}

	void CDirectionalLight::Apply(IProgram * pProgram)
	{
		if (pProgram)
		{
			CBaseLight::Apply(pProgram);
			pProgram->GetConstants()->SetFloat("uLight.Intensity", mfIntensity);
		}
	}

	CPointLight::CPointLight()
		: CBaseLight(ELightType::Point),
		mfConstantAtt(1.0f),
		mfLinearAtt(0.0f),
		mfQuadraticAtt(0.0f)
	{
	}

	CPointLight::~CPointLight()
	{
	}

	void CPointLight::SetConstantAtt(float pfAtt)
	{
		mfConstantAtt = pfAtt;
	}

	void CPointLight::SetLinearAtt(float pfAtt)
	{
		mfLinearAtt = pfAtt;
	}

	void CPointLight::SetQuadraticAtt(float pfAtt)
	{
		mfQuadraticAtt = pfAtt;
	}

	void CPointLight::SetShadowMap(CShadowMapPtr pShadowMap)
	{
		mShadowMap = std::move(pShadowMap);
	}

	void CPointLight::GetLightBlock(SLightBlock * pBlock)
	{
		if (pBlock)
		{
			CBaseLight::GetLightBlock(pBlock);
			pBlock->ConstantAtt = mfConstantAtt;
			pBlock->LinearAtt = mfLinearAtt;
			pBlock->QuadraticAtt = mfQuadraticAtt;
		}
	}

	void CPointLight::Apply(IProgram * pProgram)
	{
		if (pProgram)
		{
			CBaseLight::Apply(pProgram);
			pProgram->GetConstants()->SetFloat("uLight.ConstantAtt", mfConstantAtt);
			pProgram->GetConstants()->SetFloat("uLight.LinearAtt", mfLinearAtt);
			pProgram->GetConstants()->SetFloat("uLight.QuadraticAtt", mfQuadraticAtt);
		}
	}
}
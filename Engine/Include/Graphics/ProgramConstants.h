#pragma once

#include "Core/Result.h"
#include "Math/Vector2F.h"
#include "Math/Vector3F.h"
#include "Math/Vector4F.h"
#include "Math/Matrix3F.h"
#include "Math/Matrix4F.h"
#include "Math/Color4F.h"
#include "Graphics/Texture.h"
#include "Graphics/UniformBuffer.h"

namespace FDX
{
	enum class EShaderType
	{
		Vertex,
		TessellationControl,
		TessellationEvaluation,
		Geometry,
		Fragment
	};

	class FDX_EXPORT IProgramConstants
	{
	public:

		virtual ~IProgramConstants()
		{
		}
		 
		virtual EResult GetConstantLocation(const String &pstrName, uint32_t &puiLoc) = 0;		
		virtual EResult GetSubroutineIndex(const String &pstrName, 
			EShaderType pShaderType, uint32_t &puiLoc) = 0;

		virtual EResult SetInt(const String &pstrName, int piVal) = 0;
		virtual EResult SetFloat(const String &pstrName, float pfVal) = 0;
		virtual EResult SetVector2(const String &pstrName, const CVector2F &pVec) = 0;
		virtual EResult SetVector3(const String &pstrName, const CVector3F &pVec) = 0;
		virtual EResult SetVector4(const String &pstrName, const CVector4F &pVec) = 0;
		virtual EResult SetColor4(const String &pstrName, const CColor4F &pVec) = 0;
		virtual EResult SetMatrix3(const String &pstrName, const CMatrix3F &pMat) = 0;
		virtual EResult SetMatrix4(const String &pstrName, const CMatrix4F &pMat) = 0;
		virtual EResult SetTexture(const String &pstrName, uint32_t puiUnit, ITexture *pTexture) = 0;
		virtual EResult SetUniformBuffer(const String &pstrName, CUniformBuffer *pBuffer) = 0;
		virtual EResult SetSubroutinesName(EShaderType pShaderType, const Array<String> &pSubroutines) = 0;
		virtual EResult SetMatrix4Array(const String &pstrName, const Array<CMatrix4F> &pMatrices) = 0;
		virtual EResult SetMatrix4Array(const String &pstrName, const float *pMatrixArray, uint32_t puiCount) = 0;
		virtual EResult SetFloat4Array(const String &pstrName, float *pArray, uint32_t puiCount) = 0;

		virtual void SetInt(int puiLoc, int piVal) = 0;
		virtual void SetFloat(uint32_t puiLoc, float pfVal) = 0;
		virtual void SetVector2(uint32_t puiLoc, const CVector2F &pVec) = 0;
		virtual void SetVector3(uint32_t puiLoc, const CVector3F &pVec) = 0;
		virtual void SetVector4(uint32_t puiLoc, const CVector4F &pVec) = 0;
		virtual void SetColor4(uint32_t puiLoc, const CColor4F &pVec) = 0;
		virtual void SetMatrix3(uint32_t puiLoc, const CMatrix3F &pMat) = 0;
		virtual void SetMatrix4(uint32_t puiLoc, const CMatrix4F &pMat) = 0;
		virtual void SetTexture(uint32_t puiLoc, uint32_t puiUnit, ITexture *pTexture) = 0;
		virtual void SetUniformBuffer(uint32_t puiLoc, CUniformBuffer *pBuffer) = 0;				
		virtual void SetSubroutines(EShaderType pShaderType, const Array<uint32_t> &pIndices) = 0;
		virtual void SetMatrix4Array(uint32_t puiLoc, const Array<CMatrix4F> &pMatrices) = 0;
		virtual void SetFloat4Array(uint32_t puiLoc, float *pArray, uint32_t puiCount) = 0;
	};
}
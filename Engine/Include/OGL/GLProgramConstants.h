#pragma once

#include "Graphics/ProgramConstants.h"
#include <GL/gl3w.h>

namespace FDX
{
	class CGLProgramConstants : public IProgramConstants
	{
	public:

		CGLProgramConstants(GLuint puiProgramHandle);
		~CGLProgramConstants();

		EResult Populate();
		EResult GetConstantLocation(const String &pstrName, uint32_t &puiLoc) override;
		EResult GetSubroutineIndex(const String &pstrName, EShaderType pShaderType, uint32_t &puiLoc) override;

		EResult SetInt(const String &pstrName, int piVal) override;
		EResult SetFloat(const String &pstrName, float pfVal) override;
		EResult SetVector2(const String &pstrName, const CVector2F &pVec) override;
		EResult SetVector3(const String &pstrName, const CVector3F &pVec) override;
		EResult SetVector4(const String &pstrName, const CVector4F &pVec) override;
		EResult SetColor4(const String &pstrName, const CColor4F &pVec) override;
		EResult SetMatrix3(const String &pstrName, const CMatrix3F &pMat) override;
		EResult SetMatrix4(const String &pstrName, const CMatrix4F &pMat) override;
		EResult SetTexture(const String &pstrName, uint32_t puiUnit, ITexture *pTexture) override;
		EResult SetUniformBuffer(const String &pstrName, CUniformBuffer *pBuffer) override;
		EResult SetSubroutinesName(EShaderType pShaderType, const Array<String> &pstrSubroutines) override;
		EResult SetMatrix4Array(const String &pstrName, const Array<CMatrix4F> &pMatrices) override;
		EResult SetMatrix4Array(const String &pstrName, const float *pMatrixArray, uint32_t puiCount) override;
		EResult SetFloat4Array(const String &pstrName, float *pArray, uint32_t puiCount) override;

		void SetInt(int puiLoc, int piVal) override;
		void SetFloat(uint32_t puiLoc, float pfVal) override;
		void SetVector2(uint32_t puiLoc, const CVector2F &pVec) override;
		void SetVector3(uint32_t puiLoc, const CVector3F &pVec) override;
		void SetVector4(uint32_t puiLoc, const CVector4F &pVec) override;
		void SetColor4(uint32_t puiLoc, const CColor4F &pVec) override;
		void SetMatrix3(uint32_t puiLoc, const CMatrix3F &pMat) override;
		void SetMatrix4(uint32_t puiLoc, const CMatrix4F &pMat) override;
		void SetTexture(uint32_t puiLoc, uint32_t puiUnit, ITexture *pTexture) override;
		void SetUniformBuffer(uint32_t puiLoc, CUniformBuffer *pBuffer) override;
		void SetSubroutines(EShaderType pShaderType, const Array<uint32_t> &pIndices) override;
		void SetMatrix4Array(uint32_t puiLoc, const Array<CMatrix4F> &pMatrices) override;
		void SetFloat4Array(uint32_t puiLoc, float *pArray, uint32_t puiCount) override;

	private:

		EResult PopulateUniforms();
		EResult PopulateAttributes();
		EResult PopulateUnifromBlocks();
		EResult PopulateSubroutines();

		GLuint muiProgramHandle;
		Map<String, GLint> mConstantMap;
		Map<GLenum, Map<String, GLuint>> mSubroutineMap;
	};
}

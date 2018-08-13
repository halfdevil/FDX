
#include "OGL/GLProgramConstants.h"
#include "OS/System.h"
#include "OS/GameLogger.h"
#include "OGL/GLUtil.h"

namespace FDX
{
	CGLProgramConstants::CGLProgramConstants(GLuint puiProgramHandle)
		: muiProgramHandle(puiProgramHandle)
	{
	}

	CGLProgramConstants::~CGLProgramConstants()
	{
		mConstantMap.clear();
	}

	EResult CGLProgramConstants::Populate()
	{
		EResult lResult = EResult::Success;

		lResult = PopulateUniforms();
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Unable to populate unifroms");
		}
		else
		{
			lResult = PopulateAttributes();
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("Unable to populate attributes");
			}
			else
			{
				lResult = PopulateUnifromBlocks();
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("Unable to populate unifrom blocks");
				}
				else
				{
					lResult = PopulateSubroutines();
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("Unable to populate subroutines");
					}
				}
			}
		}

		return lResult;
	}

	EResult CGLProgramConstants::GetConstantLocation(const String & pstrName, uint32_t & puiLoc)
	{
		EResult lResult = EResult::Success;

		auto lIt = mConstantMap.find(pstrName);
		if (lIt != mConstantMap.end())
		{
			puiLoc = (*lIt).second;
		}
		else
		{
			lResult = EResult::NotFound;
		}

		return lResult;
	}

	EResult CGLProgramConstants::GetSubroutineIndex(const String & pstrName, EShaderType pShaderType, uint32_t & puiLoc)
	{
		EResult lResult = EResult::Success;
		GLenum lShaderStage = CGLUtil::GetShaderType(pShaderType);

		auto lIt = mSubroutineMap.find(lShaderStage);
		if (lIt != mSubroutineMap.end())
		{
			auto lIt2 = (*lIt).second.find(pstrName);

			if (lIt2 != (*lIt).second.end())
				puiLoc = (*lIt2).second;
			else
				lResult = EResult::NotFound;
		}
		else
		{
			lResult = EResult::NotFound;
		}

		return lResult;
	}

	EResult CGLProgramConstants::SetInt(const String & pstrName, int piVal)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniform1i(luiLoc, piVal);

		return lResult;
	}

	EResult CGLProgramConstants::SetFloat(const String & pstrName, float pfVal)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniform1f(luiLoc, pfVal);

		return lResult;
	}

	EResult CGLProgramConstants::SetVector2(const String & pstrName, const CVector2F & pVec)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniform2f(luiLoc, pVec.X, pVec.Y);

		return lResult;
	}

	EResult CGLProgramConstants::SetVector3(const String & pstrName, const CVector3F & pVec)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniform3f(luiLoc, pVec.X, pVec.Y, pVec.Z);

		return lResult;
	}

	EResult CGLProgramConstants::SetVector4(const String & pstrName, const CVector4F & pVec)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniform4f(luiLoc, pVec.X, pVec.Y, pVec.Z, pVec.W);

		return lResult;
	}

	EResult CGLProgramConstants::SetColor4(const String & pstrName, const CColor4F & pVec)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniform4f(luiLoc, pVec.R, pVec.G, pVec.B, pVec.A);

		return lResult;
	}

	EResult CGLProgramConstants::SetMatrix3(const String & pstrName, const CMatrix3F & pMat)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniformMatrix3fv(luiLoc, 1, GL_FALSE, pMat.ToPtr());

		return lResult;
	}

	EResult CGLProgramConstants::SetMatrix4(const String & pstrName, const CMatrix4F & pMat)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniformMatrix4fv(luiLoc, 1, GL_FALSE, pMat.ToPtr());

		return lResult;
	}

	EResult CGLProgramConstants::SetTexture(const String & pstrName, uint32_t puiUnit, ITexture * pTexture)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
		{
			pTexture->Activate(puiUnit);
			glUniform1i(luiLoc, puiUnit);
		}			

		return lResult;
	}

	EResult CGLProgramConstants::SetUniformBuffer(const String & pstrName, CUniformBuffer * pBuffer)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniformBlockBinding(muiProgramHandle, luiLoc, pBuffer->GetBindingIndex());

		return lResult;
	}

	EResult CGLProgramConstants::SetMatrix4Array(const String & pstrName, const Array<CMatrix4F>& pMatrices)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)		
			glUniformMatrix4fv(luiLoc, pMatrices.size(), GL_FALSE, pMatrices.data()[0].ToPtr());

		return lResult;
	}

	EResult CGLProgramConstants::SetMatrix4Array(const String & pstrName, const float * pMatrixArray, uint32_t puiCount)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniformMatrix4fv(luiLoc, puiCount, GL_FALSE, pMatrixArray);

		return lResult;
	}

	EResult CGLProgramConstants::SetFloat4Array(const String & pstrName, float * pArray, uint32_t puiCount)
	{
		EResult lResult = EResult::Success;
		uint32_t luiLoc = 0;

		lResult = GetConstantLocation(pstrName, luiLoc);
		if (lResult == EResult::Success)
			glUniform4fv(luiLoc, puiCount, pArray);

		return lResult;
	}

	void CGLProgramConstants::SetInt(int puiLoc, int piVal)
	{
		glUniform1i(puiLoc, piVal);
	}

	void CGLProgramConstants::SetFloat(uint32_t puiLoc, float pfVal)
	{
		glUniform1f(puiLoc, pfVal);
	}

	void CGLProgramConstants::SetVector2(uint32_t puiLoc, const CVector2F & pVec)
	{
		glUniform2f(puiLoc, pVec.X, pVec.Y);
	}

	void CGLProgramConstants::SetVector3(uint32_t puiLoc, const CVector3F & pVec)
	{
		glUniform3f(puiLoc, pVec.X, pVec.Y, pVec.Z);
	}

	void CGLProgramConstants::SetVector4(uint32_t puiLoc, const CVector4F & pVec)
	{
		glUniform4f(puiLoc, pVec.X, pVec.Y, pVec.Z, pVec.W);
	}

	void CGLProgramConstants::SetColor4(uint32_t puiLoc, const CColor4F & pVec)
	{
		glUniform4f(puiLoc, pVec.R, pVec.G, pVec.B, pVec.A);
	}

	void CGLProgramConstants::SetMatrix3(uint32_t puiLoc, const CMatrix3F & pMat)
	{
		glUniformMatrix3fv(puiLoc, 1, GL_FALSE, pMat.ToPtr());
	}

	void CGLProgramConstants::SetMatrix4(uint32_t puiLoc, const CMatrix4F & pMat)
	{
		glUniformMatrix4fv(puiLoc, 1, GL_FALSE, pMat.ToPtr());
	}

	void CGLProgramConstants::SetTexture(uint32_t puiLoc, uint32_t puiUnit, ITexture * pTexture)
	{
		if (pTexture != nullptr)
		{
			pTexture->Activate(puiUnit);
			glUniform1i(puiLoc, puiUnit);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0 + puiUnit);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void CGLProgramConstants::SetUniformBuffer(uint32_t puiLoc, CUniformBuffer * pBuffer)
	{
		glUniformBlockBinding(muiProgramHandle, puiLoc, pBuffer->GetBindingIndex());
	}

	EResult CGLProgramConstants::SetSubroutinesName(EShaderType pShaderType, const Array<String> &pstrSubroutines)
	{
		EResult lResult = EResult::Success;
		GLenum lShaderStage = CGLUtil::GetShaderType(pShaderType);

		if (pstrSubroutines.size() > 0)
		{
			Array<GLuint> lIndices;
			for (const auto &lSubroutine : pstrSubroutines)
				lIndices.push_back(mSubroutineMap[lShaderStage][lSubroutine]);

			glUniformSubroutinesuiv(lShaderStage, lIndices.size(), &lIndices[0]);
		}
		else
		{
			lResult = EResult::NotFound;
		}

		return lResult;
	}

	void CGLProgramConstants::SetSubroutines(EShaderType pShaderType, const Array<uint32_t> &pIndices)
	{
		GLenum lShaderStage = CGLUtil::GetShaderType(pShaderType);
		glUniformSubroutinesuiv(lShaderStage, pIndices.size(), pIndices.data());
	}

	void CGLProgramConstants::SetMatrix4Array(uint32_t puiLoc, const Array<CMatrix4F>& pMatrices)
	{
		glUniformMatrix4fv(puiLoc, pMatrices.size(), GL_FALSE, pMatrices[0].ToPtr());
	}

	void CGLProgramConstants::SetFloat4Array(uint32_t puiLoc, float * pArray, uint32_t puiCount)
	{
		glUniform4fv(puiLoc, puiCount, pArray);
	}

	EResult CGLProgramConstants::PopulateUniforms()
	{
		EResult lResult = EResult::Success;

		GLint liMaxUniformLen = 0;
		GLint liNumUniforms = 0;
		GLchar *lstrUniformName = nullptr;

		glGetProgramiv(muiProgramHandle, GL_ACTIVE_UNIFORMS, &liNumUniforms);
		glGetProgramiv(muiProgramHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &liMaxUniformLen);

		lstrUniformName = new GLchar[liMaxUniformLen];
		for (GLint liIdx = 0; liIdx < liNumUniforms; liIdx++)
		{
			GLint liSize;
			GLenum lType;

			glGetActiveUniform(muiProgramHandle, liIdx,
				liMaxUniformLen, nullptr, &liSize, &lType, lstrUniformName);

			mConstantMap[lstrUniformName] = glGetUniformLocation(muiProgramHandle, lstrUniformName);
		}

		SAFE_DELETE(lstrUniformName);

		return lResult;
	}

	EResult CGLProgramConstants::PopulateAttributes()
	{
		EResult lResult = EResult::Success;

		GLint liMaxAttributeLen = 0;
		GLint liNumAttributes = 0;
		GLchar *lstrAttributeName = nullptr;

		glGetProgramiv(muiProgramHandle, GL_ACTIVE_ATTRIBUTES, &liNumAttributes);
		glGetProgramiv(muiProgramHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &liMaxAttributeLen);

		lstrAttributeName = new GLchar[liMaxAttributeLen];
		for (GLint liIdx = 0; liIdx < liNumAttributes; liIdx++)
		{
			GLint liSize;
			GLenum lType;

			glGetActiveAttrib(muiProgramHandle, liIdx,
				liMaxAttributeLen, nullptr, &liSize, &lType, lstrAttributeName);

			mConstantMap[lstrAttributeName] = glGetAttribLocation(muiProgramHandle, lstrAttributeName);
		}

		SAFE_DELETE(lstrAttributeName);

		return lResult;
	}

	EResult CGLProgramConstants::PopulateUnifromBlocks()
	{
		EResult lResult = EResult::Success;

		GLint liMaxUniformBlocksLen = 0;
		GLint liNumUniformBlocks = 0;
		GLchar *lstrUniformBlockName = nullptr;

		glGetProgramiv(muiProgramHandle, GL_ACTIVE_UNIFORM_BLOCKS, &liNumUniformBlocks);
		glGetProgramiv(muiProgramHandle, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &liMaxUniformBlocksLen);

		lstrUniformBlockName = new GLchar[liMaxUniformBlocksLen];
		for (GLint liIdx = 0; liIdx < liNumUniformBlocks; liIdx++)
		{
			GLsizei liLength;

			glGetActiveUniformBlockName(muiProgramHandle, static_cast<GLuint>(liIdx),
				liMaxUniformBlocksLen, &liLength, lstrUniformBlockName);

			mConstantMap[lstrUniformBlockName] = glGetUniformBlockIndex(muiProgramHandle,
				lstrUniformBlockName);
		}

		SAFE_DELETE(lstrUniformBlockName);

		return lResult;
	}

	EResult CGLProgramConstants::PopulateSubroutines()
	{
		EResult lResult = EResult::Success;


		for (uint32_t lIdx = 0; lIdx <= (uint32_t)EShaderType::Fragment; lIdx++)
		{
			GLenum lShaderStage = CGLUtil::GetShaderType((EShaderType)lIdx);

			GLint liMaxSubroutineUniformLen = 0;
			GLint liNumSubroutineUniforms = 0;			
			GLchar *lstrSubroutineUniformName = nullptr;
			
			glGetProgramStageiv(muiProgramHandle, lShaderStage, 
				GL_ACTIVE_SUBROUTINE_UNIFORMS, &liNumSubroutineUniforms);

			glGetProgramStageiv(muiProgramHandle, lShaderStage, 
				GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH, &liMaxSubroutineUniformLen);

			lstrSubroutineUniformName = new GLchar[liMaxSubroutineUniformLen];

			for (GLint liIdx = 0; liIdx < liNumSubroutineUniforms; liIdx++)
			{
				GLsizei liLength;
				GLint liMaxSubroutineLen = 0;
				GLint liNumSubroutines = 0;
				GLchar *lstrSubroutineName = nullptr;
				GLint *lSubroutineIndices = nullptr;

				glGetActiveSubroutineUniformName(muiProgramHandle, lShaderStage, static_cast<GLuint>(liIdx),
					liMaxSubroutineUniformLen, &liLength, lstrSubroutineUniformName);

				mConstantMap[lstrSubroutineUniformName] = glGetSubroutineUniformLocation(muiProgramHandle,
					lShaderStage, lstrSubroutineUniformName);

				glGetActiveSubroutineUniformiv(muiProgramHandle, lShaderStage, liIdx, 
					GL_NUM_COMPATIBLE_SUBROUTINES, &liNumSubroutines);

				lSubroutineIndices = new GLint[liNumSubroutines];
				glGetActiveSubroutineUniformiv(muiProgramHandle, lShaderStage, liIdx,
					GL_COMPATIBLE_SUBROUTINES, lSubroutineIndices);

				glGetProgramStageiv(muiProgramHandle, lShaderStage,
					GL_ACTIVE_SUBROUTINE_MAX_LENGTH, &liMaxSubroutineLen);

				lstrSubroutineName = new GLchar[liMaxSubroutineLen];
				for (GLint liS = 0; liS < liNumSubroutines; liS++)
				{
					glGetActiveSubroutineName(muiProgramHandle, lShaderStage, lSubroutineIndices[liS], 
						liMaxSubroutineLen, &liLength, lstrSubroutineName);

					mSubroutineMap[lShaderStage][lstrSubroutineName] = 
						glGetSubroutineIndex(muiProgramHandle, lShaderStage, lstrSubroutineName);
				}

				SAFE_DELETE(lSubroutineIndices);
				SAFE_DELETE(lstrSubroutineName);
			}

			SAFE_DELETE(lstrSubroutineUniformName);
		}

		return lResult;
	}
}
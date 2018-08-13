#pragma once

#include "Scene/SceneEntity.h"
#include "Scene/Material.h"
#include "Graphics/DrawOperation.h"
#include "Math/Frustum.h"
#include "Scene/Camera.h"

namespace FDX
{
	enum class ERenderableType
	{
		Normal,
		Mesh
	};

	class FDX_EXPORT CRenderable : public CSceneEntity
	{
	public:

		CRenderable();
		virtual ~CRenderable();

		CRenderable(const CRenderable&) = delete;
		CRenderable& operator=(const CRenderable&) = delete;

		ERenderableType GetRenderableType() const
		{
			return mRenderableType;
		}

		bool IsRendered() const
		{
			return mbRendered;
		}

		uint32_t GetRenderCount() const
		{
			return muiRenderCount;
		}

		IMaterial* GetMaterial() const
		{
			return mMaterial;
		}

		IDrawOperation* GetDrawOperation() const
		{
			return mDrawOperation;
		}

		CMatrix4F* GetGlobalTransformPtr()
		{
			if (!mbTransformUpdated)
				UpdateTransform();

			return &mGlobalTransform;
		}

		void SetRendered(bool pbRendered);
		void SetRenderCount(uint32_t puiCount);		
		bool CollidesWithAabb(const CAxisAlignedBox &pAabb);
		bool CollidesWithFrustum(const CFrustum &pFrustum);		

		virtual void UpdateGraphics(CCamera *pCamera, IProgram *pProgram);

	protected:

		bool mbRendered;
		ERenderableType mRenderableType;
		uint32_t muiRenderCount;
		IMaterial *mMaterial;
		IDrawOperation *mDrawOperation;
	};
}
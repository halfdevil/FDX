#pragma once

#include "Game/Win32GameFramework.h"
#include "Graphics/QuadPostProcessor.h"
#include "Scene/FreeCamera.h"
#include "Scene/Light.h"
#include "Scene/RenderQueue.h"
#include "Scene/MeshEntity.h"
#include "Scene/SkyBoxEntity.h"
#include "Scene/BasicSceneTraverser.h"

using namespace FDX;

class CModelViewer : public CWin32GameFramework
{ 
public:

	EResult OnLoadContent() override;
	EResult OnUpdate() override;
	EResult OnDraw() override;

private:

	CMeshEntityPtr LoadModel(const String &pstrModel);

private:

	CMeshEntityPtr mDude;
	CMeshEntityPtr mGround;
	CSkyBoxEntityPtr mSkyBox;
	CRenderQueuePtr mRenderQueue;
	CFreeCameraPtr mFreeCamera;
	CPointLightPtr mPointLight1;
	CPointLightPtr mPointLight2;
	ISceneTraverserPtr mSceneTraverser;
	CQuadPostProcessorPtr mQuadPostProcessor;
	IProgram *mPostProcessorProgram;
	SViewport mViewport;
};
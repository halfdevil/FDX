cmake_minimum_required(VERSION 3.10)
project(Engine)

set(CMAKE_CXX_STANDARD 14)

#ThirdParty include directories

include_directories(../ThirdParty/FreeImage/dist)
include_directories(../ThirdParty/freetype-2.8.1/include)
include_directories(../ThirdParty/gl3w-dist/include)
include_directories(../ThirdParty/glew-2.1.0/include)
include_directories(../ThirdParty/glfw-3.2.1/include)
include_directories(../ThirdParty/tinyxml2)
include_directories(../ThirdParty/OpenAL\ 1.1\ SDK/include)

#ThirdParty library directories

link_directories(../ThirdParty/FreeImage/Dist)
link_directories(../ThirdParty/freetype-2.8.1/bin)
link_directories(../ThirdParty/gl3w-dist/bin)
link_directories(../ThirdParty/glfw-3.2.1/bin)
link_directories(../ThirdParty/tinyxml2/bin)

if (WIN32)
    link_directories(../ThirdParty/OpenAL\ 1.1\ SDK/libs/Win64)
endif()

#Local include directories

include_directories(Include)
include_directories(Include/Audio)
include_directories(Include/Core)
include_directories(Include/FS)
include_directories(Include/Game)
include_directories(Include/Graphics)
include_directories(Include/GUI)
include_directories(Include/Input)
include_directories(Include/Math)
include_directories(Include/OAL)
include_directories(Include/OGL)
include_directories(Include/OS)
include_directories(Include/Physics)
include_directories(Include/Scene)

set(CORE_HEADER_FILES
        Include/Core/BinaryResource.h
        Include/Core/BinaryResourceFactory.h
        Include/Core/Bitmap.h
        Include/Core/Core.h
        Include/Core/DaeModelLoader.h
        Include/Core/DateTime.h
        Include/Core/Event.h
        Include/Core/EventHandler.h
        Include/Core/EventManager.h
        Include/Core/EzmModelLoader.h
        Include/Core/Factory.h
        Include/Core/FdmModelLoader.h
        Include/Core/FdmModelStructs.h
        Include/Core/Font.h
        Include/Core/FontLoader.h
        Include/Core/FontResource.h
        Include/Core/FontResourceFactory.h
        Include/Core/FreeImageLoader.h
        Include/Core/FreeTypeLoader.h
        Include/Core/Image.h
        Include/Core/ImageLoader.h
        Include/Core/ImageResource.h
        Include/Core/ImageResourceFactory.h
        Include/Core/Logger.h
        Include/Core/Model.h
        Include/Core/ModelLoader.h
        Include/Core/ModelResource.h
        Include/Core/ModelResourceFactory.h
        Include/Core/ObjectPool.h
        Include/Core/ObjModelLoader.h
        Include/Core/Resource.h
        Include/Core/ResourceFactory.h
        Include/Core/ResourceManager.h
        Include/Core/Result.h
        Include/Core/Singleton.h
        Include/Core/StringUtil.h
        Include/Core/XmlResource.h
        Include/Core/XmlResourceFactory.h)

set(AUDIO_HEADER_FILES
        Include/Audio/AudioDataProvider.h
        Include/Audio/AudioDevice.h
        Include/Audio/AudioSource.h)

set(FS_HEADER_FILES
        Include/FS/Archive.h
        Include/FS/Directory.h
        Include/FS/FileStream.h
        Include/FS/FileSystem.h
        Include/FS/MemoryStream.h
        Include/FS/MountPoint.h
        Include/FS/StreamReader.h
        Include/FS/StreamWriter.h)

set(GAME_HEADER_FILES
        Include/Game/OSXGameFramework.h)

set(GLFW_HEADER_FILES
        Include/GLFW/GLFWGameWindow.h
        Include/GLFW/GLFWGraphicsContext.h
        Include/GLFW/GLFWKeyboard.h
        Include/GLFW/GLFWUtil.h
        Include/GLFW/GLFWMouse.h)

set(GRAPHICS_HEADER_FILES
        Include/Graphics/BatchRenderer.h
        Include/Graphics/Buffer.h
        Include/Graphics/Canvas.h
        Include/Graphics/CanvasMaterial.h
        Include/Graphics/DrawOperation.h
        Include/Graphics/GraphicsContext.h
        Include/Graphics/GraphicsDevice.h
        Include/Graphics/IndexBuffer.h
        Include/Graphics/OcclusionQuery.h
        Include/Graphics/Program.h
        Include/Graphics/ProgramConstants.h
        Include/Graphics/ProgramManager.h
        Include/Graphics/RenderData.h
        Include/Graphics/RenderState.h
        Include/Graphics/RenderTarget.h
        Include/Graphics/SpriteBatch.h
        Include/Graphics/SpriteBatchMaterial.h
        Include/Graphics/Texture.h
        Include/Graphics/Texture2D.h
        Include/Graphics/Texture2DArray.h
        Include/Graphics/TextureCube.h
        Include/Graphics/TextureManager.h
        Include/Graphics/UniformBuffer.h
        Include/Graphics/VertexBuffer.h
        Include/Graphics/VertexCache.h
        Include/Graphics/VertexCacheManager.h
        Include/Graphics/VertexFormat.h
        Include/Graphics/VertexTypes.h)

set(GUI_HEADER_FILES
        Include/GUI/Button.h
        Include/GUI/CheckBox.h
        Include/GUI/Control.h
        Include/GUI/ControlAnimation.h
        Include/GUI/ControlBorder.h
        Include/GUI/ControlContainer.h
        Include/GUI/ControlContainerLoader.h
        Include/GUI/ControlEvent.h
        Include/GUI/ControlEventHandler.h
        Include/GUI/ControlFont.h
        Include/GUI/ControlMaterial.h
        Include/GUI/ControlSkin.h
        Include/GUI/Cursor.h
        Include/GUI/DefaultUITheme.h
        Include/GUI/Dialog.h
        Include/GUI/ImageControl.h
        Include/GUI/Label.h
        Include/GUI/LayoutController.h
        Include/GUI/Menu.h
        Include/GUI/Panel.h
        Include/GUI/RadioButton.h
        Include/GUI/RadioButtonGroup.h
        Include/GUI/TextBox.h
        Include/GUI/UIManager.h
        Include/GUI/UITheme.h
        Include/GUI/XmlButtonLoader.h
        Include/GUI/XmlControlAnimationLoader.h
        Include/GUI/XmlControlContainerLoader.h
        Include/GUI/XmlControlLoader.h
        Include/GUI/XmlControlLoaderFactory.h
        Include/GUI/XmlControlMaterialLoader.h
        Include/GUI/XmlControlSkinLoader.h
        Include/GUI/XmlImageControlLoader.h
        Include/GUI/XmlLabelLoader.h
        Include/GUI/XmlMenuLoader.h
        Include/GUI/XmlPanelLoader.h)

set(INPUT_HEADER_FILES
        Include/Input/InputDevice.h
        Include/Input/InputEvent.h
        Include/Input/InputEventHandler.h
        Include/Input/Keyboard.h
        Include/Input/Mouse.h)

set(MATH_HEADER_FILES
        Include/Math/AxisAlignedBox.h
        Include/Math/Color4F.h
        Include/Math/DualQuaternion.h
        Include/Math/Frustum.h
        Include/Math/Line.h
        Include/Math/MathUtil.h
        Include/Math/Matrix3F.h
        Include/Math/Matrix4F.h
        Include/Math/Plane.h
        Include/Math/Quaternion.h
        Include/Math/Vector2F.h
        Include/Math/Vector3F.h
        Include/Math/Vector4F.h)

set(OAL_HEADER_FILES
        Include/OAL/ALAudioDevice.h
        Include/OAL/ALAudioSource.h
        Include/OAL/ALAudioThread.h
        Include/OAL/ALUtil.h)

set(OGL_HEADER_FILES
        Include/OGL/GLDrawOperation.h
        Include/OGL/GLGraphicsDevice.h
        Include/OGL/GLIndexBuffer.h
        Include/OGL/GLOcclusionQuery.h
        Include/OGL/GLProgram.h
        Include/OGL/GLProgramConstants.h
        Include/OGL/GLRenderData.h
        Include/OGL/GLRenderState.h
        Include/OGL/GLRenderTarget.h
        Include/OGL/GLTexture2D.h
        Include/OGL/GLTexture2DArray.h
        Include/OGL/GLTextureCube.h
        Include/OGL/GLUtil.h
        Include/OGL/GLVertexBuffer.h)

set(OS_HEADER_FILES
        Include/OS/AsyncQueue.h
        Include/OS/GameLogger.h
        Include/OS/GameTimer.h
        Include/OS/GameWindow.h
        Include/OS/Library.h
        Include/OS/System.h
        Include/OS/Task.h
        Include/OS/Thread.h
        Include/OS/WorkerThread.h)

if (WIN32)
    set(OS_HEADER_FILES
            ${OS_HEADER_FILES}
            Include/OS/Win32/Win32Directory.h)
elseif(APPLE)
    set(OS_HEADER_FILES
            ${OS_HEADER_FILES}
            Include/OS/OSX/OSXDirectory.h
            Include/OS/OSX/OSXFile.h
            Include/OS/OSX/OSXLibrary.h
            Include/OS/OSX/OSXGameTimer.h
            Include/OS/OSX/OSXSystem.h
            Include/OS/OSX/OSXFileSystem.h)
endif()

set(PHYSICS_HEADER_FILES
        Include/Physics/Particle.h
        Include/Physics/ParticleContact.h
        Include/Physics/ParticleContactResolver.h
        Include/Physics/ParticleForce.h
        Include/Physics/ParticleForceRegistry.h
        Include/Physics/PhysicsUtil.h
        Include/Physics/RigidBody.h
        Include/Physics/RigidBodyContact.h)

set(SCENE_HEADER_FILES
        Include/Scene/AnimationPlayer.h
        Include/Scene/BaseMaterial.h
        Include/Scene/BasicSceneTraverser.h
        Include/Scene/Camera.h
        Include/Scene/Component.h
        Include/Scene/DebugRenderer.h
        Include/Scene/FreeCamera.h
        Include/Scene/Light.h
        Include/Scene/Material.h
        Include/Scene/MeshEntity.h
        Include/Scene/MeshMaterial.h
        Include/Scene/Renderable.h
        Include/Scene/RenderNode.h
        Include/Scene/RenderNodeState.h
        Include/Scene/RenderQueue.h
        Include/Scene/SceneCuller.h
        Include/Scene/SceneEntity.h
        Include/Scene/SceneNode.h
        Include/Scene/SceneTraverser.h
        Include/Scene/SkyBoxEntity.h
        Include/Scene/SubMeshEntity.h
        Include/Scene/TerrainEntity.h
        Include/Scene/TerrainHeightMap.h
        Include/Scene/TerrainMaterial.h
        Include/Scene/TerrainTraverser.h)

set(CORE_SOURCE_FILES
        Source/Core/BinaryResource.cpp
        Source/Core/BinaryResourceFactory.cpp
        Source/Core/Bitmap.cpp
        Source/Core/DaeModelLoader.cpp
        Source/Core/DateTime.cpp
        Source/Core/EventManager.cpp
        Source/Core/EzmModelLoader.cpp
        Source/Core/FdmModelLoader.cpp
        Source/Core/FontResource.cpp
        Source/Core/FontResourceFactory.cpp
        Source/Core/FreeImageLoader.cpp
        Source/Core/FreeTypeLoader.cpp
        Source/Core/ImageResource.cpp
        Source/Core/ImageResourceFactory.cpp
        Source/Core/Logger.cpp
        Source/Core/ModelResource.cpp
        Source/Core/ModelResourceFactory.cpp
        Source/Core/ObjModelLoader.cpp
        Source/Core/ResourceManager.cpp
        Source/Core/Result.cpp
        Source/Core/StringUtil.cpp
        Source/Core/XmlResource.cpp
        Source/Core/XmlResourceFactory.cpp)

set(AUDIO_SOURCE_FILES
        Source/Audio/AudioSource.cpp)

set(FS_SOURCE_FILES
        Source/FS/Archive.cpp
        Source/FS/Directory.cpp
        Source/FS/MemoryStream.cpp
        Source/FS/StreamReader.cpp
        Source/FS/StreamWriter.cpp)

set(GAME_SOURCE_FILES
        Source/Game/OSXGameFramework.cpp)

set(GLFW_SOURCE_FILES
        Source/GLFW/GLFWGameWindow.cpp
        Source/GLFW/GLFWGraphicsContext.cpp
        Source/GLFW/GLFWKeyboard.cpp
        Source/GLFW/GLFWUtil.cpp
        Source/GLFW/GLFWMouse.cpp)

set(GRAPHICS_SOURCE_FILES
        Source/Graphics/BatchRenderer.cpp
        Source/Graphics/Canvas.cpp
        Source/Graphics/CanvasMaterial.cpp
        Source/Graphics/ProgramManager.cpp
        Source/Graphics/SpriteBatch.cpp
        Source/Graphics/SpriteBatchMaterial.cpp
        Source/Graphics/TextureManager.cpp
        Source/Graphics/VertexCache.cpp
        Source/Graphics/VertexCacheManager.cpp
        Source/Graphics/VertexFormat.cpp
        Source/Graphics/VertexTypes.cpp)

set(GUI_SOURCE_FILES
        Source/GUI/Button.cpp
        Source/GUI/CheckBox.cpp
        Source/GUI/Control.cpp
        Source/GUI/ControlAnimation.cpp
        Source/GUI/ControlBorder.cpp
        Source/GUI/ControlContainer.cpp
        Source/GUI/ControlEvent.cpp
        Source/GUI/ControlEventHandler.cpp
        Source/GUI/ControlFont.cpp
        Source/GUI/ControlMaterial.cpp
        Source/GUI/ControlSkin.cpp
        Source/GUI/Cursor.cpp
        Source/GUI/DefaultUITheme.cpp
        Source/GUI/Dialog.cpp
        Source/GUI/ImageControl.cpp
        Source/GUI/Label.cpp
        Source/GUI/LayoutController.cpp
        Source/GUI/Menu.cpp
        Source/GUI/Panel.cpp
        Source/GUI/RadioButton.cpp
        Source/GUI/RadioButtonGroup.cpp
        Source/GUI/TextBox.cpp
        Source/GUI/UIManager.cpp
        Source/GUI/XmlButtonLoader.cpp
        Source/GUI/XmlControlAnimationLoader.cpp
        Source/GUI/XmlControlContainerLoader.cpp
        Source/GUI/XmlControlLoader.cpp
        Source/GUI/XmlControlLoaderFactory.cpp
        Source/GUI/XmlControlMaterialLoader.cpp
        Source/GUI/XmlControlSkinLoader.cpp
        Source/GUI/XmlImageViewLoader.cpp
        Source/GUI/XmlLabelLoader.cpp
        Source/GUI/XmlMenuLoader.cpp
        Source/GUI/XmlPanelLoader.cpp)

set(INPUT_SOURCE_FILES
        Source/Input/InputEvent.cpp
        Source/Input/InputEventHandler.cpp)

set(MATH_SOURCE_FILES
        Source/Math/AxisAlignedBox.cpp
        Source/Math/Color4F.cpp
        Source/Math/DualQuaternion.cpp
        Source/Math/Frustum.cpp
        Source/Math/Line.cpp
        Source/Math/Matrix3F.cpp
        Source/Math/Matrix4F.cpp
        Source/Math/Plane.cpp
        Source/Math/Quaternion.cpp
        Source/Math/Vector2F.cpp
        Source/Math/Vector3F.cpp
        Source/Math/Vector4F.cpp)

set(OAL_SOURCE_FILES
        Source/OAL/ALAudioDevice.cpp
        Source/OAL/ALAudioSource.cpp
        Source/OAL/ALAudioThread.cpp
        Source/OAL/ALUtil.cpp)

set(OGL_SOURCE_FILES
        Source/OGL/GLDrawOperation.cpp
        Source/OGL/GLGraphicsDevice.cpp
        Source/OGL/GLIndexBuffer.cpp
        Source/OGL/GLOcclusionQuery.cpp
        Source/OGL/GLProgram.cpp
        Source/OGL/GLProgramConstants.cpp
        Source/OGL/GLRenderData.cpp
        Source/OGL/GLRenderState.cpp
        Source/OGL/GLRenderTarget.cpp
        Source/OGL/GLTexture2D.cpp
        Source/OGL/GLTexture2DArray.cpp
        Source/OGL/GLTextureCube.cpp
        Source/OGL/GLUtil.cpp
        Source/OGL/GLVertexBuffer.cpp)

set(OS_SOURCE_FILES
        Source/OS/AsyncQueue.cpp
        Source/OS/GameLogger.cpp
        Source/OS/Thread.cpp
        Source/OS/WorkerThread.cpp)

if (WIN32)
    set(OS_SOURCE_FILES
            ${OS_SOURCE_FILES}
            Source/OS/Win32/Win32Directory.cpp)
elseif(APPLE)
    set(OS_SOURCE_FILES
            ${OS_SOURCE_FILES}
            Source/OS/OSX/OSXDirectory.cpp
            Source/OS/OSX/OSXFile.cpp
            Source/OS/OSX/OSXLibrary.cpp
            Source/OS/OSX/OSXGameTimer.cpp
            Source/OS/OSX/OSXSystem.cpp
            Source/OS/OSX/OSXFileSystem.cpp)
endif()

set(PHYSICS_SOURCE_FILES
        Source/Physics/Particle.cpp
        Source/Physics/ParticleContact.cpp
        Source/Physics/ParticleContactResolver.cpp
        Source/Physics/ParticleForce.cpp
        Source/Physics/ParticleForceRegistry.cpp
        Source/Physics/PhysicsUtil.cpp
        Source/Physics/RigidBody.cpp
        Source/Physics/RigidBodyContact.cpp)

set(SCENE_SOURCE_FILES
        Source/Scene/AnimationPlayer.cpp
        Source/Scene/BaseMaterial.cpp
        Source/Scene/BasicSceneTraverser.cpp
        Source/Scene/Camera.cpp
        Source/Scene/DebugRenderer.cpp
        Source/Scene/FreeCamera.cpp
        Source/Scene/Light.cpp
        Source/Scene/MeshEntity.cpp
        Source/Scene/MeshMaterial.cpp
        Source/Scene/Renderable.cpp
        Source/Scene/RenderNode.cpp
        Source/Scene/RenderNodeState.cpp
        Source/Scene/RenderQueue.cpp
        Source/Scene/SceneEntity.cpp
        Source/Scene/SceneNode.cpp
        Source/Scene/SkyBoxEntity.cpp
        Source/Scene/SubMeshEntity.cpp
        Source/Scene/TerrainEntity.cpp
        Source/Scene/TerrainHeightMap.cpp
        Source/Scene/TerrainMaterial.cpp
        Source/Scene/TerrainTraverser.cpp)

set(HEADER_FILES
        ${CORE_HEADER_FILES}
        ${AUDIO_HEADER_FILES}
        ${FS_HEADER_FILES}
        ${GAME_HEADER_FILES}
        ${GLFW_HEADER_FILES}
        ${GRAPHICS_HEADER_FILES}
        ${GUI_HEADER_FILES}
        ${INPUT_HEADER_FILES}
        ${MATH_HEADER_FILES}
        ${OAL_HEADER_FILES}
        ${OGL_HEADER_FILES}
        ${OS_HEADER_FILES}
        ${PHYSICS_HEADER_FILES}
        ${SCENE_HEADER_FILES})

set(SOURCE_FILES
        ${CORE_SOURCE_FILES}
        ${AUDIO_SOURCE_FILES}
        ${FS_SOURCE_FILES}
        ${GAME_SOURCE_FILES}
        ${GLFW_SOURCE_FILES}
        ${GRAPHICS_SOURCE_FILES}
        ${GUI_SOURCE_FILES}
        ${INPUT_SOURCE_FILES}
        ${MATH_SOURCE_FILES}
        ${OAL_SOURCE_FILES}
        ${OGL_SOURCE_FILES}
        ${OS_SOURCE_FILES}
        ${PHYSICS_SOURCE_FILES}
        ${SCENE_SOURCE_FILES})

add_definitions(-D__macOS__ -D_DEBUG)
add_library(${PROJECT_NAME} STATIC
        ${HEADER_FILES}
        ${SOURCE_FILES})

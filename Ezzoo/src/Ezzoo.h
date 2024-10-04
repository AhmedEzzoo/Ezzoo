#pragma once
#include "Ezzoo/Core/Application.h"
#ifdef EZZOO_RELEASE
	//#include "Ezzoo/Core/EntryPoint.h"
#endif

#include "Ezzoo/Core/Instrumentor.h"

#include "Ezzoo/Events/Event.h"

#include "Ezzoo/Core/Core.h"

#include "Ezzoo/Core/Log.h"
#include "Ezzoo/Core/Input.h"

#include "Ezzoo/Renderer/OrthgraphicCameraController.h"


#include "Ezzoo/Renderer/Shader.h"
#include "Ezzoo/Renderer/Buffers.h"
#include "Ezzoo/Renderer/RendererCommand.h"
#include "Ezzoo/Renderer/Renderer.h"
#include "Ezzoo/Renderer/Renderer2D.h"
#include "Ezzoo/Renderer/VertexArray.h"
#include "Ezzoo/Renderer/Texture.h"
#include "Ezzoo/Renderer/FrameBuffer.h"

#include "Scene/Entity.h"
#include "Scene/Component.h"
#include "Scene/ScriptableEntity.h"
#include "Scene/SceneSerialize.h"

#include "Ezzoo/IamGui/ImGuiLayer.h"

#include "Ezzoo/Math/Math.h"



#include "ModelComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/Application.h>
#include <library/Mesh.h>
#include <library/VertexTypes.h>
#include <library/Utils.h>
#include <library/Exception.h>

#include <d3dx11effect.h>
#include <d3dcompiler.h>

namespace demo
{
	using namespace library;

	ModelComponent::ModelComponent(
		const Application& app,
		const components::CameraComponent& camera,
		const components::KeyboardComponent& keyboard
	)
		: Class(app, camera)
		, m_keyboard(keyboard)
		, m_indicesCount(0)
	{
	}

	void ModelComponent::Initialize()
	{
	}

	void ModelComponent::Update(const Time& time)
	{
	}

	void ModelComponent::Draw(const Time& time)
	{
	}

} // namespace demo

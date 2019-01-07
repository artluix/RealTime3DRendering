#include "components/BasicMaterialComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/materials/BasicMaterial.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectPass.h>
#include <library/effect/EffectTechnique.h>
#include <library/effect/EffectVariable.h>

#include <library/Application.h>
#include <library/Model.h>
#include <library/Mesh.h>
#include <library/Utils.h>
#include <library/Exception.h>

#include <d3dx11effect.h>
#include <d3dcompiler.h>

namespace demo
{
	using namespace library;
	
	namespace
	{
		constexpr float k_rotationAngle = math::constants::Pi_Div_2;
		constexpr float k_movementRate = 0.01f;

		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			fs::Path("../data/effects/BasicEffect_d.fxc")
#else
			fs::Path("../data/effects/BasicEffect.fxc")
#endif
		);
		const auto k_modelPath = utils::GetExecutableDirectory().Join(
			fs::Path("../data/models/Sphere.obj")
		);
	}

	BasicMaterialComponent::BasicMaterialComponent(
		const Application& app,
		const CameraComponent& camera,
		const KeyboardComponent& keyboard
	) 
		: SceneComponent(app, camera)
		, m_keyboard(keyboard)
		, m_indicesCount(0)
	{
	}

	void BasicMaterialComponent::Initialize()
	{
		// Load the model
		Model model(m_app, k_modelPath, true);

		// create effect
		m_effect = std::make_unique<library::Effect>(m_app);
		m_effect->LoadCompiledEffect(k_effectPath);

		// create material
		m_material = std::make_unique<BasicMaterial>(*m_effect);
		m_material->Initialize();

		// Create the vertex and index buffers
		const auto& mesh = model.GetMesh(0);
		m_vertexBuffer = m_material->CreateVertexBuffer(m_app.GetD3DDevice(), mesh);
		m_indexBuffer = mesh.CreateIndexBuffer();
		m_indicesCount = mesh.GetIndicesCount();
	}

	void BasicMaterialComponent::Update(const Time& time)
	{
		const KeyboardComponent& keyboard = m_keyboard;

		// rotation
		if (keyboard.IsKeyDown(Key::R))
		{
			const auto rotationDelta = k_rotationAngle * time.elapsed.GetSeconds<float>();
			Rotate(rotationDelta);
		}

		// movement
		{
			math::Vector2 movementAmount;
			if (keyboard.IsKeyDown(Key::Up))
			{
				movementAmount.y += 1.0f;
			}

			if (keyboard.IsKeyDown(Key::Down))
			{
				movementAmount.y -= 1.0f;
			}

			if (keyboard.IsKeyDown(Key::Left))
			{
				movementAmount.x -= 1.0f;
			}

			if (keyboard.IsKeyDown(Key::Right))
			{
				movementAmount.x += 1.0f;
			}

			if (movementAmount)
			{
				Translate(math::Vector3(movementAmount * k_movementRate));
			}
		}
	}

	void BasicMaterialComponent::Draw(const Time& time)
	{
		auto d3dDeviceContext = m_app.GetD3DDeviceContext();
		d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// input layout
		auto& pass = m_material->GetCurrentTechnique().GetPass(0);
		auto inputLayout = m_material->GetInputLayout(pass);
		d3dDeviceContext->IASetInputLayout(inputLayout);

		// vertex & index buffers
		unsigned stride = m_material->GetVertexSize();
		unsigned offset = 0;
		d3dDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		d3dDeviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		const auto wvp = m_worldMatrix * GetCamera().GetViewProjectionMatrix();
		m_material->GetWorldViewProjection() << wvp;

		pass.Apply(0, d3dDeviceContext);

		d3dDeviceContext->DrawIndexed(m_indicesCount, 0, 0);
	}

} // namespace demo

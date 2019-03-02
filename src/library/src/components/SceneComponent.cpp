#include "StdAfx.h"
#include "library/components/SceneComponent.h"

#include "library/Application.h"
#include "library/Renderer.h"

#include "library/Model.h"
#include "library/Mesh.h"

#include "library/materials/Material.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectPass.h"

#include <cassert>

namespace library
{
	SceneComponent::SceneComponent()
		: m_scaling(math::Vector3::One)
		, m_worldMatrix(math::Matrix4::Identity)
		, m_direction(math::Vector3::Forward)
		, m_up(math::Vector3::Up)
		, m_right(math::Vector3::Right)
	{
	}

	//-------------------------------------------------------------------------

	void SceneComponent::SetPosition(const math::Vector3& position)
	{
		if (m_position != position)
		{
			m_position = position;
			m_isWorldMatrixDirty = true;
		}
	}

	void SceneComponent::Translate(const math::Vector3& translation)
	{
		if (!translation)
			return;

		SetPosition(m_position + translation);
	}

	//-------------------------------------------------------------------------

	void SceneComponent::SetRotation(const math::Vector3& rotation)
	{
		if (m_rotation != rotation)
		{
			m_rotation = rotation;
			m_isWorldMatrixDirty = true;
		}
	}

	void SceneComponent::Rotate(const math::Vector3& rotation)
	{
		if (!rotation)
			return;

		SetRotation(m_rotation + rotation);
	}

	//-------------------------------------------------------------------------

	void SceneComponent::SetScaling(const math::Vector3& scaling)
	{
		if (scaling < math::Vector3::Zero)
			return;

		if (m_scaling != scaling)
		{
			m_scaling = scaling;
			m_isWorldMatrixDirty = true;
		}
	}

	void SceneComponent::Scale(const math::Vector3& scaling)
	{
		if (!scaling)
			return;

		SetScaling(m_scaling + scaling);
	}

	//-------------------------------------------------------------------------

	void SceneComponent::UpdateWorldMatrix()
	{
		if (!m_isWorldMatrixDirty)
			return;

		const auto translationMatrix = math::Matrix4::Translation(m_position);
		const auto rotationMatrix = math::Matrix4::RotationPitchYawRoll(m_rotation);
		const auto scalingMatrix = math::Matrix4::Scaling(m_scaling);

		m_direction = rotationMatrix.GetForward();
		m_up = rotationMatrix.GetUp();
		m_right = rotationMatrix.GetRight();

		m_worldMatrix = scalingMatrix * rotationMatrix * translationMatrix;

		m_direction = rotationMatrix.GetForward();
		m_up = rotationMatrix.GetUp();
		m_right = rotationMatrix.GetRight();

		m_isWorldMatrixDirty = false;
	}

	//-------------------------------------------------------------------------

	void SceneComponent::SetModelName(const std::string& modelName)
	{
		m_modelName = modelName;
	}

	//-------------------------------------------------------------------------

	void SceneComponent::SetTextureName(const std::string& textureName)
	{
		m_textureName = textureName;
	}

	//-------------------------------------------------------------------------

	void SceneComponent::Initialize(const Application& app)
	{
		DrawableComponent::Initialize(app);

		if (const auto material = GetMaterial())
		{
			assert(material->IsInitialized());

			// set default layout from material
			const auto& currentTechnique = material->GetCurrentTechnique();
			const auto& pass = currentTechnique.GetPass(0);
			m_input.layout = material->GetInputLayoutShared(pass);

			// load model from file
			if (!m_modelName.empty())
			{
				Model model(*GetApp(), m_modelName, true);
				const auto& mesh = model.GetMesh(0);

				m_input.vertices.buffer = material->CreateVertexBuffer(GetApp()->GetDevice(), mesh);
				m_input.vertices.count = mesh.GetVerticesCount();

				if (mesh.HasIndices())
				{
					m_input.indices = std::make_optional(BufferData {
						mesh.CreateIndexBuffer(),
						mesh.GetIndicesCount()
					});
				}
			}
		}

		// load texture
		if (!m_textureName.empty())
		{
			m_texture = GetApp()->LoadTexture(m_textureName);
		}
	}

	void SceneComponent::Update(const Time& time)
	{
		UpdateWorldMatrix();

		GetApp()->GetRenderer()->AddDrawable(*this);
	}

	void SceneComponent::Draw(const Time& time)
	{
		Draw_SetIA();
		Draw_SetData();
		Draw_Render();
	}

	//-------------------------------------------------------------------------

	void SceneComponent::Draw_SetIA()
	{
		auto deviceContext = GetApp()->GetDeviceContext();

		deviceContext->IASetPrimitiveTopology(m_input.topology);
		deviceContext->IASetInputLayout(m_input.layout.Get());

		// set vertex buffer
		{
			unsigned stride = GetVertexSize();
			unsigned offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, m_input.vertices.buffer.GetAddressOf(), &stride, &offset);
		}

		// set index buffer (if needed)
		if (!!m_input.indices)
		{
			deviceContext->IASetIndexBuffer(m_input.indices->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		}
	}

	void SceneComponent::Draw_SetData()
	{
		if (const auto material = GetMaterial())
		{
			auto& currentTechnique = material->GetCurrentTechnique();
			auto& pass = currentTechnique.GetPass(0);

			pass.Apply(0, GetApp()->GetDeviceContext());
		}
	}

	void SceneComponent::Draw_Render()
	{
		auto deviceContext = GetApp()->GetDeviceContext();

		if (!!m_input.indices)
			deviceContext->DrawIndexed(m_input.indices->count, 0, 0);
		else
			deviceContext->Draw(m_input.vertices.count, 0);
	}
} // namespace library

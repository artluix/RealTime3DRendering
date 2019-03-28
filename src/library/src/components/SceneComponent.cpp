#include "StdAfx.h"
#include "library/Components/SceneComponent.h"

#include "library/Application.h"
#include "library/Renderer.h"

#include "library/Model/Mesh.h"
#include "library/Model/Model.h"

#include "library/Materials/Material.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectPass.h"

#include <cassert>

namespace library
{
	SceneComponent::SceneComponent()
		: m_scaling(math::Vector3::One)
		, m_worldMatrix(math::Matrix4::Identity)
		, m_initialTransformMatrix(math::Matrix4::Identity)
		, m_direction(math::Vector3::Forward)
		, m_up(math::Vector3::Up)
		, m_right(math::Vector3::Right)
	{
	}

	SceneComponent::~SceneComponent() = default;

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

	void SceneComponent::SetInitialTransform(const math::Matrix4& initialTransform)
	{
		m_initialTransformMatrix = initialTransform;
		m_isWorldMatrixDirty = true;
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

		m_worldMatrix = m_initialTransformMatrix * (scalingMatrix * rotationMatrix * translationMatrix);

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
			m_inputLayout = material->GetInputLayoutShared(pass);

			// load model from file
			if (!m_modelName.empty())
			{
				m_model = std::make_unique<Model>(*GetApp(), m_modelName, true);

				if (m_model->HasMeshes())
				{
					m_meshesData.clear();

					const auto meshesCount = m_model->GetMeshesCount();
					m_meshesData.reserve(meshesCount);

					for (unsigned i = 0; i < meshesCount; i++)
					{
						MeshData md;

						const auto& mesh = m_model->GetMesh(i);

						md.vertexBuffer.buffer = material->CreateVertexBuffer(GetApp()->GetDevice(), mesh);
						md.vertexBuffer.elementsCount = mesh.GetVerticesCount();

						if (mesh.HasIndices())
						{
							md.indexBuffer = std::make_optional(BufferData{
								mesh.CreateIndexBuffer(),
								mesh.GetIndicesCount()
							});
						}

						// load texture from model if not specified other texture
						if (m_textureName.empty())
						{
							const auto& modelMaterial = mesh.GetMaterial();
							if (modelMaterial.HasTextureNames(m_textureType))
							{
								const auto& textureNames = modelMaterial.GetTextureNames(m_textureType);
								assert(!textureNames.empty());

								Path texturePath(textureNames.front());
								md.texture = GetApp()->LoadTexture(
									texturePath.GetBaseName().SplitExt().first.GetString()
								);
							}
						}

						m_meshesData.push_back(std::move(md));
					}
				}
			}
		}

		// load texture not from model
		if (!m_textureName.empty())
		{
			assert(m_meshesData.size() == 1);
			m_meshesData[0].texture = GetApp()->LoadTexture(m_textureName);
		}
	}

	void SceneComponent::Update(const Time& time)
	{
		UpdateWorldMatrix();

		GetApp()->GetRenderer()->AddDrawable(*this);
	}

	void SceneComponent::Draw(const Time& time)
	{
		if (m_meshesData.empty())
			return;

		for (const auto& meshData : m_meshesData)
		{
			Draw_SetIA(meshData);
			Draw_SetData(meshData);
			Draw_Render(meshData);
		}
	}

	//-------------------------------------------------------------------------

	void SceneComponent::Draw_SetIA(const MeshData& meshData)
	{
		auto deviceContext = GetApp()->GetDeviceContext();

		deviceContext->IASetPrimitiveTopology(m_primitiveTopology);
		deviceContext->IASetInputLayout(m_inputLayout.Get());

		// set vertex buffer
		{
			unsigned stride = GetVertexSize();
			unsigned offset = 0;
			deviceContext->IASetVertexBuffers(
				0, 1, meshData.vertexBuffer.buffer.GetAddressOf(), &stride, &offset
			);
		}

		// set index buffer (if needed)
		if (!!meshData.indexBuffer)
		{
			deviceContext->IASetIndexBuffer(
				meshData.indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0
			);
		}
	}

	void SceneComponent::Draw_SetData(const MeshData& meshData)
	{
		if (const auto material = GetMaterial())
		{
			auto& currentTechnique = material->GetCurrentTechnique();
			auto& pass = currentTechnique.GetPass(0);

			pass.Apply(0, GetApp()->GetDeviceContext());
		}
	}

	void SceneComponent::Draw_Render(const MeshData& meshData)
	{
		auto deviceContext = GetApp()->GetDeviceContext();

		if (!!meshData.indexBuffer)
			deviceContext->DrawIndexed(meshData.indexBuffer->elementsCount, 0, 0);
		else
			deviceContext->Draw(meshData.vertexBuffer.elementsCount, 0);
	}
} // namespace library

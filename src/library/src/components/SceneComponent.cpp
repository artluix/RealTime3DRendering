#include "StdAfx.h"
#include "library/Components/SceneComponent.h"

#include "library/Application.h"

//#include "library/Model/Mesh.h"
//#include "library/Model/Model.h"

//#include "library/Materials/Material.h"

//#include "library/Effect/Effect.h"
//#include "library/Effect/EffectTechnique.h"
//#include "library/Effect/EffectPass.h"

#include <cassert>

namespace library
{
SceneComponent::SceneComponent()
	: m_transform{}
	, m_worldMatrix(math::Matrix4::Identity)
	, m_initialTransformMatrix(math::Matrix4::Identity)
	, m_direction(math::Vector3::Forward)
	, m_up(math::Vector3::Up)
	, m_right(math::Vector3::Right)
{}

SceneComponent::~SceneComponent() = default;

//-------------------------------------------------------------------------

const library::math::Matrix4& SceneComponent::GetWorldMatrix() const
{
	assert(!m_isWorldMatrixDirty);
	return m_worldMatrix;
}

void SceneComponent::SetPosition(const math::Vector3& position)
{
	if (m_transform.position != position)
	{
		m_transform.position = position;
		m_isWorldMatrixDirty = true;
	}
}

void SceneComponent::Translate(const math::Vector3& translation)
{
	if (!translation)
		return;

	SetPosition(m_transform.position + translation);
}

//-------------------------------------------------------------------------

void SceneComponent::SetRotation(const math::Quaternion& rotation)
{
	if (m_transform.rotation != rotation)
	{
		m_transform.rotation = rotation;
		m_isWorldMatrixDirty = true;
	}
}

void SceneComponent::Rotate(const math::Quaternion& rotation)
{
	if (!rotation)
		return;

	SetRotation(m_transform.rotation * rotation);
}

void SceneComponent::SetRotation(const math::Vector3& pitchYawRoll)
{
	SetRotation(math::Quaternion::RotationPitchYawRoll(pitchYawRoll));
}

void SceneComponent::Rotate(const math::Vector3& pitchYawRoll)
{
	Rotate(math::Quaternion::RotationPitchYawRoll(pitchYawRoll));
}

//-------------------------------------------------------------------------

void SceneComponent::SetScaling(const math::Vector3& scaling)
{
	if (scaling < math::Vector3::Zero)
		return;

	if (m_transform.scaling != scaling)
	{
		m_transform.scaling = scaling;
		m_isWorldMatrixDirty = true;
	}
}

void SceneComponent::Scale(const math::Vector3& scaling)
{
	if (!scaling)
		return;

	SetScaling(m_transform.scaling + scaling);
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

	const auto scalingMatrix = math::Matrix4::Scaling(m_transform.scaling);
	const auto translationMatrix = math::Matrix4::Translation(m_transform.position);
	const auto rotationMatrix = math::Matrix4::RotationQuaternion(m_transform.rotation);

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

// void SceneComponent::SetModelName(const std::string& modelName)
//{
//	m_modelName = modelName;
//}

////-------------------------------------------------------------------------

// void SceneComponent::SetTextureName(const std::string& textureName)
//{
//	m_textureName = textureName;
//}

//-------------------------------------------------------------------------

// void SceneComponent::Initialize()
//{
//	DrawableComponent::Initialize(app);

//	if (const auto material = GetMaterial())
//	{
//		assert(material->IsInitialized());

//		// set default layout from material
//		const auto& currentTechnique = material->GetCurrentTechnique();
//		const auto& pass = currentTechnique.GetPass(0);
//		m_inputLayout = material->GetInputLayoutShared(pass);

//		// load model from file
//		if (!m_modelName.empty())
//		{
//			m_model = std::make_unique<Model>(*GetApp(), m_modelName, true);

//			if (m_model->HasMeshes())
//			{
//				m_meshesData.clear();

//				const auto meshesCount = m_model->GetMeshesCount();
//				m_meshesData.reserve(meshesCount);

//				for (unsigned i = 0; i < meshesCount; i++)
//				{
//					MeshData md;

//					const auto& mesh = m_model->GetMesh(i);

//					md.vertexBuffer.buffer = material->CreateVertexBuffer(GetApp().GetDevice(), mesh);
//					md.vertexBuffer.elementsCount = mesh.GetVerticesCount();

//					if (mesh.HasIndices())
//					{
//						md.indexBuffer = std::make_optional(BufferData{
//							mesh.CreateIndexBuffer(),
//							mesh.GetIndicesCount()
//						});
//					}

//					m_meshesData.push_back(std::move(md));
//				}
//			}
//		}
//	}

//	// load texture not from model
//	if (!m_textureName.empty())
//	{
//		assert(m_meshesData.size() == 1);
//		m_meshesData[0].texture = GetApp().LoadTexture(m_textureName);
//	}
//}

void SceneComponent::Update(const Time& time)
{
	UpdateWorldMatrix();
}
} // namespace library

#pragma once
#include "library/Components/DrawableComponent.h"
#include "library/Math/Math.h"

#include "library/MeshData.h"
#include "library/Model/ModelMaterial.h"

#include <d3dcommon.h>
#include <string>
#include <vector>
#include <memory>

namespace library
{
	class Model;

	class SceneComponent : public DrawableComponent
	{
		RTTI_CLASS(SceneComponent, DrawableComponent)

	public:
		~SceneComponent();

		const math::Vector3& GetPosition() const { return m_position; }
		const math::Vector3& GetRotation() const { return m_rotation; }
		const math::Vector3& GetScaling() const { return m_scaling; }
		const math::Matrix4& GetInitialTransformMatrix() const { return m_initialTransformMatrix; }
		const math::Matrix4& GetWorldMatrix() const { assert(!m_isWorldMatrixDirty); return m_worldMatrix; }

		void SetPosition(const math::Vector3& position);
		void Translate(const math::Vector3& translation);

		void SetRotation(const math::Vector3& rotation);
		void Rotate(const math::Vector3& rotation);

		void SetScaling(const math::Vector3& scaling);
		void Scale(const math::Vector3& scaling);

		void SetInitialTransform(const math::Matrix4& initialTransform);

		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

		//-------------------------------------------------------------------------

		const std::string& GetModelName() const { return m_modelName; }
		void SetModelName(const std::string& modelName);

		const std::string& GetTextureName() const { return m_textureName; }
		void SetTextureName(const std::string& textureName);

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;
		void Draw(const Time& time) override;

	protected:
		explicit SceneComponent();

		void UpdateWorldMatrix();

		// draw stages
		virtual void Draw_SetIA(const MeshData& meshData);
		virtual void Draw_SetData(const MeshData& meshData);
		virtual void Draw_Render(const MeshData& meshData);

		virtual unsigned GetVertexSize() const = 0;

		std::unique_ptr<Model> m_model;
		std::vector<MeshData> m_meshesData;

		D3D_PRIMITIVE_TOPOLOGY m_primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		TextureType::Type m_textureType = TextureType::Diffuse;

		ComPtr<ID3D11InputLayout> m_inputLayout;

		std::string m_modelName;
		std::string m_textureName;

	private:
		math::Vector3 m_position;
		math::Vector3 m_rotation;
		math::Vector3 m_scaling;

		math::Vector3 m_direction;
		math::Vector3 m_right;
		math::Vector3 m_up;

		math::Matrix4 m_initialTransformMatrix;
		math::Matrix4 m_worldMatrix;
		bool m_isWorldMatrixDirty = true;
	};
} // namespace library

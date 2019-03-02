#pragma once
#include "library/components/DrawableComponent.h"
#include "library/math/Math.h"

#include "library/CommonTypes.h"
#include "library/DirectXForwardDeclarations.h"

#include <d3dcommon.h>
#include <string>
#include <optional>

namespace library
{
	class SceneComponent : public DrawableComponent
	{
		RTTI_CLASS(SceneComponent, DrawableComponent)

	public:
		const math::Vector3& GetPosition() const { return m_position; }
		const math::Vector3& GetRotation() const { return m_rotation; }
		const math::Vector3& GetScaling() const { return m_scaling; }
		const math::Matrix4& GetWorldMatrix() const { assert(!m_isWorldMatrixDirty); return m_worldMatrix; }

		void SetPosition(const math::Vector3& position);
		void Translate(const math::Vector3& translation);

		void SetRotation(const math::Vector3& rotation);
		void Rotate(const math::Vector3& rotation);

		void SetScaling(const math::Vector3& scaling);
		void Scale(const math::Vector3& scaling);

		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

		//-------------------------------------------------------------------------

		const std::string& GetModelName() const { return m_modelName; }
		void SetModelName(const std::string& modelName);

		const std::string& GetTextureName() const { return m_textureName; }
		void SetTextureName(const std::string& textureName);

		ID3D11ShaderResourceView* GetTexture() const { return m_texture.Get(); }

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;
		void Draw(const Time& time) override;

	protected:
		struct BufferData
		{
			ComPtr<ID3D11Buffer> buffer;
			unsigned count = 0;
		};

		explicit SceneComponent();

		void UpdateWorldMatrix();

		// draw stages
		virtual void Draw_SetIA();
		virtual void Draw_SetData();
		virtual void Draw_Render();

		virtual unsigned GetVertexSize() const = 0;

		struct
		{
			D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			BufferData vertices; // mandatory
			std::optional<BufferData> indices; // optional
			ComPtr<ID3D11InputLayout> layout;
		} m_input;

		std::string m_modelName;
		std::string m_textureName;

	private:
		ComPtr<ID3D11ShaderResourceView> m_texture;

		math::Vector3 m_position;
		math::Vector3 m_rotation;
		math::Vector3 m_scaling;

		math::Vector3 m_direction;
		math::Vector3 m_right;
		math::Vector3 m_up;

		math::Matrix4 m_worldMatrix;
		bool m_isWorldMatrixDirty = true;
	};
} // namespace library

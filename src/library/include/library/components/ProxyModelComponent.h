#pragma once
#include "library/components/SceneComponent.h"
#include "library/Path.h"
#include "library/DirectXForwardDeclarations.h"

#include <memory>

namespace library
{
	class Effect;
	class BasicMaterial;

	class ProxyModelComponent : public SceneComponent
	{
		RTTI_CLASS(ProxyModelComponent, SceneComponent)

	public:
		explicit ProxyModelComponent(const Application& app, const CameraComponent& camera, const fs::Path& modelPath, const float scale);
		~ProxyModelComponent();

		bool IsWireframeVisible() const { return m_isWireframeVisible; }
		void SetWireframeVisible(const bool visible);

		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

		void Initialize() override;
		void Update(const Time& time) override;
		void Draw(const Time& time) override;

	private:
		fs::Path m_modelPath;

		std::unique_ptr<Effect> m_effect;
		std::unique_ptr<BasicMaterial> m_material;

		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;

		unsigned m_indicesCount;

		bool m_isWireframeVisible;

		math::Vector3 m_direction;
		math::Vector3 m_up;
		math::Vector3 m_right;
	};

} // namespace library

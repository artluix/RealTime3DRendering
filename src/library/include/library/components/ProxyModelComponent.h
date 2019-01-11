#pragma once
#include "library/components/SceneComponent.h"
#include "library/effect/materials/BasicEffectMaterial.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class Path;
	class Effect;

	class ProxyModelComponent : public SceneComponent
	{
		RTTI_CLASS(ProxyModelComponent, SceneComponent)

	public:
		explicit ProxyModelComponent(const Application& app, const Path& modelPath, const float scale);
		~ProxyModelComponent() = default;

		bool IsWireframeVisible() const { return m_isWireframeVisible; }
		void SetWireframeVisible(const bool visible);

		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

		void Initialize() override;
		void Update(const Time& time) override;
		using DrawableComponent::Draw;

	protected:
		void SetEffectData() override;
		void Render() override;

	private:
		std::shared_ptr<Effect> m_effect;
		std::unique_ptr<BasicEffectMaterial> m_material;

		bool m_isWireframeVisible = true;

		math::Vector3 m_direction;
		math::Vector3 m_up;
		math::Vector3 m_right;
	};
} // namespace library

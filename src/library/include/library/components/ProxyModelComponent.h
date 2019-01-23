#pragma once
#include "library/materials/BasicMaterial.h"

#include "library/components/SceneComponent.h"
#include "library/components/MaterialComponentGlue.hpp"

#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class Path;

	class ProxyModelComponent
		: public SceneComponent
		, public MaterialComponentGlue<BasicMaterial>
	{
		RTTI_CLASS(ProxyModelComponent, SceneComponent)

	public:
		explicit ProxyModelComponent(const Application& app, const Path& modelPath, const float scale);
		~ProxyModelComponent();

		bool IsWireframeVisible() const { return m_isWireframeVisible; }
		void SetWireframeVisible(const bool visible);

		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

		void Initialize() override;
		using DrawableComponent::Draw;

	protected:
		void SetEffectData() override;
		void Render() override;

	private:
		bool m_isWireframeVisible = true;

		math::Vector3 m_direction;
		math::Vector3 m_up;
		math::Vector3 m_right;
	};
} // namespace library

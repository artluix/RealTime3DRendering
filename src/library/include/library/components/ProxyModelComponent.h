#pragma once
#include "library/materials/BasicMaterial.h"

#include "library/components/SceneComponent.h"
#include "library/components/ConcreteDrawableInputMaterialComponent.hpp"

#include "library/DirectXForwardDeclarations.h"

#include <string>

namespace library
{
	class ProxyModelComponent
		: public SceneComponent
		, public ConcreteDrawableInputMaterialComponent<BasicMaterial>
	{
		RTTI_CLASS(ProxyModelComponent, SceneComponent)

	public:
		explicit ProxyModelComponent(const std::string& modelName, const float scale);
		~ProxyModelComponent();

		bool IsWireframeVisible() const { return m_isWireframeVisible; }
		void SetWireframeVisible(const bool visible);

		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

		void Initialize(const Application& app) override;
		using DrawableComponent::Draw;

	protected:
		void Draw_SetData() override;
		void Draw_Render() override;

	private:
		bool m_isWireframeVisible = true;

		math::Vector3 m_direction;
		math::Vector3 m_up;
		math::Vector3 m_right;
	};
} // namespace library

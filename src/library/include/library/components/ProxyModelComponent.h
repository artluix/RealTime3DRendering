#pragma once
#include "library/components/SceneComponent.h"
#include "library/materials/BasicMaterial.h"
#include "library/components/ConcreteMaterialComponent.hpp"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	namespace fs
	{
		class Path;
	} // namespace

	class ProxyModelComponent
		: public Scene
		, public ConcreteMaterialComponent<BasicMaterial>
	{
		RTTI_CLASS(ProxyModelComponent, Scene, MaterialComponent)

	public:
		explicit ProxyModelComponent(const Application& app, const fs::Path& modelPath, const float scale);
		~ProxyModelComponent();

		bool IsWireframeVisible() const { return m_isWireframeVisible; }
		void SetWireframeVisible(const bool visible);

		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

		void Initialize() override;
		void Update(const Time& time) override;

		using MaterialComponent::Draw;

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

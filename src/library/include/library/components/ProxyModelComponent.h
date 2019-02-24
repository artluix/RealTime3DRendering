#pragma once
#include "library/materials/BasicMaterial.h"
#include "library/components/ConcreteMaterialSceneComponent.hpp"

#include "library/DirectXForwardDeclarations.h"

#include <string>

namespace library
{
	class ProxyModelComponent : public ConcreteMaterialSceneComponent<BasicMaterial>
	{
		RTTI_CLASS(ProxyModelComponent, SceneComponent)

	public:
		explicit ProxyModelComponent(const std::string& modelName, const float scale);
		~ProxyModelComponent();

		bool IsWireframeVisible() const { return m_isWireframeVisible; }
		void SetWireframeVisible(const bool visible);

		void Initialize(const Application& app) override;

	protected:
		void Draw_SetData() override;
		void Draw_Render() override;

	private:
		bool m_isWireframeVisible = true;
	};
} // namespace library

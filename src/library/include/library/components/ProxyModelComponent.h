#pragma once
#include "library/Materials/BasicMaterial.h"
#include "library/Components/ConcreteMaterialSceneComponent.hpp"

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
		void Draw_SetData(const MeshData& meshData) override;
		void Draw_Render(const MeshData& meshData) override;

		bool m_isWireframeVisible = true;
	};
} // namespace library

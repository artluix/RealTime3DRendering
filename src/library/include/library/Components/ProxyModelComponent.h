#pragma once
#include "library/Materials/BasicMaterial.h"
#include "library/Components/ConcreteMaterialPrimitiveComponent.hpp"
#include "library/DxForward.h"

#include <string>

namespace library
{
class ProxyModelComponent : public ConcreteMaterialPrimitiveComponent<BasicMaterial>
{
	RTTI_CLASS(ProxyModelComponent, PrimitiveComponent)

public:
	ProxyModelComponent(const std::string& modelName, const float scale);
	~ProxyModelComponent();

	bool IsWireframeVisible() const { return m_wireframeVisible; }
	void SetWireframeVisible(const bool visible) { m_wireframeVisible = visible; }

protected:
	void InitializeInternal() override;
	void Draw_SetData(const PrimitiveData& primitiveData) override;
	void Draw_Render(const PrimitiveData& primitiveData) override;

	bool m_wireframeVisible = true;

	std::string m_modelName;
};
} // namespace library

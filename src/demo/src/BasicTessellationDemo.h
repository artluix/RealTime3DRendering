#pragma once
#include <library/Materials/TessellationMaterial.h>
#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>

#include <library/Components/InputReceivableComponent.h>

namespace library
{
	class TextComponent;
} // namespace library

class BasicTessellationDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::TessellationMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(BasicTessellationDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit BasicTessellationDemo();

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;
	void Draw_Render(const library::PrimitiveData& primitiveData) override;

private:
	// ConcreteMaterialPrimitiveComponent<T>::m_effect - triangle effect
	std::shared_ptr<library::Effect> m_quadEffect;

	std::unique_ptr<library::TextComponent> m_text;

	std::vector<float> m_tessellationEdgeFactors;
	std::vector<float> m_tessellationInsideFactors;

	bool m_isUniformTessellation;
	bool m_showQuadTopology;
};

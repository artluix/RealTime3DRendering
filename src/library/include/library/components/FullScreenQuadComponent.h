#pragma once
#include "library/Components/PrimitiveComponent.h"

#include <functional>

namespace library
{
class EffectPass;

class FullScreenQuadComponent : public PrimitiveComponent
{
	RTTI_CLASS(FullScreenQuadComponent, PrimitiveComponent)

public:
	using MaterialUpdateFunction = std::function<void()>;

	FullScreenQuadComponent();
	~FullScreenQuadComponent();

	void SetMaterial(Material& material);
	void SetMaterial(Material& material, const std::string& techniqueName, const std::string& passName = "p0");

	void SetActiveTechnique(const std::string& techniqueName, const std::string& passName = "p0");
	void SetMaterialUpdateFunction(const MaterialUpdateFunction& func);

	void Initialize() override;
	void Update(const Time& time) override;

	const Material* GetMaterial() const override { return m_material; }

protected:
	void Draw_SetData(const PrimitiveData& primitiveData) override;

	Material* GetMaterial() override { return m_material; }

	MaterialUpdateFunction m_materialUpdateFunction;
	Material* m_material = nullptr;

private:
	using PrimitiveComponent::m_dxEffectPass;
	using PrimitiveComponent::m_dxInputLayout;
};
} // namespace library

#pragma once
#include <library/Materials/DiffuseLightingMaterial.h>
#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class DirectionalLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

class DiffuseLightingDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::DiffuseLightingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(DiffuseLightingDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	DiffuseLightingDemo();
	~DiffuseLightingDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	struct Texture
	{
		enum Type : unsigned
		{
			Default = 0,

			//# Count
			Count
		};
	};

	void UpdateAmbientLight(const library::Time& time);
	void UpdateDirectionalLight(const library::Time& time);

	library::Color m_ambientColor;

	std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};

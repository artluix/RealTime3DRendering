#pragma once
#include <library/Materials/EnvironmentMappingMaterial.h>
#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
class TextComponent;
} // namespace library

class EnvironmentMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::EnvironmentMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(EnvironmentMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	EnvironmentMappingDemo();
	~EnvironmentMappingDemo();

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
			EnvironmentMap,

			//# Count
			Count
		};
	};

	library::Color m_ambientColor;
	float m_reflectionAmount;

	std::unique_ptr<library::TextComponent> m_text;
};

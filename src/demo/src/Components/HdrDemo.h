#pragma once
#include "Materials/ToneMappingMaterial.h"

#include <library/Components/ConcreteMaterialPostProcessingComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
class TextComponent;
} // namespace library

class HdrDemo
	: public library::ConcreteMaterialPostProcessingComponent<ToneMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(HdrDemo, library::PostProcessingComponent, library::InputReceivableComponent)

public:
	enum class ToneMappingType
	{
		No,
		Reinhard
	};

	HdrDemo();
	~HdrDemo();

	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

protected:
	void InitializeInternal() override;

private:
	std::unique_ptr<library::TextComponent> m_text;
};

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
		Reinhard,
		Gamma
	};

	HdrDemo();
	~HdrDemo();

	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

	static std::string ToTechniqueName(const ToneMappingType t);
	static std::string ToString(const ToneMappingType t);

protected:
	void InitializeInternal() override;

private:
	void UpdateExposure(const library::Time& time);

	std::unique_ptr<library::TextComponent> m_text;
	ToneMappingType m_toneMappingType = ToneMappingType::No;

	float m_exposure;
};

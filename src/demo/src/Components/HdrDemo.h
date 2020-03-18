#pragma once
#include <library/Components/ToneMappingComponent.h>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
class TextComponent;
} // namespace library

class HdrDemo
	: public library::ToneMappingComponent
	, public library::InputReceivableComponent
{
	RTTI_CLASS(HdrDemo, library::ToneMappingComponent, library::InputReceivableComponent)

public:
	HdrDemo() = default;
	~HdrDemo() = default;

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;

private:
	std::unique_ptr<library::TextComponent> m_text;
};

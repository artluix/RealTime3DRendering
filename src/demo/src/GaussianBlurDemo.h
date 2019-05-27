#pragma once
#include <library/Components/GaussianBlurComponent.h>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
class TextComponent;
} // namespace library

class GaussianBlurDemo
	: public library::GaussianBlurComponent
	, public library::InputReceivableComponent
{
	RTTI_CLASS(GaussianBlurDemo, library::InputReceivableComponent)

public:
	GaussianBlurDemo();
	~GaussianBlurDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;

private:
	void UpdateBlurAmount(const library::Time& time);

	std::unique_ptr<library::TextComponent> m_text;
};

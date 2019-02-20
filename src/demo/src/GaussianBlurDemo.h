#pragma once
#include <library/components/GaussianBlurComponent.h>
#include <library/components/InputReceivableComponent.h>

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
	explicit GaussianBlurDemo();
	~GaussianBlurDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

private:
	void UpdateBlurAmount(const library::Time& time);

	std::unique_ptr<library::TextComponent> m_text;
};

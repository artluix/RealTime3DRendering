#pragma once
#include <library/Components/BloomComponent.h>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
	class TextComponent;
} // namespace library

class BloomDemo
	: public library::BloomComponent
	, public library::InputReceivableComponent
{
	RTTI_CLASS(BloomDemo, library::InputReceivableComponent)

public:
	explicit BloomDemo();
	~BloomDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

private:
	void UpdateSettings(const library::Time& time);

	std::unique_ptr<library::TextComponent> m_text;
};

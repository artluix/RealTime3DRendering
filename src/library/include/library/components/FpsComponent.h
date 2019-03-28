#pragma once
#include "library/Components/TextComponent.h"
#include "library/Time.h"

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
} // namespace DirectX

namespace library
{
	class FpsComponent : public TextComponent
	{
		RTTI_CLASS(FpsComponent, TextComponent)

	public:
		explicit FpsComponent();
		~FpsComponent();

		std::size_t GetFrameRate() const { return m_frameRate; }

		void Update(const Time& time) override;

	private:
		std::size_t m_framesCount;
		unsigned m_frameRate;

		Duration m_timeAccumulator;
	};
} // namespace library

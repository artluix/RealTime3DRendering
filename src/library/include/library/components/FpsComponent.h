#pragma once
#include "library/components/TextComponent.h"
#include "library/Time.h"

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
} // namespace DirectX

namespace library
{
	class FpsComponent : public rtti::Class<FpsComponent, TextComponent>
	{
	public:
		explicit FpsComponent(const Application& app);
		~FpsComponent() = default;

		unsigned GetFrameRate() const { return m_frameRate; }

		void Update(const Time& time) override;

	private:
		unsigned m_frameCount;
		unsigned m_frameRate;

		Duration m_timeAccumulator;
	};

} // namespace library

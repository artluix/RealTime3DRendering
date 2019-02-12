#pragma once
#include "library/materials/BloomMaterial.h"
#include "library/components/ConcretePostProcessingComponent.hpp"

namespace library
{
	class BloomComponent : public ConcretePostProcessingComponent<BloomMaterial>
	{
		RTTI_CLASS(BloomComponent, PostProcessingComponent)

	public:
		explicit BloomComponent();
		~BloomComponent();

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;
		void Draw(const Time& time) override;

	private:
		void UpdateBloom(const Time& time);

		std::unique_ptr<FullScreenRenderTarget> m_renderTarget;
	};
} // namespace library

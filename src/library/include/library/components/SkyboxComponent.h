#pragma once
#include "library/components/SceneComponent.h"

#include "library/materials/SkyboxMaterial.h"
#include "library/components/ConcreteMaterialComponent.hpp"

#include "library/Path.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class SkyboxComponent
		: public SceneComponent
		, public ConcreteMaterialComponent<Skybox>
	{
		RTTI_CLASS(SkyboxComponent, SceneComponent, MaterialComponent)

	public:
		explicit SkyboxComponent(
			const Application& app,
			const fs::Path& cubeMapPath,
			const float scale
		);

		~SkyboxComponent() = default;

		void Initialize() override;
		void Update(const Time& time) override;
		using MaterialComponent::Draw;

	protected:
		void SetEffectData() override;

	private:
		fs::Path m_cubeMapPath;
		ComPtr<ID3D11ShaderResourceView> m_cubeMapShaderResourceView;
	};

} // namespace library

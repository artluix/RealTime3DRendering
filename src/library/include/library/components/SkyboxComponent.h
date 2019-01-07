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
		, public ConcreteMaterialComponent<SkyboxMaterial>
	{
		RTTI_CLASS(SkyboxComponent, MaterialComponent)

	public:
		explicit SkyboxComponent(
			const Application& app,
			const CameraComponent& camera,
			const fs::Path& cubeMapPath,
			const float scale
		);

		~SkyboxComponent() = default;

		void Initialize() override;
		void Update(const Time& time) override;

		using MaterialComponent::Render;

		//void Draw(const Time& time) override;

	protected:
		void UpdateVariables() override;

	private:
		fs::Path m_cubeMapPath;

		//std::unique_ptr<Effect> m_effect;
		//std::unique_ptr<SkyboxMaterial> m_material;

		ComPtr<ID3D11ShaderResourceView> m_cubeMapShaderResourceView;
		//ComPtr<ID3D11Buffer> m_vertexBuffer;
		//ComPtr<ID3D11Buffer> m_indexBuffer;
		unsigned m_indicesCount;
	};
} // namespace library

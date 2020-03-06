#pragma once
#include "library/Materials/SkyboxMaterial.h"
#include "library/Components/ConcreteMaterialPrimitiveComponent.hpp"

#include <string>

namespace library
{
class SkyboxComponent : public ConcreteMaterialPrimitiveComponent<SkyboxMaterial>
{
	RTTI_CLASS(SkyboxComponent, PrimitiveComponent)

public:
	SkyboxComponent(const std::string& cubeMapFilename, const float scale);
	~SkyboxComponent() = default;

	void Update(const Time& time) override;

protected:
	struct Texture
	{
		enum Type : unsigned
		{
			Skybox = 0,

			//# Count
			Count
		};
	};

	void InitializeInternal() override;
	void Draw_SetData(const PrimitiveData& primitiveData) override;

	std::string m_cubeMapFilename;
};
} // namespace library

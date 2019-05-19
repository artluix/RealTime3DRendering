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
	SkyboxComponent(const std::string& cubeMapName, const float scale);
	~SkyboxComponent() = default;

	void Initialize() override;
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

	std::string m_cubeMapName;

	void Draw_SetData(const PrimitiveData& primitiveData) override;
};
} // namespace library

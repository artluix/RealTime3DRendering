#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
class DepthMapMaterial : public Material
{
	RTTI_CLASS(DepthMapMaterial, Material)

public:
	using Vertex = VertexPosition;

	explicit DepthMapMaterial(std::shared_ptr<Effect> effect);
	~DepthMapMaterial();

	EffectVariable& GetWorldLightViewProjection() { return m_worldLightViewProjection; }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;
	unsigned GetVertexSize() const override { return sizeof(Vertex); }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_worldLightViewProjection;
};

//-------------------------------------------------------------------------

struct DepthMappingTechnique
{
	enum Type : unsigned
	{
		Create = 0,
		// RenderTarget,
		Bias,

		//# Count
		Count
	};

	static constexpr Type Next(const Type t) { return static_cast<Type>((t + 1) % Count); }

	static constexpr char* ToDisplayString(const Type t)
	{
		switch (t)
		{
			case Create: return "Create";
			case Bias:
				return "Bias";
				// case RenderTarget:	return "Render Target";
			default: return "";
		};
	}

	static constexpr char* ToString(const Type t)
	{
		switch (t)
		{
			case Create: return "create_depth_map";
			case Bias:
				return "depth_map_bias";
				// case RenderTarget:	return "depth_map_render_target";
			default: return "";
		};
	}
};
} // namespace library

#pragma once
#include <library/Materials/Material.h>

class DepthMapMaterial : public library::Material
{
	RTTI_CLASS(DepthMapMaterial, library::Material)

public:
	using Vertex = library::VertexPosition;

	explicit DepthMapMaterial(library::Effect& effect);
	~DepthMapMaterial();

	library::EffectVariable& GetWorldLightViewProjection() { return m_worldLightViewProjection; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_worldLightViewProjection;
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
			case Bias:   return "Bias";
				// case RenderTarget:	return "Render Target";
			default: return "";
		};
	}

	static constexpr char* ToString(const Type t)
	{
		switch (t)
		{
			case Create: return "create_depth_map";
			case Bias:   return "depth_map_bias";
				// case RenderTarget:	return "depth_map_render_target";
			default: return "";
		};
	}
};

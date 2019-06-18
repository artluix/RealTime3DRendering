#pragma once
#include <library/Materials/TessellationMaterial.h>
#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>

#include <library/Components/InputReceivableComponent.h>

#include <memory>

namespace library
{
class TextComponent;
} // namespace library

class BasicTessellationDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::TessellationMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(BasicTessellationDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	BasicTessellationDemo();

	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

	const library::TessellationMaterial* GetMaterial() const override;

protected:
	library::TessellationMaterial* GetMaterial() override;

	void InitializeInternal() override;

	void Draw_SetData(const library::PrimitiveData& primitiveData) override;
	void Draw_Render(const library::PrimitiveData& primitiveData) override;

private:
	struct Topology
	{
		enum Type : unsigned
		{
			Triangle = 0,
			Quad,

			//# Count
			Count
		};
	};
	using TopologyType = Topology::Type;

	std::wstring DumpEdgeFactors() const;
	std::wstring DumpInsideFactors() const;

	std::shared_ptr<library::Effect> m_quadEffect;
	std::unique_ptr<library::TessellationMaterial> m_quadMaterial;

	std::unique_ptr<library::TextComponent> m_text;

	// tessellation factors stored as for quad topology
	struct
	{
		union
		{
			std::array<float, 3> triangle;
			std::array<float, 4> quad;
		};
	} m_tessEdgeFactors;

	struct
	{
		union
		{
			float triangle;
			std::array<float, 2> quad;
		};
	} m_tessInsideFactors;

	bool m_isUniformTessellation;
	TopologyType m_topology;
};

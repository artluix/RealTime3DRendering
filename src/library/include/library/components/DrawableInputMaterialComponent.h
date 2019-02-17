#pragma once
#include "library/components/DrawableInputComponent.h"
#include "library/components/DrawableMaterialComponent.h"

namespace library
{
	class DrawableInputMaterialComponent
		: public virtual DrawableMaterialComponent
		, public DrawableInputComponent
	{
		RTTI_CLASS(DrawableInputMaterialComponent, DrawableInputComponent, DrawableMaterialComponent)

	public:
		const std::string& GetDefaultModelName() const { return m_defaultModelName; }
		void SetDefaultModelName(const std::string& modelName);

		void Initialize(const Application& app) override;

		unsigned GetVertexSize() const override;

	protected:
		explicit DrawableInputMaterialComponent() = default;

		void Draw_SetData() override;

	private:
		std::string m_defaultModelName;
	};
} // namespace library

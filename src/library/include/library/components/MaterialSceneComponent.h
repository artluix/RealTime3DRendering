#pragma once
#include "library/components/SceneComponent.h"
#include "library/components/MaterialDrawableComponent.h"

namespace library
{
	class MaterialSceneComponent
		: public SceneComponent
		, public MaterialDrawableComponent
	{
		RTTI_CLASS(MaterialSceneComponent, SceneComponent, MaterialDrawableComponent)

	public:
		const std::string& GetModelName() const { return m_modelName; }
		void SetModelName(const std::string& modelName);

		void Initialize(const Application& app) override;

	protected:
		explicit MaterialSceneComponent() = default;

		void Draw_SetData() override;

	private:
		std::string m_modelName;
	};
} // namespace library

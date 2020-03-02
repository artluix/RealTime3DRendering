#pragma once
#include "library/Components/Component.h"
#include "library/Math/Color.h"

#define USE_LIGHT_PROXY_MODEL

namespace library
{
#ifdef USE_LIGHT_PROXY_MODEL
class CameraComponent;
class ProxyModelComponent;
#endif

class LightComponent : public virtual Component
{
	RTTI_CLASS(LightComponent, Component)

public:
	const math::Color& GetColor() const { return m_color; }
	void SetColor(const math::Color& color);

#ifdef USE_LIGHT_PROXY_MODEL
	ProxyModelComponent* GetProxyModel() const { return m_proxyModel.get(); }
	virtual void SetupProxyModel(const CameraComponent& camera) = 0;
#endif

	void Update(const Time& time) override;

protected:
	explicit LightComponent() = default;

	void InitializeInternal() {}

	math::Color m_color = colors::White;

#ifdef USE_LIGHT_PROXY_MODEL
	std::unique_ptr<ProxyModelComponent> m_proxyModel;
#endif
};
} // namespace library

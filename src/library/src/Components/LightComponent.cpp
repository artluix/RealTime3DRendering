#include "StdAfx.h"
#include "library/Components/LightComponent.h"

#ifdef USE_LIGHT_PROXY_MODEL
#include "library/Components/ProxyModelComponent.h"
#endif

namespace library
{
void LightComponent::SetColor(const math::Color& color)
{
	m_color = color;
}

void LightComponent::Update(const Time& time)
{
#ifdef USE_LIGHT_PROXY_MODEL
	if (!!m_proxyModel)
		m_proxyModel->Update(time);
#endif
}
} // namespace library

#include "StdAfx.h"
#include "library/RenderTargets/SecondaryRenderTarget.h"

#include "library/Application.h"

namespace library
{
const D3D11_VIEWPORT& SecondaryRenderTarget::GetViewport() const
{
	return m_app.GetViewport();
}
} // namespace library

#pragma once
#include "library/NonConstructible.hpp"
#include "library/DxForward.h"

namespace library
{
struct RasterizerStates : public NonConstructible<RasterizerStates>
{
	static ID3D11RasterizerState* BackCulling;
	static ID3D11RasterizerState* FrontCulling;
	static ID3D11RasterizerState* DisabledCulling;
	static ID3D11RasterizerState* Wireframe;

	static void Initialize(ID3D11Device* const device);
	static void Clear();
};
} // namespace library

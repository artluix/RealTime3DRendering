#pragma once
#include "library/NonConstructible.hpp"
#include "library/DxForward.h"

namespace library
{
struct BlendStates : public NonConstructible<BlendStates>
{
	static ID3D11BlendState* Alpha;
	static ID3D11BlendState* Multiplicative;

	static void Initialize(ID3D11Device* const device);
	static void Clear();
};
} // namespace library

#pragma once
#include "library/NonConstructible.h"
#include "library/DxForward.h"

namespace library
{
struct SamplerStates : public NonConstructible<SamplerStates>
{
	static ID3D11SamplerState* TrilinearWrap;
	static ID3D11SamplerState* TrilinearMirror;
	static ID3D11SamplerState* TrilinearClamp;
	static ID3D11SamplerState* TrilinearBorder;

	static void Initialize(ID3D11Device* const device);
	static void Clear();
};
} // namespace library

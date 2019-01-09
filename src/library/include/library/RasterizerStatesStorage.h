#pragma once
#include "library/Common.h"
#include "library/NonConstructible.hpp"
#include "library/DirectXForwardDeclarations.h"

#include <map>

namespace library
{
	enum class RasterizerState
	{
		BackCulling,
		FrontCulling,
		DisabledCulling,
		Wireframe
	};

	class RasterizerStatesStorage : public NonConstructible<RasterizerStatesStorage>
	{
	public:
		static void Initialize(ID3D11Device* const device);
		static void Reset();

		static ID3D11RasterizerState* GetState(const RasterizerState rs);

	private:
		static std::map<RasterizerState, ComPtr<ID3D11RasterizerState>> s_states;
	};

} // namespace library

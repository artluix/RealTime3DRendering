#pragma once
#include "library/CommonTypes.h"
#include "library/NonConstructible.hpp"
#include "library/DirectXForwardDeclarations.h"

#include <array>

namespace library
{
	struct BlendState
	{
		using Ptr = ComPtr<ID3D11BlendState>;

		enum Type : unsigned
		{
			Alpha = 0,
			Multiplicative,

			//# Count
			Count
		};
	};

	class BlendStateHolder : public NonConstructible<BlendStateHolder>
	{
	public:
		static void Initialize(ID3D11Device* const device);
		static void Reset();

		static ID3D11BlendState* GetBlendState(const BlendState::Type bs);

	private:
		static std::array<BlendState::Ptr, BlendState::Count> s_blendStates;
	};
} // namespace library



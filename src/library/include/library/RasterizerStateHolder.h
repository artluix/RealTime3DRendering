#pragma once
#include "library/CommonTypes.h"
#include "library/NonConstructible.hpp"
#include "library/DirectXForwardDeclarations.h"

#include <array>

namespace library
{
	struct RasterizerState
	{
		using Ptr = ComPtr<ID3D11RasterizerState>;

		enum Type : unsigned
		{
			BackCulling = 0,
			FrontCulling,
			DisabledCulling,
			Wireframe,

			//# Count
			Count
		};
	};

	class RasterizerStateHolder : public NonConstructible<RasterizerStateHolder>
	{
	public:
		static void Initialize(ID3D11Device* const device);
		static void Reset();

		static RasterizerState::Ptr GetRasterizerState(const RasterizerState::Type rs);

	private:
		static std::array<RasterizerState::Ptr, RasterizerState::Count> s_rasterizerStates;
	};
} // namespace library

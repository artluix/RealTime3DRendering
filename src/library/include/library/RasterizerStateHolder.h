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

	//-------------------------------------------------------------------------

	namespace util
	{
		class RasterizerStateHolder : public NonConstructible<RasterizerStateHolder>
		{
		public:
			static void Initialize(ID3D11Device* const device);
			static void Reset();

			static ID3D11RasterizerState* GetRasterizerState(const RasterizerState rs);

		private:
			static std::map<RasterizerState, ComPtr<ID3D11RasterizerState>> s_rasterizerStates;
		};

	} // namespace util
} // namespace library
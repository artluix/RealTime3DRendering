#pragma once
#include <dinput.h>

namespace library
{
	enum class Key : std::uint8_t
	{
		Escape = DIK_ESCAPE,
		Num_0 = DIK_0,
		Num_1 = DIK_1,
		Num_2 = DIK_2,
		Num_3 = DIK_3,
		Num_4 = DIK_4,
		Num_5 = DIK_5,
		Num_6 = DIK_6,
		Num_7 = DIK_7,
		Num_8 = DIK_8,
		Num_9 = DIK_9,
		Q = DIK_Q,
		W = DIK_W,
		E = DIK_E,
		R = DIK_R,
		T = DIK_T,
		Y = DIK_Y,
		U = DIK_U,
		I = DIK_I,
		O = DIK_O,
		P = DIK_P,
		A = DIK_A,
		S = DIK_S,
		D = DIK_D,
		F = DIK_F,
		G = DIK_G,
		H = DIK_H,
		J = DIK_J,
		K = DIK_K,
		L = DIK_L,
		Z = DIK_Z,
		X = DIK_X,
		C = DIK_C,
		V = DIK_V,
		B = DIK_B,
		N = DIK_N,
		M = DIK_M,
		Backspace = DIK_BACK,
		Shift_Left = DIK_LSHIFT,
		Shift_Right = DIK_RSHIFT,
		Alt_Left = DIK_LALT,
		Alt_Right = DIK_RALT,
		Tab = DIK_TAB,
		Left_Win = DIK_LWIN,
		Right_Win = DIK_RWIN,
		Left_Ctrl = DIK_LCONTROL,
		Right_Ctrl = DIK_RCONTROL,
		Left = DIK_LEFT,
		Right = DIK_RIGHT,
		Up = DIK_UP,
		Down = DIK_DOWN,
		Space = DIK_SPACE,
		Enter = DIK_RETURN,

		// #Count
		Count
	};

} // namespace library

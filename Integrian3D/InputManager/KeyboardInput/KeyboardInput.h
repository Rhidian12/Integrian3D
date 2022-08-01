#pragma once

#ifdef _WIN32
#include <WinUser.h> /* Virtual Keys */
#endif

namespace Integrian3D
{
#ifdef _WIN32
	enum class KeyboardInput : int
	{
		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		M = 'M',
		N = 'N',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z',
		Left = VK_LEFT,
		Up = VK_UP,
		Right = VK_RIGHT,
		Down = VK_DOWN,
		Space = VK_SPACE,
		LShift = VK_LSHIFT,
		RShift = VK_RSHIFT,
		LControl = VK_LCONTROL,
		RControl = VK_RCONTROL
	};
#elif
#error There is no support for platforms outside Windows!
#endif
}
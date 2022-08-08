#pragma once

namespace Integrian3D
{
	template<typename T>
	struct Vector3 final
	{
		explicit Vector3()
			: X{}
			, Y{}
			, Z{}
		{}
		explicit Vector3(const T x, const T y, const T z)
			: X{ x }
			, Y{ y }
			, Z{ z }
		{}

		T X, Y, Z;
	};
}
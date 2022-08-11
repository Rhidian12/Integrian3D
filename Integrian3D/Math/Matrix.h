#pragma once

namespace Integrian3D
{
	template<int R, int C, typename T>
	struct Matrix final
	{
		constexpr Matrix() = default;

		constexpr T operator()(const int r, const int c) const
		{
			return Data[r][c];
		}
		constexpr T& operator()(const int r, const int c)
		{
			return Data[r][c];
		}

		constexpr int GetNumberOfRows() const
		{
			return R;
		}

		constexpr int GetNumberOfColumns() const
		{
			return C;
		}

		constexpr static Matrix MakeIdentityMatrix()
		{
			static_assert(R == C, "Matrix::MakeIdentityMatrix() > Identity Matrices must be square");

			Matrix matrix{};

			for (int i{}; i < R; ++i)
			{
				matrix.Data[i][i] = static_cast<T>(1.f);
			}

			return matrix;
		}

		T Data[R][C];
	};
}
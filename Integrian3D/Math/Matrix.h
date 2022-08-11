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

		constexpr Matrix GetCofactor(const int rowToIgnore, const int colToIgnore, const int length) const
		{
			static_assert(R == C, "Matrix::GetMatrixCofactor() > Matrix must be square");

			Matrix m{};

			int rowCounter{}, colCounter{};

			for (int r{}; r < length; ++r)
			{
				if (r == rowToIgnore)
				{
					continue;
				}

				for (int c{}; c < length; ++c)
				{
					if (c == colToIgnore)
					{
						continue;
					}

					m.Data[rowCounter][colCounter++] = Data[r][c];

					if (colCounter == length - 1)
					{
						colCounter = 0;
						++rowCounter;
					}
				}
			}

			return m;
		}

		constexpr T GetDeterminant(const int length)
		{
			static_assert(R == C, "Matrix::GetDeterminant() > Matrix must be square");

			if (length == 1)
			{
				return Data[0][0];
			}
			else if (length == 2)
			{
				return (Data[0][0] * Data[1][1]) - (Data[0][1] * Data[1][0]); // ad - bc
			}
			else
			{
				T determinant{};
				int sign{ 1 };

				for (int i{}; i < length; ++i)
				{
					Matrix m{ GetCofactor(0, i, length) };
					determinant += sign * Data[0][i] * GetDeterminant(m, length - 1);
					sign = -sign;
				}

				return determinant;
			}
		}

		T Data[R][C];
	};
}
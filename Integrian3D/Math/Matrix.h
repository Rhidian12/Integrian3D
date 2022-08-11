#pragma once

#include "../DebugUtility/DebugUtility.h"
#include "../Utils/Utils.h"

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

		constexpr Matrix GetInverse() const
		{
			static_assert(R == C, "MatrixUtility::GetInverse() > Matrix must be square");

			const T determinant{ MatrixUtility::GetDeterminant(*this, R) };

			if (Utils::AreEqual(determinant, static_cast<T>(0.f)))
			{
				Debug::LogError("Matrix::GetInverse() > Determinant is zero, no inverse matrix exists! Returning Identity Matrix!");
				return MakeIdentityMatrix();
			}

			Matrix adjointMatrix{ MatrixUtility::GetAdjoint(*this) };

			Matrix inverseMatrix{};

			for (int r{}; r < Rows; ++r)
				for (int c{}; c < Columns; ++c)
					inverseMatrix.data[r][c] = adjointMatrix.data[r][c] / determinant;

			return inverseMatrix;
		}

		T Data[R][C];
	};

	namespace MatrixUtility
	{
		template<int R, int C, typename T>
		inline constexpr Matrix<R, C, T> GetCofactor(const Matrix<R, C, T>& m, const int rowToIgnore, const int colToIgnore, const int length) 
		{
			static_assert(R == C, "MatrixUtility::GetMatrixCofactor() > Matrix must be square");

			Matrix matrix{};

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

					matrix.Data[rowCounter][colCounter++] = m.Data[r][c];

					if (colCounter == length - 1)
					{
						colCounter = 0;
						++rowCounter;
					}
				}
			}

			return matrix;
		}

		template<int R, int C, typename T>
		inline constexpr T GetDeterminant(const Matrix<R, C, T>& m, const int length)
		{
			static_assert(R == C, "MatrixUtility::GetDeterminant() > Matrix must be square");

			if (length == 1)
			{
				return m.Data[0][0];
			}
			else if (length == 2)
			{
				return (m.Data[0][0] * m.Data[1][1]) - (m.Data[0][1] * m.Data[1][0]); // ad - bc
			}
			else
			{
				T determinant{};
				int sign{ 1 };

				for (int i{}; i < length; ++i)
				{
					Matrix matrix{ GetCofactor(m, 0, i, length) };
					determinant += sign * m.Data[0][i] * GetDeterminant(matrix, length - 1);
					sign = -sign;
				}

				return determinant;
			}
		}

		template<int R, int C, typename T>
		inline constexpr Matrix<R, C, T> GetAdjoint(const Matrix<R, C, T>& m)
		{
			static_assert(R == C, "MatrixUtility::GetAdjointMatrix() > Matrix must be square!");

			Matrix matrix{};

			if constexpr (R == 1)
			{
				matrix[0][0] = 1;
			}
			else
			{
				int sign{ 1 };

				for (int r{}; r < R; ++r)
				{
					for (int c{}; c < C; ++c)
					{
						Matrix cofactorMatrix{ GetCofactor(m, r, c, R) };

						sign = ((r + c) % 2 == 0) ? 1 : -1;

						matrix.Data[c][r] = sign * GetDeterminant(cofactorMatrix, R - 1);
					}
				}
			}

			return matrix;
		}
	}
}
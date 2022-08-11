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

		constexpr static Matrix GetCofactor(const Matrix& m, const int rowToIgnore, const int colToIgnore, const int length)
		{
			static_assert(R == C, "Matrix::GetMatrixCofactor() > Matrix must be square");

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

		constexpr static T GetDeterminant(const Matrix& m, const int length)
		{
			static_assert(R == C, "Matrix::GetDeterminant() > Matrix must be square");

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

		constexpr static Matrix GetAdjoint(const Matrix& m)
		{
			static_assert(R == C, "Matrix::GetAdjointMatrix() > Matrix must be square!");

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

		constexpr Matrix GetInverse() const
		{
			static_assert(R == C, "Matrix::GetInverse() > Matrix must be square");

			const T determinant{ GetDeterminant(*this, R) };

			if (Utils::AreEqual(determinant, static_cast<T>(0.f)))
			{
				Debug::LogError("Matrix::GetInverse() > Determinant is zero, no inverse matrix exists! Returning Identity Matrix!", false);
				return MakeIdentityMatrix();
			}

			Matrix adjointMatrix{ GetAdjoint(*this) };

			Matrix inverseMatrix{};

			for (int r{}; r < R; ++r)
				for (int c{}; c < C; ++c)
					inverseMatrix.Data[r][c] = adjointMatrix.Data[r][c] / determinant;

			return inverseMatrix;
		}

		T Data[R][C];
	};

	template<int R1, int C1, int R2, int C2, typename T>
	Matrix<R1, C2, T> operator*(const Matrix<R1, C1, T>& m1, const Matrix<R2, C2, T>& m2)
	{
		static_assert(C1 == R2, "Matrix::operator*() > Nr of Columns of left hand matrix must match Nr of Rows of right hand matrix");

		Matrix<R1, C2, T> matrix{};

		constexpr int maxMatrixLength{ ColumnsM1 };

		for (int r{}; r < RowsM1; ++r)
		{
			for (int c{}; c < ColumnsM2; ++c)
			{
				for (int i{}; i < maxMatrixLength; ++i)
				{
					matrix.Data[r][c] += m1.Data[r][i] * m2.Data[i][c];
				}
			}
		}

		return matrix;
	}

	template<int R, int C, typename T>
	Matrix<R, C, T> operator*(const Matrix<R, C, T>& m, const T val)
	{
		Matrix<R, C, T> matrix{};

		for (int r{}; r < Rows; ++r)
		{
			for (int c{}; c < Columns; ++c)
			{
				matrix.Data[r][c] += m.Data[r][c] * val;
			}
		}

		return matrix;
	}

	template<int R1, int C1, int R2, int C2, typename T>
	Matrix<R1, C1, T> operator/(const Matrix<R1, C1, T>& m1, const Matrix<R2, C2, T>& m2)
	{
		static_assert(R2 == C2, "Matrix::operator/() > The divider must be square!");
		static_assert(C1 == R2, "Matrix::operator/() > Nr of Columns of left hand matrix must match Nr of Rows of right hand matrix");

		return m1 * m2.GetInverse();
	}

	template<int R, int C, typename T>
	Matrix<R, C, T> operator/(const Matrix<R, C, T>& m, const T val)
	{
		Matrix<R, C, T> matrix{};

		for (int r{}; r < Rows; ++r)
		{
			for (int c{}; c < Columns; ++c)
			{
				matrix.Data[r][c] += m.Data[r][c] / val;
			}
		}

		return matrix;
	}
}
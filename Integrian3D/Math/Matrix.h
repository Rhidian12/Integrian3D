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

		constexpr static Matrix MakeTranslationMatrix(const T x, const T y, const T z)
		{
			static_assert(R >= 4, "Matrix::MakeTranslationMatrix(x,y,z) > a 3D translation matrix requires a 4D square matrix");
			static_assert(C >= 4, "Matrix::MakeTranslationMatrix(x,y,z) > a 3D translation matrix requires a 4D square matrix");
			static_assert(R == C, "Matrix::MakeTranslationMatrix() > Matrix must be square");

			Matrix m{ MakeIdentityMatrix() };

			m(0, 3) = x;
			m(1, 3) = y;
			m(2, 3) = z;

			return m;
		}

		constexpr static Matrix MakeRotationMatrix(const T x, const T y, const T z)
		{
			static_assert(R >= 4, "Matrix::MakeRotationMatrix(x,y,z) > a 3D rotation matrix requires a 4D square matrix");
			static_assert(C >= 4, "Matrix::MakeRotationMatrix(x,y,z) > a 3D rotation matrix requires a 4D square matrix");
			static_assert(R == C, "Matrix::MakeRotationMatrix() > Matrix must be square");

			const T c1 = cos(-x);
			const T c2 = cos(-y);
			const T c3 = cos(-z);

			const T s1 = sin(-x);
			const T s2 = sin(-y);
			const T s3 = sin(-z);

			Matrix m{};

			m(0, 0) = c2 * c3;
			m(0, 1) = -c1 * s3 + s1 * s2 * c3;
			m(0, 2) = s1 * s3 + c1 * s2 * c3;
			m(0, 3) = static_cast<T>(0.f);

			m(1, 0) = c2 * s3;
			m(1, 1) = c1 * c3 + s1 * s2 * s3;
			m(1, 2) = -s1 * c3 + c1 * s2 * s3;
			m(1, 2) = static_cast<T>(0.f);

			m(2, 0) = -s2;
			m(2, 1) = s1 * c2;
			m(2, 2) = c1 * c2;

			m(3, 3) = static_cast<T>(1.f);

			return m;
		}

		constexpr static Matrix MakeScaleMatrix(const T x, const T y, const T z)
		{
			static_assert(R >= 4, "Matrix::MakeScaleMatrix(x,y,z) > a 3D scale matrix requires a 4D square matrix");
			static_assert(C >= 4, "Matrix::MakeScaleMatrix(x,y,z) > a 3D scale matrix requires a 4D square matrix");
			static_assert(R == C, "Matrix::MakeScaleMatrix() > Matrix must be square");

			Matrix m{ MakeIdentityMatrix() };

			m(0, 0) = x;
			m(1, 1) = y;
			m(2, 2) = z;

			return m;
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

		constexpr int maxMatrixLength{ C1 };

		for (int r{}; r < R1; ++r)
		{
			for (int c{}; c < C2; ++c)
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

		for (int r{}; r < R; ++r)
		{
			for (int c{}; c < C; ++c)
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

		for (int r{}; r < R; ++r)
		{
			for (int c{}; c < C; ++c)
			{
				matrix.Data[r][c] += m.Data[r][c] / val;
			}
		}

		return matrix;
	}

	template<int R, int C, typename T>
	Matrix<R, C, T> operator+(const Matrix<R, C, T>& m1, const Matrix<R, C, T>& m2)
	{
		Matrix<R, C, T> matrix{};

		for (int r{}; r < R; ++r)
		{
			for (int c{}; c < C; ++c)
			{
				matrix.Data[r][c] = m1.Data[r][c] + m2.Data[r][c];
			}
		}

		return matrix;
	}

	template<int R, int C, typename T>
	Matrix<R, C, T> operator-(const Matrix<R, C, T>& m1, const Matrix<R, C, T>& m2)
	{
		Matrix<R, C, T> matrix{};

		for (int r{}; r < R; ++r)
		{
			for (int c{}; c < C; ++c)
			{
				matrix.Data[r][c] = m1.Data[r][c] - m2.Data[r][c];
			}
		}

		return matrix;
	}

	template<int R, int C, typename T>
	bool operator==(const Matrix<R, C, T>& m1, const Matrix<R, C, T>& m2)
	{
		for (int r{}; r < R; ++r)
		{
			for (int c{}; c < C; ++c)
			{
				if (m1(r, c) != m2(r, c))
				{
					return false;
				}
			}
		}

		return true;
	}

	template<int R, int C, typename T>
	bool operator!=(const Matrix<R, C, T>& m1, const Matrix<R, C, T>& m2)
	{
		return !(m1 == m2);
	}
}
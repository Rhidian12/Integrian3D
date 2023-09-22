#pragma once

#include "EngineConstants.h"

#pragma warning( push )
#pragma warning( disable : 4201 )
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#pragma warning( pop )

#include <limits> /* std::numeric_limits */
#include <random> /* std::random_device */
#include <type_traits> /* std::enable_if */

namespace Integrian3D::Math
{
#pragma region Using
	using Vec2D = glm::vec<2, double>;
	using Vec2F = glm::vec<2, float>;
	using Vec3D = glm::vec<3, double>;
	using Vec3F = glm::vec<3, float>;
	using Vec4D = glm::vec<4, double>;
	using Vec4F = glm::vec<4, float>;
	using Mat4D = glm::mat<4, 4, double>;
	using Mat4F = glm::mat<4, 4, double>;
	using QuatD = glm::qua<double, glm::packed_highp>;
#pragma endregion

#pragma region Variables
	inline constexpr double Pi{ 3.14159265358979323846264338327950288 };
	inline constexpr double Pi_2{ Pi / 2.0 };
	inline constexpr double Pi_4{ Pi / 4.0 };

	inline constexpr Vec3D Up{ 0.0, 1.0, 0.0 };
	inline constexpr Vec3D Right{ 1.0, 0.0, 0.0 };
	inline constexpr Vec3D Forward{ 0.0, 0.0, 1.0 };

	namespace Detail
	{
		inline uint32_t Seed{}; /* Set in Core::CreateCore() once at engine start up, can be set later as well by the user */
		inline std::mt19937 RNGEngine{ Seed };
	}
#pragma endregion

	void SetSeed(const uint32_t seed);

	template<typename T>
	__NODISCARD constexpr T ToRadians(const T deg)
	{
		static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>, "Math::ToRadians<T>() > T must be an integral or floating point type");

		return static_cast<T>(deg * Pi / 180.0);
	}

	template<typename T>
	__NODISCARD constexpr T ToDegrees(const T rad)
	{
		static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>, "Math::ToDegrees<T>() > T must be an integral or floating point type");

		return static_cast<T>(rad * 180.0 / Pi);
	}

	template<typename T>
	__NODISCARD constexpr bool AreEqual(const T a, const T b, const T epsilon = std::numeric_limits<T>::epsilon())
	{
		static_assert(std::is_fundamental_v<T>, "Math::AreEqual<T>() > T must be a fundamental type");

		return static_cast<T>(abs(a - b)) <= epsilon;
	}

	template<typename T>
	constexpr void Clamp(T& value, const T& min, const T& max)
	{
		if (value < min)
		{
			value = min;
		}

		if (value > max)
		{
			value = max;
		}
	}

	/// <summary>
	/// Is a value in the range of [min, max)
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="value">: Value to check</param>
	/// <param name="min">: Min value, inclusive</param>
	/// <param name="max">: Max value, exclusive</param>
	template<typename T>
	__NODISCARD constexpr bool IsInRange(const T& value, const T& min, const T& max)
	{
		if (value < min)
			return false;

		if (value >= max)
			return false;

		return true;
	}

#pragma region Random
	/// <summary>
	/// Returns a random floating point value in the range of [min, max)
	/// </summary>
	/// <typeparam name="T">: A floating point type</typeparam>
	/// <param name="min">: Min random value, inclusive</param>
	/// <param name="max">: Max random value, exclusive</param>
	/// <returns></returns>
	template<typename T>
	__NODISCARD T RandomF(const T min, const T max)
	{
		static_assert(std::is_floating_point_v<T>, "Math::RandomF<T>() > T must be a floating point");

		std::uniform_real_distribution<T> dist(min, max);

		return dist(Detail::RNGEngine);
	}

	/// <summary>
	/// Returns a random integral value in the range of [min, max]
	/// </summary>
	/// <typeparam name="T">: An integral value</typeparam>
	/// <param name="min">: Min random value, inclusive</param>
	/// <param name="max">: Max random value, inclusive</param>
	/// <returns></returns>
	template<typename T>
	__NODISCARD T RandomI(const T min, const T max)
	{
		static_assert(std::is_integral_v<T>, "Math::RandomI<T>() > T must be an integral type");

		std::uniform_int_distribution<T> dist(min, max);

		return dist(Detail::RNGEngine);
	}

	__NODISCARD constexpr Vec2D RandomVec2D(const double min, const double max)
	{
		return Vec2D{ RandomF(min, max),RandomF(min, max) };
	}
	__NODISCARD constexpr Vec3D RandomVec3D(const double min, const double max)
	{
		return Vec3D{ RandomF(min, max),RandomF(min, max),RandomF(min, max) };
	}
	__NODISCARD constexpr Vec4D RandomVec4D(const double min, const double max)
	{
		return Vec4D{ RandomF(min, max),RandomF(min, max),RandomF(min, max),RandomF(min, max) };
	}

	__NODISCARD constexpr Vec2F RandomVec2F(const float min, const float max)
	{
		return Vec2F{ RandomF(min, max),RandomF(min, max) };
	}
	__NODISCARD constexpr Vec3F RandomVec3F(const float min, const float max)
	{
		return Vec3F{ RandomF(min, max),RandomF(min, max),RandomF(min, max) };
	}

	__NODISCARD constexpr Vec4F RandomVec4F(const float min, const float max)
	{
		return Vec4F{ RandomF(min, max),RandomF(min, max),RandomF(min, max),RandomF(min, max) };
	}
#pragma endregion

	template<typename T>
	__NODISCARD __INLINE constexpr T Max(const T& a, const T& b)
	{
		return a < b ? b : a;
	}
	template<typename T>
	__NODISCARD __INLINE constexpr T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<int64_t A, int64_t B>
	__NODISCARD constexpr int64_t GetGCD()
	{
		if constexpr (B == 0)
			return A;
		else
			return GetGCD<B, A% B>();
	}
	template<typename T>
	__NODISCARD constexpr T GetGCD(const T a, const T b)
	{
		static_assert(std::is_integral_v<T>, "Math::GetGCD<T>() > T must be an integral type");

		if (b == 0)
			return a;
		else
			return GetGCD(b, a % b);
	}
}
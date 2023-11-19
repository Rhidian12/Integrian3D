#pragma once

#undef max
#undef min

	/* [[nodiscard]] */
#define __NODISCARD [[nodiscard]]

	/* inline */
#ifdef _WIN32
#	define __INLINE __forceinline
#elif
#	define __INLINE inline
#endif

	/* RNG seed */
#ifndef SEED
#define SEED rand() % RAND_MAX
#endif

	/* std::move */
// [CRINGE]: just remove this for now
#define I_MOVE(val) std::move(val)

	/* std::forward */
#ifdef _DEBUG
#define __FORWARD(val) std::forward<decltype((val))>((val))
#else
#define __FORWARD(val) std::forward<decltype((val))>((val))
#endif

#define __DELETE(p) do { delete p; p = nullptr; } while (0);
#define __DELETEARR(p) do { delete[] (p); (p) = nullptr; } while (0);

#define I_DISABLE_WARNING(WarningNr)	__pragma(warning( push )) \
										__pragma(warning(disable:WarningNr))
#define I_ENABLE_WARNING(WarningNr)	__pragma(warning( pop ))

#define CONCAT_IMPL(x, y) x ## y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
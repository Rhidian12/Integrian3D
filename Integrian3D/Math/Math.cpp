#include "Math.h"

namespace Integrian3D::Math
{
	void SetSeed(const uint32_t seed)
	{
		Detail::Seed = seed;
		Detail::RNGEngine = std::mt19937{ seed };
	}
}
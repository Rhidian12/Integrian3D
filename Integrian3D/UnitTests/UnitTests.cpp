#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include "Logger/Logger.h"

int RunUnitTests(int argc, char* argv[])
{
	const int ReturnVal = Catch::Session().run(argc, argv);

	Integrian3D::Logger::Cleanup();

	return ReturnVal;
}
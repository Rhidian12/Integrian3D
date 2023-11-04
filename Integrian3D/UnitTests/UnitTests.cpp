#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

int RunUnitTests(int argc, char* argv[])
{
	return Catch::Session().run(argc, argv);
}
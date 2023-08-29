#include <catch.hpp>

#include "../IO/IAsset/IAssetWriter.h"
#include "../IO/IAsset/IAssetReader.h"
#include "../Math/Math.h"

struct PODTestData final
{
	int A;
	float B;
	bool C;
	double D;
	char E;
};

TEST_CASE("Writing and Reading a .iasset file")
{
	using namespace Integrian3D::IO;
	using namespace Integrian3D::Math;

	SECTION("Testing POD data")
	{
		PODTestData data{};
		data.A = 15;
		data.B = 30.f;
		data.C = true;
		data.D = 60.0;
		data.E = 'F';

		SECTION("Writing a .iasset file")
		{
			IAssetWriter writer{ "Resources/TestIAsset" };

			writer.Serialize(data);
		}

		SECTION("Reading a .iasset file")
		{
			IAssetReader reader{ "Resources/TestIAsset" };

			PODTestData newData{ reader.Deserialize<PODTestData>() };

			REQUIRE(data.A == newData.A);
			REQUIRE(AreEqual(data.B, newData.B));
			REQUIRE(data.C == newData.C);
			REQUIRE(AreEqual(data.D, newData.D));
			REQUIRE(data.E == newData.E);
		}
	}
}
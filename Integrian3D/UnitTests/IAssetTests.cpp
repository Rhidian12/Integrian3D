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

Integrian3D::IO::File& operator<<(Integrian3D::IO::File& File, const PODTestData& Data)
{
	File << Data.A << Data.B << Data.C << Data.D << Data.E;

	return File;
}

const Integrian3D::IO::File& operator>>(const Integrian3D::IO::File& File, PODTestData& Data)
{
	File >> Data.A >> Data.B >> Data.C >> Data.D >> Data.E;

	return File;
}

TEST_CASE("Writing and Reading a .iasset file")
{
	using namespace Integrian3D::IO;
	using namespace Integrian3D::Math;

	SECTION("Testing POD data")
	{
		PODTestData Data{};
		Data.A = 15;
		Data.B = 30.f;
		Data.C = true;
		Data.D = 60.0;
		Data.E = 'F';

		SECTION("Writing and Reading a .iasset file")
		{
			IAssetWriter::Serialize("Resources/TestIAsset.iasset", Data);

			PODTestData DeserializedData{};
			IAssetReader::Deserialize("Resources/TestIAsset.iasset", DeserializedData);

			REQUIRE(Data.A == DeserializedData.A);
			REQUIRE(AreEqual(Data.B, DeserializedData.B));
			REQUIRE(Data.C == DeserializedData.C);
			REQUIRE(AreEqual(Data.D, DeserializedData.D));
			REQUIRE(Data.E == DeserializedData.E);
		}
	}
}
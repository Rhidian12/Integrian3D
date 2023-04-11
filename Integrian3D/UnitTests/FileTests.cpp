#include <catch.hpp>

#include "../IO/File/File.h"
#include "../IO/Serializer/Serializer.h"
#include "../Math/Math.h"

#include <string>

TEST_CASE("Testing the File")
{
	using namespace Integrian3D::IO;
	using namespace Integrian3D;

	SECTION("Writing to a file some text")
	{
		File file{ "Resources/TestASCIIFile.txt" };

		file.ClearBuffer();
		REQUIRE(file.GetBuffer().Size() == 0);

		file << 5 << '\n';
		file << 16.f << '\n';
		file << "This is a test string\n";
		file << "This is a second test string on a new line";

		REQUIRE(file.GetBuffer().Size() > 0);

		file.Write();
	}

	SECTION("Reading a file with some text")
	{
		File file{ "Resources/TestASCIIFile.txt" };

		REQUIRE(file.GetBuffer().Size() > 0);

		REQUIRE(std::stoi(file.GetLine()) == 5);
		REQUIRE(Math::AreEqual(std::stof(file.GetLine()), 16.f));
		REQUIRE(file.GetLine() == "This is a test string\n");
		REQUIRE(file.GetLine() == "This is a second test string on a new line");
	}

	SECTION("Writing to a file some text using a specific delimiter")
	{
		File file{ "Resources/TestASCIIFile.txt" };

		file.ClearBuffer();
		REQUIRE(file.GetBuffer().Size() == 0);

		file << 5 << ',';
		file << 16.f << ',';
		file << "This is a test string,";
		file << "This is a second test string on a new line";

		REQUIRE(file.GetBuffer().Size() > 0);

		file.Write();
	}

	SECTION("Reading a file with some text using a specific delimiter")
	{
		File file{ "Resources/TestASCIIFile.txt" };

		REQUIRE(file.GetBuffer().Size() > 0);

		REQUIRE(std::stoi(file.GetLine(',')) == 5);
		REQUIRE(Math::AreEqual(std::stof(file.GetLine(',')), 16.f));
		REQUIRE(file.GetLine(',') == "This is a test string,");
		REQUIRE(file.GetLine(',') == "This is a second test string on a new line");
	}
}
#include <catch.hpp>

#include "../IO/File/File.h"
#include "../IO/Serializer/Serializer.h"
#include "../Math/Math.h"

#include <string>

struct FileTestData final
{
	std::string Word;
	int Number;
};

Integrian3D::IO::File& operator<<(Integrian3D::IO::File& File, const FileTestData& Data)
{
	File << Data.Number << "\n";
	File << Data.Word << "\n";

	return File;
}

TEST_CASE("Testing the File")
{
	using namespace Integrian3D::IO;
	using namespace Integrian3D;

	SECTION("Writing to a file some text")
	{
		File File{ "Resources/TestASCIIFile.txt", OpenMode::CreateAlways, FileMode::ASCII };

		REQUIRE(File.GetFilesize() == 0);

		File << 14562 << "\n";
		File << 16.35f << "\n";
		File << "This is some string";
		File << "\tThis is another string";

		REQUIRE(File.GetFilesize() > 0);
	}

	SECTION("Reading a file with some text")
	{
		File File{ "Resources/TestASCIIFile.txt", OpenMode::OpenExisting, FileMode::ASCII };

		REQUIRE(File.GetFilesize() > 0);

		const std::string FileContents{ File.GetFileContents() };
		const std::string ContentsToCompare{ "14562\n16.35\nThis is some string\tThis is another string" };

		REQUIRE(FileContents == ContentsToCompare);
	}

	SECTION("Writing a Custom Type to a text file")
	{
		File File{ "Resources/TestASCIIFile.txt", OpenMode::CreateAlways, FileMode::ASCII };

		REQUIRE(File.GetFilesize() == 0);

		FileTestData Data{};
		Data.Number = 15;
		Data.Word = "Hello World!";

		File << Data;

		REQUIRE(File.GetFilesize() > 0);
	}

	SECTION("Reading a text file with a Custom Type")
	{
		File File{ "Resources/TestASCIIFile.txt", OpenMode::OpenExisting, FileMode::ASCII };

		REQUIRE(File.GetFilesize() > 0);

		const std::string FileContents{ File.GetFileContents().c_str() };
		const std::string ContentsToCompare{ "15\nHello World!\n" };

		REQUIRE(FileContents == ContentsToCompare);
	}
}
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
	File << Data.Number << "\n" << Data.Word << "\n";

	return File;
}

const Integrian3D::IO::File& operator>>(const Integrian3D::IO::File& File, FileTestData& Data)
{
	File >> Data.Number >> Data.Word;

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
		const File File{ "Resources/TestASCIIFile.txt", OpenMode::OpenExisting, FileMode::ASCII };

		REQUIRE(File.GetFilesize() > 0);

		const std::string FileContents{ File.GetFileContents() };
		const std::string ContentsToCompare{ "14562\n16.35\nThis is some string\tThis is another string" };

		REQUIRE(FileContents == ContentsToCompare);

		int A{};
		float B{};
		std::string C{};

		File >> A >> B >> C;

		REQUIRE(A == 14562);
		REQUIRE(Math::AreEqual(B, 16.35f));
		REQUIRE(C == "This is some string\tThis is another string");
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
		const File File{ "Resources/TestASCIIFile.txt", OpenMode::OpenExisting, FileMode::ASCII };

		REQUIRE(File.GetFilesize() > 0);

		const std::string FileContents{ File.GetFileContents().c_str() };
		const std::string ContentsToCompare{ "15\nHello World!\n" };

		REQUIRE(FileContents == ContentsToCompare);

		FileTestData Data{};
		File >> Data;

		REQUIRE(Data.Number == 15);
		REQUIRE(Data.Word == "Hello World!");
	}

	SECTION("Writing a Binary file with some simple information")
	{
		File File{ "Resources/TestBinaryFile.bin", OpenMode::CreateAlways, FileMode::Binary };

		REQUIRE(File.GetFilesize() == 0);

		File << 15 << 265 << 1831 << 15.6 << 3684.51f << 'D' << "HelloWorld!";

		REQUIRE(File.GetFilesize() > 0);
	}
	
	SECTION("Reading a Binary file with some simple information")
	{
		const File File{ "Resources/TestBinaryFile.bin", OpenMode::OpenExisting, FileMode::Binary };

		REQUIRE(File.GetFilesize() > 0);

		int32 A{}, B{}, C{};
		double D{};
		float E{};
		char F{};
		std::string G{};
		File >> A >> B >> C >> D >> E >> F >> G;

		REQUIRE(A == 15);
		REQUIRE(B == 265);
		REQUIRE(C == 1831);
		REQUIRE(Math::AreEqual(D, 15.6, 0.1));
		REQUIRE(Math::AreEqual(E, 3684.51f, 0.1f));
		REQUIRE(F == 'D');
		REQUIRE(G == "HelloWorld!");
	}
}
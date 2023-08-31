//#include <catch.hpp>
//
//#include "../IO/File/File.h"
//#include "../IO/Serializer/Serializer.h"
//#include "../Math/Math.h"
//
//class NonPOD final
//{
//public:
//	NonPOD()
//		: m_A{}
//		, m_B{}
//	{}
//	NonPOD(const int a, const double b)
//		: m_A{ a }
//		, m_B{ b }
//	{}
//
//	std::string Serialize() const
//	{
//		std::string data{};
//		data.append(std::to_string(m_A) + ",");
//		data.append(std::to_string(m_B));
//
//		return data;
//	}
//
//	void Deserialize(std::string data)
//	{
//		m_A = std::stoi(data.substr(0, data.find_first_of(',')));
//		data = data.substr(data.find_first_of(',') + 1);
//
//		m_B = std::stod(data);
//	}
//
//	int GetA() const { return m_A; }
//	double GetB() const { return m_B; }
//
//private:
//	int m_A;
//	double m_B;
//};
//
//template<>
//__NODISCARD __INLINE Integrian3D::TArray<char> Serialize<NonPOD>(const NonPOD& data)
//{
//	Integrian3D::TArray<char> arr{ Serialize(data.GetA()) };
//
//	Integrian3D::TArray<char> arr2{ Serialize(data.GetB()) };
//
//	arr.AddRange(arr2.cbegin(), arr2.cend());
//
//	return arr;
//}
//
//template<>
//__NODISCARD __INLINE NonPOD Deserialize<NonPOD>(const Integrian3D::TArray<char>& data)
//{
//	const int a{ Deserialize<int>(data) };
//	const double b{ Deserialize<double>(data.SubArray(sizeof(int))) };
//
//	return NonPOD{ a,b };
//}
//
//TEST_CASE("Testing Writing and Reading Binary files")
//{
//	using namespace Integrian3D::IO;
//	using namespace Integrian3D::Math;
//
//	SECTION("Testing Simple POD data")
//	{
//		{
//			File file{ "Resources/TestBinaryFile.bin" };
//			file.ClearBuffer();
//
//			file.Write(Serialize(5));
//			file.Write(Serialize(10.0));
//			file.Write(Serialize(15.f));
//			file.Write(Serialize('c'));
//			file.Write(Serialize(36u), SeekMode::Start);
//
//			const uint64_t constInt{ 951060u };
//			file.Write(Serialize(constInt));
//
//			file.Write();
//		}
//
//		{
//			File file{ "Resources/TestBinaryFile.bin" };
//
//			REQUIRE(Deserialize<uint32_t>(file.Read<uint32_t>()) == 36u);
//			REQUIRE(Deserialize<int>(file.Read<int>()) == 5);
//			REQUIRE(AreEqual(Deserialize<double>(file.Read<double>()), 10.0));
//			REQUIRE(AreEqual(Deserialize<float>(file.Read<float>()), 15.f));
//			REQUIRE(Deserialize<char>(file.Read<char>()) == 'c');
//			REQUIRE(Deserialize<uint64_t>(file.Read<uint64_t>()) == 951060u);
//		}
//	}
//
//	SECTION("Testing Custom POD data")
//	{
//		struct POD final
//		{
//			int A;
//			float B;
//			char C;
//			double D;
//			unsigned int E;
//		};
//
//		POD data{};
//		data.A = 15;
//		data.B = 36.f;
//		data.C = 'w';
//		data.D = -680.0;
//		data.E = 68661u;
//
//		{
//			File file{ "Resources/TestBinaryFile.bin" };
//			file.ClearBuffer();
//
//			file.Write(Serialize(data));
//
//			file.Write();
//		}
//
//		{
//			File file{ "Resources/TestBinaryFile.bin" };
//
//			const POD newData{ Deserialize<POD>(file.Read<POD>()) };
//
//			REQUIRE(newData.A == data.A);
//			REQUIRE(AreEqual(newData.B, data.B));
//			REQUIRE(newData.C == data.C);
//			REQUIRE(AreEqual(newData.D, data.D));
//			REQUIRE(newData.E == data.E);
//		}
//	}
//
//	SECTION("Testing custom non-POD data")
//	{
//		NonPOD data{ 974654, -651.64 };
//
//		{
//			File file{ "Resources/TestBinaryFile.bin" };
//			file.ClearBuffer();
//
//			file.Write(Serialize(data));
//
//			file.Write();
//		}
//
//		{
//			File file{ "Resources/TestBinaryFile.bin" };
//
//			const NonPOD newData{ Deserialize<NonPOD>(file.Read<NonPOD>()) };
//
//			REQUIRE(newData.GetA() == data.GetA());
//			REQUIRE(AreEqual(newData.GetB(), data.GetB()));
//		}
//	}
//}
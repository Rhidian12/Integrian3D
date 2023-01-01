#pragma once

#include "../../EngineConstants.h"
#include "../Binary/BinaryWriter.h"

namespace Integrian3D::IO
{
	struct PODTestData final
	{
		int A;
		float B;
		bool C;
		double D;
		char E;
	};

	class TestConverter final
	{
	public:
		void Serialize(BinaryWriter& writer, const PODTestData& data)
		{
			writer.Write(data);
		}

		void Deserialize(BinaryReader& reader, PODTestData& data)
		{
			reader.Read(data);
		}

		PODTestData Deserialize(BinaryReader& reader)
		{
			return reader.Read<PODTestData>();
		}
	};
}
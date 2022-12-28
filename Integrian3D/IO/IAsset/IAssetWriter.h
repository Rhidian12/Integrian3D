#pragma once

#include "../../EngineConstants.h"

#include <string> /* std::string */

namespace Integrian3D::IO
{
	/* Inheritance, Template (Specialisation) or if-constexpr (using an enum) */
	/* Design-wise, I'm not sure what would be best here. I'm guessing inheritance */
	/* However, it would suck to have to every time instantiate the correct class instead of this one */
	/* Inheritance would also make RAII a bitch */

	/// <summary>
	/// An RAII .iasset file writer	that can write
	/// </summary>
	class IAssetWriter final
	{
	public:
		explicit IAssetWriter(std::string filepath);
		~IAssetWriter();

	private:
		void* m_pHandle;
	};
}
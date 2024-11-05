#ifndef AMMO_H
#define AMMO_H

#include "SkyPatcher/utility.h"
#include "CLIBUtil/utils.hpp"

namespace AMMO
{
	struct line_content
	{
		std::vector<std::string>              ammo;//
		std::string                           damage;//
		std::string                           damageMult;//
		std::string                           damageToAdd;//
		std::vector<std::string>              objectExcluded;//
		std::string                           projectile;//
		std::string                           weightLessThan;//
		std::vector<std::string>              keywords;//
		std::vector<std::string>              keywordsOr;//
		std::vector<std::string>              keywordsExcluded;//
		std::vector<std::string>              modNames;//
		std::string                           restrictToBolts;//
		std::string                           speed;//
		std::string                           speedMult;//
		std::string                           gravity;//
		std::string                           gravityMult;//
		std::vector<std::string>              alternateTexturesFind;//
		std::vector<std::string>              filterByEditorIdContains;//
		std::vector<std::string>              filterByEditorIdContainsOr;//
		std::vector<std::string>              filterByEditorIdContainsExcluded;//
	};

	line_content                    create_patch_instruction_ammo(const std::string& line);
	const std::vector<line_content> readConfig(const std::string& folder);
}

#endif

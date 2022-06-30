//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef oredef_hpp
#define oredef_hpp

#include <cstdint>
#include <string>
#include <map>
#include <filesystem>

#include "uoxtype.hpp"
#include "secparser.hpp"
//====================================================================================================
// oredefentry_t
//====================================================================================================
//====================================================================================================
struct oredefentry_t {
	color_t color ;
	int menu ;
	int threshold ;
	std::string name ;
	int chance ;
	oredefentry_t();
};
//====================================================================================================
// oredef_t
//====================================================================================================
//====================================================================================================
struct oredef_t : public secparser {
private:
	oredefentry_t *currentsection ;
	static const std::filesystem::path location ;
	auto startSection(const std::string &secline)->void final;
	auto keyvalue(const std::string &key, const std::string &value)->void final;
	auto endSection() ->void final;

public:
	std::map<std::string, oredefentry_t> ores ;
	auto size() const ->size_t;
	
	auto load(const std::filesystem::path &serverdata) ->bool ;
	oredef_t(const std::filesystem::path &serverdata = std::filesystem::path());
};

#endif /* oredef_hpp */

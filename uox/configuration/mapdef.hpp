//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef mapdef_hpp
#define mapdef_hpp

#include <cstdint>
#include <string>
#include <filesystem>
#include <utility>
#include <map>

#include "secparser.hpp"

//====================================================================================================
struct mapdef_t {
	int mapnum ;
	std::filesystem::path mappath ;
	std::filesystem::path uoppath ;
	std::filesystem::path stamul ;
	std::filesystem::path staidx ;
	std::filesystem::path mapdif ;
	std::filesystem::path mapdifl ;
	std::filesystem::path stadifl ;
	std::filesystem::path stadifi ;
	std::filesystem::path stadif ;
	std::pair<int,int> size ;
	mapdef_t(int mapnumber = -1):mapnum(mapnumber){size = std::make_pair(0, 0);}
};
//====================================================================================================
class mapdef : public secparser {
	
	mapdef_t current ;
	bool active ;
	auto startSection(const std::string &secline)->void final;
	auto keyvalue(const std::string &key, const std::string &value)->void final ;
	auto endSection() ->void final;

public:
	std::map<int, mapdef_t> mapdefinitions ;
	auto load(const std::filesystem::path &datapath) ->bool ;
	mapdef(const std::filesystem::path &datapath = std::filesystem::path()) ;
	auto size() const ->size_t ;
	auto operator[](int index) const ->const mapdef_t& ;
	auto operator[](int index) ->mapdef_t&;
};
#endif /* mapdef_hpp */

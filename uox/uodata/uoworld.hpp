//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef uoworld_hpp
#define uoworld_hpp

#include <cstdint>
#include <string>
#include <map>
#include <filesystem>

#include "secparser.hpp"
#include "ultimamap.hpp"
#include "tiledata.hpp"
#include "multicollection.hpp"

class mapdef ;
struct mapdef_t ;
//====================================================================================================
class uoworld : public secparser {
	std::map<int, ultimamap> uomaps ;
	tiledata tileinfo ;
	multicollection uomulti ;

	bool tile_active ;
	int current_tile ;
	
	auto startSection(const std::string &secline)->void final;
	auto keyvalue(const std::string &key, const std::string &value)->void final;
	auto endSection() ->void final;

	auto processFlag0(const std::string &lkey, const std::string &value) ->bool ;
	auto processFlag1(const std::string &lkey, const std::string &value) ->bool ;
	auto processFlag2(const std::string &lkey, const std::string &value) ->bool ;
	auto processFlag3(const std::string &lkey, const std::string &value) ->bool ;

	auto processMap(ultimamap &map,const  mapdef_t &entry, const std::filesystem::path &uodir) ->bool ;
	auto processOverrides(const std::filesystem::path &serverdata) ->void ;
public:
	uoworld() = default ; ;
	auto load(std::filesystem::path &serverdata, std::filesystem::path &uodir, const mapdef &mapdefs ) ->bool ;
	auto size() const ->size_t ;
	auto world(int number) const ->const ultimamap& ;
	auto world(int number)  -> ultimamap& ;

	auto multi(int number) const -> const collection_item& ;
	auto multi(int number)  ->  collection_item& ;
};

#endif /* uoworld_hpp */

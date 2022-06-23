//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef servcfg_hpp
#define servcfg_hpp

#include <cstdint>
#include <string>
#include <limits>
#include <vector>
#include <array>
#include <filesystem>

#include "secparser.hpp"
#include "dictionary.hpp"

//====================================================================================================
enum clientenable_t {
	client4000,
	client5000,
	client6000,
	client6050,
	client7000,
	client70160,
	client70240,
	client70300,
	client70331,
	client704565,
	client70610,
	totalclients
};

//====================================================================================================
// startlocation_t
//====================================================================================================
struct startlocation_t {
	std::string town ;
	std::string description ;
	std::int32_t x;
	std::int32_t y;
	std::int32_t z ;
	std::int32_t mapnum ;
	std::int32_t realm;
	std::uint32_t cliloc ;
	startlocation_t() ;
	startlocation_t(const std::string &town,const std::string &descrip,int x, int y, int z, int mapnum,int realm, int cliloc);
	startlocation_t(const std::string &value) ;
};

//====================================================================================================
// servercfg
//====================================================================================================
struct servercfg : public secparser {
private:
	enum loadsection {
		invalid,startlocation,clientsupport,server
	};
	loadsection current_load ;

	auto startSection(const std::string &secline)->void final ;
	auto keyvalue(const std::string &key, const std::string &value)->void final ;
	auto endSection() ->void final ;

	auto processClientSupport(const std::string &key, const std::string &value) ->void ;
	auto processStartLocation(const std::string &key, const std::string &value) ->void ;
	auto processServer(const std::string &key, const std::string &value) ->void ;

public:
	DistinctLanguage serverlanguage ;
	std::vector<startlocation_t> startlocations ;
	std::array<bool,clientenable_t::totalclients> enabledclients ;
	servercfg() ;
	servercfg(const std::filesystem::path &serverdata) ;
	auto load(const std::filesystem::path &serverdata) ->bool ;
};

//====================================================================================================

#endif /* servcfg_hpp */

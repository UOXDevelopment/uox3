//Copyright © 2022 Charles Kerr. All rights reserved.

#include "telelocation.hpp"

#include "strutil.hpp"

#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std::string_literals;


//====================================================================================================
const std::filesystem::path telelocation_t::location = std::filesystem::path("configuration")/ std::filesystem::path("settings") / std::filesystem::path("teleport.cfg");
//====================================================================================================
telelocation_t::telelocation_t(const std::filesystem::path &serverdata):insection(false){
	if (!serverdata.empty()){
		if (!load(serverdata)){
			throw std::runtime_error("Unable to process teleport locations: "s+serverdata.string());
		}
	}
}

//====================================================================================================
auto telelocation_t::load(const std::filesystem::path &serverdata) ->bool {
	locations.clear();
	insection = false ;
	auto rvalue = processFile(serverdata / location) ;
	// now sort it?
	std::sort(locations.begin(),locations.end(),[](const duallocation_t &lhs, const duallocation_t &rhs){
		return lhs.source.x < rhs.source.x ;
	});
	return rvalue ;
}
//====================================================================================================
auto telelocation_t::startSection(const std::string &secline)->void {
	auto [sec,type] = strutil::split(secline," ");
	if ((strutil::lower(sec) == "teleport") && (strutil::lower(type) == "entries")){
		insection = true ;
	}
}
//====================================================================================================
auto telelocation_t::keyvalue(const std::string &key, const std::string &value)->void {
	if (insection){
		if (strutil::lower(key) == "locations") {
			auto [src,dst] = strutil::split(value,":");
			auto loc = duallocation_t() ;
			loc.source = location_t(src) ;
			loc.destination = location_t(dst) ;
			locations.push_back(loc);
		}
	}
}
//====================================================================================================
auto telelocation_t::endSection() ->void {
	insection = false ;
}
//====================================================================================================
auto telelocation_t::size() const ->size_t {
	return locations.size();
}

//Copyright © 2022 Charles Kerr. All rights reserved.

#include "mapdef.hpp"
#include "strutil.hpp"

#include <iostream>
#include <stdexcept>

using namespace std::string_literals;
//====================================================================================================
mapdef::mapdef(const std::filesystem::path &datapath):active(false){
	if (!datapath.empty()){
		if (!load(datapath)){
			throw std::runtime_error("Unable to process configuration/maps.cfg at: "s+datapath.string());
		}
	}
}
//====================================================================================================
auto mapdef::load(const std::filesystem::path &datapath) ->bool{
	auto path = datapath / std::filesystem::path("configuration") / std::filesystem::path("maps.cfg");
	return processFile(path);
}
//====================================================================================================
//	virtual functions
//====================================================================================================
//====================================================================================================
auto mapdef::startSection(const std::string &secline)->void {
	auto [title,num] = strutil::split(strutil::simplify(secline)," ") ;
	if (strutil::lower(title) == "map"){
		auto number = strutil::ston<int>(num) ;
		current = mapdef_t(number);
		active = true ;
	}
	
}
//====================================================================================================
auto mapdef::keyvalue(const std::string &key, const std::string &value)->void {
	if (active) {
		auto lkey = strutil::lower(key) ;
		if (lkey == "map"){
			current.mappath = std::filesystem::path(value);
		}
		else if (lkey == "uop"){
			current.uoppath = std::filesystem::path(value);
		}
		else if (lkey == "statics"){
			current.stamul = std::filesystem::path(value);
		}
		else if (lkey == "staidx"){
			current.staidx = std::filesystem::path(value);
		}
		else if (lkey == "statics"){
			current.stamul = std::filesystem::path(value);
		}
		else if (lkey == "mapdiff"){
			current.mapdif = std::filesystem::path(value);
		}
		else if (lkey == "mapdifflist"){
			current.mapdifl = std::filesystem::path(value);
		}
		else if (lkey == "staticsdiff"){
			current.stadif = std::filesystem::path(value);
		}
		else if (lkey == "staticsdifflist"){
			current.stadifl = std::filesystem::path(value);
		}
		else if (lkey == "staticsdiffindex"){
			current.stadifi = std::filesystem::path(value);
		}
	}
}
//====================================================================================================
auto mapdef::endSection() ->void {
	if (active){
		mapdefinitions.insert_or_assign(current.mapnum, current);
		active = false ;
	}
}
//====================================================================================================
auto mapdef::size() const ->size_t {
	return mapdefinitions.size() ;
}
//====================================================================================================
auto mapdef::operator[](int index) const ->const mapdef_t& {
	return mapdefinitions.at(index);
}
//====================================================================================================
auto mapdef::operator[](int index) ->mapdef_t&{
	return mapdefinitions.at(index);
}

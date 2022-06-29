//Copyright © 2022 Charles Kerr. All rights reserved.

#include "golocation.hpp"

#include "strutil.hpp"

#include <iostream>
#include <stdexcept>

using namespace std::string_literals;

//====================================================================================================
const std::filesystem::path golocation_t::location = std::filesystem::path("configuration") / std::filesystem::path("settings") / std::filesystem::path("location.cfg");

//====================================================================================================
auto golocation_t::startSection(const std::string &secline)->void {
	auto [sec,num] = strutil::split(secline," ") ;
	goplace = -1 ;
	if (strutil::lower(sec) == "location"){
		goplace = strutil::ston<int>(num) ;
	}
}
//====================================================================================================
auto golocation_t::keyvalue(const std::string &key, const std::string &value)->void {
	if (goplace>-1) {
		if (strutil::lower(key) == "location"){
			places.insert_or_assign(goplace, location_t(value));
		}
	}
}
//====================================================================================================
auto golocation_t::endSection() ->void {
	goplace = -1 ;
}
//====================================================================================================
golocation_t::golocation_t(const std::filesystem::path &serverdata ){
	goplace = -1 ;
	if (!serverdata.empty()){
		if (!load(serverdata)){
			throw std::runtime_error("Unable to load goplaces: "s + (serverdata /location).string());
		}
	}
}
//====================================================================================================
auto golocation_t::load(const std::filesystem::path &serverdata) ->bool {
	places.clear();
	goplace = -1 ;
	return processFile(serverdata/location);
}
//====================================================================================================
auto golocation_t::size() const ->size_t {
	return places.size();
}

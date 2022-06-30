//Copyright © 2022 Charles Kerr. All rights reserved.

#include "oredef.hpp"

#include "strutil.hpp"

#include <iostream>
#include <stdexcept>

using namespace std::string_literals;
//====================================================================================================
// oredefentry_t
//====================================================================================================
//====================================================================================================
oredefentry_t::oredefentry_t(){
	color = 0 ;
	menu = 0 ;
	threshold = 0 ;
	chance = 0;
}
//====================================================================================================
// oredef_t
//====================================================================================================
//====================================================================================================
const std::filesystem::path oredef_t::location = std::filesystem::path("configuration") / std::filesystem::path("settings") / std::filesystem::path("ore.cfg");

//====================================================================================================
auto oredef_t::startSection(const std::string &secline)->void {
	auto [sec,type] = strutil::split(secline," ") ;
	currentsection = nullptr ;
	if ("ore" == strutil::lower(sec) ){
		type = strutil::lower(type);
		ores.insert_or_assign(type, oredefentry_t());
		currentsection = &(ores.at(type));
	}
}
//====================================================================================================
auto oredef_t::keyvalue(const std::string &key, const std::string &value)->void {
	if (currentsection) {
		auto lkey = strutil::lower(key) ;
		if (lkey == "color"){
			currentsection->color = strutil::ston<color_t>(value) ;
		}
		else if (lkey == "menu"){
			currentsection->menu = strutil::ston<int>(value);
		}
		else if (lkey == "threshold"){
			currentsection->threshold = strutil::ston<int>(value) ;
		}
		else if (lkey == "name"){
			currentsection->name = value ;
		}
		else if (lkey == "chance"){
			currentsection->chance = strutil::ston<int>(value) ;
		}
		
	}
}
//====================================================================================================
auto oredef_t::endSection() ->void {
	currentsection = nullptr ;
}
//====================================================================================================
auto oredef_t::size() const ->size_t{
	return ores.size();
}

//====================================================================================================
auto oredef_t::load(const std::filesystem::path &serverdata) ->bool {
	currentsection = nullptr;
	ores.clear();
	return processFile(serverdata/location);
}
//====================================================================================================
oredef_t::oredef_t(const std::filesystem::path &serverdata ){
	if (!serverdata.empty()){
		if (!load(serverdata)){
			throw std::runtime_error("Unable to load ore definitions: "s+(serverdata/location).string());
		}
	}
}

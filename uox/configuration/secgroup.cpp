//Copyright © 2022 Charles Kerr. All rights reserved.

#include "secgroup.hpp"

#include "strutil.hpp"

#include <iostream>

using namespace std::string_literals ;

//====================================================================================================
const std::unordered_map<sectype_t, std::string> secgroup::seclocations{
	{sectype_t::item,"dfndata/item"s},
	{sectype_t::npc,"dfndata/npc"s},
	{sectype_t::region,"dfndata/region"s},
	{sectype_t::create,"dfndata/create"s},
	{sectype_t::misc,"dfndata/misc"s},
	{sectype_t::skill,"dfndata/skill"s},
	{sectype_t::location,"dfndata/location"s},
	{sectype_t::menu,"dfndata/menu"s},
	{sectype_t::spell,"dfndata/spell"s},
	{sectype_t::newbie,"dfndata/newbie"s},
	{sectype_t::title,"dfndata/title"s},
	{sectype_t::advance,"dfndata/advance"s},
	{sectype_t::house,"dfndata/house"s},
	{sectype_t::color,"dfndata/color"s},
	{sectype_t::spawn,"dfndata/spawn"s},
	{sectype_t::race,"dfndata/race"s},
	{sectype_t::weather,"dfndata/weather"s},
	{sectype_t::hard_item,"dfndata/harditem"s},
	{sectype_t::command,"dfndata/command"s},
	{sectype_t::msgboard,"dfndata/msgboard"s},
	{sectype_t::carve,"dfndata/carve"s},
	{sectype_t::creature,"dfndata/creature"s}
};
//====================================================================================================
//		static functions
//====================================================================================================
//====================================================================================================
auto secgroup::pathFor(sectype_t type) -> std::filesystem::path {
	try {
		return std::filesystem::path(seclocations.at(type)) ;
	}
	catch(...) {
		return std::filesystem::path() ;
	}
}
//====================================================================================================
auto secgroup::load(const std::filesystem::path &root, sectype_t type) ->bool {
	auto rvalue = false ;
	sec_type = type ;
	try {
		auto loc = seclocations.at(type) ;
		auto path = root / std::filesystem::path(loc) ;
		rvalue = this->processDirectory(path);
	}
	catch(...) {
		// couldn't find the type to get a directory, just fail
	}
	return rvalue ;
}
//====================================================================================================
auto secgroup::clear() {
	contents.clear() ;
}
//====================================================================================================
auto secgroup::type() const ->sectype_t {
	return sec_type ;
}
//====================================================================================================
auto secgroup::size() const ->size_t {
	return contents.size() ;
}
//====================================================================================================
auto secgroup::operator[](const std::string &name) const ->const section& {
	auto value = strutil::upper(name) ;
	return contents.at(value) ;
}
//====================================================================================================
auto secgroup::operator[](const std::string &name) -> section& {
	auto value = strutil::upper(name) ;
	return contents.at(value) ;
}
//====================================================================================================
auto secgroup::contains(const std::string &name) const ->bool {
	auto iter = contents.find(name) ;
	return iter != contents.end() ;
}
//====================================================================================================
auto secgroup::remove(const std::string &name) ->bool {
	auto iter = contents.find(name) ;
	auto rvalue = false ;
	if (iter != contents.end()){
		rvalue = true ;
		contents.erase(iter);
	}
	return rvalue ;
}

//==================================================================================================
// virtual functions for secparser
//==================================================================================================

//====================================================================================================
auto secgroup::startSection(const std::string &secline)->void {
	
	current_section = section(secline,sec_type);
}
//====================================================================================================
auto secgroup::keyvalue(const std::string &key, const std::string &value)->void {
	if (current_section.type != sectype_t::invalid){
		// we upper the key, for searchs
		current_section.append(strutil::upper(key), value) ;
	}
}
//====================================================================================================
auto secgroup::endSection() ->void {
	if (current_section.type != sectype_t::invalid){
		contents.insert_or_assign(strutil::upper(current_section.label), current_section);
		current_section = section() ;
	}
}
//====================================================================================================
auto secgroup::closeFile() ->void {
	if (current_section.type != sectype_t::invalid){
		// Missing a closing bracket, should we use it anyway, or dispose?
		// we dispose
		current_section = section() ;
	}
}


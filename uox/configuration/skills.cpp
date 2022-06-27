//Copyright © 2022 Charles Kerr. All rights reserved.

#include "skills.hpp"

#include "section.hpp"
#include "strutil.hpp"

#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std::string_literals ;
//====================================================================================================
// constants
//====================================================================================================
//====================================================================================================
const std::map<skilltype_t,std::string> skilldefinition_t::skillnames{
	{skilltype_t::alchemey,"alchemey"s},{skilltype_t::anatomy,"anatomy"s},
	{skilltype_t::animallore,"animallore"s},{skilltype_t::itemid,"itemid"s},
	{skilltype_t::armslore,"armslore"s},{skilltype_t::parrying,"parrying"s},
	{skilltype_t::begging,"begging"s},{skilltype_t::blacksmithing,"blacksmithing"s},
	{skilltype_t::bowcraft,"bowcraft"s},{skilltype_t::peacemaking,"peacemaking"s},
	{skilltype_t::camping,"camping"s},{skilltype_t::carpentry,"carpentry"s},
	{skilltype_t::cartography,"cartography"s},{skilltype_t::cooking,"cooking"s},
	{skilltype_t::detectinghidden,"detectinghidden"s},{skilltype_t::enticement,"enticement"s},
	{skilltype_t::evaluatingintel,"evaluatingintel"s},{skilltype_t::healing,"healing"s},
	{skilltype_t::fishing,"fishing"s},{skilltype_t::forensics,"forensics"s},
	{skilltype_t::herding,"herding"s},{skilltype_t::hiding,"hiding"s},
	{skilltype_t::provocation,"provocation"s},{skilltype_t::inscription,"inscription"s},
	{skilltype_t::lockpicking,"lockpicking"s},{skilltype_t::magery,"magery"s},
	{skilltype_t::magicresistance,"magicresistance"s},{skilltype_t::tatics,"tatics"s},
	{skilltype_t::snooping,"snooping"s},{skilltype_t::musicianship,"musicianship"s},
	{skilltype_t::poisoning,"poisoning"s},{skilltype_t::archery,"archery"s},
	{skilltype_t::spiritspeak,"spiritspeak"s},{skilltype_t::stealing,"stealing"s},
	{skilltype_t::tailoring,"tailoring"s},{skilltype_t::taming,"taming"s},
	{skilltype_t::tasteid,"tasteid"s},{skilltype_t::tinkering,"tinkering"s},
	{skilltype_t::tracking,"tracking"s},{skilltype_t::veterinary,"veterinary"s},
	{skilltype_t::swordsmanship,"swordsmanship"s},{skilltype_t::macefighting,"macefighting"s},
	{skilltype_t::fencing,"fencing"s},{skilltype_t::wrestling,"wrestling"s},
	{skilltype_t::lumberjacking,"lumberjacking"s},{skilltype_t::mining,"mining"s},
	{skilltype_t::meditation,"meditation"s},{skilltype_t::stealth,"stealth"s},
	{skilltype_t::removetrap,"removetrap"s},{skilltype_t::necromancy,"necromancy"s},
	{skilltype_t::focus,"focus"s},{skilltype_t::chivalry,"chivalry"s},
	{skilltype_t::bushido,"bushido"s},{skilltype_t::ninjitsu,"ninjitsu"s},
	{skilltype_t::spellweaving,"spellweaving"s},{skilltype_t::imbuing,"imbuing"s},
	{skilltype_t::mysticism,"mysticism"s},{skilltype_t::throwing,"throwing"s}
};
//====================================================================================================
auto skilldefinition_t::nameFor(skilltype_t type) ->const std::string & {
	static auto emptystring  = std::string();
	try {
		return skillnames.at(type) ;
	}
	catch(...) {
		return emptystring;
	}
}
//====================================================================================================
auto skilldefinition_t::typeFor(const std::string &name) ->skilltype_t {
	auto iter = std::find_if(skillnames.begin(),skillnames.end(),[name](const std::pair<skilltype_t,std::string> &entry){
		return name == entry.second ;
	});
	if (iter != skillnames.end()){
		return iter->first ;
	}
	throw std::runtime_error("Unable find type for skill name: "s + name);
}

//====================================================================================================
// skilldefentry_t
//====================================================================================================
//====================================================================================================

//====================================================================================================
skilldefentry_t::skilldefentry_t(){
	strength = 0 ;
	dexterity = 0 ;
	intelligence = 0 ;
	delay = -1 ;
	verb = "made";
}

//====================================================================================================
// skilldefinition_t
//====================================================================================================
//====================================================================================================
skilldefinition_t::skilldefinition_t(){
	currentsection = nullptr ;
}
//====================================================================================================
auto skilldefinition_t::clear() ->void {
	currentsection = nullptr ;
	skill_definitions.clear() ;
}
//====================================================================================================
auto skilldefinition_t::load(const std::filesystem::path &serverdata) ->bool {
	clear() ;
	auto path = serverdata / std::filesystem::path("skills.cfg") ;
	return processFile(path);
}

//====================================================================================================
auto skilldefinition_t::startSection(const std::string &secline)->void {
	currentsection = nullptr ;
	auto [key,value] = strutil::split(secline," ");
	key = strutil::lower(key) ;
	value = strutil::lower(value);
	if (key == "skill"){
		auto type = skilldefinition_t::typeFor(value) ;
		skill_definitions.insert_or_assign(type, skilldefentry_t());
		currentsection = &skill_definitions.at(type);
	}
}
//====================================================================================================
auto skilldefinition_t::keyvalue(const std::string &key, const std::string &value)->void {
	if (currentsection){
		auto lkey = strutil::lower(key) ;
		if (key == "advance") {
			
		}
		else if (lkey == "strength"){
			currentsection->strength = strutil::ston<int>(value) ;
		}
		else if (lkey == "dexterity"){
			currentsection->dexterity = strutil::ston<int>(value) ;
		}
		else if (lkey == "intelligence"){
			currentsection->intelligence = strutil::ston<int>(value) ;
		}
		else if (lkey == "delay"){
			currentsection->delay = strutil::ston<int>(value) ;
		}
		else if (lkey == "verb"){
			currentsection->verb = value ;
		}
	}
}
//====================================================================================================
auto skilldefinition_t::endSection() ->void {
	currentsection = nullptr ;
}

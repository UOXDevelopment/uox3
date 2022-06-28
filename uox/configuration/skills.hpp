//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef skills_hpp
#define skills_hpp

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include <filesystem>

#include "uoxtype.hpp"
#include "secparser.hpp"

struct section;

enum class skilltype_t {
	alchemy,anatomy,animallore,itemid,armslore,parrying,
	begging,blacksmithing,bowcraft,peacemaking,camping,carpentry,
	cartography,cooking,detectinghidden,enticement,evaluatingintel,healing,
	fishing,forensics,herding,hiding,provocation,inscription,
	lockpicking,magery,magicresistance,tactics,snooping,musicianship,
	poisoning,archery,spiritspeak,stealing,tailoring,taming,
	tasteid,tinkering,tracking,veterinary,swordsmanship,macefighting,
	fencing,wrestling,lumberjacking,mining,meditation,stealth,
	removetrap,necromancy,focus,chivalry,bushido,ninjitsu,
	spellweaving,imbuing,mysticism,throwing
};
//====================================================================================================
// skilldefentry_t
//====================================================================================================
//====================================================================================================
struct skilldefentry_t {
	std::int32_t strength ;
	std::int32_t dexterity ;
	std::int32_t intelligence ;
	std::int32_t delay ;
	std::string verb ;
	std::string name ;
	std::vector<advancement_t> advancement ;
	skilldefentry_t() ;
	auto save(std::ostream &output) const ->void ;
};
//====================================================================================================
// skilldefinition_t
//====================================================================================================
//====================================================================================================
struct skilldefinition_t : public secparser {
private:
	skilldefentry_t *currentsection ;
	auto startSection(const std::string &secline)->void final ;
	auto keyvalue(const std::string &key, const std::string &value)->void final;
	auto endSection() ->void final;
	static const std::filesystem::path location ;

public:
	static const std::map<skilltype_t,std::string> skillnames ;
	static auto nameFor(skilltype_t type) ->const std::string & ;
	static auto typeFor(const std::string &name) ->skilltype_t ;
	std::map<skilltype_t,skilldefentry_t> skill_definitions ;
	auto clear() ->void ;
	auto load(const std::filesystem::path &serverdata) ->bool ;
	skilldefinition_t() ;
	auto save(const std::filesystem::path &serverdata) const ->bool ;
	auto operator[](skilltype_t type) const ->const skilldefentry_t& ;
	auto operator[](skilltype_t type)  -> skilldefentry_t& ;
	auto operator[](skillid_t id) const ->const skilldefentry_t& ;
	auto operator[](skillid_t id)  -> skilldefentry_t& ;
	auto size() const ->size_t;
};
#endif /* skills_hpp */

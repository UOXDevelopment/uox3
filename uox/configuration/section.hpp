//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef section_hpp
#define section_hpp

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <ostream>

//=================================================================================================
//	define the type of section
//=================================================================================================
enum class sectype_t :int {
	item = 0,
	npc,
	region,
	create,
	misc,
	skill,
	location,
	menu,
	spell,
	newbie,
	title,
	advance,
	house,
	color,
	spawn,
	race,
	weather,
	hard_item,
	command,
	msgboard,
	carve,
	creature,
	invalid

};
//====================================================================================================
struct section {
	sectype_t type ;
	std::string label ;
	std::vector<std::pair<std::string,std::string>> contents ;

	section(const std::string &label ="", sectype_t type = sectype_t::invalid) ;
	auto append(const std::string &key, const std::string &value) ->void;
	
	auto size() const -> size_t ;
	auto operator[](int index) const ->const std::pair<std::string,std::string>& ;
	auto operator[](int index) ->std::pair<std::string,std::string>& ;
	auto values(const std::string &key) ->std::vector<std::string> ;
	
	auto description(std::ostream &output) const ->void ;

};
#endif /* section_hpp */

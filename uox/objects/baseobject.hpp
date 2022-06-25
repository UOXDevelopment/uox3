//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef baseobject_hpp
#define baseobject_hpp

#include <cstdint>
#include <string>
#include <ostream>
#include <map>
#include <vector>
#include <array>
#include <utility>

#include "uoxtype.hpp"

class section ;
class objectfactory_t ;

enum class objtype_t{base,item,mob,character,multi,boat,spawn} ;
enum class objfeatures_t{temporary,enabled,total};
//====================================================================================================
struct baseobject_t {

	static const std::map<objtype_t,std::string> object_names ;
	static auto nameForObjType(objtype_t obj) ->const std::string& ;
	static auto typeForObjName(const std::string &name) ->objtype_t ;
	
	struct secobject {
		objtype_t type ;
		serial_t serial;
		std::vector<std::pair<std::string,std::string>> values ;
		secobject(objtype_t type, serial_t serial) { this->type = type ; this->serial = serial ;}
	};
	
	objtype_t type ;
	
	serial_t serial ;
	std::array<bool,static_cast<size_t>(objfeatures_t::total)> features ;
	
	baseobject_t();
	virtual ~baseobject_t() = default ;
	
	virtual auto saveHeader(std::ostream &output) const ->void ;
	virtual auto saveContents(std::ostream &output) const  ->void ;
	virtual auto saveClose(std::ostream &output) const ->void ;
	auto save(std::ostream &output) const ->void ;

	
	virtual auto initialize(section &sec) ->bool ;
	virtual auto load(secobject &sec,objectfactory_t *factory) ->bool;


};
#endif /* baseobject_hpp */

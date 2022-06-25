//Copyright © 2022 Charles Kerr. All rights reserved.

#include "baseobject.hpp"

#include "strutil.hpp"
#include "objectfactory.hpp"
#include "section.hpp"

#include <iostream>
#include <algorithm>

using namespace std::string_literals;

//====================================================================================================
//  Static items
//====================================================================================================
const std::map<objtype_t,std::string> baseobject_t::object_names{
	{objtype_t::base,"base"s},	{objtype_t::item,"item"s},
	{objtype_t::mob,"mob"s},	{objtype_t::character,"character"s},
	{objtype_t::multi,"multi"s},	{objtype_t::boat,"boat"s},
	{objtype_t::spawn,"spawn"s}
};
//====================================================================================================
auto baseobject_t::nameForObjType(objtype_t obj) ->const std::string& {
	static auto notfound = std::string("unknown") ;
	auto iter = object_names.find(obj) ;
	if (iter != object_names.end()){
		return iter->second;
	}
	return notfound ;
}

//====================================================================================================
auto baseobject_t::typeForObjName(const std::string &name) ->objtype_t {
	auto type = objtype_t::base ;
	auto iter = std::find_if(object_names.begin(),object_names.end(),[name](const std::pair<objtype_t,std::string> &entry){
		return name == entry.second ;
	});
	if (iter != object_names.end()){
		type = iter->first ;
	}
	return type ;
}
//====================================================================================================
//
//====================================================================================================

//====================================================================================================
baseobject_t::baseobject_t(){
	this->type = objtype_t::base ;
	this->serial = invalid_serial ;
	this->features.fill(false);
}
//====================================================================================================
auto baseobject_t::saveHeader(std::ostream &output) const ->void {
	output <<"[" << nameForObjType(type)<<":" << strutil::ntos(serial,strutil::radix_t::hex,true,8)<<"]\n{\n";
}
//====================================================================================================
auto baseobject_t::saveContents(std::ostream &output) const ->void {

}
//====================================================================================================
auto baseobject_t::saveClose(std::ostream &output)const  ->void {
	output << "}\n";
	output.flush();
}
//====================================================================================================
auto baseobject_t::save(std::ostream &output) const ->void {
	if (!features[static_cast<size_t>(objfeatures_t::temporary)]){
		saveHeader(output);
		saveContents(output);
		saveClose(output);
	}
}

//====================================================================================================
auto baseobject_t::initialize(section &sec) ->bool {
	return true ;
}
//====================================================================================================
auto baseobject_t::load(secobject &sec,objectfactory_t *factory) ->bool{
	return true ;
}

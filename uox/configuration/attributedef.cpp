//Copyright © 2022 Charles Kerr. All rights reserved.

#include "attributedef.hpp"

#include "strutil.hpp"

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <fstream>

using namespace std::string_literals;

//====================================================================================================
// attribdef_t
//====================================================================================================
//====================================================================================================
const std::map<attribtype_t,std::string> attribdef_t::attribute_names{
	{attribtype_t::strength,"strength"s},{attribtype_t::dexterity,"dexterity"s},
	{attribtype_t::intelligence,"intelligence"s}
};

//====================================================================================================
auto attribdef_t::nameFor(attribtype_t type) ->const std::string& {
	try{
		return attribute_names.at(type) ;
	}
	catch(...){
		throw std::runtime_error("Unable to find attribute name for type of: "s + std::to_string(static_cast<int>(type)));
	}
}
//====================================================================================================
auto attribdef_t::typeFor(const std::string &name) ->attribtype_t {
	auto iter = std::find_if(attribute_names.begin(),attribute_names.end(),[name](const std::pair<attribtype_t,std::string> &entry){
		return entry.second == name ;
	});
	if (iter != attribute_names.end()){
		return iter->first;
	}
	throw std::runtime_error("Unable to find attribute type for name of: "s + name);
}
//====================================================================================================
attribdef_t::attribdef_t(attribtype_t type ){
	this->type = type ;
}
//====================================================================================================
auto attribdef_t::size() const -> size_t{
	return advancement.size();
}
//====================================================================================================
auto attribdef_t::operator[](int index) const ->const advancement_t&{
	return advancement.at(index) ;
}
//====================================================================================================
auto attribdef_t::operator[](int index)  -> advancement_t& {
	return advancement.at(index);
}
//====================================================================================================
auto attribdef_t::save(std::ostream &output) const ->void {
	output <<"[attribute " << nameFor(type)<<"]\n{\n";
	for (const auto &entry:advancement) {
		entry.save(output) ;
	}
	output <<"}\n\n";
}

//====================================================================================================
// attribdefinition_t
//====================================================================================================
//====================================================================================================
const std::filesystem::path attribdefinition_t::location = std::filesystem::path("configuration") / std::filesystem::path("settings") / std::filesystem::path("attributes.cfg");

//====================================================================================================
attribdefinition_t::attribdefinition_t(const std::filesystem::path &serverdata){
	currentsection = nullptr ;
	if (!serverdata.empty()){
		load(serverdata);
	}
}
//====================================================================================================
auto attribdefinition_t::clear() ->void {
	attributes.clear();
}
//====================================================================================================
auto attribdefinition_t::load(const std::filesystem::path &serverdata) ->bool {
	attributes.clear();
	return processFile(serverdata / location);
}
//====================================================================================================
auto attribdefinition_t::operator[](attribtype_t type) const -> const attribdef_t& {
	return attributes.at(type) ;
}
//====================================================================================================
auto attribdefinition_t::operator[](attribtype_t type)  ->  attribdef_t& {
	return attributes.at(type) ;
}
//====================================================================================================
auto attribdefinition_t::size() const ->size_t {
	return attributes.size() ;
}
//====================================================================================================
auto attribdefinition_t::startSection(const std::string &secline)->void {
	auto [sec,type] = strutil::split(secline," ");
	sec = strutil::lower(sec) ;
	type = strutil::lower(type) ;
	currentsection = nullptr ;
	if (sec == "attribute"){
		auto atttype = attribdef_t::typeFor(type);
		auto def = attribdef_t(atttype) ;
		attributes.insert_or_assign(atttype, def);
		currentsection = &attributes.at(atttype);
	}
}
//====================================================================================================
auto attribdefinition_t::keyvalue(const std::string &key, const std::string &value)->void {
	if (currentsection){
		auto lkey = strutil::lower(key) ;
		if (key == "advance"){
			auto adv = advancement_t(value);
			currentsection->advancement.push_back(adv) ;
		}
	}
}
//====================================================================================================
auto attribdefinition_t::endSection() ->void {
	currentsection = nullptr ;
}
//====================================================================================================
auto attribdefinition_t::save(std::filesystem::path &serverdata) const ->bool {
	auto output = std::ofstream((serverdata / location).string());
	auto rvalue = output.is_open();
	if (rvalue) {
		for (const auto &[type,entry]:attributes){
			entry.save(output) ;
		}
	}
	return rvalue ;
}

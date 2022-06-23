//Copyright © 2022 Charles Kerr. All rights reserved.

#include "section.hpp"

#include "strutil.hpp"

#include <iostream>
#include <algorithm>
//====================================================================================================
section::section(const std::string &label, sectype_t type):label(label),type(type){
	
}
//====================================================================================================
auto section::append(const std::string &key, const std::string &value) ->void{
	contents.push_back(std::make_pair(key, value));
}
	
//====================================================================================================
auto section::size() const -> size_t {
	return contents.size();
}
//====================================================================================================
auto section::operator[](int index) const ->const std::pair<std::string,std::string>& {
	return contents[index];
}
//====================================================================================================
auto section::operator[](int index) ->std::pair<std::string,std::string>& {
	return contents[index] ;
}
//====================================================================================================
auto section::values(const std::string &key) ->std::vector<std::string> {
	auto rvalue = std::vector<std::string>() ;
	auto search = strutil::lower(key) ;
	for (const auto &[seckey,value]:contents){
		auto lkey = strutil::lower(seckey) ;
		if (lkey == search){
			rvalue.push_back(value) ;
		}
	}
	return rvalue ;
}
//====================================================================================================
auto section::description(std::ostream &output) const ->void {
	output <<"["<<label<<"]\n{\n";
	for (const auto &[key,value]:contents){
		output <<"\t"<<key ;
		if (!value.empty()){
			output <<" = "<<value;
		}
		output <<"\n";
	}
	output <<"}\n\n";
	output.flush();
}

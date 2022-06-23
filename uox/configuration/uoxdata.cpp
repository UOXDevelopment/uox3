//Copyright © 2022 Charles Kerr. All rights reserved.

#include "uoxdata.hpp"

#include <iostream>

//====================================================================================================
auto uoxdata::load(const std::filesystem::path &dataroot) -> bool {
	auto rvalue = true ;
	for (auto i = 0; i < static_cast<int>(sectype_t::invalid); ++i){
		auto group = secgroup() ;
		rvalue = group.load(dataroot, static_cast<sectype_t>(i));
		if (rvalue) {
			contents.insert_or_assign(static_cast<sectype_t>(i), std::move(group));
		}
		else {
			std::cerr <<"Error loading " << secgroup::pathFor(static_cast<sectype_t>(i)).filename().string() << std::endl;
			break ;
		}
	}
	return rvalue ;
}
//====================================================================================================
auto uoxdata::remove(sectype_t type) ->void {
	auto iter = contents.find(type);
	if (iter != contents.end()) {
		contents.erase(iter);
	}
}
//====================================================================================================
auto uoxdata::clear() ->void {
	contents.clear() ;
}
//====================================================================================================
auto uoxdata::size() const ->size_t {
	return contents.size() ;
}

//====================================================================================================
auto uoxdata::operator[](sectype_t type) const ->const secgroup& {
	return contents.at(type) ;
}
//====================================================================================================
auto uoxdata::operator[](sectype_t type)  -> secgroup& {
	return contents.at(type) ;
}

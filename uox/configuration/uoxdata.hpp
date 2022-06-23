//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef uoxdata_hpp
#define uoxdata_hpp

#include <cstdint>
#include <string>
#include <unordered_map>

#include "secgroup.hpp"
//====================================================================================================
class uoxdata {
	
public:
	std::unordered_map<sectype_t, secgroup> contents ;
	
	auto load(const std::filesystem::path &dataroot) -> bool ;
	auto remove(sectype_t type) ->void ;
	auto clear() ->void ;
	auto size() const ->size_t ;
	auto operator[](sectype_t type) const ->const secgroup& ;
	auto operator[](sectype_t type)  -> secgroup& ;

};
#endif /* uoxdata_hpp */

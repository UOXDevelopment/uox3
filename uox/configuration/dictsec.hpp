//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef dictsec_hpp
#define dictsec_hpp

#include <cstdint>
#include <string>
#include <unordered_map>
//====================================================================================================
struct dictsec {
	std::string language ;
	std::unordered_map<int, std::string> contents ;
	
	auto size() const ->size_t ;
	auto clear() ->void ;
	auto operator[](int index) const ->const std::string& ;
	auto operator[](int index)  -> std::string& ;
	
	auto add(const std::string &key, const std::string &value) ->void ;
	
	dictsec(const std::string &lang="") ;
	
};

#endif /* dictsec_hpp */

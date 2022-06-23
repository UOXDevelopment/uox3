//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef secgroup_hpp
#define secgroup_hpp

#include <cstdint>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "section.hpp"
#include "secparser.hpp"

//====================================================================================================
class secgroup : public secparser{

public:
	static auto pathFor(sectype_t) -> std::filesystem::path ;
private:
	static const std::unordered_map<sectype_t, std::string> seclocations ;
	sectype_t sec_type ;
	auto startSection(const std::string &secline)->void final ;
	auto keyvalue(const std::string &key, const std::string &value)->void final ;
	auto endSection() ->void final ;
	auto closeFile() ->void final ;
	section current_section ;
public:
	std::unordered_map<std::string,section> contents ;
	auto load(const std::filesystem::path &root, sectype_t type) ->bool ;
	auto clear() ;
	auto type() const ->sectype_t ;
	auto size() const ->size_t ;
	auto operator[](const std::string &name) const ->const section& ;
	auto operator[](const std::string &name) -> section& ;
	auto contains(const std::string &name) const ->bool ;
	auto remove(const std::string &name) ->bool ;
	
};
#endif /* secgroup_hpp */

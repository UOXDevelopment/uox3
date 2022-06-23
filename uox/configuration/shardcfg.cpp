//Copyright © 2022 Charles Kerr. All rights reserved.

#include "shardcfg.hpp"

#include <iostream>
#include <map>
#include <stdexcept>

using namespace std::string_literals ;
//====================================================================================================
static const std::map<shardpaths,std::filesystem::path> baseshardloc{
	{shardpaths::log,std::filesystem::path("log")},
	{shardpaths::account,std::filesystem::path("account")},
	{shardpaths::world,std::filesystem::path("world")},
	{shardpaths::book,std::filesystem::path("book")},
	{shardpaths::msgboard,std::filesystem::path("msgboard")}
};
//====================================================================================================
shardcfg::shardcfg(const std::filesystem::path &shardpath) {
	if (!shardpath.empty()) {
		if (!load(shardpath)) {
			throw std::runtime_error("Error setting shard paths for: "s + shardpath.string());
		}
			
	}
}
//====================================================================================================
auto shardcfg::load(const std::filesystem::path &shardpath) ->bool {
	auto rvalue = true  ;
	for (const auto &[index,loc]:baseshardloc){
		auto path = shardpath / loc ;
		paths[static_cast<int>(index)] = path ;
		if (!std::filesystem::exists(path)){
			if (!std::filesystem::create_directories(path)){
				rvalue = false ;
				break;
			}
		}
	}
	return rvalue ;
}

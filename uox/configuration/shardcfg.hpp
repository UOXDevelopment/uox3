//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef shardcfg_hpp
#define shardcfg_hpp

#include <cstdint>
#include <string>
#include <array>
#include <filesystem>

//====================================================================================================
enum class shardpaths {
	log,account,world,book,msgboard,numpaths
};
//====================================================================================================
struct shardcfg {
private:
	
public:
	std::array<std::filesystem::path,static_cast<int>(shardpaths::numpaths)> paths ;
	shardcfg(const std::filesystem::path &shardpath = std::filesystem::path()) ;
	auto load(const std::filesystem::path &shardpath) ->bool ;
};
#endif /* shardcfg_hpp */

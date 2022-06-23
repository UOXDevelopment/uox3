//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef uoxcfg_hpp
#define uoxcfg_hpp

#include <cstdint>
#include <filesystem>

#include "secparser.hpp"
//=========================================================
struct uoxcfg :public secparser {
private:
	auto keyvalue(const std::string &key, const std::string &value)->void final;

public:
	std::string name ;
	std::string wanip ;
	std::string port ;
	std::filesystem::path uodir ;
	std::filesystem::path sharddata ;
	std::filesystem::path serverdata ;
	uoxcfg(const std::string &cfgpath = "") ;
	auto load(const std::string &cfgpath) ->bool ;
};
#endif /* uoxcfg_hpp */

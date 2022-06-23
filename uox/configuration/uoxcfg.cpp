//Copyright © 2022 Charles Kerr. All rights reserved.

#include "uoxcfg.hpp"

#include "strutil.hpp"

#include <iostream>
#include <stdexcept>

using namespace std::string_literals ;
//===================================================================================
uoxcfg::uoxcfg(const std::string &cfgpath) {
	if (!cfgpath.empty()) {
		if (!load(cfgpath)) {
			throw std::runtime_error("Unable to load configuration: "s + cfgpath);
		}
	}
}
//===================================================================================
auto uoxcfg::load(const std::string &cfgpath) ->bool {
	auto rvalue = false ;
	if (!cfgpath.empty()){
		rvalue = processFile(cfgpath);
	}
	return rvalue ;
}

//===================================================================================
auto uoxcfg::keyvalue(const std::string &key, const std::string &value)->void {
	auto lkey = strutil::lower(key) ;
	if (lkey == "name") {
		name = value;
	}
	else if (lkey == "wanip"){
		wanip = value ;
	}
	else if (lkey == "port") {
		port = value ;
	}
	else if (lkey == "uodata"){
		uodir = std::filesystem::path(value) ;
	}
	else if (lkey == "sharddata"){
		sharddata = std::filesystem::path(value);
	}
	else if (lkey == "serverdata"){
		serverdata = std::filesystem::path(value) ;
	}
}

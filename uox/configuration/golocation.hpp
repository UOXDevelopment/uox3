//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef golocation_hpp
#define golocation_hpp

#include <cstdint>
#include <string>
#include <map>
#include <filesystem>

#include "uoxtype.hpp"
#include "secparser.hpp"
//====================================================================================================
struct golocation_t :public secparser{
private:
	static const std::filesystem::path location;
	int goplace ;
	auto startSection(const std::string &secline)->void final;
	auto keyvalue(const std::string &key, const std::string &value)->void final;
	auto endSection() ->void final;
public:
	std::map<int,location_t> places ;
	golocation_t(const std::filesystem::path &serverdata = std::filesystem::path());
	auto load(const std::filesystem::path &serverdata) ->bool ;
	auto size() const ->size_t ;
};
#endif /* golocation_hpp */

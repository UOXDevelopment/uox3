//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef telelocation_hpp
#define telelocation_hpp

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>

#include "uoxtype.hpp"
#include "secparser.hpp"
//====================================================================================================
struct telelocation_t : public secparser {
private:
	
	static const std::filesystem::path location ;
	bool insection ;
	auto startSection(const std::string &secline)->void final;
	auto keyvalue(const std::string &key, const std::string &value)->void final;
	auto endSection() ->void final;
public:
	std::vector<duallocation_t> locations ;

	telelocation_t(const std::filesystem::path &serverdata = std::filesystem::path());
	auto load(const std::filesystem::path &serverdata) ->bool ;
	auto size() const ->size_t ;
};
#endif /* telelocation_hpp */

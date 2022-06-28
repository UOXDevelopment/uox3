//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef attributedef_hpp
#define attributedef_hpp

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

#include "uoxtype.hpp"
#include "secparser.hpp"
//====================================================================================================
enum class attribtype_t {strength,dexterity,intelligence};

//====================================================================================================
struct attribdef_t{
	attribtype_t type ;
	std::vector<advancement_t> advancement;
};
//====================================================================================================
struct attribdefinition_t : public secparser {
private:
	attribdef_t *currentsection ;
	auto startSection(const std::string &secline)->void final ;
	auto keyvalue(const std::string &key, const std::string &value)->void final;
	auto endSection() ->void final;
	static const std::filesystem::path location ;
public:
	std::map<attribtype_t,attribdef_t> attributes ;
	auto load(const std::filesystem::path &serverdata) ->bool ;
	auto operator[](attribtype_t type) const -> const attribdef_t& ;
	auto operator[](attribtype_t type)  ->  attribdef_t& ;
	auto size() const ->size_t ;
};
#endif /* attributedef_hpp */

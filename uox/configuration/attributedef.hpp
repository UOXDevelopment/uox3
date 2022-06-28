//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef attributedef_hpp
#define attributedef_hpp

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <ostream>

#include "uoxtype.hpp"
#include "secparser.hpp"
//====================================================================================================
enum class attribtype_t {strength,dexterity,intelligence};

//====================================================================================================
struct attribdef_t{
	attribtype_t type ;
	static const std::map<attribtype_t,std::string> attribute_names ;
	static auto nameFor(attribtype_t type) ->const std::string& ;
	static auto typeFor(const std::string &name) ->attribtype_t ;
	std::vector<advancement_t> advancement;
	auto size() const -> size_t;
	auto operator[](int index) const ->const advancement_t&;
	auto operator[](int index)  -> advancement_t&;
	attribdef_t(attribtype_t type = attribtype_t::strength);
	auto save(std::ostream &output) const ->void ;
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
	attribdefinition_t(const std::filesystem::path &serverdata = std::filesystem::path());
	auto clear() ->void ;
	auto load(const std::filesystem::path &serverdata) ->bool ;
	auto operator[](attribtype_t type) const -> const attribdef_t& ;
	auto operator[](attribtype_t type)  ->  attribdef_t& ;
	auto size() const ->size_t ;
	auto save(std::filesystem::path &serverdata) const ->bool ;
};
#endif /* attributedef_hpp */

//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef objectfactory_hpp
#define objectfactory_hpp

#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <filesystem>

#include "uoxtype.hpp"
#include "baseobject.hpp"
#include "secparser.hpp"

using factory_collection = std::unordered_map<serial_t, baseobject_t*> ;
//====================================================================================================
class objectfactory_t : public secparser {
	std::vector<baseobject_t::secobject> loaded_sections ;
	baseobject_t::secobject *cursection ;
	
	struct serialgen_t {
		serial_t serial_number ;
		serialgen_t(serial_t initial = 0);
		auto registerSerial(serial_t serial) ->void ;
		auto unregisterSerial(serial_t serial) ->void ;
		auto next() ->serial_t ;
		auto operator=(serial_t value) ->serialgen_t& ;
		auto operator=(std::int32_t value) ->serialgen_t& ;
	};
	serialgen_t item_serials ;
	serialgen_t mob_serials ;
	std::map<objtype_t,factory_collection> collections ;
	
	
	auto collectionForType(objtype_t type) const ->const factory_collection*;
	auto collectionForType(objtype_t type) -> factory_collection*;
	auto present(const baseobject_t * object, const factory_collection *collection) const ->bool ;
	
	auto startSection(const std::string &secline)->void final;
	auto keyvalue(const std::string &key, const std::string &value)->void final ;
	auto endSection() ->void final ;

	auto clear() ->void ;

public:
	objectfactory_t() ;
	~objectfactory_t() ;
	
	auto size(objtype_t type = objtype_t::base) const -> size_t ;
	
	auto valid(baseobject_t *object) const ->bool ;
	auto object( serial_t serial,objtype_t type = objtype_t::base) const -> const baseobject_t* ;
	auto object( serial_t serial,objtype_t type = objtype_t::base)  ->  baseobject_t* ;
	
	auto remove(baseobject_t *object) ->void ;
	auto create(objtype_t type) -> baseobject_t*;
	auto load(const std::filesystem::path &worldpath) ->bool ;
};
#endif /* objectfactory_hpp */

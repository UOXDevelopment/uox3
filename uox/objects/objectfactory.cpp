//Copyright © 2022 Charles Kerr. All rights reserved.

#include "objectfactory.hpp"
#include "strutil.hpp"

#include "itemobject.hpp"
#include "multiobject.hpp"
#include "mobobject.hpp"
#include "charobject.hpp"
#include "boatobject.hpp"
#include "spawnobject.hpp"


#include <iostream>
#include <algorithm>

using namespace std::string_literals ;
//====================================================================================================

//===================================================================================================
// objectfactory_t::serialgen_t
//===================================================================================================
//===================================================================================================
objectfactory_t::serialgen_t::serialgen_t(serial_t initial) :serial_number(initial){
	
}
//===================================================================================================
auto objectfactory_t::serialgen_t::next() -> serial_t {
	return serial_number++;
}
//===================================================================================================
auto objectfactory_t::serialgen_t::registerSerial(std::uint32_t serial) ->void {
	if (serial >= serial_number) {
		serial_number = serial + 1 ;
	}
}
//===================================================================================================
auto objectfactory_t::serialgen_t::unregisterSerial(std::uint32_t serial) ->void {
	if (serial_number == (serial+1)){
		serial_number = serial ;
	}
}

//===================================================================================================
auto objectfactory_t::serialgen_t::operator=(serial_t value) ->serialgen_t& {
	serial_number = value ;
	return *this;
}
//===================================================================================================
auto objectfactory_t::serialgen_t::operator=(std::int32_t value) ->serialgen_t& {
	serial_number = static_cast<serial_t>(value) ;
	return *this;
}

//===================================================================================================
// objectfactory_t
//===================================================================================================

//===================================================================================================
objectfactory_t::objectfactory_t() {
	cursection = nullptr ;
	// We need to make a collection for each objtype
	collections[objtype_t::item] = factory_collection() ;
	collections[objtype_t::spawn] = factory_collection() ;
	collections[objtype_t::mob] = factory_collection() ;
	collections[objtype_t::character] = factory_collection() ;
	collections[objtype_t::multi] = factory_collection() ;
	collections[objtype_t::boat] = factory_collection() ;
}

//===================================================================================================
objectfactory_t::~objectfactory_t() {
	clear();
}

//===================================================================================================
auto objectfactory_t::collectionForType(objtype_t type) const ->const factory_collection*{
	factory_collection *collection = nullptr ;
	auto iter = collections.find(type) ;
	if (iter != collections.end()){
		return &(iter->second) ;
	}

	return collection ;
}
//===================================================================================================
auto objectfactory_t::collectionForType(objtype_t type)  -> factory_collection*{
	factory_collection *collection = nullptr ;
	auto iter = collections.find(type) ;
	if (iter != collections.end()){
		return &(iter->second) ;
	}
	
	return collection ;
}

//===================================================================================================
auto objectfactory_t::present(const baseobject_t * object, const factory_collection *collection) const ->bool {
	auto rvalue = false ;
	if (collection){
		for (const auto &[serial,ptr]:*collection){
			if (object == ptr){
				rvalue = true ;
				break;
			}
		}
	}
	return rvalue;
}
//===================================================================================================
auto objectfactory_t::clear() ->void {
	for (auto &[type,collection] : collections){
		for (auto &[serial,ptr]: collection) {
			delete ptr ;
		}
		collection.clear();
	}
	
}

//===================================================================================================
auto objectfactory_t::size(objtype_t type) const -> size_t {
	auto collection = collectionForType(type) ;
	auto rvalue = size_t(0) ;
	if (collection) {
		rvalue = collection->size();
	}
	else {
		for (const auto &[objtype,collect]:collections){
			rvalue += collect.size() ;
		}
	}
	return rvalue ;
}
//===================================================================================================
auto objectfactory_t::valid(baseobject_t *object) const ->bool {
	auto rvalue = false ;
	if (object){
		auto collection = collectionForType(object->type) ;
		if (collection) {
			rvalue = present(object,collection);
		}
		else {
			for (const auto &[type,coll]:collections){
				rvalue = present(object,&coll);
				if (rvalue) {
					break;
				}
			}
		}
	}
	return rvalue ;
}
//===================================================================================================
auto objectfactory_t::object( serial_t serial,objtype_t type) const -> const baseobject_t* {
	auto collection = collectionForType(type) ;
	baseobject_t *obj = nullptr ;
	if (collection){
		auto iter = collection->find(serial);
		if (iter != collection->end()){
			obj =  iter->second ;
		}
	}
	else {
		for (const auto &[coltype,col]:collections){
			auto iter = col.find(serial);
			if (iter != col.end()){
				obj =  iter->second ;
				break;
			}
		}
	}
	return obj ;
}
//===================================================================================================
auto objectfactory_t::object( serial_t serial,objtype_t type)  ->  baseobject_t* {
	auto collection = collectionForType(type) ;
	baseobject_t *obj = nullptr ;
	if (collection){
		auto iter = collection->find(serial);
		if (iter != collection->end()){
			obj =  iter->second ;
		}
	}
	else {
		for (auto &[coltype,col]:collections){
			auto iter = col.find(serial);
			if (iter != col.end()){
				obj =  iter->second ;
				break;
			}
		}
	}
	return obj ;
}

//===================================================================================================
//  virtual functions from secparser
//===================================================================================================
auto objectfactory_t::startSection(const std::string &secline)->void{
	auto [sectype,sernum] = strutil::split(secline,":");
	sectype = strutil::lower(sectype);
	auto serial = strutil::ston<serial_t>(sernum) ;
	auto type = baseobject_t::typeForObjName(sectype);
	baseobject_t *ptr = nullptr ;
	switch (type) {
		case objtype_t::item:
			ptr = new itemobject_t() ;
			ptr->serial = serial ;
			item_serials.registerSerial(serial);
			break;
		case objtype_t::mob:
			ptr = new mobobject_t() ;
			ptr->serial = serial ;
			mob_serials.registerSerial(serial);
			break;
		case objtype_t::boat:
			ptr = new boatobject_t() ;
			ptr->serial = serial ;
			item_serials.registerSerial(serial);
			break;
		case objtype_t::multi:
			ptr = new multiobject_t() ;
			ptr->serial = serial ;
			item_serials.registerSerial(serial);
			break;
		case objtype_t::character:
			ptr = new charobject_t() ;
			ptr->serial = serial ;
			mob_serials.registerSerial(serial);
			break;
		case objtype_t::spawn:
			ptr = new spawnobject_t() ;
			ptr->serial = serial ;
			item_serials.registerSerial(serial);
			break;
		default:
			break;
	}
	if (ptr) {
		ptr->serial = serial ;
		collections[type].insert_or_assign(serial, ptr);

		auto section = baseobject_t::secobject(type,serial) ;
		loaded_sections.push_back(section);
		cursection = &(*loaded_sections.rbegin());
	}
	
}
//===================================================================================================
auto objectfactory_t::keyvalue(const std::string &key, const std::string &value)->void {
	if (cursection){
		cursection->values.push_back(std::make_pair(key, value));
	}
}
//===================================================================================================
auto objectfactory_t::endSection() ->void {
	cursection = nullptr ;
}

//===================================================================================================
auto objectfactory_t::load(const std::filesystem::path &worldpath) ->bool {
	auto rvalue = processDirectory(worldpath,".wsf") ;
	// Ok, so we have created all the objects, we need to load them now!
	if (rvalue) {
		for (auto &sec:loaded_sections){
			auto obj = object(sec.serial,sec.type);
			if (obj) {
				obj->load(sec, this);
			}
		}
	}
	return rvalue ;
}
//===================================================================================================
auto objectfactory_t::remove(baseobject_t *object) ->void{
	auto collection = collectionForType(object->type);
	if (collection){
		auto iter = collection->find(object->serial) ;
		if (iter != collection->end()) {
			collection->erase(iter);
		}
	}
	delete object ;
}
//===================================================================================================
auto objectfactory_t::create(objtype_t type) ->baseobject_t * {
	baseobject_t *obj = nullptr ;
	switch (type) {
		case objtype_t::item:
			obj = new itemobject_t() ;
			obj->serial = item_serials.next() ;
			break;
		case objtype_t::mob:
			obj = new mobobject_t() ;
			obj->serial = mob_serials.next() ;
			break;
		case objtype_t::boat:
			obj = new boatobject_t() ;
			obj->serial = item_serials.next() ;
			break;
		case objtype_t::multi:
			obj = new multiobject_t() ;
			obj->serial = item_serials.next();

			break;
		case objtype_t::character:
			obj = new charobject_t() ;
			obj->serial = mob_serials.next() ;
			break;
		case objtype_t::spawn:
			obj = new spawnobject_t() ;
			obj->serial = item_serials.next();
			break;
		default:
			break;
	}
	if (obj) {
		collections[type].insert_or_assign(obj->serial, obj);
	}
	return obj ;
}

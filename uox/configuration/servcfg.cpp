//Copyright © 2022 Charles Kerr. All rights reserved.

#include "servcfg.hpp"

#include "strutil.hpp"

#include <iostream>
#include <stdexcept>

using namespace std::string_literals ;

//====================================================================================================
// startlocation_t
//====================================================================================================
//====================================================================================================
startlocation_t::startlocation_t(){
	this->x = 0 ;
	this->y = 0 ;
	this->z = 0 ;
	this->mapnum = 0;
	this->realm = 0 ;
	this->cliloc = 0;

}
//====================================================================================================
startlocation_t::startlocation_t(const std::string &town,const std::string &descrip,int x, int y, int z, int mapnum,int realm, int cliloc){
	this->town = town ;
	this->description = descrip;
	this->x = x ;
	this->y = y ;
	this->z = z ;
	this->mapnum = mapnum;
	this->realm = realm ;
	this->cliloc = cliloc;
}
//====================================================================================================
startlocation_t::startlocation_t(const std::string &value):startlocation_t(){
	auto values = strutil::parse(value,",");
	if (values.size() > 7){
		town = values[0] ;
		description = values[1] ;
		x = strutil::ston<int>(values[2]);
		y = strutil::ston<int>(values[3]);
		z = strutil::ston<int>(values[4]);
		mapnum = strutil::ston<int>(values[5]);
		realm = strutil::ston<int>(values[6]) ;
		cliloc = strutil::ston<std::uint32_t>(values[7]);
	}
	else {
		throw std::runtime_error("Malformed start location found in system.cfg: "s + value) ;
	}
}

//====================================================================================================
// servercfg
//====================================================================================================
//====================================================================================================
//   overridden functions
//====================================================================================================
//====================================================================================================
auto servercfg::startSection(const std::string &secline)->void{
	auto lsec = strutil::lower(secline) ;
	if (lsec == "server"){
		current_load = loadsection::server ;
	}
	else if (lsec == "clientsupport"){
		current_load = loadsection::clientsupport ;
	}
	else if (lsec == "startlocation"){
		current_load = loadsection::startlocation;
	}
}
//====================================================================================================
auto servercfg::keyvalue(const std::string &key, const std::string &value)->void {
	switch (current_load) {
		case loadsection::server:
			processServer(key, value);
			break;
		case loadsection::startlocation:
			processStartLocation(key, value);
			break;
		case loadsection::clientsupport:
			processClientSupport(key, value);
			break;
		default:
			break;
	}
}
//====================================================================================================
auto servercfg::endSection() ->void {
	current_load = invalid ;
}

//====================================================================================================
//   section processing
//====================================================================================================
//====================================================================================================
auto servercfg::processClientSupport(const std::string &key, const std::string &value) ->void {
	auto lkey = strutil::lower(key) ;
	auto setting = strutil::ston<bool>(value) ;
	if (lkey == "client4000"){
		enabledclients[client4000] = setting ;
	}
	else if (lkey == "client5000"){
		enabledclients[client5000] = setting ;
	}
	else if (lkey == "client6000"){
		enabledclients[client6000] = setting ;
	}
	else if (lkey == "client7000"){
		enabledclients[client7000] = setting ;
	}
	else if (lkey == "client70160"){
		enabledclients[client70160] = setting ;
	}
	else if (lkey == "client70240"){
		enabledclients[client70240] = setting ;
	}
	else if (lkey == "client70300"){
		enabledclients[client70300] = setting ;
	}
	else if (lkey == "client704565"){
		enabledclients[client704565] = setting ;
	}
	else if (lkey == "client70610"){
		enabledclients[client70610] = setting ;
	}
}
//====================================================================================================
auto servercfg::processStartLocation(const std::string &key, const std::string &value) ->void {
	auto lkey = strutil::lower(key) ;
	if (lkey == "location") {
		try{
			auto loc = startlocation_t(value) ;
			startlocations.push_back(loc);
		}
		catch(const std::exception &e){
			std::cerr <<e.what()<<std::endl;
		}
	}
}
//====================================================================================================
auto servercfg::processServer(const std::string &key, const std::string &value) ->void {
	auto lkey = strutil::lower(key) ;
	if (key == "language") {
		serverlanguage = dictionary::getLanguage(strutil::upper(value));
	}
}


//====================================================================================================
//  constructor/load
//====================================================================================================
//====================================================================================================
servercfg::servercfg() {
	enabledclients.fill(false) ;
	enabledclients[enabledclients.size()-1] = true ;
	serverlanguage = DistinctLanguage::DL_ENGLISH ;
	current_load = invalid ;
}
//====================================================================================================
servercfg::servercfg(const std::filesystem::path &serverdata) :servercfg() {
	if (!serverdata.empty()){
		if (!load(serverdata)){
			throw std::runtime_error("Unabled  to load server configuration 'configuration/system.cfg' at: "s + serverdata.string());
		}
	}
}
//====================================================================================================
auto servercfg::load(const std::filesystem::path &serverdata) ->bool {
	auto path = serverdata / std::filesystem::path("configuration" / std::filesystem::path("system.cfg"));
	return processFile(path);
}

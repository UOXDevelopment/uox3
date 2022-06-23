//Copyright © 2022 Charles Kerr. All rights reserved.

#include "uoworld.hpp"

#include "mapdef.hpp"
#include "strutil.hpp"

#include <iostream>
#include <stdexcept>

//====================================================================================================
auto uoworld::startSection(const std::string &secline)->void {
	auto [header,num] = strutil::split(strutil::simplify(secline)," ") ;
	if (strutil::lower(header) == "tile"){
		current_tile = strutil::ston<int>(num) ;
		tile_active = true ;
	}
}
//====================================================================================================
auto uoworld::keyvalue(const std::string &key, const std::string &value)->void {
	if (tile_active) {
		auto lkey = strutil::lower(key) ;
		if (current_tile >0 && current_tile <0xFFFF){
			if (lkey == "weight"){
				tileinfo.art(current_tile).Weight(strutil::ston<std::uint8_t>(value));
			}
			else if (lkey == "height"){
				tileinfo.art(current_tile).Height(strutil::ston<std::int8_t>(value));
			}
			else if (lkey == "layer"){
				tileinfo.art(current_tile).Layer(strutil::ston<std::int8_t>(value));
			}
			else if (lkey == "animation"){
				tileinfo.art(current_tile).Animation(strutil::ston<std::uint16_t>(value));
			}
			else if (lkey == "hue"){
				tileinfo.art(current_tile).Hue(strutil::ston<std::uint8_t>(value));
			}
			else if (lkey == "quantity"){
				tileinfo.art(current_tile).Quantity(strutil::ston<std::uint8_t>(value));
			}
			else if (lkey == "name"){
				tileinfo.art(current_tile).Name(value);
			}
			else {
				if (!processFlag0(lkey, value)){
					if (!processFlag1(lkey, value)){
						if (!processFlag2(lkey, value)){
							processFlag3(lkey, value);
						}
					}
				}
			}
			
		}
	}
}
//====================================================================================================
auto uoworld::processFlag0(const std::string &lkey, const std::string &value) ->bool {
	auto rvalue = true ;
	if (lkey == "atfloorlevel"){
		tileinfo.art(current_tile).SetFlag(TF_FLOORLEVEL, strutil::ston<bool>(value));
	}
	else if (lkey == "holdable"){
		tileinfo.art(current_tile).SetFlag(TF_HOLDABLE, strutil::ston<bool>(value));
	}
	else if (lkey == "signguildbanner"){
		tileinfo.art(current_tile).SetFlag(TF_TRANSPARENT, strutil::ston<bool>(value));
	}
	else if (lkey == "webdirtblood"){
		tileinfo.art(current_tile).SetFlag(TF_TRANSLUCENT, strutil::ston<bool>(value));
	}
	else if (lkey == "wallverttile"){
		tileinfo.art(current_tile).SetFlag(TF_WALL, strutil::ston<bool>(value));
	}
	else if (lkey == "damaging"){
		tileinfo.art(current_tile).SetFlag(TF_DAMAGING, strutil::ston<bool>(value));
	}
	else if (lkey == "blocking"){
		tileinfo.art(current_tile).SetFlag(TF_BLOCKING, strutil::ston<bool>(value));
	}
	else if (lkey == "liquidwet"){
		tileinfo.art(current_tile).SetFlag(TF_WET, strutil::ston<bool>(value));
	}
	else {
		rvalue = false ;
	}
	return rvalue ;
}
//====================================================================================================
auto uoworld::processFlag1(const std::string &lkey, const std::string &value) ->bool {
	auto rvalue = true ;
	if (lkey == "unknownflag1"){
		tileinfo.art(current_tile).SetFlag(TF_UNKNOWN1, strutil::ston<bool>(value));
	}
	else if (lkey == "standable"){
		tileinfo.art(current_tile).SetFlag(TF_SURFACE, strutil::ston<bool>(value));
	}
	else if (lkey == "climbable"){
		tileinfo.art(current_tile).SetFlag(TF_CLIMBABLE, strutil::ston<bool>(value));
	}
	else if (lkey == "stackable"){
		tileinfo.art(current_tile).SetFlag(TF_STACKABLE, strutil::ston<bool>(value));
	}
	else if (lkey == "windowarchdoor"){
		tileinfo.art(current_tile).SetFlag(TF_WINDOW, strutil::ston<bool>(value));
	}
	else if (lkey == "cannotshootthru"){
		tileinfo.art(current_tile).SetFlag(TF_NOSHOOT, strutil::ston<bool>(value));
	}
	else if (lkey == "displayasa"){
		tileinfo.art(current_tile).SetFlag(TF_DISPLAYA, strutil::ston<bool>(value));
	}
	else if (lkey == "displayasan"){
		tileinfo.art(current_tile).SetFlag(TF_DISPLAYAN, strutil::ston<bool>(value));
	}
	else {
		rvalue = false ;
	}
	return rvalue ;
}
//====================================================================================================
auto uoworld::processFlag2(const std::string &lkey, const std::string &value) ->bool {
	auto rvalue = true ;
	if (lkey == "descriptiontile"){
		tileinfo.art(current_tile).SetFlag(TF_DESCRIPTION, strutil::ston<bool>(value));
	}
	else if (lkey == "fadewithtrans"){
		tileinfo.art(current_tile).SetFlag(TF_FOLIAGE, strutil::ston<bool>(value));
	}
	else if (lkey == "partialhue"){
		tileinfo.art(current_tile).SetFlag(TF_PARTIALHUE, strutil::ston<bool>(value));
	}
	else if (lkey == "unknownflag2"){
		tileinfo.art(current_tile).SetFlag(TF_UNKNOWN2, strutil::ston<bool>(value));
	}
	else if (lkey == "map"){
		tileinfo.art(current_tile).SetFlag(TF_MAP, strutil::ston<bool>(value));
	}
	else if (lkey == "container"){
		tileinfo.art(current_tile).SetFlag(TF_CONTAINER, strutil::ston<bool>(value));
	}
	else if (lkey == "equipable"){
		tileinfo.art(current_tile).SetFlag(TF_WEARABLE, strutil::ston<bool>(value));
	}
	else if (lkey == "lightsource"){
		tileinfo.art(current_tile).SetFlag(TF_LIGHT, strutil::ston<bool>(value));
	}
	else {
		rvalue = false ;
	}
	return rvalue ;
}

//====================================================================================================
auto uoworld::processFlag3(const std::string &lkey, const std::string &value) ->bool {
	auto rvalue = true ;
	if (lkey == "animated"){
		tileinfo.art(current_tile).SetFlag(TF_ANIMATED, strutil::ston<bool>(value));
	}
	else if (lkey == "nodiagonal"){
		tileinfo.art(current_tile).SetFlag(TF_NODIAGONAL, strutil::ston<bool>(value));
	}
	else if (lkey == "unknownflag3"){
		tileinfo.art(current_tile).SetFlag(TF_UNKNOWN3, strutil::ston<bool>(value));
	}
	else if (lkey == "wholebodyitem"){
		tileinfo.art(current_tile).SetFlag(TF_ARMOR, strutil::ston<bool>(value));
	}
	else if (lkey == "wallroofweap"){
		tileinfo.art(current_tile).SetFlag(TF_ROOF, strutil::ston<bool>(value));
	}
	else if (lkey == "door"){
		tileinfo.art(current_tile).SetFlag(TF_DOOR, strutil::ston<bool>(value));
	}
	else if (lkey == "climbablebit1"){
		tileinfo.art(current_tile).SetFlag(TF_STAIRBACK, strutil::ston<bool>(value));
	}
	else if (lkey == "climbablebit2"){
		tileinfo.art(current_tile).SetFlag(TF_STAIRRIGHT, strutil::ston<bool>(value));
	}
	else {
		rvalue = false ;
	}
	return rvalue ;
}


//====================================================================================================
auto uoworld::endSection() ->void {
	current_tile = -1 ;
	tile_active = false ;
}
//====================================================================================================
auto uoworld::processOverrides(const std::filesystem::path &serverdata) ->void {
	current_tile = -1 ;
	tile_active = false ;
	auto path = serverdata / std::filesystem::path("configuration") / std::filesystem::path("tiles.cfg") ;
	processFile(path);
}


//====================================================================================================
auto uoworld::processMap(ultimamap &map,const mapdef_t &entry, const std::filesystem::path &uodir) ->bool {
	auto uop = uodir / entry.uoppath ;
	auto rvalue = false ;
	if (std::filesystem::exists(uop)){
		rvalue = map.loadUOPTerrainFile(uop.string()) ;
	}
	if (!rvalue) {
		rvalue = map.loadMulTerrainFile(uodir / entry.mappath) ;
	}
	if (rvalue) {
		rvalue = map.loadArt((uodir / entry.stamul).string(), (uodir /entry.staidx).string());
	}
	if (rvalue) {
		// still trucking!
		if (!map.uop()){
			map.applyTerrainDiff((uodir / entry.mapdifl).string(), (uodir / entry.mapdifl).string());
		}
		map.applyDiff((uodir / entry.stadifl).string(), (uodir / entry.stadifi).string(), (uodir / entry.stadif).string());
	}
	return rvalue ;
}

//====================================================================================================
auto uoworld::load(std::filesystem::path &serverdata,std::filesystem::path &uodir, const mapdef &mapdefs ) ->bool {
	auto rvalue = false ;
	
	// First load the tiledata
	auto path = uodir / std::filesystem::path("tiledata.mul") ;
	if (tileinfo.load(path)) {
		// We should get overrides here, before we do statics/maps/multis
		processOverrides(serverdata);
		if (uomulti.load(uodir,&tileinfo) ) {
			for (const auto &entry : mapdefs.mapdefinitions){
				
				uomaps.insert_or_assign(entry.first,ultimamap(entry.first,entry.second.size.first,entry.second.size.second,&tileinfo));
				auto &curmap = uomaps[entry.first] ;
				rvalue = processMap(curmap, entry.second, uodir);
				if (!rvalue) {
					break;
				}
				
				
				
			}
		}
	}
	return rvalue ;
}

//====================================================================================================
auto uoworld::size() const ->size_t {
	return uomaps.size() ;
}
//====================================================================================================
auto uoworld::world(int number) const ->const ultimamap& {
	return uomaps.at(number) ;
}
//====================================================================================================
auto uoworld::world(int number)  -> ultimamap& {
	return uomaps.at(number) ;
}
//====================================================================================================
auto uoworld::multi(int number) const -> const collection_item& {
	return uomulti.multi(number);
}
//====================================================================================================
auto uoworld::multi(int number)  ->  collection_item& {
	return uomulti.multi(number);
}

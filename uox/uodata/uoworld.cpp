//Copyright © 2022 Charles Kerr. All rights reserved.

#include "uoworld.hpp"

#include "mapdef.hpp"
#include "strutil.hpp"

#include <iostream>
#include <stdexcept>

using namespace std::string_literals;
//====================================================================================================
//=========================================================
const std::map<std::string, std::uint64_t> art_overrides{
	{"atflorlevel"s,ultima::background},{"holdable"s,ultima::holdable},{"signguildbanner"s,ultima::transparent},
	{"webdirtblood"s,ultima::translucent},{"wallverttile"s,ultima::translucent},{"damaging"s,ultima::damaging},
	{"blocking"s,ultima::impassable},{"liquidwet"s,ultima::wet},{"unknownflag1"s,ultima::unknownBit9},
	{"standable"s,ultima::surface},{"climbable"s,ultima::climable},{"stackable"s,ultima::stackable},
	{"windowarchdoor"s,ultima::window},{"cannotshootthru"s,ultima::noShoot},{"displayasa"s,ultima::articleA},
	{"displayasan"s,ultima::articleAn},{"descriptiontile"s,ultima::articleThe},{"fadewithtrans"s,ultima::foliage},
	{"partialhue"s,ultima::partialHue},{"unknownflag2"s,ultima::noHouse},{"map"s,ultima::map},
	{"container"s,ultima::container},{"equipable"s,ultima::wearable},{"lightsource"s,ultima::lightSource},
	{"animated"s,ultima::animated},{"nodiagonal"s,ultima::hoverOver},{"unknownflag3"s,ultima::noDiagnol},
	{"wholebodyitem"s,ultima::armor},{"wallroofweap"s,ultima::roof},{"door"s,ultima::door},
	{"climbablebit1"s,ultima::stairBack},{"climbablebit2"s,ultima::stairRight}
};
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
				info.art(current_tile).weight =strutil::ston<weight_t>(value);
			}
			else if (lkey == "height"){
				info.art(current_tile).height = strutil::ston<height_t>(value);
			}
			else if (lkey == "layer"){
				info.art(current_tile).layer =strutil::ston<layer_t>(value);
			}
			else if (lkey == "animation"){
				info.art(current_tile).animid = strutil::ston<animid_t>(value);
			}
			else if (lkey == "hue"){
				info.art(current_tile).hue = strutil::ston<int>(value);
			}
			else if (lkey == "quantity"){
				info.art(current_tile).quantity = strutil::ston<int>(value);
			}
			else if (lkey == "name"){
				info.art(current_tile).name = value;
			}
			else {
				auto iter = art_overrides.find(lkey) ;
				if (iter != art_overrides.end()){
					auto setvalue =strutil::ston<bool>(value);
					if (setvalue) {
						info.art(current_tile).flags |= iter->second;
					}
					else {
						info.art(current_tile).flags &= (~iter->second);
					}
					
				}
			}
			
		}
	}
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
auto uoworld::processMap(uomap &map,const mapdef_t &entry, const std::filesystem::path &uodir) ->bool {
	auto uop = uodir / entry.uoppath ;
	auto rvalue = false ;
	if (std::filesystem::exists(uop)){
		rvalue = map.loadTerrain(uop) ;
	}
	if (!rvalue) {
		rvalue = map.loadMulTerrain((uodir / entry.mappath)) ;
	}
	if (rvalue) {
		rvalue = map.loadArt( (uodir /entry.staidx),(uodir / entry.stamul));
	}
	if (rvalue) {
		// still trucking!
		if (!map.uop){
			map.applyTerrainDiff((uodir / entry.mapdif), (uodir / entry.mapdifl));
		}
		map.applyArtDiff((uodir / entry.stadifl), (uodir / entry.stadifi), (uodir / entry.stadif));
	}
	return rvalue ;
}

//====================================================================================================
auto uoworld::load(std::filesystem::path &serverdata,std::filesystem::path &uodir, const mapdef &mapdefs ) ->bool {
	auto rvalue = false ;
	
	// First load the tiledata
	auto path = uodir / std::filesystem::path("tiledata.mul") ;
	if (info.load(path)) {
		// We should get overrides here, before we do statics/maps/multis
		processOverrides(serverdata);
		if (uomulti.load(uodir,"",&info) ) {
			for (const auto &entry : mapdefs.mapdefinitions){
				
				uomaps.insert_or_assign(entry.first,uomap(entry.first,entry.second.size.first,entry.second.size.second,&info));
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
auto uoworld::world(int number) const ->const uomap& {
	return uomaps.at(number) ;
}
//====================================================================================================
auto uoworld::world(int number)  -> uomap& {
	return uomaps.at(number) ;
}
//====================================================================================================
auto uoworld::multi(int number) const -> const collection_item& {
	return uomulti[number];
}
//====================================================================================================
auto uoworld::multi(int number)  ->  collection_item& {
	return uomulti[number];
}

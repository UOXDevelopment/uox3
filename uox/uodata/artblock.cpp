//

#include "artblock.hpp"
#include "tiledata.hpp"

#include <iostream>

//================================================================================
// artblock
//================================================================================

//=========================================================
artblock::artblock(int length,std::uint8_t *data,const tiledata *info){
	if (data != nullptr) {
		load(length,data,info) ;
	}
}

//=========================================================
auto artblock::tile(int x, int y) ->std::vector<tile_t>& {
	return _tiles.at(x).at(y) ;
}
//=========================================================
auto artblock::tile(int x, int y) const ->const std::vector<tile_t>& {
	return _tiles.at(x).at(y) ;
}



//=========================================================
auto artblock::clear() ->void {
	for (auto &row : _tiles){
		for (auto &tile: row){
			tile.clear();
		}
	}
}
//==========================================================
auto artblock::load(int length, std::istream &input,const tiledata *info) ->void{
	this->clear() ;
	auto count = length/7 ;
	auto x = std::uint8_t(0);
	auto y = std::uint8_t(0) ;
	auto alt = std::int8_t(0) ;
	for (auto i=0; i<count ;++i){
		auto arttile = tile_t() ;
		arttile.type = tiletype_t::art ;
		input.read(reinterpret_cast<char*>(&arttile.tileid),2) ;
		input.read(reinterpret_cast<char*>(&x),1);
		input.read(reinterpret_cast<char*>(&y),1);
		input.read(reinterpret_cast<char*>(&alt),1);
		arttile.altitude = alt ;
		input.read(reinterpret_cast<char*>(&arttile.static_hue),2);
		if (info){
			arttile.artInfo = &info->art(arttile.tileid);
		}
		tile(x,y).push_back(std::move(arttile));
		
	}
}
//==========================================================
auto artblock::load(int length,std::uint8_t *data,const tiledata *info) ->void{
	this->clear() ;
	
	auto count = length/7 ;
	auto x = std::uint8_t(0);
	auto y = std::uint8_t(0) ;
	for (auto i=0; i<count ;++i){
		auto arttile = tile_t() ;
		arttile.type = tiletype_t::art ;
		std::copy(data,data+2,reinterpret_cast<char*>(&arttile.tileid)) ;
		x = *(data+2) ;
		y = *(data+3);
		arttile.altitude = static_cast<int>(*(reinterpret_cast<std::int8_t*>(data+4)));
		std::copy(data+5,data+7,reinterpret_cast<char*>(&arttile.static_hue)) ;
		if (info){
			arttile.artInfo = &info->art(arttile.tileid);
		}
		
		tile(x,y).push_back(std::move(arttile));
		data +=7 ;
	}
}


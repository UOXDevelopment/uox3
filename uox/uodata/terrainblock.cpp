//

#include "terrainblock.hpp"
#include "tiledata.hpp"

#include <iostream>

//====================================================================================================
// terrainblock
//====================================================================================================
//=========================================================
terrainblock::terrainblock(std::uint8_t *data,const tiledata *info){
	if (data != nullptr) {
		load(data,info) ;
	}
}
//=========================================================
auto terrainblock::load(std::uint8_t *data,const tiledata *info) ->void {
	if (data !=nullptr){
		data +=4 ;
		for (auto y= 0 ; y < 8 ;++y){
			for (auto x= 0 ; x < 8 ;++x){
				auto tileid = std::uint16_t(0) ;
				// we copy here, because of potential alignment issues
				std::copy(data,data+2,reinterpret_cast<std::uint8_t*>(&(tileid)));
				_tiles[x][y].tileid = tileid ;
				if (info){
					_tiles[x][y].terrainInfo = &info->terrain(tileid);
				}
				_tiles[x][y].altitude = *(reinterpret_cast<std::int8_t*>(data+2));
				data += 3 ; // advance data three bytes ;
			}
		}
	}
}

//=========================================================
auto terrainblock::tile(int x, int y) ->tile_t& {
	return _tiles.at(x).at(y) ;
}
//=========================================================
auto terrainblock::tile(int x, int y) const ->const tile_t& {
	return _tiles.at(x).at(y) ;
}

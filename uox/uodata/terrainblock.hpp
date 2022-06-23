//

#ifndef terrainblock_hpp
#define terrainblock_hpp

#include "tileinfo.hpp"

#include <cstdint>
#include <array>
class tiledata ;


class terrainblock {
	std::array<std::array<tile_t,8>,8> _tiles ;
	
public:
	terrainblock(std::uint8_t *data = nullptr,const tiledata *info = nullptr);
	auto load(std::uint8_t *data,const tiledata *info = nullptr) ->void ;
	auto tile(int x, int y) ->tile_t& ;
	auto tile(int x, int y) const ->const tile_t& ;
};
#endif /* terrainblock_hpp */

//

#ifndef artblock_hpp
#define artblock_hpp

#include "tileinfo.hpp"

#include <array>
#include <cstdint>
#include <vector>

class tiledata ;
//=========================================================
class artblock {
	std::array<std::array<std::vector<tile_t>,8>,8> _tiles ;
	
public:
	artblock(int length=0,std::uint8_t *data = nullptr,const tiledata *info = nullptr);
	auto load(int length,std::uint8_t *data,const tiledata *info = nullptr) ->void ;
	auto load(int length,std::istream &input,const tiledata *info = nullptr) ->void ;
	auto tile(int x, int y) ->std::vector<tile_t>& ;
	auto tile(int x, int y) const ->const std::vector<tile_t>& ;
	auto clear() ->void ;
};
#endif /* artblock_hpp */

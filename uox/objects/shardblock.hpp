//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef shardblock_hpp
#define shardblock_hpp

#include <cstdint>
#include <string>
#include <array>
#include <vector>

#include "uoxtype.hpp"

constexpr auto shardblockwidth = 8 ;
constexpr auto shardblockheight = 8 ;

struct baseobject_t;

//====================================================================================================
struct shardblock_t {
	std::array<std::array<std::vector<baseobject_t*>,shardblockheight>,shardblockwidth> objects ;
	
	mutable bool modified ;
	
	auto objectsAt(coord_t xoffset, coord_t yoffset) const ->const std::vector<baseobject_t*>& ;
	auto objectsAt(coord_t xoffset, coord_t yoffset) ->const std::vector<baseobject_t*>& ;

};
#endif /* shardblock_hpp */

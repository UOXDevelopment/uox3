//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef uoxtype_hpp
#define uoxtype_hpp

#include <cstdint>
#include <string>
#include <limits>
//====================================================================================================
// These define types we use thoughout the code
// Note, we use int a lot, and expect the translation to occur from uodata/packet.
// We use int, as that is a more natural storage for modern processors
// but we can change it if needed.
using serial_t = std::uint32_t ;
using coord_t = std::int32_t ;
using elevation_t = std::int32_t ;
using light_t = std::int32_t ;
using color_t = std::int32_t ;
using tileid_t = std::uint16_t ;
using weight_t = std::int32_t ;
using animid_t = std::int32_t ;
using texid_t = std::int32_t ;
using height_t = std::int32_t ;

constexpr auto invalid_serial =std::numeric_limits<serial_t>::max();
constexpr auto invalid_tile = std::numeric_limits<tileid_t>::max() ;
constexpr auto elev_cap = 127 ;
constexpr auto max_weight = 255 ;

#endif /* uoxtype_hpp */

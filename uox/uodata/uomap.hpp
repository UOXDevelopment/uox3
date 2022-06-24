//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef uomap_hpp
#define uomap_hpp

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <utility>
#include <filesystem>
#include <tuple>

#include "ultima_types.hpp"
#include "uopfile.hpp"
#include "terrainblock.hpp"
#include "artblock.hpp"

class tileinfo ; // Forward declaratin
//=========================================================
class uomap : public uopfile {
	std::vector<terrainblock> _terrain ;
	std::vector<artblock> _art;
	
	static constexpr int _totalmaps = 6 ;
	static constexpr std::array<std::pair<int,int>,_totalmaps> _mapsizes{{
		{7168,4096},{7168,4096},{2304,1600},
		{2560,2048},{1448,1448},{1280,4096}
	}};
	int _mapnum ;
	int _width ;
	int _height ;
	
	const tileinfo *_tileInfo ;
	int _diffcount ;
	int _diffterrain ;
	auto processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool final ;
	auto calcBlock(int x,int y) const ->int ;
	auto calcXYOffset(int block) const ->std::pair<int,int> ;
	auto loadTerrainBlock(int blocknum,std::uint8_t *data)->void ;
public:
	bool uop ;
	uomap() =default;
	uomap(int mapnum,int width, int height,const tileinfo *tileInfo );
	auto width() const ->int ;
	auto height() const ->int ;
	auto diffArt() const ->int ;
	auto diffTerrain() const ->int ;
	auto setSize(int width, int height) ->void ;
	auto loadTerrain(const std::filesystem::path &uoppath) ->bool ;
	auto loadMulTerrain(const std::filesystem::path &mulpath) -> bool;
	auto loadArt(const std::filesystem::path &idxpath,const std::filesystem::path &mulpath) ->bool ;
	auto applyTerrainDiff(const std::filesystem::path &diffpath,const std::filesystem::path &diflpath) ->int ;
	auto applyArtDiff(const std::filesystem::path &diflpath,const std::filesystem::path &difipath, const  std::filesystem::path &diffpath) ->int ;

	auto blockAndIndexFor(int x, int y) const ->std::tuple<int, int, int>;

	
	auto terrain(int x, int y) const ->const tile_t& ;
	auto terrain(int x, int y)  ->tile_t& ;
	auto art(int x, int y) const ->const std::vector<tile_t>& ;
	auto art(int x, int y)  ->std::vector<tile_t>& ;

};
#endif /* uomap_hpp */

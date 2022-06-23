//

#ifndef ultimamap_hpp
#define ultimamap_hpp
#include "terrainblock.hpp"
#include "artblock.hpp"
#include "uopfile.hpp"
//=========================================================
class ultimamap : public uopfile {
	std::vector<terrainblock> _terrain ;
	std::vector<artblock> _art;
	
	const tiledata *tile_info ;
	static constexpr int _totalmaps = 6 ;
	static constexpr std::array<std::pair<int,int>,_totalmaps> _mapsizes{{
		{7168,4096},{7168,4096},{2304,1600},
		{2560,2048},{1448,1448},{1280,4096}
	}};
	int _mapnum ;
	int _width ;
	int _height ;
	bool isUOP ;
	int _diffcount ;
	int _diffterrain ;
	auto processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool final ;
	auto calcBlock(int x,int y) const ->int ;
	auto calcXYOffset(int block) const ->std::pair<int,int> ;
	auto loadTerrainBlock(int blocknum,std::uint8_t *data)->void ;
public:
	ultimamap() ;
	ultimamap(int mapnum,int width=0, int height = 0,const tiledata *info = nullptr);
	auto width() const ->int ;
	auto height() const ->int ;
	auto diffArt() const ->int ;
	auto diffTerrain() const ->int ;
	auto setSize(int width, int height) ->void ;
	auto size() const ->std::pair<int,int> ;
	auto loadUOPTerrainFile(const std::string &filename) ->bool ;
	auto loadMulTerrainFile(const std::string &filename) ->bool ;
	auto loadArt(const std::string &mulfile, const std::string &idxfile)->bool ;
	auto applyDiff(const std::string &difflpath,const std::string &diffipath, const std::string &diffpath) ->int ;
	auto applyTerrainDiff(const std::string &difflpath,const std::string &diffpath) ->int ;
	
	auto blockAndIndexFor(int x, int y) const ->std::tuple<int, int, int>;
	auto uop()const ->bool { return isUOP;}
	
	auto terrain(int x, int y) const ->const tile_t& ;
	auto terrain(int x, int y)  ->tile_t& ;
	auto art(int x, int y) const ->const std::vector<tile_t>& ;
	auto art(int x, int y)  ->std::vector<tile_t>& ;
};

#endif /* ultimamap_hpp */

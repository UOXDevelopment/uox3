//

#include "ultimamap.hpp"
#include "tiledata.hpp"

#include <fstream>

//================================================================================
// ultimamap
//================================================================================
//===============================================================================
ultimamap::ultimamap() :_diffcount(0),_diffterrain(0),_width(0),_height(0),tile_info(nullptr){
}
//=========================================================
ultimamap::ultimamap(int mapnum,int width,int height,const tiledata *info):ultimamap(){
	_width = width ;
	_height= height ;
	tile_info = info ;
	_mapnum = mapnum ;
	auto size = _mapsizes.at(mapnum) ;
	if (_width == 0){
		_width = size.first ;
	}
	if (_height == 0){
		_height = size.second ;
	}
	_terrain.resize( (_width/8) * (_height/8) );
	_art.resize(_terrain.size()) ;
	isUOP = true ;
}
//=========================================================
auto ultimamap::width() const ->int {
	return _width ;
}
//=========================================================
auto ultimamap::height() const ->int {
	return _height ;
}
//=========================================================
auto ultimamap::setSize(int width, int height) ->void {
	_width = width ;
	_height = height ;
	_terrain.resize( (_width/8) * (_height/8) );
	_art.resize(_terrain.size()) ;
}
//=========================================================
auto ultimamap::size() const ->std::pair<int,int> {
	return std::make_pair(static_cast<int>(_width),static_cast<int>(_height));
}

//=========================================================
auto ultimamap::diffArt() const ->int {
	return _diffcount ;
}
//=========================================================
auto ultimamap::diffTerrain() const ->int {
	return _diffterrain ;
}
//===========================================================
auto ultimamap::loadUOPTerrainFile(const std::string &filename) ->bool {
	auto hash = this->format("build/map%ilegacymul/%s", _mapnum,"%.8u.dat");
	//auto filename = this->format("map%iLegacyMUL.uop",_mapnum);
	isUOP = loadUOP(filename, 0x300, hash);
	return isUOP ;
}
//===========================================================
auto ultimamap::loadMulTerrainFile(const std::string &filename) ->bool {
	auto rvalue = false ;
	auto mul = std::ifstream(filename,std::ios::binary) ;
	if (mul.is_open()){
		rvalue = true;
		isUOP = false ;
		auto data = std::vector<std::uint8_t>(196,0);
		auto blocknum = 0 ;
		while (mul.good() && !mul.eof()) {
			mul.read(reinterpret_cast<char*>(data.data()),196);
			if (mul.gcount() == 196) {
				loadTerrainBlock(blocknum, data.data());
			}
			blocknum++;
		}
	}
	return rvalue ;
}
//=========================================================
auto ultimamap::loadArt(const std::string &mulfile, const std::string &idxfile)->bool {
	auto idx = std::ifstream(idxfile,std::ios::binary);
	auto rvalue = idx.is_open();
	if (rvalue){
		auto mul = std::ifstream(mulfile,std::ios::binary);
		rvalue = mul.is_open();
		if (rvalue) {
			auto offset = std::uint32_t(0) ;
			auto length = std::int32_t(0) ;
			auto extra = std::uint32_t(0) ;
			auto block = 0 ;
			while (idx.good()&& !idx.eof()){
				idx.read(reinterpret_cast<char*>(&offset),4);
				idx.read(reinterpret_cast<char*>(&length),4);
				idx.read(reinterpret_cast<char*>(&extra),4);
				if ((length >0) && (idx.gcount()==4)) {
					mul.seekg(offset,std::ios::beg);
					if (block < _art.size()){
						_art[block].load(length, mul,tile_info);
					}
				}
				++block ;
			}
		}
	}
	return rvalue ;
}
//=========================================================
auto ultimamap::applyDiff(const std::string &difflpath,const std::string &diffipath, const std::string &diffpath) ->int {
	_diffcount = 0 ;
	auto diffblock = std::ifstream(difflpath,std::ios::binary) ;
	if (diffblock.is_open()){
		auto diffidx = std::ifstream(diffipath,std::ios::binary) ;
		if (diffidx.is_open()){
			auto diff = std::ifstream(diffpath,std::ios::binary) ;
			if (diff.is_open()){
				auto blocknum = std::uint32_t(0) ;
				auto offset = std::uint32_t(0);
				auto length = std::int32_t(0) ;
				auto extra = std::uint32_t(0);
				while (diffblock.good() && !diffblock.eof()){
					diffblock.read(reinterpret_cast<char*>(&blocknum),4);
					diffidx.read(reinterpret_cast<char*>(&offset),4);
					diffidx.read(reinterpret_cast<char*>(&length),4);
					diffidx.read(reinterpret_cast<char*>(&extra),4);
					if ((diffblock.gcount()==4) && (diffidx.gcount()==4)){
						if (blocknum< _art.size()){
							if (length >0){
								diff.seekg(offset,std::ios::beg);
								_art[blocknum].load(length, diff,tile_info);
							}
							else {
								_art[blocknum].clear();
							}
						}
						++_diffcount;
					}
				}
			}
		}
		
	}
	return _diffcount ;
}
//=========================================================
auto ultimamap::applyTerrainDiff(const std::string &difflpath,const std::string &diffpath) ->int {
	auto diffblock = std::ifstream(difflpath,std::ios::binary) ;
	_diffterrain = 0 ;
	if (diffblock.is_open()) {
		auto diff = std::ifstream(diffpath,std::ios::binary) ;
		if (diff.is_open()) {
			auto blocknum = std::uint32_t(0) ;
			auto data = std::vector<std::uint8_t>(196,0) ;
			while (diffblock.good() && !diffblock.eof()){
				diffblock.read(reinterpret_cast<char*>(&blocknum),4) ;
				if (diffblock.gcount()==4) {
					diff.read(reinterpret_cast<char*>(data.data()),196);
					if (diff.gcount()==196){
						_terrain[blocknum].load(data.data(),tile_info);
						_diffterrain++;
					}
				}
			}
		}
	}
	return _diffterrain;
	
}
//=========================================================
auto ultimamap::calcBlock(int x,int y) const ->int {
	// How many blocks is x across?
	return  ((x/8)*(_height/8)) + (y/8) ;
}
//=========================================================
auto ultimamap::calcXYOffset(int block) const ->std::pair<int,int> {
	auto x = (block / (_height/8)) * 8 ;
	auto y = (block % (_height/8)) * 8;
	return std::make_pair(x, y);
}
//=========================================================
auto ultimamap::blockAndIndexFor(int x, int y) const ->std::tuple<int, int, int>{
	auto block = calcBlock(x, y);
	auto offset = calcXYOffset(block);
	return std::make_tuple(block,x-offset.first,y-offset.second);
}
//=========================================================
auto ultimamap::processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool {
	auto count = data.size()/196 ;
	auto block = (static_cast<int>(index) * 0xC4000)/196 ;
	for (auto i=0 ;i<count;++i){
		auto ptr = data.data()+(i*196);
		if (block < _terrain.size()){
			loadTerrainBlock(block, ptr);
		}
		++block ;
	}
	return true ;
}
//=========================================================
auto ultimamap::loadTerrainBlock(int blocknum,std::uint8_t *data)->void {
	_terrain[blocknum].load(data,tile_info);
}

//=========================================================
auto ultimamap::terrain(int x, int y) const ->const tile_t& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _terrain[blocknum].tile(xoffset, yoffset);
}
//=========================================================
auto ultimamap::terrain(int x, int y)  ->tile_t& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _terrain[blocknum].tile(xoffset, yoffset);
	
}
//=========================================================
auto ultimamap::art(int x, int y) const ->const std::vector<tile_t>& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _art[blocknum].tile(xoffset, yoffset);
	
}
//=========================================================
auto ultimamap::art(int x, int y)  ->std::vector<tile_t>& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _art[blocknum].tile(xoffset, yoffset);
}


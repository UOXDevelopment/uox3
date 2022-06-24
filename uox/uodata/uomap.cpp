//Copyright © 2022 Charles Kerr. All rights reserved.

#include "uomap.hpp"

#include "tileinfo.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

//=========================================================
uomap::uomap(int mapnum,int width, int height,const tileinfo *tileInfo):_diffcount(0),_diffterrain(0){
	_tileInfo = tileInfo ;
	_mapnum = mapnum ;
	auto size = _mapsizes.at(mapnum) ;
	if (width != 0){
		_width  = width ;
	}
	else {
		_width = size.first ;
	}
	if (height != 0 ){
		_height = height;
	}
	else {
		_height = size.second ;
	}
	_terrain.resize( (_width/8) * (_height/8) );
	_art.resize(_terrain.size()) ;
	uop = true  ;
}
//=========================================================
auto uomap::width() const ->int {
	return _width ;
}
//=========================================================
auto uomap::height() const ->int {
	return _height ;
}
//=========================================================
auto uomap::setSize(int width, int height) ->void {
	_width = width ;
	_height = height ;
	_terrain.resize( (_width/8) * (_height/8) );
	_art.resize(_terrain.size()) ;

}

//=========================================================
auto uomap::diffArt() const ->int {
	return _diffcount ;
}
//=========================================================
auto uomap::diffTerrain() const ->int {
	return _diffterrain ;
}

//==========================================================
auto uomap::loadTerrain(const std::filesystem::path &uoppath) ->bool {
	_diffterrain = 0 ;

	auto hash = this->format("build/map%ilegacymul/%s", _mapnum,"%.8u.dat");
	auto rvalue = false ;
	if (std::filesystem::exists(uoppath)){
		
		rvalue = loadUOP(uoppath.string(),0x300,hash);
		uop = rvalue ;
	}
	return rvalue ;
}
//==========================================================
auto uomap::loadMulTerrain(const std::filesystem::path &mulpath) -> bool{
	_diffterrain = 0 ;
	uop = false ;
	auto rvalue = false ;
	auto mul = std::ifstream(mulpath.string(),std::ios::binary) ;
	if (mul.is_open()){
		rvalue = true;
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
	return rvalue;
}
//==========================================================
auto uomap::loadArt(const std::filesystem::path &idxpath,const std::filesystem::path &mulpath) ->bool {
	_diffcount = 0 ;
	auto idx = std::ifstream(idxpath.string(),std::ios::binary);
	auto rvalue = idx.is_open();
	if (rvalue){
		auto mul = std::ifstream(mulpath.string(),std::ios::binary);
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
						_art[block].load(length, mul, _tileInfo);
					}
				}
				++block ;
			}
		}
	}
	return rvalue ;

}
//==========================================================
auto uomap::applyTerrainDiff(const std::filesystem::path &diffpath,const std::filesystem::path &diflpath) ->int {
	_diffterrain = 0 ;
	auto diffblock = std::ifstream(diflpath.string(),std::ios::binary) ;
	if (diffblock.is_open()) {
		auto diff = std::ifstream(diffpath.string(),std::ios::binary) ;
		if (diff.is_open()) {
			auto blocknum = std::uint32_t(0) ;
			auto data = std::vector<std::uint8_t>(196,0) ;
			while (diffblock.good() && !diffblock.eof()){
				diffblock.read(reinterpret_cast<char*>(&blocknum),4) ;
				if (diffblock.gcount()==4) {
					diff.read(reinterpret_cast<char*>(data.data()),196);
					if (diff.gcount()==196){
						_terrain[blocknum].load(data.data(), _tileInfo);
						_diffterrain++;
					}
				}
			}
		}
	}
	return _diffterrain;

}
//==========================================================
auto uomap::applyArtDiff(const std::filesystem::path &diflpath,const std::filesystem::path &difipath, const  std::filesystem::path &diffpath) ->int {
	_diffcount = 0 ;
	auto diffblock = std::ifstream(diflpath.string(),std::ios::binary) ;
	if (diffblock.is_open()){
		auto diffidx = std::ifstream(difipath.string(),std::ios::binary) ;
		if (diffidx.is_open()){
			auto diff = std::ifstream(diffpath.string(),std::ios::binary) ;
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
								_art[blocknum].load(length, diff, _tileInfo);
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
auto uomap::calcBlock(int x,int y) const ->int {
	// How many blocks is x across?
	return  ((x/8)*(_height/8)) + (y/8) ;
}
//=========================================================
auto uomap::calcXYOffset(int block) const ->std::pair<int,int> {
	auto x = (block / (_height/8)) * 8 ;
	auto y = (block % (_height/8)) * 8;
	return std::make_pair(x, y);
}
//=========================================================
auto uomap::blockAndIndexFor(int x, int y) const ->std::tuple<int, int, int>{
	auto block = calcBlock(x, y);
	auto offset = calcXYOffset(block);
	return std::make_tuple(block,x-offset.first,y-offset.second);
}
//=========================================================
auto uomap::processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool {
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
auto uomap::loadTerrainBlock(int blocknum,std::uint8_t *data)->void {
	_terrain[blocknum].load(data, _tileInfo);
}

//=========================================================
auto uomap::terrain(int x, int y) const ->const tile_t& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _terrain[blocknum].tile(xoffset, yoffset);
}
//=========================================================
auto uomap::terrain(int x, int y)  ->tile_t& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _terrain[blocknum].tile(xoffset, yoffset);

}
//=========================================================
auto uomap::art(int x, int y) const ->const std::vector<tile_t>& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _art[blocknum].tile(xoffset, yoffset);

}
//=========================================================
auto uomap::art(int x, int y)  ->std::vector<tile_t>& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _art[blocknum].tile(xoffset, yoffset);
}


//Copyright © 2022 Charles Kerr. All rights reserved.

#include "secparser.hpp"
#include "strutil.hpp"

#include <fstream>
#include <iostream>

using namespace std::string_literals ;


//=========================================================
// All the virtual funcitons
//=========================================================
//=========================================================
auto secparser::startSection(const std::string &secline)->void {
	
}
//=========================================================
auto secparser::keyvalue(const std::string &key, const std::string &value)->void {
	
}
//=========================================================
auto secparser::endSection() ->void {
	
}
//=========================================================
auto secparser::closeFile() ->void {
	
}
//=========================================================
auto secparser::openFile(const std::filesystem::path &path) ->void {
	
}


//=========================================================
// "true" functions
//=========================================================
//=========================================================
auto secparser::processFile(const std::string &filepath) ->bool {
	return processFile(std::filesystem::path(filepath));
}
//=========================================================
auto secparser::processFile(const std::filesystem::path &filepath) ->bool {
	auto input = std::ifstream(filepath.string());
	auto rvalue = input.is_open();
	if (rvalue){
		char buffer[4096] ;
		enum state_t {section,startsection,endsection};
		auto state = state_t::section ;
		auto block_comment = false ;
		auto secname = std::string() ;
		auto linenumber = 0 ;  // this is really just for error messages
		openFile(filepath);
		while (input.good() && !input.eof()){
			input.getline(buffer, 4095);
			
			if (input.gcount()>0){
				// we read in something
				linenumber++ ;
				buffer[input.gcount()] = 0 ;
				auto line = std::string(buffer);
				line = strutil::trim(strutil::strip(line,"//")) ;
				if (!line.empty()){
					if (line == "/*") {
						// we are turning on block comment
						block_comment = true ;
					}
					else if (line == "*/") {
						block_comment = false ;
					}
					if (!block_comment && line != "*/"){
						// we are processing the line!
						switch (state) {
							case state_t::section:{
								if (line[0] == '}' || line[0] == '{') {
									std::cerr <<"Invald synatx (start or end before a header section) at line "<<linenumber << " in: "<<filepath.filename().string()<<std::endl;
								}
								else if ((line[0] == '[') && (line[line.size()-1]==']')) {
									// it is a section header!
									secname = strutil::trim(line.substr(1,line.find("]")-1)) ;
									state = startsection ;
								}
								break;
							}
							case state_t::startsection:{
								if (line[0] == '[' || line[0] == '}') {
									std::cerr <<"Invald synatx (header or end before a start section) at line "<<linenumber << " in: "<<filepath.filename().string()<<std::endl;
								}
								else if (line[0] == '{') {
									startSection(secname);
									state = endsection;
								}
								break;
							}
							case state_t::endsection:{
								// Lets see if we got a section header or start before end/
								if (line[0] == '[' || line[0] == '{') {
									std::cerr <<"Invald synatx (header or start inside a section) at line "<<linenumber << " in: "<<filepath.filename().string()<<std::endl;
								}
								else if (line[0] != '}'){
									auto [key,value] = strutil::split(line,"=") ;
									keyvalue(key, value);
								}
								else {
									endSection();
									state = section ;
								}
								break;
							}
							default:
								break;
						}
					}
				}
			}
		}
		if (state != section){
			std::cerr <<"End of file before completed section at "<<linenumber << " in: "<<filepath.filename().string()<<std::endl;
		}
		closeFile();
	}
	return rvalue ;
}
//=======================================================================================================
auto secparser::processDirectory(const std::filesystem::path &dirpath, const std::string &extension) ->bool {
	bool rvalue = false ;
	if (std::filesystem::exists(dirpath)){
		if (std::filesystem::is_directory(dirpath)){
			rvalue = true ;
			std::vector<std::filesystem::path> directories  ; // we dont use a recursive iterator, as we want to get all done in a single directory first
			for (const auto &entry : std::filesystem::directory_iterator(dirpath) ) {
				if (entry.is_directory()){
					directories.push_back(entry.path());
				}
				else if (entry.is_regular_file()){
					auto process = true ;
					if (!extension.empty()) {
						if (entry.path().extension() != extension){
							process = false ;
						}
					}
					if (process){
						processFile(entry.path());
					}
				}
			}
			for (const auto &path : directories){
				processDirectory(path,extension);
			}
			
		}
	}
	return true ;
}

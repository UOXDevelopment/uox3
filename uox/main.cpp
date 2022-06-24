//Copyright © 2022 Charles Kerr. All rights reserved.

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include "uoxcfg.hpp"
#include "servcfg.hpp"
#include "shardcfg.hpp"
#include "uoxdata.hpp"
#include "dictionary.hpp"
#include "mapdef.hpp"
#include "uoworld.hpp"
#include "account.hpp"
//===========================================================================================
//	Forward declares
//===========================================================================================

using namespace std::string_literals ;
auto parseArgs(int argc,const char *argv[]) ->std::vector<std::string> ;

//===========================================================================================
//  main program
//===========================================================================================
int main(int argc, const char * argv[]) {
	auto configfile = std::string("uox.cfg");
	auto arguments = parseArgs(argc, argv) ;
	if (!arguments.empty()) {
		configfile = arguments[0];
	}
	//===========================================================
	// load our startup configuration
	//============================================================
	auto cfg = uoxcfg() ;
	if (!cfg.load(configfile)){
		std::cerr << "Unable to load startup configuration: "<<configfile << std::endl;
		return EXIT_FAILURE;
	}
	//===========================================================
	// load our server configuration
	//============================================================
	auto serverconfig = servercfg() ;
	if (!serverconfig.load(cfg.serverdata)){
		std::cerr << "Unable to load server configuration (configuration/system.cfg) at: "<<cfg.serverdata.string() << std::endl;
		return EXIT_FAILURE;

	}
	//===========================================================
	// set the shard paths
	//============================================================
	auto shardsetup = shardcfg() ;
	if (!shardsetup.load(cfg.sharddata)){
		std::cerr << "Unable to set shard data locations for: "<< cfg.sharddata.string()<<std::endl;
		return EXIT_FAILURE ;
	}
	//===========================================================
	// load our server data
	//============================================================
	auto serverdata = uoxdata() ;
	if (!serverdata.load(cfg.serverdata)){
		return EXIT_FAILURE;
	}
	//==============================================================
	// load dictionaries
	//===============================================================
	auto babble = dictionary(cfg.serverdata);
	// set the default language
	babble.setDefault(serverconfig.serverlanguage);
	std::cout <<"Loaded "<<babble.size() << " languages"<<std::endl;
	
	//==============================================================
	// load UOData
	//===============================================================
	auto mapdefinitions = mapdef(cfg.serverdata);
	std::cout <<"Loaded " << mapdefinitions.size() << " map definitions." << std::endl;
	
	std::cout <<"Loading UO data (this may take a while)." << std::endl;
	auto uomaps = uoworld() ;
	if (!uomaps.load(cfg.serverdata,cfg.uodir,mapdefinitions) ){
		std::cerr <<"Error loading UO data, check configuration/map.cfg or uo data location: " << cfg.uodir.string()<< std::endl;
		return EXIT_FAILURE ;
	}
	std::cout <<"Loaded " << uomaps.size() << " maps" << std::endl;
	
	//=====================================================================
	// setup accounts
	//====================================================================
	auto Account = account_t(shardsetup.paths[static_cast<int>(shardpaths::account)]);
	return EXIT_SUCCESS;
}

//===========================================================================================
// Support functions
//===========================================================================================

//===========================================================================================
auto parseArgs(int argc,const char *argv[]) ->std::vector<std::string> {
	auto rvalue = std::vector<std::string>() ;
	for (auto i = 1 ; i<argc ; ++i){
		rvalue.push_back(std::string(argv[i]));
	}
	return rvalue ;
}

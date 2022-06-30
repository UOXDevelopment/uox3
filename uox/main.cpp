//Copyright © 2022 Charles Kerr. All rights reserved.

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdexcept>

#include "uoxcfg.hpp"
#include "servcfg.hpp"
#include "shardcfg.hpp"
#include "uoxdata.hpp"
#include "dictionary.hpp"
#include "mapdef.hpp"
#include "uoworld.hpp"
#include "account.hpp"

#include "skilldefs.hpp"
#include "attributedef.hpp"
#include "telelocation.hpp"
#include "golocation.hpp"
#include "oredef.hpp"
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
	try {
		//===========================================================
		// load our startup configuration
		//============================================================
		auto cfg = uoxcfg(configfile) ;
		//===========================================================
		// set the shard paths
		//============================================================
		auto shardsetup = shardcfg(cfg.sharddata) ;
		//===========================================================
		// load our server configuration
		//============================================================
		auto serverconfig = servercfg(cfg.serverdata) ;

		auto serverdata = uoxdata(cfg.serverdata) ;
		//===========================================================
		// load our server data
		//============================================================
		auto skilldefinition = skilldefinition_t(cfg.serverdata) ;
		auto attributedefinition = attribdefinition_t(cfg.serverdata);
		auto teleportlocations = telelocation_t(cfg.serverdata);
		auto golocations = golocation_t(cfg.serverdata);
		auto oredefinition = oredef_t(cfg.serverdata);
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

	}
	catch(const std::exception &e){
		std::cerr << e.what()<<std::endl;
		return EXIT_FAILURE ;
	}

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

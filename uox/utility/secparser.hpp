//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef secparser_hpp
#define secparser_hpp

#include <cstdint>
#include <string>
#include <filesystem>
#include <optional>

//=========================================================
class secparser {
protected:
	virtual auto startSection(const std::string &secline)->void ;
	virtual auto keyvalue(const std::string &key, const std::string &value)->void ;
	virtual auto endSection() ->void ;
	virtual auto closeFile() ->void ;
	virtual auto openFile(const std::filesystem::path &path) ->void ;

public:
	auto processFile(const std::string &filepath) ->bool ;
	auto processFile(const std::filesystem::path &filepath) ->bool ;
	auto processDirectory(const std::filesystem::path &dirpath, const std::string &extension = ".dfn") ->bool ;
};
#endif /* secparser_hpp */

//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef account_hpp
#define account_hpp

#include <cstdint>
#include <string>
#include <array>
#include <map>
#include <filesystem>

#include "secparser.hpp"

constexpr auto maxcharacter = 7 ;
//====================================================================================================
// acct_character
//====================================================================================================
struct acct_character {
	
	std::uint32_t serial ;
	std::string name ;
	acct_character(const std::string &value = "");
	auto save(std::ostream &output,int number) const  ->void ;
};
//====================================================================================================
// acct_entry
//====================================================================================================
struct acct_entry {
	enum flagbits {Banned=0,Suspended=0x02,Public=4,LoggedIn=0x08,
		Block1=0x10,Block2=0x20,Block3=0x40,Block4=0x80,
		Block5=0x100,Block6=0x200,Block7=0x400,Seer = 0x2000,
		Counselor = 0x4000,GM=0x8000
	};
	bool valid ;
	bool online ;
	int acctnum ;
	std::string firstlogin;
	std::string lastlogin;
	std::string username ;
	std::string password ;
	std::string lastip ;
	int flags ;
	std::array<acct_character,maxcharacter> characters ;
	acct_entry(int acctnumber=0,int acctflags = 0,bool isvalid = true ) ;
	auto authenticate(const std::string &user, const std::string &pass) const ->bool ;
	auto save(std::ostream &output) const ->void ;
};
//====================================================================================================
// account_t
//====================================================================================================
class account_t : public secparser {

	acct_entry tempacct ;
	enum acctsectype {invalid, acct,addition};
	acctsectype processing;
	
	std::filesystem::path acctfile ;
	std::map<int,acct_entry> accounts ;
	auto header(std::ostream &output) const ->void ;
	auto writeDefault(const std::filesystem::path &filepath) ->bool ;
	
	auto startSection(const std::string &secline)->void final;
	auto keyvalue(const std::string &key, const std::string &value)->void final;
	auto endSection() ->void final;
	auto addAccount(const std::string &key , const std::string &value)->void ;
	auto processAccount(const std::string &key, const std::string &value) -> void;

public:
	mutable bool changed ;
	account_t(const std::filesystem::path &acctrid) ;
	auto load(const std::filesystem::path &acctdir) ->bool ;
	auto size() const ->size_t ;
	auto accountNumber() const -> int ;
	auto operator[](int index) const ->const acct_entry& ;
	auto operator[](int index)  -> acct_entry& ;
	auto operator[](const std::string& user) const ->const acct_entry& ;
	auto operator[](const std::string& user)  -> acct_entry& ;

	auto add(const std::string & user, const std::string pass,int flags = 0) ->bool ;
	auto save(const std::filesystem::path &acctdir) const ->bool ;
	auto save() const -> bool;

};
#endif /* account_hpp */

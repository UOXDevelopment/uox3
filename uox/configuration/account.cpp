//Copyright © 2022 Charles Kerr. All rights reserved.

#include "account.hpp"

#include "strutil.hpp"

#include <iostream>
#include <limits>
#include <algorithm>
#include <fstream>
#include <stdexcept>

using namespace std::string_literals ;

static auto invalid_account = acct_entry(-1,0,false) ;
//====================================================================================================
// acct_character
//====================================================================================================
//====================================================================================================
acct_character::acct_character(const std::string &value):serial(std::numeric_limits<std::uint32_t>::max()){
	name = "UNKNOWN"s ;
	if (!value.empty()){
		auto [ser,nam] = strutil::split(value,":") ;
		serial = strutil::ston<std::uint32_t>(ser);
		name = nam ;
	}
}
//====================================================================================================
auto acct_character::save(std::ostream &output,int number) const ->void {
	output <<"\tcharacter = "<<number<<" : "<<strutil::ntos(serial,strutil::radix_t::hex,true) <<" : " <<name <<"\n";
}
//====================================================================================================
// acct_character
//====================================================================================================

//====================================================================================================
acct_entry::acct_entry(int acctnumber,int acctflags,bool isvalid):online(false),acctnum(acctnumber),flags(acctflags),valid(isvalid){
	
}
//====================================================================================================
auto acct_entry::authenticate(const std::string &user, const std::string &pass) const ->bool {
	auto rvalue = false ;
	if (strutil::lower(username)== strutil::lower(user)){
		// paswword has to be an exact match
		rvalue = (pass == password);
	}
	return rvalue ;
}
//====================================================================================================
auto acct_entry::save(std::ostream &output) const ->void {
	output <<"[account "<<acctnum<<"]\n{\n";
	output <<"\tusername = "<<username<<"\n";
	output <<"\tpassword = "<<password<<"\n";
	output <<"\tflags = "<<strutil::ntos(flags,strutil::radix_t::hex,true)<<"\n";
	if (!firstlogin.empty()){
		output <<"\tfirstlogin = "<<firstlogin <<"\n" ;

	}
	if (!lastlogin.empty()){
		output <<"\tlastlogin = "<<lastlogin <<"\n" ;
	}
	if (!lastip.empty()){
		output <<"\tlastip = "<<lastip <<"\n" ;
	}
	int number = 0 ;
	for (const auto &entry:characters){
		entry.save(output,number++) ;
	}
	output <<"}\n" ;
}

//====================================================================================================
// account_t
//====================================================================================================
//====================================================================================================
auto account_t::header(std::ostream &output) const ->void {
	output <<"//=====================================================\n";
	output <<"//                     uox accounts\n";
	output <<"//=====================================================\n";
	output <<"//=====================================================\n";
	output <<"// flag bit meanings:\n";
	output <<"//      bit  0 = banned\n";
	output <<"//      bit  1 = suspended\n";
	output <<"//      bit  2 = public\n";
	output <<"//      bit  3 = loggedin\n";
	output <<"//      bit  4 = character 0 blocked\n";
	output <<"//      bit  5 = character 1 blocked\n";
	output <<"//      bit  6 = character 2 blocked\n";
	output <<"//      bit  7 = character 3 blocked\n";
	output <<"//      bit  8 = character 4 blocked\n";
	output <<"//      bit  9 = character 5 blocked\n";
	output <<"//      bit 10 = character 6 blocked\n";
	output <<"//      bit 11 = unused\n";
	output <<"//      bit 12 = unused\n";
	output <<"//      bit 13 = Seer\n";
	output <<"//      bit 14 = Counselor\n";
	output <<"//      bit 15 = GM account\n";
	output <<"//=====================================================\n";
	output << std::endl;
}
//====================================================================================================
auto account_t::writeDefault(const std::filesystem::path &filepath) ->bool {
	auto output = std::ofstream(filepath.string());
	auto rvalue = false ;
	if (output.is_open()){
		header(output) ;
		auto entry = acct_entry(0,0x8000) ;
		entry.username = "admin";
		entry.password = "admin" ;
		entry.save(output) ;
		entry =acct_entry(1,0) ;
		entry.username = "guest1";
		entry.password = "guest1" ;
		entry.save(output);
		rvalue = true ;
	}
	return rvalue ;
}
//====================================================================================================
account_t::account_t(const std::filesystem::path &accdir) {
	if (!accdir.empty()){
		if (!load(accdir)){
			throw std::runtime_error("Unable to process accounts.adm at: "s +accdir.string());
		}
	}
}
//====================================================================================================
auto account_t::load(const std::filesystem::path &acctdir) ->bool {
	auto rvalue = false ;
	auto path = acctdir / std::filesystem::path("accounts.adm") ;
	if (!std::filesystem::exists(path)) {
		writeDefault(path);
	}
	processing = invalid;
	rvalue = processFile(path) ;
	changed = false ;
	acctfile = path ;
	return rvalue ;
}

//====================================================================================================
auto account_t::startSection(const std::string &secline)->void {
	auto [sec,num] = strutil::split(strutil::lower(secline)," ") ;
	if (sec == "account"){
		if (!num.empty()){
			tempacct = acct_entry(strutil::ston<int>(num),0) ;
		}
		else {
			tempacct = acct_entry(accountNumber()+1,0) ;
		}
		processing = acct ;
	}
	else if (sec == "create"){
		processing = addition;
	}
	else {
		processing = invalid;
	}
}
//====================================================================================================
auto account_t::keyvalue(const std::string &key, const std::string &value)->void {
	switch (processing){
		case acct:
			processAccount(key, value);
			break;
		case addition:
			addAccount(key, value);
			break;
		default:
			break;
	}
}
//====================================================================================================
auto account_t::processAccount(const std::string &key, const std::string &value) ->void {
	auto lkey = strutil::lower(key) ;
	if (lkey == "username"){
		tempacct.username = value ;
	}
	else if (lkey == "password"){
		tempacct.username = value ;
	}
	else if (lkey == "flags"){
		tempacct.flags = strutil::ston<int>(value) ;
	}
	else if (lkey == "firstlogin"){
		tempacct.firstlogin = value;
	}
	else if (lkey == "lastlogin"){
		tempacct.lastlogin = value;
	}
	else if (lkey == "lastip"){
		tempacct.lastip = value;
	}
	else if (lkey == "character"){
		auto [num,rest] = strutil::split(value,":"); ;
		auto character = acct_character(rest) ;
		tempacct.characters[strutil::ston<int>(num)] = character ;
		
	}
}
//====================================================================================================
auto account_t::addAccount(const std::string &key , const std::string &value)->void {
	auto lkey = strutil::lower(key) ;
	if (lkey == "add") {
		auto values = strutil::parse(value,",") ;
		auto username = std::string();
		auto password = std::string() ;
		auto flags = 0 ;
		switch (values.size()){
			default:
			case 3:
				flags = strutil::ston<int>(values[2]);
				[[fallthrough]];
			case 2:
				password = values[1] ;
				[[fallthrough]] ;
			case 1:
				username = values[0];
				[[fallthrough]];
			case 0:
				break;
		}
		if (!username.empty()) {
			add(username,password,flags);
		}
		
	}
}
//====================================================================================================
auto account_t::endSection() ->void {
	if (processing == acct) {
		accounts.insert_or_assign(tempacct.acctnum,tempacct);
	}
	processing = invalid;
}


//====================================================================================================
auto account_t::size() const ->size_t {
	return accounts.size() ;
}
//====================================================================================================
auto account_t::accountNumber() const -> int {
	auto rvalue = -1 ;
	auto iter = accounts.rbegin() ;
	if (iter != accounts.rend()){
		rvalue = iter->first ;
	}
	return rvalue ;
}

//====================================================================================================
auto account_t::operator[](int index) const ->const acct_entry&{
	auto iter = accounts.find(index) ;
	if (iter != accounts.end()){
		return iter->second ;
	}
	return invalid_account ;
}
//====================================================================================================
auto account_t::operator[](int index)  -> acct_entry&{
	auto iter = accounts.find(index) ;
	if (iter != accounts.end()){
		return iter->second ;
	}
	return invalid_account ;
}
//====================================================================================================
auto account_t::operator[](const std::string& user) const ->const acct_entry& {
	auto luser = strutil::lower(user) ;
	auto iter = std::find_if(accounts.begin(),accounts.end(),[luser](const std::pair<int,acct_entry> &entry){
		return luser == strutil::lower(entry.second.username);
	});
	if (iter != accounts.end()){
		return iter->second ;
	}
	return invalid_account ;
}
//====================================================================================================
auto account_t::operator[](const std::string& user)  -> acct_entry& {
	auto luser = strutil::lower(user) ;
	auto iter = std::find_if(accounts.begin(),accounts.end(),[luser](const std::pair<int,acct_entry> &entry){
		return luser == strutil::lower(entry.second.username);
	});
	if (iter != accounts.end()){
		return iter->second ;
	}
	return invalid_account ;

}

//====================================================================================================
auto account_t::add(const std::string & user, const std::string pass,int flags) ->bool {
	const auto &oldacc = this->operator[](user) ;
	auto rvalue = oldacc.valid ;
	if (!rvalue){
		auto acc = acct_entry(accountNumber()+1,flags,true);
		acc.username = user ;
		acc.password = pass ;
		accounts.insert_or_assign(acc.acctnum, acc);
		rvalue = true ;
		changed = true ;
	}
	return rvalue ;
	
}
//====================================================================================================
auto account_t::save(const std::filesystem::path &acctdir) const ->bool {
	auto path = acctdir / std::filesystem::path("accounts.adm") ;
	auto output = std::ofstream(path.string()) ;
	auto rvalue = false ;
	if (output.is_open()) {
		rvalue =  true ;
		header(output) ;
		for (const auto &[number,acct]:accounts){
			acct.save(output) ;
		}
	}
	return rvalue ;
}
//====================================================================================================
auto account_t::save() const ->bool {
	auto rvalue = true ;
	if (changed) {
		rvalue = false ;
		if (!acctfile.empty()) {
			auto output = std::ofstream(acctfile.string()) ;
			if (output.is_open()) {
				rvalue = true ;
				header(output) ;
				for (const auto &[number,acct]:accounts){
					acct.save(output) ;
				}
			}
		}
	}
	return rvalue;
}

//
// Created by focusexplorer(github) on 2023/10/27.
//
#include "hyperscan_cpp.h"
#include "sstream"
namespace hsc
{
static int SimpleMatch_eventHandler(unsigned int id, unsigned long long from,
	unsigned long long to, unsigned int flags, void *ctx) {
	*(bool *)ctx = true;
	return HS_SCAN_TERMINATED;
}
static int MultiMatch_onMatch(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx)
{
	MultiMatch* p=(MultiMatch*)ctx;
	return p->onMatch(id,from,to,flags,p->user_ctx);
}
SimpleMatch::SimpleMatch(const char* pattern)
{
	if (hs_compile(pattern, HS_FLAG_DOTALL, HS_MODE_BLOCK, NULL, &database, &compile_err) != HS_SUCCESS) {
		std::stringstream ss;
		ss<<"ERROR: Unable to compile pattern \""<<pattern<<"\": "<<compile_err->message;
		err=ss.str();
		valid= false;
		return;
	}
	if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
		std::stringstream ss;
		ss<<"ERROR: Unable to allocate scratch space. Exiting.";
		err=ss.str();
		valid= false;
		return;
	}
	valid=true;
}
SimpleMatch::~SimpleMatch()
{
	if(database)
		hs_free_database(database);
	if(compile_err)
		hs_free_compile_error(compile_err);
	if(scratch)
		hs_free_scratch(scratch);
}
bool SimpleMatch::match(const std::string& str)
{
	err.clear();
	if(!valid)
	{
		err="SimpleMatch is not valid, the pattern may be wrong.";
		return false;
	}
	bool match=false;
	auto ret=hs_scan(database, str.c_str(), (unsigned int)str.size(), 0, scratch,  SimpleMatch_eventHandler, &match);
	if(ret!= HS_SUCCESS && ret!=HS_SCAN_TERMINATED)
	{
		std::stringstream ss;
		ss<<"ERROR: Unable to scan input buffer. Exiting."<<ret;
		err=ss.str();
		return false;
	}
	return match;
}

void MultiMatch::add_reg(const string& regexp, int id)
{
	expressions.push_back(regexp);
	ids.push_back(id);
	flags.push_back(HS_FLAG_DOTALL|HS_FLAG_SINGLEMATCH);
}
bool MultiMatch::compile()
{
	vector<const char*> exps;
	for(auto&v:expressions)
	{
		exps.push_back(v.c_str());
	}
	hs_error_t rerr;
	rerr = hs_compile_multi(exps.data(), flags.data(), ids.data(),exps.size(), HS_MODE_BLOCK, nullptr, &db_block, &compileErr);

	if (rerr != HS_SUCCESS) {
		std::stringstream ss;
		if (compileErr->expression < 0) {
			// The error does not refer to a particular expression.
			ss<<"compile error:"<<compileErr->message;
		} else {
			ss<<"ERROR: Pattern "<<exps[compileErr->expression]<<", failed compilation with error:"<<compileErr->message;
		}
		err=ss.str();
		return false;
	}
	rerr = hs_alloc_scratch(db_block, &scratch);
	if (rerr != HS_SUCCESS) {
		std::stringstream ss;
		ss<<"ERROR: could not allocate scratch space. Exiting.";
		err=ss.str();
		return false;
	}
	return true;
}
bool MultiMatch::scan(const string& msg,void*ctx)
{
	user_ctx=ctx;
	hs_error_t rerr = hs_scan(db_block, msg.c_str(), msg.length(), 0, scratch, MultiMatch_onMatch, this);
	if (rerr != HS_SUCCESS) {
		std::stringstream ss;
		ss<< "ERROR: Unable to scan packet. Exiting.";
		err=ss.str();
		return false;
	}
	return true;
}
MultiMatch::~MultiMatch()
{
	if (db_block)
		hs_free_database(db_block);
	if (compileErr)
		hs_free_compile_error(compileErr);
	if (scratch)
		hs_free_scratch(scratch);
}

}//namespace hsc
//
// Created by focusexplorer(github) on 2023/10/27.
//

#ifndef HYPERSCAN_CPP_PROJ_LIBRARY_HYPERSCAN_CPP_H_
#define HYPERSCAN_CPP_PROJ_LIBRARY_HYPERSCAN_CPP_H_
#include "hs.h"
#include "string"
#include "vector"
namespace hsc
{
using std::string;
using std::vector;
class SimpleMatch
{
 public:
	SimpleMatch(const char* pattern);
	~SimpleMatch();
	operator bool(){return valid;};
	bool match(const std::string& str);
	const char* error(){return err.c_str();};

 private:
	hs_database_t *database{0};
	hs_compile_error_t *compile_err{0};
	hs_scratch_t *scratch {0};

	bool valid{false};
	std::string err;
};


class MultiMatch
{
 public:
	~MultiMatch();
	void add_reg(const string&regexp,int id);
	bool compile();
	bool scan(const string&data,void*ctx);

	//通过虚函数回调的方式通知哪个ID的pattern被匹配到。
	virtual  int onMatch(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx){return 0;};
	const char*error(){return err.c_str();};

 private:
	vector<string> expressions;
	vector<unsigned >ids;
	vector<unsigned >flags;
	hs_database_t *db_block{0};
	hs_compile_error_t *compileErr{0};
	hs_scratch_t *scratch{0};//必须初始化为0，否则分配会失败；
	std::string err;
 public:
	void* user_ctx{0};
};
}
#endif //HYPERSCAN_CPP_PROJ_LIBRARY_HYPERSCAN_CPP_H_

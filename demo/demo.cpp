//
// Created by focusexplorer on 2023/10/27.
//
#include "iostream"
#include "hyperscan_cpp.h"
using namespace std;
int main()
{
	{
		hsc::SimpleMatch simpleMatch("^hello");
		if(!simpleMatch)
		{
			cout<<"wrong pattern:"<<simpleMatch.error()<<endl;
			return -1;
		}
		cout<<simpleMatch.match("hello world")<<endl;
		cout<<simpleMatch.match("aaa hello world")<<endl;
	}
	{
		hsc::SimpleMatch simpleMatch(".*in.*$");
		cout<<simpleMatch.match("I am in world")<<endl;
		cout<<simpleMatch.match("hello world")<<endl;
		cout<<simpleMatch.match("in world")<<endl;
		cout<<simpleMatch.match("world in")<<endl;
	}
	{
		cout<<"MultiMatch:"<<endl;
		class MyMatch:public hsc::MultiMatch{
		 public:
			virtual int onMatch(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx)
			{
//				cout<<"match:"<<id<<","<<from<<","<<to<<","<<flags<<","<<*(string*)ctx<<endl;
				result[id]=true;
				return 0;
			}
			vector<bool> result;
		};
		MyMatch mm;
		vector<string> patters{"^hello.*",".*world$",".*in.*"};
		mm.result.resize(patters.size(),false);
		for(int i=0;i<patters.size();i++)
		{
			mm.add_reg(patters[i],i);
		}
		if(!mm.compile())
		{
			cout<<mm.error()<<endl;
			return -1;
		}
		string data="hello world";
		if(!mm.scan(data,&data))
		{
			cout<<mm.error()<<endl;
			return -1;
		}
		for(int i=0;i<mm.result.size();i++)
		{
			cout<<i<<":"<<mm.result[i]<<endl;
		}
		data="I love the world";
		mm.result.assign(mm.result.size(),false);
		if(!mm.scan(data,&data))
		{
			cout<<mm.error()<<endl;
			return -1;
		}
		for(int i=0;i<mm.result.size();i++)
		{
			cout<<i<<":"<<mm.result[i]<<endl;
		}

	}
	return 0;
}
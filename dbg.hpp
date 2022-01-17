#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <queue>
#include <stack>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windef.h>
#include <Winbase.h>
#include <Wincon.h>
#endif

#define dbg(...) debugFunc(__FILE__, __LINE__, __func__, #__VA_ARGS__ __VA_OPT__(,) __VA_ARGS__)

template<typename T>
static std::string getDemangledTypename()
{
#ifdef __GNUC__
	const char *typeName = typeid(T).name();
	int status;
	char *demangledName = abi::__cxa_demangle(typeName, NULL, NULL, &status);
	std::string temp;
	
	if(status == 0)
	{
		temp = demangledName;
		free(demangledName);
	}

	return temp;
#endif
	return "";
}

template<typename A, typename B>
static std::ostream &operator<<(std::ostream &str, const std::pair<A, B> &p)
{
	str << "(" << p.first << ", " << p.second << ")";
	return str;
}

template<typename T>
static std::ostream &operator<<(std::ostream &str, const std::vector<T> &v)
{
	str << "[";
	bool first = true;
	for(const T& x : v)
	{
		if(!first)
			str << ", ";
		str << x;
		first = false;
	}
	str << "]";
	return str;
}

template<typename T>
static std::ostream &operator<<(std::ostream &str, const std::set<T> &s)
{
	str << "{";
	bool first = true;
	for(const T& x : s)
	{
		if(!first)
			str << ", ";
		str << x;
		first = false;
	}
	str << "}";
	return str;
}

template<typename Key, typename Value>
static std::ostream &operator<<(std::ostream &str, const std::map<Key, Value> &m)
{	
	str << "{";
	bool first = true;
	for(const auto& x : m)
	{
		if(!first)
			str << ", ";
		str << x;
		first = false;
	}
	str << "}";
	return str;
}


template<typename T>
static std::ostream &operator<<(std::ostream &str, const std::queue<T> &q)
{
	str << "[";
	std::queue<T> temp = q;
	bool first = true;
	while(!temp.empty())
	{
		if(!first)
			str << ", ";
		str << temp.front();
		temp.pop();
		first = false;
	}
	str << "]";
	return str;
}

template<typename T>
static std::ostream &operator<<(std::ostream &str, const std::stack<T> &s)
{
	str << "[";
	std::stack<T> temp = s;
	bool first = true;
	while(!temp.empty())
	{
		if(!first)
			str << ", ";
		str << temp.top();
		temp.pop();
		first = false;
	}
	str << "]";
	return str;
}

template<typename T>
static std::ostream &operator<<(std::ostream &str, const std::priority_queue<T> &pq)
{
	str << "[";
	std::priority_queue<T> temp = pq;
	bool first = true;
	while(!temp.empty())
	{
		if(!first)
			str << ", ";
		str << temp.top();
		temp.pop();
		first = false;
	}
	str << "]";
	return str;
}

template<typename... Args>
void debugFunc(const char *file, unsigned int line, const char *funcName, const char *argNames, Args&&... argValues)
{
	std::cout << "[" << file << ":" << line << " @ " << funcName << "] " << argNames << std::endl;
}

void (*defSIGINTHandler) (int);
void (*defSIGSEGVHandler) (int);
void (*defSIGFPEHandler) (int);
void (*defSIGABRTHandler) (int);

static void _sigHandler(int sigN)
{
	//Installing a signal handler to avoid color contamination to console
	
	if(sigN == SIGINT)
		return defSIGINTHandler(sigN);
	else if(sigN == SIGSEGV)
		return defSIGSEGVHandler(sigN);
	else if(sigN == SIGFPE)
		return defSIGFPEHandler(sigN);
	else if(sigN == SIGABRT)
		return defSIGABRTHandler(sigN);
	//exit(EXIT_FAILURE);
}

static void _final()
{

}

struct _init
{
	_init()
	{
		atexit(_final);
		defSIGINTHandler = signal(SIGINT, _sigHandler);
		defSIGSEGVHandler = signal(SIGSEGV, _sigHandler);
		defSIGFPEHandler = signal(SIGSEGV, _sigHandler);
		defSIGABRTHandler = signal(SIGABRT, _sigHandler);
	}
} _initInstance;

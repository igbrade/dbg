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
#include <Windows.h>
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

static volatile sig_atomic_t intOcurred;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
std::vector<int> splitArgnames(const char *argNames)
{
	std::vector<int> ret;
	ret.push_back(0);
	
	int parenthesis = 0, brackets = 0, sqbrackets = 0;
	bool quot = false;
	
	size_t i;
	for(i = 0; argNames[i] != '\0'; ++i)
	{
		if(argNames[i] == ',' && parenthesis == 0 && brackets == 0 && sqbrackets == 0 && !quot)
		{
			ret.push_back(i + 1);
		}
		parenthesis += (argNames[i] == '(' ? 1 : (argNames[i] == ')' ? -1 : 0));
		brackets += (argNames[i] == '{' ? 1 : (argNames[i] == '}' ? -1 : 0));
		sqbrackets += (argNames[i] == '[' ? 1 : (argNames[i] == ']' ? -1 : 0));
		quot = (argNames[i] == '\"' ? !quot : quot);
	}
	ret.push_back(i + 1);
	return ret;
}

static void logArg(const char *argNames, const std::vector<int> &splitIndices, int curIndex)
{
}
#pragma GCC diagnostic pop

template<typename First, typename... Args>
static void logArg(const char *argNames, const std::vector<int> &splitIndices, int curIndex, First&& first, Args&&... args)
{
	std::string_view curArgName(argNames + splitIndices[curIndex], (splitIndices[curIndex + 1] - splitIndices[curIndex] - 1));
	std::cerr << curArgName << "(" << getDemangledTypename<decltype(first)>() <<  ")" << " = " << first; 
	if(sizeof...(Args))
	{
		std::cerr << ", ";
		logArg(argNames, splitIndices, curIndex + 1, std::forward<Args>(args)...);
	}
}

template<typename... Args>
void debugFunc(const char *file, unsigned int line, const char *funcName, const char *argNames, Args&&... argValues)
{
	std::cout << std::flush;
	std::cerr << std::flush;
#ifdef _WIN32
	HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
	GetConsoleScreenBufferInfo(hErr, &csbInfo);
	SetConsoleTextAttribute(hErr, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
	std::cerr << "\u001b[31m";
#endif
	intOcurred = 0;
	void (*prevSIGINTHandler) (int) = signal(SIGINT, [](int sig)
	{
		intOcurred = 1;	
	});

//Actual printing of stuff goes here
	std::cerr << "[" << file << ":" << line << " @ " << funcName << "] ";
	std::vector<int> argPos = splitArgnames(argNames);
	logArg(argNames, argPos, 0, std::forward<Args>(argValues)...);
	std::cerr << std::endl;
	/*for(size_t i = 0; i + 1 < argPos.size(); ++i)
	{
		std::cerr << std::string_view(argNames + argPos[i], (argPos[i + 1] - argPos[i] - 1)) << std::endl;
	}*/
//End of print

#ifdef _WIN32
	SetConsoleTextAttribute(hErr, csbInfo.wAttributes);
#else
	std::cerr << "\u001b[0m";
#endif
	signal(SIGINT, prevSIGINTHandler);
	if(intOcurred)
		raise(SIGINT);
}

#if false

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
#endif

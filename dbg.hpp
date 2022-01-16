#include <iostream>
#include <vector>
#include <string>

#ifdef __GNUC__
#include <cxxabi.h>
#include <stdlib.h>
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

template<typename... Args>
void debugFunc(const char *file, unsigned int line, const char *funcName, const char *argNames, Args&&... argValues)
{
	std::cout << "[" << file << ":" << line << " @ " << funcName << "] " << argNames << std::endl;
}

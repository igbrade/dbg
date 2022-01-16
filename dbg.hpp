#include <iostream>

#define dbg(...) debugFunc(__FILE__, __LINE__, #__VA_ARGS__)

void debugFunc(const char *file, unsigned int line, const char *argNames)
{
	std::cout << "[" << file << ":" << line << "] " << argNames << std::endl;
}

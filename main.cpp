#include <iostream>
#include <vector>

int main()
{
	int x = 10, y = 20;
	std::vector<int> v = {5, 7};

	dbg(x);

	dbg();

	dbg(x, y);

	std::cout << v << std::endl;

	std::cout << getDemangledTypename<decltype(v)>() << std::endl;
}

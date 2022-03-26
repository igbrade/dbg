#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <utility>
#include <queue>
#include <stack>

void test1()
{
	int x = 10, y = 20;
	std::vector<int> v = {5, 7};
	std::set<int> s = {10, -5};
	std::pair<int, char> p = {1, 'c'};
	std::map<std::string, int> m = {{"abc", 1}, {"foo", 7}};
	std::queue<int> q;
	q.push(263);
	q.push(429);
	std::stack<int> st;
	st.push(10);
	st.push(90);
	std::priority_queue<int> pq;
	pq.push(9);
	pq.push(5);
	pq.push(17);

	dbg(x);

	dbg();

	dbg(x, y);

	std::cout << v << std::endl;

	std::cout << s << std::endl;

	std::cout << p << std::endl;

	std::cout << m << std::endl;

	std::cout << q << std::endl;

	std::cout << st << std::endl;

	std::cout << pq << std::endl;

	std::cout << getDemangledTypename<decltype(v)>() << std::endl;
}

void testColorContamination()
{
	int x = 10;
	while(true)
	{
		dbg(x);
	}
}

int main()
{
	testColorContamination();
}

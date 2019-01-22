#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <functional>

using namespace std;

template<typename type1, typename type2>
struct PairHash{
	size_t operator()(const pair<type1, type2>& key) const noexcept
	{
		size_t h1 = hash<type1>{}(key.first), h2 = hash<type2>{}(key.second);
		return h1 ^ (h2 << 1);
	}
};

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

#include <stdint.h>


#include "alg_tools.h"

int main()
{
    //std::string in_str("Variables declared at block scope with the specifier static have static storage duration but are initialized the first time control passes through their declaration (unless their initialization is zero- or constant-initialization, which can be performed before the block is first entered). On all further calls, the declaration is skipped." \
    //    "If the initialization throws an exception, the variable is not considered to be initialized, and initialization will be attempted again the next time control passes through the declaration." \
    //    "If the initialization recursively enters the block in which the variable is being initialized, the behavior is undefined."\
    //    "If multiple threads attempt to initialize the same static local variable concurrently, the initialization occurs exactly once(similar behavior can be obtained for arbitrary functions with std::call_once)."\
    //    "Note: usual implementations of this feature use variants of the double - checked locking pattern, which reduces runtime overhead for already - initialized local statics to a single non - atomic boolean comparison.	(since C++11)"\
    //      "The destructor for a block - scope static variable is called at program exit, but only if the initialization took place successfully.");
    //
	std::string in_str("come to me");
    std::unordered_map<rtvdt::Element, uint64_t, rtvdt::KeyHash> dict;
    countElements(in_str.data(), in_str.size(), 1, dict);

    // Print
    std::cout << "Len of buffer: " << in_str.size() << std::endl;
    std::cout << "Len of dictionary: " << dict.size() << std::endl;
    for (auto item : dict)
        std::cout << item.first << " : " << item.second << std::endl;

    std::multimap<uint64_t, std::unique_ptr<TreeNode>> sort_dict;
    sortDict(dict, sort_dict);

    std::cout << "Sort dict: " << std::endl;
    //for (auto item : sort_dict)   // Doesn't work. WTF?
    for (auto item = sort_dict.begin(); item != sort_dict.end(); ++item)
        std::cout << item->first << " : " << *item->second->elem << std::endl;

	buildTree(sort_dict);

	auto it = sort_dict.begin();
	if (it != sort_dict.end())
	{
		containers::vector_bb currentCode;
		auto currentNode = it->second.get();
		std::unordered_map<rtvdt::Element, containers::vector_bb, rtvdt::KeyHash> _outDict;
		recursiveBypass(currentNode, currentCode, _outDict);
		std::cout << "currentCode: \'" << (std::string)currentCode << "\'" << std::endl;
		for (const auto& item : _outDict)
		{
			std::cout << "Element: \'" << (std::string)item.first << "\' vector_bb: \'" << (std::string)item.second << "\'" << std::endl;
		}
	}
}
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <utility>
#include <memory>
#include <stdint.h>

#include "vector_bb.h"
#include "element.h"

// Подсчет кол-ва повторений каждого элемента в сообщении
void countElements(const char *in_buf, const uint64_t len,  // Буфер
	const int gran,											// Грануляция или длина элемента
    std::unordered_map<rtvdt::Element, uint64_t, rtvdt::KeyHash> & out_dict)
{
    uint64_t ind = 0;
    while (ind < len)
    {
        if (ind + gran > len)
            break;

        const char *ptr = in_buf + ind;
        const rtvdt::Element e(ptr, gran);
        ++out_dict[e];
        ind += gran;
    }
}

// Представляем каждый элемент в виде узла с указателями на правого и левого потомка и сортируем узлы 
// в порядке возрастания их частоты появления в исходном сообщении
void sortDict(const std::unordered_map<rtvdt::Element, uint64_t, rtvdt::KeyHash> & dict,
    std::multimap<uint64_t, std::unique_ptr<treeNode>> & out_dict)
{
    for (auto item : dict)
    {
        auto it = std::make_unique<treeNode>();
        it->elem = std::make_unique<rtvdt::Element>(item.first);
        out_dict.insert(std::make_pair(item.second, std::move(it)));
    }
}

// Строим дерево рекурсивно путем объединения двух узлов с наименьшей частотой появления
void buildTree(std::multimap<uint64_t, std::unique_ptr<treeNode>> & dict)
{
    auto it = dict.begin();
    while (dict.size() > 1)
    {
		auto sz = dict.size();
		auto node = std::make_unique<treeNode>();
		auto left = dict.begin();
		auto right = left;
		++right;
		node->left = std::move(left->second);
		node->right = std::move(right->second);
		auto weight = left->first + right->first;
		dict.erase(left);
		dict.erase(right);
		dict.insert(std::make_pair(weight, std::move(node)));
    }
}

void recursiveBypass(const treeNode *node, 
					 containers::vector_bb &curCode,
					 std::unordered_map<rtvdt::Element, containers::vector_bb, rtvdt::KeyHash> &outDict)
{
	if (node->left)
	{
		curCode.push_back(false);
		node = node->left.get();
		recursiveBypass(node, curCode, outDict);
	}
	if (node->right)
	{
		curCode.push_back(true);
		node = node->right.get();
		recursiveBypass(node, curCode, outDict);
	}

	//We must meet Element
	if (node->elem)
		outDict.insert(std::make_pair(*node->elem, curCode));

	curCode.pop_back();
}

void bypassTree(const treeNode &node,
				const char *inMsg,
				const uint64_t len,
				std::vector<char> &outMsg, 
				std::unordered_map<rtvdt::Element,containers::vector_bb,rtvdt::KeyHash> &outDict)
{
	containers::vector_bb currentCode;
	auto currentNode = &node;
	recursiveBypass(currentNode, currentCode, outDict);

	auto gran = outDict.begin()->first.m_len;
    uint64_t offset = 0;
	while (offset < len)
	{
        rtvdt::Element cur_element(inMsg + offset, gran);
		auto it = outDict.find(cur_element);
		if (it != outDict.end())
		{

		}
        offset += gran;

    }
}

int main()
{
    std::string in_str("Variables declared at block scope with the specifier static have static storage duration but are initialized the first time control passes through their declaration (unless their initialization is zero- or constant-initialization, which can be performed before the block is first entered). On all further calls, the declaration is skipped." \
        "If the initialization throws an exception, the variable is not considered to be initialized, and initialization will be attempted again the next time control passes through the declaration." \
        "If the initialization recursively enters the block in which the variable is being initialized, the behavior is undefined."\
        "If multiple threads attempt to initialize the same static local variable concurrently, the initialization occurs exactly once(similar behavior can be obtained for arbitrary functions with std::call_once)."\
        "Note: usual implementations of this feature use variants of the double - checked locking pattern, which reduces runtime overhead for already - initialized local statics to a single non - atomic boolean comparison.	(since C++11)"\
          "The destructor for a block - scope static variable is called at program exit, but only if the initialization took place successfully.");
    
    std::unordered_map<rtvdt::Element, uint64_t, rtvdt::KeyHash> dict;
    countElements(in_str.data(), in_str.size(), 1, dict);

    // Print
    std::cout << "Len of buffer: " << in_str.size() << std::endl;
    std::cout << "Len of dictionary: " << dict.size() << std::endl;
    for (auto item : dict)
        std::cout << item.first << " : " << item.second << std::endl;

    std::multimap<uint64_t, std::unique_ptr<treeNode>> sort_dict;
    sortDict(dict, sort_dict);

    std::cout << "Sort dict: " << std::endl;
    //for (auto item : sort_dict)   // Doesn't work. WTF?
    for (auto item = sort_dict.begin(); item != sort_dict.end(); ++item)
        std::cout << item->first << " : " << *item->second->elem << std::endl;

	buildTree(sort_dict);

	

}
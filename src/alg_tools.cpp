#include <utility>
#include <memory>

#include "alg_tools.h"


// Подсчет кол-ва повторений каждого элемента в сообщении
void countElements(const char* _inBuf, const uint64_t _len,		// Буфер
					const int _gran,							// Грануляция или длина элемента в байтах
					std::unordered_map<rtvdt::Element, uint64_t, rtvdt::KeyHash>& _outDict)
{
	uint64_t ind = 0;
	while (ind < _len)
	{
		if (ind + _gran > _len)
			break;

		const char *ptr = _inBuf + ind;
		const rtvdt::Element e(ptr, _gran);
		++_outDict[e];
		ind += _gran;
	}
}

// Представляем каждый элемент в виде узла с указателями на правого и левого потомка и сортируем узлы 
// в порядке возрастания их частоты появления в исходном сообщении
void sortDict(const std::unordered_map<rtvdt::Element, uint64_t, rtvdt::KeyHash>& _dict,
			std::multimap<uint64_t, std::unique_ptr<treeNode>>& _outDict)
{
	for (auto item : _dict)
	{
		auto it = std::make_unique<treeNode>();
		it->elem = std::make_unique<rtvdt::Element>(item.first);
		_outDict.insert(std::make_pair(item.second, std::move(it)));
	}
}

// Строим дерево рекурсивно путем объединения двух узлов с наименьшей частотой появления
void buildTree(std::multimap<uint64_t, std::unique_ptr<treeNode>>& _dict)
{
	
	while (_dict.size() > 1)
	{
		auto sz = _dict.size();
		auto node = std::make_unique<treeNode>();
		auto left = _dict.begin();
		auto right = left;
		++right;
		node->left = std::move(left->second);
		node->right = std::move(right->second);
		auto weight = left->first + right->first;
		_dict.erase(left);
		_dict.erase(right);
		_dict.insert(std::make_pair(weight, std::move(node)));
	}
}

void recursiveBypass(const treeNode* _node,
	containers::vector_bb& _curCode,
	std::unordered_map<rtvdt::Element, containers::vector_bb, rtvdt::KeyHash>& _outDict)
{
	if (_node->left)
	{
		_curCode.push_back(false);
		_node = _node->left.get();
		recursiveBypass(_node, _curCode, _outDict);
	}
	if (_node->right)
	{
		_curCode.push_back(true);
		_node = _node->right.get();
		recursiveBypass(_node, _curCode, _outDict);
	}

	//We must meet Element
	if (_node->elem)
		_outDict.insert(std::make_pair(*_node->elem, _curCode));

	_curCode.pop_back();
}

void bypassTree(const treeNode& _node,
	const char* _inMsg,
	const uint64_t _len,
	std::vector<char>& _outMsg,
	std::unordered_map<rtvdt::Element, containers::vector_bb, rtvdt::KeyHash>& _outDict)
{
	containers::vector_bb currentCode;
	auto currentNode = &_node;
	recursiveBypass(currentNode, currentCode, _outDict);

	auto gran = _outDict.begin()->first.m_len;
	uint64_t offset = 0;
	while (offset < _len)
	{
		rtvdt::Element cur_element(_inMsg + offset, gran);
		auto it = _outDict.find(cur_element);
		if (it != _outDict.end())
		{

		}
		offset += gran;

	}
}
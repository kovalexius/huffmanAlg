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
			std::multimap<uint64_t, std::unique_ptr<TreeNode>>& _outDict)
{
	for (auto item : _dict)
	{
		auto it = std::make_unique<TreeNode>();
		it->m_elem = std::make_unique<rtvdt::Element>(item.first);
		it->m_elemTitle = (std::string)item.first;
		_outDict.insert(std::make_pair(item.second, std::move(it)));
	}
}

// Строим дерево рекурсивно путем объединения двух узлов с наименьшей частотой появления
void buildTree(std::multimap<uint64_t, std::unique_ptr<TreeNode>>& _dict)
{
	while (_dict.size() > 1)
	{
		auto sz = _dict.size();
		auto node = std::make_unique<TreeNode>();
		auto leftIt = _dict.begin();
		auto rightIt = leftIt;
		++rightIt;

		//if(leftIt->second->m_elem)
		//	std::cout << __FUNCTION__ << "  left element: \'" << (std::string)*leftIt->second->m_elem << "\' ";
		//std::cout << __FUNCTION__ << "  left weight: \'" << leftIt->first << "\' " <<
		//	"  left title: \'" << leftIt->second->m_elemTitle << "\'  " << std::endl;
		//if (rightIt->second->m_elem)
		//	std::cout << __FUNCTION__ << "  right element: \'" << (std::string)*rightIt->second->m_elem << "\' ";
		//std::cout << __FUNCTION__ << "  right weight: \'" << rightIt->first << "\' " <<
		//	"  right title: \'" << rightIt->second->m_elemTitle << "\'  " << std::endl;
		

		node->m_left = std::move(leftIt->second);
		node->m_right = std::move(rightIt->second);
		node->m_elemTitle = node->m_left->m_elemTitle + node->m_right->m_elemTitle;
		auto weight = leftIt->first + rightIt->first;
		_dict.erase(leftIt);
		_dict.erase(rightIt);
		auto newNodeIt = _dict.insert(std::make_pair(weight, std::move(node)));

		//std::cout << __FUNCTION__ << " new node weight: \'" << newNodeIt->first << "\' " <<
		//	"title: \'" << newNodeIt->second->m_elemTitle << "\' " << std::endl;
		//std::cout << __FUNCTION__ << " left title: \'" << newNodeIt->second->m_left->m_elemTitle << "\' " << std::endl;
		//std::cout << __FUNCTION__ << " right title: \'" << newNodeIt->second->m_right->m_elemTitle << "\' " << std::endl;
		//std::cout << std::endl;
	}
}

// Построение кодовых цепочек для каждого элемента
void recursiveBypass(const TreeNode* _node,
	containers::vector_bb& _curCode,
	std::unordered_map<rtvdt::Element, containers::vector_bb, rtvdt::KeyHash>& _outDict)
{
	//std::cout << __FUNCTION__ << "  title: \'" << _node->m_elemTitle << "\'" << std::endl;

	if (_node->m_left)
	{
		_curCode.push_back(false);
		auto leftNode = _node->m_left.get();
		recursiveBypass(leftNode, _curCode, _outDict);
		_curCode.pop_back();
	}
	if (_node->m_right)
	{
		_curCode.push_back(true);
		auto rightNode = _node->m_right.get();
		recursiveBypass(rightNode, _curCode, _outDict);
		_curCode.pop_back();
	}

	//We must meet Element
	if (_node->m_elem)
	{
		std::cout << __FUNCTION__ << " _node->elem: \'" << (std::string)*_node->m_elem << 
			"\' curCode: \'" << (std::string)_curCode << "\'" << std::endl;
		_outDict.insert(std::make_pair(*_node->m_elem, _curCode));
	}
}


// Построение сжатого сообщения
void bypassTree(const TreeNode* _node,
	const char* _inMsg,
	const uint64_t _len,
	std::vector<char>& _outMsg,
	std::unordered_map<rtvdt::Element, containers::vector_bb, rtvdt::KeyHash>& _outDict)
{
	containers::vector_bb currentCode;
	auto currentNode = _node;
	recursiveBypass(currentNode, currentCode, _outDict);

	std::cout << "currentCode: \'" << (std::string)currentCode << "\'" << std::endl;
	for (const auto& item : _outDict)
	{
		std::cout << "Element: \'" << (std::string)item.first << "\' vector_bb: \'" << (std::string)item.second << "\'" << std::endl;
	}

	containers::vector_bb resultMsg;
	auto gran = _outDict.begin()->first.m_len;
	uint64_t offset = 0;
	while (offset < _len)
	{
		rtvdt::Element cur_element(_inMsg + offset, gran);
		auto it = _outDict.find(cur_element);
		if (it != _outDict.end())
		{
			resultMsg += it->second;
		}
		offset += gran;
	}
}
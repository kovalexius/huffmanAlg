#ifndef __ALG_TOOLS__H
#define __ALG_TOOLS__H


#include <string>
#include <unordered_map>
#include <map>

#include "vector_bb.h"
#include "rtv_data_types.h"


// Подсчет кол-ва повторений каждого элемента в сообщении
void countElements(const char* _inBuf, const uint64_t _len,		// Буфер и его длина
					const int _gran,							// Грануляция или длина элемента в байтах
					std::unordered_map<rtvdt::Element, uint64_t, rtvdt::KeyHash>& _outDict);


// Представляем каждый элемент в виде узла с указателями на правого и левого потомка и сортируем узлы 
// в порядке возрастания их частоты появления в исходном сообщении
void sortDict(const std::unordered_map<rtvdt::Element, uint64_t, rtvdt::KeyHash>& _dict,
				std::multimap<uint64_t, std::unique_ptr<TreeNode>>& _outDict);

// Строим дерево рекурсивно путем объединения двух узлов с наименьшей частотой появления
void buildTree(std::multimap<uint64_t, std::unique_ptr<TreeNode>>& _dict);

// Построение кодовых цепочек для каждого элемента
void recursiveBypass(const TreeNode* _node,
					containers::vector_bb& _curCode,
					std::unordered_map<rtvdt::Element, containers::vector_bb, rtvdt::KeyHash>& _outDict);

// Построение сжатого сообщения
void bypassTree(const TreeNode* _node,
				const char* _inMsg,
				const uint64_t _len,
				containers::vector_bb& _outMsg,
				std::unordered_map<rtvdt::Element, containers::vector_bb, rtvdt::KeyHash>& _outDict);


#endif
#include <string>
#include <unordered_map>
#include <map>

#include "vector_bb.h"
#include "element.h"


// ������� ���-�� ���������� ������� �������� � ���������
void countElements(const char* _inBuf, const uint64_t _len,		// ����� � ��� �����
					const int _gran,							// ���������� ��� ����� �������� � ������
					std::unordered_map<rtvdt::Element, uint64_t, rtvdt::KeyHash>& _outDict);


// ������������ ������ ������� � ���� ���� � ����������� �� ������� � ������ ������� � ��������� ���� 
// � ������� ����������� �� ������� ��������� � �������� ���������
void sortDict(const std::unordered_map<rtvdt::Element, uint64_t, rtvdt::KeyHash>& _dict,
				std::multimap<uint64_t, std::unique_ptr<treeNode>>& _outDict);

// ������ ������ ���������� ����� ����������� ���� ����� � ���������� �������� ���������
void buildTree(std::multimap<uint64_t, std::unique_ptr<treeNode>>& _dict);


void recursiveBypass(const treeNode* _node,
					containers::vector_bb& _curCode,
					std::unordered_map<rtvdt::Element, containers::vector_bb, rtvdt::KeyHash>& _outDict);

void bypassTree(const treeNode& _node,
				const char* _inMsg,
				const uint64_t _len,
				std::vector<char>& _outMsg,
				std::unordered_map<rtvdt::Element, containers::vector_bb, rtvdt::KeyHash>& _outDict);
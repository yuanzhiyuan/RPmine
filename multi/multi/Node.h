#ifndef GUARD_Node_h
#define GUARD_Node_h

//��������δ������ʹ�õĴ���
#include <unordered_map>
#include "FPtree.h"
#include <map>
class Node_;
class Node;
class FPtree;
//���Ǽ���������²��� ||   �﷨���� : ȱ�١�; ��(�ڡ�<����ǰ��)  || �Ĵ���
using std::unordered_map;

class Node_{
public:
	int id;
	unsigned count; //���нڵ��count��С�ڵ���support��
	Node* parent;
	bool mark;
	//set<Node>* children;
	//key:id val:Node ��map�����ڲ��Ҷ�Ӧid��children
	unordered_map<int, Node*>* children_map;
	void remove_children();
	//map <int, int> test;
	Node* next;
	Node_(int id, Node* parent);
	Node_(Node* next) :next(next), parent(0), children_map(new unordered_map<int,Node*>),mark(false){}
	~Node_();


};


class Node{
public:

	//Node(const Node& n);
	Node(int id, Node* parent);
	//Ϊ�˴���dummy
	Node(Node* next);
	~Node();
	bool check_mark() const{ return node->mark; }
	void set_mark(){ node->mark = true; }
	int getid() const{ return node->id; }
	unsigned getsup() const{ return node->count; }
	unordered_map<int, Node*>* getChildren(){ return node->children_map; }
	void add_count(unsigned added = 1){ node->count += added; }
	Node* get_next(){ return node->next; }
	void mod_next(Node* new_next){ node->next = new_next; }
	Node* get_parent(){ return node->parent; }
	void remove_children();
	void mark_children();
	void sub_anc(FPtree& tree);//�Ѹýڵ���������Ƚڵ������ȥm��mΪ�ýڵ�ļ���������rpmine
private:
	Node_* node;
};


#endif
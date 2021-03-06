#ifndef GUARD_Node_h
#define GUARD_Node_h

//否则会出现未声明就使用的错误
#include <unordered_map>
#include "FPtree.h"
#include <map>
class Node_;
class Node;
class FPtree;
//忘记加这个，导致产生 ||   语法错误 : 缺少“; ”(在“<”的前面)  || 的错误
using std::unordered_map;

class Node_{
public:
	int id;
	unsigned count; //树中节点的count（小于等于support）
	Node* parent;
	bool mark;
	//set<Node>* children;
	//key:id val:Node 的map，便于查找对应id的children
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
	//为了创建dummy
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
	void sub_anc(FPtree& tree);//把该节点的所有祖先节点计数减去m，m为该节点的计数，用于rpmine
private:
	Node_* node;
};


#endif
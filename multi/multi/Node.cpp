#include "Node.h"
//#include <unordered_map>
#include <iostream>
#include "FPtree.h"

using namespace std;
Node_::Node_(int id, Node* parent) : id(id), count(0), parent(parent), next(0), children_map(new unordered_map<int, Node*>), mark(false){
	

}

Node::Node(int id, Node* parent){
	node = new Node_(id, parent);
}
Node::Node(Node* next){
	node = new Node_(next);
}
Node_::~Node_(){
	//if (parent)	delete parent;
	if (children_map){
		//delete children_map;
	}
	//���ɾ��next�ڵ㣬FP-tree.cpp��87��do while��0��������ɾ��dummy�ᵼ�´���
	//if (next) delete next;
}
Node::~Node(){

	if (node) delete node;
}

void Node::remove_children(){
	if (!node->children_map) return;
	for (auto i = node->children_map->begin(); i != node->children_map->end(); i++){
		i->second->remove_children();
	}
	delete node->children_map;

	node->children_map = 0;
	delete node;
	node = 0;
}

void Node::mark_children(){
	//��Node��������ڵ㶼���
	//�����ǰNode����ǣ�����������ڵ㶼�Ѿ�����ǡ�
	
	if (check_mark())	return;
	node->mark = true;
	if (getChildren() == 0 || getChildren()->size() == 0)	return;
	for (auto each : *getChildren()){
		each.second->node->mark = true;
		each.second->mark_children();
	}
}

void Node::sub_anc(FPtree& tree){
	if (getid() == -1)	return;
	Node* now = get_parent();
	while (now->getid() != -1){
		now->node->count -= getsup();
		Node* now_parent = now->get_parent();
		if (now->node->count < 0){
			cout << "sub_anc������" << endl;
			return;
		}
		int cur_id = now->getid();
		if (now->node->count == 0){
			//����Ҫdelete������ڵ��ˣ������ȵð�link�����
			Node* node_link_cur = tree.get_id_head_map()->find(cur_id)->second->node_link;
			Node dummy(node_link_cur);
			node_link_cur = &dummy;
			while (node_link_cur->get_next() != now){
				node_link_cur = node_link_cur->get_next();
			}
			node_link_cur->mod_next(now->get_next());
			delete now;
			now = 0;
			//�ǵð����ĸ��ڵ��childrenmap���Ǹ�pairɾ��
			//ɾ��ĳ�����ڽڵ��ʱ���п������link������
		}
		now = now_parent;
		now->getChildren()->erase(now->getChildren()->find(cur_id));
	}
}
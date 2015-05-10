#include "Transation.h"
#include "FPtree.h"
#include <vector>

#include <set>
#include <iostream>
#include <algorithm>


using namespace std;


FPtree::~FPtree(){
	if (root)
	{
		root->remove_children();

		delete root;
	}
	root = 0;
	if (head){
		for (auto each : *head){
			delete each;
		}
		delete head;
		head = 0;
	}
	if (id_sup_map)	delete id_sup_map;
	id_sup_map = 0;
	if (id_head_map)	delete id_head_map;
	id_head_map = 0;
	if (id_index_map)	delete id_index_map;
	id_index_map = 0;

}

void FPtree::sort_head(){
	//ɾ��С����С֧�ֶȵ�head�ڵ�
	head->erase(remove_if(head->begin(), head->end(), [=](const head_node* n)->bool{return n->sup < get_min_sup(); }), head->end());
	//��head�ڵ�����
	sort(head->begin(), head->end(), [=](head_node* pnode1, head_node* pnode2)->bool{return pnode1->sup>pnode2->sup; });
	int index = 0;//����¼id_index_map��index
	for (head_node* pnode : *head){
		id_sup_map->insert(make_pair(pnode->id, pnode->sup));
		id_head_map->insert(make_pair(pnode->id, pnode));
		id_index_map->insert(make_pair(pnode->id, index));
		index++;
	}
}
void FPtree::sort_head(const vector<Transation>& trans){
	//����ÿ��item������һ�鵱ǰͷ������ҵ�һ���ģ���sup++�������½�һ��������ٶ�head����sort
	//���Ż���hash��heap
	vector<head_node*>* head = get_head();
	for (Transation t : trans){
		int* cur_items = t.get_items();
		for (unsigned i = 0; i < t.get_count(); i++){
			my_equal me;
			me.id = cur_items[i];
			vector<head_node*>::iterator j = find_if(head->begin(), head->end(), me);
			if (j != head->end())	(*j)->sup++;
			else{
				//head_node node{ cur_items[i], 0, 0 };
				head_node* pnode = new head_node(cur_items[i]);
				head->push_back(pnode);
			}
		}
	}
	//ɾ��С����С֧�ֶȵ�head�ڵ�
	head->erase(remove_if(head->begin(), head->end(), [=](const head_node* n)->bool{return n->sup < get_min_sup(); }), head->end());
	//��head�ڵ�����
	sort(head->begin(), head->end(), [=](head_node* pnode1, head_node* pnode2)->bool{return pnode1->sup>pnode2->sup; });
	int index = 0;//����¼id_index_map��index
	for (head_node* pnode : *head){
		id_sup_map->insert(make_pair(pnode->id, pnode->sup));
		id_head_map->insert(make_pair(pnode->id, pnode));
		id_index_map->insert(make_pair(pnode->id, index));
		index++;
	}

}

void FPtree::sort_trans(Transation& tran){
	int* items = tran.get_items();
	unsigned count = tran.get_count();
	unordered_map<int, unsigned>* map = get_map();
	unordered_map<int, int>* index_map = get_id_index_map();
	//��items����sup�Ľ������У����map��û�ҵ�id���ﲻ����С֧�ֶȣ����ʹ���һ��pair����Ĭ�Ϲ��캯��
	sort(&items[0], &(items[count]), [=](int a, int b)->bool{return (*index_map)[a] < (*index_map)[b]; });
	unsigned i = 0;
	//�ҵ�С����С֧�ֶȵĿ�ʼλ��
	for (; i < count && (*map)[items[i]] >= min_sup; i++);
	//ɾ��С����С֧�ֶȵ�id
	tran.mod_count(i);
}


//�����Ѿ��ź��������
void FPtree::processTrans(Transation& tran, Node* node){
	unsigned tran_count = tran.get_count();
	if (tran_count == 0)	return;
	int* items = tran.get_items();
	//��ǰ�ĸ��ڵ���ӽڵ�
	unordered_map<int, Node*>* children_map = node->getChildren();
	unordered_map<int, head_node*>* id_head_map = get_id_head_map();
	if (tran_count != 0){

		unordered_map<int, Node*>::iterator i = children_map->find(items[0]);
		if (i != children_map->end()){
			Node* inode = i->second;//node*
			inode->add_count();
			tran.go();
			processTrans(tran, inode);

		}
		else{

			Node* new_node = new Node(items[0], node);
			do{
				Node dummy(id_head_map->find(items[0])->second->node_link);
				if (dummy.get_next() == 0){
					id_head_map->find(items[0])->second->node_link = new_node;
					break;
				}
				Node* now = &dummy;
				while (now->get_next() != 0){
					now = now->get_next();
				}
				now->mod_next(new_node);
			} while (0);

			new_node->add_count();
			children_map->insert(make_pair(items[0], new_node));
			tran.go();
			processTrans(tran, new_node);

		}
	}

}

void FPtree::processTrans(Transation& tran, unsigned times, Node* node){
	unsigned tran_count = tran.get_count();
	if (tran_count == 0)	return;
	int* items = tran.get_items();
	//��ǰ�ĸ��ڵ���ӽڵ�
	unordered_map<int, Node*>* children_map = node->getChildren();
	unordered_map<int, head_node*>* id_head_map = get_id_head_map();
	if (tran_count != 0){
		if (id_head_map->find(items[0]) == id_head_map->end())	return;
		unordered_map<int, Node*>::iterator i = children_map->find(items[0]);
		if (i != children_map->end()){
			Node* inode = i->second;//node*
			inode->add_count(times);
			tran.go();
			processTrans(tran, times, inode);

		}
		else{

			Node* new_node = new Node(items[0], node);
			do{
				Node dummy(id_head_map->find(items[0])->second->node_link);
				if (dummy.get_next() == 0){
					id_head_map->find(items[0])->second->node_link = new_node;
					break;
				}
				Node* now = &dummy;
				while (now->get_next() != 0){
					now = now->get_next();
				}
				now->mod_next(new_node);
			} while (0);

			new_node->add_count(times);
			children_map->insert(make_pair(items[0], new_node));
			tran.go();
			processTrans(tran, times, new_node);

		}
	}

}

bool FPtree::isEmpty(){

	return root->getChildren() == 0 || root->getChildren()->size() == 0;
}


void FPtree::construction(const vector<Transation>& trans){
	if (trans.size() == 0)	return;
	sort_head(trans);

	for (Transation tran : trans){
		if (tran.get_count() == 0)	continue;
		sort_trans(tran);
		processTrans(tran, root);
	}
}


void FPtree::outputTree(){
	for (head_node* node : *head){
		Node dummy(node->node_link);
		Node* cur = &dummy;
		while (cur != nullptr){
			cout << " |id:| " << cur->getid() << " |count:| " << cur->getsup() << "----->" << " |mark| " << cur->check_mark();
			cur = cur->get_next();
		}
		cout << endl;
	}
}

bool FPtree::check_single(){
	Node* now = root;
	while (now != nullptr){
		if (now->getChildren()->size() > 1)	return false;
		if (now->getChildren() && now->getChildren()->size() == 0)	break;
		now = now->getChildren()->begin()->second;
	}
	return true;

}




//int main(){
//	FPtree tree(2);
//	int items1[] = { 1, 2, 3 };
//	Transation tran1(3, items1);
//	int items2[] = { 1, 2, 3 };
//	Transation tran2(3, items2);
//	int items3[] = { 1, 2, 4 };
//	Transation tran3(3, items3);
//	int items4[] = { 1, 2, 4 };
//	Transation tran4(3, items4);
//	int items5[] = { 1, 2};
//	Transation tran5(2, items5);
//	int items6[] = { 1, 3, 5 };
//	Transation tran6(3, items6);
//	int items7[] = { 1, 3, 5 };
//	Transation tran7(3, items7);
//	int items8[] = { 1, 2};
//	Transation tran8(2, items8);
//	int items9[] = { 1, 3, 4 };
//	Transation tran9(3, items9);
//	int items10[] = { 1, 3, 4 };
//	Transation tran10(3, items10);
//
//	vector<Transation> t;
//	t.push_back(tran1);
//	t.push_back(tran2);
//	t.push_back(tran3);
//	t.push_back(tran4);
//	t.push_back(tran5);
//	t.push_back(tran6);
//	t.push_back(tran7);
//	t.push_back(tran8);
//	t.push_back(tran9);
//	t.push_back(tran10);
//	tree.construction(t);
//	tree.outputTree();
//
//	system("pause");
//}
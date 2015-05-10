#include "rpmine.h"

using namespace std;

RPmine::RPmine(vector<Transation>& t,unsigned min_sup,vector<int>& cont):tree(min_sup),containing(cont){
	tree.construction(t);
	tree.outputTree();
}

RPmine::~RPmine(){

}


void RPmine::mark_node(){
	vector<head_node*>* head = tree.get_head();
	for (auto each_head : *head){
		int cur_id = each_head->id;
		//���ɨ�赽�Ľڵ㲻��containing�У������¸��ڵ�
		if (find(containing.begin(), containing.end(), cur_id) == containing.end())	continue;
		//����ڣ������
		Node* now = each_head->node_link;
		while (now != 0){
			
			if (!now->check_mark()){//û�б���ǣ�������Լ�����ڵ�ȫ�����
				now->mark_children();
			}
			
				//�������ǣ���������һ���ڵ�

			now = now->get_next();
		}
	}
}


void RPmine::mine(){
	vector<head_node*>* head = tree.get_head();
	for (auto ri = head->rbegin(); ri != head->rend(); ri++){
		int cur_id = (*ri)->id;

		if (find(containing.begin(), containing.end(), cur_id) == containing.end()){
			//i����containing��,���м�֦
			prune(*ri);
			
		}
		if ((*ri)->node_link != 0){
			//���������֦����ǰ��ڵ㻹�ڣ����ھ�
			mine(*ri);
		}
	}
}

void RPmine::prune(head_node* phn){
	Node* now = phn->node_link;
	while (now != 0){
		Node* now_next = now->get_next();//��ֹ��֦�ѵ�ǰ�ڵ�������Ҳ�����һ���ˣ������ȴ�����һ����ָ��
		//�п������link���ϵ�
		if (now->check_mark()){
			//�������ǣ��������¸��ڵ�
			now = now_next;
			continue;
		}
		else{
			//���û�б����
			//��ǰ�ڵ㣨conutΪm�����������ڵ�count��ȥm��Ȼ��ɾ����ǰ�ڵ����������
			prune(now);
			now = now_next;
		}
	}
}
void RPmine::prune(Node* pn){
	bool if_parent_del = pn->get_parent()->getsup() == pn->getsup();
	unsigned cur_count = pn->getsup();
	//�����ȼ�
	pn->sub_anc(tree);
	//������״��ڣ��Ѹ��׵�childrenmap���Ǹ�pairɾ��
	//�޷�ȷ������ɾ��ûɾ��ֻ���жϸ��׵�count�Ƿ��������count��������ɾ��
	if (!if_parent_del){
		//����û��ɾ
		pn->get_parent()->getChildren()->erase(pn->get_parent()->getChildren()->find(pn->getid()));
	}

	//�Ѻ���ɾ��
	pn->remove_children();
	delete pn;
	pn = 0;
}

void RPmine::mine(head_node* phn){

}



int main(){
	int items1[] = { 1, 2, 3 };
	Transation tran1(3, items1);
	int items2[] = { 1, 2, 3 };
	Transation tran2(3, items2);
	int items3[] = { 1, 2, 4 };
	Transation tran3(3, items3);
	int items4[] = { 1, 2, 4 };
	Transation tran4(3, items4);
	int items5[] = { 1, 2 };
	Transation tran5(2, items5);
	int items6[] = { 1, 3, 5 };
	Transation tran6(3, items6);
	int items7[] = { 1, 3, 5 };
	Transation tran7(3, items7);
	int items8[] = { 1, 2 };
	Transation tran8(2, items8);
	int items9[] = { 1, 3, 4 };
	Transation tran9(3, items9);
	int items10[] = { 1, 3, 4 };
	Transation tran10(3, items10);

	vector<Transation> t;
	t.push_back(tran1);
	t.push_back(tran2);
	t.push_back(tran3);
	t.push_back(tran4);
	t.push_back(tran5);
	t.push_back(tran6);
	t.push_back(tran7);
	t.push_back(tran8);
	t.push_back(tran9);
	t.push_back(tran10);
	vector<int> a = {  2, 4 };
	RPmine rpmine(t, 2, a);
	rpmine.mark_node();
	
	rpmine.output_tree();
	system("pause");
	return 0;
}
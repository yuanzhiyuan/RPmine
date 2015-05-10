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
		//如果扫描到的节点不在containing中，跳到下个节点
		if (find(containing.begin(), containing.end(), cur_id) == containing.end())	continue;
		//如果在，则遍历
		Node* now = each_head->node_link;
		while (now != 0){
			
			if (!now->check_mark()){//没有被标记，则把它以及子孙节点全部标记
				now->mark_children();
			}
			
				//如果被标记，则跳到下一个节点

			now = now->get_next();
		}
	}
}


void RPmine::mine(){
	vector<head_node*>* head = tree.get_head();
	for (auto ri = head->rbegin(); ri != head->rend(); ri++){
		int cur_id = (*ri)->id;

		if (find(containing.begin(), containing.end(), cur_id) == containing.end()){
			//i不在containing中,进行剪枝
			prune(*ri);
			
		}
		if ((*ri)->node_link != 0){
			//如果经过剪枝，当前层节点还在，就挖掘
			mine(*ri);
		}
	}
}

void RPmine::prune(head_node* phn){
	Node* now = phn->node_link;
	while (now != 0){
		Node* now_next = now->get_next();//防止剪枝把当前节点减掉，找不着下一个了，所以先存着下一个的指针
		//有可能造成link链断掉
		if (now->check_mark()){
			//如果被标记，则跳到下个节点
			now = now_next;
			continue;
		}
		else{
			//如果没有被标记
			//当前节点（conut为m）的所有祖宗的count减去m，然后删除当前节点和所有子孙
			prune(now);
			now = now_next;
		}
	}
}
void RPmine::prune(Node* pn){
	bool if_parent_del = pn->get_parent()->getsup() == pn->getsup();
	unsigned cur_count = pn->getsup();
	//对祖先减
	pn->sub_anc(tree);
	//如果父亲存在，把父亲的childrenmap的那个pair删掉
	//无法确定父亲删了没删，只能判断父亲的count是否等于它的count。等于则被删了
	if (!if_parent_del){
		//父亲没被删
		pn->get_parent()->getChildren()->erase(pn->get_parent()->getChildren()->find(pn->getid()));
	}

	//把孩子删除
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
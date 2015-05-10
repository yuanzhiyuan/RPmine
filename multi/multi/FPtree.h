#ifndef GUARD_FP_tree_h
#define GUARD_FP_tree_h

#include "Node.h"
#include "Transation.h"
#include <vector>
#include <set>

#include <algorithm>
#include <unordered_map>


using std::vector;	using std::set;	using std::unordered_map; 
class Node;
class Node_;
struct head_node{
	int id;
	unsigned sup;
	Node* node_link;
	void add_sup(unsigned added = 1){ sup += added; }
	~head_node(){ if (node_link) delete node_link; }
	head_node(int id, unsigned sup = 1) :id(id), sup(sup), node_link(0){}
	//����<������������FP-tree.cpp�У�sort_head��������������ǽ���
	bool operator<(head_node node){ return sup>node.sup; }
};


class FPtree{

public:
	FPtree(unsigned sup) :min_sup(sup), root(new Node(-1, nullptr)), head(new vector<head_node*>), id_sup_map(new unordered_map<int, unsigned>()), id_head_map(new unordered_map<int, head_node*>()), id_index_map(new unordered_map<int, int>()){}
	~FPtree();
	void construction(const vector<Transation>&);
	void sort_head(const vector<Transation>&);
	void sort_head();//this.head�Ѿ������ݣ�ֻ����������С֧�ֶȼ���������
	void sort_trans(Transation&);
	vector<head_node*>* get_head(){ return head; }
	unordered_map<int, unsigned>* get_map(){ return id_sup_map; }
	unordered_map<int, head_node*>* get_id_head_map(){ return id_head_map; }
	unordered_map<int, int>* get_id_index_map(){ return id_index_map; }
	unsigned get_min_sup() const{ return min_sup; }
	void set_min_sup(unsigned sup){ min_sup = sup; }
	void processTrans(Transation& tran, Node* node);
	void FPtree::processTrans(Transation& tran, unsigned times, Node* node);
	Node* get_root(){ return root; }
	void outputTree();
	void growth();
	//���������±�������ǲ��ǵ���·��
	bool check_single();
	bool isEmpty();

private:
	Node* root;//���ڵ��idΪ-1
	vector<head_node*>* head;
	//����������Ϣ�����ڲ���
	unordered_map<int, unsigned>* id_sup_map;
	unordered_map<int, head_node*>* id_head_map;
	//ͷ����id��˳���ӳ��
	//�Ƚ�index������ֱ�ӱȽ�sup��Ϊ�˱�֤��֧�ֶ���ͬ����������λ��Ҳ��ͬ
	unordered_map<int, int>* id_index_map;
	unsigned min_sup;



};

struct my_equal{
	int id;
	bool operator()(head_node* node){
		if (node->id == id)	return true;
		else return false;
	}
};


#endif
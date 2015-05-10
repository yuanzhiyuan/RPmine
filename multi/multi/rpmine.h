#ifndef GUARD_rpmine_h
#define GUARD_rpmine_h
#include "FPtree.h"

using std::vector;

class RPmine{
public:
	RPmine(vector<Transation>&,unsigned,vector<int>&);
	~RPmine();
	void mark_node();
	void output_tree(){ tree.outputTree(); }
	void mine();
	
private:
	void prune(head_node*);
	void prune(Node*);
	void mine(head_node*);
	
	FPtree tree;
	vector<vector<int>> result;
	vector<int> containing;
};


#endif

#ifndef GUARD_Transation_h
#define GUARD_Transation_h
class Transation{
public:
	Transation() :count(0), items(0){}
	Transation(unsigned count) :count(count){ items = new int[count]; }
	Transation(unsigned count, int* items) :count(count), items(items){}
	~Transation(){}
	unsigned get_count() const{ return count; }
	void mod_count(unsigned new_count){ count = new_count; }
	int* get_items() const{ return items; }
	//��items��ǰ�ƽ�
	void go();

private:
	unsigned count;//�ж��ٸ�item
	int* items;//������item��id����


};



#endif
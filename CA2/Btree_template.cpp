/*
*Copyright@Naveen
*6:24:49 PM
*Btree.cpp
*
*/
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "CA2.h"


using namespace std;
/* In BtreeNode class 
   We just store the key and we do not store value 
   if you want, you can modify the class definition of Btree Node.
*/

static int key_num = 0;
class BtreeNode
{
	pair<pair<int, int>, int> *a; // the first attribute refers to the id of each person while the second attribute refers to "block_idx"(the index of the block taht stores the tuple)
	BtreeNode **C; //pointer to a sequence of btree nodes
	int n; //number of keys in the node
	int t; //least number of keys in the node
	bool leaf;
public:
	BtreeNode(int t,bool leaf)
    {
		this->t = t;
		this->leaf = leaf;
		a = new pair<pair<int, int>, int>[2*t - 1]; //sequence of keys
		C = new BtreeNode*[2*t]; //sequence of children nodes
		n = 0; 
    }
	int findId(pair<int, int> k);
	void remove(int k);
	void removeFromLeaf(int id);
	void removeFromNonLeaf(int id);
	void borrowFromPrev(int id);
	void borrowFromSucc(int id);
	pair<pair<int, int>, int> findSucc(int id);
	pair<pair<int, int>, int> findPrec(int id);
	void merge(int id);
	void fill(int id);
	void traverse(ofstream& file);
	BtreeNode *search(int k);
	void insertNonFull(pair<pair<int, int>, int> key);
	void splitChild(int i,BtreeNode *y);
	bool smaller(pair<int, int> k, int idx){
		if(a[idx].first.second <= k.second){
			if(a[idx].first.first < k.first){
				return true;
			}
		}
		return false;
	}

friend class Btree;
};

class Btree
{
	BtreeNode* root;
	int t;
public:
	Btree(int t) //threshold (order/2)
    {
		root = new BtreeNode(t,true);
		this->t = t;
    }
	void insert(pair<pair<int,int>, int>);
	void remove(int key);
    int find_max();
    int find_min();
	pair<pair<int, int>, int> find_prec(pair<pair<int, int>, int> key);
	pair<pair<int, int>, int> find_succ(pair<pair<int, int>, int> key);
	void traverse()
	{
		if(root == NULL) return;
		else{
			cout << "Generating report for B-tree..." << endl;
			ofstream fout;
			string filename = "Report/B-tree.txt";
			fout.open(filename);
			cout << "Secondary keys are age_category, profession_category and risk level." << endl;
			fout << "Secondary keys are age_category, profession_category and risk level." << endl << endl;
			root->traverse(fout);

			fout << endl << "Key number is " << key_num << endl;
			fout.close();
			cout << "There are " << key_num << " people in the B-tree." << endl;
			cout << "Finished." << endl;

		}
	}
	BtreeNode *search(int k)
	{
		return (root==NULL) ? NULL : root->search(k);
	}

	static bool cmp(pair<pair<int, int>,int> key1, pair<pair<int, int>, int> key2)
	{
		if(key1.first.second <= key2.first.second){
			if(key1.first.first < key2.first.first){
				return true;
			}
		}
		return false;
	}

};

int BtreeNode :: findId(pair<int, int> k)
{
	int i;
	for(i = 0;i < n && smaller(k, i);i++);
	return i; //index of key in the key sequence or index of the minimum key that is larger than the given k
}

void BtreeNode :: remove(int k)
{
	
	// int id = findId(k);
	int id;
	for (id = 0; id < n && a[id].first.first != k; id++); // Find the node by id.

	if(id < n && a[id].first.first == k) //found
	{
		if(leaf)
			removeFromLeaf(id);
		else
			removeFromNonLeaf(id);
	}
	else // not found in this node
	{
		if(leaf) // not found in the whole tree
		{
		cout<<"Element does not exist"<<endl;
		return;
		}
		else //continue searching in the whole tree
		{
			bool flag;
			if(n == id) //given k is larger than all existed key
				flag = true;
			else
				flag = false;

			if(C[id]->t < t) //underflow
				fill(id);

			if(flag && id > n) //how can id achieve a value lrger than n?
				C[id-1]->remove(k);
			else
				C[id]->remove(k);
		}
	}
}

void BtreeNode::removeFromLeaf(int id)
{
	for(int i = id+1;i < n;i++)
	{
		a[i-1] = a[i];
	}
	n--;
}

void BtreeNode :: removeFromNonLeaf(int id)
{
	if(C[id]->t >= t)
	{
		a[id] = findPrec(id); 
		//replace the key with the largest key in the subtree rooted at ith child
		C[id]->remove(a[id].first.first);
	}
	else if(C[id+1]->t >= t)
	{
		a[id] = findSucc(id);
		//replace the key with the smallest key in the subtree rooted at (i+1)st child
		C[id+1]->remove(a[id].first.first);
	}
	else
	{
		int d = a[id].first.first;
		merge(id);
		C[id]->remove(d);
		//since we've merge the key itself with its two children (smaller and larger) into one node, we need to delete the key in the newly merged node
	}
}

void BtreeNode :: fill(int id)
{
	if(id!=0 && C[id-1]->n>=t)
		borrowFromPrev(id); 
		//move the largerst key from its previous node in the same level
	else if(id!=n && C[id+1]->n>=t)
		borrowFromSucc(id);
		//move the smallest key from its subsequent node in the same level
	else
	{
		//since either direct borrowing from siblings cause underflow, merge itself with one of the siblings to maintain the node property
		if(id!=n)
			merge(id);
		else
			merge(id-1);
	}

}

void BtreeNode :: borrowFromPrev(int id)
{
	BtreeNode *child = C[id];
	BtreeNode *sibling = C[id-1];

	for(int i = child->n-1;i >=0 ;i--)
		child->a[i+1] = child->a[i]; //right shift keys

	if(!child->leaf)
	{
		for(int i=child->n;i>=0;i++)
			child->C[i+1] = child->C[i]; //right shift children pointers
	}
	child->a[0] = a[id-1]; 
	if(!child->leaf)
		child->C[0] = sibling->C[sibling->n];
	a[id-1] = sibling->a[sibling->n-1];
	(child->n)++;
	(sibling->n)--;
}

void BtreeNode :: borrowFromSucc(int id)
{

	BtreeNode *child = C[id];
	BtreeNode *sibling = C[id+1];

	child->a[child->n] = a[id];
	a[id] = sibling->a[0];

	if(!child->leaf){
	child->C[(child->n)+1] = sibling->C[0];
	}

	for(int i = 1;i<sibling->n;i++ )
		sibling->a[i-1] = sibling->a[i];
	if(!child->leaf){
	    for(int i = 1;i<=sibling->n;i++ )
		    sibling->C[i-1] = sibling->C[i];
	}

	(sibling->n)--;
	(child->n)++;
}


void BtreeNode::merge(int id)
{
	BtreeNode *child = C[id];
	BtreeNode *sibling = C[id+1];

	child->a[child->n] = a[id]; 
	//merge its two child and itself into one node

	for(int i=0;i<sibling->n;i++)
		child->a[child->n+1+i] = sibling->a[i];

	if(!child->leaf)
	{	for(int i=0;i<=sibling->n;i++)
				child->C[child->n+1+i] = sibling->C[i];
	}

	for(int i=id+1;i<n;i++)
		a[i-1] = a[i]; //delete the key in the node
	for(int i=id+1;i<=n;i++)
		C[i-1]=C[i];
	child->n += sibling->n+1; //update key number in the children
	n--; //decrease key number in the node
	delete(sibling);
	return ;
}

void Btree :: remove(int k)
{
	if(!root)
	{
		cout<<"The tree is empty\n";
		return;
	}
	root->remove(k);
	if(root->n == 0) 
	{
		BtreeNode *t = root;
		//Case1: when only root in the tree 
		if(root->leaf)
			root = NULL;
		else
			root = root->C[0];
	    //Case2: when perform the last type of romveFromNonLeaf, i.e. merge the key with its both children
		//thus, no key in the root, and the root now hay only one child (the newly merged node)
		delete t;
	}
	return;
}

void BtreeNode::traverse(ofstream& fout)
{
	int i;
	for(i=0;i<n;i++)
	{
		if(leaf == false)
			C[i]->traverse(fout);
		fout << "Index value by calculating secondary keys: " << a[i].first.second << " \tid: " << a[i].first.first << " \tBlock index: " << a[i].second << endl;
		key_num++;
	}
	if(leaf == false)
		C[i]->traverse(fout);
}

BtreeNode * BtreeNode :: search(int k)
{	int i;
	for(i=0;i<n && a[i].first.first < k;i++);
	if(a[i].first.first == k)
		return this;

	if(leaf == true)
		return NULL;

	return C[i]->search(k);

}

void Btree ::insert(pair<pair<int, int>, int> key)
{
	if(root == NULL)
	{
		root = new BtreeNode(t,true);
		root->a[0].first = key.first;
		root->a[0].second = key.second;
		root->n = 1;
	}
	else
	{
		if(root->n == 2*t-1)
		{
			BtreeNode *p = new BtreeNode(t,false);
			p->C[0] = root;
			p->splitChild(0,p->C[0]);
			int i=0;
			if(p->a[0].first < key.first)
				i++;
			p->C[i]->insertNonFull(key);
			root = p;
		}
		else
			root->insertNonFull(key);
	}
}

void BtreeNode :: insertNonFull(pair<pair<int, int>, int> key)
{
 int k = key.first.first;
 // cout << k << endl;
 int i = n-1;
 if(leaf == true)
 {
	 while(i >=0 && k < a[i].first.first)
		 {
		 a[i+1] = a[i];
		 i--;
		 }
	// cout << key.first << endl;
	a[i+1] = key;
	n = n+1;
}
 else
 {
	 while(i >=0 && k <a[i].first.first)
		 i--;
	// k should be inserted to C[i+1]
	 if(C[i+1]->n == 2*t-1)
	 {
		 splitChild(i+1,C[i+1]);
		 // first parameter is the index of the child node to be splited, the seoncd index is the pointer to this child node
		 if(a[i+1].first.first < k)
			 i++; //  determine which splited child node should the key be inserted in
	 }
	 C[i+1]->insertNonFull(key);

 }
}

void BtreeNode::splitChild(int i, BtreeNode *y)
{
	BtreeNode *z = new BtreeNode(y->t,y->leaf);
	z->n = t-1;
	// node z stores the last t-1 keys
	for(int j=0;j<t-1;j++)
		z->a[j] = y->a[j+t];
	if(y->leaf == false)
	{
		for(int j=0;j<t;j++)
			z->C[j] = y->C[j+t];
	}
	y->n = t-1; // index of the last key is t-2
	for(int j=n;j>=i+1;j--)
		C[j+1] = C[j];
	C[i+1] = z;
	for(int j=n-1;j>=i;j--)
		a[j+1] = a[j];
	a[i]=y->a[t-1];
	n = n+1;
}

//find the max key stored in B-tree//
int Btree::find_max()
{
    //Implement your code here//
	BtreeNode *temp = root;
	while(!temp->leaf)
		temp = temp->C[temp->n];
    return temp->a[temp->n-1].first.first;
}

//find the min key stored in B-tree//
int Btree::find_min()
{
    //Implement your code here//
	BtreeNode *temp = root;
	while(!temp->leaf)
		temp = temp->C[0];
    return temp->a[0].first.first;
}

//find the predecessor of the given key
pair<pair<int, int>, int> BtreeNode :: findPrec(int id)
{
	// id represents the index of key in the key sequence
	//find the largest key that is smaller than the key to be removed
	BtreeNode *curr = C[id];
	while(!curr->leaf)
		curr = curr->C[curr->n];
	return curr->a[curr->n-1];
	
}

//find the successor of the given key
pair<pair<int, int>,int> BtreeNode :: findSucc(int id)
{
	// id represents the index of key in the key sequence
	//find the smallest key that is larger than the key to be removed
	BtreeNode *curr = C[id+1];
	while(!curr->leaf)
		curr = curr->C[0];
	return curr->a[0];
}

pair<pair<int, int>, int> Btree :: find_prec(pair<pair<int, int>, int> key){
	//First examine whether the given key is in the tree
	BtreeNode *node_key =search(key.first.first);
	if(node_key == NULL){
		cout <<"Key not found in the tree" << "\n" <<endl;
		pair<pair<int, int>, int> new_pair = make_pair(make_pair(0,0), 0);
		return new_pair;
	}

	//Case 1: if the given key is the smallest key in the tree, return the pair itself
	if(key.first.first == this->find_min()){	return key; }		

	int key_id = node_key->findId(key.first);

	//Case 2: if key is stored in an internal node
	if(node_key->leaf == false){	return node_key->findPrec(key_id); }

	//Case 3: if the given key is in a leaf node
	//Case 3.1: if it is not the first key stored in the node, just return its previous key
	if(key_id != 0){	return node_key->a[key_id - 1]; }
	//Case 3.2: if it is the first key in the node, trace upward to find the largest key that is smaller than the given key
	else{
		vector<pair<pair<int, int>,int>> smaller_keys;
		BtreeNode* temp = root;
		while(temp->leaf == false){
			int i = 0;
			for(; (i < temp->n) && (key.first > temp->a[i].first); i++);
			if(i != 0){	smaller_keys.push_back(temp->a[i-1]); }
			temp = temp->C[i];
		}
		sort(smaller_keys.begin(), smaller_keys.end(), cmp);
		// cout << "These possible keys are:" <<endl;
		// for(auto val:smaller_keys){	cout << val <<endl; }
		// cout << "\n" <<endl;
		return smaller_keys.back();
	}
}

pair<pair<int, int>, int> Btree :: find_succ(pair<pair<int, int>, int> key){
	//First examine whether the given key is in the tree
	BtreeNode *node_key =search(key.first.first);
	if(node_key == NULL){
		cout <<"Key not found in the tree" << "\n" <<endl;
		pair<pair<int, int>, int> new_pair = make_pair(make_pair(0, 0), 0);
		return new_pair;
	}

	//Case 1: if the given key is the largest key in the tree, return the key itself
	if(key.first.first == this->find_max()){	return key; }

	int key_id = node_key->findId(key.first);

	//Case 2: if key is stored in an internal node
	if(node_key->leaf == false){	return node_key->findSucc(key_id); }

	//Case 3: if the given key is in a leaf node
	//Case 3.1: if it is not the last key stored in the node, just return its subsequent key
	if(key_id != node_key->n-1){	return node_key->a[key_id + 1]; }
	//Case 3.2: if it is the last key in the node, trace upward to find the smallest key that is larger than the given key
	else{
		vector<pair<pair<int,int>, int>> larger_keys;
		BtreeNode* temp = root;
		while(temp->leaf == false){
			int i = 0;
			for(; (i < temp->n) && (key.first > temp->a[i].first); i++);
			if(i != temp->n){	larger_keys.push_back(temp->a[i]); }
			temp = temp->C[i];
		}
		sort(larger_keys.begin(), larger_keys.end(), cmp);
		// cout << "These possible keys are:" <<endl;
		// for(auto val:larger_keys){	cout << val <<endl; }
		// cout << "\n" <<endl;
		return larger_keys[0];
	}
}

// int main()
// {
// 	Btree t(3);
// 	t.insert(make_pair(1, 2));
// 	t.insert(make_pair(3, 1));
// 	t.insert(make_pair(7, 0));
// 	t.insert(make_pair(10, 2));
// 	t.insert(make_pair(11, 7));
// 	t.insert(make_pair(13, 2));
// 	t.insert(make_pair(14, 2));
// 	t.insert(make_pair(15, 4));
// 	t.insert(make_pair(18, 0));
// 	t.insert(make_pair(16, 4));
// 	t.insert(make_pair(19, 4));
// 	t.insert(make_pair(24, 3));
// 	t.insert(make_pair(25, 5));
// 	t.insert(make_pair(26, 0));
// 	t.insert(make_pair(4, 1));
// 	t.insert(make_pair(5, 6));
// 	t.insert(make_pair(20, 2));
// 	t.insert(make_pair(22, 7));
// 	t.insert(make_pair(2, 1));
// 	t.insert(make_pair(17, 7));
// 	t.insert(make_pair(12, 2));
// 	t.insert(make_pair(6, 4));
// 	//further insertion
// 	t.insert(make_pair(28, 5));
// 	t.insert(make_pair(31, 5));
// 	t.insert(make_pair(43, 5));
// 	t.insert(make_pair(44, 5));
// 	t.insert(make_pair(46, 5));
// 	t.insert(make_pair(52, 5));
// 	t.insert(make_pair(57, 5));
// 	t.insert(make_pair(61, 5));
// 	t.insert(make_pair(62, 5));
// 	t.insert(make_pair(64, 5));
// 	t.insert(make_pair(67, 5));
// 	t.insert(make_pair(73, 5));
// 	t.insert(make_pair(75, 5));
// 	t.insert(make_pair(79, 5));
// 	t.insert(make_pair(80, 5));
// 	t.insert(make_pair(81, 5));
// 	t.insert(make_pair(85, 5));
// 	t.insert(make_pair(88, 5));
// 	t.insert(make_pair(94, 5));
// 	t.insert(make_pair(96, 5));
// 	t.insert(make_pair(99, 5));
//     //write your test here//
// 	cout <<"The minimum key in the tree is: " << t.find_min() <<"\n" <<endl;
// 	cout <<"The maximum key in the tree is: " << t.find_max() <<"\n" <<endl;

// 	t.traverse();

// 	// cout <<"The predecessor of key 1 is: " << t.find_prec(1).first <<"\n" <<endl;
// 	// cout <<"The predecessor of key 16 is: " << t.find_prec(16).first <<"\n" <<endl;
// 	// cout <<"The predecessor of key 24 is: " << t.find_prec(24).first <<"\n" <<endl;
// 	// cout <<"The predecessor of key 67 is: " << t.find_prec(67).first <<"\n" <<endl;
// 	// cout <<"The predecessor of key 79 is: " << t.find_prec(79).first <<"\n" <<endl;
// 	// cout <<"The successor of key 15 is: " << t.find_succ(15).first <<"\n" <<endl;
// 	// cout <<"The successor of key 43 is: " << t.find_succ(43).first <<"\n" <<endl;
// 	// cout <<"The successor of key 88 is: " << t.find_succ(88).first <<"\n" <<endl;
// 	// cout <<"The successor of key 99 is: " << t.find_succ(99).first <<"\n" <<endl;

// 	//system("pause");
//     return 0;
// }

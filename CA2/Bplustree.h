#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_
//Author: Shashikant Kadam
//Roll number 16CSE1026
/*****B+ Tree*****/

//some ambiguities to be noticed: 1.same values are allowed to be stored together in the node.
//2.the order of the tree is MAX+1
//3.the final node pointer stored in each leaf node's ptr[] Will point to the next leaf node.

#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<climits>
using namespace std;
const int MAX = 4; //size of each node
class BPTree; //self explanatory classes
class Node
{
public:
	bool IS_LEAF;
	pair<int, int> *key; 
	int size;
	Node** ptr;
	friend class BPTree;
	Node();
	int find_id(int x);
};
class BPTree
{
	Node *root;
	void insertInternal(pair<int, int>,Node*,Node*);
	void removeInternal(int,Node*,Node*);
	Node* findParent(Node*,Node*);
public:
	BPTree();
	int search(int);
	void insert(pair<int, int> x);
	void remove(int);
	void display(Node*);
	Node* getRoot(void);
	void cleanUp(Node*);
	int find_max(Node* cursor);
	int find_min(Node* cursor);
	int find_pred(int x);
	int find_succ(int x);
	void display_only_leaf(Node* cursor);	
	// ~BPTree();
};
#endif

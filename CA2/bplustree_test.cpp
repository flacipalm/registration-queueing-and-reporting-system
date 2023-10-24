#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<climits>

#include "Bplustree.cpp"
//using namespace std;
using std::cout;
int main(){
    cout<<"we have set the order of the tree to be 5" <<endl<<endl;
    BPTree bpt;
    bpt.insert(make_pair(1, 2));
	bpt.insert(make_pair(3, 1));
	bpt.insert(make_pair(7, 0));
	bpt.insert(make_pair(10, 2));
	bpt.insert(make_pair(11, 7));
	bpt.insert(make_pair(13, 2));
	bpt.insert(make_pair(14, 2));
	bpt.insert(make_pair(15, 4));
	bpt.insert(make_pair(18, 0));
	bpt.insert(make_pair(16, 4));
	bpt.insert(make_pair(19, 4));
	bpt.insert(make_pair(24, 3));
	bpt.insert(make_pair(25, 5));
	bpt.insert(make_pair(26, 0));
	bpt.insert(make_pair(4, 1));
	bpt.insert(make_pair(5, 6));
	bpt.insert(make_pair(20, 2));
	bpt.insert(make_pair(22, 7));
	bpt.insert(make_pair(2, 1));
	bpt.insert(make_pair(17, 7));
	bpt.insert(make_pair(12, 2));
	bpt.insert(make_pair(6, 4));
	//further insertion
	bpt.insert(make_pair(28, 5));
	bpt.insert(make_pair(31, 5));
	bpt.insert(make_pair(43, 5));
	bpt.insert(make_pair(44, 5));
	bpt.insert(make_pair(46, 5));
	bpt.insert(make_pair(52, 5));
	bpt.insert(make_pair(57, 5));
	bpt.insert(make_pair(61, 5));
	bpt.insert(make_pair(62, 5));
	bpt.insert(make_pair(64, 5));
	bpt.insert(make_pair(67, 5));
	bpt.insert(make_pair(73, 5));
	bpt.insert(make_pair(75, 5));
	bpt.insert(make_pair(79, 5));
	bpt.insert(make_pair(80, 5));
	bpt.insert(make_pair(81, 5));
	bpt.insert(make_pair(85, 5));
	bpt.insert(make_pair(88, 5));
	bpt.insert(make_pair(94, 5));
	bpt.insert(make_pair(96, 5));
	bpt.insert(make_pair(99, 5));
    bpt.remove(17);
    cout << endl << endl;

    cout<<"now use the original display function, notice that it's a depth-first display for the whole tree, so it's a little tricky"<<endl;
    bpt.display(bpt.getRoot());
    cout<<endl<<endl;

    cout<<"now let's display all the keys in the leaf nodes"<<endl;
    bpt.display_only_leaf(bpt.getRoot());
    cout<<endl<<endl;

    cout<<"test find max(should display 723): "<<bpt.find_max(bpt.getRoot());
    cout<<endl<<endl;

    cout<<"test find min(should display 3): "<<bpt.find_min(bpt.getRoot());
    cout<<endl<<endl;

    cout<<"test find_pred"<<endl;
    cout<<"now find_pred(3), which should display 3: "<<bpt.find_pred(3)<<endl;
    cout<<"now find_pred(15), which should display 8: "<<bpt.find_pred(15)<<endl;
    cout<<"now find_pred(18), which should display 16: "<<bpt.find_pred(18)<<endl;
    cout<<"now find_pred(723), which should display 483: "<<bpt.find_pred(723)<<endl;
    cout<<"all border cases are covered"<<endl;
    cout<<endl<<endl;

    cout<<"test find_succ"<<endl;
    cout<<"now find_succ(3), which should display 4: "<<bpt.find_succ(3)<<endl;
    cout<<"now find_succ(15), which should display 16: "<<bpt.find_succ(15)<<endl;
    cout<<"now find_succ(27), which should display 28: "<<bpt.find_succ(27)<<endl;
    cout<<"now find_succ(723), which should display 723: "<<bpt.find_succ(723)<<endl;
    cout<<"all border cases are covered"<<endl;
    return 0;
}
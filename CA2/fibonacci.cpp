// Header file included from library
#include <vector>
#include <iostream>
// Header file written by LY and KD
#include "fifo.cpp"
using namespace std;
// Used in method "delete_node"
#define MINUS_IFINITY -1000000000






//int switch1;






/**
 * DESCRIPTION: 
 *  This function takes the original key (int, double, pair, etc.) as paramete, create a new fibonacci node, and directly insert it into the fibonacci heap
 * 
 */
void Fib_heap::insert(Person *value) 
{
    Fib_node *elem = new Fib_node(value);
    insert_node(elem);
    return;
}

/**
 * DISCRIPTION:
 *  This function simply return the minimal element, no modification to the heap
 *
template<class T> Fib_node<T> *Fib_heap<T>::minimal(void)
{
    return min;
}

/**
 * DISCIPTION:
 *  The function works as follows:
 *      If the heap is empty, just return NULL pointer
 *      Else 
 *          "cut" all the children of "min" and insert them into the rootlist
 *          if there is only one element in the heap, set min = NULL and numitems = 0.
 *          Else temporarily set min = min->right and consolidate the heap.
 *          return the min pointer.
 */
Fib_node *Fib_heap::extract_min(void)
{
    Fib_node *minimal = min;
    if (min != NULL) {

        // extern ofstream heapfile;
        // extern int switch1;
        // extern Fib_node *omin;
        // if (omin != NULL){
        //     if (min->value->identification_number == omin->value->identification_number){
        //         //extern ofstream heapfile;
        //         heapfile << "   OH NO! ";
        //     }
        // }
        
        //heapfile << "1 min's ID: " << min->value->identification_number << endl;
        // if the heap is not empty
        // Cut all the children of the minimal node
        // and insert them into the root list (helper function "cut")

        
        //switch1 = 0;
        Fib_node *leftmost = min->left_most_child;
        Fib_node *temp_min = min;
        while (min->left_most_child != NULL) {
            cut(min->left_most_child, min);
        }
        //switch1 = 0;
        //heapfile << endl << "2 min's ID: " << min->value->identification_number << endl;
        
        //switch1 = 1;

        min = temp_min;



        // Delete the "minimal" node from the root list
        Fib_node *deleted_node = delete_node_root(min);
        // DON'T PRINT THE HEAP HERE
        // THIS WILL CAUSE IFINITE LOOP!!!!!!
        if (deleted_node->right == deleted_node) {
            // The only element is deleted, so reset the heap into empty heap
            min = NULL;
            numitems = 0;
        }
        else {
            // Temporarily set the minimal to the right slibling
            // Fib_node *cur_min = min->right->right;
            // Fib_node *temp_min = min->right;

            


            // if (min->value->identification_number == min->right->value->identification_number){
            //     extern ofstream heapfile;
            //     heapfile << "OH NO!";
            // }


            // if (omin != NULL){
            //     if (min->value->identification_number == omin->value->identification_number){
            //         //extern ofstream heapfile;
            //         heapfile << "   OH NO! ";
            //     }
            // }
            

            min = min->right;


            // extern Fib_node *omin;
            // if (omin != NULL){
            //     if (min->value->identification_number == omin->value->identification_number){
            //         extern ofstream heapfile;
            //         heapfile << "OH NO!";
            //     }
            // }



            
            // extern int i;
            // if (cur_min->right != cur_min) 
            // {
            //     cout << i++ << endl;
            //     do {
            //         if (!cur_min->value->operator>(temp_min->value)) temp_min = cur_min;
            //         cur_min = cur_min->right;
            //     } while (cur_min != min->right->right);    
            // }
            // min = temp_min;
            // "consolidate" operation optimize the heap structure and restores the heap property
            consolidate();
            // Fib_node *cur_node = min;
            // Fib_node *temp_min;
            // Update the minimal
            // temp_min = cur_node;
            // for (; cur_node->right != min; cur_node = cur_node->right) {
            //     if (!(cur_node->value->operator>(temp_min->value))) temp_min = cur_node;
            // }
            // if (!(cur_node->value->operator>(temp_min->value))) temp_min = cur_node; 
            // min = temp_min;
        }
        numitems--;
    }
    return minimal;
}

/**
 * DESCRIPTION:
 *  This function merges the second fibonacci heap "H" with the original heap by concatenating the root list together. (THE "H" DOES NOT EXIST AFTER CALLING "mergeheap" !!!)
 */
void Fib_heap::mergeheap(Fib_heap *H)
{
    Fib_node *second_min = H->min;
    while (H->min != NULL) {
        // temp keep all the information about the H->min pointer
        Fib_node *temp = new Fib_node(NULL);
        temp->copy_node(H->min);
        // delete the node with the return value discarded
        H->delete_node_root(H->min);
        // If there are elements remaining in the rootlist of H, set the pointer to the right
        if (H->min != NULL) H->min = H->min->right;
        // insert temp (temp is no longer a local variable)
        insert_node(temp);
        
    }
    // update the min pointer to the min pointer of "H" if
    //     the original heap was empty
    //     the minimal of the second heap is smaller than the minimal of the firt heap
    if (min == NULL || (second_min != NULL && second_min->value < min->value)) min = second_min;
    return;
}

/**
 * DESCRIPTION:
 *  This function decrease the value of "node" to k and perform "cut" to the subheap rooted at "node"
 */
void Fib_heap::decrease_key(Fib_node *node, Person *k)
{
    if (k->operator>(node->value)) {
        cout << "new key is greater than the current key!" << endl;
        exit(0);
    }
    node->value = k;
    Fib_node *parent = node->parent;
    // If the node is not in the root list and the min-heap property is violated
    // cut the sub-tree rooted at node
    if (parent != NULL && (!node->value->operator>(parent->value)) ) {
        cut(node, parent);
        cascading_cut(parent);
    }
    // update min if necessary
    if (!node->value->operator>(min->value)) min = node;
}

void Fib_heap::adjust_priority(Fib_node *node)
{
    Fib_node *parent = node->parent;
    // If the node is not in the root list and the min-heap property is violated
    // cut the sub-tree rooted at node
    if (parent != NULL && (!node->value->operator>(parent->value)) ) {
        cut(node, parent);
        cascading_cut(parent);
    }
    // update min if necessary
    if (!node->value->operator>(min->value)) min = node;
}

/**
 * DESCRIPTION:
 *  This function delete the node "node" in the heap by setting it as the minimal element in the heap and perform extractmin
 */
void Fib_heap::deletenode(Fib_node *node)
{
    // Make the node to be deleted as the minimal element in the heap
    decrease_key(node, NULL);
    // Delete the root, the return value is discarded
    extract_min();
}

//===========================Helper function below============================

void Fib_heap::consolidate(void)
{
    



    // vector for concatenation
    vector<Fib_node*> con_array(10000, NULL);
    Fib_node *cur;
    // make a copy of the root list in a vector
    vector<Fib_node*> root_copy(0);
    for (cur = min; cur != NULL && cur->right != min; cur = cur->right) {
        root_copy.push_back(cur);
    }
    if (cur != NULL) root_copy.push_back(cur);
    // cout << root_copy.size() << endl;
    // Main procedure starts here
    for (int i = 0; i < root_copy.size(); i++) {
        cur = root_copy[i];
        int cur_degree = cur->degree;
        // print_heap();
        // If there is already a node in the array, combine them in proper order
        // and put the combined tree into next position (if empty), or continously combine
        // with next node until enter the first available position
        while (con_array[cur_degree] != NULL) {
            Fib_node *another_node = con_array[cur_degree];
            // make sure that cur->value < another->value
            if (cur->value->operator>(another_node->value)) {
                Fib_node *temp = cur;
                cur = another_node;
                another_node = temp;
            }
            delete_node_root(another_node);
            // avoid infinite loop with calling print_rootlist
            if (another_node == min) min = min->right;
            // Make the node with larger key value as the child of the node with smaller key value
            make_child(cur, another_node);
            // Clean the old position
            con_array[cur_degree] = NULL;
            // Explore to the next position to see if it is empty
            cur_degree++;
        }
        // When an empty position is found, directly insert the node into this position
        con_array[cur_degree] = cur;
    }
    // update the minimal
    Fib_node *cur_node = min;

    Fib_node *temp_min;
    // cur_node is null means the heap is empty
    if (cur_node == NULL) {
        min = NULL;
        return;
    }
    // Update the minimal
    temp_min = cur_node;



    for (; cur_node->right != min; cur_node = cur_node->right) {
        if (!(cur_node->value->operator>(temp_min->value))) temp_min = cur_node;
    }
    if (!(cur_node->value->operator>(temp_min->value))) temp_min = cur_node; 
    min = temp_min;


    return;
}

void Fib_heap::cut(Fib_node *node, Fib_node *parent)
{
    
    Fib_node *on;
    on = node;
    extern ofstream heapfile;




    // remove node from child list of parent
    delete_child(parent, node);
    // reinsert it to the rootlist



    insert_node(node);




    // if (node->value->identification_number != on->value->identification_number){
    //     heapfile << " child is changed   ";
    // }






    numitems--;
    node->parent = NULL;
    node->mark = false;
}

// A recursive function to cut the parent node (if marked)
void Fib_heap::cascading_cut(Fib_node *parent)
{
    Fib_node *grandparent = parent->parent;
    if (grandparent != NULL) {
        if (parent->mark == false) parent->mark = true;
        else {
            cut(parent, grandparent);
            cascading_cut(grandparent);
        }
    }
}

void Fib_heap::insert_node(Fib_node *elem)
{
    // NOTE: ONLY LEFT AND RIGHT POINTER OF ELEM IS RESET!!!

    // If the heap is empty
    if (min == NULL) {
        min = elem;
        elem->left = elem->right = elem;
        numitems = 1;
        return;
    }
    // Else, insert the newly-created node to the left of min
    elem->left = min->left;
    elem->right = min;
    // If the heap contains only one element, adjust min->right
    if (min->right == min && min->left == min) min->right = elem;
    // update the pointer field of min->left and min
    min->left->right = elem;
    min->left = elem;
    // update min if necessary



    if (!(elem->value->operator>(min->value))){
        // extern ofstream heapfile;
        // extern int switch1;
        // if (switch1 == 1){
            
        //     heapfile << "   min is changed ";
        //     heapfile << "   old min's ID: " << min->value->identification_number;
        //     heapfile << "   new min's ID: " << elem->value->identification_number;
        // }
        
        min = elem;
    } 
    numitems++;
    return;
}

Fib_node *Fib_heap::delete_node_root(Fib_node *elem)
{
    // NOTE: THIS FUNCTION ASSUMES THAT "elem" ALREADY EXISTS IN THE ROOT LIST
    //       NO PREVIOUS CHECKING IS PERFOEMED!!!

    // "elem" IS RETURNED WITHOUT MODIFICATION (ESPECIALLY ALL POINTER FIELDS)
    
    if (min == elem && min->right == min) {
        // The only node will be deleted
        min = NULL;
        numitems = 0;
        return elem;
    }

    // if (switch1 == 1){
    //     extern ofstream heapfile;
    //     heapfile << "   min deleted!  ";
    // }
    // switch1 = 0;
    


    // reset pointer fields
    elem->left->right = elem->right;
    elem->right->left = elem->left;
    return elem;
}

void Fib_heap::make_child(Fib_node *parent, Fib_node *elem)
{
    // If "parent" does not have a child
    if (parent->left_most_child == NULL) {
        parent->left_most_child = elem;
        elem->left = elem->right = elem;
        elem->parent = parent;
        elem->mark = false;
        parent->degree++;
        return;
    }
    // other cases
    Fib_node *right_bound = parent->left_most_child->left;
    elem->left = right_bound;
    elem->right = parent->left_most_child;
    right_bound->right = elem;
    parent->left_most_child->left = elem;
    
    elem->parent = parent;
    parent->degree++;
    elem->mark = false;
    return;
}

void Fib_heap::delete_child(Fib_node *parent, Fib_node *elem)
{
    if (elem->right == elem) {
        // the only child is deleted
        parent->left_most_child = NULL;
        parent->degree--;
        return;
    }
    if (elem == parent->left_most_child) {
        // The boundary element (leftmost child) is deleted
        elem->right->left = parent->left_most_child->left;
        parent->left_most_child = elem->right;
        parent->left_most_child->left->right = parent->left_most_child;
    }

    else {
        elem->left->right = elem->right;
        elem->right->left = elem->left;
    }
    parent->degree--;
}

void Fib_node::copy_node(Fib_node *old)
{
    // copy the node information of "old" to the newly created node
    value = old->value;
    degree = old->degree;
    left = old->left;
    left_most_child = old->left_most_child;
    mark = old->mark;
    parent = old->parent;
    right = old->right;
    return;
}

//=======================Functions for visualization ===============================
// void Fib_heap::print_rootlist(void)
// {
//     cout << "Now print the root list:" << endl;
//     Fib_node *cur;
//     for (cur = min; cur != NULL && cur->right != min; cur = cur->right) {
//         cout << cur->value->identification_number << "  ";
//     }
//     if (cur != NULL) cout << cur->value->identification_number << "  ";
//     cout << endl << endl;
// }

// void Fib_heap::print_children(Fib_node *parent)
// {
//     cout << "Now Print the children of node " << parent->value->identification_number << ":" << endl;
//     Fib_node *first = parent->left_most_child;
//     Fib_node *cur;
//     for (cur = first; cur != NULL && cur->right != first; cur = cur->right) {
//         cout << cur->value->identification_number << "  ";
//     }
//     if (cur != NULL) cout << cur->value->identification_number << "  ";
//     cout << endl << endl;
// }

// void Fib_heap::print_parent(Fib_node *child)
// {
//     cout << "Now print the parent of node " << child->value->identification_number << endl;
//     cout << child->parent->value->identification_number << endl;
// }

// // Use a fifo queue to print all the nodes in level order
// // NEED MODIFICATION HERE
// void Fib_heap::print_heap(void)
// {
//     cout << "Now print the whole Fibonacci heap:" << endl;
//     print_rootlist();
//     // use a fifo queue to print out all the elements in the fibonacci heap
//     fifo node_queue(10); 
//     if (min == NULL) {
//         cout << "The heap is empty!" << endl;
//         exit(0);
//     }
//     // first append all the root list into the queue
//     Fib_node *cur = min;
//     for (; cur->right != min; cur = cur->right) node_queue.pushback(cur->value);
//     node_queue.pushback(cur->value);
//     // loop until the queue is empty
//     while (!node_queue.isempty()) {
//         Person *cur_node = node_queue.popfront();
//         print_children(cur_node);
//         // add its children to the queue
//         Fib_node *first_child = cur_node->left_most_child;
//         if (first_child == NULL) continue;
//         else {
//             for (cur = first_child; cur->right != first_child; cur = cur->right) 
//                 node_queue.pushback(cur);
//             node_queue.pushback(cur);
//         }
//     }
//     return;
// }

// void Fib_heap::print_minimal(void)
// {
//     cout << "The minimal element in the heap is:" << endl;
//     cout << min->value << endl << endl;
// }h
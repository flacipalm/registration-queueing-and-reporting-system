#include "CA2.h"
#include "Overflow_block.cpp"

using namespace std;


int Block::delete_tuple(int id)
{
    // Deletion in overflow block
    // for (int i = 0; i < overflow.size; i++){    
    //     if (overflow.block_data[i].id == id){
    //         overflow.block_data.erase(overflow.block_data.begin() + i);
    //         return;
    //     } 
    // }
    if (overflow.del(id)) return 2;

    for (int j = 0; j < size; j++){
        // Deletion in block data
        if (block_data[j].id == id){
            block_data.erase(block_data.begin() + j); 
            block_data.push_back(0);
            return 1;         
        }
    }
    
    // printf("No such relation tuple with this id!!!");
    return 0;
}

void Block::merge(void)
{
    int block_size = size, overflow_size = overflow.size;
    int i = 0, j = 0;
    vector<Tuple> temp = vector<Tuple>(0);
    for (int i = 0; i < fix_size; i++) {
        Tuple *new_tuple = new Tuple();
        temp.push_back(*new_tuple);
    }
    int pos = 0;
    while (i < block_size && j < overflow_size)
    {
        if (block_data[i].id < overflow.block_data[j].id)
        {
            temp[pos++] = block_data[i++];
        }
        else {
            temp[pos++] = overflow.block_data[j++];
        }
    }
    while (i < block_size)
    {
        temp[pos++] = block_data[i++];
    }
    while (j < overflow_size)
    {
        temp[pos++] = overflow.block_data[j++];
    }
    block_data = temp;
    overflow.size = 0;
    size += overflow_size;
}

void Block::insert(Person *person) {
    Tuple new_tuple = Tuple(person);
    overflow.insert(new_tuple);
}
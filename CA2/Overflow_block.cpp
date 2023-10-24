#include "CA2.h"

using namespace std;

void Overflow_block::insert(Tuple relation)
{
    block_data[size++] = relation;
}

int Overflow_block::del(int id)
{
    for (int i = 0; i < block_data.size(); i++){
        if (block_data[i].id == id) {
            block_data.erase(block_data.begin() + i);
            size--;
            return 1;
        }
    }
    return 0;
}

void Overflow_block::sort_block(void)
{
    sort(block_data.begin(), block_data.end(), comparison_functor);         //sort from smaller id to bigger id
}
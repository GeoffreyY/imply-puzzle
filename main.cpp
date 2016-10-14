#include <iostream>
#include <vector>
#include <algorithm>

unsigned SIZE = 10;

//this iterates through and output the elements of any vector
template <typename T>
void printVector(std::vector<T> v) {
    for (auto i : v)
        std::cout << i << ' ';
    std::cout << std::endl;
    return;
}

//this is the imply operator
//can't call it "if" because that has been used for, well, "if statements"
bool imply(bool left, bool right) {
    if (left && !right)
        return false;
    return true;
}

//keeps track of blocks of elements that can be reduced
struct Block {
    unsigned start, end;
};

//given vector of n true-false elements in order
//and vector of n-1 int as the order to execute the imply operation
//return the resulting statement's value, true or false
//verbose option used to debug / check whether program's valid
bool applyLogic(std::vector<bool> values, std::vector<unsigned> order, bool verbose = false) {
    //debug option
    if (verbose)
        printVector(order);
    //exception handling
    if (values.size() != order.size() + 1) {
        std::cerr << "different value vector size and order vector size" << std::endl;
        return false;
    }
    static unsigned s = order.size();
    //this vector keeps track of which elements are "used" in an imply operation
    //TODO: actually this doesn't work. oops.
    std::vector<Block> positionDone;
    //iterate through the list of orders
    for (unsigned i = 0; i < s; i++) {
        //cache the element in order
        const unsigned pos = order[i];
        //execute the imply operator, and cache the result
        bool implyValue = imply(values[pos], values[pos + 1]);
        //debug option
        if (verbose)
            std::cout << pos << ' ' << values[pos] << "->" << values[pos+1] << " = " << implyValue << std::endl;
        //these remember that these elements are involved in an imply operation, and can be reduced
        bool foundFirst = false, foundSecond = false;
        //these remember the position of the blocks found
        int firstBlock, secondBlock;
        for (unsigned b = 0; b < positionDone.size(); b++) {
            //test if the block's start is our end
            if (positionDone[b].start == pos + 1) {
                foundSecond = true;
                secondBlock = b;
            }
            //test if the block's end is our start
            if (positionDone[b].end == pos) {
                foundFirst = true;
                firstBlock = b;
            }
        }
        if (foundFirst && foundSecond) {
            //if both blocks exist, join them
            positionDone[firstBlock].end = positionDone[secondBlock].end;
            positionDone.erase(positionDone.begin()+secondBlock);
        } else if (foundSecond) {
            //if only second block exists, extend its start
            positionDone[secondBlock].start--;
        } else if (foundFirst) {
            //if only first block exists, extend its end
            positionDone[firstBlock].end++;
        } else {
            //otherwise no valid block exists, create a new one
            Block newBlock;
            newBlock.start = pos;
            newBlock.end = pos+1;
            positionDone.push_back(newBlock);
        }
        //go through the continuous sections of elements that can be reduced
        for (Block b : positionDone) {
            //find the block that we belnogs to
            if (pos >= b.start || pos+1 <= b.end) {
                //and make the values and all elements in this block the same
                for (unsigned it = b.start; it <= b.end; it++) {
                    values[it] = implyValue;
                }
            }
        }
        //debug option
        if (verbose)
            printVector(values);
    }
    //return statement's value, as all elements should be reduced and have the same value
    return values[0];
}

int main(int argc, char *argv[]) {
    //command line option to set size of statement
    if (argc > 1) {
        SIZE = atoi(argv[1]);
        std::cout << "SIZE = " << argv[1] << std::endl;
    }
    //set initial value...
    std::vector<bool> values;
    //...of n/2 number of false...
    for (unsigned i = 0; i < SIZE / 2; i++)
        values.push_back(false);
    ///...and n/2 number of true
    for (unsigned i = 0; i < SIZE / 2; i++)
        values.push_back(true);
    //iterate through all permutations of values
    do {
        //modified version of the problem
        //throw away this permutation of valueif it ends with true
        if (values.back()) {
            //std::cout << "advance" << std::endl;
        } else {
        //set initial order of {1,2,3,...}
        std::vector<unsigned> order;
        for (unsigned i = 0; i < SIZE - 1; i++) {
            order.push_back(i);
        }
        //this helps reduceing running time
        bool secondPlayerWin = false;
        //iterate through all permutatios orders
        do {
            //check if the statement's value is false
            if (!applyLogic(values, order)) {
                //if it is false, second player found a strategy for this permutation of values!
                //debugging...
                printVector(order);
                secondPlayerWin = true;
                break;
            }
        } while (std::next_permutation(order.begin(),order.end()));
        //debugging...
        printVector(values);
        if (secondPlayerWin) {
            //log that second player wins
            std::cout << "second player wins this combination" << std::endl;
        } else {
            //logs that first player wins
            std::cout << "first player found the winning strategy!" << std::endl;
            //debugging...
            //do {applyLogic(values, order, true);} while (std::next_permutation(order.begin(),order.end()));
            break;
        }
        }
    } while (std::next_permutation(values.begin(),values.end()));
    return 0;
}
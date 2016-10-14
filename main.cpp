#include <iostream>
#include <vector>
#include <algorithm>

unsigned SIZE = 10;

template <typename T>
void printVector(std::vector<T> v) {
    for (auto i : v)
        std::cout << i << ' ';
    std::cout << '\n';
    return;
}

bool imply(bool left, bool right) {
    if (left && !right)
        return false;
    return true;
}

bool applyLogic(std::vector<bool> values, std::vector<unsigned> order, bool verbose = false) {
    if (verbose)
        printVector(order);
    if (values.size() != order.size() + 1) {
        std::cerr << "different value vector size and order vector size" << std::endl;
        return false;
    }
    static unsigned s = order.size();
    std::vector<bool> positionDone(s+1, false);
    for (unsigned i = 0; i < s; i++) {
        int pos = order[i];
        bool implyValue = imply(values[pos], values[pos + 1]);
        if (verbose)
            std::cout << pos << ' ' << values[pos] << "->" << values[pos+1] << " = " << implyValue << std::endl;
        positionDone[pos] = true;
        positionDone[pos+1] = true;

        unsigned iteratePos = pos;
        bool shouldBreak = false;
        while (!shouldBreak) {
            if (positionDone[iteratePos]) {
                values[iteratePos] = implyValue;
                if (iteratePos == 0)
                    shouldBreak = true;
                iteratePos--;
            } else {
                shouldBreak = true;
            }
        }
        shouldBreak = false;
        iteratePos = pos + 1;
        while (!shouldBreak) {
            if (positionDone[iteratePos]) {
                values[iteratePos] = implyValue;
                if (iteratePos >= s)
                    shouldBreak = true;
                iteratePos++;
            } else {
                shouldBreak = true;
            }
        }
        if (verbose)
            printVector(values);
    }
    return values[0];
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        SIZE = atoi(argv[1]);
        std::cout << "SIZE = " << argv[1] << std::endl;
    }
    std::vector<bool> values;
    for (unsigned i = 0; i < SIZE / 2; i++)
        values.push_back(false);
    for (unsigned i = 0; i < SIZE / 2; i++)
        values.push_back(true);

    do {
        std::vector<unsigned> order;
        for (unsigned i = 0; i < SIZE - 1; i++) {
            order.push_back(i);
        }
        if (values.back()) {
            std::cout << "advance" << std::endl;
        } else {
        bool secondPlayerWin = false;
        do {
            if (!applyLogic(values, order)) {
                printVector(order);
                secondPlayerWin = true;
                break;
            }
        } while (std::next_permutation(order.begin(),order.end()));
        printVector(values);
        if (secondPlayerWin) {
            std::cout << "second player wins this combination" << std::endl;
        } else {
            std::cout << "first player found the winning strategy!" << std::endl;
            //do {applyLogic(values, order, true);} while (std::next_permutation(order.begin(),order.end()));
            break;
        }
        }
    } while (std::next_permutation(values.begin(),values.end()));
    return 0;
}
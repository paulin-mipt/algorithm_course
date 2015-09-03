#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

int main() {
    std::vector< std::pair<size_t, size_t> > values;
    size_t valueNum;
    std::cin >> valueNum;

    values.resize(valueNum);
    for (size_t i = 0; i < valueNum; ++i) {
        size_t value;
        std::cin >> value;
        values[i] = std::make_pair(value, i);
    }

    std::sort(values.begin(), values.end());

    size_t queryNum;
    std::cin >> queryNum;
    for (size_t i = 0; i < queryNum; ++i) {
        size_t leftBound, rightBound, k;
        std::cin >> leftBound >> rightBound >> k;
        size_t usedValues = 0, j = 0;
        while (usedValues < k) {
            if (values[j].second >= leftBound && values[j].second <= rightBound) {
                ++usedValues;
            }
            ++j;
        }
        std::cout << values[j - 1].first << std::endl;
    }
    return 0;
}

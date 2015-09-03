#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

struct vertex {
	vertex* left;
	vertex* right;
	size_t value;
	size_t references;

	vertex(size_t pvalue) : left(NULL), right(NULL), value(pvalue), references(0) {}

    //build vertex from children
	vertex(vertex* pleft, vertex* pright) : left(pleft), right(pright) {
        references = 0;
		value = left->value + right->value;
		if (left) {
            left->references++;
		}
		if (right) {
            right->references++;
		}
	}

	~vertex() {
        if (left) {
            if (left->references > 1) {
                left->references--;
            } else {
                delete left;
            }
		}
		if (right) {
           if (right->references > 1) {
                right->references--;
            } else {
                delete right;
            }
		}
	}
};

vertex* build(size_t fromV, size_t toV) {
	if (fromV == toV) {
		return new vertex(0);
	} else {
		size_t mediumV = (fromV + toV) / 2;
		return new vertex(build(fromV, mediumV), build(mediumV + 1, toV));
	}
}

size_t get(vertex* tree, size_t fromV, size_t toV, size_t leftBound, size_t rightBound) {
	if (fromV > toV) {
		return 0;
	}

	if ((fromV == leftBound) && (toV == rightBound)) {
		return tree->value;
	}

	size_t mediumV = (leftBound + rightBound) / 2;
	size_t result = get(tree->left, fromV, std::min(mediumV, toV), leftBound, mediumV) +
        get(tree->right, std::max(mediumV + 1, fromV), toV, mediumV + 1, rightBound);
    return result;
}

vertex* update(vertex* tree, size_t fromV, size_t toV, size_t newValue, size_t index) {
	if (fromV == toV) {
		return new vertex(newValue);
	}

	size_t mediumV = (fromV + toV) / 2;
	if (index <= mediumV) {
		return new vertex(update(tree->left, fromV, mediumV, newValue, index), tree->right);
	} else {
		return new vertex(tree->left, update(tree->right, mediumV + 1, toV, newValue, index));
	}
}

void init(std::vector< std::pair<size_t, size_t> >& values, std::vector<vertex*>& roots) {
	size_t valueNum;
    std::cin >> valueNum;
    values.resize(valueNum);
    for (size_t i = 0; i < valueNum; ++i) {
        size_t value;
        std::cin >> value;
        values[i] = std::make_pair(value, i);
    }

    std::sort(values.begin(), values.end());

	roots.push_back(build(0, valueNum - 1));
    for (size_t i = 0; i < valueNum; ++i) {
        roots.push_back(update(roots[i], 0, valueNum - 1, 1, values[i].second));
    }
}

size_t binSearch(std::vector<vertex*>& roots, size_t leftArray, size_t rightArray, size_t k) {
    size_t leftBound = 0, rightBound = roots.size() - 1;
    while (leftBound != rightBound - 1) {
        size_t midBound = (leftBound + rightBound) / 2;
        size_t leastOnSegment = get(roots[midBound], leftArray, rightArray, 0, roots.size() - 2);
        if (leastOnSegment < k) {
            leftBound = midBound;
        } else {
            rightBound = midBound;
        }
    }
    return rightBound;
}

size_t solve(size_t k, std::vector< std::pair<size_t, size_t> >& values, std::vector<vertex*>& roots,
                                                    size_t leftBound, size_t rightBound) {
    size_t serialNum = binSearch(roots, leftBound, rightBound, k);
    return values[serialNum - 1].first;
}

int main() {
    std::vector< std::pair<size_t, size_t> > values;
    std::vector<vertex*> roots;
    init(values, roots);

    size_t queryNum;
    std::cin >> queryNum;
    for (size_t i = 0; i < queryNum; ++i) {
        size_t leftBound, rightBound, k;
        std::cin >> leftBound >> rightBound >> k;
        std::cout << solve(k, values, roots, leftBound, rightBound) << std::endl;
    }

    for (size_t i = 0; i <= values.size(); ++i) {
        delete roots[i];
    }
    return 0;
}

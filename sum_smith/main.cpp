#include <iostream>
#include <vector>
#include <climits>
#include <queue>

const int INF = INT_MAX;

struct vertex {
    size_t number;
    std::vector<size_t> children;
    std::vector<size_t> parents;
};

bool hasLabeledChild(vertex vert, const std::vector<int>& grundy, int label) {
    for (size_t j = 0; j < vert.children.size(); ++j) {
        if (grundy[vert.children[j]] == label) {
            return true;
        }
    }
    return false;
}

//generalized Grundy function
void calcGrundy(std::vector<int>& result, const std::vector<vertex>& graph) {
    std::queue<size_t> processing;
    size_t vertNum = graph.size();
    result.resize(vertNum, INF);
    std::vector<bool> inQueue(vertNum, false);
    //find and push lists
    for (size_t i = 0; i < vertNum; ++i) {
        if (graph[i].children.size() == 0) {
            processing.push(i);
            inQueue[i] = true;
        }
    }

    size_t notChanged = 0;
    while (!processing.empty() && notChanged < processing.size()) {
        vertex currVert = graph[processing.front()];
        processing.pop();
        //finding mex of the nim-equivalent children
        std::vector<bool> nimChildren(vertNum, false);
        for (size_t i = 0; i < currVert.children.size(); ++i) {
            int childValue = result[currVert.children[i]];
            if (childValue != INF) {
                nimChildren[childValue] = true;
            }
        }
        size_t grundyValue = 0;
        while (nimChildren[grundyValue]) {
            grundyValue++;
        }

        size_t i = 0;
        bool appropriate = true; //it means that the not-nim-equivalent vertex
                                 //has a child with the label "grundyValue"
        while (i < currVert.children.size() && appropriate) {
            int childValue = result[currVert.children[i]];
            if (childValue == INF) {
                appropriate = false;
                size_t childNum = currVert.children[i];
                if (hasLabeledChild(graph[childNum], result, grundyValue)) {
                    appropriate = true;
                }
            }
            ++i;
        }

        for (size_t i = 0; i < currVert.parents.size(); ++i) {
            int parentValue = result[currVert.parents[i]];
            if (parentValue == INF && !inQueue[currVert.parents[i]]) {
                processing.push(currVert.parents[i]);
                inQueue[currVert.parents[i]] = true;
            }
        }

        if (appropriate) {
            result[currVert.number] = grundyValue;
            inQueue[currVert.number] = false;
            notChanged = 0;
        } else {
            notChanged++;
            processing.push(currVert.number);
        }
    }
}

void scanAndCalculate(std::vector<int>& grundy, std::vector<vertex>& graph) {
    size_t vertNum, edgeNum;
    std::cin >> vertNum >> edgeNum;
    graph.resize(vertNum);
    grundy.resize(vertNum, INF);

    for (size_t i = 0; i < vertNum; ++i) {
        graph[i].number = i;
    }
    for (size_t i = 0; i < edgeNum; ++i) {
        size_t x, y;
        std::cin >> x >> y;
        graph[x - 1].children.push_back(y - 1);
        graph[y - 1].parents.push_back(x - 1);
    }

    calcGrundy(grundy, graph);
}

void solve(const std::vector<int>& firstGrundy, const std::vector<int>& secondGrundy,
            const std::vector<vertex>& firstGraph, const std::vector<vertex>& secondGraph) {
    size_t queries;
    std::cin >> queries;
    for (size_t i = 0; i < queries; ++i) {
        size_t firstPos, secondPos;
        std::cin >> firstPos >> secondPos;
        firstPos--;
        secondPos--;
        //std::cout << firstPos << firstGraph[firstPos] << std::endl;
        //std::cout << secondPos << secondGraph[secondPos] << std::endl;
        if (firstGrundy[firstPos] != INF && secondGrundy[secondPos] != INF) {
            size_t grundy = firstGrundy[firstPos] ^ secondGrundy[secondPos];
            if (grundy == 0) {
                std::cout << "second" << std::endl;
            } else {
                std::cout << "first" << std::endl;
            }
        } else {
            if (firstGrundy[firstPos] == INF && secondGrundy[secondPos] == INF) {
                std::cout << "draw" << std::endl;
            } else {
                if (firstGrundy[firstPos] != INF) {
                    if (hasLabeledChild(secondGraph[secondPos], secondGrundy, firstGrundy[firstPos])) {
                        std::cout << "first" << std::endl;
                    } else {
                        std::cout << "draw" << std::endl;
                    }
                } else {
                    if (hasLabeledChild(firstGraph[firstPos], firstGrundy, secondGrundy[secondPos])) {
                        std::cout << "first" << std::endl;
                    } else {
                        std::cout << "draw" << std::endl;
                    }
                }
            }
        }
    }
}

int main() {
    std::vector<int> grundyFirst, grundySecond;
    std::vector<vertex> graphFirst, graphSecond;
    scanAndCalculate(grundyFirst, graphFirst);
    scanAndCalculate(grundySecond, graphSecond);

    solve(grundyFirst, grundySecond, graphFirst, graphSecond);
    return 0;
}

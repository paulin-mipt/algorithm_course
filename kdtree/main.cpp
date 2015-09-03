#include <cstdio>
#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <vector>

#define node_vector std::vector< kdTreeNode<coordType> >
#define node_iterator class std::vector< kdTreeNode<coordType> >::iterator
#define sqr(x) (x)*(x)

template<typename coordType>
class kdTreeNode {
    std::vector<coordType> coords;
public:
    size_t number;
    kdTreeNode() {}
    kdTreeNode(size_t dimension) {
        coords.resize(dimension);
    }

    void setCoord(size_t index, coordType value) {
        coords[index] = value;
    }

    coordType getCoord(size_t index) const {
        return coords[index];
    }

    size_t getDimension() const {
        return coords.size();
    }
	node_iterator left, right;
};

//squared dist between a and b
template<typename coordType>
double dist(const kdTreeNode<coordType>& a, const kdTreeNode<coordType>& b) {
	double answer = 0;
	for (size_t i = 0; i < a.getDimension(); i++) {
		answer += sqr(a.getCoord(i)- b.getCoord(i));
	}
	return answer;
}

template<typename coordType>
class kdTree {
    node_vector nodes;
public:
    node_iterator root;

    kdTree(node_vector& _tree) {
        nodes = _tree;
        root = buildTree(nodes.begin(), nodes.size(), 0);
    }

    //builds the tree over the initially unordered vector
    //'t' stores iterator for the beginning of the segment of the vector of nodes to order into a tree
    //'segmSize' stores the size of this segment, ordering is being done by the 'axisInd' axis coordinate
    node_iterator buildTree(node_iterator t, size_t segmSize, size_t axisInd) {
        if (segmSize < 1) {
            return nodes.end();
        }
        size_t dimension = t->getDimension();
        std::nth_element(t, t + segmSize / 2, t + segmSize,
            [axisInd](const kdTreeNode<coordType>& a, const kdTreeNode<coordType>& b){
                return a.getCoord(axisInd) < b.getCoord(axisInd);
            });
        node_iterator n = t + segmSize / 2;

        size_t nextAxisInd = (axisInd + 1) % dimension;
        n->left  = buildTree(t, n - t, nextAxisInd);
        n->right = buildTree(n + 1, t + segmSize - (n + 1), nextAxisInd);

        return n;
    }

    //when the function ends working, answer is stored in 'bestNode' and 'bestFoundDist'
    void nearest(const node_iterator root, const kdTreeNode<coordType>& queryNode, size_t axisInd,
                            kdTreeNode<coordType>& bestNode, double& bestFoundDist) const {
        size_t dimension = root->getDimension();
        if (root == nodes.end()) {
            return;
        }
        double distToRoot = dist(*root, queryNode);

        if (root == this->root || distToRoot < bestFoundDist) {
            bestFoundDist = distToRoot;
            bestNode = *root;
        }

        //searching where more probable
        size_t nextAxisInd = (axisInd + 1) % dimension;
        if (root->getCoord(axisInd) > queryNode.getCoord(axisInd)) {
            nearest(root->left, queryNode, nextAxisInd, bestNode, bestFoundDist);
        } else {
            nearest(root->right, queryNode, nextAxisInd, bestNode, bestFoundDist);
        }

        //if there is no way to find something here - return
        if (sqr(root->getCoord(axisInd) - queryNode.getCoord(axisInd)) >= bestFoundDist) {
            return;
        }
        if (root->getCoord(axisInd) > queryNode.getCoord(axisInd)) {
            nearest(root->right, queryNode, nextAxisInd, bestNode, bestFoundDist);
        } else {
            nearest(root->left, queryNode, nextAxisInd, bestNode, bestFoundDist);
        }
    }
};

template<typename coordType>
void stupidSolver(const node_vector& nodes, const node_vector& queries,
                                std::vector<double>& answers) {
    for (size_t i = 0; i < queries.size(); i++) {
        double ans = INFINITY;
        for (size_t j = 0; j < nodes.size(); j++) {
            ans = std::min(ans, dist(queries[i], nodes[j]));
        }
        answers.push_back(ans);
    }
}

template<typename coordType>
void kdTreeSolver(const kdTree<double>& tree, const node_vector& nodes,
                    const node_vector& queries, std::vector<double>& answers) {
    for (size_t i = 0; i < queries.size(); i++) {
        double ans = INFINITY;
        kdTreeNode<double> found;
        tree.nearest(tree.root, queries[i], 0, found, ans);
        answers.push_back(ans);
    }
}

template<typename coordType>
void generateData(size_t dimension, size_t pointsNum, node_vector& data) {
    for (size_t i = 0; i < pointsNum; i++) {
        for (size_t i = 0; i < pointsNum; i++) {
            kdTreeNode<coordType> newNode(dimension);
            for (size_t j = 0; j < dimension; j++) {
                newNode.setCoord(j, rand()/(rand() + 1));
            }
            newNode.number = i;
            data.push_back(newNode);
        }
    }
}

template<typename coordType>
void readData(node_vector& data) {
    size_t pointsNum, dimension;
    std::cin >> pointsNum >> dimension;
    for (size_t i = 0; i < pointsNum; i++) {
        kdTreeNode<coordType> newNode(dimension);
        for (size_t j = 0; j < dimension; j++) {
            coordType coord;
            std::cin >> coord;
            newNode.setCoord(j, coord);
        }
        newNode.number = i;
        data.push_back(newNode);
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    std::vector<kdTreeNode<double> > nodes, queries;

    //readData(nodes);
    //readData(queries);
    generateData(10, 100, nodes);
    generateData(10, 1000, queries);

    kdTree<double> tree(nodes);
    std::vector<double> answersStupid, answersKdTree;
    clock_t time1 = clock();
    stupidSolver(nodes, queries, answersStupid);
    clock_t time2 = clock();

    clock_t time3 = clock();
    kdTreeSolver(tree, nodes, queries, answersKdTree);
    clock_t time4 = clock();
    //std::cout << answersStupid.size() << answersKdTree.size();
    for (size_t i = 0; i < answersStupid.size(); i++) {
        if (abs(answersStupid[i] - answersKdTree[i]) > 1e-5) {
            std::cout << "FALSE!" << std::endl;
        } //else {
          //  std::cout << "Correct" << std::endl;
          //}
    }

    std::cout << "Stupid algo worked " << time2 - time1 << " ticks" << std::endl;
    std::cout << "kdtree algo worked " << time4 - time3 << " ticks" << std::endl;
	return 0;
}

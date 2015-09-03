#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include<boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

const size_t ALPHSIZE = 'z' - 'a' + 1;

class vertex;
size_t getLink(size_t vertexNum, std::vector<vertex>& forest);

class vertex {
public:
    std::vector<int> successors;
    std::vector<int> go;
    std::vector<size_t> substrNum;
    int parent;
    size_t number;
    char parentChar;
    bool isLeaf;
    int link;
    int nearest;
    //removed redundant initialization.
    //It was redundant because was used only in init of root vertex
    //and redetermined for all other => O(N) extra actions performed
    vertex(size_t alphSize) {
        successors.resize(alphSize, -1);
        go.resize(alphSize, -1);
        link = -1;
        nearest = -1;
    }
};

int getNearestLeaf(size_t number, std::vector<vertex>& forest) {
    if (forest[number].nearest >= 0)
        return forest[number].nearest;

    if (forest[number].isLeaf) {
        return number;
    }

    if (!number) {
        forest[number].nearest = 0;
        return 0;
    }

    return getNearestLeaf(forest[getLink(number, forest)].number, forest);
}

size_t go(size_t v, char c, std::vector<vertex>& forest);

size_t getLink(size_t vertexNum, std::vector<vertex>& forest) {
	if (forest[vertexNum].link == -1) {
		if (vertexNum == 0 || forest[vertexNum].parent == 0) {
			forest[vertexNum].link = 0;
        }
		else {
			forest[vertexNum].link = go(getLink(forest[vertexNum].parent, forest),
                                        forest[vertexNum].parentChar, forest);
        }
    }
	return forest[vertexNum].link;
}

size_t go(size_t vertIndex, char c, std::vector<vertex>& forest) {
	if (forest[vertIndex].go[c] == -1) {
		if (forest[vertIndex].successors[c] != -1) {
			forest[vertIndex].go[c] = forest[vertIndex].successors[c];
        }
		else {
            if (vertIndex == 0) {
                forest[vertIndex].go[c] = 0;
            }
            else {
                forest[vertIndex].go[c] = go(getLink(vertIndex, forest), c, forest);
            }
        }
    }
	return forest[vertIndex].go[c];
}

void addString(const std::string& str, std::vector<vertex>& forest,
                                            size_t stringIndex) {
	size_t currVert = 0;
	for (size_t i = 0; i < str.length(); ++i) {
		char c = str[i] - 'a';
		if (forest[currVert].successors[c] == -1) {
            vertex newVert(ALPHSIZE);
            size_t newVertNum = forest.size();
			newVert.parent = currVert;
			newVert.parentChar = c;
			newVert.isLeaf = false;
			newVert.number = newVertNum;
			forest.push_back(newVert);
			forest[currVert].successors[c] = newVertNum;
		}
		currVert = forest[currVert].successors[c];
	}
	forest[currVert].isLeaf = true;
	forest[currVert].substrNum.push_back(stringIndex);
}

void goThroughForest(const std::string& text, std::vector<vertex>& forest,
                        const std::vector<size_t>& substrEndIndex, const size_t substrNum) {
    //go through forest with a text string
    std::vector<size_t> answer(text.length(), 0);
    size_t vertIndex = 0;
    for (size_t i = 0; i < text.length(); ++i) {
        vertIndex = go(vertIndex, text[i] - 'a', forest);
        size_t nextIndex = vertIndex;
        while (nextIndex > 0) {
            for (size_t j = 0; j < forest[nextIndex].substrNum.size(); ++j) {
                size_t substrFound = forest[nextIndex].substrNum[j];
                if (i >= substrEndIndex[substrFound]) {
                    answer[i - substrEndIndex[substrFound]]++;
                }
            }
            if (forest[nextIndex].isLeaf) {
                nextIndex = getNearestLeaf(getLink(nextIndex, forest), forest);
            }
            else {
                nextIndex = getNearestLeaf(nextIndex, forest);
            }
        }
    }

    //output answers. Position is an answer only if substrNum substrings starts in it.
    for (size_t i = 0; i <= text.length(); ++i) {
        if (answer[i] == substrNum) {
            std::cout << i << " ";
        }
    }
}

void solve(const std::string& text, const std::string& sample,
                                std::vector<vertex>& forest) {
    if (sample.length() > text.length()) {
        return;
    }

    //parse text for the constant substring
    //Another way to parse a string: done.
    //Also could use strtok (on char*), for example

    //It is a bit shorter.
    //But! Using any standart parsing we have to perform extra actions
    //to find indexes of substrings after that. +O(N). Does it justify the means?
    size_t substrNum = 0;
    std::vector<size_t> substrEndIndex;
    std::vector<std::string> seglist;
    boost::split(seglist, sample, boost::is_any_of("?"));

    size_t pos = 0;
    for (size_t i = 0; i < seglist.size(); ++i) {
        if (seglist[i].length() == 0) {
            continue;
        }
        ++substrNum;
        pos = sample.find(seglist[i], pos);
        substrEndIndex.push_back(pos + seglist[i].length() - 1);
        addString(seglist[i], forest, substrNum - 1);
    }

    //if sample contain only ?'s
    if (substrNum == 0) {
        for (size_t i = 0; i <= text.length() - sample.length(); ++i) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        return;
    }

    goThroughForest(text, forest, substrEndIndex, substrNum);
    return;
}

void init(std::string& text, std::string& sample, std::vector<vertex>& forest) {
    std::cin >> sample;
    std::cin >> text;
    vertex root(ALPHSIZE);
    root.parent = -1;
    root.isLeaf = false;
    root.number = 0;
    forest.push_back(root);
}

int main() {
    std::string text, sample;
    std::vector<vertex> forest;

    init(text, sample, forest);
    solve(text, sample, forest);

    return 0;
}

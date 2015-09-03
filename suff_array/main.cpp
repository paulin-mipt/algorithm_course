#include <iostream>
#include <cstdio>
#include <std::string>
#include <algorithm>
#include <std::vector>

void buildSuffixArray(const std::string& str, std::vector<size_t>& suffixArray) {
    size_t textLength = str.length();
    std::vector<size_t> sortSecond(textLength, 0); //sequence, sorted by second half of substrings
    std::vector<size_t> classes(textLength, 0); //equivalence class number for i'th cyclic subtring
    std::vector<size_t> tempClasses(textLength, 0);
	size_t classesNumber = 'z'; //max char value
    std::vector<size_t> classesCount(max(classesNumber, textLength)); //number of substrs in i'th class
    //initialization
	for (size_t i = 0; i < textLength; ++i) {
        suffixArray[i] = i;
    }
	for (size_t i = 0; i < textLength; ++i) {
        classes[i] = str[i];
    }

	for (size_t k = 0; k <= textLength; k = k*2 + !k) {
        //cyclic shift on k right
		for (size_t i = 0; i < textLength; ++i) {
			sortSecond[i] = (suffixArray[i] - k + textLength) % textLength;
        }
        //init classesCount
		for (size_t i = 0; i < classesNumber; ++i) {
			classesCount[i] = 0;
        }
		for (size_t i = 0; i < textLength; ++i) {
			++classesCount[classes[i]];
        }
		// classesCount calculation
		for (size_t i = 1; i < classesNumber; ++i) {
			classesCount[i] += classesCount[i - 1];
        }
		for (size_t i = textLength - 1; i + 1 > 0; --i) {
            size_t secondPartBlock = classes[sortSecond[i]];
            --classesCount[secondPartBlock];
			suffixArray[classesCount[secondPartBlock]] = sortSecond[i];
        }

		tempClasses[suffixArray[0]] = 0;
		for (size_t i = 1; i < textLength; ++i) {
            size_t curr = suffixArray[i];
            size_t prev = suffixArray[i - 1];
			tempClasses[curr] = tempClasses[prev];
			if (classes[curr] != classes[prev] ||
                classes[(curr + k) % textLength] != classes[(prev + k) % textLength])
				tempClasses[suffixArray[i]]++;
		}

		classesNumber = tempClasses[suffixArray[textLength - 1]] + 1;
		//copying new classes
		for (size_t i = 0; i < textLength; ++i) {
            classes[i] = tempClasses[i];
        }
	}
}

//simple counting, O(N)
size_t longestCommonPref(size_t samplePos, size_t textPos, std::string& text, std::string& sample) {
    size_t answer = 0;
    while (samplePos < sample.length() && sample[samplePos] == text[textPos]) {
        answer++;
        samplePos++;
        textPos++;
    }
    return answer;
}

//compare function
int cmpPrefix(size_t samplePos, size_t textPos, std::string& text, std::string& sample) {
    size_t shared = longestCommonPref(samplePos, textPos, text, sample);
    if (shared + samplePos == sample.length()) {
        return 0;
    }
    if (sample[shared + samplePos] < text[shared + textPos]) {
        return -1;
    }
    else if (sample[shared + samplePos] > text[shared + textPos]) {
        return 1;
    } else {
        return 0;
    }
}

//search with mlr-optimization
void binSearch(size_t left, size_t right, size_t mlr, size_t& leftBound, size_t& rightBound,
                            std::vector<size_t>& pos, std::string& text, std::string& sample) {
    if (left > right) {
        return;
    }

    size_t middle = (left + right) / 2;
    mlr += std::min(longestCommonPref(mlr, mlr + pos[left], text, sample),
                    longestCommonPref(mlr, mlr + pos[right], text, sample));
    int compare = cmpPrefix(mlr, pos[middle] + mlr, text, sample);
    if (compare == 0) {
        if (middle < leftBound) {
            leftBound = middle;
        }
        if (middle > rightBound) {
            rightBound = middle;
        }
        binSearch(left, middle - 1, mlr, leftBound, rightBound, pos, text, sample);
        binSearch(middle + 1, right, mlr, leftBound, rightBound, pos, text, sample);
    }
    else if (compare < 0) {
        binSearch(left, middle - 1, mlr, leftBound, rightBound, pos, text, sample);
    }
    else {
        binSearch(middle + 1, right, mlr, leftBound, rightBound pos, text, sample);
    }
}

void init(const std::string& text, const std::string& sample, std::vector<size_t>& suffixArray) {
    std::cin >> text;
	std::cin >> sample;
	suffixArray.resize(text.length() + 1);
    if (sample.length() > text.length()) {
        exit(0);
    }

    buildSuffixArray(text + '$', suffixArray);
}

void solve(const std::string& text, const std::string& sample, const std::vector<size_t>& suffixArray) {
    size_t leftBound = text.size(); //store left bound of answer segment on suffix array
    size_t rightBound = 0; //right bound
    binSearch(0, text.size(), 0, leftBound, rightBound, suffixArray, text, sample);

    std::vector<size_t> answer;
    for (size_t i = leftBound; i <= rightBound; ++i) {
        answer.push_back(suffixArray[i]);
    }

    sort(answer.begin(), answer.end());
    for (size_t i = 0; i < answer.size(); ++i) {
        std::cout << answer[i] << " ";
    }
}

int main() {
	std::string text, sample;
    std::vector<size_t> suffixArray;
    init(text, sample, suffixArray);
    solve(ext, sample, suffixArray);
	return 0;
}

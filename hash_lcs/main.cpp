#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>

const int NOSTRING = -1;
const long long HASHBASE = 1000000007;
const long long HASHMULT = 13;

long long binPower(long long number, long long degree) {
	if (degree == 0) {
        return 1;
	}
	if (degree % 2 == 1) {
		return (binPower(number, degree - 1) * number) % HASHBASE;
    } else {
		long long sqr = binPower(number, degree / 2);
		return (sqr * sqr) % HASHBASE;
	}
}

long long countSubstrHash(const std::vector<long long>& hashFunc, size_t startIndex, size_t endIndex) {
    if (startIndex == 0) {
        return hashFunc[endIndex];
    }
    long long answer = hashFunc[endIndex] - hashFunc[startIndex - 1];
    if (answer < 0) {
        answer += HASHBASE;
    }
    long long degree = 1;
    for (size_t i = 0; i < startIndex; ++i) {
        degree *= HASHMULT;
        degree %= HASHBASE;
    }
    answer *= binPower(degree, HASHBASE - 2);
    return answer % HASHBASE;
}

void countHashFuncVector(std::vector<long long>& answer, const std::string& str) {
    answer.resize(str.length());
    long long currentHash = 0;
    long long currentDegree = 1;

    for (size_t i = 0; i < str.length(); ++i) {
        currentHash += (currentDegree * str[i]) % HASHBASE;
        currentHash %= HASHBASE;
        answer[i] = currentHash;
        currentDegree *= HASHMULT;
        currentDegree %= HASHBASE;
    }
}

void countHashFuncMap(size_t length, std::map<long long, size_t>& answer, const std::string& str) {
    for (size_t i = 0; i < str.length() - length + 1; ++i) {
        long long currentHash = 0;
        long long currentDegree = 1;
        for (size_t j = 0; j < length; ++j) {
            currentHash += (currentDegree * str[i + j]) % HASHBASE;
            currentHash %= HASHBASE;
            currentDegree *= HASHMULT;
            currentDegree %= HASHBASE;
        }
        answer[currentHash] = i;
    }
}

std::pair<int, int> indexOfCS(size_t length, const std::vector<long long>& hashFirst,
                    const std::string first, const std::string second) {
    std::map<long long, size_t> hashSecond;
    countHashFuncMap(length, hashSecond, second);
    for (size_t i = 0; i < hashFirst.size() - length + 1; ++i) {
        long long substrHash = countSubstrHash(hashFirst, i, i + length - 1);
        std::map<long long, size_t>::const_iterator it = hashSecond.find(substrHash);
        if (it != hashSecond.end()) {
            size_t foundIndexSecond = it->second;
            //compare in a stupid way
            size_t j = 0;
            while (j < length && first[i + j] == second[foundIndexSecond + j]) {
                ++j;
            }
            if (j == length) {
                return std::make_pair(i, foundIndexSecond);
            }
        }
    }
    return std::make_pair(NOSTRING, NOSTRING);
}

std::pair<std::pair<int, int>, int> findLCS(const std::string& first, const std::string& second) {
    size_t rightBound = std::min(first.length(), second.length()) + 1;
    size_t leftBound = 1;

    std::vector<long long> firstHashVector;
    countHashFuncVector(firstHashVector, first);

    //binsearch by length of the substring
    while (leftBound < rightBound - 1) {
        size_t midBound = (leftBound + rightBound) / 2;
        if (indexOfCS(midBound, firstHashVector, first, second).first != NOSTRING) {
            leftBound = midBound;
        } else {
            rightBound = midBound;
        }
    }

    std::pair<long long, long long> answerPosition;
    //std::cout << leftBound << " bound" << std::endl;
    answerPosition = indexOfCS(leftBound, firstHashVector, first, second);
    if (answerPosition.first != NOSTRING) {
        return std::make_pair(answerPosition, leftBound);
    } else {
        return std::make_pair(std::make_pair(NOSTRING, NOSTRING), NOSTRING);
    }
}

std::string randomString(size_t length, size_t alphSize) {
    std::string ans = "";
    for (size_t i = 0; i < length; i++) {
        ans += ('a' + (rand() % alphSize));
    }
    return ans;
}

void test() {
    //std::cin >> sample >> text;
    std::string text = randomString(100, 3);
    std::string sample = randomString(100, 3);
    std::string answer = "";
    for (size_t i = 0; i < text.length(); ++i) {
        for (size_t j = 0; j < sample.length(); ++j) {
            std::string temp = "";
            size_t k = 0;
            while (i + k < text.length() && j + k < sample.length() && text[i + k] == sample[j + k]) {
                temp += text[i + k];
                ++k;
            }
            if (temp.length() > answer.length()) {
                answer = temp;
            }
        }
    }
    std::pair<std::pair<int, int>, int> hashAns = findLCS(sample, text);
    int firstStart = hashAns.first.first;
    int secondStart = hashAns.first.second;
    int leng = hashAns.second;
    std::string ansFirst = sample.substr(firstStart, leng);
    std::string ansSecond = text.substr(secondStart, leng);

    if (ansFirst != ansSecond || ansFirst.size() != answer.size()) {
        std::cout << "FAILED" << std::endl;
        std::cout << sample << std::endl;
        std::cout << text << std::endl;
        std::cout << "found: " << ansFirst << " and " << ansSecond << std::endl;
        std::cout << "expected: " << answer << std::endl;
        exit(1);
    } else {
        std::cout << "OK" << std::endl;
    }
}

int main() {
    /*std::string first, second;
    std::cin >> first >> second;
    //first = "caacc";
    //second = "bcbaabbc";
    std::pair<int, int> answer = findLCS(first, second);

    if (answer.first == NOSTRING) {
        std::cout << "No common substrings";
        return 0;
    }

    std::cout << first.substr(answer.first, answer.second - answer.first); */
    srand(clock());
    while (1) {
        test();
    }
    return 0;
}

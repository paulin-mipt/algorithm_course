#include <iostream>
#include <vector>

const size_t CHARNUM = 'z' - 'a' + 1;

struct state {
    size_t len;
    int suffLink;
	std::vector<int> moves;
};

void updateOnSymbol(std::vector<state>& states, size_t& stringSize,
                                size_t& endingState, const size_t addedChar) {
	size_t newEndingState = stringSize;
	stringSize++;
	states[newEndingState].moves.resize(CHARNUM, -1);
	states[newEndingState].len = states[endingState].len + 1;
	int previous = endingState;
	while (previous != -1 && states[previous].moves[addedChar] == -1) {
        states[previous].moves[addedChar] = newEndingState;
        previous = states[previous].suffLink;
	}

	if (previous == -1) {
		states[newEndingState].suffLink = 0;
	} else {
		int q = states[previous].moves[addedChar];
		if (states[previous].len + 1 == states[q].len) {
			states[newEndingState].suffLink = q;
		} else {
			size_t clone = stringSize++;
			states[clone].moves.resize(CHARNUM, -1);
			states[clone].len = states[previous].len + 1;
			states[clone].moves = states[q].moves;
			states[clone].suffLink = states[q].suffLink;
			while (previous != -1 && states[previous].moves[addedChar] == q) {
                states[previous].moves[addedChar] = clone;
                previous = states[previous].suffLink;
			}
			states[q].suffLink = states[newEndingState].suffLink = clone;
		}
	}
	endingState = newEndingState;
}

size_t countDiffSubstrNum(const std::vector<state> states, size_t stateNum) {
    size_t result = 1;
    for (size_t i = 0; i < CHARNUM; ++i) {
        if (states[stateNum].moves[i] != -1) {
            result += countDiffSubstrNum(states, states[stateNum].moves[i]);
        }
    }

    return result;
}

void buildSuffAutomata(std::vector<state>& states, const std::string str) {
    size_t currSize = 1, lastState = 0;
	states[0].len = 0;
	states[0].suffLink = -1;
    states[0].moves.resize(CHARNUM, -1);
    for (size_t i = 0; i < str.length(); ++i) {
        updateOnSymbol(states, currSize, lastState, str[i] - 'a');
    }
}

int main() {
    std::string str;
	std::cin >> str;
    std::vector<state> states(2 * str.length());

    buildSuffAutomata(states, str);
    std::cout << countDiffSubstrNum(states, 0) - 1;
    return 0;
}

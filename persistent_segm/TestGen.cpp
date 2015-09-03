#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <random>
#include <algorithm>
#include <unistd.h>

using namespace std; 

int TEST_SIZE = 10;
int MAX_NUMBER = 20;
int MAX_QUERY_NUMBER = 10;

void testGen(vector<int>& test, vector<pair<int, int> >& queries, vector<int>& stat) {

	std::random_device rd;
    std::mt19937 gen(rd()); 
	std::uniform_int_distribution<int> len(1, TEST_SIZE), number(0, MAX_NUMBER), query(0, MAX_QUERY_NUMBER - 1);


	test.resize(len(gen) + 1);
	for (int i = 0;i < test.size();++i) {
		test[i] = number(gen);
	}

	int queryNum = query(gen);
	queries.resize(queryNum);

	std::uniform_int_distribution<int> left(0, test.size() - 1);

	for (int i = 0;i < queries.size();++i) {
		queries[i].first = left(gen);

		std::uniform_int_distribution<int> right(queries[i].first, test.size() - 1);

		queries[i].second = right(gen);

		std::uniform_int_distribution<int> statistic(1, queries[i].second - queries[i].first + 1);

		stat.push_back(statistic(gen));

	}

	/*ofstream testlog;	
	testlog.open("testlog.txt", std::ios_base::app);
	testlog << first << endl << second << endl; 
	testlog.close();*/
}

void print_test(vector<int>& test, vector<pair<int, int> >& queries, vector<int>& stat) {
	std::cout << test.size() << endl;

	for (int i = 0;i < test.size();++i) {
		cout << test[i] << " ";
	}

	cout << endl;

	cout << queries.size() << endl;

	for (int i = 0;i < queries.size();++i) {
		cout << queries[i].first << " " << queries[i].second << " " << stat[i] << endl;
	}
}

int main(int argc, char ** argv) {

    if (argc > 1) TEST_SIZE = atol(argv[1]);
    if (argc > 2) MAX_NUMBER = atol(argv[2]);
    if (argc > 3) MAX_QUERY_NUMBER = atol(argv[3]);

	srand(time(NULL) ^ getpid());
	vector<int> test;
	vector<pair<int, int> > queries;
	vector<int> stat;

	testGen(test, queries, stat);
	print_test(test, queries, stat);
	return 0;
}

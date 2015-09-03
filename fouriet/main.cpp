#define _USE_MATH_DEFINES

#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

using std::vector;

size_t bitReverse(size_t index, size_t log2n) {
	int reversed = 0;
	for (size_t i = 0; i < log2n; ++i) {
        if (index & (1 << i)) {
            reversed |= 1 << (log2n - i - 1);
        }
	}
	return reversed;
}

void fourier(vector< std::complex<double> >& polynom, size_t log2n, bool inverse) {
	size_t polynomSize = polynom.size();

	for (size_t i = 0; i < polynomSize; ++i) {
        if (i < bitReverse(i, log2n)) {
            std::swap(polynom[i], polynom[bitReverse(i, log2n)]);
        }
	}

	size_t length = 2;
	for (size_t blockSize = 1; blockSize <= log2n; ++blockSize) {
        double mainRootAngle = 2 * M_PI / length;
        if (inverse) {
            mainRootAngle *= -1;
        }
        std::complex<double> mainRoot(cos(mainRootAngle), sin(mainRootAngle));

        for (size_t i = 0; i < polynomSize; i += length) {
            std::complex<double> root(1);
            //apply butterfly transform
            for (size_t j = 0; j < length / 2; ++j) {
                std::complex<double> leftHalf = polynom[i + j];
                std::complex<double> rightHalf = polynom[i + j + length / 2];
				polynom[i + j] = leftHalf + rightHalf * root;
				polynom[i + j + length / 2] = leftHalf - rightHalf * root;
				root *= mainRoot;
			}
        }
        length *= 2;
	}

	if (inverse) {
        for (size_t i = 0; i < polynomSize; ++i) {
            polynom[i] /= polynomSize;
        }
	}
}

void polynomMultiply(const vector<size_t>& first, const vector<size_t>& second, vector<size_t>& result) {
	vector< std::complex<double> > tempFirst, tempSecond;
	for (size_t i = 0; i < first.size(); ++i) {
        tempFirst.push_back(std::complex<double>(first[i]));
	}
	for (size_t i = 0; i < second.size(); ++i) {
        tempSecond.push_back(std::complex<double>(second[i]));
	}

	size_t polynomSize = 1;
	size_t log2n = 0;
	while (polynomSize < first.size()) {
        polynomSize *= 2;
        ++log2n;
	}

	tempFirst.resize(polynomSize), tempSecond.resize(polynomSize);
	fourier(tempFirst, log2n, false);
	fourier(tempSecond, log2n, false);

	for (size_t i = 0; i < polynomSize; ++i) {
        tempFirst[i] *= tempSecond[i];
	}
	fourier(tempFirst, log2n, true);

	result.resize(polynomSize);
	for (size_t i = 0; i < polynomSize; ++i) {
        result[i] = tempFirst[i].real() + 0.5;
	}
}

int main() {
    std::string first, second;
    std::cin >> first >> second;
    //first = "bbaaaaabbb";
    //second = "ababbabbba";
    size_t len = first.size();

    vector<size_t> first_coeff(len * 2), second_coeff(len * 2);
    for (size_t i = 0; i < len; ++i) {
        first_coeff[len - i - 1] = first[i] - 'a';
        first_coeff[len + i] = 0;
        second_coeff[i] = second[i] - 'a';
        second_coeff[len + i] = second_coeff[i];
    }

    vector<size_t> resultOnes(len * 2);
    polynomMultiply(first_coeff, second_coeff, resultOnes);

    for (size_t i = 0; i < len; ++i) {
        first_coeff[i] = (first_coeff[i] + 1) % 2;
        second_coeff[i] = (second_coeff[i] + 1) % 2;;
        second_coeff[len + i] = second_coeff[i];
    }

    vector<size_t> resultZeros(len * 2);
    polynomMultiply(first_coeff, second_coeff, resultZeros);

    size_t minDiff = len + 1;
    for (size_t i = len; i < len * 2; ++i) {
        size_t shift = len - (resultOnes[i] + resultZeros[i]);
        minDiff = std::min(minDiff, shift);
    }

    std::cout << minDiff << std::endl;
    return 0;
}

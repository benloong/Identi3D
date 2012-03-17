//
// File: matrix_test.cc
// ====================
//

#include "matrix_test.h"

using namespace std;

namespace Identi3D
{
	
	const Matrix MatrixTest::generateRandomMatrix(void)
	{
		srand((unsigned int)time(NULL));
		Matrix m((float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100),
				 (float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100),
				 (float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100),
				 (float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100));
		return m;
	}

	bool checkMMMAnswer(const Matrix &a, const Matrix &b, const Matrix &target)
	{
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				if(!_fcomp(a(i, 0) * b(0, j) + a(i, 1) * b(1, j) + a(i, 2) * b(2, j) + a(i, 3) * b(3, j), 
					target(i, j))) {
						cout << "MMM> Unmatching pair detected at (" << i << ", " << j << ")" << endl;
						return false;
				}
			}
		}
		return true;
	}

	void MatrixTest::testMatrixMatrixMultiply(void)
	{
		Matrix a = generateRandomMatrix();
		Matrix b = generateRandomMatrix();
		Matrix result = a * b;

		cout << "MMM> Matrix a:" << endl << a;
		cout << "MMM> plus matrix b:" << endl << b;
		cout << "MMM> Math library result:" << endl << result;
		if(checkMMMAnswer(a, b, result)) cout << "MMM> Correct." << endl;
		else cout << "MMM> Incorrect." << endl;
		cout << endl;
	}

	bool checkMIAnswer(const Matrix &m, const Matrix &target)
	{
		Matrix t = m * target;
		cout << "MI> M * M^-1 = " << endl << t;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				if(i == j) {
					if(!_fcomp(t(i, j), 1)) {
						cout << "MI> Unmatching pair detected at (" << i << ", " << j << ")" << endl;
						return false;
					}
				} else {
					if(!_fcomp(t(i, j), 0)) {
						cout << "MI> Unmatching pair detected at (" << i << ", " << j << ")" << endl;
						return false;
					}
				}
			}
		}
		return true;
	}

	void MatrixTest::testMatrixInverse(void)
	{
		Matrix m = generateRandomMatrix();
		Matrix result;

		cout << "MI> Inverse the matrix -" << endl << m;
		result.inverse(m);
		cout << "MI> Math library result:" << endl << result;
		if(checkMIAnswer(m, result)) cout << "MI> Correct." << endl;
		else cout << "MI> Incorrect." << endl;
		cout << endl;
	}

};

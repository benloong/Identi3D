//
// File: vector_test.cc
// ====================
//

#include "vector_test.h"
#include "matrix_test.h"

using namespace std;

namespace Identi3D
{

	const Vector3 VectorTest::generateRandomVector(void)
	{
		Vector3 v((float)(rand() % 100), (float)(rand() % 100), (float)(rand() % 100));
		return v;
	}

	bool checkVMMAnswer(const Vector3 &v, const Matrix &m, const Vector3 &target)
	{
		float x = v.getX() * m(0, 0) + v.getY() * m(1, 0) + v.getZ() * m(2, 0) + m(3, 0);
		float y = v.getX() * m(0, 1) + v.getY() * m(1, 1) + v.getZ() * m(2, 1) + m(3, 1);
		float z = v.getX() * m(0, 2) + v.getY() * m(1, 2) + v.getZ() * m(2, 2) + m(3, 2);
		float w = v.getX() * m(0, 3) + v.getY() * m(1, 3) + v.getZ() * m(2, 3) + m(3, 3);

		x /= w, y /= w, z /= w;
		cout << "VMM> Checking against (" << x << ", " << y << ", " << z << ")" << endl;
		if(_fcomp(x, target.getX()) && _fcomp(y, target.getY()) && _fcomp(z, target.getZ())) {
			return true;
		}
		return false;
	}

	void VectorTest::testVectorMatrixMultiply(void)
	{
		Vector3 v = generateRandomVector();
		Matrix m = MatrixTest::generateRandomMatrix();

		cout << "VMM> Matrix:" << endl << m;
		cout << "VMM> plus vector " << v << endl;
		Vector3 result = v * m;
		cout << "VMM> Math library result: " << result << endl;
		if(checkVMMAnswer(v, m, result)) cout << "VMM> Correct." << endl;
		else cout << "VMM> Incorrect." << endl;
		cout << endl;
	}

	bool checkVNAnswer(const Vector3 &v, const Vector3 &target)
	{
		float length = sqrtf(v.getX() * v.getX() + v.getY() * v.getY() + v.getZ() * v.getZ());
		float x = v.getX() / length;
		float y = v.getY() / length;
		float z = v.getZ() / length;

		cout << "VN> Checking against (" << x << ", " << y << ", " << z << ")" << endl;
		if(_fcomp(target.getX(), x) && _fcomp(target.getY(), y) && _fcomp(target.getZ(), z)) {
			return true;
		}
		return false;
	}

	void VectorTest::testVectorNormalize(void)
	{
		Vector3 v = generateRandomVector();
		Vector3 result;

		cout << "VN> Normalizing Vector " << v << endl;
		result = v;
		result.normalize();
		cout << "VN> Math library result: " << result << endl;
		if(checkVNAnswer(v, result)) cout << "VN> Correct." << endl;
		else cout << "VN> Incorrect." << endl;
		cout << endl;
	}

	bool checkVCAnswer(const Vector3 &u, const Vector3 &v, const Vector3 &target)
	{
		if(_fzero(target * u) && _fzero(target * v)) return true;
		return false;
	}

	void VectorTest::testVectorCross(void)
	{
		Vector3 u = generateRandomVector();
		Vector3 v = generateRandomVector();
		Vector3 result;

		cout << "VC> " << u << " crosses " << v << endl;
		result.cross(u, v);
		cout << "VC> Math library result: " << result << endl;
		if(checkVCAnswer(u, v, result)) cout << "VC> Correct." << endl;
		else cout << "VC> Incorrect." << endl;
		cout << endl;
	}

};

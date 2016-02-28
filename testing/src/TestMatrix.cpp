/*
 * TestMatrix.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: andrew
 */

#include "lib/util/Matrix.h"
#include "TestMatrix.h"

using namespace frc973;

void TestMatrix::TestMatrixSetGet() {
	Matrix m = Matrix(4, 6);

	m.Set(0, -17.5);
	ASSERT(m.Get(0) == -17.5);
}

void TestMatrix::TestMatrixFlash() {
	Matrix m = Matrix(4, 6);

	m.Set(7, -5.0);

	double *d = new double[7];
	d[0] = 3.0;
	d[1] = 5.0;
	d[2] = 7.0;
	d[3] = 9.0;
	d[4] = 11.0;
	d[5] = 13.0;
	d[6] = 15.0;

	m.Flash(d, 7);

	for (int i = 0; i < 7; i++) {
		ASSERT_EQUAL(m.Get(i), 3.0 + (2.0 * i));
	}

	ASSERT(m.Get(7) == -5.0);

	delete[] d;
}

/**
 * Populate a matrix with dummy data for testing purposes
 *
 * If you give different seeds, you'll probably get different numbers,
 * but if you give the same seed, you'll definitely get the same numbers
 */
static void PopulateMatrixWithDummyData(Matrix &m, int seed) {
	for (int i = (m.GetWidth() * m.GetHeight()) - 1; i >= 0; i--) {
		m.Set(i, (seed * i + 5) * (3 * i * i - 4));
	}
}

/**
 * A should be equal to B and only B.
 *  * c has different values but same dimensions
 *  * d, e, and f have different dimensions
 */
void TestMatrix::TestMatrixEquality() {
	Matrix a = Matrix(2, 2);
	PopulateMatrixWithDummyData(a, 7);

	Matrix b = Matrix(2, 2);
	PopulateMatrixWithDummyData(b, 7);

	Matrix c = Matrix(2, 2);
	PopulateMatrixWithDummyData(c, 8);

	Matrix d = Matrix(2, 3);
	PopulateMatrixWithDummyData(d, 7);

	Matrix e = Matrix(3, 2);
	PopulateMatrixWithDummyData(e, 7);

	Matrix f = Matrix(1, 4);
	PopulateMatrixWithDummyData(f, 7);

	ASSERTM("Matrices of same size and val should be equal", a.Equals(&b));
	ASSERTM("Matrices of same size but different val should be unequal",
			!a.Equals(&c));
	ASSERTM("Matrices of different size should be unequal",
			!a.Equals(&d));
	ASSERTM("Matrices of different size should be unequal",
			!a.Equals(&e));
	ASSERTM("Matrices of different size should be unequal",
			!a.Equals(&f));
}

void TestMatrix::TestMatrixMultiplication() {
	Matrix x = Matrix(3, 2);
	Matrix y = Matrix(2, 3);
	Matrix xy = Matrix(2, 2);

	x.Set(0, 0, 1);
	x.Set(1, 0, 2);
	x.Set(2, 0, 3);
	x.Set(0, 1, 4);
	x.Set(1, 1, 5);
	x.Set(2, 1, 6);

	y.Set(0, 0, 1);
	y.Set(0, 1, 2);
	y.Set(0, 2, 3);
	y.Set(1, 0, 4);
	y.Set(1, 1, 5);
	y.Set(1, 2, 6);

	xy.Set(0, 0, 14);
	xy.Set(0, 1, 32);
	xy.Set(1, 0, 32);
	xy.Set(1, 1, 77);

	Matrix *z = Matrix::Multiply(&y, &x);

	//x.Display();
	//y.Display();
	//z->Display();

	ASSERTM("Multiplication of matrices didn't work", xy.Equals(z));
	ASSERTM("Expected error on dimension mismatch",
			Matrix::Multiply(&y, z) == nullptr);

	delete z;
}

void TestMatrix::TestMatrixAddition() {
	Matrix x = Matrix(2, 2);
	Matrix y = Matrix(2, 2);
	Matrix xy = Matrix(2, 2);
	Matrix r = Matrix(5, 5);

	x.Set(0, 0, 4);
	x.Set(1, 0, 7);
	x.Set(0, 1, 2);
	x.Set(1, 1, 7);

	y.Set(0, 0, 43);
	y.Set(1, 0, 2);
	y.Set(0, 1, 76);
	y.Set(1, 1, 2);

	xy.Set(0, 0, 47);
	xy.Set(1, 0, 9);
	xy.Set(0, 1, 78);
	xy.Set(1, 1, 9);

	Matrix *z = Matrix::Add(&y, &x);

	ASSERTM("Addition of matrices didn't work", xy.Equals(z));
	ASSERTM("Expected error on dimension mismatch",
			Matrix::Multiply(&r, z) == nullptr);

	delete z;
}

void TestMatrix::TestMatrixSubtraction() {	Matrix x = Matrix(2, 2);
	Matrix y = Matrix(2, 2);
	Matrix xy = Matrix(2, 2);
	Matrix r = Matrix(5, 5);

	x.Set(0, 0, 4);
	x.Set(1, 0, 7);
	x.Set(0, 1, 2);
	x.Set(1, 1, 7);

	y.Set(0, 0, 43);
	y.Set(1, 0, 2);
	y.Set(0, 1, 76);
	y.Set(1, 1, 2);

	xy.Set(0, 0, -39);
	xy.Set(1, 0, 5);
	xy.Set(0, 1, -74);
	xy.Set(1, 1, 5);

	Matrix *z = Matrix::Subtract(&x, &y);

	ASSERTM("Subtraction of matrices didn't work", xy.Equals(z));
	ASSERTM("Expected error on dimension mismatch",
			Matrix::Multiply(&r, z) == nullptr);

	delete z;
}

cute::suite TestMatrix::MakeSuite(){
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestMatrix, TestMatrixFlash));
	s.push_back(CUTE_SMEMFUN(TestMatrix, TestMatrixSetGet));
	s.push_back(CUTE_SMEMFUN(TestMatrix, TestMatrixEquality));
	s.push_back(CUTE_SMEMFUN(TestMatrix, TestMatrixMultiplication));
	s.push_back(CUTE_SMEMFUN(TestMatrix, TestMatrixAddition));
	s.push_back(CUTE_SMEMFUN(TestMatrix, TestMatrixSubtraction));
	return s;
}

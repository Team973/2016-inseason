/*
 * Matrix.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Andrew
 */

#ifndef LIB_MATRIX_H_
#define LIB_MATRIX_H_

#include "math.h"
#include <stdio.h>

/**
 * Matrix implementation. All fields are doubles.
 */
class Matrix {
public:
	Matrix(int nRows, int nCols) {
		int i;
		m_width = nCols;
		m_height = nRows;
		m_data = new double[m_width * m_height];
		for(i = 0; i<nRows*nCols; i++) {
		  m_data[i] = 0.0;
		}
	}

	virtual ~Matrix() {
		delete[] m_data;
	}

	double *GetData() {
		return m_data;
	}

	double Get(int y,int x){
		if (x >= m_width || y >= m_height){
			printf("Array out of bounds: wanted (%d, %d) from (%d, %d)\n",
					y, x, m_height, m_width);
			return NAN;
		}
		return m_data[x + (m_width * y)];
	}

	double Get(int i) {
		if (i >= m_width * m_height){
			printf("Array out of bounds: wanted %d from (%d, %d)\n",
					i, m_height, m_width);
			return NAN;
		}
		return m_data[i];
	}

	void Set(int y, int x, double val){
		if (x >= m_width || y >= m_height){
			printf("Array out of bounds: wanted (%d, %d) from (%d, %d)\n",
					y, x, m_height, m_width);
		}
		m_data[x + (m_width * y)] = val;
	}

	void Set(int i, double val){
		if (i >= m_width * m_height){
			printf("Array out of bounds: wanted %d from (%d, %d)\n",
					i, m_height, m_width);
		}
		m_data[i] = val;
	}

	int GetWidth() {
		return m_width;
	}

	int GetHeight() {
		return m_height;
	}

	bool SameSize(Matrix *m) {
		return (GetWidth() == m->GetWidth()) && (GetHeight() == m->GetHeight());
	}

	static Matrix *Subtract(Matrix *mat1, Matrix *mat2) {
		if (!mat1->SameSize(mat2)) {
		}
		Matrix *result = new Matrix(mat2->GetHeight(),mat1->GetWidth());
		int comp = mat1->GetWidth() * mat2->GetHeight();

		for(int i = 0; i < comp; i++){
			result->Set(i, mat1->Get(i) - mat2->Get(i));
		}
		return result;
	}

	static Matrix *Add(Matrix *mat1,Matrix *mat2) {
		if (!mat1->SameSize(mat2)) {
			printf("Tried to multiple matrices of not the same size");
			return nullptr;
		}

		Matrix *result = new Matrix(mat1->GetHeight(), mat1->GetWidth());
		int comp = mat1->GetWidth() * mat1->GetHeight();
		int i;
		for(i = 0; i < comp; i++){
			result->Set(i, mat1->Get(i) + mat2->Get(i));
		}
		return result;
	}

	static Matrix *Multiply(Matrix *mat1, Matrix *mat2) {
		if (mat1->GetWidth() != mat2->GetHeight()) {
			printf("Cant multiply matrices of disparit dimensions\n");
		}

		int destHeight = mat1->GetHeight();
		int destWidth = mat2->GetWidth();

		Matrix *result = new Matrix(destHeight, destWidth);

		int pMax = mat1->GetWidth();
		int width1 = mat1->GetWidth();
		int width2 = mat2->GetWidth();
		for(int i = 0; i < destWidth; i++){
			for(int j = 0; j < destHeight;j++){
				double tmp = 0.0;
				for(int p = 0; p < pMax; p++){
					tmp += mat2->m_data[i + width2 * p] * mat1->m_data[p + width1 * j];
				}
				result->m_data[i + destWidth * j] = tmp;
			}
		}
		return result;
	}


  	/**
  	 * Replace the first |n| values in this matrix with the first |n|
  	 * values in the array defined by d
  	 */
	void Flash(const double *d, const int N){
		for (int i = 0; i < N; i++) {
			m_data[i] = d[i];
		}
	}

	bool Equals(Matrix *m) {
		if (!SameSize(m)) {
			return false;
		}

		for (int i = 0; i < GetHeight(); ++i) {
			for (int j = 0; j < GetWidth(); ++j) {
				if (Get(i,j) != m->Get(i,j)) {
					return false;
				}
			}
		}

		return true;
	}


private:
	double *m_data;
	int m_width;
	int m_height;
};

#endif /* LIB_MATRIX_H_ */

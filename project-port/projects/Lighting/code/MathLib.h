#pragma once

/**
 * @file
 * @author Emil Lindgren
 * @version 0.1
 * 
 * @section LICENSE
 * 
 * MIT License
 * 
 * Copyright (c) 2019 Emil Lindgren
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * @section DESCRIPTION
 * 
 * "matlib" is a helper library for 3D graphics.
 * 
 * It contains routines and classes to help with transformations,
 * specifically a 4D Vector class and a 4D matrix class.
 */

// C++ headers
#include <stdexcept>

// C headers
#include <cmath>


namespace MathLib {

	/**
	 * A four dimensional homogenous vector.
	 * 
	 * Multiplication, addition, subtraction, multiplication, normalize,
	 * dot product and cross product all only act on the x/y/z axes.
	 * The w axis is reserved for matrix operations.
	 */
	class Vec4 {
	public:
		/**
		 * Create a new homogenous 4D vector.
		 * 
		 * @param x is the x value.
		 * @param y is the y value.
		 * @param z is the z value.
		 * @param w is the w value.
		 */
		Vec4(float x = 0, float y = 0, float z = 0, float w = 0) {
			values[0] = x;
			values[1] = y;
			values[2] = z;
			values[3] = w;
		}

		/** Copy constructor. */
		Vec4(const Vec4& other){
			values[0] = other.values[0];
			values[1] = other.values[1];
			values[2] = other.values[2];
			values[3] = other.values[3];
		}

		/** Index overload with security bounds. */
		float operator [] (int index) const {

			if (index >=0 && index <= 3) {
				return values[index];
			}
			else {
				throw std::out_of_range("Index out of range.");
			}
		}

		/** Index overload with security bounds. */
		float & operator [] (int index) {
			if (index >=0 && index <= 3) {
				return values[index];
			}
			else {
				throw std::out_of_range("Index out of range.");
			}
		}

		/** Addition overload. */
		Vec4 operator + (const Vec4& other) {
			return Vec4(
				values[0] + other.values[0],
				values[1] + other.values[1],
				values[2] + other.values[2]
			);
		}

		/** Subtraction overload. */
		Vec4 operator - (const Vec4& other) {
			return Vec4(
				values[0] - other.values[0],
				values[1] - other.values[1],
				values[2] - other.values[2]
			);
		}

		/** Multiplication overload. */
		Vec4 operator * (const float& other) {
			return Vec4(
				values[0] * other,
				values[1] * other,
				values[2] * other
			);
		}

		/** Calculates the length of the xyz vector. */
		float Length() {
			return sqrt(
				values[0]*values[0] +
				values[1]*values[1] +
				values[2]*values[2]
			);
		}

		/** Normalizes this vector. */
		Vec4& Normalize() {
			float len = Length();
			values[0] /= len;
			values[1] /= len;
			values[2] /= len;
			return *this;
		}

		static Vec4 Normalized(const Vec4& vec) {
			float len = (
				vec[0] * vec[0] +
				vec[1] * vec[1] +
				vec[2] * vec[2]
				);

			return Vec4(
				vec[0] * len,
				vec[1] * len,
				vec[2] * len,
				vec[3]
			);
		}

		/**
		 * Returns the dot product of two vectors.
		 * 
		 * @param x is the first vector.
		 * @param y is the second vector.
		 */
		static float DotProduct(Vec4& x, Vec4& y) {
			return (x[0]*y[0] + x[1]*y[1] + x[2]*y[2]);
		}

		/** Returns the cross product of two vectors. */
		static Vec4 CrossProduct(const Vec4& x, const Vec4& y) {
			return Vec4(
				x[1]*y[2] - y[1]*x[2],
				x[2]*y[0] - y[2]*x[0],
				x[0]*y[1] - y[0]*x[1]
			);
		}

		void Print() {
			printf("%f ", values[0]);
			printf("%f ", values[1]);
			printf("%f ", values[2]);
			printf("%f", values[3]);
			printf("\n");
		}

	private:
		/** The x/y/z/w floats of this vector. */
		float values[4];
	};


	/** A four dimensional matrix. */
	class Mat4 {
	public:
		/**
		 * Create 4D matrix using 16 floats.
		 * 
		 * a, b, c, d is the first column etc.
		 * */
		Mat4(
			float a = 0, float b = 0, float c = 0, float d = 0,
			float e = 0, float f = 0, float g = 0, float h = 0,
			float i = 0, float j = 0, float k = 0, float l = 0,
			float m = 0, float n = 0, float o = 0, float p = 0) {

			columns[0] = Vec4(a, b, c, d);
			columns[1] = Vec4(e, f, g, h);
			columns[2] = Vec4(i, j, k, l);
			columns[3] = Vec4(m, n, o, p);
		}

		/** Create 4D matric using 4 vector columns. */
		Mat4(Vec4 a, Vec4 b, Vec4 c, Vec4 d) {
			columns[0] = a;
			columns[1] = b;
			columns[2] = c;
			columns[3] = d;
		}

		/** Copy constructor. */
		Mat4(const Mat4& other) {
			columns[0] = other.columns[0];
			columns[1] = other.columns[1];
			columns[2] = other.columns[2];
			columns[3] = other.columns[3];
		}
		/** Index overload with security bounds. */
		Vec4 operator [] (int index) const {
			if (index >=0 && index <= 3) {
				return columns[index];
			}
			else {
				throw std::out_of_range("Index out of range.");
			}
		}
		/** Index overload with security bounds. */
		Vec4 &operator [] (int index) {
			if (index >=0 && index <= 3) {
				return columns[index];
			}
			else {
				throw std::out_of_range("Index out of range.");
			}
		}

		/** Matrix-matrix multiplication. */
		Mat4 operator * (const Mat4& other) {
			// Do stuff.
			Mat4 t = Mat4::Transpose(other);

			return Mat4(
				columns[0][0] * t[0][0] + columns[0][1] * t[0][1] + columns[0][2] * t[0][2] + columns[0][3] * t[0][3],
				columns[0][0] * t[1][0] + columns[0][1] * t[1][1] + columns[0][2] * t[1][2] + columns[0][3] * t[1][3],
				columns[0][0] * t[2][0] + columns[0][1] * t[2][1] + columns[0][2] * t[2][2] + columns[0][3] * t[2][3],
				columns[0][0] * t[3][0] + columns[0][1] * t[3][1] + columns[0][2] * t[3][2] + columns[0][3] * t[3][3],

				columns[1][0] * t[0][0] + columns[1][1] * t[0][1] + columns[1][2] * t[0][2] + columns[1][3] * t[0][3],
				columns[1][0] * t[1][0] + columns[1][1] * t[1][1] + columns[1][2] * t[1][2] + columns[1][3] * t[1][3],
				columns[1][0] * t[2][0] + columns[1][1] * t[2][1] + columns[1][2] * t[2][2] + columns[1][3] * t[2][3],
				columns[1][0] * t[3][0] + columns[1][1] * t[3][1] + columns[1][2] * t[3][2] + columns[1][3] * t[3][3],

				columns[2][0] * t[0][0] + columns[2][1] * t[0][1] + columns[2][2] * t[0][2] + columns[2][3] * t[0][3],
				columns[2][0] * t[1][0] + columns[2][1] * t[1][1] + columns[2][2] * t[1][2] + columns[2][3] * t[1][3],
				columns[2][0] * t[2][0] + columns[2][1] * t[2][1] + columns[2][2] * t[2][2] + columns[2][3] * t[2][3],
				columns[2][0] * t[3][0] + columns[2][1] * t[3][1] + columns[2][2] * t[3][2] + columns[2][3] * t[3][3],

				columns[3][0] * t[0][0] + columns[3][1] * t[0][1] + columns[3][2] * t[0][2] + columns[3][3] * t[0][3],
				columns[3][0] * t[1][0] + columns[3][1] * t[1][1] + columns[3][2] * t[1][2] + columns[3][3] * t[1][3],
				columns[3][0] * t[2][0] + columns[3][1] * t[2][1] + columns[3][2] * t[2][2] + columns[3][3] * t[2][3],
				columns[3][0] * t[3][0] + columns[3][1] * t[3][1] + columns[3][2] * t[3][2] + columns[3][3] * t[3][3]);
		}

		/** Matrix-vector multiplication. */
		Vec4 operator * (const Vec4& other) {
			Vec4 v;

			// Do stuff.
			v[0] =
				other[0] * columns[0][0] +
				other[1] * columns[0][1] +
				other[2] * columns[0][2] +
				other[3] * columns[0][3];
				
			v[1] =
				other[0] * columns[1][0] +
				other[1] * columns[1][1] +
				other[2] * columns[1][2] +
				other[3] * columns[1][3];
				
			v[2] =
				other[0] * columns[2][0] +
				other[1] * columns[2][1] +
				other[2] * columns[2][2] +
				other[3] * columns[2][3];
				
			v[3] =
				other[0] * columns[3][0] +
				other[1] * columns[3][1] +
				other[2] * columns[3][2] +
				other[3] * columns[3][3];

			return v;
		}

		/** Matrix-scalar multiplication. */
		Mat4 operator * (const float& other) {
			return Mat4(
				columns[0][0] * other, columns[0][1] * other, columns[0][2] * other, columns[0][3] * other,
				columns[1][0] * other, columns[1][1] * other, columns[1][2] * other, columns[1][3] * other,
				columns[2][0] * other, columns[2][1] * other, columns[2][2] * other, columns[2][3] * other,
				columns[3][0] * other, columns[3][1] * other, columns[3][2] * other, columns[3][3] * other
			);
		}

		static const Mat4 Identity;

		/** Create a rotation matrix for the x axis in radians. */
		static Mat4 RotateEulerX(float a) {
			return Mat4(
				1,      0,       0, 0,
				0, cos(a), -sin(a), 0,
				0, sin(a),  cos(a), 0,
				0,      0,       0, 1
			);
		}

		/** Create a rotation matrix for the y axis in radians. */
		static Mat4 RotateEulerY(float a) {
			return Mat4(
				cos(a), 0, sin(a), 0,
					0, 1,      0, 0,
				-sin(a), 0, cos(a), 0,
					0, 0,      0, 1
			);
		}


		/** Create a rotation matrix for the z axis in radians. */
		static Mat4 RotateEulerZ(float a) {
			return Mat4(
				cos(a), -sin(a), 0, 0,
				sin(a),  cos(a), 0, 0,
					0,       0, 1, 0,
					0,       0, 0, 1
			);
		}

		/**
		 * Rotate around a vector axis, using w as the angle.
		 *
		 * @param axis has the x, y, and z components of the axis vector,
		 * w is used as the right hand rotation in radians around that axis.
		 */
		static Mat4 RotateRodriges(const Vec4& axis) {
			Vec4 a(Vec4::Normalized(axis));

			float ca = cos(a[3]); // Cos angle.
			float omca = 1 - cos(a[3]); // One minus cos angle.

			float sa = sin(a[3]);

			float x = a[0], y = a[1], z = a[2];

			return Mat4(
				ca+x*x*omca,	x*y*omca-z*sa,	y*sa+x*z*omca,	0,
				z*sa+x*y*omca,	ca+y*y*omca,	-x*sa+y*z*omca,	0,
				-y*sa+x*z*omca,	x*sa+y*z*omca,	ca+z*z*omca,	0,
				0,				0,				0,				1
			);
		}

		/** Returns a transposed copy of a matrix. */
		static Mat4 Transpose(Mat4 matrix) {
			return Mat4(
				matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
				matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
				matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
				matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]
			);
		}

		/** Transposes this matrix. */
		Mat4 Transpose() {
			Mat4 m(
				columns[0][0], columns[1][0], columns[2][0], columns[3][0],
				columns[0][1], columns[1][1], columns[2][1], columns[3][1],
				columns[0][2], columns[1][2], columns[2][2], columns[3][2],
				columns[0][3], columns[1][3], columns[2][3], columns[3][3]
			);
			columns[0] = m[0];
			columns[1] = m[1];
			columns[2] = m[2];
			columns[3] = m[3];

			return *this;
		}

		/** 
		 * Returns the determinant of a 4x4 matrix.
		 * 
		 * Code shamelessly copied from stackoverflow:
		 * https://stackoverflow.com/questions/2937702/i-want-to-find-determinant-of-4x4-matrix-in-c-sharp
		 */
		static float Determinant(Mat4 matrix) {
			return
				matrix[0][3] * matrix[1][2] * matrix[2][1] * matrix[3][0] - matrix[0][2] * matrix[1][3] * matrix[2][1] * matrix[3][0] -
				matrix[0][3] * matrix[1][1] * matrix[2][2] * matrix[3][0] + matrix[0][1] * matrix[1][3] * matrix[2][2] * matrix[3][0] +
				matrix[0][2] * matrix[1][1] * matrix[2][3] * matrix[3][0] - matrix[0][1] * matrix[1][2] * matrix[2][3] * matrix[3][0] -
				matrix[0][3] * matrix[1][2] * matrix[2][0] * matrix[3][1] + matrix[0][2] * matrix[1][3] * matrix[2][0] * matrix[3][1] +
				matrix[0][3] * matrix[1][0] * matrix[2][2] * matrix[3][1] - matrix[0][0] * matrix[1][3] * matrix[2][2] * matrix[3][1] -
				matrix[0][2] * matrix[1][0] * matrix[2][3] * matrix[3][1] + matrix[0][0] * matrix[1][2] * matrix[2][3] * matrix[3][1] +
				matrix[0][3] * matrix[1][1] * matrix[2][0] * matrix[3][2] - matrix[0][1] * matrix[1][3] * matrix[2][0] * matrix[3][2] -
				matrix[0][3] * matrix[1][0] * matrix[2][1] * matrix[3][2] + matrix[0][0] * matrix[1][3] * matrix[2][1] * matrix[3][2] +
				matrix[0][1] * matrix[1][0] * matrix[2][3] * matrix[3][2] - matrix[0][0] * matrix[1][1] * matrix[2][3] * matrix[3][2] -
				matrix[0][2] * matrix[1][1] * matrix[2][0] * matrix[3][3] + matrix[0][1] * matrix[1][2] * matrix[2][0] * matrix[3][3] +
				matrix[0][2] * matrix[1][0] * matrix[2][1] * matrix[3][3] - matrix[0][0] * matrix[1][2] * matrix[2][1] * matrix[3][3] -
				matrix[0][1] * matrix[1][0] * matrix[2][2] * matrix[3][3] + matrix[0][0] * matrix[1][1] * matrix[2][2] * matrix[3][3];
		}

		/** Returns the determinant of this matrix. */
		float Determinant() {
			return Mat4::Determinant(*this);
		}

		/**
		 * Tries to invert a matrix.
		 *
		 * Code shamelessly copied from stackoverflow:
		 * https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
		 * 
		 * @param matrix is the matrix to be inverted.
		 * @param invertableOut is a pointer to the resulting inverted matrix.
		 * 
		 * @returns if matrix was successfully inverted.
		 */
		static bool Inverse(Mat4 matrix, Mat4* invertedOut) {
			float det = matrix.Determinant();

			// Sanity check for non invertable matrix.
			if (det == 0) {
				return false;
			}

			Mat4 m;

			m[0][0] = matrix[1][1] * matrix[2][2] * matrix[3][3] -
				matrix[1][1] * matrix[2][3] * matrix[3][2] -
				matrix[2][1] * matrix[1][2] * matrix[3][3] +
				matrix[2][1] * matrix[1][3] * matrix[3][2] +
				matrix[3][1] * matrix[1][2] * matrix[2][3] -
				matrix[3][1] * matrix[1][3] * matrix[2][2];

			m[1][0] = -matrix[1][0] * matrix[2][2] * matrix[3][3] +
				matrix[1][0] * matrix[2][3] * matrix[3][2] +
				matrix[2][0] * matrix[1][2] * matrix[3][3] -
				matrix[2][0] * matrix[1][3] * matrix[3][2] -
				matrix[3][0] * matrix[1][2] * matrix[2][3] +
				matrix[3][0] * matrix[1][3] * matrix[2][2];

			m[2][0] = matrix[1][0] * matrix[2][1] * matrix[3][3] -
				matrix[1][0] * matrix[2][3] * matrix[3][1] -
				matrix[2][0] * matrix[1][1] * matrix[3][3] +
				matrix[2][0] * matrix[1][3] * matrix[3][1] +
				matrix[3][0] * matrix[1][1] * matrix[2][3] -
				matrix[3][0] * matrix[1][3] * matrix[2][1];

			m[3][0] = -matrix[1][0] * matrix[2][1] * matrix[3][2] +
				matrix[1][0] * matrix[2][2] * matrix[3][1] +
				matrix[2][0] * matrix[1][1] * matrix[3][2] -
				matrix[2][0] * matrix[1][2] * matrix[3][1] -
				matrix[3][0] * matrix[1][1] * matrix[2][2] +
				matrix[3][0] * matrix[1][2] * matrix[2][1];

			m[0][1] = -matrix[0][1] * matrix[2][2] * matrix[3][3] +
				matrix[0][1] * matrix[2][3] * matrix[3][2] +
				matrix[2][1] * matrix[0][2] * matrix[3][3] -
				matrix[2][1] * matrix[0][3] * matrix[3][2] -
				matrix[3][1] * matrix[0][2] * matrix[2][3] +
				matrix[3][1] * matrix[0][3] * matrix[2][2];

			m[1][1] = matrix[0][0] * matrix[2][2] * matrix[3][3] -
				matrix[0][0] * matrix[2][3] * matrix[3][2] -
				matrix[2][0] * matrix[0][2] * matrix[3][3] +
				matrix[2][0] * matrix[0][3] * matrix[3][2] +
				matrix[3][0] * matrix[0][2] * matrix[2][3] -
				matrix[3][0] * matrix[0][3] * matrix[2][2];

			m[2][1] = -matrix[0][0] * matrix[2][1] * matrix[3][3] +
				matrix[0][0] * matrix[2][3] * matrix[3][1] +
				matrix[2][0] * matrix[0][1] * matrix[3][3] -
				matrix[2][0] * matrix[0][3] * matrix[3][1] -
				matrix[3][0] * matrix[0][1] * matrix[2][3] +
				matrix[3][0] * matrix[0][3] * matrix[2][1];

			m[3][1] = matrix[0][0] * matrix[2][1] * matrix[3][2] -
				matrix[0][0] * matrix[2][2] * matrix[3][1] -
				matrix[2][0] * matrix[0][1] * matrix[3][2] +
				matrix[2][0] * matrix[0][2] * matrix[3][1] +
				matrix[3][0] * matrix[0][1] * matrix[2][2] -
				matrix[3][0] * matrix[0][2] * matrix[2][1];

			m[0][2] = matrix[0][1] * matrix[1][2] * matrix[3][3] -
				matrix[0][1] * matrix[1][3] * matrix[3][2] -
				matrix[1][1] * matrix[0][2] * matrix[3][3] +
				matrix[1][1] * matrix[0][3] * matrix[3][2] +
				matrix[3][1] * matrix[0][2] * matrix[1][3] -
				matrix[3][1] * matrix[0][3] * matrix[1][2];

			m[1][2] = -matrix[0][0] * matrix[1][2] * matrix[3][3] +
				matrix[0][0] * matrix[1][3] * matrix[3][2] +
				matrix[1][0] * matrix[0][2] * matrix[3][3] -
				matrix[1][0] * matrix[0][3] * matrix[3][2] -
				matrix[3][0] * matrix[0][2] * matrix[1][3] +
				matrix[3][0] * matrix[0][3] * matrix[1][2];

			m[2][2] = matrix[0][0] * matrix[1][1] * matrix[3][3] -
				matrix[0][0] * matrix[1][3] * matrix[3][1] -
				matrix[1][0] * matrix[0][1] * matrix[3][3] +
				matrix[1][0] * matrix[0][3] * matrix[3][1] +
				matrix[3][0] * matrix[0][1] * matrix[1][3] -
				matrix[3][0] * matrix[0][3] * matrix[1][1];

			m[3][2] = -matrix[0][0] * matrix[1][1] * matrix[3][2] +
				matrix[0][0] * matrix[1][2] * matrix[3][1] +
				matrix[1][0] * matrix[0][1] * matrix[3][2] -
				matrix[1][0] * matrix[0][2] * matrix[3][1] -
				matrix[3][0] * matrix[0][1] * matrix[1][2] +
				matrix[3][0] * matrix[0][2] * matrix[1][1];

			m[0][3] = -matrix[0][1] * matrix[1][2] * matrix[2][3] +
				matrix[0][1] * matrix[1][3] * matrix[2][2] +
				matrix[1][1] * matrix[0][2] * matrix[2][3] -
				matrix[1][1] * matrix[0][3] * matrix[2][2] -
				matrix[2][1] * matrix[0][2] * matrix[1][3] +
				matrix[2][1] * matrix[0][3] * matrix[1][2];

			m[1][3] = matrix[0][0] * matrix[1][2] * matrix[2][3] -
				matrix[0][0] * matrix[1][3] * matrix[2][2] -
				matrix[1][0] * matrix[0][2] * matrix[2][3] +
				matrix[1][0] * matrix[0][3] * matrix[2][2] +
				matrix[2][0] * matrix[0][2] * matrix[1][3] -
				matrix[2][0] * matrix[0][3] * matrix[1][2];

			m[2][3] = -matrix[0][0] * matrix[1][1] * matrix[2][3] +
				matrix[0][0] * matrix[1][3] * matrix[2][1] +
				matrix[1][0] * matrix[0][1] * matrix[2][3] -
				matrix[1][0] * matrix[0][3] * matrix[2][1] -
				matrix[2][0] * matrix[0][1] * matrix[1][3] +
				matrix[2][0] * matrix[0][3] * matrix[1][1];

			m[3][3] = matrix[0][0] * matrix[1][1] * matrix[2][2] -
				matrix[0][0] * matrix[1][2] * matrix[2][1] -
				matrix[1][0] * matrix[0][1] * matrix[2][2] +
				matrix[1][0] * matrix[0][2] * matrix[2][1] +
				matrix[2][0] * matrix[0][1] * matrix[1][2] -
				matrix[2][0] * matrix[0][2] * matrix[1][1];

			// Scale matrix by the inverse of the determinant.
			m = m * (1.0f / det);

			// Done with matrix, send it out to the world
			*invertedOut = m;

			// Return true indicating successful inverting of matrix.
			return true;
		}

		/** Creates a translation matrix. */
		static Mat4 Translate(float x, float y, float z) {
			return Mat4(
				1, 0, 0, x,
				0, 1, 0, y,
				0, 0, 1, z,
				0, 0, 0, 1				
			);
		}

		/** Creates a scaling matrix. */
		static Mat4 Scale(float x, float y, float z) {
			return Mat4(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1
			);
		}

		/** Creates lookat matrix. */
		static Mat4 LookAt(const Vec4& eye, const Vec4& target, const Vec4& up) {
			// Quickhack for const params.
			MathLib::Vec4 forwards(
				eye[0] - target[0],
				eye[1] - target[1],
				eye[2] - target[2]
			);
			// Normalized forwards vector.
			MathLib::Vec4 f = forwards.Normalize();

			// Left vector.
			MathLib::Vec4 left = MathLib::Vec4::CrossProduct(up, forwards);
			// Normalized left vector.
			MathLib::Vec4 l = left.Normalize();

			// Cross up vector.
			MathLib::Vec4 u = MathLib::Vec4::CrossProduct(f, l);

			// Not sure if needed but I'm just that badass.
			l[3] = 0;
			u[3] = 0;
			f[3] = 0;

			// Final camera rotation matrix.
			MathLib::Mat4 rot(l, u, f, MathLib::Vec4(0, 0, 0, 1));

			// Move matrix for camera.
			MathLib::Mat4 move = MathLib::Mat4::Translate(-eye[0], -eye[1], -eye[2]);

			// Return final lookat matrix.
			return rot * move;
		}

		/**
		 * Creates a perspective matrix.
		 *
		 * @param aspect is the screen's width divided by its height.
		 */
		static Mat4 Perspective(float near, float far, float angle, float aspect) {
			// Near, far and angle in degrees.
			float n = near, f = far, a = angle;
			// Creating the physical half height of the camera.
			float tmp = tan(a / 2) * n;
			float b = -tmp;				// Bottom.
			float t = tmp;				// Top.
			float l = -tmp * aspect;	// Left.
			float r = -l;				// Right.

			return MathLib::Mat4(
				(2 * n) / (r - l),	0,					(r + l),			0,
				0,					(2 * n) / (t - b),	(t + b) / (t - b),	0,
				0,					0,					-(f + n) / (f - n),	-(2 * f * n) / (f - n),
				0,					0,					-1,					0
			);
		}

		void Print() {
			this->columns[0].Print();
			this->columns[1].Print();
			this->columns[2].Print();
			this->columns[3].Print();
			printf("\n");
		}

	private:
		/** The columns of this matrix. */
		Vec4 columns[4];
	};
}

#ifndef _QMATRIX_H_
#define _QMATRIX_H_

#include "util.h"

//represent the l * l kernel matrix
class QMatrix {
	public:
		QMatrix(int l, svm_node * const *x, const svm_parameter *param);
		virtual ~QMatrix();
		// get one column from the Q Matrix
		virtual float *get_QM(int column, int len) const = 0;
		// get the Q Matrix[i, i]
		virtual double *get_QMD() const = 0;
		// swap the index of i and j
		virtual void swap_index(int i, int j) const = 0;
		// kernel function
		static double k_function(const svm_node *x, const svm_node *y,
				const svm_parameter *param);
	protected:
		double (QMatrix::*kernel_function)(int i, int j) const;
	private:
		// data pointer
		const svm_node **x;
		double *x_square;

		// kernel parameters
		const int kernel_type;
		const int degree;
		const double gamma;
		const double coef0;

		// px * py
		static double dot(const svm_node *x, const svm_node *y);

		// kernel functions
		double kernel_linear(int i, int j) const {
			return dot(x[i], x[j]);
		}
		double kernel_poly(int i, int j) const {
			return powi(gamma * dot(x[i], x[j]) + coef0, degree);
		}
		double kernel_rbf(int i, int j) const {
			return exp(-gamma * (x_square[i] + x_square[j] - 2 * dot(x[i], x[j])));
		}
		double kernel_sigmoid(int i, int j) const {
			return tanh(gamma * dot(x[i],x[j]) + coef0);
		}
		double kernel_precomputed(int i, int j) const {
			return x[i][(int)x[j][0].value].value;
		}
};

#endif

#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <climits>
using namespace std;

// signed char type
#define INF HUGE_VAL
#define TAU 1e-12

typedef signed char schar;

template <class S, class T>
inline void clone(T*& dst, S* src, int n) {
	dst = new T[n];
	memcpy((void *)dst, (void *)src, sizeof(T) * n);
}

inline double powi(double base, int times) {
	double _temp = base, _result = 1.0;

	for (int t = times; t > 0; t /= 2) {
		if (t % 2 == 1) {
			_result *= _temp;
		}
		_temp = _temp * _temp;
	}
	return _result;
}

#ifdef __cplusplus
extern "C" {
#endif

// svm problem data
struct svm_node {
	int index;
	double value;
};
struct svm_problem {
	int l; // data length
	double *y; // class label
	struct svm_node **x; // data
};

// svm parameters
enum {C_SVC, NU_SVC};// svm type
enum {LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED}; // kernel type
struct svm_parameter {
	int svm_type;
	int kernel_type;
	int degree; // for poly
	double gamma; // for poly/rbf/sigmoid
	double coef0; // for poly/sigmoid

	double cache_size; // in MB
	double eps; // stopping criteria
	double C; // for C_SVC
	double nu; // for nu-SVC, one-class SVM, and nu-SVR
	int nr_weight; // for C_SVC
	int *weight_label; // for C_SVC
	double *weight; // for C_SVC

	int shrinking; // use the shrinking heuristics
};

struct decision_function {
	double *alpha;
	double rho;
};

// svm model
struct svm_model {
	struct svm_parameter param;//parameters
	int nr_class;//number of classes
	int *class_label;//label of each class
	double *rho;
	int *nSV; //number of SVs for each class
	int total_sv;// total number of SVs
	struct svm_node **SV;//SVs
	double **sv_coef;//coefficients for SVs in decision functions
};

#ifdef __cplusplus
}
#endif

#endif

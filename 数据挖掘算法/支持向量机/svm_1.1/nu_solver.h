#ifndef _NU_SOLVER_H_
#define NU_SOLVER_H_

#include "solver.h"

// Solver for nu-svm classification and regression
//
// additional constraint: e^T * \alpha = constant
//
class NU_Solver:public Solver {
	public:
		// Q, p, Cp, Cn and an initial feasible point \alpha
		// l is the size of vectors and matrics
		// eps is the stopping tolerance
		// shrinking is label of using shrinking mechanism or not
		NU_Solver(int l, const QMatrix *Q, const double *p, const schar *y,
				double *alpha, double Cp, double Cn, double eps, int shrinking)
			:Solver(l, Q, p, y, alpha, Cp, Cn, eps, shrinking) { }
		~NU_Solver() { }

		// r
		double r;
	private:
		// select two working set
		int select_working_set(int &i, int &j);

		// calculate_rho
		double calculate_rho();

		// shrinking
		bool shrinkable(int i, double Gmax1, double Gmax2, double Gmax3, double Gmax4);
		void do_shrinking();
};

#endif

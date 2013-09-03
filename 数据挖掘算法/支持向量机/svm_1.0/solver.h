#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "svc_qmatrix.h"

//
// An SMO algorithm to solve the following question:
//
// min 0.5 * (\alpha^T * Q * \alpha) + p^T * \alpha
// s.t.    y^T * \alpha = \delta
//         y_i = +1 or -1
//         0 <= alpha_i <= Cp for y_i = +1
//         0 <= alpha_i <= Cn for y_i = -1
//
class Solver {
	public:
		// Q, p, Cp, Cn and an initial feasible point \alpha
		// l is the size of vectors and matrics
		// eps is the stopping tolerance
		// shrinking is label of using shrinking mechanism or not
		Solver(int l, const QMatrix *Q, const double *p_, const schar *y_, 
				double *alpha_, double Cp, double Cn, double eps, int shrinking);
		virtual ~Solver();

		// solve the c-svc question
		void solve(struct decision_function *function);
	protected:
		int l;
		const QMatrix *QM;
		const double *QMD;
		double *p;
		schar *y;
		
		double *alpha;
		enum {LOWER_BOUND, UPPER_BOUND, FREE};
		char *alpha_status;

		double Cp, Cn;
		double eps;
		int shrinking;

		// active set
		int active_size;
		int *active_set;

		// gradient
		double *G;
		double *G_bar;

		// the first time to arrive 10eps 
		bool first_10eps;
		
		// alpha status
		double get_C(int i) {
			return y[i] > 0 ? Cp : Cn;
		}
		void update_alpha_status(int i) {
			if (alpha[i] >= get_C(i)) {
				alpha_status[i] = UPPER_BOUND;
			} else if (alpha[i] <= 0) {
				alpha_status[i] = LOWER_BOUND;
			} else {
				alpha_status[i] = FREE;
			}
		}

		bool is_upper_bound(int i) {
			return alpha_status[i] == UPPER_BOUND;
		}
		bool is_lower_bound(int i) {
			return alpha_status[i] == LOWER_BOUND;
		}
		bool is_free(int i) {
			return alpha_status[i] == FREE;
		}

		// swap the content of the index of i and j
		void swap_index(int i, int j);

		// reconstruct gradient
		void reconstruct_gradient();

		// select two working set
		virtual int select_working_set(int &i, int &j);

		// calculate rho
		virtual double calculate_rho();

		// shrinking
		virtual bool shrinkable(int i, double Gmax1, double Gmax2);
		virtual void do_shrinking();
};

#endif

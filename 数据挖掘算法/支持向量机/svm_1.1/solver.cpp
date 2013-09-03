#include "solver.h"

Solver::Solver(int l_, const QMatrix *QM_, const double *p_, const schar *y_,
		double *alpha_, double Cp_, double Cn_, double eps_, int shrinking_) {
	l = l_;
	QM = QM_;
	QMD = QM->get_QMD();
	clone(p, p_, l_);
	clone(y, y_, l_);
	clone(alpha, alpha_, l_);
	Cp = Cp_;
	Cn = Cn_;
	eps = eps_;
	shrinking = shrinking_;

	first_10eps = false;

	int i;
	//initialize alpha_status
	alpha_status = new char[l];
	for (i = 0; i < l; i++) {
		update_alpha_status(i);
	}

	//initialize active set
	active_set = new int[l];
	for (i = 0; i < l; i++) {
		active_set[i] = i;
	}
	active_size = l;

	//initialize gradient
	G = new double[l];
	G_bar = new double[l];
	for (i = 0; i < l; i++) {
		G_bar[i] = 0;
		G[i] = p[i];
	}
	for (i = 0; i < l; i++) {
		if (!is_lower_bound(i)) {
			const float *QM_i = QM->get_QM(i, l);
			double alpha_i = alpha[i];
			double C_i = get_C(i);
			int j;
			for (j = 0; j < l; j++) {
				G[j] += alpha_i * QM_i[j];
			}
			if (is_upper_bound(i)) {
				for (j = 0; j < l; j++) {
					G_bar[j] += C_i * QM_i[j];
				}
			}
		}
	}
}

Solver::~Solver() {
	delete []p;
	delete []y;
	delete []alpha;
	delete []alpha_status;
	delete []active_set;
	delete []G;
	delete []G_bar;
}

void Solver::swap_index(int i, int j) {
	QM->swap_index(i, j);
	swap(y[i], y[j]);
	swap(G[i], G[j]);
	swap(alpha_status[i], alpha_status[j]);
	swap(alpha[i], alpha[j]);
	swap(p[i], p[j]);
	swap(active_set[i], active_set[j]);
	swap(G_bar[i], G_bar[j]);
}

// return 1 if already optimal, return 0 otherwise
int Solver::select_working_set(int &out_i, int &out_j) {
	double m_alpha = -INF;
	int m_alpha_index = -1;
	int t;
	for (t = 0; t < active_size; t++) {
		if (y[t] == +1) {
			if (!is_upper_bound(t)) {
				if (-G[t] >= m_alpha) {
					m_alpha = -G[t];
					m_alpha_index = t;
				}
			}
		} else {
			if (!is_lower_bound(t)) {
				if (G[t] >= m_alpha) {
					m_alpha = G[t];
					m_alpha_index = t;
				}
			}
		}
	}

	const float *QM_i = QM->get_QM(m_alpha_index, active_size);
	double M_alpha = INF;
	int M_alpha_index = -1;
	double obj_diff_min = INF;
	for (t = 0; t < active_size; t++) {
		if (y[t] == +1) {
			if (!is_lower_bound(t)) {
				double grad_diff = m_alpha + G[t];
				if (-G[t] < M_alpha) {
					M_alpha = -G[t];
				}
				if (grad_diff > 0) {
					double obj_diff;
					double quad_coef = QMD[m_alpha_index] + QMD[t] - 2.0 * y[m_alpha_index] * QM_i[t];
					if (quad_coef > 0) {
						obj_diff = -(grad_diff * grad_diff) / quad_coef;
					} else {
						obj_diff = -(grad_diff * grad_diff) / TAU;
					}

					if (obj_diff <= obj_diff_min) {
						M_alpha_index = t;
						obj_diff_min = obj_diff;
					}
				}
			}
		} else {
			if (!is_upper_bound(t)) {
				double grad_diff = m_alpha - G[t];
				if (G[t] <= M_alpha) {
					M_alpha = G[t];
				}
				if (grad_diff > 0) {
					double obj_diff;
					double quad_coef = QMD[m_alpha_index] + QMD[t] + 2.0 * y[m_alpha_index] * QM_i[t];
					if (quad_coef > 0) {
						obj_diff = -(grad_diff * grad_diff) / quad_coef;
					} else {
						obj_diff = -(grad_diff * grad_diff) / TAU;
					}

					if (obj_diff <= obj_diff_min) {
						M_alpha_index = t;
						obj_diff_min = obj_diff;
					}
				}
			}
		}
	}

	if (m_alpha - M_alpha <= eps) {
		return 1;
	}

	out_i = m_alpha_index;
	out_j = M_alpha_index;

	return 0;
}

// reconstruct inactive element of G from G_bar and free variables
void Solver::reconstruct_gradient() {
	if (active_size == l) {
		return;
	}

	int i;
	for (i = active_size; i < l; i++) {
		G[i] = G_bar[i] + p[i];
	}

	int nr_free = 0;
	for (i = 0; i < active_size; i++) {
		if (is_free(i)) {
			nr_free++;
		}
	}

	if (2 * nr_free < active_size) {
		//cout << "WARNING: using -h 0 maybe faster\n" << endl;
		shrinking = 0;
	}

	if (nr_free * l > 2 * active_size * (l - active_size)) {
		for (i = active_size; i < l; i++) {
			const float *QM_i = QM->get_QM(i, active_size);
			for (int j = 0; j < active_size; j++) {
				if (is_free(j)) {
					G[i] += alpha[j] * QM_i[j];
				}
			}
		}
	} else {
		for (i = 0; i < active_size; i++) {
			if (is_free(i)) {
				const float *QM_i = QM->get_QM(i, l);
				double alpha_i = alpha[i];
				for (int j = active_size; j < l; j++) {
					G[j] += alpha_i * QM_i[j];
				}
			}
		}
	}
}

bool Solver::shrinkable(int i, double m_alpha, double M_alpha) {
	if (is_upper_bound(i)) {
		if (y[i] == +1) {
			return -G[i] > m_alpha; 
		} else {
			return G[i] < M_alpha;
		}
	} else if (is_lower_bound(i)) {
		if (y[i] == +1) {
			return -G[i] < M_alpha;
		} else {
			return G[i] > m_alpha;
		}
	} else {
		return false;
	}
}

void Solver::do_shrinking() {
	double m_alpha = -INF;
	double M_alpha = INF;
	for (int t = 0; t < active_size; t++) {
		if (y[t] == +1) {
			if (!is_upper_bound(t)) {
				if (-G[t] >= m_alpha) {
					m_alpha = -G[t];
				}
			}
			if (!is_lower_bound(t)) {
				if (-G[t] <= M_alpha) {
					M_alpha = -G[t];
				}
			}
		} else {
			if (!is_lower_bound(t)) {
				if (G[t] >= m_alpha) {
					m_alpha = G[t];
				}
			}
			if (!is_upper_bound(t)) {
				if (G[t] <= M_alpha) {
					M_alpha = G[t];
				}
			}
		}
	}

	if (!first_10eps && m_alpha - M_alpha <= 10 * eps) {
		first_10eps = true;
		reconstruct_gradient();
		active_size = l;
	}

	for (int i = 0; i < active_size; i++) {
		if (shrinkable(i, m_alpha, M_alpha)) {
			active_size--;
			while (active_size > i) {
				if (!shrinkable(active_size, m_alpha, M_alpha)) {
					swap_index(i, active_size);
					break;
				}
				active_size--;
			}
		}
	}
}

double Solver::calculate_rho() {
	 int nr_free = 0;
	 double ub = INF, lb = -INF, sum_free = 0;
	 for (int i = 0; i < active_size; i++) {
		 double yG = y[i] * G[i];

		 if (is_upper_bound(i)) {
			 if (y[i] == -1) {
				 ub = min(ub, yG);
			 } else {
				 lb = max(lb, yG);
			 }
		 } else if (is_lower_bound(i)) {
			 if (y[i] == +1) {
				 ub = min(ub, yG);
			 } else {
				 lb = max(lb, yG);
			 }
		 } else {
			 nr_free++;
			 sum_free += yG;
		 }
	 }

	 if (nr_free > 0) {
		 return sum_free / nr_free;
	 } else {
		 return (ub + lb) / 2;
	 }
}

void Solver::solve(struct decision_function *function) {
	int iter = 0;
	int max_iter = (l > INT_MAX/100 ? INT_MAX : 100*l);
	int count = min(l, 1000) + 1; 

	while (iter < max_iter) {
		iter++;

		//do shrinking if --count==0
		if (--count == 0) {
			count = min(l, 1000);
			if (shrinking) {
				do_shrinking();
			}
		}

		//select working set
		int i, j;
		if (select_working_set(i, j) != 0) {
			reconstruct_gradient();
			active_size = l;
			//check if it is optimal for all
			if (select_working_set(i, j) != 0) {
				break;
			} else {
				count = 1; //do shrinking next iteration
			}
		}

		// update alpha[i] and alpha[j]
		const float *QM_i = QM->get_QM(i, active_size);
		const float *QM_j = QM->get_QM(j, active_size);
		double C_i = get_C(i);
		double C_j = get_C(j);

		double old_alpha_i = alpha[i];
		double old_alpha_j = alpha[j];

		if (y[i] != y[j]) {
			double quad_coef = QMD[i] + QMD[j] + 2 * QM_i[j];
			if (quad_coef <= 0) {
				quad_coef = TAU;
			}
			double delta = (-G[i]-G[j]) / quad_coef;
			double diff = alpha[i] - alpha[j];
			alpha[i] += delta;
			alpha[j] += delta;

			if (diff > 0) {
				if (alpha[j] < 0) {
					alpha[j] = 0;
					alpha[i] = diff;
				}
			} else {
				if (alpha[i] < 0) {
					alpha[i] = 0;
					alpha[j] = -diff;
				}
			}
			if (diff > C_i -C_j) {
				if (alpha[i] > C_i) {
					alpha[i] = C_i;
					alpha[j] = C_i - diff;
				}
			} else {
				if (alpha[j] > C_j) {
					alpha[j] = C_j;
					alpha[i] = C_j + diff;
				}
			}
		} else {
			double quad_coef = QMD[i] + QMD[j] - 2 * QM_i[j];
			if (quad_coef <= 0) {
				quad_coef = TAU;
			}
			double delta = (G[i] - G[j]) / quad_coef;
			double sum = alpha[i] + alpha[j];
			alpha[i] -= delta;
			alpha[j] += delta;

			if (sum > C_i) {
				if (alpha[i] > C_i) {
					alpha[i] = C_i;
					alpha[j] = sum - C_i;
				}
			} else {
				if (alpha[j] < 0) {
					alpha[j] = 0;
					alpha[i] = sum;
				}
			}
			if (sum > C_j) {
				if (alpha[j] > C_j) {
					alpha[j] = C_j;
					alpha[i] = sum - C_j;
				}
			} else {
				if (alpha[i] < 0) {
					alpha[i] = 0;
					alpha[j] = sum;
				}
			}
		}

		//update G
		double delta_alpha_i = alpha[i] - old_alpha_i;
		double delta_alpha_j = alpha[j] - old_alpha_j;
		for (int t = 0; t < active_size; t++) {
			G[t] += QM_i[t] * delta_alpha_i + QM_j[t] * delta_alpha_j;
		}

		//update alpha_status and G_bar
		bool ui = is_upper_bound(i);
		bool uj = is_upper_bound(j);
		update_alpha_status(i);
		update_alpha_status(j);

		int k = 0;
		if (ui != is_upper_bound(i)) {
			QM_i = QM->get_QM(i, l);
			if (ui) {
				for (k = 0; k < l; k++) {
					G_bar[k] -= C_i * QM_i[k];
				}
			} else {
				for (k = 0; k < l; k++) {
					G_bar[k] += C_i * QM_i[k];
				}
			}
		}
		if (uj != is_upper_bound(j)) {
			QM_j = QM->get_QM(j, l);
			if (uj) {
				for (k = 0; k < l; k++) {
					G_bar[k] -= C_j * QM_j[k];
				}
			} else {
				for (k = 0; k < l; k++) {
					G_bar[k] += C_j * QM_j[k];
				}
			}
		}
	}

	if (iter >= max_iter) {
		if (active_size < l) {
			reconstruct_gradient();
			active_size = l;
		}
		cout << "WARNING: reaching max number of iterations." << endl;
	}

	// put back the solution
	for (int i = 0; i < l; i++) {
		function->alpha[active_set[i]] = alpha[i];
	}

	function->rho = calculate_rho();
}

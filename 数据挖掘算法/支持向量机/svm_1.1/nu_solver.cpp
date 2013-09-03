#include "nu_solver.h"

int NU_Solver::select_working_set(int &out_i, int &out_j) {
	double mp_alpha = -INF;
	int mp_alpha_index = -1;
	double mn_alpha = -INF;
	int mn_alpha_index = -1;
	int t;
	for (t = 0; t < active_size; t++) {
		if (y[t] == +1) {
			if (!is_upper_bound(t)) {
				if (-G[t] >= mp_alpha) {
					mp_alpha = -G[t];
					mp_alpha_index = t;
				}
			}
		} else {
			if (!is_lower_bound(t)) {
				if (G[t] >= mn_alpha) {
					mn_alpha = G[t];
					mn_alpha_index = t;
				}
			}
		}
	}

	const float *QM_ip = NULL;
	const float *QM_in = NULL;
	if (mp_alpha_index != -1) {
		QM_ip = QM->get_QM(mp_alpha_index, active_size);
	}
	if (mn_alpha_index != -1) {
		QM_in = QM->get_QM(mn_alpha_index, active_size);
	}

	double Mp_alpha = INF;
	double Mn_alpha = INF;
	int M_alpha_index = -1;
	double obj_diff_min = INF;
	for (t = 0; t < active_size; t++) {
		if (y[t] == +1) {
			if (!is_lower_bound(t)) {
				double grad_diff = mp_alpha + G[t];
				if (-G[t] <= Mp_alpha) {
					Mp_alpha = -G[t];
				}
				if (grad_diff > 0) {
					double obj_diff;
					double quad_coef = QMD[mp_alpha_index] + QMD[t] - 2.0 * y[mp_alpha_index] * QM_ip[t];
					if (quad_coef > 0) {
						obj_diff = -(grad_diff * grad_diff) / quad_coef;
					} else {
						obj_diff = -(grad_diff * grad_diff) / TAU;
					}

					if (obj_diff <= obj_diff_min) {
						obj_diff_min = obj_diff;
						M_alpha_index = t;
					}
				}
			}
		} else {
			if (!is_upper_bound(t)) {
				double grad_diff = mn_alpha -G[t];
				if (G[t] <= Mn_alpha) {
					Mn_alpha = G[t];
				}
				if (grad_diff > 0) {
					double obj_diff;
					double quad_coef = QMD[mn_alpha_index] + QMD[t] + 2.0 * y[mn_alpha_index] * QM_in[t];
					if (quad_coef > 0) {
						obj_diff = -(grad_diff * grad_diff) / quad_coef;
					} else {
						obj_diff = -(grad_diff * grad_diff) / TAU;
					}

					if (obj_diff <= obj_diff_min) {
						obj_diff_min = obj_diff;
						M_alpha_index = t;
					}
				}
			}
		}
	}

	if (max(mp_alpha - Mp_alpha, mn_alpha - Mn_alpha) < eps) {
		return 1;
	}

	if (y[M_alpha_index] == +1) {
		out_i = mp_alpha_index;
	} else {
		out_i = mn_alpha_index;
	}

	out_j = M_alpha_index;

	return 0;
}

bool NU_Solver::shrinkable(int i, double mp_alpha, double Mp_alpha, double mn_alpha, double Mn_alpha) {
	if (is_upper_bound(i)) {
		if (y[i] == +1) {
			return -G[i] > mp_alpha;
		} else {
			return G[i] < Mn_alpha;
		}
	} else if (is_lower_bound(i)) {
		if (y[i] == +1) {
			return -G[i] < Mp_alpha;
		} else {
			return G[i] > mn_alpha;
		}
	} else {
		return false;
	}
}

void NU_Solver::do_shrinking() {
	double mp_alpha = -INF;
	double Mp_alpha = INF;
	double mn_alpha = -INF;
	double Mn_alpha = INF;

	int t;
	for (t = 0; t < active_size; t++) {
		if (!is_upper_bound(t)) {
			if (y[t] == +1) {
				if (-G[t] >= mp_alpha) {
					mp_alpha = -G[t];
				}
			} else {
				if (G[t] <= Mn_alpha) {
					Mn_alpha = G[t];
				}
			}
		}

		if (!is_lower_bound(t)) {
			if (y[t] == +1) {
				if (-G[t] <= Mp_alpha) {
					Mp_alpha = -G[t];
				}
			} else {
				if (G[t] >= mn_alpha) {
					mn_alpha = G[t];
				}
			}
		}
	}

	if (!first_10eps && max(mp_alpha - Mp_alpha, mn_alpha - Mn_alpha) <= eps * 10) {
		first_10eps = true;
		reconstruct_gradient();
		active_size = l;
	}

	for (int i = 0; i < active_size; i++) {
		if (shrinkable(i, mp_alpha, Mp_alpha, mn_alpha, Mn_alpha)) {
			active_size--;
			while (active_size > i) {
				if (!shrinkable(active_size, mp_alpha, Mp_alpha, mn_alpha, Mn_alpha)) {
					swap_index(i, active_size);
					break;
				}
				active_size--;
			}
		}
	}
}

double NU_Solver::calculate_rho() {
	int nr_free1 = 0, nr_free2 = 0;
	double ub1 = INF, ub2 = INF;
	double lb1 = -INF, lb2 = -INF;
	double sum_free1 = 0, sum_free2 = 0;

	for (int i = 0; i < active_size; i++) {
		if (y[i] == +1) {
			if (is_upper_bound(i)) {
				lb1 = max(lb1, G[i]);
			} else if (is_lower_bound(i)) {
				ub1 = min(ub1, G[i]);
			} else {
				nr_free1++;
				sum_free1 += G[i];
			}
		} else {
			if (is_upper_bound(i)) {
				lb2 = max(lb2, G[i]);
			} else if (is_lower_bound(i)) {
				ub2 = min(ub2, G[i]);
			} else {
				nr_free2++;
				sum_free2 += G[i];
			}
		}
	}

	double r1, r2;
	if (nr_free1 > 0) {
		r1 = sum_free1 / nr_free1;
	} else {
		r1 = (ub1 + lb2) / 2;
	}

	if (nr_free2 > 0) {
		r2 = sum_free2 / nr_free2;
	} else {
		r2 = (ub2 + lb2) / 2;
	}

	r = (r1 + r2) / 2;

	return (r1 - r2) / 2;
}

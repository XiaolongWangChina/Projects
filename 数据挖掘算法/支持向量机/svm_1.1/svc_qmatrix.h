#ifndef _SVC_QMATRIX_H_
#define _SVC_QMATRIX_H_

#include "qmatrix.h"
#include "cache.h"
#include "util.h"

// QMatrix for SVC problems
class SVC_QMatrix:public QMatrix {
	public:
		SVC_QMatrix(const svm_problem *prob, const svm_parameter *param, const schar *y_)
			:QMatrix(prob->l, prob->x, param) {
				clone(y, y_, prob->l);
				cache = new Cache(prob->l, (long int)param->cache_size * (1 << 20));
				QMD = new double[prob->l];
				for (int i = 0; i < prob->l; i++) {
					QMD[i] = (this->*kernel_function)(i, i);
				}
			}

		~SVC_QMatrix() {
			delete []y;
			delete cache;
			delete []QMD;
		}

		float *get_QM(int i, int len) const {
			float *data;
			int start;
			if ((start = cache->get_data(i, &data, len)) < len) {
				// fill in the uncalculated data
				for (int j = start; j < len; j++) {
					data[j] = (float)(y[i] * y[j] * (this->*kernel_function)(i, j));
				}
			}
			return data;
		}

		double *get_QMD() const {
			return QMD;
		}

		void swap_index(int i, int j) const {
			QMatrix::swap_index(i, j);
			swap(y[i], y[j]);
			cache->swap_index(i, j);
			swap(QMD[i], QMD[j]);
		}
	private:
		schar *y;
		Cache *cache;
		double *QMD;
};

#endif

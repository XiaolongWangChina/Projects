#include "svm.h"
#include "solver.h"
#include "nu_solver.h"

SVM::SVM(struct svm_problem *prob_, struct svm_parameter *param_) {
	prob = prob_;
	param = param_;
	model = NULL;
}

SVM::~SVM() {
	//prob
	free(prob->x);
	free(prob->y);
	free(prob);

	//param
	free(param->weight_label);
	free(param->weight);
	free(param);

	if (model != NULL) {
		//model
		for (int i = 0; i < model->nr_class - 1; i++) {
			free(model->sv_coef[i]);
		}
		free(model->SV);
		free(model->sv_coef);
		free(model->class_label);
		free(model->rho);
		free(model->nSV);
		free(model);
	}
}

void SVM::setParamCg(int C_, int gamma_) {
	param->C = C_;
	param->gamma = gamma_;
}

//group data of the same class together
void SVM::svm_group_classes(const struct svm_problem *prob, int *nr_class_ret, int **label_ret,
		int **class_start_ret, int **class_count_ret, int *reverse_index_ret) {
	int l = prob->l;
	int nr_class = 0;
	int max_nr_class = 16;

	int *label = (int *)malloc(sizeof(int) * max_nr_class);
	int *class_count = (int *)malloc(sizeof(int) * max_nr_class);
	int *data_label = (int *)malloc(sizeof(int) * l);

	for (int i = 0; i < l; i++) {
		int this_label = (int)prob->y[i];
		int j;
		for (j = 0; j < nr_class; j++) {
			if (this_label == label[j]) {
				class_count[j]++;
				break;
			}
		}
		if (j == nr_class) {
			if (nr_class == max_nr_class) {
				max_nr_class *= 2;
				label = (int *)realloc(label, sizeof(int) * max_nr_class);
				class_count = (int *)realloc(class_count, sizeof(int) * max_nr_class);
			}
			label[nr_class] = this_label;
			class_count[nr_class] = 1;
			nr_class++;
		}
		data_label[i] = j;
	}

	int *class_start = (int *)malloc(sizeof(int) * max_nr_class);
	class_start[0] = 0;
	for (int i = 1; i < nr_class; i++) {
		class_start[i] = class_start[i - 1] + class_count[i - 1];
	}

	for (int i = 0; i < l; i++) {
		reverse_index_ret[class_start[data_label[i]]] = i;
		class_start[data_label[i]]++;
	}

	class_start[0] = 0;
	for (int i = 1; i < nr_class; i++) {
		class_start[i] = class_start[i - 1] + class_count[i - 1];
	}

	*nr_class_ret = nr_class;
	*label_ret = label;
	*class_start_ret = class_start;
	*class_count_ret = class_count;
	free(data_label);
}

//solve C_SVC problem
void SVM::solve_c_svc(const svm_problem *prob, const svm_parameter *param,
		double Cp, double Cn, struct decision_function &function) {
	int l = prob->l;
	double *alpha = (double *)malloc(sizeof(double) * l);
	double *p = (double *)malloc(sizeof(double) * l);
	schar *y = (schar *)malloc(sizeof(schar) * l);;

	for (int i = 0; i < l; i++) {
		alpha[i] = 0;
		p[i] = -1;
		if (prob->y[i] > 0) {
			y[i] = +1;
		} else {
			y[i] = -1;
		}
	}

	SVC_QMatrix *svc_qmatrix = new SVC_QMatrix(prob, param, y);
	Solver *solver = new Solver(l, svc_qmatrix, p, y, 
			alpha, Cp, Cn, param->eps, param->shrinking);
	solver->solve(&function);

	for (int i = 0; i < l; i++) {
		function.alpha[i] *= y[i];
	}

	delete svc_qmatrix;
	delete solver;
	free(y);
	free(p);
	free(alpha);
}

void SVM::solve_nu_svc(const svm_problem *prob, const svm_parameter *param,
		struct decision_function &function) {
	int l = prob->l;
	double nu = param->nu;
	double *alpha = (double *)malloc(sizeof(double) * l);
	double *p = (double *)malloc(sizeof(double) * l);
	schar *y = (schar *)malloc(sizeof(schar) * l);
	double sum_pos = nu * l / 2;
	double sum_neg = nu * l / 2;
	for (int i = 0; i < l; i++) {
		p[i] = 0;
		if (prob->y[i] > 0) {
			y[i] = +1;
			alpha[i] = min(1.0, sum_pos);
			sum_pos -= alpha[i];
		} else {
			y[i] = -1;
			alpha[i] = min(1.0, sum_neg);
			sum_neg -= alpha[i];
		}
	}

	SVC_QMatrix *svc_qmatrix = new SVC_QMatrix(prob, param, y);
	NU_Solver *solver = new NU_Solver(l, svc_qmatrix, p, y, 
			alpha, 1.0, 1.0, param->eps, param->shrinking);
	solver->solve(&function);

	for (int i = 0; i < l; i++) {
		function.alpha[i] *= y[i] / solver->r;
	}
	function.rho /= solver->r;

	delete svc_qmatrix;
	delete solver;
	free(y);
	free(p);
	free(alpha);
}

struct decision_function SVM::svm_train_one(const svm_problem *prob, const svm_parameter *param,
		double Cp, double Cn) {
	decision_function func;
	func.alpha = (double *)malloc(sizeof(double) * prob->l);
	switch (param->svm_type) {
		case C_SVC:
			solve_c_svc(prob, param, Cp, Cn, func);
			break;
		case NU_SVC:
			solve_nu_svc(prob, param, func);
			break;
		default:
			cout << "unknown svm_type for now!" << endl;
			break;
	}

	return func;
}

// Interface functions
void SVM::svm_train() {
	// build svm_model
	model = (struct svm_model *)malloc(sizeof(struct svm_model));
	//param
	model->param = *param;

	int l = prob->l;
	int nr_class;//class count
	int *label = NULL;//class label
	int *class_start = NULL;//start index of each class
	int *class_count = NULL;//element count of each class
	int *reverse_index = (int *)malloc(sizeof(int) * l);//reverse_index[i]=j,the jth data element
	// is stored in the ith position when all data are 
	// rearraged to store the data of same class together

	// group training data of the same class
	svm_group_classes(prob, &nr_class, &label, &class_start, &class_count, reverse_index);

	if (nr_class == 1) {
		cerr << "training data in only one class!" << endl;
		exit(1);
	}

	int i, j;
	// rearrange the input data
	struct svm_node **x = (struct svm_node **)malloc(sizeof(struct svm_node *) * l);
	for (i = 0; i < l; i++) {
		x[i] = prob->x[reverse_index[i]];
	}

	// calculate weighted C
	double *weighted_C = (double *)malloc(sizeof(double) * nr_class);
	for (i = 0; i < nr_class; i++) {
		weighted_C[i] = param->C;
	}

	for (i = 0; i < param->nr_weight; i++) {
		for (j = 0; j < nr_class; j++) {
			if (param->weight_label[i] == label[j]) {
				break;
			}
		}
		if (j == nr_class) {
			cerr << "class label " << param->weight[i] << " specified in weight is not found!" << endl;
			exit(1);
		}
		weighted_C[j] *= param->weight[i];
	}

	// train nr_class * (nr_class - 1) / 2 models
	struct decision_function *f = (struct decision_function *)malloc(sizeof(struct decision_function) * nr_class * (nr_class - 1) / 2);

	bool *is_sv = (bool *)malloc(sizeof(bool) * l);
	for (i = 0; i < l; i++) {
		is_sv[i] = false;
	}

	int index = 0;
	for (i = 0; i < nr_class; i++) {
		for (j = i + 1; j < nr_class; j++) {
			svm_problem sub_prob;
			int si = class_start[i], sj = class_start[j];
			int ci = class_count[i], cj = class_count[j];
			sub_prob.l = ci + cj;
			sub_prob.x = (svm_node **)malloc(sizeof(svm_node *) * sub_prob.l);
			sub_prob.y = (double *)malloc(sizeof(double) * sub_prob.l);

			int k;
			for (k = 0; k < ci; k++) {
				sub_prob.x[k] = x[si + k];
				sub_prob.y[k] = +1;
			}
			for (k = 0; k < cj; k++) {
				sub_prob.x[ci + k] = x[sj + k];
				sub_prob.y[ci + k] = -1;
			}

			f[index] = svm_train_one(&sub_prob, param, weighted_C[i], weighted_C[j]);

			for (k = 0; k < ci; k++) {
				if (!is_sv[si + k] && fabs(f[index].alpha[k]) > 0) {
					is_sv[si + k] = true;
				}
			}
			for (k = 0; k < cj; k++) {
				if (!is_sv[sj + k] && fabs(f[index].alpha[ci + k]) > 0) {
					is_sv[sj + k] = true;
				}
			}

			free(sub_prob.x);
			free(sub_prob.y);
			index++;
		}
	}


	//class label
	model->nr_class = nr_class;
	model->class_label = (int *)malloc(sizeof(int) * nr_class);
	for(i = 0; i < nr_class; i++) {
		model->class_label[i] = label[i];
	}

	// rho
	model->rho = (double *)malloc(sizeof(double) * nr_class * (nr_class - 1) / 2);
	for (i = 0; i < nr_class * (nr_class - 1) / 2; i++) {
		model->rho[i] = f[i].rho;
	}

	//nSV
	int total_sv = 0;
	int *sv_start = (int *)malloc(sizeof(int) * nr_class);
	model->nSV = (int *)malloc(sizeof(int) * nr_class);
	for (i = 0; i < nr_class; i++) {
		model->nSV[i] = 0;
		for (j = 0; j < class_count[i]; j++) {
			if (is_sv[class_start[i] + j]) {
				model->nSV[i]++;
				total_sv++;
			}
		}
		if (i == 0) {
			sv_start[i] = 0;
		} else {
			sv_start[i] = sv_start[i - 1] + model->nSV[i - 1];
		}
	}
	model->total_sv = total_sv;

	//SV
	model->SV = (struct svm_node **)malloc(sizeof(svm_node *) * total_sv);
	index = 0;
	for (i = 0; i < l; i++) {
		if (is_sv[i]) {
			model->SV[index++] = x[i];
		}
	}

	//sv_coef
	model->sv_coef = (double **)malloc(sizeof(double *) * (nr_class - 1));
	for (i = 0; i < nr_class - 1; i++) {
		model->sv_coef[i] = (double *)malloc(sizeof(double) * total_sv);
	}

	index = 0;
	for (i = 0; i < nr_class; i++) {
		for (j = i + 1; j < nr_class; j++) {
			int si = class_start[i], ci = class_count[i];
			int sj = class_start[j], cj = class_count[j];

			int tmp_index = sv_start[i];
			int k;
			for (k = 0; k < ci; k++) {
				if (is_sv[si + k]) {
					model->sv_coef[j - 1][tmp_index++] = f[index].alpha[k];
				}
			}
			tmp_index = sv_start[j];
			for (k = 0; k < cj; k++) {
				if (is_sv[sj + k]) {
					model->sv_coef[i][tmp_index++] = f[index].alpha[ci + k];
				}
			}

			index++;

		}
	}

	free(label);
	free(class_start);
	free(class_count);
	free(reverse_index);
	free(x);
	free(weighted_C);
	free(is_sv);
	for (i = 0; i < nr_class * (nr_class - 1) / 2; i++) {
		free(f[i].alpha);
	}
	free(f);
	free(sv_start);
}

double SVM::svm_predict(const svm_node *x) {
	int nr_class = model->nr_class;
	int l = model->total_sv;

	double *kvalue = (double *)malloc(sizeof(double) * l);
	for (int i = 0; i < l; i++) {
		kvalue[i] = QMatrix::k_function(x, model->SV[i], &model->param);
	}

	int *sv_start = (int *)malloc(sizeof(int) * nr_class);
	sv_start[0] = 0;
	for (int i = 1; i < nr_class; i++) {
		sv_start[i] = sv_start[i - 1] + model->nSV[i - 1];
	}

	int *vote = (int *)malloc(sizeof(int) * nr_class);
	for (int i = 0; i < nr_class; i++) {
		vote[i] = 0;
	}

	int index = 0;
	for (int i = 0; i < nr_class; i++) {
		for (int j = i + 1; j < nr_class; j++) {
			double sum = 0;
			int si = sv_start[i], ci = model->nSV[i];
			int sj = sv_start[j], cj = model->nSV[j];

			int k;
			double *coef1 = model->sv_coef[j - 1];
			for (k = 0; k < ci; k++) {
				sum += coef1[si + k] * kvalue[si + k];
			}
			double *coef2 = model->sv_coef[i];
			for (k = 0; k < cj; k++) {
				sum +=coef2[sj + k] * kvalue[sj + k];
			}
			sum -= model->rho[index];

			if (sum > 0) {
				vote[i]++;
			} else {
				vote[j]++;
			}

			index++;
		}
	}

	int vote_max_idx = 0;
	for (int i = 1; i < nr_class; i++) {
		if (vote[i] > vote[vote_max_idx]) {
			vote_max_idx = i;
		}
	}

	free(kvalue);
	free(sv_start);
	free(vote);

	return model->class_label[vote_max_idx];
}


// only for C-SVC or nu-SVC
double SVM::svm_cross_validation() {
	if (param->svm_type != C_SVC && param->svm_type != NU_SVC) {
		cerr << "svm cross validation only for C-SVC or nu-SVC" << endl;
		exit(1);
	}

	int nr_fold = 10;
	//int nr_fold = param->nr_fold;
	if (nr_fold <= 0) {
		cerr << "the nr_fold parameter must > 0" << endl;
		exit(1);
	}

	int *fold_start = (int *)malloc(sizeof(int) * (nr_fold + 1));
	int l = prob->l;
	int *reverse_index = (int *)malloc(sizeof(int) * l);

	// random shuffle and then data grouped by fold using the array reverse_index
	if (nr_fold < l) {
		int nr_class;
		int *label = NULL;
		int *class_start = NULL;
		int *class_count = NULL;

		svm_group_classes(prob, &nr_class, &label, &class_start, &class_count, reverse_index);

		int *fold_count = (int *)malloc(sizeof(int) * nr_fold);
		int *index = (int *)malloc(sizeof(int) * l);
		int i, j, k;
		for (i = 0; i < l; i++) {
			index[i] = reverse_index[i];
		}

		for (i = 0; i < nr_class; i++) {
			for (j = 0; j < class_count[i]; j++) {
				k = j + rand() % (class_count[i] - j);
				swap(index[class_start[i] + j], index[class_start[i] + k]);
			}
		}

		for (i = 0; i < nr_fold; i++) {
			fold_count[i] = 0;
			for (j = 0; j < nr_class; j++) {
				fold_count[i] += (i + 1) * class_count[j] / nr_fold - i * class_count[j] / nr_fold;
			}
		}

		fold_start[0] = 0;
		for (i = 1; i <= nr_fold; i++) {
			fold_start[i] = fold_start[i - 1] + fold_count[i - 1];
		}
		for (i = 0; i < nr_class; i++) {
			for (j = 0; j < nr_fold; j++) {
				int begin = class_start[i] + j * class_count[i] / nr_fold;
				int end = class_start[i] + (j + 1) * class_count[i] / nr_fold;
				for (k = begin; k < end; k++) {
					reverse_index[fold_start[j]] = index[k];
					fold_start[j]++;
				}
			}
		}
		fold_start[0] = 0;
		for (i = 1; i <= nr_fold; i++) {
			fold_start[i] = fold_start[i - 1] + fold_count[i - 1];
		}

		free(label);
		free(class_start);
		free(class_count);
		free(index);
		free(fold_count);

	} else {
		int i;
		for (i = 0; i < l; i++) {
			reverse_index[i] = i;
		}

		for (i = 0; i < l; i++) {
			int j = i + rand() % (l - i);
			swap(reverse_index[i], reverse_index[j]);
		}

		for (i = 0; i <= nr_fold; i++) {
			fold_start[i] = i * l / nr_fold;
		}
	}

	int i, j , k;
	double sum = 0;
	for (i = 0; i < nr_fold; i++) {
		int begin = fold_start[i];
		int end = fold_start[i + 1];

		struct svm_problem *subprob = (struct svm_problem *)malloc(sizeof(struct svm_problem));
		subprob->l = l - (end - begin);
		subprob->x = (struct svm_node **)malloc(sizeof(struct svm_node *) * subprob->l);
		subprob->y = (double *)malloc(sizeof(double) * subprob->l);

		k = 0;
		for (j = 0; j < begin; j++) {
			subprob->x[k] = prob->x[reverse_index[j]];
			subprob->y[k] = prob->y[reverse_index[j]];
			k++;
		}
		for (j = end; j < l; j++) {
			subprob->x[k] = prob->x[reverse_index[j]];
			subprob->y[k] = prob->y[reverse_index[j]];
			k++;
		}

		struct svm_parameter *subparam = (struct svm_parameter *)malloc(sizeof(struct svm_parameter));
		memcpy((void *)subparam, (void *)param, sizeof(struct svm_parameter));

		SVM *svm = new SVM(subprob, subparam);
		svm->svm_train();
		int right = 0;
		for (j = begin; j < end; j++) {
			if (svm->svm_predict(prob->x[reverse_index[j]]) == prob->y[reverse_index[j]]) {
				right++;
			}
		}
		
		sum += right * 1.0 / (end - begin + 1);

		delete svm;
	}

	sum /= nr_fold;

	free(fold_start);
	free(reverse_index);

	return sum;
}

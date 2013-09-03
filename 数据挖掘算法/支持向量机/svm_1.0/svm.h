#ifndef _SVM_H_
#define _SVM_H_

#include "util.h"

class SVM {
	public:
		SVM(struct svm_problem *prob, struct svm_parameter *param,
				struct svm_node *x_space);
		~SVM();
		//train
		void svm_train();
		//predict
		double svm_predict(const svm_node *x);
	private:
		//train
		void svm_group_classes(const struct svm_problem *prob, int *nr_class, int **label_ret,
				int **class_start_ret, int **class_count_ret, int *reverse_index_ret);
		void solve_c_svc(const svm_problem *prob, const svm_parameter *param, double Cp, double Cn,
				struct decision_function &function); 
		void solve_nu_svc(const svm_problem *prob, const svm_parameter *param,
				struct decision_function &function);
		struct decision_function svm_train_one(const svm_problem *prob, const svm_parameter *param,
				double Cp, double Cn); 
		struct svm_problem *prob;
		struct svm_parameter *param;
		struct svm_node *x_space;
		struct svm_model *model;
};

#endif

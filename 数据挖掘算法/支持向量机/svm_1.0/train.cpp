#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include "util.h"
#include "svm.h"
using namespace std;

// print the usage message and exit
void exit_with_help() {
	cout << "Usage: svm_train [options] training_set_file [model_file]" << endl;
	cout << "\tOptions:" << endl;
	cout << "\t-s svm_type: set type of SVM (default 0)" << endl;
	cout << "\t\t0 -- C-SVC" << endl;
	cout << "\t\t1 -- nu-SVC" << endl;
	cout << "\t-t kernel_type: set type of kernel function (default 2)" << endl;
	cout << "\t\t0 -- linear" << endl;
	cout << "\t\t1 -- polynomial" << endl;
	cout << "\t\t2 -- radial basis function" << endl;
	cout << "\t\t3 -- sigmoid" << endl;
	cout << "\t\t4 -- precomputed kernel" << endl;
	cout << "\t-d degree: set degree in kernel function (default 3)" << endl;
	cout << "\t-g gamma: set gamma in kernel function (default 1 / num_features)" << endl;
	cout << "\t-r coef0: set coef0 in kernel function (default 0)" << endl;
	cout << "\t-m cachesize: set the cache memory size in MB (default 100)" << endl;
	cout << "\t-e epsilon: set tolerance of termination criterion (default 0.001)" << endl;
	cout << "\t-c cost: set the parameter C of C-SVC (default 1)" << endl;
	cout << "\t-n nu: set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)" << endl;
	cout << "\t-wi weight: set the parameter C of class i to weight * C for C-SVC (default 1)" << endl;
	cout << "\t-h shrinking: wheter to use the shrinking heuristics, 0 or 1 (default 1)" << endl;

	exit(1);
}

// parse the command line parameters
struct svm_parameter *parse_command_line(int argc, char **argv, string *input_file_name, string *model_file_name) {
	struct svm_parameter *param = (struct svm_parameter *)malloc(sizeof(struct svm_parameter));
	//default values
	param->svm_type = 0;
	param->kernel_type = RBF;
	param->degree = 3;
	param->gamma = 0;
	param->coef0 = 0;
	param->cache_size = 100;
	param->eps = 1e-3;
	param->C = 1;
	param->nu = 0.5;
	param->nr_weight = 0;
	param->weight_label = NULL;
	param->weight = NULL;
	param->shrinking = 1;

	// parse options
	int i;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		}

		if (++i >= argc) {
			exit_with_help();
		}

		switch (argv[i - 1][1]) {
			case 's':
				param->svm_type = atoi(argv[i]);
				break;
			case 't':
				param->kernel_type = atoi(argv[i]);
				break;
			case 'd':
				param->degree = atoi(argv[i]);
				break;
			case 'g':
				param->gamma = atof(argv[i]);
				break;
			case 'r':
				param->coef0 = atof(argv[i]);
				break;
			case 'm':
				param->cache_size = atof(argv[i]);
				break;
			case 'e':
				param->eps = atof(argv[i]);
				break;
			case 'c':
				param->C = atof(argv[i]);
				break;
			case 'n':
				param->nu = atof(argv[i]);
				break;
			case 'w':
				param->nr_weight++;
				param->weight_label = (int *)realloc(param->weight_label, sizeof(int) * param->nr_weight);
				param->weight = (double *)realloc(param->weight, sizeof(double) * param->nr_weight);
				param->weight_label[param->nr_weight - 1] = atoi(&argv[i - 1][2]);
				param->weight[param->nr_weight - 1] = atof(argv[i]);
				break;
			case 'h':
				param->shrinking = atoi(argv[i]);
				break;
			default:
				cerr << "Unknown option: -" << argv[i - 1][1] << endl;
				exit_with_help();
		}
	}
	
	if (i >= argc) {
		exit_with_help();
	}

	*input_file_name = argv[i];
	i++;
	if (i < argc) {
		*model_file_name = argv[i];
	} else {
		int index = (*input_file_name).find_last_of('/', (*input_file_name).length());
		if (index == -1) {
			*model_file_name = (*input_file_name) + ".model";
		} else {
			*model_file_name = (*input_file_name).substr(index + 1) + ".model";
		}
	}

	return param;
}

//data line format:
//<class label>\t<index_1>:<value_1>\t......<index_n>:<value_n>\t\n
//<class label>[space]<index_1>:<value_1>[space]......<index_n>:<value_n>[space]\n
//index_1 < index_2 < index_3 < ... < index_n
struct svm_problem *read_problem(const string filename, struct svm_node **x_space, int *max_index) {
	struct svm_problem *prob = (struct svm_problem *)malloc(sizeof(struct svm_problem));

	ifstream fin;
	fin.open(filename.c_str(), ios::in);
	if (!fin.is_open()) {
		cerr << "Can't open file " << filename << endl;
		exit(1);
	}

	//statistic the lines number and the element number
	string line;
	prob->l = 0;
	int elements = 0;
	while (getline(fin, line)) {
		prob->l++;
		string::size_type index, last_index = 0;
		// 分割符为[space]或者\t
		while ((index = line.find(' ', last_index)) != string::npos
				|| (index = line.find('\t', last_index)) != string::npos) {
			elements++;
			last_index = index + 1;
		}
	}


	fin.clear();
	fin.seekg(0);

	// initialize the data storage
	prob->y = (double *)malloc(sizeof(double) * prob->l);
	prob->x = (struct svm_node **)malloc(sizeof(struct svm_node *) * prob->l);
	*x_space = (struct svm_node *)malloc(sizeof(struct svm_node) * elements);

	int j = 0;
	for (int i = 0; i < prob->l; i++) {
		prob->x[i] = &(*x_space)[j];
		getline(fin, line);
		string::size_type index = 0, last_index = 0;
		// 分割符为[space]或者\t
		while ((index = line.find(' ', last_index)) != string::npos
				|| (index = line.find('\t', last_index)) != string::npos) {
			if (last_index == 0) {
				prob->y[i] = strtod(line.substr(0, index).c_str(), NULL);
			} else {
				string temp_str = line.substr(last_index, index);
				int temp_index = temp_str.find(':', 0);
				(*x_space)[j].index = atoi(temp_str.substr(0, temp_index).c_str());
				(*x_space)[j].value = strtod(temp_str.substr(temp_index + 1, temp_str.length()).c_str(), NULL);
				j++;
			}
			last_index = index + 1;
		}

		if ((*x_space)[j - 1].index > (*max_index)) {
			*max_index = (*x_space)[j - 1].index;
		}

		(*x_space)[j++].index = -1;
	}

	fin.close();

	return prob;
}


int main(int argc, char **argv) {

	string input_file_name;
	string model_file_name;

	// parameter
	struct svm_parameter *param = parse_command_line(argc, argv, &input_file_name, &model_file_name);

	cout << "Parameter Read Finish!" << endl;

	// problem
	struct svm_node *x_space;
	int max_index = 0;
	struct svm_problem *prob = read_problem(input_file_name, &x_space, &max_index);
	if (param->gamma == 0 && max_index > 0) {
		param->gamma = 1.0 / max_index;
	}

	cout << "Train Begin!" << endl;

	// train
	SVM* svm = new SVM(prob, param, x_space);
	svm->svm_train();

	cout << "Train Finished!" << endl;

	// predict
	int right = 0;
	for (int i = 0; i < prob->l; i++) {
		if (svm->svm_predict(prob->x[i]) == prob->y[i]) {
			right++;
		}
	}
	cout << right * 1.0 / prob->l << endl;

	delete svm;

	return 0;
}

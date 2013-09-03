#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "backpropagation.h"

// allocate 1d array of doubles
double *alloc_1d_double_array(int n) {
	double *array;

	array = (double *)malloc(sizeof(double) * n);

	return array;
}

//allocate 2d array of doubles
double **alloc_2d_double_array(int m, int n) {
	double **array;

	array = (double **)malloc(sizeof(double *) * m);
	int i;
	for (i = 0; i < m; i++) {
		array[i] = (double *)malloc(sizeof(double) * n);
	}

	return array;
}

// the internal function for create bpnn network
BPNN *bpnn_internal_create(int n_in, int n_hidden, int n_out, double learn_rate) {
	BPNN *network;

	network = (BPNN *)malloc(sizeof(BPNN));

	// units
	network->input_num = n_in;
	network->hidden_num = n_hidden;
	network->output_num = n_out;
	network->learn_rate = learn_rate;

	// input
	network->input_units = alloc_1d_double_array(network->input_num);
	network->hidden_units = alloc_1d_double_array(network->hidden_num);
	network->output_units = alloc_1d_double_array(network->output_num);

	// bias
	network->bias_hidden_units = alloc_1d_double_array(network->hidden_num);
	network->bias_output_units = alloc_1d_double_array(network->output_num);

	// error
	network->error_hidden_units = alloc_1d_double_array(network->hidden_num);
	network->error_output_units = alloc_1d_double_array(network->output_num);

	// weight
	network->input_hidden_weights = alloc_2d_double_array(network->input_num, network->hidden_num);
	network->hidden_output_weights = alloc_2d_double_array(network->hidden_num, network->output_num);


	// target
	network->target = alloc_1d_double_array(network->output_num);

	return network;
}

#define BIGRND 0x7fffffff

// return a random number between -1.0 and 1.0
double dpn1() {
	return (((double)random() / (double)BIGRND) * 2.0 - 1.0);
}

// randomize weights
void bpnn_randomize_weights(double **weights, int m, int n) {
	int i, j;

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			weights[i][j] = dpn1();
		}
	}
}

// zero weights
void bpnn_zero_weights(double **weights, int m, int n) {
	int i, j;

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			weights[i][j] = 0.0;
		}
	}
}

// randomize bias
void bpnn_randomize_bias(double *bias, int n) {
	int i;

	for (i = 0; i < n; i++) {
		bias[i] = dpn1();
	}
}

// create a fully connected network from scratch
BPNN *bpnn_create(int n_in, int n_hidden, int n_out, double learn_rate) {
	BPNN *network;
	// using internal bpnn create function to create a bpnn network
	network = bpnn_internal_create(n_in, n_hidden, n_out, learn_rate);

	// set the random number generator seed
	srandom(time(NULL));

	// generate random weights
	bpnn_randomize_weights(network->input_hidden_weights, n_in, n_hidden);
	bpnn_randomize_weights(network->hidden_output_weights, n_hidden, n_out);

	// generate random bias
	bpnn_randomize_bias(network->bias_hidden_units, n_hidden);
	bpnn_randomize_bias(network->bias_output_units, n_out);

	return network;
}

// delete BPNN network
void bpnn_delete(BPNN *network) {
	int n_in, n_hidden, n_output;

	// units
	n_in = network->input_num;
	n_hidden = network->hidden_num;
	n_output = network->output_num;

	// input
	free(network->input_units);
	free(network->hidden_units);
	free(network->output_units);

	// bias
	free(network->bias_hidden_units);
	free(network->bias_output_units);


	// error
	free(network->error_hidden_units);
	free(network->error_output_units);

	// weight
	int i;
	for (i = 0; i < n_in; i++) {
		free(network->input_hidden_weights[i]);
	}
	free(network->input_hidden_weights);

	for (i = 0; i < n_hidden; i++) {
		free(network->hidden_output_weights[i]);
	}
	free(network->hidden_output_weights);

	// target
	free(network->target);

	// network
	free(network);
}

// bpnn layer forward
void bpnn_layer_forward(double *layer1, double *layer2, double **weights, int n1, int n2, double *bias) {
	int i, j;
	double sum;

	for (j = 0; j < n2; j++) {
		sum = 0.0;
		for (i = 0; i < n1; i++) {
			sum += weights[i][j] * layer1[i];
		}
		sum += bias[j];

		layer2[j] = (1.0 / (1.0 + exp(-sum)));
	}
}

// calculate the error of the output layer
void bpnn_output_layer_error(double *error_output_units, double *output_units, double *target, int n) {
	int i;

	for (i = 0; i < n; i++) {
		error_output_units[i] = output_units[i] * (1 - output_units[i]) * (target[i] - output_units[i]);
	}
}

// calculate the error of the hidden layer
void bpnn_hidden_layer_error(double *error_hidden_units, double *hidden_units, double * error_output_units, double **hidden_output_weights, int n_hidden, int n_out) {
	int i, j;
	double sum;

	for (i = 0; i < n_hidden; i++) {
		sum = 0.0;
		for (j = 0; j < n_out; j++) {
			sum += error_output_units[j] * hidden_output_weights[i][j];
		}
		error_hidden_units[i] = hidden_units[i] * (1 - hidden_units[i]) * sum;
	}
}

// adjust weights and bias
void bpnn_adjust_weights_and_bias(double *units, double *error, double **weights, double *bias, int n1, int n2, double learn_rate) {
	int i, j;
	for (j = 0; j < n2; j++) {
		bias[j] += error[j] * learn_rate;
	}
	for (i = 0; i < n1; i++) {
		for (j = 0; j < n2; j++) {
			weights[i][j] += error[j] * units[i] * learn_rate;
		}
	}

}

// bpnn train
void bpnn_train(BPNN *network) {
	int n_in, n_hidden, n_out;
	n_in = network->input_num;
	n_hidden = network->hidden_num;
	n_out = network->output_num;

	// layer forward
	bpnn_layer_forward(network->input_units, network->hidden_units, network->input_hidden_weights, n_in, n_hidden, network->bias_hidden_units);
	bpnn_layer_forward(network->hidden_units, network->output_units, network->hidden_output_weights, n_hidden, n_out, network->bias_output_units);

	// output layer error
	bpnn_output_layer_error(network->error_output_units, network->output_units, network->target, n_out);

	// hidden layer error
	bpnn_hidden_layer_error(network->error_hidden_units, network->hidden_units, network->error_output_units, network->hidden_output_weights, n_hidden, n_out);

	// adjust weights and bias
	bpnn_adjust_weights_and_bias(network->hidden_units, network->error_output_units, network->hidden_output_weights, network->bias_output_units, n_hidden, n_out, network->learn_rate);
	bpnn_adjust_weights_and_bias(network->input_units, network->error_hidden_units, network->input_hidden_weights, network->bias_hidden_units, n_in, n_hidden, network->learn_rate);
}

// bpnn test
void bpnn_test(BPNN *network) {
	int n_in, n_hidden, n_out;
	n_in = network->input_num;
	n_hidden = network->hidden_num;
	n_out = network->output_num;

	// layer forward
	bpnn_layer_forward(network->input_units, network->hidden_units, network->input_hidden_weights, n_in, n_hidden, network->bias_hidden_units);
	bpnn_layer_forward(network->hidden_units, network->output_units, network->hidden_output_weights, n_hidden, n_out, network->bias_output_units);
}


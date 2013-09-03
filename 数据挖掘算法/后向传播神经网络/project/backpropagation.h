#ifndef _BACKPROPAGATION_H_
#define _BACKPROPAGATION_H_

typedef struct {
	int input_num;
	int hidden_num;
	int output_num;

	double learn_rate;

	// the input value of three layers
	double *input_units;
	double *hidden_units;
	double *output_units;

	// the bias value of the last two layers
	double *bias_hidden_units;
	double *bias_output_units;

	// error of the last two layers
	double *error_hidden_units;
	double *error_output_units;

	// weights of the first two layers
	double **input_hidden_weights;
	double **hidden_output_weights;

	// the target of the classification
	double *target;
} BPNN;


// create a fully connected network from scratch
BPNN *bpnn_create(int n_in, int n_hidden, int n_out, double learn_rate);

// delete the network specified
void bpnn_delete(BPNN *network);

// one interaction of bpnn train
void bpnn_train(BPNN *network);

// one interaction of bpnn test
void bpnn_test(BPNN *network);

#endif


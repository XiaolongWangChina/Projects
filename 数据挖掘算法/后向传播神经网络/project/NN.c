#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "backpropagation.h"


int main(int argc, char **argv) {

	char trainfile[256], testfile[256];
	trainfile[0] = testfile[0] = '\0';

	int attr_per_record, class_num, train_record_num, test_record_num, middle_layer_nodes, pornot;
	double learn_rate, miss_rate;

	// Process the args in command line 
	int i, j;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
				case 'r':
					strcpy(trainfile, argv[++i]);
					break;
				case 't':
					strcpy(testfile, argv[++i]);
					break;
				case 'd':
					attr_per_record = atoi(argv[++i]);
					break;
				case 'c':
					class_num = atoi(argv[++i]);
					break;
				case 's':
					train_record_num = atoi(argv[++i]);
					break;
				case 'm':
					test_record_num = atoi(argv[++i]);
					break;
				case 'n':
					middle_layer_nodes = atoi(argv[++i]);
					break;
				case 'l':
					learn_rate = atof(argv[++i]);
					break;
				case 'a':
					miss_rate = atof(argv[++i]);
					break;
				case 'p':
					pornot = atoi(argv[++i]);
					break;
				default:
					printf("Unknown args!\n");
					break;
			}
		}
	}

	// read the training data from training data file into memory
	DataNode **training_data = load_data_from_file(trainfile, train_record_num, attr_per_record);

	// preprocessing of the training data
	// get the classes and the data
	int *classes = (int *)malloc(sizeof(int) * train_record_num);
	DataNode *classname = (DataNode *)malloc(sizeof(DataNode) * train_record_num);
	double **normal_training_data = preprocessing(training_data, train_record_num, attr_per_record, classes, classname);

	// create the BPNN
	BPNN *bpnn = bpnn_create(attr_per_record - 2, middle_layer_nodes, class_num, learn_rate);

	// confusion matrix
	int **confusion_matrix = (int **)malloc(sizeof(int) * class_num);
	for (i = 0; i < class_num; i++) {
		confusion_matrix[i] = (int *)malloc(sizeof(int) * class_num);
	}


	// interaction to train and test the BPNN model
	int interaction_count = 0;
	while (1) {
		// train the BPNN model
		int train_record = train_record_num * 2 / 3;
		for (i = 0; i < train_record; i++) {
			// set the input units
			for (j = 0; j < attr_per_record - 2; j++) {
				bpnn->input_units[j] = normal_training_data[i][j+2];
			}
			// set the target
			for (j = 0; j < class_num; j++) {
				if (classes[i] == j) {
					bpnn->target[j] = 0.9;
				} else {
					bpnn->target[j] = 0.1;
				}
			}

			// one train
			bpnn_train(bpnn);
		}

		// set all the data in confusion matrix to zero
		for (i = 0; i < class_num; i++) {
			for (j = 0; j < class_num; j++) {
				confusion_matrix[i][j] = 0;
			}
		}

		// test the BPNN model
		int wrong = 0;
		for (i = train_record; i < train_record_num; i++) {
			// set the input units
			for (j = 0; j < attr_per_record - 2; j++) {
				bpnn->input_units[j] = normal_training_data[i][j+2];
			}

			// one test
			bpnn_test(bpnn);

			// get the predicted class number
			double max = bpnn->output_units[0];
			int c_no = 0;
			for (j = 1; j < class_num; j++) {
				if (bpnn->output_units[j] > max) {
					max = bpnn->output_units[j];
					c_no = j;
				}
			}

			// set confusion_matrix
			confusion_matrix[classes[i]][c_no]++;

			// set wrong value
			if (c_no == classes[i]) {
				//printf("right\n");
			} else {
				//printf("wrong\n");
				wrong++;
			}
		}
		double miss_classify_rate = (double)wrong/(double)(train_record_num-train_record);
		printf("interaction: %d\n", interaction_count);
		printf("error rate:%lf\n", miss_classify_rate);
		printf("classification accuracy:%lf\n", 1 - miss_classify_rate);
		printf("confusion matrix:\n");


		printf("\t");
		for (i = 0; i < class_num; i++) {
			printf("%s\t", classname[i].data_value);
		}
		printf("\n");
		for (i = 0; i < class_num; i++) {
			printf("%s\t", classname[i].data_value);
			for (j = 0; j < class_num; j++) {
				printf("%d\t", confusion_matrix[i][j]);
			}
			printf("\n");
		}

		printf("\n\n");
		// end the interaction meet the requirement
		interaction_count++;
		if (interaction_count >= 1000 || miss_classify_rate < miss_rate) {
		//if (interaction_count >= 2000) {
			break;
		}
	}


	// free the memory
	for (i = 0; i < train_record_num; i++) {
		free(training_data[i]);
		free(normal_training_data[i]);
	}
	free(training_data);
	free(normal_training_data);
	free(classes);
	for (i = 0; i < class_num; i++) {
		free(confusion_matrix[i]);
	}
	free(confusion_matrix);

	bpnn_delete(bpnn);

	return 0;

}

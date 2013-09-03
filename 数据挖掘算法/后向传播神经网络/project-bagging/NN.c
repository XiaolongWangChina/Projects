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


	// confusion matrix
	int **confusion_matrix = (int **)malloc(sizeof(int) * class_num);
	for (i = 0; i < class_num; i++) {
		confusion_matrix[i] = (int *)malloc(sizeof(int) * class_num);
	}

	// using bagging method to classification
	// create 20 BPNN models
	BPNN *bpnn[20];
	for (i = 0; i < 20; i++) {
		bpnn[i] = bpnn_create(attr_per_record - 2, middle_layer_nodes, class_num, learn_rate);
	}

	// interaction to train and test the BPNN model
	int interaction_count = 0;
	while (1) {
		// train the 20 BPNN models
		// 2/3 of the train_record_num is used as train_record
		// the rest 1/3 is used as test record to compute some args
		int train_record = train_record_num * 2 / 3;
		// sampling 9/10 of the train_record
		//int sample_record = train_record * 9 / 10;
		int sample_record = train_record;
		//srand(time(NULL));
		int k;
		for (k = 0; k < 20; k++) {
			// sampling
			int *index = (int *)malloc(sizeof(int) * train_record);
			for (i = 0; i < sample_record; i++) {
				//index[i] = rand() % train_record;
				index[i] = i;
			}


			// train one bpnn model
			for (i = 0; i < sample_record; i++) {
				// set the input units
				for (j = 0; j < attr_per_record - 2; j++) {
					bpnn[k]->input_units[j] = normal_training_data[index[i]][j+2];
				}
				// set the target
				for (j = 0; j < class_num; j++) {
					if (classes[index[i]] == j) {
						bpnn[k]->target[j] = 0.9;
					} else {
						bpnn[k]->target[j] = 0.1;
					}
				}

				// one train
				bpnn_train(bpnn[k]);
			}

			// free memory of the index array
			free(index);
		}

		// record the predict class no
		int predict_result[train_record_num][class_num];
		for (i = 0; i < train_record_num; i++) {
			for (j = 0; j < class_num; j++) {
				predict_result[i][j] = 0;
			}
		}

		// test the 20 BPNN models
		for (k = 0; k < 20; k++) {
			for (i = train_record; i < train_record_num; i++) {
				// set the input units
				for (j = 0; j < attr_per_record - 2; j++) {
					bpnn[k]->input_units[j] = normal_training_data[i][j+2];
				}

				// one test
				bpnn_test(bpnn[k]);

				// get the predicted class number
				double max = bpnn[k]->output_units[0];
				int c_no = 0;
				for (j = 1; j < class_num; j++) {
					if (bpnn[k]->output_units[j] > max) {
						max = bpnn[k]->output_units[j];
						c_no = j;
					}
				}
				predict_result[i][c_no]++;
			}
		}

		// vote
		int max_index;
		for (i = train_record; i < train_record_num; i++) {
			max_index = 0;
			for (j = 1; j < class_num; j++) {
				if (predict_result[i][j] > predict_result[i][max_index]) {
					max_index = j;
				}
			}

			// using predict_result[i][0] to record the final result
			predict_result[i][0] = max_index;
		}

		// set all the data in confusion matrix to zero
		for (i = 0; i < class_num; i++) {
			for (j = 0; j < class_num; j++) {
				confusion_matrix[i][j] = 0;
			}
		}

		// calculate the miss classify rate
		int miss_match = 0;
		for (i = train_record; i < train_record_num; i++) {
			confusion_matrix[classes[i]][predict_result[i][0]]++;
			if (predict_result[i][0] != classes[i]) {
				miss_match++;
			}
		}
		double miss_classify_rate = (double)(miss_match) / (double)(train_record_num - train_record + 1);
		printf("interaction: %d\n", interaction_count + 1);
		printf("error rate = %lf\n", miss_classify_rate);
		printf("classification accuracy = %lf\n", 1 - miss_classify_rate);

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

		// end the interaction if meet the requirement
		interaction_count++;
		//if (miss_classify_rate < miss_rate || interaction_count >= 1000) {
		if (interaction_count >= 2000) {
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

	return 0;

}

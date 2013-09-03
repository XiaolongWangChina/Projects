#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

// load data from data file into memory
DataNode **load_data_from_file(char* filename, int train_record_num, int attr_per_record) {
	// open the specified file
	FILE *file = fopen(filename, "r");
	int i, j, k;

	// allocate the array to store the data
	DataNode **data = (DataNode **)malloc(sizeof(DataNode *) * train_record_num);
	for (i = 0; i < train_record_num; i++) {
		data[i] = (DataNode *)malloc(sizeof(DataNode) * attr_per_record);
	}

	// read all the data into memory
	char c;
	for (i = 0; i < train_record_num; i++) {
		// attributes
		for (j = 0; j <= attr_per_record - 2; j++) {
			k = 0;
			while (((c = fgetc(file)) != '\t') && (k < 256)) {
				data[i][j].data_value[k++] = c;
			}
			data[i][j].data_value[k] = '\0';
		}
		// the last attribute
		k = 0;
		while (((c = fgetc(file)) != '\n') && (k < 256)) {
			data[i][j].data_value[k++] = c;
		}
		data[i][j].data_value[k] = '\0';
	}
	fclose(file);

	return data;
}

// test if is a number string
// the string is a number string if it only contains 0-9 and '.' or is "NAN"
// and a null string is not a number string
int is_number(char *string) {

	if (string[0] == '\0') {
		return 0;
	}

	if (strcmp(string, "NAN") == 0) {
		return 2;
	}
	int i = 0;
	while (string[i] != '\0') {
		if ((string[i] > '9' || string[i] < '0') && (string[i] != '.')) {
			return 0;
		}
		i++;
	}

	return 1;
}

// preprocessing of the data
// record (ID class attr1 attr2 attr3 ... ...)
// the data_value in data are alpha string or number string
// fill in the missing data
// using min-max normalization
// the class is like 0, 1, 2, etc.
double **preprocessing(DataNode **data, int train_record_num, int attr_per_record, int *classes, DataNode *classname) {
	int i, j;

	// create the result array
	double **result = (double **)malloc(sizeof(double *) * train_record_num);
	for (i = 0; i < train_record_num; i++) {
		result[i] = (double *)malloc(sizeof(double) * attr_per_record);
	}

	// missing data:
	// class label -- ""(null char) -- omit
	// number data -- NAN -- fill with the average of the rest
	// string data -- ""(null char) -- Discretization, and fill the missing data with the average of the rest

	// process the class attribute
	// note: classname can be returned if it is needed.
	//DataNode *classname = (DataNode *)malloc(sizeof(DataNode) * train_record_num);
	//classname = (DataNode *)malloc(sizeof(DataNode) * train_record_num);
	int classno = -1;
	int find;
	for (i = 0; i < train_record_num; i++) {
		// if the class label is not null
		if (data[i][1].data_value[0] != '\0') {
			find = 0;
			for (j = classno; j >= 0; j--) {
				//printf("%s\t%s\n", classname[j].data_value, data[i][1].data_value);
				if (strcmp(classname[j].data_value, data[i][1].data_value) == 0) {
					find = 1;
					break;
				}
			}
			if (find) {
				classes[i] = j;
			} else {
				strcpy(classname[++classno].data_value, data[i][1].data_value);
				classes[i] = classno;
			}
		} else {
			classes[i] = -1; // use -1 as a label to omit the i'th record
		}
	} 
	// release the memory
	//free(classname);

	// process the other attributes
	for (j = 2; j < attr_per_record; j++) {
		// if the string of the first record is number
		// then we assume that all the rest of this column is number
		// maybe we should test all the data in the column to make this decision 
		// but it is time consuming.
		int missing_data = 0;
		if (is_number(data[0][j].data_value)) {
			double min, max, sum;
			// the first record contains NAN
			i = -1;
			while (is_number(data[++i][j].data_value) == 2) {}

			result[i][j] = min = max = sum = atof(data[i][j].data_value);
			for (i = i + 1; i < train_record_num; i++) {
				if (strcmp(data[i][j].data_value, "NAN") == 0) {
					missing_data++;
				} else {
					result[i][j] = atof(data[i][j].data_value);
					if (result[i][j] > max) {
						max = result[i][j];
					} else if (result[i][j] < min) {
						min = result[i][j];
					}
					sum += result[i][j];
				}
			}
			sum = sum / (train_record_num - missing_data);
			// min-max normalization
			for (i = 0; i < train_record_num; i++) {
				if (strcmp(data[i][j].data_value, "NAN") == 0) {
					result[i][j] = (sum - min) / (max - min);
				} else {
					result[i][j] = (result[i][j] - min) / (max - min);
				}
			}
		} else { // alpha data 
			DataNode *names = (DataNode *)malloc(sizeof(DataNode) * train_record_num);
			int alloc_num = -1;
			int find, k;
			for (i = 0; i < train_record_num; i++) {
				if (data[i][j].data_value[0] != '\0') {
					find = 0;
					for (k = alloc_num; k >= 0; k--) {
						if (strcmp(names[k].data_value, data[i][j].data_value) == 0) {
							find = 1;
							break;
						}
					}
					if (find) {
						result[i][j] = k;
					} else {
						strcpy(names[++alloc_num].data_value, data[i][j].data_value);
						result[i][j] = alloc_num;
					}
				} else {
					result[i][j] = -1;
				}
			}
			double sum = 0.0;
			for (k = 0; k <= alloc_num; k++) {
				sum += k;
			}
			sum = sum / (alloc_num + 1);
			// min-max normalization
			for (i = 0; i < train_record_num; i++) {
				if (result[i][j] == -1) {
					result[i][j] = (sum - 0) / (alloc_num - 0);
				} else {
					result[i][j] = (result[i][j] - 0) / (alloc_num - 0);
				}
			}
		}
	}

	return result;
}

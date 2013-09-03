#ifndef _DATA_H_
#define _DATA_H_

typedef struct {
	char data_value[256];
}DataNode;

// load data from file into memory
DataNode **load_data_from_file(char *filename, int train_record_num, int attr_per_record);

// preprocessing of the data
double **preprocessing(DataNode **data, int train_record_num, int attr_per_record, int *classes, DataNode *classname);

#endif

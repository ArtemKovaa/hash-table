#ifndef HASHTABLEFILEMANAGEMENT
#define HASHTABLEFILEMANAGEMENT

#include <iostream>
#include "hash_table.h"
#include "binary_file.h"

struct hash_table_file_management {
	hash_table* table;

	hash_table_file_management(int capacity);
	void add_from_binary_file(string filename, int index);
	void remove_by_key(string filename, int key);
	company find_by_key(string filename, int key);
};

#endif
#include "hash_table_file_management.h"

hash_table_file_management::hash_table_file_management(int capacity) {
	this->table = new hash_table(capacity);
}

void hash_table_file_management::add_from_binary_file(string filename, int index) {
	company c = find_in_binary_file_by_index(filename, index);
 	node* temp = new node(c.key, c.index);
	table_entity* entity = new table_entity(c.key + 7);
	this->table->add(entity, temp, c.key + 7);
}

void hash_table_file_management::remove_by_key(string filename, int key) {
	int index = this->table->find_by_key(key + 7)->i;
	delete_by_key_from_binary_file(filename, index);
	this->table->remove_by_key(key);
}

company hash_table_file_management::find_by_key(string filename, int key) {
	int index = this->table->find_by_key(key)->i;
	company c = find_in_binary_file_by_index(filename, index);
	return c;
}
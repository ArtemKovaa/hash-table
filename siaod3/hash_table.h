#ifndef HASHTABLE
#define HASHTABLE

#include <iostream>
#include <forward_list>

using namespace std;

struct node {
	int key;
	int i;
	node(int key, int i);
};

struct table_entity {
	int index;
	forward_list<node*>* list;
	table_entity();
	table_entity(int index);
};


struct hash_table {
	int capacity, length, removed;
	table_entity* entities;
	hash_table(int capacity);
	~hash_table();
	void add(table_entity* tab_entity, node* entity, int index);
	node* find_by_key(int key);
	void remove_by_key(int key);
	int hash_key(int key, int capacity);

private:
	void rehash();
	void add_to_list(table_entity* new_entities, table_entity* table_entity, node* entity, int capacity);
};


#endif

#include "hash_table.h"
#include <forward_list>

table_entity::table_entity(int index) {
	this->index = index;
	this->list = new forward_list<node*>;
}

table_entity::table_entity() {
	this->index = 0;
	this->list = new forward_list<node*>;
}

node::node(int key, int i) {
	this->key = key;
	this->i = i;
}

hash_table::hash_table(int capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->removed = 0;
	this->entities = new table_entity[capacity];
	for (int i = 0; i < capacity; i++) {
		this->entities[i] = table_entity();
	}
}

hash_table::~hash_table() {
	delete[] this->entities;
}

int hash_table::hash_key(int key, int capacity) {
	return key % capacity;
}

node* hash_table::find_by_key(int key) {
	int hash_key = this->hash_key(key, this->capacity);
	table_entity* entity = &(this->entities[hash_key]);
	forward_list<node*>* list = entity->list;
	for (auto iter = list->begin(); iter != list->end(); iter++) {
		if ((*iter)->key == key - 7) {
			return *iter;
		}
	}
	cout << "\nОшибка! Не удалось найти компанию по ключу!";
	exit(1);
}

void hash_table::remove_by_key(int key) {
	int hash_key = this->hash_key(key + 7, this->capacity);
	table_entity* entity = &(this->entities[hash_key]);
	forward_list<node*>* list = entity->list;
	for (auto iter = list->begin(); iter != list->end(); iter++) {
		if ((*iter)->key == key) {
			list->remove(*iter);
			this->removed++;
			break;
		}
	}
}

void hash_table::add_to_list(table_entity* new_entities, table_entity* table_entity, node* entity, int capacity) {
	int hash_key = this->hash_key(table_entity->index, capacity);
	if (new_entities[hash_key].list->empty()) {
		table_entity->list->push_front(entity);
		new_entities[hash_key] = *table_entity;
	}
	else {
		new_entities[hash_key].list->push_front(entity);
	}
}

void hash_table::rehash() {
	int new_length = 0;
	table_entity* new_entities = new table_entity[this->capacity * 2];
	for (int i = 0; i < this->capacity; i++) {
		if (!this->entities[i].list->empty()) {
			forward_list<node*>* list = this->entities[i].list;
			for (auto iter = list->begin(); iter != list->end(); iter++) {
				node* temp = new node((*iter)->key, (*iter)->i);
				table_entity* entity = new table_entity((*iter)->key + 7);
				this->add_to_list(new_entities, entity, temp, this->capacity * 2);
				new_length++;
			}
		}
	}

	this->capacity *= 2;
	this->length = new_length;
	this->removed = 0;
	delete[] this->entities;
	this->entities = new_entities;
}

void hash_table::add(table_entity* tab_entity, node* entity, int index) {
	if ((this->length + this->removed) >= (this->capacity * 3 / 4)) {
		this->rehash();
	}
	int hash_key = this->hash_key(tab_entity->index, this->capacity);
	if (entities[hash_key].list->empty()) {
		tab_entity->list->push_front(entity);
		entities[hash_key] = *tab_entity;
	}
	else {
		entities[hash_key].list->push_front(entity);
	}
	
	this->length++;
}


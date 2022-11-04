using namespace std;

#include <iostream>
#include <fstream>
#include "hash_table_file_management.h"
#include "binary_file.h"

company::company(int _key, string _license_number, string _title, string _founder, int _is_license_revoked) :
	key(_key), license_number(_license_number), title(_title), founder(_founder), is_license_revoked(_is_license_revoked) {}

company::company() {};

// Получение компаний с текстового файла
company* get_companies(string filename, int& count) {
	ifstream itf(filename);
	itf >> count;
	company* companies = new company[count];

	for (int i = 0; i < count; i++) {
		itf >> companies[i].key >> companies[i].license_number >> companies[i].title >> companies[i].founder >> companies[i].is_license_revoked;
	}
	itf.close();
	return companies;
}

// Заполнение двоичного файла компаниями
void fill_binary_file(string binary_filename, company* companies, int count) {
	ofstream otf(binary_filename, ios::binary | ios::trunc);
	otf.write((char*)&count, sizeof(int));
	otf.write((char*)companies, count * sizeof(company));
	otf.close();
}

// Конвертация текстового файла в двоичный
void convert_to_binary(string filename, string binary_filename) {
	int count;
	company* companies = get_companies(filename, count);
	fill_binary_file(binary_filename, companies, count);
}

// Поиск компании по индексу
company find_in_binary_file_by_index(string filename, int index) {
	company c;
	ifstream ibf(filename, ios::in | ios::binary);
	ibf.seekg(index * sizeof(company) + sizeof(int), ios::beg);
	ibf.read((char*)&c, sizeof(company));
	c.index = index;
	ibf.close();
	return c;
}

// Удаление компании по ключу
void delete_by_key_from_binary_file(string filename, int key) {
	fstream fbf(filename, ios::binary | ios::out | ios::in);
	int n;
	fbf.read((char*)&n, sizeof(int));

	if (key + 1 == n) {
		fbf.close();
		return;
	}

	company last_record = find_in_binary_file_by_index(filename, n - 1);
	fbf.seekg(sizeof(company) * key, ios::cur);
	fbf.write((char*)&last_record, sizeof(company));

	n--;
	fbf.seekg(0, ios::beg);
	fbf.write((char*)&n, sizeof(int));
	fbf.close();
}

// Чтение двоичного файла
company* get_companies_from_binary(string binary_filename, int& count) {
	ifstream ibf(binary_filename, ios::in | ios::binary);
	ibf.read((char*)&count, sizeof(int));
	company* companies = new company[count];
	ibf.read((char*)companies, count * sizeof(company));
	ibf.close();
	return companies;
}

// Вывод информации о компании
void print_company(company x) {
	cout << "Company key: " << x.key << ", company info: " << x.license_number << " " << x.title << " " << x.founder << " " << x.is_license_revoked;
}

// Вспомогательная функция для добавления компаний в текстовый файл при замере скорости работы программы
void add_to_file(string filename, company c) {
	ofstream otf(filename, ios::app);
	otf << "\n" << c.key << " " << c.license_number << " " << c.title << " " << c.founder << " " << c.is_license_revoked;
	otf.close();
}

int main()
{
	setlocale(LC_ALL, "RUS");
	convert_to_binary("text.txt", "binary.bin"); // Конвертация входных данных из текстового файла в двоичный
	cout << "Выберите номер теста:\n1 - вставка ключа без коллизии\n2 - вставка ключа с разрешением коллизии\n3 - вставка ключа с рехешированием\n";
	cout << "4 - удаление ключа из таблицы\n5 - поиск в таблице из 50000 элементов\n";
	int choice;
	hash_table_file_management* htfm;
	cin >> choice;
	switch (choice) {
		case 1:
			htfm = new hash_table_file_management(10); // Создание хеш-таблицы
			for (int i = 0; i < 5; i++) {
				htfm->add_from_binary_file("binary.bin", i);
			}
			convert_to_binary("text.txt", "binary.bin");
			for (int i = 0; i < 5; i++) {
				print_company(htfm->find_by_key("binary.bin", i + 7));
				cout << "\n";
			}
			break;
		case 2:
			htfm = new hash_table_file_management(10); // Создание хеш-таблицы
			convert_to_binary("textcol.txt", "binary.bin");
			for (int i = 0; i < 5; i++) {
				htfm->add_from_binary_file("binary.bin", i);
			}
			convert_to_binary("textcol.txt", "binary.bin");

			for (int i = 0; i < 4; i++) {
				print_company(htfm->find_by_key("binary.bin", i + 7));
				cout << "\n";
			}
			print_company(htfm->find_by_key("binary.bin", 100 + 7));
			cout << "\n";
			break;
		case 3:
			htfm = new hash_table_file_management(2); // Создание хеш-таблицы
			for (int i = 0; i < 5; i++) {
				htfm->add_from_binary_file("binary.bin", i);
			}
			convert_to_binary("text.txt", "binary.bin");
			for (int i = 0; i < 5; i++) {
				print_company(htfm->find_by_key("binary.bin", i + 7));
				cout << "\n";
			}
			break;
		case 4:
			htfm = new hash_table_file_management(10); // Создание хеш-таблицы
			for (int i = 0; i < 5; i++) {
				htfm->add_from_binary_file("binary.bin", i);
			}
			convert_to_binary("text.txt", "binary.bin");

			htfm->remove_by_key("binary.bin", 2);

			convert_to_binary("text.txt", "binary.bin");

			print_company(htfm->find_by_key("binary.bin", 1 + 7));
			cout << "\n";
			print_company(htfm->find_by_key("binary.bin", 0 + 7));
			cout << "\n";
			print_company(htfm->find_by_key("binary.bin", 3 + 7));
			print_company(htfm->find_by_key("binary.bin", 2 + 7));
			break;
		case 5:
			htfm = new hash_table_file_management(80000);

			company* companies = new company[50000];
			for (int i = 0; i < 50000; i++) {
				add_to_file("empty.txt", company(i, "AAAA", "BB", "CC", 1));
			}
			convert_to_binary("empty.txt", "benchmark.bin");

			cout << "\nТаблица заполняется...";
			for (int i = 0; i < 50000; i++) {
				htfm->add_from_binary_file("benchmark.bin", i);
			}
			cout << "\nТаблица успешно заполнена! Начинается поиск...\n";

			convert_to_binary("empty.txt", "benchmark.bin");

			unsigned int start_time = clock();
			print_company(htfm->find_by_key("benchmark.bin", 1007 + 7));
			unsigned int end_time = clock();

			cout << "\nВремя выполнения поиска в таблице из 50000 элементов: " << end_time - start_time << "ms\n";

	}
}


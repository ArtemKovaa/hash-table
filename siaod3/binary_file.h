#ifndef BINFILE
#define BINFILE

#include <iostream>
using namespace std;

struct company {
	int key;
	string license_number, title, founder;
	bool is_license_revoked;
	int index;

	company(int _key, string _license_number, string _title, string _founder, int _is_license_revoked);
	company();
};

company* get_companies(string filename, int& count);
void fill_binary_file(string binary_filename, company* companies, int count);
void convert_to_binary(string filename, string binary_filename);
company find_in_binary_file_by_index(string filename, int index);
void delete_by_key_from_binary_file(string filename, int key);
company* get_companies_from_binary(string binary_filename, int& count);
void print_company(company x);

#endif

#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
using namespace std;

struct Array {
    string* data;
    size_t size;
    size_t cap;
};


Array Array_create(size_t capacity );
void Array_destroy(Array& arr);

void Array_increase_capacity(Array& arr);
void Array_test_capacity(Array& arr);

void Array_push_back(Array& arr, string value);
void Array_insert(Array& arr, size_t idx, string value);

string Array_get(const Array& arr, size_t idx);
void Array_erase(Array& arr, size_t idx);
void Array_set(Array& arr, size_t idx, string value);

size_t Array_length(const Array& arr);
string Array_read(const Array& arr);

#endif

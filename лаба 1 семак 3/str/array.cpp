#include "array.h"
#include <iostream>
#include <sstream>
using namespace std;

Array Array_create(size_t capacity) {
    Array arr;
    arr.size = 0;

    if (capacity > 256*256){
        throw length_error("Объём превысил максимум");
    }
    arr.cap = (capacity<16) ? 16 :capacity;
    arr.data = new string[arr.cap];
    return arr;
}

void Array_destroy(Array& arr) {
    delete[] arr.data;
    arr.data = nullptr;
    arr.size = 0;
    arr.cap = 0;
}

void Array_increase_capacity(Array& arr) {
    size_t new_cap = arr.cap ? arr.cap * 2 : 16;
    if (new_cap > 256*256){
        throw length_error("Объём превысил максимум");
    }
    string* new_arr = new string[new_cap];
    for (size_t i = 0; i < arr.size; i++) {
        new_arr[i] = arr.data[i];
    }
    delete[] arr.data;
    arr.data = new_arr;
    arr.cap = new_cap;
}

void Array_test_capacity(Array& arr) {
    if (arr.size >= arr.cap) {
        Array_increase_capacity(arr);
    }
}

void Array_push_back(Array& arr, string value) {
    if (!arr.data){
        return;
    }
    Array_test_capacity(arr);
    arr.data[arr.size++] = value;
}

void Array_insert(Array& arr, size_t idx, string value) {
    if (idx > arr.size) return;
    Array_test_capacity(arr);
    for (size_t i = arr.size; i > idx; i--) {
        arr.data[i] = arr.data[i - 1];
    }
    arr.data[idx] = value;
    arr.size++;
}

string Array_get(const Array& arr, size_t idx) {
    if (idx >= arr.size) throw out_of_range("Array_get: index out of range");
    return arr.data[idx];
}

void Array_erase(Array& arr, size_t idx) {
    if (idx >= arr.size) return;
    for (size_t i = idx; i < arr.size - 1; i++) {
        arr.data[i] = arr.data[i + 1];
    }
    arr.size--;
}

void Array_set(Array& arr, size_t idx, string value) {
    if (idx >= arr.size) return;
    arr.data[idx] = value;
}

size_t Array_length(const Array& arr) {
    return arr.size;
}

string Array_read(const Array& arr){
    stringstream ss;
    for (size_t i = 0;i != Array_length(arr);i++){
        ss << " ";
        ss << Array_get(arr,i);
    }
    return ss.str();
}
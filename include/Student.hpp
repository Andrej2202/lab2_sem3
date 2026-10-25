#pragma once
#include <cstddef>

constexpr size_t MAX_NAME_LEN = 64;
constexpr size_t MAX_GROUP_LEN = 32;

struct Student {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int birthYear;
    char group[MAX_GROUP_LEN];

    Student();
    Student(const char* fName, const char* lName, int year, const char* grp);

    bool operator==(const Student& other) const;
    bool operator<(const Student& other) const;
    bool operator>(const Student& other) const;
};
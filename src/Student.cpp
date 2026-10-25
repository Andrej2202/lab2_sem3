#include "Student.hpp"
#include <cstring>

Student::Student() : birthYear(0) {
    std::memset(firstName, 0, MAX_NAME_LEN);
    std::memset(lastName, 0, MAX_NAME_LEN);
    std::memset(group, 0, MAX_GROUP_LEN);
}

Student::Student(const char* fName, const char* lName, int year, const char* grp)
    : birthYear(year) {
    std::strncpy(firstName, fName ? fName : "", MAX_NAME_LEN - 1);
    firstName[MAX_NAME_LEN - 1] = '\0';

    std::strncpy(lastName, lName ? lName : "", MAX_NAME_LEN - 1);
    lastName[MAX_NAME_LEN - 1] = '\0';

    std::strncpy(group, grp ? grp : "", MAX_GROUP_LEN - 1);
    group[MAX_GROUP_LEN - 1] = '\0';
}

bool Student::operator==(const Student& other) const {
    return birthYear == other.birthYear &&
           std::strcmp(firstName, other.firstName) == 0 &&
           std::strcmp(lastName, other.lastName) == 0 &&
           std::strcmp(group, other.group) == 0;
}

bool Student::operator<(const Student& other) const {
    return birthYear < other.birthYear;
}

bool Student::operator>(const Student& other) const {
    return birthYear > other.birthYear;
}
#include "test_student.hpp"
#include "Student.hpp"
#include <iostream>
#include <cassert>
#include <cstring>

void test_student() {
    std::cout << "=== Тесты Student ===\n";
    {
        Student s;
        assert(s.firstName[0] == '\0');
        assert(s.lastName[0] == '\0');
        assert(s.birthYear == 0);
        assert(s.group[0] == '\0');
        std::cout << "  [OK] Конструктор по умолчанию\n";
    }
    {
        Student s("Иван", "Иванов", 2001, "ИТ-21");
        assert(std::strcmp(s.firstName, "Иван") == 0);
        assert(std::strcmp(s.lastName, "Иванов") == 0);
        assert(s.birthYear == 2001);
        assert(std::strcmp(s.group, "ИТ-21") == 0);
        std::cout << "  [OK] Конструктор с параметрами\n";
    }
    {
        Student s("Тест", "Тестов", 2000, "ТГ-01");
        std::cout << "  [OK] Создание студента с данными\n";
    }
    std::cout << "Все тесты Student пройдены!\n";
}
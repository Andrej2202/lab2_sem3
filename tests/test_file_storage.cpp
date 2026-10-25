#include "test_file_storage.hpp"
#include "FileStorage.hpp"
#include "Student.hpp"
#include "base/Sequence.hpp"
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstring>

void test_file_storage() {
    std::cout << "=== Тесты FileStorage ===\n";
    const std::string testFile = "test_storage.bin";
    std::remove(testFile.c_str());

    {
        FileStorage<Student> storage(testFile);
        Student s1("Иван", "Иванов", 2001, "ИТ-21");
        size_t offset = storage.Write(s1);
        Student r1 = storage.Read(offset);
        
        assert(std::strcmp(r1.firstName, "Иван") == 0);
        assert(std::strcmp(r1.lastName, "Иванов") == 0);
        assert(r1.birthYear == 2001);
        assert(std::strcmp(r1.group, "ИТ-21") == 0);
        std::cout << "  [OK] Запись и чтение одного студента\n";
    }

    {
        FileStorage<Student> storage(testFile);
        storage.Clear();
        
        Student s1("Иван", "Иванов", 2001, "ИТ-21");
        Student s2("Мария", "Петрова", 2002, "ИТ-22");
        Student s3("Алексей", "Сидоров", 2000, "ИТ-21");
        
        size_t idx1 = storage.Write(s1);
        size_t idx2 = storage.Write(s2);
        size_t idx3 = storage.Write(s3);
        
        Student r1 = storage.Read(idx1);
        Student r2 = storage.Read(idx2);
        Student r3 = storage.Read(idx3);
        
        assert(std::strcmp(r1.firstName, s1.firstName) == 0 && r1.birthYear == s1.birthYear);
        assert(std::strcmp(r2.lastName, s2.lastName) == 0 && std::strcmp(r2.group, s2.group) == 0);
        assert(r3.birthYear == s3.birthYear && std::strcmp(r3.firstName, s3.firstName) == 0);
        std::cout << "  [OK] Запись нескольких студентов\n";
    }

    {
        FileStorage<Student> storage(testFile);
        auto students = storage.ReadAll();
        assert(students->GetLength() == 3);
        assert(std::strcmp(students->Get(0).firstName, "Иван") == 0);
        assert(std::strcmp(students->Get(1).firstName, "Мария") == 0);
        assert(std::strcmp(students->Get(2).firstName, "Алексей") == 0);
        delete students;
        std::cout << "  [OK] ReadAll\n";
    }

    {
        FileStorage<Student> storage(testFile);
        storage.Clear();
        auto students = storage.ReadAll();
        assert(students->GetLength() == 0);
        delete students;
        std::cout << "  [OK] Clear\n";
    }

    {
        FileStorage<Student> storage(testFile);
        storage.Clear();
        Student s("", "Тест", 2000, "");
        size_t offset = storage.Write(s);
        Student r = storage.Read(offset);
        
        assert(r.firstName[0] == '\0');
        assert(std::strcmp(r.lastName, "Тест") == 0);
        assert(r.group[0] == '\0');
        std::cout << "  [OK] Пустые строки\n";
    }

    {
        FileStorage<Student> storage(testFile);
        storage.Clear();
        
        char longName[64];
        std::memset(longName, 'A', 63);
        longName[63] = '\0';
        
        Student s(longName, "Test", 2000, "Group");
        size_t offset = storage.Write(s);
        Student r = storage.Read(offset);
        
        assert(std::strcmp(r.firstName, longName) == 0);
        assert(std::strlen(r.firstName) == 63);
        std::cout << "  [OK] Максимальная длина строки (63 символа)\n";
    }

    {
        FileStorage<Student> storage(testFile);
        storage.Clear();
        
        Student s1("A", "B", 2000, "C");
        Student s2("D", "E", 2001, "F");
        storage.Write(s1);
        storage.Write(s2);
        
        assert(storage.GetRecordCount() == 2);
        std::cout << "  [OK] GetRecordCount\n";
    }

    std::remove(testFile.c_str());
    std::cout << "Все тесты FileStorage пройдены!\n";
}
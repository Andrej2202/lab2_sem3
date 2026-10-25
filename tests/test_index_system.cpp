#include "test_index_system.hpp"
#include "IndexSystem.hpp"
#include "Student.hpp"
#include <iostream>
#include <cassert>
#include <fstream>
#include <cstdio>
#include <cstring>

void test_index_system() {
    std::cout << "=== Тесты IndexSystem ===\n";
    const std::string testDb = "test_index.bin";
    const std::string testCsv = "test_index.csv";
    std::remove(testDb.c_str());
    std::remove(testCsv.c_str());

    IndexSystem system(testDb);

    {
        std::cout << "  [OK] Создание системы\n";
    }

    {
        system.AddStudent(Student("Иван", "Иванов", 2001, "ИТ-21"));
        system.AddStudent(Student("Мария", "Петрова", 2002, "ИТ-22"));
        system.AddStudent(Student("Алексей", "Сидоров", 2001, "ИТ-21"));
        auto all = system.GetAllStudents();
        assert(all->GetLength() == 3);
        delete all;
        std::cout << "  [OK] Добавление студента\n";
    }

    {
        auto results = system.SearchByYear(2001, system.GetTreeIndex());
        assert(results->GetLength() == 2);
        
        bool foundIvan = false;
        bool foundAlexey = false;
        for (size_t i = 0; i < results->GetLength(); i++) {
            if (std::strcmp(results->Get(i).firstName, "Иван") == 0) foundIvan = true;
            if (std::strcmp(results->Get(i).firstName, "Алексей") == 0) foundAlexey = true;
        }
        assert(foundIvan && foundAlexey);
        delete results;
        std::cout << "  [OK] Поиск по году (Дерево)\n";
    }

    {
        auto results = system.SearchByYear(2001, system.GetSortedIndex());
        assert(results->GetLength() == 2);
        
        bool foundIvan = false;
        bool foundAlexey = false;
        for (size_t i = 0; i < results->GetLength(); i++) {
            if (std::strcmp(results->Get(i).firstName, "Иван") == 0) foundIvan = true;
            if (std::strcmp(results->Get(i).firstName, "Алексей") == 0) foundAlexey = true;
        }
        assert(foundIvan && foundAlexey);
        delete results;
        std::cout << "  [OK] Поиск по году (Массив)\n";
    }

    {
        auto results = system.SearchByYearRange(2001, 2002, system.GetTreeIndex());
        assert(results->GetLength() == 3);
        delete results;
        std::cout << "  [OK] Поиск по диапазону (Дерево)\n";
    }

    {
        auto results = system.SearchByYearRange(2001, 2002, system.GetSortedIndex());
        assert(results->GetLength() == 3);
        delete results;
        std::cout << "  [OK] Поиск по диапазону (Массив)\n";
    }

    {
        auto resultsTree = system.SearchByYear(1990, system.GetTreeIndex());
        auto resultsSeq = system.SearchByYear(1990, system.GetSortedIndex());
        assert(resultsTree->GetLength() == 0);
        assert(resultsSeq->GetLength() == 0);
        delete resultsTree;
        delete resultsSeq;
        std::cout << "  [OK] Поиск несуществующего года\n";
    }

    {
        system.SetSearchStrategy(IndexSystem::DefaultSplitGolden());
        
        auto results = system.SearchByYear(2001, system.GetSortedIndex());
        assert(results->GetLength() == 2);
        
        bool foundIvan = false;
        bool foundAlexey = false;
        for (size_t i = 0; i < results->GetLength(); i++) {
            if (std::strcmp(results->Get(i).firstName, "Иван") == 0) foundIvan = true;
            if (std::strcmp(results->Get(i).firstName, "Алексей") == 0) foundAlexey = true;
        }
        assert(foundIvan && foundAlexey);
        delete results;

        system.SetSearchStrategy(IndexSystem::DefaultSplitMid());
        std::cout << "  [OK] Поиск с стратегией 'Золотое сечение'\n";
    }

    {
        {
            std::ofstream csv(testCsv);
            csv << "Имя,Фамилия,Год,Группа\n";
            csv << "Петр,Петров,2003,ИТ-23\n";
            csv << "Сергей,Сергеев,2003,ИТ-23\n";
        }
        system.LoadFromCSV(testCsv);
        auto all = system.GetAllStudents();
        assert(all->GetLength() == 5);
        delete all;
        std::cout << "  [OK] Загрузка из CSV\n";
    }

    std::remove(testDb.c_str());
    std::remove(testCsv.c_str());
    std::cout << "Все тесты IndexSystem пройдены!\n";
}
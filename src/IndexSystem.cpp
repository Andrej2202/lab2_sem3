#include "IndexSystem.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <limits>
#include <cstdio>

IndexSystem::IndexSystem(const std::string& dataFilePath)
    : dataFilePath_(dataFilePath)
{
    storage_     = new FileStorage<Student>(dataFilePath);
    treeIndex_   = new BinaryTree<int, size_t>();
    sortedIndex_ = new SortedSequence<Pair<int, size_t>>(DefaultSplitMid());
}

IndexSystem::~IndexSystem() {
    delete storage_;
    delete treeIndex_;
    delete sortedIndex_;
}

void IndexSystem::SetSearchStrategy(SplitFunc func) {
    if (func && sortedIndex_) {
        sortedIndex_->SetSplitFunction(func);
    }
}

Sequence<Student>* IndexSystem::FetchStudents(Sequence<Pair<int, size_t>>* indexResult) const {
    MutableArraySequence<Student>* result = new MutableArraySequence<Student>();
    if (!indexResult) return result;

    size_t len = indexResult->GetLength();
    for (size_t i = 0; i < len; i++) {
        Pair<int, size_t> p = indexResult->Get(i);
        try {
            result->Append(storage_->Read(p.second));
        } catch (const std::exception& e) {
            std::cerr << "Ошибка чтения студента по смещению "
                      << p.second << ": " << e.what() << "\n";
        }
    }
    delete indexResult;
    return result;
}

void IndexSystem::LoadFromCSV(const std::string& csvPath) {
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << csvPath << "\n";
        return;
    }

    std::string line;
    bool headerSkipped = false;
    int count = 0;
    int skipped = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (!headerSkipped) {
            if (line.find("Год") != std::string::npos ||
                line.find("Year") != std::string::npos ||
                line.find("Имя") != std::string::npos ||
                line.find("Name") != std::string::npos) {
                headerSkipped = true;
                continue;
            }
            headerSkipped = true;
        }

        std::stringstream ss(line);
        std::string firstName, lastName, yearStr, group;

        try {
            if (std::getline(ss, firstName, ',') &&
                std::getline(ss, lastName, ',') &&
                std::getline(ss, yearStr, ',') &&
                std::getline(ss, group)) {

                auto trim = [](std::string& s) {
                    while (!s.empty() && (s.back() == '\r' || s.back() == '\n' || s.back() == ' '))
                        s.pop_back();
                };
                trim(firstName); trim(lastName); trim(yearStr); trim(group);

                int year = std::stoi(yearStr);
                Student student(firstName.c_str(), lastName.c_str(), year, group.c_str());
                AddStudent(student);
                count++;
            } else {
                skipped++;
            }
        } catch (const std::exception&) {
            skipped++;
        }
    }

    std::cout << "Загружено " << count << " записей из CSV";
    if (skipped > 0) std::cout << " (пропущено " << skipped << ")";
    std::cout << ".\n";
}

void IndexSystem::AddStudent(const Student& student) {
    try {
        size_t offset = storage_->Write(student);
        Pair<int, size_t> record(student.birthYear, offset);
        treeIndex_->Insert(record);
        sortedIndex_->Add(record);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка добавления студента: " << e.what() << "\n";
        throw;
    }
}


void IndexSystem::ClearDatabase() {
    delete storage_;
    delete treeIndex_;
    delete sortedIndex_;

    storage_     = new FileStorage<Student>(dataFilePath_);
    storage_->Clear();
    treeIndex_   = new BinaryTree<int, size_t>();
    sortedIndex_ = new SortedSequence<Pair<int, size_t>>(DefaultSplitMid());

    std::cout << "База данных очищена.\n";
}

Sequence<Student>* IndexSystem::GetAllStudents() const {
    try {
        return storage_->ReadAll();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка чтения всех студентов: " << e.what() << "\n";
        return new MutableArraySequence<Student>();
    }
}

IndexSystem::BenchmarkResult IndexSystem::RunBenchmark(size_t N, bool isSorted, bool isAscending) const {
    BenchmarkResult result{};
    result.dataSize = N;
    result.dataType = isSorted ? (isAscending ? "Sorted Asc" : "Sorted Desc") : "Random";

    MutableArraySequence<Student>* students = new MutableArraySequence<Student>();
    std::mt19937 gen(42);

    if (isSorted) {
        int startYear = isAscending ? 1990 : 2010;
        int endYear   = isAscending ? 2010 : 1990;
        int step      = isAscending ? 1 : -1;
        int year      = startYear;
        for (size_t i = 0; i < N; i++) {
            Student s("Name", "LastName", year, "Group");
            students->Append(s);
            year += step;
            if ((isAscending && year > endYear) || (!isAscending && year < endYear))
                year = startYear;
        }
    } else {
        std::uniform_int_distribution<> yearDist(1990, 2010);
        for (size_t i = 0; i < N; i++) {
            Student s("Name", "LastName", yearDist(gen), "Group");
            students->Append(s);
        }
    }

    FileStorage<Student> tempStorage("benchmark_temp.bin");
    tempStorage.Clear();
    BinaryTree<int, size_t> tempTree;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < students->GetLength(); i++) {
        const Student& s = students->Get(i);
        size_t offset = tempStorage.Write(s);
        tempTree.Insert(Pair<int, size_t>(s.birthYear, offset));
    }
    auto end = std::chrono::high_resolution_clock::now();
    result.treeBuildTime_ms = std::chrono::duration<double, std::milli>(end - start).count();

    FileStorage<Student> tempStorage2("benchmark_temp2.bin");
    tempStorage2.Clear();
    SortedSequence<Pair<int, size_t>> tempSeqMid(DefaultSplitMid());

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < students->GetLength(); i++) {
        const Student& s = students->Get(i);
        size_t offset = tempStorage2.Write(s);
        tempSeqMid.Add(Pair<int, size_t>(s.birthYear, offset));
    }
    end = std::chrono::high_resolution_clock::now();
    result.sortedBuildTime_ms = std::chrono::duration<double, std::milli>(end - start).count();

    Pair<int, size_t> minP(1995, 0);
    Pair<int, size_t> maxP(2005, std::numeric_limits<size_t>::max());

    start = std::chrono::high_resolution_clock::now();
    auto treeRange = tempTree.GetRange(minP, maxP);
    delete treeRange;
    end = std::chrono::high_resolution_clock::now();
    result.treeSearchTime_ms = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    auto seqRangeMid = tempSeqMid.GetRange(minP, maxP);
    delete seqRangeMid;
    end = std::chrono::high_resolution_clock::now();
    result.sortedSearchTimeMid_ms = std::chrono::duration<double, std::milli>(end - start).count();

    tempSeqMid.SetSplitFunction(DefaultSplitGolden());

    start = std::chrono::high_resolution_clock::now();
    auto seqRangeGolden = tempSeqMid.GetRange(minP, maxP);
    delete seqRangeGolden;
    end = std::chrono::high_resolution_clock::now();
    result.sortedSearchTimeGolden_ms = std::chrono::duration<double, std::milli>(end - start).count();

    delete students;
    std::remove("benchmark_temp.bin");
    std::remove("benchmark_temp2.bin");

    return result;
}

void IndexSystem::RunFullBenchmark(const std::string& outputFile) const {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл " << outputFile << "\n";
        return;
    }

    out << "Тип_данных; N; Построение_Дерево; Построение_Массив; "
        << "Поиск_Дерево; Поиск_Массив_Пополам; Поиск_Массив_Золотое\n";

    size_t sizes[] = {1000, 10000};
    for (size_t N : sizes) {
        std::cout << "Тестирование N=" << N << "...\n";

        auto writeRow = [&](const BenchmarkResult& r) {
            out << r.dataType << "; " << r.dataSize << "; "
                << r.treeBuildTime_ms << "; " << r.sortedBuildTime_ms << "; "
                << r.treeSearchTime_ms << "; "
                << r.sortedSearchTimeMid_ms << "; "
                << r.sortedSearchTimeGolden_ms << "\n";
        };

        std::cout << "  Sorted Asc... " << std::flush;
        writeRow(RunBenchmark(N, true, true));
        std::cout << "готово\n";

        std::cout << "  Sorted Desc... " << std::flush;
        writeRow(RunBenchmark(N, true, false));
        std::cout << "готово\n";

        std::cout << "  Random... " << std::flush;
        writeRow(RunBenchmark(N, false, false));
        std::cout << "готово\n";
    }

    out.close();
    std::cout << "\nБенчмарк завершён. Результаты сохранены в " << outputFile << "\n";
}
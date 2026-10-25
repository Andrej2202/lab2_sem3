#pragma once
#include "Student.hpp"
#include "Pair.hpp"
#include "BinaryTree.hpp"
#include "SortedSequence.hpp"
#include "FileStorage.hpp"
#include "base/Sequence.hpp"
#include "base/MutableArraySequence.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iostream>
#include <random>
#include <algorithm>
#include <limits>
#include <concepts>

template <typename Index>
concept HasGetRange = requires(const Index& idx) {
    { idx.GetRange(Pair<int, size_t>(0, 0), Pair<int, size_t>(0, 0)) }
        -> std::convertible_to<Sequence<Pair<int, size_t>>*>;
};

class IndexSystem {
public:
    using SplitFunc = typename SortedSequence<Pair<int, size_t>>::SplitFunc;

    static SplitFunc DefaultSplitMid() {
        return [](size_t l, size_t r) { return l + (r - l) / 2; };
    }

    static SplitFunc DefaultSplitGolden() {
        return [](size_t l, size_t r) {
            return l + static_cast<size_t>((r - l) * 0.382);
        };
    }

private:
    std::string dataFilePath_;
    FileStorage<Student>* storage_;
    BinaryTree<int, size_t>* treeIndex_;
    SortedSequence<Pair<int, size_t>>* sortedIndex_;

    Sequence<Student>* FetchStudents(Sequence<Pair<int, size_t>>* indexResult) const;

public:
    explicit IndexSystem(const std::string& dataFilePath);
    ~IndexSystem();

    void SetSearchStrategy(SplitFunc func);

    void LoadFromCSV(const std::string& csvPath);
    void AddStudent(const Student& student);
    void ClearDatabase();
    Sequence<Student>* GetAllStudents() const;

    const BinaryTree<int, size_t>& GetTreeIndex() const { return *treeIndex_; }
    const SortedSequence<Pair<int, size_t>>& GetSortedIndex() const { return *sortedIndex_; }

    template <HasGetRange Index>
    Sequence<Student>* SearchByYearRange(int minYear, int maxYear, const Index& index) const;

    template <HasGetRange Index>
    Sequence<Student>* SearchByYear(int year, const Index& index) const;

    struct BenchmarkResult {
        std::string dataType;
        size_t dataSize;
        double treeBuildTime_ms;
        double sortedBuildTime_ms;
        double treeSearchTime_ms;
        double sortedSearchTimeMid_ms;
        double sortedSearchTimeGolden_ms;
    };

    BenchmarkResult RunBenchmark(size_t N, bool isSorted, bool isAscending) const;
    void RunFullBenchmark(const std::string& outputFile) const;
};

#include "IndexSystem.tpp"
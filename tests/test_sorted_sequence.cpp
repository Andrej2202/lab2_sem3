#include "test_sorted_sequence.hpp"
#include "SortedSequence.hpp"
#include <iostream>
#include <cassert>

void test_sorted_sequence() {
    std::cout << "=== Тесты SortedSequence ===\n";

    {
        SortedSequence<int> seq;
        seq.Add(50);
        seq.Add(30);
        seq.Add(70);
        seq.Add(20);
        seq.Add(40);

        assert(seq.GetLength() == 5);
        assert(seq.Get(0) == 20);
        assert(seq.Get(1) == 30);
        assert(seq.Get(2) == 40);
        assert(seq.Get(3) == 50);
        assert(seq.Get(4) == 70);
        std::cout << "  [OK] Добавление и сортировка\n";
    }

    {
        SortedSequence<int> seq(SortedSequence<int>::SplitMid());
        seq.Add(10);
        seq.Add(20);
        seq.Add(30);
        seq.Add(40);
        seq.Add(50);

        assert(seq.IndexOf(30) == 2);
        assert(seq.IndexOf(10) == 0);
        assert(seq.IndexOf(50) == 4);
        assert(seq.IndexOf(99) == -1);
        std::cout << "  [OK] IndexOf (пополам)\n";
    }

    {
        SortedSequence<int> seq(SortedSequence<int>::SplitGolden());
        seq.Add(10);
        seq.Add(20);
        seq.Add(30);
        seq.Add(40);
        seq.Add(50);

        assert(seq.IndexOf(30) == 2);
        assert(seq.IndexOf(10) == 0);
        assert(seq.IndexOf(50) == 4);
        assert(seq.IndexOf(99) == -1);
        std::cout << "  [OK] IndexOf (золотое сечение)\n";
    }

    {
        SortedSequence<int> seq(SortedSequence<int>::SplitMid());
        for (int i = 1; i <= 10; i++) seq.Add(i);

        auto range = seq.GetRange(3, 7);
        assert(range->GetLength() == 5);
        assert(range->Get(0) == 3);
        assert(range->Get(4) == 7);
        delete range;
        std::cout << "  [OK] GetRange (пополам)\n";
    }

    {
        SortedSequence<int> seq(SortedSequence<int>::SplitGolden());
        for (int i = 1; i <= 10; i++) seq.Add(i);

        auto range = seq.GetRange(3, 7);
        assert(range->GetLength() == 5);
        assert(range->Get(0) == 3);
        assert(range->Get(4) == 7);
        delete range;
        std::cout << "  [OK] GetRange (золотое сечение)\n";
    }

    {
        SortedSequence<int> seq;
        seq.Add(10);
        seq.Add(20);
        seq.Add(30);

        auto range = seq.GetRange(50, 100);
        assert(range->GetLength() == 0);
        delete range;
        std::cout << "  [OK] Пустой диапазон\n";
    }

    {
        SortedSequence<int> seq;
        seq.Add(10);
        seq.Add(10);
        seq.Add(10);

        assert(seq.GetLength() == 3);
        assert(seq.IndexOf(10) == 0); 
        std::cout << "  [OK] Дубликаты\n";
    }

    {
        SortedSequence<int> seq(SortedSequence<int>::SplitMid());
        for (int i = 1; i <= 20; i++) seq.Add(i);

        seq.SetSplitFunction(SortedSequence<int>::SplitGolden());
        assert(seq.IndexOf(15) == 14);
        assert(seq.IndexOf(1) == 0);
        assert(seq.IndexOf(20) == 19);
        assert(seq.IndexOf(999) == -1);
        std::cout << "  [OK] Смена стратегии на лету\n";
    }

    {
        SortedSequence<int> seq;
        bool threw = false;
        try {
            seq.Append(10);
        } catch (...) {
            threw = true;
        }
        assert(threw);

        threw = false;
        try {
            seq.Set(0, 10);
        } catch (...) {
            threw = true;
        }
        assert(threw);
        std::cout << "  [OK] Запрещённые операции (Append, Set)\n";
    }

    std::cout << "Все тесты SortedSequence пройдены!\n";
}
#include "test_binary_tree.hpp"
#include "BinaryTree.hpp"
#include "Pair.hpp"
#include <iostream>
#include <cassert>
#include <limits>

void test_binary_tree() {
    std::cout << "=== Тесты BinaryTree ===\n";

    {
        BinaryTree<int, int> tree;
        assert(tree.IsEmpty());
        assert(tree.GetSize() == 0);

        tree.Insert(Pair<int, int>(50, 1));
        tree.Insert(Pair<int, int>(30, 2));
        tree.Insert(Pair<int, int>(70, 3));

        assert(tree.GetSize() == 3);
        assert(!tree.IsEmpty());
        std::cout << "  [OK] Вставка и размер\n";
    }

    {
        BinaryTree<int, int> tree;
        tree.Insert(Pair<int, int>(50, 1));
        tree.Insert(Pair<int, int>(30, 2));
        tree.Insert(Pair<int, int>(70, 3));
        tree.Insert(Pair<int, int>(20, 4));
        tree.Insert(Pair<int, int>(40, 5));
        tree.Insert(Pair<int, int>(60, 6));
        tree.Insert(Pair<int, int>(80, 7));

        int expected[] = {20, 30, 40, 50, 60, 70, 80};
        int i = 0;
        for (auto it = tree.begin(); it != tree.end(); ++it) {
            assert((*it).first == expected[i]);
            i++;
        }
        assert(i == 7);
        std::cout << "  [OK] In-order обход (итератор)\n";
    }

    {
        BinaryTree<int, int> tree;
        tree.Insert(Pair<int, int>(50, 1));
        tree.Insert(Pair<int, int>(30, 2));
        tree.Insert(Pair<int, int>(70, 3));
        tree.Insert(Pair<int, int>(20, 4));
        tree.Insert(Pair<int, int>(40, 5));
        tree.Insert(Pair<int, int>(60, 6));
        tree.Insert(Pair<int, int>(80, 7));

        auto range = tree.GetRange(
            Pair<int, int>(30, 0),
            Pair<int, int>(60, std::numeric_limits<int>::max())
        );

        assert(range->GetLength() == 4);
        assert(range->Get(0).first == 30);
        assert(range->Get(1).first == 40);
        assert(range->Get(2).first == 50);
        assert(range->Get(3).first == 60);
        delete range;
        std::cout << "  [OK] Поиск по диапазону\n";
    }

    {
        BinaryTree<int, int> tree;
        tree.Insert(Pair<int, int>(10, 1));
        tree.Insert(Pair<int, int>(20, 2));
        tree.Insert(Pair<int, int>(30, 3));

        auto range = tree.GetRange(
            Pair<int, int>(50, 0),
            Pair<int, int>(100, std::numeric_limits<int>::max())
        );
        assert(range->GetLength() == 0);
        delete range;
        std::cout << "  [OK] Пустой диапазон\n";
    }

    {
        BinaryTree<int, int> tree;
        tree.Insert(Pair<int, int>(10, 1));
        tree.Insert(Pair<int, int>(10, 2));
        tree.Insert(Pair<int, int>(10, 3));

        assert(tree.GetSize() == 3);

        int count = 0;
        for (auto it = tree.begin(); it != tree.end(); ++it) {
            assert((*it).first == 10);
            count++;
        }
        assert(count == 3);
        std::cout << "  [OK] Дубликаты\n";
    }

    {
        BinaryTree<int, int> tree;
        tree.Insert(Pair<int, int>(1, 1));
        tree.Insert(Pair<int, int>(2, 2));
        tree.Insert(Pair<int, int>(3, 3));
        tree.Insert(Pair<int, int>(4, 4));
        tree.Insert(Pair<int, int>(5, 5));

        int expected[] = {1, 2, 3, 4, 5};
        int i = 0;
        for (auto it = tree.begin(); it != tree.end(); ++it) {
            assert((*it).first == expected[i]);
            i++;
        }
        std::cout << "  [OK] Вырожденное дерево (отсортированные данные)\n";
    }

    std::cout << "Все тесты BinaryTree пройдены!\n";
}
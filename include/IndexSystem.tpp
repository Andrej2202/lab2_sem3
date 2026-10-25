template <HasGetRange Index>
Sequence<Student>* IndexSystem::SearchByYearRange(int minYear, int maxYear, const Index& index) const {
    try {
        auto range = index.GetRange(
            Pair<int, size_t>(minYear, 0),
            Pair<int, size_t>(maxYear, std::numeric_limits<size_t>::max())
        );
        return FetchStudents(range);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка поиска по диапазону: " << e.what() << "\n";
        return new MutableArraySequence<Student>();
    }
}

template <HasGetRange Index>
Sequence<Student>* IndexSystem::SearchByYear(int year, const Index& index) const {
    return SearchByYearRange(year, year, index);
}
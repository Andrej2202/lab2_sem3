import csv
import random

MALE_FIRST_NAMES = [
    "Иван", "Алексей", "Дмитрий", "Сергей", "Андрей", "Михаил",
    "Николай", "Владимир", "Павел", "Артём", "Максим", "Кирилл",
    "Роман", "Егор", "Тимофей", "Матвей", "Даниил", "Фёдор",
    "Илья", "Григорий", "Василий", "Борис", "Виктор", "Олег",
    "Антон", "Денис", "Константин", "Евгений", "Александр", "Никита",
    "Тимур", "Руслан", "Марк", "Лев", "Степан", "Пётр", "Глеб", "Захар"
]

FEMALE_FIRST_NAMES = [
    "Мария", "Елена", "Анна", "Ольга", "Татьяна", "Екатерина",
    "Наталья", "Юлия", "Софья", "Дарья", "Варвара", "Полина",
    "Анастасия", "Алиса", "Виктория", "Ксения", "Марина", "Светлана",
    "Ирина", "Вера", "Надежда", "Людмила", "Галина", "Зинаида",
    "Александра", "Валерия", "Кристина", "Елизавета", "Вероника", "Ульяна",
    "Милана", "Арина", "Карина", "Диана", "Ксения", "Виктория"
]

MALE_LAST_NAMES = [
    "Иванов", "Петров", "Сидоров", "Смирнов", "Кузнецов", "Попов",
    "Васильев", "Зайцев", "Павлов", "Семенов", "Голубев", "Виноградов",
    "Богданов", "Воробьев", "Федоров", "Михайлов", "Новиков", "Морозов",
    "Волков", "Алексеев", "Лебедев", "Соколов", "Козлов", "Новиков",
    "Морозов", "Петров", "Волков", "Алексеев", "Лебедев", "Соколов",
    "Козлов", "Новиков", "Орлов", "Андреев", "Макаров", "Никитин", "Захаров"
]

FEMALE_LAST_NAMES = [name + "а" for name in MALE_LAST_NAMES]

GROUPS = [
    "ИТ-21", "ИТ-22", "ИТ-23",
    "ПМ-21", "ПМ-22",
    "ИБ-21", "ИБ-22",
    "СА-21", "СА-22"
]

MIN_YEAR = 1990
MAX_YEAR = 2010

TOTAL_ROWS = 100000

def generate_student():
    is_male = random.choice([True, False])
    
    if is_male:
        first_name = random.choice(MALE_FIRST_NAMES)
        last_name = random.choice(MALE_LAST_NAMES)
    else:
        first_name = random.choice(FEMALE_FIRST_NAMES)
        last_name = random.choice(FEMALE_LAST_NAMES)
    
    birth_year = random.randint(MIN_YEAR, MAX_YEAR)
    group = random.choice(GROUPS)
    
    return first_name, last_name, birth_year, group

def main():
    output_file = "students_100k.csv"
    
    random.seed(42)
    
    print(f"Генерация {TOTAL_ROWS} записей...")
    
    with open(output_file, mode="w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["Имя", "Фамилия", "Год", "Группа"])
        
        for i in range(TOTAL_ROWS):
            writer.writerow(generate_student())
            if (i + 1) % 10000 == 0:
                print(f"  Сгенерировано: {i + 1}/{TOTAL_ROWS}")
    
    print(f"Готово! Файл сохранён: {output_file}")
    print(f"Размер: {TOTAL_ROWS} строк + 1 заголовок")

if __name__ == "__main__":
    main()
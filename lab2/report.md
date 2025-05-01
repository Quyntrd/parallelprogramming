# Отчет по лабораторной работе №2

## Задание

Модифицировать программу из л/р №1 для параллельной работы по
технологии OpenMP.

## Реализация
Изменен метод `operator*` для поддержки OpenMP

## Результат
Программа была протестирована с использованием CMake в двух версиях Debug-x64 и Release-x64.

Полученные средние времена выполнения перемножения матриц были визуализированы. График зависимости времени выполнения от размерности матрицы:

Время без технологии OpenMP

![График 1, Debug-x64](https://github.com/Quyntrd/parallelprogramming/blob/main/lab1/Graph_1.png)

![График 2, Release-x64](https://github.com/Quyntrd/parallelprogramming/blob/main/lab1/Graph_2.png)

Время с технологией OpenMP

![График 3, Debug-x64](https://github.com/Quyntrd/parallelprogramming/blob/lab2/lab2/Graph_1.png)

![График 4, Release-x64](https://github.com/Quyntrd/parallelprogramming/blob/lab2/lab2/Graph_2.png)
## Вывод
Использование OpenMP в разы улучшило скорость выполнения программы. Время умножения уменьшилось в 2-4 раза. 

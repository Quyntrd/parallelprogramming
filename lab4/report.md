# Отчет по лабораторной работе №4

## Задание

Модифицировать программу из л/р №1 для параллельной работы по
технологии CUDA.

## Реализация
Изменен метод `operator*` для поддержки CUDA. Также изменен CMakeLists.txt для поддержки CUDA.
## Результат
Программа была протестирована с использованием CMake в двух версиях Debug-x64 и Release-x64.

Полученные средние времена выполнения перемножения матриц были визуализированы. График зависимости времени выполнения от размерности матрицы:

Время без технологии CUDA

![График 1, Debug-x64](https://github.com/Quyntrd/parallelprogramming/blob/main/lab1/Graph_1.png)

![График 2, Release-x64](https://github.com/Quyntrd/parallelprogramming/blob/main/lab1/Graph_2.png)

Время с технологией CUDA

![График 3, Debug-x64](https://github.com/Quyntrd/parallelprogramming/blob/lab4/lab4/Graph_1.png)

![График 4, Release-x64](https://github.com/Quyntrd/parallelprogramming/blob/lab4/lab4/Graph_2.png)
## Вывод
Использование CUDA в разы улучшило скорость выполнения программы. Время умножения уменьшилось в десятки раз. Однако на матрицах 100x100 наблюдается скачок во времени выполнения. Также зависимость времени от размера матрицы менее выражена. 

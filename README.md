# Доп Задание
    Недетерминированный конечный автомат задан в виде таблицы переходов. 
    Напишите программу, которая считает эту таблицу из файла и с помощью алгоритма детерминизации 
    строит эквивалентный детерминированный автомат.

## Формат файла с автоматом
    0 1 // Алфавит
    + S0 0:A0 1:A1 // строка состояния
    + S1 0:C 1:A1 // "+" значит что состояние конечно "-" нет
    + A0 0:S0 1:S1 // переход представлен как <символ>:<переход>
    + A1 0:S0 1:S1
    - C 0:B0 1:B0 1:C
    - B0 0:C 1:B1
    - B1 1:B2
    + B2
 
### Создание
    finite_state_automata automata("automata.txt"); //создание конечного автомата где "automata.txt" название файла
    //(если запускать .exe то в файл должен лежать в папке с ним, если просто в ide то в папке cmake-build-debug)

### Методы
    automata.determinization() //детерминизирует данный конечный автомат
    automata.transition_table() //печатает таблицу переходов
    automata.recognize("input string") //определяет проходит слово или нет


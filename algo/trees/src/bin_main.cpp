#include <iostream>
#include "bintree.hpp"
#include "drawtree.hpp"

using namespace std;


int main() {
    BinTree<string, int> jobs;
    jobs.insert("Bard", 99);
    jobs.insert("Paladin", 42);
    jobs.insert("Warrior", 34);
    jobs.insert("Scholar", 76);
    jobs.insert("Sage", 67);
    jobs.insert("Monk", 94);
    jobs.insert("Reaper", 34);
    jobs.insert("Viper", 23);

    DrawTrees<string, int> dtrees;

    // Фиксируем состояние дерева.
    dtrees.push(jobs.get_static());

    // Изменяем объект
    jobs.insert("Scholar", 86);

    // Фиксируем новое состояние дерева
    dtrees.push(jobs.get_static());

    // Поиск
    int *monk = jobs.search("Monk");
    if (monk != nullptr)
        cout << "Уровень Monk: " << *monk << endl;
    else
        cout << "Класс Monk не найден\n";

    // Вывод на экран
    jobs.print();


    // Вспомогательные функции поиска
    auto first = jobs.minimum();
    auto second = jobs.succeser(first.key);
    auto last = jobs.maximum();
    auto prelast = jobs.predecessor(last.key);

    cout << "Сортировка по алфавиту:\n";
    cout << "1.  " << first.key << endl;
    cout << "2.  " << second.key << endl;
    cout << "-2. " << prelast.key << endl;
    cout << "-1. " << last.key << endl;

    while (first.key.length() > 0) {
        cout << "- " << first.key << ' ' << *first.data << endl;
        first = jobs.succeser(first.key);
    }


    jobs.remove("Monk");
    dtrees.push(jobs.get_static());
    jobs.remove("Bard");
    dtrees.push(jobs.get_static());
    jobs.remove("Viper");
    dtrees.push(jobs.get_static());
    jobs.remove("Some invalid key");
    dtrees.push(jobs.get_static());

    jobs.print();
    // Отрисовка всех деревьев
    dtrees.draw();
}

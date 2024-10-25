#include <iostream>
#include "bintree.hpp"
#include "drawtree.hpp"

using namespace std;

int main() {
    BinTree<string, string> phone_book;
    phone_book.insert("Миша", "+706");
    phone_book.insert("Артем", "+702");
    phone_book.insert("Ваня", "+703");
    phone_book.insert("Саша", "+707");
    phone_book.insert("Ира", "+705");
    phone_book.insert("Аня", "+701");
    phone_book.insert("Витя", "+704");
    phone_book.insert("Яна", "+708");
    phone_book.insert("Диоген", "+709");
    phone_book.insert("Вера", "+709");

    auto arr = phone_book.get_static();
    for (int i=0; i<arr.size(); ++i)
        cout << "(" << arr[i].key << ", " << arr[i].data << ") ";
    cout << endl;

    DrawTrees<string, string> dtrees;
    dtrees.push(arr);

    cout << "Витя " << *phone_book.search("Витя") << endl;
    cout << "Аня " << *phone_book.search("Аня") << endl;
    cout << "----\n";

    phone_book.print();
    cout << endl;

    auto first = phone_book.minimum().first;
    auto second = phone_book.succeser(first).first;
    auto last = phone_book.maximum().first;
    auto prelast = phone_book.predecessor(last).first;

    cout << "Первое имя в телефонной книге: " << first << endl;
    cout << "Второе имя в телефонной книге: " << second << endl;
    cout << "Предпоследнее имя в телефонной книге: " << prelast << endl;
    cout << "Последнее имя в телефонной книге: " << last << endl;


    phone_book.remove("Ира");
    dtrees.push(phone_book.get_static());
    phone_book.remove("Артем");
    dtrees.push(phone_book.get_static());
    phone_book.remove("Саша");
    dtrees.push(phone_book.get_static());
    phone_book.remove("Миша");
    phone_book.print();
    cout << endl << endl;

    dtrees.push(phone_book.get_static());
    dtrees.draw();
}

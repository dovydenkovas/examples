#include <QWidget>
#include <QPen>
#include <QPainter>
#include <cstdlib>
#include <qapplication.h>
#include <qbrush.h>
#include <qcontainerfwd.h>
#include <qevent.h>
#include <qimage.h>
#include <qline.h>
#include <qnamespace.h>
#include <qpixmap.h>
#include <qt6/QtCore/qnamespace.h>
#include <qwindowdefs.h>
#include <QApplication>
#include <QPaintEvent>
#include <qclipboard.h>
#include <vector>
#include <math.h>
#include <sstream>
#include <QtGui>

#include "node.hpp"

using std::string;
using std::vector;
using std::pair;

/**
 * Служебный виджет (окно) отрисовки дерева.
 */
template <class K, class T>
class _TreeDrawer;

/**
 * Нарисовать дерево tree в окне title.
 */
template<typename K, typename T>
void draw_tree(vector<Node<K, T>> tree, string title="Дерево");


/**
 * Класс управления выводом нескольких деревьев.
 */
template <class K, class T>
class DrawTrees {
public:
    /**
     * Добавить дерево в список деревьев.
     */
    void push(vector<Node<K, T>> tree, string title="");

    /**
     * Нарисовать деревья
     */
    void draw();

private:
    vector<pair<vector<Node<K, T>>, string>> trees;
};



////////////////////
// Дальше реализация
////////////////////

/**
 * Виджет (окно) отрисовки дерева
 */
template <class K, class T>
class _TreeDrawer: public QWidget {
    vector<Node<K, T>> tree;

public:
    _TreeDrawer(vector<Node<K, T>> tr, string win_title): tree{tr} {
        setWindowTitle(QString::fromStdString(win_title));
        resize(800, 600);
    }

    int get_x(int i, int w) {
        int deep = floor(log2(i+1));
        int nodes_on_level = pow(2, deep);
        int i_on_level = (2*nodes_on_level-i-1);
        return w - i_on_level*w/(nodes_on_level+1);
    }

    void paintEvent(QPaintEvent* event) {
        QPainter p(this);
        p.setPen(QPen(Qt::white, 3));
        p.fillRect(event->rect(), QBrush(Qt::white));


        QPen black_pen(Qt::black, 2);
        QPen red_pen(Qt::red, 2);
        p.setPen(QPen(black_pen));
        p.setFont(QFont("Arial", 12));
        p.setRenderHint(QPainter::Antialiasing);

        int max_deep = ceil(log2(tree.size()));
        int w = event->rect().width();
        int h = event->rect().height();

        int dy = h/(max_deep)/2;
        int r = dy;

        Node<K,T> nil = {};
        for (int i=0; i<tree.size(); i++) {
            if (tree[i] != nil) {
                int deep = floor(log2(i+1));
                int nodes_on_level = pow(2, deep);
                int i_on_level = (2*nodes_on_level-i-1);
                int x = get_x(i, w);
                int y = r+deep*(dy+r);

                if (i > 0) {
                    int parent_x = get_x((i-1)/2, w);
                    int parent_y = r+(deep-1)*(dy+r);
                    p.drawLine(QLine(x, y-r/2, parent_x, parent_y+r/2));
                }

                if (tree[i].color == 'r')
                    p.setPen(QPen(red_pen));
                p.drawEllipse(QRect(x-r/2, y-r/2, r, r));
                p.setPen(QPen(black_pen));
                p.drawText(QRect(x-r/2, y-r/2, r, r), Qt::AlignCenter, QString::fromStdString(tree[i].key));
            }
        }
    }

    void keyPressEvent( QKeyEvent * event ) {
        if( event->key() == Qt::Key_P ) {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setPixmap(this->grab());
        }
        if (event->key() == Qt::Key_Q) {
            exit(0);
        }

    }
};


template<typename K, typename T>
void draw_tree(vector<Node<K, T>> tree, string title) {
    int argc = 0;
    QApplication app(argc, nullptr);
    _TreeDrawer<K, T> drawer{tree, title};
    drawer.show();
    app.exec();
}


template <class K, class T>
void DrawTrees<K, T>::push(vector<Node<K, T>> tree, string title) {
    trees.push_back({tree, title});
}

template <class K, class T>
void DrawTrees<K, T>::draw() {
    int argc = 0;
    QApplication app(argc, nullptr);
    for (int i=0; i<trees.size(); ++i) {
        string title = trees[i].second;
        if (title.length() == 0) {
            std::ostringstream s;
            s << "Дерево " << i;
            title = s.str();
        }

        _TreeDrawer<K,T> *drawer = new _TreeDrawer<K, T>(trees[i].first, title);
        drawer->show();
    }
    app.exec();
}

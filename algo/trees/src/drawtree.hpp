#include <QWidget>
#include <QPen>
#include <QPainter>
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
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>
#include <QtGui>

#include "bintree.hpp"

template <typename K, typename T>
class TreeDrawer: public QWidget {
public:
    std::vector<Node<K, T>> tree;

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
    }
};

template<typename K, typename T>
void draw_tree(std::vector<std::pair<K, T>> tree) {
    int argc = 0;
    QApplication app(argc, nullptr);
    TreeDrawer<K,T> drawer;
    drawer.setWindowTitle("Дерево");
    drawer.tree = tree;

    drawer.resize(800, 600);
    drawer.show();

    app.exec();
}

template <typename K, typename T>
class DrawTrees {
    std::vector<std::vector<Node<K, T>>> trees;
public:
    void push(std::vector<Node<K, T>> tree) {
        trees.push_back(tree);
    }

    void draw() {
        int argc = 0;
        QApplication app(argc, nullptr);
        for (int i=0; i<trees.size(); ++i) {
            TreeDrawer<K,T> *drawer = new TreeDrawer<K, T>;
            std::ostringstream s;
            s << "Дерево " << i;
            drawer->setWindowTitle(QString::fromStdString(s.str()));
            drawer->tree = trees[i];

            drawer->resize(800, 600);
            drawer->show();
        }
        app.exec();
    }
};

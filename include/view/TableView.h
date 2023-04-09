#ifndef TABLE_VIEW_H
#define TABLE_VIEW_H

#include <QTableView>
#include <QHeaderView>

class QMouseEvent;
class QStandardItemModel;

class TableView : public QTableView {
Q_OBJECT
public:
    explicit TableView(QWidget *parent = nullptr);

    explicit TableView(QStandardItemModel *item_model, QWidget *parent = nullptr);

    ~TableView() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QHeaderView *horizontalHeaderView;
};

class HorizontalHeaderView : public QHeaderView {
Q_OBJECT
public:
    explicit HorizontalHeaderView(QWidget *parent = nullptr);

    ~HorizontalHeaderView() override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

};

#endif // TABLE_VIEW_H

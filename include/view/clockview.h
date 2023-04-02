#ifndef CLOCK_VIEW_H
#define CLOCK_VIEW_H

#include <QWidget>
#include "ActionMenu.h"
#include "Settings.h"

namespace Ui {
    class ClockView;
}

enum ClockState {
    WORK,
    PAUSE,
    STOP
};

class ClockView : public QWidget {
Q_OBJECT

public:

    explicit ClockView(QWidget *parent = nullptr, ActionMenu *actionMenu = nullptr, Settings *mSettings = nullptr);

    ~ClockView() override;

signals:

    void pauseClicked();

    void startClicked();

public slots:

    void setText(const QString &text);

    void setSize(int width, int height);

    void setState(ClockState state);

    void setColor(ClockState state, const QColor &color);

    void showContextMenu(const QPoint &point);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::ClockView *ui;
    QPoint oldMousePosition;
    bool isMouseDrag = false;
    ClockState state = WORK;
    QColor colorWork;
    QColor colorPause;
    QColor colorBreak;
    QMenu *menu;
    Settings *settings;
};

#endif // CLOCK_VIEW_H

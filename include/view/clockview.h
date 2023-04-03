#ifndef CLOCK_VIEW_H
#define CLOCK_VIEW_H

#include <QWidget>
#include "ActionMenu.h"
#include "Settings.h"
#include "ClockSettingsFacade.h"

namespace Ui {
    class ClockView;
}

class ClockView : public QWidget {
Q_OBJECT

public:

    enum struct clockStateEnum {
        WORK,
        PAUSE,
        STOP
    };

    explicit ClockView(QWidget *parent = nullptr, ActionMenu *actionMenu = nullptr, Settings *mSettings = nullptr);

    ~ClockView() override;

signals:

    void pauseClicked();

    void startClicked();

public slots:

    void setText(const QString &text);

    void setSize(QSize size);

    void setState(clockStateEnum state);

    void setColor(ClockView::clockStateEnum state, QColor color);

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
    clockStateEnum state;
    Settings::timerColorsStruct colors;
    QMenu *menu;
    Settings *settings;
    ClockSettingsFacade *clockSettingsFacade;
};

#endif // CLOCK_VIEW_H

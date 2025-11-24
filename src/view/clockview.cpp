#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include "menu/ClockViewMenu.h"
#include "ui_clock_view_widget.h"
#include "view/clockview.h"

QT_USE_NAMESPACE

ClockView::ClockView(QWidget *parent, ClockViewMenu *actionMenu, Settings *mSettings) : QWidget(parent),
                                                                                        ui(new Ui::ClockView),
                                                                                        state(clockStateEnum::STOP) {
    settings = mSettings;
    menu = actionMenu->getMenu();
    colors = settings->getTimerColors();

    ui->setupUi(this);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setContextMenuPolicy(Qt::CustomContextMenu);

    QSize windowsSize;
    if (settings->getIsTimerCustomSize()) {
        windowsSize = settings->getTimerWindowCustomSize();
    } else {
        QString sizeStr = settings->getTimerWindowSize();
        windowsSize = settings->getTimerWindowSize(sizeStr);
    }

    setSize(windowsSize);

    QPoint savedPosition = settings->getTimerWindowPosition();
    if (!savedPosition.isNull()) {
        this->move(savedPosition.x(), savedPosition.y());
    }

    ui->lcdDisplay->display("000:00:00");
    connect(this, &ClockView::customContextMenuRequested, this, &ClockView::showContextMenu);
    setState(clockStateEnum::STOP);
}

ClockView::~ClockView() {
    delete ui;
}

void ClockView::setText(const QString &text) {
    ui->lcdDisplay->display(text);
}

void ClockView::setSize(QSize size) {
    this->resize(size);
}

void ClockView::setState(clockStateEnum clockState) {
    state = clockState;

    switch (state) {
        case clockStateEnum::WORK:
            ui->lcdDisplay->setStyleSheet(tr("background-color: rgb(%1, %2, %3);")
                                                  .arg(colors.work.red())
                                                  .arg(colors.work.green())
                                                  .arg(colors.work.blue()));
            break;
        case clockStateEnum::PAUSE:
            ui->lcdDisplay->setStyleSheet(tr("background-color: rgb(%1, %2, %3);")
                                                  .arg(colors.pause.red())
                                                  .arg(colors.pause.green())
                                                  .arg(colors.pause.blue()));
            break;
        case clockStateEnum::STOP:
        default:
            ui->lcdDisplay->setStyleSheet(tr("background-color: rgb(%1, %2, %3);")
                                                  .arg(colors.stop.red())
                                                  .arg(colors.stop.green())
                                                  .arg(colors.stop.blue()));
            break;
    }
}

void ClockView::setColor(ClockView::clockStateEnum clockState, const QColor &color) {
    switch (clockState) {
        case clockStateEnum::WORK:
            colors.work = color;
            break;
        case clockStateEnum::PAUSE:
            colors.pause = color;
            break;
        case clockStateEnum::STOP:
            colors.stop = color;
            break;
        default:
            return;
    }

    if (clockState == state) {
        ui->lcdDisplay->setStyleSheet(tr("background-color: rgb(%1, %2, %3);")
                                              .arg(color.red())
                                              .arg(color.green())
                                              .arg(color.blue()));
    }
}

void ClockView::showContextMenu(const QPoint &point) {
    menu->popup(mapToGlobal(point));
}

void ClockView::mouseMoveEvent(QMouseEvent *event) {
    if (isMouseDrag && event->buttons() & Qt::LeftButton) {
        const QPoint p = this->pos() + (event->globalPos() - oldMousePosition);
        oldMousePosition = event->globalPos();
        move(p);
        event->accept();
    }
}

void ClockView::mousePressEvent(QMouseEvent *event) {

    if (event->buttons() & Qt::LeftButton) {
        oldMousePosition = event->globalPos();
    }

    isMouseDrag = true;
    event->accept();
}

void ClockView::mouseReleaseEvent(QMouseEvent *event) {
    isMouseDrag = false;
    settings->setTimerWindowPosition(this->pos());
    event->accept();
}

void ClockView::mouseDoubleClickEvent(QMouseEvent *event) {

    if (state == clockStateEnum::WORK) {
        emit pauseClicked();
    } else if (state == clockStateEnum::PAUSE) {
        emit startClicked();
    }
}

void ClockView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {

        if (state == clockStateEnum::WORK) {
            emit pauseClicked();
        } else if (state == clockStateEnum::PAUSE) {
            emit startClicked();
        }

    }
}

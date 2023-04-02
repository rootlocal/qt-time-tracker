#include "view/clockview.h"
#include "ui_clockview.h"
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>

ClockView::ClockView(QWidget *parent, ActionMenu *actionMenu, Settings *mSettings) : QWidget(parent),
                                                                                     ui(new Ui::ClockView) {
    settings = mSettings;
    menu = actionMenu->getMenu();
    colorWork = settings->getColorWork();
    colorPause = settings->getColorPause();
    colorBreak = settings->getColorStop();

    ui->setupUi(this);
    ui->lcdDisplay->display("000:00:00");
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setState(STOP);
    setSize(124, 28);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(showContextMenu(QPoint)));
}

ClockView::~ClockView() {
    delete ui;
}

void ClockView::setText(const QString &text) {
    ui->lcdDisplay->display(text);
}

void ClockView::setSize(int width, int height) {
    this->resize(QSize(width, height));
}

void ClockView::setState(ClockState clockState) {
    state = clockState;

    switch (state) {
        case WORK:
            ui->lcdDisplay->setStyleSheet(tr("background-color: rgb(%1, %2, %3);")
                                                  .arg(colorWork.red())
                                                  .arg(colorWork.green())
                                                  .arg(colorWork.blue()));
            break;
        case PAUSE:
            ui->lcdDisplay->setStyleSheet(tr("background-color: rgb(%1, %2, %3);")
                                                  .arg(colorPause.red())
                                                  .arg(colorPause.green())
                                                  .arg(colorPause.blue()));
            break;
        case STOP:
        default:
            ui->lcdDisplay->setStyleSheet(tr("background-color: rgb(%1, %2, %3);")
                                                  .arg(colorBreak.red())
                                                  .arg(colorBreak.green())
                                                  .arg(colorBreak.blue()));
            break;
    };
}

void ClockView::setColor(ClockState clockState, const QColor &color) {
    switch (clockState) {
        case WORK:
            colorWork = color;
            break;
        case PAUSE:
            colorPause = color;
            break;
        case STOP:
            colorBreak = color;
            break;
        default:
            return;
    };

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
    event->accept();
}

void ClockView::mouseDoubleClickEvent(QMouseEvent *event) {

    if (state == WORK) {
        emit pauseClicked();
    } else if (state == PAUSE) {
        emit startClicked();
    }
}

void ClockView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {

        if (state == WORK) {
            emit pauseClicked();
        } else if (state == PAUSE) {
            emit startClicked();
        }

    }
}

#include "view/clockview.h"
#include "ui_clockview.h"
#include <QMouseEvent>

QT_USE_NAMESPACE

ClockView::ClockView(QWidget *parent, ActionMenu *actionMenu, Settings *mSettings) : QWidget(parent),
                                                                                     ui(new Ui::ClockView) {
    settings = mSettings;
    clockSettingsFacade = new ClockSettingsFacade(this, settings);
    menu = actionMenu->getMenu();
    colors = settings->getTimerColors();

    ui->setupUi(this);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setSize(clockSettingsFacade->getWindowsSize());

    QPoint savedPosition = settings->getTimerWindowPosition();
    if (!savedPosition.isNull()) {
        this->move(savedPosition.x(), savedPosition.y());
    }

    ui->lcdDisplay->display("000:00:00");

    setState(STOP);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(showContextMenu(QPoint)));
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

void ClockView::setState(ClockState clockState) {
    state = clockState;

    switch (state) {
        case WORK:
            ui->lcdDisplay->setStyleSheet(tr("background-color: rgb(%1, %2, %3);")
                                                  .arg(colors.work.red())
                                                  .arg(colors.work.green())
                                                  .arg(colors.work.blue()));
            break;
        case PAUSE:
            ui->lcdDisplay->setStyleSheet(tr("background-color: rgb(%1, %2, %3);")
                                                  .arg(colors.pause.red())
                                                  .arg(colors.pause.green())
                                                  .arg(colors.pause.blue()));
            break;
        case STOP:
        default:
            ui->lcdDisplay->setStyleSheet(tr("background-color: rgb(%1, %2, %3);")
                                                  .arg(colors.stop.red())
                                                  .arg(colors.stop.green())
                                                  .arg(colors.stop.blue()));
            break;
    }
}

void ClockView::setColor(ClockState clockState, const QColor &color) {
    switch (clockState) {
        case WORK:
            colors.work = color;
            break;
        case PAUSE:
            colors.pause = color;
            break;
        case STOP:
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
        settings->setTimerWindowPosition(this->pos());
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

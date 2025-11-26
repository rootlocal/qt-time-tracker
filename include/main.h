#ifndef TRACKER_MAIN_H
#define TRACKER_MAIN_H

QScopedPointer<QFile> logFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // TRACKER_MAIN_H

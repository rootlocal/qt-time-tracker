#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include "helpers/FileHelper.h"

QString FileHelper::getConfigPath() {
    if (configPath.isEmpty()) {
        configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

        if (!QDir(configPath).exists()) {
            qInfo() << "Creating config directory: " << configPath;
            QDir().mkdir(configPath);
        }
    }

    return configPath;
}

QString FileHelper::getTaskDbFile() {
    if (taskDbFile.isEmpty()) {
        taskDbFile = getConfigPath() + QDir::separator() + "tasks.db";
    }

    return taskDbFile;
}

QString FileHelper::getLogFile() {
    if (logFile.isEmpty()) {
        QString path = getConfigPath() + QDir::separator() + "log";

        if (!QDir(path).exists()) {
            qInfo() << "Creating log directory: " << path;
            QDir().mkdir(path);
        }

        logFile = path + QDir::separator() + "app.log";
    }

    return logFile;
}

QString FileHelper::getBackupDir() {
    if (backupDIr.isEmpty()) {
        backupDIr = getConfigPath() + QDir::separator() + "backups";

        if (!QDir(backupDIr).exists()) {
            qInfo() << "Creating backup directory: " << backupDIr;
            QDir().mkdir(backupDIr);
        }
    }

    return backupDIr;
}

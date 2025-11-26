#include <QDir>
#include <QDateTime>
#include <QDebug>
#include "Utils.h"
#include "helpers/FileHelper.h"

Utils::Utils(QObject *parent) {

}

bool Utils::backupDatabase() {

    QFileInfo fileInfo((FileHelper::getTaskDbFile()));
    if (fileInfo.exists()) {
        QString backupFile = QString("%1%2backup_task_%3.db")
                .arg(FileHelper::getBackupDir())
                .arg(QDir::separator())
                .arg(fileInfo.lastModified().toSecsSinceEpoch());

        if (!QFile(backupFile).exists()) {
            qDebug() << "Backup database task: " << backupFile;
            bool is_backup = QFile(FileHelper::getTaskDbFile()).copy(backupFile);
            cleanBackupDatabase();
            return is_backup;
        }
    }
    cleanBackupDatabase();
    return false;
}

bool Utils::cleanBackupDatabase() {
    QString backupNameDir = FileHelper::getBackupDir();
    QDir dir(backupNameDir);
    dir.setSorting(QDir::Time);
    QStringList files = dir.entryList(QStringList("*.db"), QDir::Files | QDir::NoSymLinks);

    if (files.count() > MAX_BACKUPS_DB_TASK_FILES) {
        qDebug() << "Clean backups Database";

        int i = 1;
        for (const QString &file: files) {
            if (i > MAX_BACKUPS_DB_TASK_FILES) {
                QFile f(QString("%1%2%3").arg(backupNameDir).arg(QDir::separator()).arg(file));
                qDebug() << "Delete backup file" << i << ":" << f.fileName();
                f.remove();
            }

            i++;
        }

    }

    return false;
}
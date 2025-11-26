#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <QObject>

class FileHelper : public QObject {
Q_OBJECT

public:
    static QString getConfigPath();

    static QString getTaskDbFile();

    static QString getLogFile();

    static QString getBackupDir();

private:
    static inline QString configPath{};
    static inline QString taskDbFile{};
    static inline QString logFile{};
    static inline QString backupDIr{};
};


#endif //FILE_HELPER_H

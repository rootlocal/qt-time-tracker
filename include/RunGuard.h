#ifndef RUN_GUARD_H
#define RUN_GUARD_H


#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

class RunGuard {

public:
    explicit RunGuard(const QString &key);

    ~RunGuard();

    bool isAnotherRunning();

    bool tryToRun();

    void release();

private:
    const QString key;
    const QString memLockKey;
    const QString sharedMemKey;

    QSharedMemory sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY(RunGuard)
};

#endif //RUN_GUARD_H

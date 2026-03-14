#include <QTest>
#include <QThread>
#include <QElapsedTimer>
#include <QAtomicInt>

#include <Kanoop/mutexevent.h>
#include <Kanoop/lockingqueue.h>
#include <Kanoop/ratemonitor.h>

class TstConcurrency : public QObject
{
    Q_OBJECT

private slots:
    // ---- MutexEvent ----

    void mutexEvent_setThenWait()
    {
        MutexEvent event;
        event.set();
        QVERIFY(event.wait(100));
    }

    void mutexEvent_waitThenSet()
    {
        MutexEvent event;
        bool result = false;

        QThread *waiter = QThread::create([&]() {
            result = event.wait(2000);
        });
        waiter->start();

        QThread::msleep(50);
        QVERIFY(event.isWaiting());
        event.set();
        waiter->wait();

        QVERIFY(result);
        QVERIFY(!event.isWaiting());
        delete waiter;
    }

    void mutexEvent_waitTimeout()
    {
        MutexEvent event;
        QElapsedTimer timer;
        timer.start();

        bool result = event.wait(50);

        QVERIFY(!result);
        QVERIFY(timer.elapsed() >= 40); // allow small timing slack
    }

    void mutexEvent_autoReset()
    {
        MutexEvent event;
        event.set();
        QVERIFY(event.wait(100));

        // Event should have auto-reset — second wait should timeout
        QVERIFY(!event.wait(50));
    }

    void mutexEvent_clearResetsSignal()
    {
        MutexEvent event;
        event.set();
        event.clear();

        QVERIFY(!event.wait(50));
    }

    void mutexEvent_isWaitingFlag()
    {
        MutexEvent event;
        QVERIFY(!event.isWaiting());

        QThread *waiter = QThread::create([&]() {
            event.wait(2000);
        });
        waiter->start();

        QThread::msleep(50);
        QVERIFY(event.isWaiting());

        event.set();
        waiter->wait();

        QVERIFY(!event.isWaiting());
        delete waiter;
    }

    void mutexEvent_dataPayload()
    {
        MutexEvent event;
        QVariant received;

        QThread *waiter = QThread::create([&]() {
            event.wait(2000);
            received = event.data();
        });
        waiter->start();

        QThread::msleep(50);
        event.setData(QVariant(42));
        event.set();
        waiter->wait();

        QCOMPARE(received.toInt(), 42);
        delete waiter;
    }

    void mutexEvent_wakeAllWaiters()
    {
        MutexEvent event(true); // wakeAll mode
        QAtomicInt wokenCount(0);

        auto makeWaiter = [&]() {
            return QThread::create([&]() {
                if (event.wait(2000)) {
                    wokenCount.fetchAndAddRelaxed(1);
                }
            });
        };

        QThread *w1 = makeWaiter();
        QThread *w2 = makeWaiter();
        QThread *w3 = makeWaiter();
        w1->start();
        w2->start();
        w3->start();

        QThread::msleep(100); // let all threads enter wait()
        event.set();

        w1->wait();
        w2->wait();
        w3->wait();

        QCOMPARE(wokenCount.loadRelaxed(), 3);
        delete w1;
        delete w2;
        delete w3;
    }

    void mutexEvent_wakeSingleWaiter()
    {
        MutexEvent event; // default: wake single
        QAtomicInt wokenCount(0);

        auto makeWaiter = [&]() {
            return QThread::create([&]() {
                if (event.wait(200)) {
                    wokenCount.fetchAndAddRelaxed(1);
                }
            });
        };

        QThread *w1 = makeWaiter();
        QThread *w2 = makeWaiter();
        QThread *w3 = makeWaiter();
        w1->start();
        w2->start();
        w3->start();

        QThread::msleep(100);
        event.set(); // should wake exactly 1

        // Wait for all threads to finish (2 will timeout at 200ms)
        w1->wait();
        w2->wait();
        w3->wait();

        QCOMPARE(wokenCount.loadRelaxed(), 1);
        delete w1;
        delete w2;
        delete w3;
    }

    // ---- LockingQueue ----

    void lockingQueue_enqueueDequeue()
    {
        LockingQueue<int> queue;
        queue.enqueue(10);
        queue.enqueue(20);
        queue.enqueue(30);

        bool ok = false;
        QCOMPARE(queue.dequeue(100, ok), 10);
        QVERIFY(ok);
        QCOMPARE(queue.dequeue(100, ok), 20);
        QVERIFY(ok);
        QCOMPARE(queue.dequeue(100, ok), 30);
        QVERIFY(ok);
    }

    void lockingQueue_dequeueTimeout()
    {
        LockingQueue<int> queue;
        bool ok = true;
        QElapsedTimer timer;
        timer.start();

        int result = queue.dequeue(50, ok);

        QVERIFY(!ok);
        QCOMPARE(result, 0); // default-constructed int
        QVERIFY(timer.elapsed() >= 40);
    }

    void lockingQueue_producerConsumer()
    {
        LockingQueue<int> queue;
        const int itemCount = 100;
        QList<int> received;
        QMutex receivedLock;

        // 2 producers, each enqueuing half the items
        QThread *p1 = QThread::create([&]() {
            for (int i = 0; i < itemCount / 2; ++i) {
                queue.enqueue(i);
                QThread::usleep(100);
            }
        });
        QThread *p2 = QThread::create([&]() {
            for (int i = itemCount / 2; i < itemCount; ++i) {
                queue.enqueue(i);
                QThread::usleep(100);
            }
        });

        // 1 consumer — keep dequeuing until all items received
        QThread *consumer = QThread::create([&]() {
            while (true) {
                {
                    QMutexLocker locker(&receivedLock);
                    if (received.count() >= itemCount) break;
                }
                bool ok = false;
                int val = queue.dequeue(500, ok);
                if (ok) {
                    QMutexLocker locker(&receivedLock);
                    received.append(val);
                }
            }
        });

        consumer->start();
        p1->start();
        p2->start();

        p1->wait();
        p2->wait();
        consumer->wait();

        QCOMPARE(received.count(), itemCount);

        // Verify all items present (order may vary due to 2 producers)
        std::sort(received.begin(), received.end());
        for (int i = 0; i < itemCount; ++i) {
            QCOMPARE(received.at(i), i);
        }

        delete p1;
        delete p2;
        delete consumer;
    }

    void lockingQueue_multiConsumer()
    {
        LockingQueue<int> queue;
        const int itemCount = 90; // divisible by 3 consumers
        QList<int> received;
        QMutex receivedLock;

        // 1 producer
        QThread *producer = QThread::create([&]() {
            for (int i = 0; i < itemCount; ++i) {
                queue.enqueue(i);
                QThread::usleep(50);
            }
        });

        auto makeConsumer = [&]() {
            return QThread::create([&]() {
                while (true) {
                    {
                        QMutexLocker locker(&receivedLock);
                        if (received.count() >= itemCount) break;
                    }
                    bool ok = false;
                    int val = queue.dequeue(500, ok);
                    if (ok) {
                        QMutexLocker locker(&receivedLock);
                        received.append(val);
                    }
                }
            });
        };

        QThread *c1 = makeConsumer();
        QThread *c2 = makeConsumer();
        QThread *c3 = makeConsumer();

        c1->start();
        c2->start();
        c3->start();
        producer->start();

        producer->wait();
        c1->wait();
        c2->wait();
        c3->wait();

        QCOMPARE(received.count(), itemCount);

        // All items present, no duplicates
        std::sort(received.begin(), received.end());
        for (int i = 0; i < itemCount; ++i) {
            QCOMPARE(received.at(i), i);
        }

        delete producer;
        delete c1;
        delete c2;
        delete c3;
    }

    // ---- RateMonitor ----

    void rateMonitor_zeroEvents()
    {
        RateMonitor monitor;
        QCOMPARE(monitor.eventsPerSecond(), 0.0);
    }

    void rateMonitor_knownRate()
    {
        RateMonitor monitor(500); // 500ms window

        // Add 10 events over ~100ms
        for (int i = 0; i < 10; ++i) {
            monitor.addEvent();
            QThread::msleep(10);
        }

        double rate = monitor.eventsPerSecond();
        // 10 events in ~100ms ≈ 100 events/sec, but window is 500ms
        // Accept a wide range due to timing imprecision
        QVERIFY2(rate > 10.0, qPrintable(QString("Rate too low: %1").arg(rate)));
        QVERIFY2(rate < 200.0, qPrintable(QString("Rate too high: %1").arg(rate)));
    }

    void rateMonitor_evaluationWindow()
    {
        RateMonitor monitor(200); // 200ms window

        // Add events
        for (int i = 0; i < 5; ++i) {
            monitor.addEvent();
        }

        // Wait for events to age out of the window
        QThread::msleep(300);

        double rate = monitor.eventsPerSecond();
        QCOMPARE(rate, 0.0);
    }

    void rateMonitor_bulkEvents()
    {
        RateMonitor monitor(1000);
        monitor.addEvent(100); // add 100 at once

        double rate = monitor.eventsPerSecond();
        QVERIFY2(rate >= 50.0, qPrintable(QString("Rate too low: %1").arg(rate)));
    }
};

QTEST_MAIN(TstConcurrency)
#include "tst_concurrency.moc"

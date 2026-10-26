#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>
#include <QTest>

#include <Kanoop/fileutil.h>

#ifdef Q_OS_UNIX
#include <unistd.h>
#endif

class TstFileUtil : public QObject
{
    Q_OBJECT

private:
    static QByteArray readBack(const QString& path)
    {
        QByteArray result;
        if(FileUtil::readAllBytes(path, result) == false) {
            result = "<unreadable>";
        }
        return result;
    }

private slots:

    void writeAllBytes_replacesWholeContent()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        const QString path = dir.filePath("a.txt");

        QVERIFY(FileUtil::writeAllBytes(path, "old content that is long"));
        QCOMPARE(readBack(path), QByteArray("old content that is long"));
        QVERIFY(FileUtil::writeAllBytes(path, "new"));
        QCOMPARE(readBack(path), QByteArray("new"));
    }

    void writeAllBytes_leavesNoTemporaryFile()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        QVERIFY(FileUtil::writeAllBytes(dir.filePath("a.txt"), "one"));
        QVERIFY(FileUtil::writeAllBytes(dir.filePath("a.txt"), "two"));
        QCOMPARE(QDir(dir.path()).entryList(QDir::Files | QDir::Hidden), QStringList({ "a.txt" }));
    }

    void writeAllBytes_readOnlyDirectoryWritesInPlace()
    {
#ifdef Q_OS_UNIX
        if(geteuid() == 0) {
            QSKIP("Directory permissions do not bind root");
        }
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        const QString sub = dir.filePath("r");
        QVERIFY(QDir().mkpath(sub));
        const QString path = QDir(sub).filePath("a.txt");
        QVERIFY(FileUtil::writeAllBytes(path, "old"));

        const QFileDevice::Permissions original = QFile::permissions(sub);
        QVERIFY(QFile::setPermissions(sub, QFileDevice::ReadOwner | QFileDevice::ExeOwner));
        const bool written = FileUtil::writeAllBytes(path, "new");
        const QByteArray content = readBack(path);
        QFile::setPermissions(sub, original);

        QVERIFY(written);
        QCOMPARE(content, QByteArray("new"));
#else
        QSKIP("Read-only directory semantics are POSIX-specific");
#endif
    }

    void writeAllBytes_directoryTargetFails()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        QVERIFY(QDir().mkpath(dir.filePath("target")));
        const QStringList before = QDir(dir.path()).entryList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);

        QCOMPARE(FileUtil::writeAllBytes(dir.filePath("target"), "x"), false);
        QVERIFY(QFileInfo(dir.filePath("target")).isDir());
        QCOMPARE(QDir(dir.path()).entryList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot), before);
    }

    void writeAllBytes_missingDirectoryFails()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        QCOMPARE(FileUtil::writeAllBytes(dir.filePath("no/such/a.txt"), "x"), false);
        QVERIFY(QFileInfo::exists(dir.filePath("no")) == false);
    }

    void writeAllBytes_encodingOverloadEncodes()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        const QString path = dir.filePath("e.txt");
        const QByteArray text = QString("ü").toUtf8();

        QVERIFY(FileUtil::writeAllBytes(path, text, QStringConverter::Utf8));
        QCOMPARE(readBack(path), QByteArray("\xC3\xBC"));
        QVERIFY(FileUtil::writeAllBytes(path, text, QStringConverter::Latin1));
        QCOMPARE(readBack(path), QByteArray("\xFC"));
    }

    void writeAllLines_writesLatin1()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        const QString path = dir.filePath("l.txt");

        QVERIFY(FileUtil::writeAllLines(path, { "first", "second", "third" }));
        QVERIFY(FileUtil::writeAllLines(path, { "a", QString("é"), QString("€") }));
        QCOMPARE(readBack(path), QByteArray("a\n\xE9\n?\n"));
    }
};

QTEST_MAIN(TstFileUtil)
#include "tst_fileutil.moc"

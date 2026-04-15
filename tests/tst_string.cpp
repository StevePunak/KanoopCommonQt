#include <QTest>
#include <QUuid>

#include <Kanoop/stringutil.h>

class TstString : public QObject
{
    Q_OBJECT

private slots:

    // ---- toBool ----

    void toBool_trueStrings()
    {
        QVERIFY(StringUtil::toBool(QStringLiteral("true")));
        QVERIFY(StringUtil::toBool(QStringLiteral("True")));
        QVERIFY(StringUtil::toBool(QStringLiteral("TRUE")));
        QVERIFY(StringUtil::toBool(QStringLiteral("1")));
    }

    void toBool_falseStrings()
    {
        QVERIFY(!StringUtil::toBool(QStringLiteral("false")));
        QVERIFY(!StringUtil::toBool(QStringLiteral("0")));
        QVERIFY(!StringUtil::toBool(QStringLiteral("yes")));
        QVERIFY(!StringUtil::toBool(QStringLiteral("")));
        QVERIFY(!StringUtil::toBool(QString()));
    }

    void toBool_int()
    {
        QVERIFY(StringUtil::toBool(1));
        QVERIFY(StringUtil::toBool(-1));
        QVERIFY(StringUtil::toBool(42));
        QVERIFY(!StringUtil::toBool(0));
    }

    // ---- toString(bool) ----

    void toString_bool()
    {
        QCOMPARE(StringUtil::toString(true), QStringLiteral("true"));
        QCOMPARE(StringUtil::toString(false), QStringLiteral("false"));
    }

    // ---- toString(double) ----

    void toString_double_defaultPrecision()
    {
        // Trailing zeros should be trimmed
        QCOMPARE(StringUtil::toString(1.0), QStringLiteral("1"));
        QCOMPARE(StringUtil::toString(3.14), QStringLiteral("3.14"));
        QCOMPARE(StringUtil::toString(0.0), QStringLiteral("0"));
    }

    void toString_double_customPrecision()
    {
        QString result = StringUtil::toString(1.23456789, 2);
        QCOMPARE(result, QStringLiteral("1.23"));
    }

    void toString_double_negative()
    {
        QString result = StringUtil::toString(-42.5);
        QCOMPARE(result, QStringLiteral("-42.5"));
    }

    void toString_double_forcePadding()
    {
        // With forcePadding, trailing zeros out to the full precision are retained
        QCOMPARE(StringUtil::toString(1.0, 6, true), QStringLiteral("1.000000"));
        QCOMPARE(StringUtil::toString(3.14, 4, true), QStringLiteral("3.1400"));
        QCOMPARE(StringUtil::toString(0.0, 3, true), QStringLiteral("0.000"));
        QCOMPARE(StringUtil::toString(-42.5, 2, true), QStringLiteral("-42.50"));
    }

    void toString_double_forcePadding_zeroPrecision()
    {
        // With precision 0, there is no decimal portion regardless of forcePadding
        QCOMPARE(StringUtil::toString(1.0, 0, true), QStringLiteral("1"));
        QCOMPARE(StringUtil::toString(1.0, 0, false), QStringLiteral("1"));
    }

    // ---- toByteArray ----

    void toByteArray_hexString()
    {
        // toByteArray parses space-delimited hex byte strings
        QByteArray ba = StringUtil::toByteArray(QStringLiteral("48 65 6c 6c 6f"));
        QCOMPARE(ba, QByteArray("Hello"));
    }

    void toByteArray_singleByte()
    {
        QByteArray ba = StringUtil::toByteArray(QStringLiteral("FF"));
        QCOMPARE(ba.size(), 1);
        QCOMPARE(static_cast<uint8_t>(ba.at(0)), uint8_t(0xFF));
    }

    void toByteArray_empty()
    {
        QByteArray ba = StringUtil::toByteArray(QString());
        QVERIFY(ba.isEmpty());
    }

    // ---- toString(QByteArray) ----

    void toString_byteArray_defaultDelimiter()
    {
        QByteArray data;
        data.append('\xAB');
        data.append('\xCD');
        data.append('\x01');
        QString result = StringUtil::toString(data);
        // Each byte as two-char hex separated by space
        QVERIFY(result.contains("ab", Qt::CaseInsensitive));
        QVERIFY(result.contains("cd", Qt::CaseInsensitive));
        QVERIFY(result.contains("01", Qt::CaseInsensitive));
    }

    void toString_byteArray_customDelimiter()
    {
        QByteArray data;
        data.append('\x0A');
        data.append('\x0B');
        QString result = StringUtil::toString(data, QStringLiteral(":"));
        QVERIFY(result.contains(':'));
    }

    // ---- toHexTable ----

    void toHexTable_basic()
    {
        QByteArray data(32, '\x41'); // 32 'A' characters
        QString table = StringUtil::toHexTable(data);
        // Should have hex and ASCII text columns
        QVERIFY(table.contains("41"));
        QVERIFY(table.contains('A'));
    }

    void toHexTable_empty()
    {
        QString table = StringUtil::toHexTable(QByteArray());
        // Empty buffer should produce empty or minimal output
        QVERIFY(table.isEmpty() || table.trimmed().isEmpty());
    }

    // ---- UUID round-trip ----

    void uuidRoundTrip_single()
    {
        QUuid id = QUuid::createUuid();
        QList<QUuid> input = { id };
        QString str = StringUtil::toString(input);
        QList<QUuid> output = StringUtil::uuidsFromString(str);
        QCOMPARE(output.size(), 1);
        QCOMPARE(output.first(), id);
    }

    void uuidRoundTrip_multiple()
    {
        QList<QUuid> input = {
            QUuid::createUuid(),
            QUuid::createUuid(),
            QUuid::createUuid()
        };
        QString str = StringUtil::toString(input, QStringLiteral(","));
        QList<QUuid> output = StringUtil::uuidsFromString(str, QStringLiteral(","));
        QCOMPARE(output.size(), input.size());
        for (int i = 0; i < input.size(); ++i) {
            QCOMPARE(output.at(i), input.at(i));
        }
    }

    void uuidRoundTrip_empty()
    {
        QList<QUuid> input;
        QString str = StringUtil::toString(input);
        QList<QUuid> output = StringUtil::uuidsFromString(str);
        QVERIFY(output.isEmpty());
    }

    // ---- toDelimitedString ----

    void toDelimitedString_stringList()
    {
        QStringList list = { "alpha", "beta", "gamma" };
        QCOMPARE(StringUtil::toDelimitedString(list, ','), QStringLiteral("alpha,beta,gamma"));
    }

    void toDelimitedString_stringList_defaultDelimiter()
    {
        QStringList list = { "one", "two" };
        QCOMPARE(StringUtil::toDelimitedString(list), QStringLiteral("one two"));
    }

    void toDelimitedString_intList()
    {
        QList<int> list = { 10, 20, 30 };
        QCOMPARE(StringUtil::toDelimitedString(list, ','), QStringLiteral("10,20,30"));
    }

    void toDelimitedString_uint8List()
    {
        QList<uint8_t> list = { 0, 128, 255 };
        QCOMPARE(StringUtil::toDelimitedString(list, '-'), QStringLiteral("0-128-255"));
    }

    void toDelimitedString_empty()
    {
        QCOMPARE(StringUtil::toDelimitedString(QStringList()), QString());
    }

    // ---- toKMG ----

    void toKMG_bytes()
    {
        QString result = StringUtil::toKMG(500);
        QVERIFY(result.contains("500"));
    }

    void toKMG_kilobytes()
    {
        QString result = StringUtil::toKMG(1024);
        QVERIFY(result.contains('K') || result.contains('k'));
    }

    void toKMG_megabytes()
    {
        QString result = StringUtil::toKMG(1048576); // 1 MB
        QVERIFY(result.contains('M'));
    }

    void toKMG_gigabytes()
    {
        QString result = StringUtil::toKMG(Q_INT64_C(1073741824)); // 1 GB
        QVERIFY(result.contains('G'));
    }

    // ---- trimFront / trimEnd / trimBothEnds ----

    void trimFront_basic()
    {
        QList<QChar> chars = { ' ', '-' };
        QCOMPARE(StringUtil::trimFront(QStringLiteral("--  hello"), chars), QStringLiteral("hello"));
    }

    void trimFront_nothingToTrim()
    {
        QList<QChar> chars = { '-' };
        QCOMPARE(StringUtil::trimFront(QStringLiteral("hello"), chars), QStringLiteral("hello"));
    }

    void trimEnd_whitespace()
    {
        QCOMPARE(StringUtil::trimEnd(QStringLiteral("hello   ")), QStringLiteral("hello"));
        QCOMPARE(StringUtil::trimEnd(QStringLiteral("hello\t\n")), QStringLiteral("hello"));
    }

    void trimEnd_chars()
    {
        QList<QChar> chars = { '.', '!' };
        QCOMPARE(StringUtil::trimEnd(QStringLiteral("hello...!!!"), chars), QStringLiteral("hello"));
    }

    void trimBothEnds_basic()
    {
        QList<QChar> chars = { '*' };
        QCOMPARE(StringUtil::trimBothEnds(QStringLiteral("***hello***"), chars), QStringLiteral("hello"));
    }

    void trimBothEnds_empty()
    {
        QList<QChar> chars = { ' ' };
        QCOMPARE(StringUtil::trimBothEnds(QString(), chars), QString());
    }

    // ---- unquoted / quoted ----

    void unquoted_basic()
    {
        QCOMPARE(StringUtil::unquoted(QStringLiteral("\"hello\"")), QStringLiteral("hello"));
    }

    void unquoted_noQuotes()
    {
        // Should return the original or reasonable result when no quotes present
        QString input = QStringLiteral("hello");
        QString result = StringUtil::unquoted(input);
        QVERIFY(!result.isEmpty());
    }

    void quoted_basic()
    {
        QCOMPARE(StringUtil::quoted(QStringLiteral("hello")), QStringLiteral("\"hello\""));
    }

    void quoted_empty()
    {
        QCOMPARE(StringUtil::quoted(QString()), QStringLiteral("\"\""));
    }

    // ---- trimmed (QStringList) ----

    void trimmed_basic()
    {
        QStringList input = { "  a  ", " b ", "  c" };
        QStringList result = StringUtil::trimmed(input);
        QCOMPARE(result, QStringList({"a", "b", "c"}));
    }

    // ---- splitWithQuotes ----

    void splitWithQuotes_basic()
    {
        QString input = QStringLiteral("one two three");
        QStringList result = StringUtil::splitWithQuotes(input, ' ');
        QCOMPARE(result.size(), 3);
        QCOMPARE(result.at(0), QStringLiteral("one"));
        QCOMPARE(result.at(2), QStringLiteral("three"));
    }

    void splitWithQuotes_preservesQuotedContent()
    {
        QString input = QStringLiteral("hello \"world of\" code");
        QStringList result = StringUtil::splitWithQuotes(input, ' ');
        // The quoted section should stay together
        bool found = false;
        for (const QString& s : result) {
            if (s.contains("world") && s.contains("of")) {
                found = true;
                break;
            }
        }
        QVERIFY2(found, "Quoted substring 'world of' was split apart");
    }

    void splitWithQuotes_skipEmpty()
    {
        QString input = QStringLiteral("a,,b,,c");
        QStringList result = StringUtil::splitWithQuotes(input, ',', Qt::SkipEmptyParts);
        QCOMPARE(result.size(), 3);
    }

    void splitWithQuotes_multipleSeparators()
    {
        QString input = QStringLiteral("a;b,c");
        QList<QChar> seps = { ';', ',' };
        QStringList result = StringUtil::splitWithQuotes(input, seps);
        QCOMPARE(result.size(), 3);
    }

    // ---- indexOfWord ----

    void indexOfWord_first()
    {
        // wordNumber is 0-based
        QCOMPARE(StringUtil::indexOfWord(QStringLiteral("hello world"), 0), 0);
    }

    void indexOfWord_second()
    {
        QCOMPARE(StringUtil::indexOfWord(QStringLiteral("hello world"), 1), 6);
    }

    void indexOfWord_outOfRange()
    {
        QCOMPARE(StringUtil::indexOfWord(QStringLiteral("hello"), 5), -1);
    }

    void indexOfWord_empty()
    {
        QCOMPARE(StringUtil::indexOfWord(QString(), 1), -1);
    }

    // ---- fuzzyIndexOf (Bitap) ----

    void fuzzyIndexOf_exactMatch()
    {
        int idx = StringUtil::fuzzyIndexOf(QStringLiteral("world"), QStringLiteral("hello world"), 0);
        QCOMPARE(idx, 6);
    }

    void fuzzyIndexOf_oneTypo()
    {
        // "wirld" is distance 1 from "world"
        int idx = StringUtil::fuzzyIndexOf(QStringLiteral("world"), QStringLiteral("hello wirld"), 1);
        QVERIFY(idx >= 0);
    }

    void fuzzyIndexOf_noMatch()
    {
        int idx = StringUtil::fuzzyIndexOf(QStringLiteral("xyz"), QStringLiteral("hello world"), 0);
        QCOMPARE(idx, -1);
    }

    // ---- levenshteinDistance ----

    void levenshteinDistance_identical()
    {
        QCOMPARE(StringUtil::levenshteinDistance(QStringLiteral("abc"), QStringLiteral("abc")), 0);
    }

    void levenshteinDistance_oneEdit()
    {
        QCOMPARE(StringUtil::levenshteinDistance(QStringLiteral("cat"), QStringLiteral("bat")), 1);
    }

    void levenshteinDistance_insertDelete()
    {
        // "kitten" -> "sitting" = 3
        QCOMPARE(StringUtil::levenshteinDistance(QStringLiteral("kitten"), QStringLiteral("sitting")), 3);
    }

    void levenshteinDistance_empty()
    {
        QCOMPARE(StringUtil::levenshteinDistance(QString(), QStringLiteral("abc")), 3);
        QCOMPARE(StringUtil::levenshteinDistance(QStringLiteral("abc"), QString()), 3);
        QCOMPARE(StringUtil::levenshteinDistance(QString(), QString()), 0);
    }

    // ---- Edge cases ----

    void edgeCase_singleCharacter()
    {
        QCOMPARE(StringUtil::trimFront(QStringLiteral("x"), { 'x' }), QString());
        QCOMPARE(StringUtil::trimEnd(QStringLiteral("x"), { 'x' }), QString());
        QCOMPARE(StringUtil::quoted(QStringLiteral("x")), QStringLiteral("\"x\""));
    }

    void edgeCase_combineToEol()
    {
        QStringList lines = { "first part", "second;", "after" };
        int consumed = 0;
        QString result = StringUtil::combineToEol(lines, 0, ';', &consumed);
        QVERIFY(consumed >= 2);
        QVERIFY(result.contains("first"));
        QVERIFY(result.contains("second"));
    }
};

QTEST_MAIN(TstString)
#include "tst_string.moc"

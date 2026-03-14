#include <QTest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUuid>

#include <Kanoop/floatutil.h>
#include <Kanoop/commonexception.h>
#include <Kanoop/kanoopcommon.h>
#include <Kanoop/entitymetadata.h>
#include <Kanoop/jsonhelper.h>

class TstMisc : public QObject
{
    Q_OBJECT

private slots:

    // ---- FloatUtil ----

    void floatUtil_exactDivisible()
    {
        QVERIFY(FloatUtil::isEvenlyDivisible(10.0, 5.0));
        QVERIFY(FloatUtil::isEvenlyDivisible(9.0, 3.0));
        QVERIFY(FloatUtil::isEvenlyDivisible(100.0, 25.0));
    }

    void floatUtil_notDivisible()
    {
        QVERIFY(!FloatUtil::isEvenlyDivisible(10.0, 3.0));
        QVERIFY(!FloatUtil::isEvenlyDivisible(7.0, 2.0));
    }

    void floatUtil_fractionalDivisible()
    {
        QVERIFY(FloatUtil::isEvenlyDivisible(1.0, 0.5));
        QVERIFY(FloatUtil::isEvenlyDivisible(1.5, 0.75));
    }

    void floatUtil_zeroDividend()
    {
        QVERIFY(FloatUtil::isEvenlyDivisible(0.0, 5.0));
    }

    void floatUtil_negativeDivisible()
    {
        QVERIFY(FloatUtil::isEvenlyDivisible(-10.0, 5.0));
        QVERIFY(FloatUtil::isEvenlyDivisible(10.0, -5.0));
        QVERIFY(FloatUtil::isEvenlyDivisible(-10.0, -5.0));
    }

    // ---- CommonException ----

    void exception_messageOnly()
    {
        CommonException ex("something went wrong");
        QCOMPARE(ex.message(), QString("something went wrong"));
        QCOMPARE(ex.code(), 0);
    }

    void exception_messageAndCode()
    {
        CommonException ex("disk full", 42);
        QCOMPARE(ex.message(), QString("disk full"));
        QCOMPARE(ex.code(), 42);
    }

    void exception_clone()
    {
        CommonException original("clone me", 7);
        CommonException *copy = original.clone();
        QVERIFY(copy != nullptr);
        QCOMPARE(copy->message(), QString("clone me"));
        QCOMPARE(copy->code(), 7);
        delete copy;
    }

    void exception_raiseAndCatch()
    {
        CommonException ex("thrown", 99);
        bool caught = false;
        try {
            ex.raise();
        } catch (const CommonException& e) {
            caught = true;
            QCOMPARE(e.message(), QString("thrown"));
            QCOMPARE(e.code(), 99);
        }
        QVERIFY(caught);
    }

    void exception_catchAsQException()
    {
        CommonException ex("base catch");
        bool caught = false;
        try {
            ex.raise();
        } catch (const QException&) {
            caught = true;
        }
        QVERIFY(caught);
    }

    // ---- KANOOP::EnumToStringMap ----

    void enumMap_getStringFound()
    {
        enum Color { Red = 1, Green, Blue };
        KANOOP::EnumToStringMap<Color> map;
        map.insert(Red, "Red");
        map.insert(Green, "Green");
        map.insert(Blue, "Blue");

        QCOMPARE(map.getString(Red), QString("Red"));
        QCOMPARE(map.getString(Blue), QString("Blue"));
    }

    void enumMap_getStringMissing()
    {
        enum Color { Red = 1, Green, Blue };
        KANOOP::EnumToStringMap<Color> map;
        map.insert(Red, "Red");

        QCOMPARE(map.getString(Green), QString());
        QCOMPARE(map.getString(Green, "unknown"), QString("unknown"));
    }

    void enumMap_getTypeCaseSensitive()
    {
        enum Fruit { Apple = 1, Banana, Cherry };
        KANOOP::EnumToStringMap<Fruit> map;
        map.insert(Apple, "Apple");
        map.insert(Banana, "Banana");
        map.insert(Cherry, "Cherry");

        QCOMPARE(map.getType("Banana"), Banana);
        QCOMPARE(map.getType("banana"), Fruit());  // case-sensitive miss
    }

    void enumMap_getTypeCaseInsensitive()
    {
        enum Fruit { Apple = 1, Banana, Cherry };
        KANOOP::EnumToStringMap<Fruit> map;
        map.insert(Apple, "Apple");
        map.insert(Banana, "Banana");

        QCOMPARE(map.getType("apple", Qt::CaseInsensitive), Apple);
        QCOMPARE(map.getType("BANANA", Qt::CaseInsensitive), Banana);
    }

    void enumMap_getTypeFromNumericString()
    {
        enum Level { Low = 1, Medium = 2, High = 3 };
        KANOOP::EnumToStringMap<Level> map;
        map.insert(Low, "Low");
        map.insert(Medium, "Medium");

        // Numeric string should parse directly to enum value
        QCOMPARE(map.getType("2"), Medium);
        QCOMPARE(map.getType("3"), High);
    }

    void enumMap_containsString()
    {
        enum Animal { Cat = 1, Dog };
        KANOOP::EnumToStringMap<Animal> map;
        map.insert(Cat, "Cat");
        map.insert(Dog, "Dog");

        QVERIFY(map.containsString("Cat"));
        QVERIFY(map.containsString("cat"));  // default case-insensitive
        QVERIFY(!map.containsString("Fish"));
    }

    // ---- EntityMetadata ----

    void entity_defaultInvalid()
    {
        EntityMetadata meta;
        QVERIFY(!meta.isValid());
        QCOMPARE(meta.type(), 0);
    }

    void entity_typeConstructor()
    {
        EntityMetadata meta(42);
        QVERIFY(meta.isValid());
        QCOMPARE(meta.type(), 42);
    }

    void entity_dataAccessors()
    {
        EntityMetadata meta(1, QString("payload"));
        QCOMPARE(meta.data().toString(), QString("payload"));

        meta.setData(QString("updated"));
        QCOMPARE(meta.data().toString(), QString("updated"));
    }

    void entity_uuidAccessor()
    {
        QUuid id = QUuid::createUuid();
        EntityMetadata meta(1, QString("test"), id);
        QCOMPARE(meta.uuid(), id);
    }

    void entity_equality()
    {
        // EntityMetadata::operator== compares type, iconId, and data map —
        // not UUID. Two entities with same type and same data are equal.
        QUuid id = QUuid::createUuid();
        EntityMetadata a(5, QString("hello"), id);
        EntityMetadata b(5, QString("hello"), id);
        EntityMetadata c(6, QString("hello"), id);

        QVERIFY(a == b);    // same type, same data
        QVERIFY(!(a == c)); // different type
    }

    void entity_variantRoundTrip()
    {
        QUuid id = QUuid::createUuid();
        EntityMetadata original(10, QString("round-trip"), id);
        QVariant v = original.toVariant();
        EntityMetadata restored = EntityMetadata::fromVariant(v);

        QCOMPARE(restored.type(), 10);
        QCOMPARE(restored.data().toString(), QString("round-trip"));
        QCOMPARE(restored.uuid(), id);
    }

    // ---- JsonHelper tryGet ----

    void json_tryGetStringSuccess()
    {
        QJsonObject obj;
        obj["name"] = "Alice";
        QString value;
        QVERIFY(JsonHelper::tryGetString(obj, "name", value));
        QCOMPARE(value, QString("Alice"));
    }

    void json_tryGetStringMissingKey()
    {
        QJsonObject obj;
        QString value;
        QVERIFY(!JsonHelper::tryGetString(obj, "absent", value));
    }

    void json_tryGetInt32Success()
    {
        QJsonObject obj;
        obj["count"] = 42;
        qint32 value = 0;
        QVERIFY(JsonHelper::tryGetInt32(obj, "count", value));
        QCOMPARE(value, 42);
    }

    void json_tryGetInt32MissingKey()
    {
        QJsonObject obj;
        qint32 value = -1;
        QVERIFY(!JsonHelper::tryGetInt32(obj, "absent", value));
    }

    void json_tryGetDoubleSuccess()
    {
        QJsonObject obj;
        obj["pi"] = 3.14159;
        qreal value = 0;
        QVERIFY(JsonHelper::tryGetDouble(obj, "pi", value));
        QCOMPARE(value, 3.14159);
    }

    void json_tryGetBoolSuccess()
    {
        QJsonObject obj;
        obj["enabled"] = true;
        obj["visible"] = false;
        bool v1 = false, v2 = true;
        QVERIFY(JsonHelper::tryGetBool(obj, "enabled", v1));
        QVERIFY(v1);
        QVERIFY(JsonHelper::tryGetBool(obj, "visible", v2));
        QVERIFY(!v2);
    }

    void json_tryGetBoolMissingKey()
    {
        QJsonObject obj;
        bool value = true;
        QVERIFY(!JsonHelper::tryGetBool(obj, "absent", value));
    }

    void json_tryGetObjectSuccess()
    {
        QJsonObject inner;
        inner["x"] = 1;
        QJsonObject outer;
        outer["nested"] = inner;

        QJsonObject result;
        QVERIFY(JsonHelper::tryGetObject(outer, "nested", result));
        QCOMPARE(result["x"].toInt(), 1);
    }

    void json_tryGetArraySuccess()
    {
        QJsonArray arr;
        arr.append(1);
        arr.append(2);
        QJsonObject obj;
        obj["items"] = arr;

        QJsonArray result;
        QVERIFY(JsonHelper::tryGetArray(obj, "items", result));
        QCOMPARE(result.size(), 2);
    }

    void json_stringListRoundTrip()
    {
        QStringList original = {"alpha", "beta", "gamma"};
        QJsonArray arr = JsonHelper::toJsonArray(original);
        QStringList restored = JsonHelper::toStringList(arr);
        QCOMPARE(restored, original);
    }

    void json_uuidOrNull()
    {
        QUuid id = QUuid::createUuid();
        QJsonValue val = JsonHelper::uuidOrNull(id);
        QVERIFY(!val.isNull());
        QCOMPARE(val.toString(), id.toString(QUuid::WithoutBraces));

        QJsonValue nullVal = JsonHelper::uuidOrNull(QUuid());
        QVERIFY(nullVal.isNull());
    }
};

QTEST_MAIN(TstMisc)
#include "tst_misc.moc"

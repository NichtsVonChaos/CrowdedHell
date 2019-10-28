#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QObject>
#include <QtSql/QtSql>
#include <QApplication>
#include <QVariantList>
#include <QStringList>
#include <QMap>

#include "Codes/GlobalComponent/globalComponent.h"

class SQLiteDatabase : public QObject
{
    Q_OBJECT
public:
    explicit SQLiteDatabase(QString databasePath, QObject *parent = nullptr);
    ~SQLiteDatabase();

    enum class DataType
    {
        Null, Integer, Real, Text
    };

    class Column
    {
    public:
        Column(const QString &label = QString(""), DataType dataType = DataType::Null, bool isAutoIncrementIndex = false, bool isPrimaryKey = false);
        Column(const Column &another);

        QString label;
        DataType dataType;
        bool isAutoIncrementIndex;
        bool isPrimaryKey;

        bool operator==(const Column &another) const;
        bool operator<(const Column &another) const;
        bool operator>(const Column &another) const;
    };

    typedef QMap<Column, QVariant> Record;

    class Table
    {
    public:
        QString tableName;
        QList<Column> columns;
        QList<Column> primaryKeys() const;

        bool contains(const QString &label) const;

        Column &operator[](const QString &label);
    };

    bool valid();

    bool create(const Table &table);
    bool insert(const Table &table, const QList<Record> &records);
    bool remove(const Table &table, const QMap<Column, QVariant> &keysValues);
    bool remove(const Table &table, const QString &condition);
    bool drop(const QString &tableName);
    bool drop(const Table &table);
    bool exists(const QString &tableName);
    bool exists(const Table &table);
    QList<Record> read(const Table &table, const QMap<Column, QVariant> &keysValues);
    QList<Record> read(const Table &table, const QList<Column> requiredColumns, const QMap<Column, QVariant> &keysValues);
    QList<Record> readAll(const Table &table);
    QList<Record> readAll(const Table &table, const QList<Column> requiredColumns);

signals:
    void message(Logger::Type type, const QString &module, const QString &message);

public slots:

private:
    const QString m_connectionName;
    bool m_isValid;
};

#endif // SQLITEDATABASE_H

#include "sqlitedatabase.h"

SQLiteDatabase::SQLiteDatabase(QString databasePath, QObject *parent) :
    QObject(parent), m_connectionName(QString("SQLITE_") + QString::number(qrand() % 10000)), m_isValid(false)
{
    connect(this, &SQLiteDatabase::message, logger(), &Logger::message);

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    database.setDatabaseName(databasePath);
    if(!database.open())
    {
        message(Logger::Type::Error, "SQLite", tr("Falied to open database file \"%1\".").arg(databasePath));
        return;
    }

    m_isValid = true;
}

SQLiteDatabase::~SQLiteDatabase()
{
    if(valid())
    {
        QSqlDatabase::database(m_connectionName).close();
        QSqlDatabase::removeDatabase(m_connectionName);
    }
}

bool SQLiteDatabase::valid()
{
    return m_isValid;
}

bool SQLiteDatabase::create(const SQLiteDatabase::Table &table)
{
    if(!valid())
    {
        message(Logger::Type::Error, "SQLite", tr("Database is invalid."));
        return false;
    }

    if(table.tableName.isEmpty())
    {
        message(Logger::Type::Error, "SQLite", tr("Name of data table must be provided.").arg(table.tableName));
        return false;
    }

    if(table.columns.size() == 0)
    {
        message(Logger::Type::Error, "SQLite", tr("Data table \"%1\" has no column.").arg(table.tableName));
        return false;
    }

    QSqlQuery query(QSqlDatabase::database(m_connectionName));

    QString sentence = "CREATE TABLE IF NOT EXISTS \"%1\" (%2)";
    sentence = sentence.arg(table.tableName);

    QString tableStructure;
    for(const auto &i : table.columns)
    {
        if(i.label.isEmpty())
        {
            message(Logger::Type::Warning, "SQLite", tr("Data table \"%1\" has an unnamed column. Ignore this column.").arg(table.tableName));
            continue;
        }
        tableStructure += i.label;
        switch(i.dataType)
        {
            case SQLiteDatabase::DataType::Null:
                tableStructure += " NULL";
            break;

            case SQLiteDatabase::DataType::Integer:
                tableStructure += " INTEGER";
            break;

            case SQLiteDatabase::DataType::Real:
                tableStructure += " REAL";
            break;

            case SQLiteDatabase::DataType::Text:
                tableStructure += " TEXT";
            break;
        }
        if(i.isPrimaryKey)
            tableStructure += " PRIMARY KEY";
        if(i.isAutoIncrementIndex)
            tableStructure += " AUTOINCREMENT";
        tableStructure += ",";
    }
    tableStructure.remove(tableStructure.size() - 1, 1);
    sentence = sentence.arg(tableStructure);

    if(!query.exec(sentence))
    {
        message(Logger::Type::Error, "SQLite", tr("Failed to execute \"%1\", error message : \"%2\".").arg(sentence).arg(query.lastError().text()));
        return false;
    }

    return true;
}

bool SQLiteDatabase::insert(const SQLiteDatabase::Table &table, const QList<SQLiteDatabase::Record> &records)
{
    if(!valid())
    {
        message(Logger::Type::Error, "SQLite", tr("Database is invalid."));
        return false;
    }

    if(table.tableName.isEmpty())
    {
        message(Logger::Type::Error, "SQLite", tr("Name of data table must be provided.").arg(table.tableName));
        return false;
    }

    if(table.columns.size() == 0)
    {
        message(Logger::Type::Error, "SQLite", tr("Data table \"%1\" has no column.").arg(table.tableName));
        return false;
    }

    QSqlQuery query(QSqlDatabase::database(m_connectionName));

    for(const auto &record : records)
    {
        QString sentence = "INSERT OR REPLACE INTO \"%1\" (%2) VALUES(%3)";
        sentence = sentence.arg(table.tableName);

        QString columns, values;

        for(auto itr = record.constBegin(); itr != record.constEnd(); ++itr)
        {
            if(itr.key().label.isEmpty())
            {
                message(Logger::Type::Warning, "SQLite", tr("A record has an unnamed column. Ignore this column."));
                continue;
            }
            if(table.contains(itr.key().label))
            {
                message(Logger::Type::Warning, "SQLite", tr("Data table \"%1\" has no column named \"%2\". Ignore this column.").arg(table.tableName).arg(itr.key().label));
                continue;
            }

            columns += itr.key().label;
            columns += ",";

            switch (itr.key().dataType)
            {
                case SQLiteDatabase::DataType::Null:
                    values += "NULL,";
                break;

                case SQLiteDatabase::DataType::Integer:
                    values += itr.value().toString();
                    values += ",";
                break;

                case SQLiteDatabase::DataType::Real:
                    values += itr.value().toString();
                    values += ",";
                break;

                case SQLiteDatabase::DataType::Text:
                    values += "\"";
                    values += itr.value().toString();
                    values += "\",";
                break;
            }
        }

        columns.remove(columns.size() - 1, 1);
        values.remove(values.size() - 1, 1);
        sentence = sentence.arg(columns).arg(values);

        if(!query.exec(sentence))
        {
            message(Logger::Type::Error, "SQLite", tr("Failed to execute \"%1\", error message : \"%2\".").arg(sentence).arg(query.lastError().text()));
            return false;
        }
    }

    return true;
}

bool SQLiteDatabase::remove(const SQLiteDatabase::Table &table, const QMap<SQLiteDatabase::Column, QVariant> &keysValues)
{

}

bool SQLiteDatabase::drop(const QString &tableName)
{

}

bool SQLiteDatabase::drop(const SQLiteDatabase::Table &table)
{

}

QList<SQLiteDatabase::Record> SQLiteDatabase::read(const SQLiteDatabase::Table &table, const QMap<QString, QVariant> &keysValues)
{

}

QList<SQLiteDatabase::Record> SQLiteDatabase::read(const SQLiteDatabase::Table &table, const QMap<SQLiteDatabase::Column, QVariant> &keysValues)
{

}

QList<SQLiteDatabase::Record> SQLiteDatabase::read(const SQLiteDatabase::Table &table, const QList<SQLiteDatabase::Column> requiredColumns, const QMap<QString, QVariant> &keysValues)
{

}

QList<SQLiteDatabase::Record> SQLiteDatabase::read(const SQLiteDatabase::Table &table, const QList<SQLiteDatabase::Column> requiredColumns, const QMap<SQLiteDatabase::Column, QVariant> &keysValues)
{

}

QList<SQLiteDatabase::Record> SQLiteDatabase::readAll(const SQLiteDatabase::Table &table)
{

}

QList<SQLiteDatabase::Record> SQLiteDatabase::readAll(const SQLiteDatabase::Table &table, const QList<SQLiteDatabase::Column> requiredColumns)
{

}

SQLiteDatabase::Column::Column(const QString &label, SQLiteDatabase::DataType dataType, bool isAutoIncrementIndex, bool isPrimaryKey):
    label(label), dataType(dataType), isAutoIncrementIndex(isAutoIncrementIndex), isPrimaryKey(isPrimaryKey)
{

}

SQLiteDatabase::Column::Column(const SQLiteDatabase::Column &another):
    label(another.label), dataType(another.dataType), isAutoIncrementIndex(another.isAutoIncrementIndex), isPrimaryKey(another.isPrimaryKey)
{

}

bool SQLiteDatabase::Column::operator==(const SQLiteDatabase::Column &another)
{
    return label == another.label && dataType == another.dataType && isAutoIncrementIndex == another.isAutoIncrementIndex && isPrimaryKey == another.isPrimaryKey;
}

QList<SQLiteDatabase::Column> SQLiteDatabase::Table::primaryKeys() const
{
    QList<SQLiteDatabase::Column> primaryKeys;
    for(const auto &i : columns)
        if(i.isPrimaryKey)
            primaryKeys << i;
    return primaryKeys;
}

bool SQLiteDatabase::Table::contains(const QString &label) const
{
    bool contained = false;
    for(const auto &i : columns)
        if(i.label == label)
        {
            contained = true;
            break;
        }
    return contained;
}

SQLiteDatabase::Column &SQLiteDatabase::Table::operator[](const QString &label)
{
    for(auto &i : columns)
        if(i.label == label)
            return i;
    columns.push_back(Column(label));
    return columns.back();
}

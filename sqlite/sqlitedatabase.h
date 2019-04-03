#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QtSql>
#include <QTime>
#include <QRandomGenerator>

class CrowdedHellGUI;
enum class MessageType;
enum class Language;

#ifndef CROWDEDHELLGUI_H
#include "crowdedhellgui.h"
#endif

class SQLiteDatabase : QObject
{
	Q_OBJECT

public:
	SQLiteDatabase(const QString &databasePath);
	~SQLiteDatabase();

	enum class DataType
	{
		INTEGER, TEXT, REAL
	};

	/**
	 * @brief The DataTable struct
	 * Structure of a data table.
	 * The column of auto increment index will named "_AutoIndex" and won't be included in the member "columns".
	 * If hasAutoIncrementIndex is true, primaryKey can be empty or not.
	 */
	struct DataTable
	{
		QString tableName;
		QList<QPair<QString, SQLiteDatabase::DataType>> columns;
		bool hasAutoIncrementIndex;
		QString primaryKey;
	};

	/**
	 * @brief isValid
	 * Return whether database is valid.
	 */
	bool isValid();

	/**
	 * @brief getAllTableNames
	 * Return a list of names of all tables in this database.
	 */
	QStringList getAllTableNames();

	/**
	 * @brief createTable
	 * Create a new Table.
	 * This function will call selectTable(table) if succesfully created this table.
	 * @param table
	 * Necessary information for create table.
	 * @return
	 * Return whether create table successfully.
	 * If this table already exists, it will return false too.
	 */
	bool createTable(const DataTable &table);

	/**
	 * @brief insert
	 * Insert a new record or replace existed record into a table.
	 * @param tableName
	 * Table name.
	 * @param record
	 * All values of this record.
	 * @return
	 * return whether successfully executed.
	 */
	bool insert(const QString &tableName, const QVariantList &record);

	/**
	 * @brief update
	 * Update a record.
	 * @param tableName
	 * Table name.
	 * @param columnName
	 * Column name where data will be updated.
	 * @param value
	 * New data.
	 * @return
	 * return whether successfully executed.
	 */
	bool update(const DataTable &table, const QString &primaryKeyValue, const QString &columnName, const QVariant &value);

	/**
	 * @brief exists
	 * Return whether a table exists.
	 * @param tableName
	 * Table name.
	 */
	bool exists(const QString &tableName);

	/**
	 * @brief close
	 * Close the database.
	 * All operation will be unusable after close.
	 */
	void close();

signals:
	void sendMessage(MessageType type, QString module, QString message);

private:
	QString m_connectionName;

	bool m_valid;
};

#endif // SQLITEDATABASE_H

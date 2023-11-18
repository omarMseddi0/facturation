#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QSqlDatabase>
#include <QStringList>
#include <QMap>
#include "company.h"
#include"user.h"


class DatabaseHandler
{
public:
    DatabaseHandler();
    ~DatabaseHandler();

    bool openConnection();
    void closeConnection();

    QList<Company> getCompanies();
    Company getCompanyById(const int id);
    bool createCompany(const Company company);
    bool deleteCompanyById(const int id);
    bool updateCompany(const Company company);
    bool createUser(const QString &username, const QString &password);
    bool checkCredentials(const QString &username, const QString &password);
    QList<User> getUsers();
    User getUserById(const int id);
    bool createeUser(const User user);
    bool deleteUserById(const int id);
    bool updateUser(const User user);
    bool isAdmin(const QString &username);


QSqlDatabase m_database = QSqlDatabase();
private:

};

#endif // DATABASEHANDLER_H

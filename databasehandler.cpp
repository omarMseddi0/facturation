#include "databasehandler.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QMessageBox>


DatabaseHandler::DatabaseHandler()
{
}


DatabaseHandler::~DatabaseHandler()
{
    closeConnection();
}

bool DatabaseHandler::openConnection()
{
    const QString connectionName = "sfmi_connection";



    if (QSqlDatabase::contains()) {
        // Close and remove default connection if it exists
        QSqlDatabase::database().close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

    if (QSqlDatabase::contains(connectionName)) {
        m_database = QSqlDatabase::database(connectionName);
        if (m_database.isOpen()) {
            return true;    // Connection is already open
        }
        // If the connection is not open, fall through to reopen it
    } else {
        m_database = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    }

    QSettings settings("sfmi.ini", QSettings::IniFormat);  // Adjust the filename and format as needed
    QString hostName = settings.value("Database/HostName").toString();
    QString databaseName = settings.value("Database/DatabaseName").toString();
    QString userName = settings.value("Database/UserName").toString();
    QString password = settings.value("Database/Password").toString();

    m_database.setHostName(hostName);
    m_database.setDatabaseName(databaseName);
    m_database.setUserName(userName);
    m_database.setPassword(password);


    if (!m_database.open()) {
        qDebug() << "Failed to open database:" << m_database.lastError().text();
        return false;
    }
    qDebug() << "Database connection opened successfully";

    return true;
}

void DatabaseHandler::closeConnection()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
    QString connectionName = m_database.connectionName();
    m_database = QSqlDatabase(); // reset the database
    QSqlDatabase::removeDatabase(connectionName); // this will remove the connection
}



QList<Company> DatabaseHandler::getCompanies()
{
    QList<Company> companies;

    if (!m_database.isOpen()) {
        qDebug() << "Database connection not open.";
        return companies;
    }
    QSettings settings("sfmi.ini", QSettings::IniFormat);
    QString tableName = settings.value("Database/TableName").toString();

    QSqlQuery query(m_database);
    query.prepare("SELECT name, id FROM " + tableName);
    if (query.exec()) {
        while (query.next()) {
            QString companyName = query.value(0).toString();
            QString companyId = query.value(1).toString();
            Company company;
            company.setId(companyId);
            company.setName(companyName);
            companies.append(company);
        }
    } else {
        qDebug() << "Query failed:" << query.lastError().text();
    }

    return companies;
}

Company DatabaseHandler::getCompanyById(const int id)
{
    Company company;
    if (!m_database.isOpen()) {
        qDebug() << "Database connection not open.";
        return company;
    }

    QSettings settings("sfmi.ini", QSettings::IniFormat);
    QString tableName = settings.value("Database/TableName").toString();

    QSqlQuery query(m_database);
    query.prepare("SELECT * FROM " + tableName + " WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        QString companyName = query.value("name").toString();
        QString companyId = query.value("id").toString();
        QString matFisc = query.value("matricule_Fiscale").toString();
        QString adresse = query.value("address").toString();
        QString email = query.value("email").toString();
        QString tel = query.value("tel").toString();
        QString tel2 = query.value("tel2").toString();
        QString fax = query.value("fax").toString();
        QString fax2 = query.value("fax2").toString();
        QString contact = query.value("contact").toString();


        company.setName(companyName);
        company.setId(companyId);
        company.setMat(matFisc);
        company.setAdr(adresse);
        company.setEmail(email);
        company.setTel(tel);
        company.setTel2(tel2);
        company.setFax(fax);
        company.setFax2(fax2);
        company.setContact(contact);


        qDebug() << "Company retrieved successfully.";
    } else {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }

    return company;

}

bool DatabaseHandler::createCompany(const Company company) {
    if (!m_database.isOpen()) {
        qDebug() << "Database connection not open.";
        return false;
    }

    QSettings settings("sfmi.ini", QSettings::IniFormat);
    QString tableName = settings.value("Database/TableName").toString();

    // Calculate the next id value
    QSqlQuery maxIdQuery(m_database);
    maxIdQuery.prepare("SELECT MAX(id) FROM " + tableName);
    int nextId = 1;
    if (maxIdQuery.exec() && maxIdQuery.next()) {
        nextId = maxIdQuery.value(0).toInt() + 1;
    }

    QSqlQuery query(m_database);

    // Include the id value in the INSERT statement
    query.prepare("INSERT INTO "+ tableName +" (id, name, matricule_Fiscale, address, email, tel, tel2, fax, fax2, contact) "
                                               "VALUES (:id, :companyName, :matFisc, :adresse, :email, :tel, :tel2, :fax, :fax2, :contact)");
    query.bindValue(":id", nextId);
    query.bindValue(":companyName", company.getName());
    query.bindValue(":matFisc", company.getMat());
    query.bindValue(":adresse", company.getAdr());
    query.bindValue(":email", company.getEmail());
    query.bindValue(":tel", company.getTel());
    query.bindValue(":tel2", company.getTel2());
    query.bindValue(":fax", company.getFax());
    query.bindValue(":fax2", company.getFax2());
    query.bindValue(":contact", company.getContact());

    if (query.exec()) {
        qDebug() << "Insert query executed successfully.";
        return true;
    } else {
        qDebug() << "Failed to execute insert query:" << query.lastError().text();
        return false;
    }
}


bool DatabaseHandler::updateCompany(const Company company) {
    if (!m_database.isOpen()) {
        qDebug() << "Database connection not open.";
        return false;
    }

    QSettings settings("sfmi.ini", QSettings::IniFormat);
    QString tableName = settings.value("Database/TableName").toString();

    QSqlQuery query(m_database);

    query.prepare("UPDATE "+ tableName +" SET name=:companyName, matricule_Fiscale=:matFisc, address=:address, email=:email,"
                                               "tel=:tel, tel2=:tel2, fax=:fax, fax2=:fax2, contact=:contact WHERE id=:id");
    query.bindValue(":companyName", company.getName());
    query.bindValue(":matFisc", company.getMat());
    query.bindValue(":address", company.getAdr());
    query.bindValue(":email", company.getEmail());
    query.bindValue(":tel", company.getTel());
    query.bindValue(":tel2", company.getTel2());
    query.bindValue(":fax", company.getFax());
    query.bindValue(":fax2", company.getFax2());
    query.bindValue(":contact", company.getContact());
    query.bindValue(":id", company.getId());

    if (query.exec()) {
        qDebug() << "Update query executed successfully.";
        return true;
    } else {
        qDebug() << "Failed to execute update query:" << query.lastError().text();
        return false;
    }

}


bool DatabaseHandler::deleteCompanyById(const int id) {
    if (!m_database.isOpen()) {
        qDebug() << "Database connection not open.";
        return false;
    }
    QSettings settings("sfmi.ini", QSettings::IniFormat);
    QString tableName = settings.value("Database/TableName").toString();

    QSqlQuery query(m_database);
    query.prepare("DELETE FROM " + tableName + " WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "Delete query executed successfully.";
        return true;
    } else {
        qDebug() << "Failed to execute delete query:" << query.lastError().text();
        return false;
    }
}
bool DatabaseHandler::createUser(const QString &username , const QString &password)
{
    if (!openConnection()) {
        qDebug() << "Failed to open database connection.";
        return false;
    }
    QSqlQuery maxIdQuery(m_database);
    maxIdQuery.prepare("SELECT MAX(id) FROM users" );
    int nextId = 1;
    if (maxIdQuery.exec() && maxIdQuery.next()) {
        nextId = maxIdQuery.value(0).toInt() + 1;
    }
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO users (id ,username, password) VALUES (:id,:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":id", nextId);

    if (query.exec()) {
        qDebug() << "User created successfully.";
        return true;
    } else {
        qDebug() << "Failed to create user:" << query.lastError().text();
        return false;
    }
}

bool DatabaseHandler::checkCredentials(const QString &username, const QString &password)
{
    if (!openConnection()) {
        qDebug() << "Failed to open database connection.";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("SELECT COUNT(*) FROM (SELECT username, password FROM users UNION SELECT username, password FROM admins) AS combined WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        if (count == 1) {
            qDebug() << "Credentials are valid.";
            return true;
        } else {
            qDebug() << "Invalid credentials.";
            return false;
        }
    } else {
        qDebug() << "Failed to check credentials:" << query.lastError().text();
        return false;
    }
}

bool DatabaseHandler::isAdmin(const QString &username)
{
    if (!openConnection()) {
        qDebug() << "Failed to open database connection.";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("SELECT COUNT(*) FROM admins WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        if (count == 1) {
            qDebug() << "User is an admin.";
            return true;
        } else {
            qDebug() << "User is not an admin.";
            return false;
        }
    } else {
        qDebug() << "Failed to check if user is an admin:" << query.lastError().text();
        return false;
    }
}
QList<User> DatabaseHandler::getUsers()
{
    QList<User> users;

    if (!m_database.isOpen()) {
        qDebug() << "Database connection not open.";
        return users;
    }
    QSettings settings("sfmi.ini", QSettings::IniFormat);
    QString tableName = settings.value("Database/UsersTableName").toString();

    QSqlQuery query(m_database);
    query.prepare("SELECT username, password, id FROM users");
    if (query.exec()) {
        while (query.next()) {
            QString username = query.value(0).toString();
            QString password = query.value(1).toString();
            QString userId = query.value(2).toString();
            User user;
            user.setId(userId);
            user.setUsername(username);
            user.setPassword(password);
            users.append(user);
        }
    } else {
        qDebug() << "Query failed:" << query.lastError().text();
    }

    return users;
}

User DatabaseHandler::getUserById(const int id)
{
    User user;
    if (!m_database.isOpen()) {
        qDebug() << "Database connection not open.";
        return user;
    }

    QSettings settings("sfmi.ini", QSettings::IniFormat);
    QString tableName = settings.value("Database/UsersTableName").toString();

    QSqlQuery query(m_database);
    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        QString username = query.value("username").toString();
        QString password = query.value("password").toString();
        QString userId = query.value("id").toString();

        user.setUsername(username);
        user.setPassword(password);
        user.setId(userId);

        qDebug() << "User retrieved successfully.";
    } else {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }

    return user;
}

bool DatabaseHandler::createeUser(const User user) {
    if (!m_database.isOpen()) {
        qDebug() << "Database connection not open.";
        return false;
    }

    QSettings settings("sfmi.ini", QSettings::IniFormat);
    QString tableName = settings.value("Database/TableName").toString();

    // Calculate the next id value
    QSqlQuery maxIdQuery(m_database);
    maxIdQuery.prepare("SELECT MAX(id) FROM users");
    int nextId = 1;
    if (maxIdQuery.exec() && maxIdQuery.next()) {
        nextId = maxIdQuery.value(0).toInt() + 1;
    }

    QSqlQuery query(m_database);

    // Include the id value in the INSERT statement
    query.prepare("INSERT INTO users (id ,username , password) "
                                               "VALUES (:id, :username, :password)");
    query.bindValue(":id", nextId);
    query.bindValue(":username", user.getUsername());
    query.bindValue(":password", user.getPassword());


    if (query.exec()) {
        qDebug() << "Insert query executed successfully.";
        return true;
    } else {
        qDebug() << "Failed to execute insert query:" << query.lastError().text();
        return false;
    }
}
    // Same as createCompany but with appropriate changes for the users table


bool DatabaseHandler::updateUser(const User user){
    if (!m_database.isOpen()) {
    qDebug() << "Database connection not open.";
    return false;
}

QSettings settings("sfmi.ini", QSettings::IniFormat);
QString tableName = settings.value("Database/TableName").toString();

QSqlQuery query(m_database);

query.prepare("UPDATE users SET username=:username , password=:password WHERE id=:id");
query.bindValue(":id", user.getId());
query.bindValue(":username", user.getUsername());
query.bindValue(":password", user.getPassword());

if (query.exec()) {
    qDebug() << "Update query executed successfully.";
    return true;
} else {
    qDebug() << "Failed to execute update query:" << query.lastError().text();
    return false;
}

}


bool DatabaseHandler::deleteUserById(const int id)  {
if (!m_database.isOpen()) {
qDebug() << "Database connection not open.";
return false;
}
QSettings settings("sfmi.ini", QSettings::IniFormat);
QString tableName = settings.value("Database/TableName").toString();

QSqlQuery query(m_database);
query.prepare("DELETE FROM users WHERE id = :id");
query.bindValue(":id", id);

if (query.exec()) {
qDebug() << "Delete query executed successfully.";
return true;
} else {
qDebug() << "Failed to execute delete query:" << query.lastError().text();
return false;
}

}



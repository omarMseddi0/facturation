#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();
    ~User();

    QString getId() const;
    void setId(const QString &value);

    QString getUsername() const;
    void setUsername(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

private:
    QString id;
    QString username;
    QString password;
};

#endif // USER_H

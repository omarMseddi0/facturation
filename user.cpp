#include "user.h"

User::User()
{
}

User::~User()
{
}

QString User::getId() const
{
    return id;
}

void User::setId(const QString &value)
{
    id = value;
}

QString User::getUsername() const
{
    return username;
}

void User::setUsername(const QString &value)
{
    username = value;
}

QString User::getPassword() const
{
    return password;
}

void User::setPassword(const QString &value)
{
    password = value;
}

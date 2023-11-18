#include "company.h"

Company::Company()
{

}

QString Company::getId() const
{
    return m_id;
}

void Company::setId(const QString& id)
{
    m_id = id;
}

QString Company::getName() const
{
    return m_name;
}

void Company::setName(const QString& name)
{
    m_name = name;
}

QString Company::getMat() const
{
    return m_mat;
}

void Company::setMat(const QString& mat)
{
    m_mat = mat;
}

QString Company::getTel() const
{
    return m_tel;
}

void Company::setTel(const QString& tel)
{
    m_tel = tel;
}


QString Company::getTel2() const
{
    return m_tel2;
}

void Company::setTel2(const QString& tel2)
{
    m_tel2 = tel2;
}


QString Company::getFax() const
{
    return m_fax;
}

void Company::setFax(const QString& fax)
{
    m_fax = fax;
}


QString Company::getFax2() const
{
    return m_fax2;
}

void Company::setFax2(const QString& fax2)
{
    m_fax2 = fax2;
}


QString Company::getAdr() const
{
    return m_adr;
}

void Company::setAdr(const QString& adr)
{
    m_adr = adr;
}

QString Company::getEmail() const
{
    return m_email;
}

void Company::setEmail(const QString& email)
{
    m_email = email;
}

QString Company::getContact() const
{
    return m_contact;
}

void Company::setContact(const QString& contact)
{
    m_contact = contact;
}

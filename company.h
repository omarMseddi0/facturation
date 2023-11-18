#ifndef COMPANY_H
#define COMPANY_H

#include <QStringList>

class Company
{
public:
    Company();

    QString getId() const;
    void setId(const QString& value);

    QString getName() const;
    void setName(const QString& value);

    QString getMat() const;
    void setMat(const QString& value);

    QString getTel() const;
    void setTel(const QString& value);

    QString getTel2() const;
    void setTel2(const QString& value);

    QString getFax() const;
    void setFax(const QString& value);

    QString getFax2() const;
    void setFax2(const QString& value);

    QString getAdr() const;
    void setAdr(const QString& value);

    QString getEmail() const;
    void setEmail(const QString& value);

    QString getContact() const;
    void setContact(const QString& value);

private:
    QString m_name;
    QString m_id;
    QString m_mat;
    QString m_tel;
    QString m_tel2;
    QString m_fax;
    QString m_fax2;
    QString m_adr;
    QString m_email;
    QString m_contact;
};

#endif // COMPANY_H

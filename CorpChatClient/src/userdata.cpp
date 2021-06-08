#include "../include/userdata.h"

UserData::UserData(QObject *parent) : QObject(parent)
{

}

QString UserData::username() const
{
    return m_username;
}

void UserData::setUsername(const QString &username)
{
    m_username = username;
    emit usernameChanged(username);
}

QString UserData::imageUrl() const
{
    return m_imageUrl;
}

void UserData::setImageUrl(const QString &imageUrl)
{
    m_imageUrl = imageUrl;
    emit imageUrlChanged(imageUrl);
}

QString UserData::email() const
{
    return m_email;
}

void UserData::setEmail(const QString &email)
{
    m_email = email;
}

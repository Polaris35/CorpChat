#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>

class UserData : public QObject
{
    Q_OBJECT
public:
    explicit UserData(QObject *parent = nullptr);

    QString username() const;
    void setUsername(const QString &username);

    QString imageUrl() const;
    void setImageUrl(const QString &imageUrl);

    QString email() const;
    void setEmail(const QString &email);


signals:
    void usernameChanged(QString newUsername);
    void emailChanged(QString newEmail);
    void imageUrlChanged(QString newUrl);

private:
    QString m_username;
    QString m_email;
    QString m_imageUrl;
    QString role;

};

#endif // USERDATA_H

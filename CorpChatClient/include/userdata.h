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

signals:
    void usernameChanged(QString newUsername);
    void imageUrlChanged(QString newUrl);

private:
    QString m_username;
    QString m_imageUrl;

};

#endif // USERDATA_H

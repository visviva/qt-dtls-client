#ifndef DTLSCONFIGURATIONFACTORY_H
#define DTLSCONFIGURATIONFACTORY_H

#include <QObject>
#include <QSslConfiguration>
#include <QString>

class DTLSConfigurationFactory : public QObject
{
    Q_OBJECT
  private:
    QString mKey;
    QString mCertificate;
    QString mCA;
    QSslSocket::PeerVerifyMode mVerifyPeer{QSslSocket::VerifyNone};
    QByteArray readFile(QString const &File);

  public:
    explicit DTLSConfigurationFactory(QObject *parent = nullptr);

  Q_SIGNALS:
    void newConfiguration(QSslConfiguration Config);

  public Q_SLOTS:
    void setKeyFilename(QString const &Filename);
    void setCertificateFilename(QString const &Filename);
    void setCAFilename(QString const &Filename);
    void setPeerVerify(bool VerifyPeer);
    QSslConfiguration createConfiguration();
};

#endif // DTLSCONFIGURATIONFACTORY_H

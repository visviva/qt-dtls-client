#ifndef DTLSCLIENT_H
#define DTLSCLIENT_H

#include <QDtls>
#include <QFile>
#include <QObject>
#include <QSslConfiguration>
#include <QTimer>
#include <QUdpSocket>

class DTLSClient : public QObject
{
    Q_OBJECT

  private:
    QDtls mClientDtls{QSslSocket::SslClientMode, this};
    QUdpSocket mUdpSocket{this};
    QFile mCertificate{this};
    QFile mKey{this};

  public:
    explicit DTLSClient(QObject *parent = nullptr);
    QByteArray decrypt(QByteArray EncryptedData);
    void handleError();

  Q_SIGNALS:
    void received(QByteArray Data);
    void status(QString Status);

  public Q_SLOTS:
    void startHandshake(QHostAddress PeerAddress, int PeerPort,
                        QSslConfiguration Config);
    void receive();
    void send(QByteArray Data);
    void closeConnection();
};

#endif // DTLSCLIENT_H

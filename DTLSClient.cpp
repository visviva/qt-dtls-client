#include "DTLSClient.h"
#include <QNetworkDatagram>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslKey>
#include <QtDebug>

DTLSClient::DTLSClient (QObject *parent) : QObject (parent)
{
    connect (&mUdpSocket, &QUdpSocket::readyRead, this, &DTLSClient::receive);

    mCertificate.setFileName (QStringLiteral (
        "/home/simon/Workspace/QtDTLS/DTLSClient/cert_client/cert.pem"));

    mKey.setFileName (QStringLiteral (
        "/home/simon/Workspace/QtDTLS/DTLSClient/cert_client/key.pem"));
}

QByteArray DTLSClient::decrypt (QByteArray EncryptedData)
{
    auto DecryptedData =
        mClientDtls.decryptDatagram (&mUdpSocket, EncryptedData);
    Q_EMIT received (DecryptedData);

    qDebug () << DecryptedData << "\n";
    return DecryptedData;
}

void DTLSClient::handleError ()
{
    auto Error = mClientDtls.dtlsError ();
    if (Error == QDtlsError::NoError)
    {
        return;
    }

    auto ErrorString = mClientDtls.dtlsErrorString ();
    Q_EMIT status (ErrorString);
}

void DTLSClient::startHandshake (QHostAddress PeerAddress, int PeerPort)
{
    auto Configuration = QSslConfiguration::defaultDtlsConfiguration ();

    mKey.open (QIODevice::ReadOnly | QIODevice::Text);
    mCertificate.open (QIODevice::ReadOnly | QIODevice::Text);

    auto Key = mKey.readAll ();
    auto Cert = mCertificate.readAll ();

    QSslKey SslKey (Key, QSsl::Rsa);
    QSslCertificate SslCert (Cert);

    Configuration.setPrivateKey (SslKey);
    Configuration.setLocalCertificate (SslCert);
    Configuration.setPeerVerifyMode (QSslSocket::VerifyNone);
    mClientDtls.setDtlsConfiguration (Configuration);
    mUdpSocket.connectToHost (PeerAddress, PeerPort);
    mClientDtls.setPeer (PeerAddress, PeerPort);
    mClientDtls.doHandshake (&mUdpSocket);
}

void DTLSClient::receive ()
{
    auto Data = mUdpSocket.receiveDatagram ();
    auto HandshakeState = mClientDtls.handshakeState ();

    switch (HandshakeState)
    {
    case QDtls::HandshakeNotStarted: /* do nothing*/
        Q_EMIT status (QStringLiteral ("Handshake not started"));
        break;
    case QDtls::HandshakeInProgress:
        mClientDtls.doHandshake (&mUdpSocket, Data.data ());
        Q_EMIT status (QStringLiteral ("Handshake in progress"));
        break;
    case QDtls::HandshakeComplete:
        Q_EMIT status (QStringLiteral ("Handshake done"));
        decrypt (Data.data ());
        break;
    case QDtls::PeerVerificationFailed:
        Q_EMIT status (QStringLiteral ("Peer verification failed"));
        break;
    }
    handleError ();
}

void DTLSClient::send (QByteArray Data)
{
    mClientDtls.writeDatagramEncrypted (&mUdpSocket, Data);
}

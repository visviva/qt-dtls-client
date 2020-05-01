#include "DTLSConfigurationFactory.h"
#include <QFile>
#include <QSslCertificate>
#include <QSslKey>

QByteArray DTLSConfigurationFactory::readFile(const QString &File)
{
    QFile OpenedFile(File);
    OpenedFile.open(QIODevice::ReadOnly | QIODevice::Text);
    return OpenedFile.readAll();
}

DTLSConfigurationFactory::DTLSConfigurationFactory(QObject *parent)
    : QObject(parent)
{
}

void DTLSConfigurationFactory::setKeyFilename(const QString &Filename)
{
    mKey = Filename;
}

void DTLSConfigurationFactory::setCertificateFilename(const QString &Filename)
{
    mCertificate = Filename;
}

void DTLSConfigurationFactory::setCAFilename(const QString &Filename)
{
    mCA = Filename;
}

void DTLSConfigurationFactory::setPeerVerify(bool VerifyPeer)
{
    if (VerifyPeer)
    {
        mVerifyPeer = QSslSocket::VerifyPeer;
    }
    else
    {
        mVerifyPeer = QSslSocket::VerifyNone;
    }
}

QSslConfiguration DTLSConfigurationFactory::createConfiguration()
{
    auto Configuration = QSslConfiguration::defaultDtlsConfiguration();

    if (mKey.size() != 0)
    {
        QSslKey Key(readFile(mKey), QSsl::Rsa);
        Configuration.setPrivateKey(Key);
    }

    if (mCertificate.size() != 0)
    {
        QSslCertificate Cert(readFile(mCertificate));
        Configuration.setLocalCertificate(Cert);
    }

    if (mCA.size() != 0)
    {
        QList<QSslCertificate> CAList;
        QSslCertificate CA(readFile(mCA));
        CAList.append(CA);
        Configuration.setCaCertificates(CAList);
    }

    Configuration.setPeerVerifyMode(mVerifyPeer);

    Q_EMIT newConfiguration(Configuration);

    return Configuration;
}

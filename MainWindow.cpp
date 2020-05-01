#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QDateTime>
#include <QFileDialog>
#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      mClient(new DTLSClient(this)),
      mConfigFactory(new DTLSConfigurationFactory(this))
{
    ui->setupUi(this);

    connect(this, &MainWindow::signalStartHandshake, mClient,
            &DTLSClient::startHandshake);

    connect(ui->CloseConnectionBtn, &QPushButton::clicked, mClient,
            &DTLSClient::closeConnection);

    connect(ui->StartHandshakeBtn, &QPushButton::clicked,
            Q_EMIT[this](bool /*checked*/) {
                auto Ip = QHostAddress(this->ui->IpEdit->text());
                auto Port = this->ui->PortEdit->text().toInt();

                auto KeyFile = ui->KeyLineEdit->text();
                if (KeyFile.size() != 0)
                {
                    mConfigFactory->setKeyFilename(KeyFile);
                }

                auto CertFile = ui->CertLineEdit->text();
                if (CertFile.size() != 0)
                {
                    mConfigFactory->setCertificateFilename(CertFile);
                }

                auto CAFile = ui->CALineEdit->text();
                if (CAFile.size() != 0)
                {
                    mConfigFactory->setCAFilename(CAFile);
                }

                mConfigFactory->setPeerVerify(
                    this->ui->VerifyPeerCheckbox->checkState() == Qt::Checked);

                auto Config = mConfigFactory->createConfiguration();

                Q_EMIT signalStartHandshake(Ip, Port, Config);
            });

    connect(mClient, &DTLSClient::status, ui->ConnecntionStateLabel,
            &QLabel::setText);

    connect(mClient, &DTLSClient::received, [this](QByteArray Data) {
        this->appendToLog(std::move(Data), true);
    });

    connect(ui->SendDataBtn, &QPushButton::clicked, [this](bool /*checked*/) {
        auto Data = ui->SendDataEdit->text();
        ui->SendDataEdit->clear();
        mClient->send(Data.toLocal8Bit());
        this->appendToLog(Data.toLocal8Bit(), false);
    });

    connect(ui->SetKeyBtn, &QPushButton::clicked, [this](bool /*checked*/) {
        auto File = QFileDialog::getOpenFileName(
            this, QStringLiteral("Open Key"), QStringLiteral("."),
            QStringLiteral("Key (*.pem)"));
        this->ui->KeyLineEdit->setText(File);
    });

    connect(ui->SetCertBtn, &QPushButton::clicked, [this](bool /*checked*/) {
        auto File = QFileDialog::getOpenFileName(
            this, QStringLiteral("Open Certificate"), QStringLiteral("."),
            QStringLiteral("Key (*.pem)"));
        this->ui->CertLineEdit->setText(File);
    });

    connect(ui->SetCABtn, &QPushButton::clicked, [this](bool /*checked*/) {
        auto File = QFileDialog::getOpenFileName(
            this, QStringLiteral("Open Certificate Authority"),
            QStringLiteral("."), QStringLiteral("Key (*.pem)"));
        this->ui->CALineEdit->setText(File);
    });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::appendToLog(const QByteArray &Data, bool ReceivedNotSend)
{
    auto CurrentTime = QDateTime::currentDateTime().toString();
    QString Text;
    Text = CurrentTime;
    Text += QLatin1String(" <> ");

    if (ReceivedNotSend)
    {
        Text += QLatin1String("Received: ");
    }
    else
    {
        Text += QLatin1String("Send: ");
    }
    Text += QString::fromUtf8(Data);

    ui->Log->appendPlainText(Text);
}

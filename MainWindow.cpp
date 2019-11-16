#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QDateTime>

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow (parent), ui (new Ui::MainWindow),
      mClient (new DTLSClient (this))
{
    ui->setupUi (this);

    connect (this, &MainWindow::signalStartHandshake, mClient,
             &DTLSClient::startHandshake);

    connect (ui->StartHandshakeBtn, &QPushButton::clicked,
             [this](bool checked) {
                 auto Ip = QHostAddress (this->ui->IpEdit->text ());
                 auto Port = this->ui->PortEdit->text ().toInt ();

                 emit signalStartHandshake (Ip, Port);
             });

    connect (mClient, &DTLSClient::status, ui->ConnecntionStateLabel,
             &QLabel::setText);

    connect (mClient, &DTLSClient::received,
             [this](QByteArray Data) { this->appendToLog (Data, true); });

    connect (ui->SendDataBtn, &QPushButton::clicked, [this](bool checked) {
        auto Data = ui->SendDataEdit->text ();
        ui->SendDataEdit->clear ();
        mClient->send (Data.toLocal8Bit ());
        this->appendToLog (Data.toLocal8Bit (), false);
    });
}

MainWindow::~MainWindow () { delete ui; }

void MainWindow::appendToLog (QByteArray Data, bool ReceivedNotSend)
{
    auto CurrentTime = QDateTime::currentDateTime ().toString ();
    QString Text;
    Text = CurrentTime;
    Text += QLatin1String(" <> ");

    if (ReceivedNotSend)
    {
        Text += QLatin1String ("Received: ");
    }
    else
    {
        Text += QLatin1String ("Send: ");
    }
    Text += QString::fromUtf8 (Data);

    ui->Log->appendPlainText (Text);
}

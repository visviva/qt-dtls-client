#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DTLSClient.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow (QWidget *parent = nullptr);
    ~MainWindow () override;

  Q_SIGNALS:
    void signalStartHandshake (const QHostAddress &, const int &);

  public Q_SLOTS:
    void appendToLog (QByteArray Data, bool ReceivedNotSend);

  private:
    Ui::MainWindow *ui;
    DTLSClient *mClient;
};
#endif // MAINWINDOW_H

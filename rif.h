#ifndef RIF_H
#define RIF_H

#include <QWidget>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

namespace Ui {
class rif;
}

class rif : public QWidget
{
    Q_OBJECT

public slots:
    void replyFinished(QNetworkReply *reply);
    //void finished();

public:
    explicit rif(QWidget *parent = 0);
    ~rif();
    void validarRIF();
    void borrarCampos();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_salirButton_clicked();
    void on_consultarButton_clicked();
    void on_rifEdit_editingFinished();

private:
    Ui::rif *ui;
    QString _rif();
};

#endif // RIF_H

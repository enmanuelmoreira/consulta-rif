#include "rif.h"
#include "ui_rif.h"

#include <stdlib.h>
#include <iostream>

#include <QDebug>
#include <QtXml>
#include <QRegExp>
#include <QRegExpValidator>
#include <QMessageBox>

rif::rif(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rif)
{
    ui->setupUi(this);
}

rif::~rif()
{
    delete ui;
}

void rif::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString rif::_rif()
{
    /*QRegExpValidator *exp = new QRegExpValidator(QRegExp("\\^([VEJPG]{1})([0-9]{9}$)\\"), this);
    ui->rifEdit->setValidator(exp);*/
    return ui->rifEdit->text().replace("-","");
}

void rif::on_salirButton_clicked()
{
    this->close();
}

void rif::borrarCampos()
{
    ui->nombreEdit->clear();
    ui->sugeridoEdit->clear();
    ui->contribuyenteEdit->clear();
    ui->retencionEdit->clear();
    ui->porcentajeSpin->clear();
}

void rif::on_rifEdit_editingFinished()
{
    //colocarmos las letras en mayusculas
    ui->rifEdit->setText(ui->rifEdit->text().toUpper());

    // eliminamos los espacios de la cadena
    ui->rifEdit->setText(ui->rifEdit->text().trimmed());
}

void rif::replyFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QString str(data);

    QDomDocument doc;
    doc.setContent(data);
    QDomNodeList nombre = doc.elementsByTagName("rif:Nombre");
    QDomNodeList agente = doc.elementsByTagName("rif:AgenteRetencionIVA");
    QDomNodeList contribuyente = doc.elementsByTagName("rif:ContribuyenteIVA");
    QDomNodeList tasa = doc.elementsByTagName("rif:Tasa");

    if (nombre.size() > 0)
    {
        ui->nombreEdit->setText(nombre.at(0).toElement().text().trimmed());

        // Transformamos el nombre original del SENIAT al Sugerido
        QString nombreOriginal = nombre.at(0).toElement().text().trimmed();
        QStringList sugerido = nombreOriginal.split("(");
        QString nombreSugerido;
        if (sugerido.size() > 1)
        {
            nombreSugerido.append(sugerido.at(0));
        }
        else
        {
            nombreSugerido.append(nombreOriginal);
        }
        nombreSugerido.replace(")", "");
        qDebug() << "Nombre Original:" << nombreOriginal.trimmed();
        qDebug() << "Nombre Sugerido:" << nombreSugerido.trimmed();
        ui->sugeridoEdit->setText(nombreSugerido.trimmed());

    }
    if (contribuyente.size() > 0)
    {
         qDebug() << "¿Contribuyente de IVA?:" << contribuyente.at(0).toElement().text();
         ui->contribuyenteEdit->setText(contribuyente.at(0).toElement().text().trimmed());
    }

    if (agente.size() > 0)
    {
         qDebug() << "¿Agente de Retecion?:" << agente.at(0).toElement().text();
         ui->retencionEdit->setText(agente.at(0).toElement().text().trimmed());
    }
    if (tasa.size() > 0)
    {
        ui->porcentajeSpin->setValue(tasa.at(0).toElement().text().toDouble());
    }
    reply->close();
    reply->disconnect();
}

void rif::on_consultarButton_clicked()
{
    // Borramos todos los campos
    borrarCampos();

    // Implementamos una condicion si el campo rif esta vacio
    if (ui->rifEdit->text().isEmpty())
    {
        QMessageBox::critical(this, trUtf8("Consulta de rif"),
                                    trUtf8("Ingrese el rif a consultar"));
    }
    else
    {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));

        manager->get(QNetworkRequest(QUrl("http://contribuyente.seniat.gob.ve/getContribuyente/getrif?rif=" + this->_rif())));


        // this->validarrif();

        //manager->disconnect();
    }

}

void rif::validarRIF()
{
    // TODO: calcular el Digito Validador

}



#include <QtGui>
#include <QLabel>
#include <QIODevice>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QAbstractScrollArea>
#include "myclass.h"
#include <QHeaderView>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

/**
 * The constructor of MyClass. It displays conversion
 * rate information about crypto-currencies.
 * The http request is sent to get the list of coins with their ids, names
 * and symbols to search for the specific coins' ids by thier name or symbol.
 * Calls replyFinished function when the retrieval
 * of the data from the web page is completed
 * @param parent
 */
MyClass::MyClass(QTableView *parent) : QTableWidget(parent){
    //this URL is used to retrieve the information about all the crypto-currencies
     QString url = "https://api.coingecko.com/api/v3/coins/list";

     manager = new QNetworkAccessManager(this) ; 

     connect(manager, SIGNAL(finished(QNetworkReply *)),this, SLOT(replyFinished(QNetworkReply *)));

     // Initiate retrieval of the data from the web page.
     // When completed, slot replyFinished will be called. 
     manager->get(QNetworkRequest(QUrl(url)));
}
/**
 * The replyFinished function reads the data
 * that was fetched from the website.
 * The list of crypto-currencies that will be displayed
 * should be read from a file the name of which is obtained
 * from environment variable named MYCRYPTOCONVERT.
 * After reading file, the name or symbol of coins are matched with their
 * id using the regex and then the http request is sent to get the information about
 * the conversion rate of the crypto currencies provided
 * by the file. Calls replyFinished2 function when the retrieval
 * of the data from the web page is completed
 * @param reply
 */
void MyClass::replyFinished(QNetworkReply *reply)  {

    QString str ;

    // read the data fetched from the web site
    QString data = (QString) reply->readAll();


    QByteArray root  = qgetenv("MYCRYPTOCONVERT");
    QString fileName = QString::fromLocal8Bit(root);
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QString fileContent;
    QTextStream myStream(&file);
    int count = 0;
    while(!myStream.atEnd())
    {
        QString myString = myStream.readLine();
        myString=myString.trimmed();

        //Regex to find the crypto-currency with the given name or symbol
        QRegularExpression  re("{[^{]*(\"id\":\""+myString+"\"|\"symbol\":\""+myString+"\"|\"name\":\""+myString+"\")[^}]*}");
        QRegularExpressionMatch match = re.match(data);

        if ( match.hasMatch()) {
            QString obj_str = match.captured(0);
            QJsonDocument doc = QJsonDocument::fromJson(obj_str.toUtf8());
            QJsonObject jObject = doc.object();
            QVariantMap map = jObject.toVariantMap();

            fileContent.append(map["id"].toString());
            if(!myStream.atEnd()) {
                fileContent.append(",");
            }
        }
        else {
            continue;
        }

        count++;
    }

    QString url = "https://api.coingecko.com/api/v3/simple/price?ids=";
    url.append(fileContent);
    url.append("&vs_currencies=usd,eur,gbp");


    table = new QTableWidget(count + 1, 4);

    QHBoxLayout *layout = new QHBoxLayout;

    QHeaderView *v = table->horizontalHeader();
    QHeaderView *v2 = table->verticalHeader();
    v->setSectionResizeMode(QHeaderView::Stretch);
    v2->setSectionResizeMode(QHeaderView::Stretch);

    table->setMinimumWidth(500);
    table->setMinimumHeight(50 * (count + 1));

    layout->addWidget(table);
    setLayout(layout);

    manager = new QNetworkAccessManager(this) ;

    connect(manager, SIGNAL(finished(QNetworkReply *)),this, SLOT(replyFinished2(QNetworkReply *)));

    // Initiate retrieval of the data from the web page.
    // When completed, slot replyFinished will be called.
    manager->get(QNetworkRequest(QUrl(url)));
}



/**
 * The replyFinished2 function reads the data
 * that was fetched from the website. The data
 * is read using the JSon object. And then the table
 * items are set to QTableWidgetItems containing
 * the conversion rate information.
 * @param reply
 */
void MyClass::replyFinished2(QNetworkReply *reply)  {


     // read the data fetched from the web site 
     QString data = (QString) reply->readAll();

     QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
     QJsonObject jObject = doc.object();

     QVariantMap response = jObject.toVariantMap();

     QStringList keys = jObject.keys();
     QStringList currencies = {"usd", "eur", "gbp"};

     //Sets the table sells with the corresponding information on the conversion rate between
     //the crypto-currency and usd/eur/gbp
     for(int i = 0; i  < table->rowCount(); i++) {
         for(int j = 0; j  < table->columnCount(); j++) {
             if(i == 0 && j == 0) {
                 continue;
             }
             else if(i == 0) {
                 table->setItem(i, j, new QTableWidgetItem(currencies[j-1].toUpper()));
             }
             else if(j == 0) {
                 table->setItem(i, j, new QTableWidgetItem(keys[i-1]));
             }
             else {
                 QVariant qv = response[keys[i-1]].toMap()[currencies[j-1]];
                 table->setItem(i, j, new QTableWidgetItem(qv.toString()));
             }

         }
     }
}

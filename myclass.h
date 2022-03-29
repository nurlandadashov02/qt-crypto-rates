#include <QTableView>
#include <QtGui>
#include <QTableWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
/**
 * The MyClass class inherited from QTableWidget class
 */
class MyClass : public QTableWidget
{
    Q_OBJECT

    public:
      /**
       * Constructor of the MyClass
       * @brief MyClass
       */
      MyClass(QTableView *parent = 0);
	 
	 
    public slots:
      /**
       * The replyFinished method
       * @param reply
       */
      void replyFinished(QNetworkReply * reply) ; 
      /**
       * The replyFinished2 method
       * @param reply
       */
      void replyFinished2(QNetworkReply * reply) ;

    private:
      /**
       * private pointer variable table
       */
      QTableWidget *table;
      /**
       * private pointer variable manager
       */
      QNetworkAccessManager *manager ;
} ;

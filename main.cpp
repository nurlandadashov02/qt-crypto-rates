#include <QtCore>
#include <QApplication>
#include "myclass.h"
/**
 * a QT program that displays conversion rate information
 * about crypto-currencies.
 * @param argc
 * @param argv
 */
int main(int argc,char *argv[])
{
   /**
    * Create a QApplication object
    */
   QApplication a(argc, argv);
   /**
    * Create a MyClass object
    */
   MyClass my ;

   my.show();

   return a.exec();
}

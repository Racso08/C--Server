#include "server.h"
    #include <iostream>
    #include <fstream>
#include <QDebug>

    using namespace std;



    Server::Server(QObject* parent , quint16 port): QTcpServer(parent)
    {
        if(!servidor){
            servidor = this;
        }
      connect(this, SIGNAL(newConnection()),this, SLOT(acceptConnection()));

      if (!this->listen(QHostAddress::Any, port)) {
              qDebug() << "ERROR";
          }
    }

    Server::~Server()
    {
      delete client;
      close();
    }

    void Server::acceptConnection()
    {
      client = nextPendingConnection();

      connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
      connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));

      qDebug() << "New client from:" << client->peerAddress().toString();
    }

    void Server::startRead()
    {
        while(client->canReadLine())
        {
            QString line = QString::fromUtf8(client->readAll().trimmed());
            if (line == "Close"){
                servidor->close();
                servidor->acceptConnection();
                return;
            }
            string messageRecived = line.toStdString();
//            QString string1;
//            QXmlStreamReader reader(string1);
//            QXmlStreamWriter writer(&string1);
//            // use QXmlStreamWriter class to generate the XML
//            writer.setAutoFormatting(true);
//            writer.writeStartDocument();

//            writer.writeStartElement("xml");
//            writer.writeStartElement("Data");
//            writer.writeAttribute("Message", line.toUtf8().constData());
//            writer.writeCharacters("Mi mensaje");
//            writer.writeEndElement();
//            writer.writeEndElement();
//            writer.writeEndDocument();
            cout << "Client :\n" << messageRecived << endl;
            QString str1 = QString("FUNCIONA....");
//            string str2 =  string1.toStdString();

            /////////////////////////////////////////////////////////////////
            ///Testeo
            //char const* filename = "/home/racso/Project#2/SigueloBailando.mp3";

            std::ifstream is ("/home/racso/Project#2/SigueloBailando.mp3",std::fstream::binary);

            std::ofstream f2 ("/home/racso/Project#2/Stream.txt",std::fstream::trunc|std::fstream::binary);


             if (is) {
               is.seekg(-5,ios_base::end); //go to 5 before the end
               int end = is.tellg(); //grab that index
               std::cout<<end;
               is.seekg(22); //go to 22nd position
               int begin = is.tellg(); //grab that index
               std::cout<<begin;

               // allocate memory:
               char * buffer = new char [end-begin];

               // read data as a block:
               is.read (buffer,end-begin); //read everything from the 22nd position to 5 before the end

               is.close();

               // print content:
               //std::cout.write (buffer,length);
               f2.write(buffer,end-begin);

               delete[] buffer;
             }

            QString s_data = QString("Stream.txt");
            //QString s_data = data.trimmed();
            //QString string2 = s_data.toUtf8().constData();
            sendMessage(s_data);

            ////////////////////////////////////////////////////////////////


            //sendMessage(str1);
            QString str = QString::fromUtf8(messageRecived.c_str());
            client->write(("I've have recieved the message:" + str ).toUtf8());
        }

    }

    void Server::disconnected()
    {

        qDebug() << "Client disconnected:" << client->peerAddress().toString();

        client->write(QString("Server : I wish you didn't leave ):\n").toUtf8());
        client->waitForBytesWritten(3000);
    }


    void Server::sendMessage(QString data){


            client->write(QString(data+"\n").toUtf8());
            client->waitForBytesWritten(1000);
        }

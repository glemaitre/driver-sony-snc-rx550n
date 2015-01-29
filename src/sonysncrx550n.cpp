#include "sonysncrx550n.h"

//#include <QUrl>
// #include <QtGui>
//#include <QtNetwork>
//#include <QMessageBox>
//#include <QString>
//#include <QHttp>
//#include <iostream>

// int SonyControl::i=0;
// char SonyControl::hexVal[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
// QString SonyControl::pan="";
// QString SonyControl::tilt="";



SonySNCRX550N::SonySNCRX550N(const QString& _ip_address, QObject *parent) : 
  QObject(parent) {
  // Set up the ip address
  set_ip_address(_ip_address);
  // CamAddress = "192.168.0.100";
  // URLCommand = "http://"+CamAddress+"/command/ptzf.cgi?";
  // URLDownload = "http://"+CamAddress+"/oneshotimage.jpg";
}

// Set the IP address
void SonySNCRX550N::set_ip_address(const QString& _ip_address) {
  ip_address = _ip_address;
  // Create url for future http request
  url_request_command.setUrl("http://" + ip_address + "/command/ptzf.cgi?");
  url_request_one_shot.setUrl("http://" + ip_address + "/oneshotimage.jpg");
}

// Relative motion - The different parameters are given as:
// _p_angle: pan position  | -360 to 360
// _t_angle: tilt position |  -90 to 90
// speed: engine speed     |    1 to 24
void SonySNCRX550N::relative_motion(const long _p_angle, const long _t_angle, const long speed) {
  QString query = convert_panning_rel_to_hex(_p_angle) + "," + convert_tilt_rel_to_hex(_t_angle) + "," + QString::number(speed);
  url_request_command.addQueryItem("relativepantilt", query);
}

// void SonyControl::startRequests(QUrl _url)
// {
//   qnam.get(QNetworkRequest(_url));
// }

// void SonyControl::startRequestDownload(QUrl _url)
// {
//   downloadFinished = false;
//   reply = qnam.get(QNetworkRequest(_url));
//   connect(reply, SIGNAL(finished()),
// 	  this, SLOT(httpFinished()));
//   connect(reply, SIGNAL(readyRead()),
// 	  this, SLOT(httpReadyRead()));

// }

// void SonyControl::absoluteConf(int _apan, int _atilt, int _aspeed){
//   url = URLCommand;
//   QString a = convToHexPan(_apan) +","+convToHexTilt(_atilt)+","+QString("%1").arg(_aspeed);
//   url.addQueryItem("absolutepantilt",a);
//   startRequests(url);
// }

// //For relative movement
// void SonyControl::relativeConf(int _rpan, int _rtilt, int _rspeed){
//   url = URLCommand;
//   QString a = convToHexPan(_rpan)+","+convToHexTilt(_rtilt)+","+QString("%1").arg(_rspeed);
//   url.addQueryItem("relativepantilt",a);
//   startRequests(url);
// }


// QString SonyControl::convToHexPan(int _decP){
//   uint step=floor(_decP/0.022);
//   char hash[4];
//   uint x[4];

//   //converting to hexa
//   x[0] = step & 0xF000;
//   x[0] = x[0]>>12;
//   x[1] = step & 0x0F00;
//   x[1] = x[1]>>8;
//   x[2] = step & 0x00F0;
//   x[2] = x[2]>>4;
//   x[3] = step & 0x000F;

//   hash[0]= SonyControl::hexVal[x[0]];
//   hash[1]= SonyControl::hexVal[x[1]];
//   hash[2]= SonyControl::hexVal[x[2]];
//   hash[3]= SonyControl::hexVal[x[3]];

//   QString p = QString(hash[0])+QString(hash[1])+QString(hash[2])+QString(hash[3]);
//   return(p);
// }

// QString SonyControl::convToHexTilt(int _decT){
//   uint step=floor(_decT/0.023);
//   char hash[4];
//   uint x[4];

//   //converting to hexa
//   x[0] = step & 0xF000;
//   x[0] = x[0]>>12;
//   x[1] = step & 0x0F00;
//   x[1] = x[1]>>8;
//   x[2] = step & 0x00F0;
//   x[2] = x[2]>>4;
//   x[3] = step & 0x000F;

//   hash[0]= SonyControl::hexVal[x[0]];
//   hash[1]= SonyControl::hexVal[x[1]];
//   hash[2]= SonyControl::hexVal[x[2]];
//   hash[3]= SonyControl::hexVal[x[3]];

//   QString t = QString(hash[0]).append(QString(hash[1]).append(QString(hash[2]).append(QString(hash[3]))));
//   return t;

// }

// void SonyControl::downloadFile(QString fname)
// {
//   url = URLDownload;

//   //QFileInfo fileInfo(url.path());

//   if (i>32){
//     i=0;
//   };
//   i=i+1;
//   fileName = fname+QString("%1").arg(i,2,'d',0,'0')+".png";
//   file = new QFile(fileName);
//   if (!file->open(QIODevice::WriteOnly)) {
//     QMessageBox::information(0, tr("HTTP"),
// 			     tr("Unable to save the file %1: %2.")
// 			     .arg(fileName).arg(file->errorString()));
//     delete file;
//     file = 0;
//     return;
//   }
//   startRequestDownload(url);
// }

// void SonyControl::httpFinished()
// {

//   file->flush();
//   file->close();
//   reply->deleteLater();
//   reply = 0;
//   delete file;
//   file = 0;

//   downloadFinished = true;
// }

// void SonyControl::httpReadyRead()
// {
//   // this slot gets called every time the QNetworkReply has new data.
//   // We read all of its new data and write it into the file.
//   // That way we use less RAM than when reading it at the finished()
//   // signal of the QNetworkReply
//   if (file)
//     file->write(reply->readAll());
// }

// void SonyControl::delay(int _delay) {
//   QTime dieTime= QTime::currentTime().addSecs(_delay);
//   while( QTime::currentTime() < dieTime )
//     QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
// }

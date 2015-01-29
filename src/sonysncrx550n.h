#ifndef SONYSNCRX550N_H_
#define SONYSNCRX550N_H_

// stl library
#include <iostream>

// qt library
//#include <QtCore>
#include <QObject>
#include <QString>
//#include <QtNetwork>
#include <QUrl>


class SonySNCRX550N : public QObject
{
  Q_OBJECT

  /* PUBLIC MEMBERS AND FUNCTIONS */

public:
  // Constructor
  explicit SonySNCRX550N(const QString& _ip_address, QObject *parent = 0);

  /* Network management */
  // Create a function to set up the camera to a new IP address
  void set_ip_address(const QString& _ip_adress);

  /* Command management */
  // Relative motion - The different parameters are given as:
  // _p_angle: pan position  | -360 to 360
  // _t_angle: tilt position |  -90 to 90
  // speed: engine speed     |    1 to 24
  void relative_motion(const long _p_angle = 0, const long _t_angle = 0, const long speed = 0);
  
  // Absolute motion - The different parameters are given as:
  // _p_angle: pan position  | -180 to 180
  // _t_angle: tilt position |  -45 to 45
  // speed: engine speed     |    1 to 24
  void absolute_motion(const long _p_angle = 0, const long _t_angle = 0, const long speed = 0);

  /* PRIVATE MEMBERS AND FUNCTIONS */

private:
  
  /* Network management */
  // Private member regarding network information of the camera
  QString ip_address;

  /* Command management */
  // Private member regarding engine command
  QUrl url_request_command;
  // Private parameter regarding the panning
  static const long number_panning_steps = 16320;
  static const long total_panning_angle = 360;
  static const long min_panning_rel = -360;
  static const long max_panning_rel = 360;
  static const long min_panning_abs = -180;
  static const long max_panning_abs = 180;

  // RELATIVE CASE - Private function to convert an panning angle in degrees to hexadecimal
  inline QString convert_panning_rel_to_hex(const long _p_angle) {
    if ((_p_angle < min_panning_rel)||(_p_angle > max_panning_rel)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      return QString::number(0, 16).toUpper();
    }
    return QString::number(_p_angle * number_panning_steps / total_panning_angle, 16).toUpper();
  }

  // ABSOLUTE CASE - Private function to convert an panning angle in degrees to hexadecimal
  inline QString convert_panning_abs_to_hex(const long _p_angle) {
    if ((_p_angle < min_panning_abs)||(_p_angle > max_panning_abs)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      return QString::number(0, 16).toUpper();
    }
    return QString::number(_p_angle * number_panning_steps / total_panning_angle, 16).toUpper();
  }

  // Private parameter regarding the tilting
  //FROM THE DOCUMENTATION IT SEEMS THAT THE ENGINE CANNOT REACH SOME REMAINING 6 DEGREES
  static const long number_tilt_steps = 4352;
  static const long total_tilt_angle = 96;
  static const long min_tilt_rel = -90;
  static const long max_tilt_rel = 90;
  static const long min_tilt_abs = -45;
  static const long max_tilt_abs = 45;

  // RELATIVE CASE - Private function to convert an panning angle in degrees to hexadecimal
  inline QString convert_tilt_rel_to_hex(const long _t_angle) {
    if ((_t_angle < min_tilt_rel)||(_t_angle > max_tilt_rel)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      return QString::number(0, 16).toUpper();
    }
    return QString::number(_t_angle * number_tilt_steps / total_tilt_angle, 16).toUpper();
  }

  // ABSOLUTE CASE - Private function to convert an tilt angle in degrees to hexadecimal
  inline QString convert_tilt_abs_to_hex(const long _t_angle) {
    if ((_t_angle < min_tilt_abs)||(_t_angle > max_tilt_abs)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      return QString::number(0, 16).toUpper();
    }
    return QString::number(_t_angle * number_tilt_steps / total_tilt_angle, 16).toUpper();
  }

  /* Camera management */
  // Private member regarding camera shot
  QUrl url_request_one_shot;

  // Relative motion
  

  //     QString CamAddress;
  //     QString URLCommand;
  //     QString URLDownload;
  //     QString fileName;

  //     QUrl url;
  //     QUrl urlD;
  //     QNetworkAccessManager qnam;
  //     QNetworkReply *reply;
  //     QFile *file;

  // public:
  //     /// VARIABLES

  //     static char hexVal[16];

  //     static QString pan;
  //     static QString tilt;
  //     static int i;

  //     bool downloadFinished;

  //     /// COMMON METHODS
  //     void startRequests(QUrl url);
  //     void startRequestDownload(QUrl _url);

  //     /// Functions & Relatives
  //     void relativeConf(int _rpan, int _rtilt, int _rspeed);
  //     void absoluteConf(int _apan, int _atilt, int _aspeed);
  //     QString convToHexPan(int _decP);
  //     QString convToHexTilt(int _decT);

  //     //delay
  //     void delay(int _delay);

signals:

public slots:
       // void downloadFile(QString fname);
       // void httpReadyRead();
       // void httpFinished();

};

#endif  // SONYSNCRX550N_H_

#ifndef SONYSNCRX550N_H_
#define SONYSNCRX550N_H_

// stl library
#include <iostream>
#include <vector>
#include <iterator>

// qt library
#include <QObject>
#include <QString>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>

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
  // _t_angle: tilt position |  -48 to 48
  // speed: engine speed     |    1 to 24
  void absolute_motion(const long _p_angle = 0, const long _t_angle = 0, const long speed = 0);

  /* Camera management */
  // Private function to store an image
  void grab_image();

  /* PRIVATE MEMBERS AND FUNCTIONS */

private:
  
  /* Network management */
  // Private member regarding network information of the camera
  QString ip_address;
  // Private member for network access manager
  QNetworkAccessManager* net_acc_manager;

  // Private function in order to make network requests
  void network_request(const QUrl& _url);

  /* Command management */
  // Private member regarding engine command
  QUrl url_request_abs_command;
  QUrl url_request_rel_command;

  // Private member for the position of the camera
  double pan_pos;
  double tilt_pos;
  long zoom_opt;
  long zoom_dig;
  double focus;

  // Private parameter regarding the panning
  static const long number_panning_steps = 16320;
  static constexpr double total_panning_angle = 360.0;
  static constexpr double min_panning_rel = -360.0;
  static constexpr double max_panning_rel = 360.0;
  static constexpr double min_panning_abs = -180.0;
  static constexpr double max_panning_abs = 180.0;

  // RELATIVE CASE - Private function to convert an panning angle in degrees to hexadecimal
  inline QString convert_panning_rel_to_hex(const double _p_angle) {
    if ((_p_angle < min_panning_rel)||(_p_angle > max_panning_rel)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      return QString::number(0, 16).toUpper();
    }
    return QString::number(static_cast<long> (std::round(_p_angle * static_cast<double> (number_panning_steps) / total_panning_angle)), 16).toUpper();
  }

  // ABSOLUTE CASE - Private function to convert an panning angle in degrees to hexadecimal
  inline QString convert_panning_abs_to_hex(const double _p_angle) {
    if ((_p_angle < min_panning_abs)||(_p_angle > max_panning_abs)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      return QString::number(0, 16).toUpper();
    }
    return QString::number(static_cast<long> (std::round(_p_angle * static_cast<double> (number_panning_steps) / total_panning_angle)), 16).toUpper();
  }

  // Private parameter regarding the tilting
  //FROM THE DOCUMENTATION IT SEEMS THAT THE ENGINE CANNOT REACH SOME REMAINING 6 DEGREES
  static const long number_tilt_steps = 4352;
  static constexpr double total_tilt_angle = 96.0;
  static constexpr double min_tilt_rel = -96.0;
  static constexpr double max_tilt_rel = 96.0;
  static constexpr double min_tilt_abs = -48.0;
  static constexpr double max_tilt_abs = 48.0;

  // RELATIVE CASE - Private function to convert an panning angle in degrees to hexadecimal
  inline QString convert_tilt_rel_to_hex(const double _t_angle) {
    if ((_t_angle < min_tilt_rel)||(_t_angle > max_tilt_rel)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      return QString::number(0, 16).toUpper();
    }
    return QString::number(static_cast<long> (std::round(_t_angle * static_cast<double> (number_tilt_steps) / total_tilt_angle)), 16).toUpper();
  }

  // ABSOLUTE CASE - Private function to convert an tilt angle in degrees to hexadecimal
  inline QString convert_tilt_abs_to_hex(const double _t_angle) {
    if ((_t_angle < min_tilt_abs)||(_t_angle > max_tilt_abs)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      return QString::number(0, 16).toUpper();
    }
    return QString::number(static_cast<long> (std::round(_t_angle * static_cast<double> (number_tilt_steps) / total_tilt_angle)), 16).toUpper();
  }

  /* Camera management */
  // Private member regarding camera shot
  QUrl url_request_one_shot;

public slots:
  // slot to take decision about the transmitted data
  void net_data_transmitted(QNetworkReply* _p_net_reply);



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

// signals:

// public slots:
       // void downloadFile(QString fname);
       // void httpReadyRead();
       // void httpFinished();

};

#endif  // SONYSNCRX550N_H_

/*
 * Copyright (c) 2015
 * Guillaume Lemaitre (g.lemaitre58@gmail.com)
 * Francois Rameau
 * Devesh Adlakha
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

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
#include <QEventLoop>
#include <QDir>

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
  // _t_angle: tilt position |  -96 to 96
  // speed: engine speed     |    1 to 24
  void relative_motion(const long _p_angle = 0, const long _t_angle = 0, const long _speed = 0);
  
  // Absolute motion - The different parameters are given as:
  // _p_angle: pan position  | -180 to 180
  // _t_angle: tilt position |  -48 to 48
  // speed: engine speed     |    1 to 24
  void absolute_motion(const long _p_angle = 0, const long _t_angle = 0, const long _speed = 0);

  // Function to get the position of the camera
  inline void get_camera_positions(double& _pan_pos, double& _tilt_pos, QString& _zoom_pos, QString& _focus_pos) const { _pan_pos = pan_pos; _tilt_pos = tilt_pos; _zoom_pos = zoom_pos; _focus_pos = focus_pos; }
  inline double get_pan_position() const { return pan_pos; }
  inline double get_tilt_position() const { return tilt_pos; }
  inline QString get_zoom_position() const { return zoom_pos; }
  inline QString get_focus_position() const { return focus_pos; }

  /* Camera management */
  // Private function in order to control the optical zoom
  void absolute_zoom(const QString& _zoom_position = "oz-1");

  // Private function in order to control the optical zoom
  void absolute_focus(const QString& _focus_position = "f-200");

  // Private function to store an image
  void grab_image();

  /* Computer Vision */
  // Private function for spherical acquisition - TODO IMPLEMENT ZOOM AND FOCUS
  void spherical_acquisition(const long step_pan = 18, const long step_tilt = 8, const long speed = 24, const QString& _zoom = "oz-1", const QString& _focus = "f-inf", const QString& _directory_storage = "./");

  /* PRIVATE MEMBERS AND FUNCTIONS */

private:
  
  /* Network management */
  // Private member regarding network information of the camera
  QString ip_address;
  // Private member for network access manager
  QNetworkAccessManager* net_acc_manager;
  QEventLoop loop;

  // Private function in order to make network requests
  void network_request(const QUrl& _url);

  /* Command management */
  // Private member regarding engine command
  QUrl url_request_command;

  // Private member for the position of the camera
  double pan_pos;
  double tilt_pos;
  QString zoom_pos;
  QString focus_pos;

  // Private members regarding the speed
  long speed;
  static const long low_speed = 1;
  static const long high_speed = 24;

  // Private parameter regarding the panning
  static const long number_panning_steps = 16320;
  static constexpr double total_panning_angle = 360.0;
  static constexpr double min_panning_rel = -360.0;
  static constexpr double max_panning_rel = 360.0;
  static constexpr double min_panning_abs = -180.0;
  static constexpr double max_panning_abs = 180.0;

  // RELATIVE CASE - PAN
  // Degree to hexadecimal
  inline QString convert_panning_rel_to_hex(const double _p_angle) const {
    if ((_p_angle < min_panning_rel)||(_p_angle > max_panning_rel)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      QString tmp = QString::number(0, 16).toUpper();      
      if (tmp.length() > 4)
	return tmp.mid(tmp.length() - 4, 4);
      else
	return tmp;
    }    
    QString tmp = QString::number(static_cast<long> (std::round(_p_angle * static_cast<double> (number_panning_steps) / total_panning_angle)), 16).toUpper();
    if (tmp.length() > 4)
      return tmp.mid(tmp.length() - 4, 4);
    else
      return tmp;
  }

  // ABSOLUTE CASE - PAN
  // Degree to hexadecimal
  inline QString convert_panning_abs_to_hex(const double _p_angle) const {
    if ((_p_angle < min_panning_abs)||(_p_angle > max_panning_abs)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      return QString::number(0, 16).toUpper();
      QString tmp = QString::number(0, 16).toUpper();      
      if (tmp.length() > 4)
	return tmp.mid(tmp.length() - 4, 4);
      else
	return tmp;
    }
    QString tmp = QString::number(static_cast<long> (std::round(_p_angle * static_cast<double> (number_panning_steps) / total_panning_angle)), 16).toUpper();
    if (tmp.length() > 4) {
      return tmp.mid(tmp.length() - 4, 4);;
    }
    else
      return tmp;
  }
  
  // Hexadecimal to degree
  inline double convert_panning_to_deg(const QString _p_hexa) const {
    // Convert hexadecimal to decimal steps
    short int dec_steps = std::stoul(_p_hexa.toStdString(), nullptr, 16);
    // Convert the steps to degree
    return (static_cast<double> (dec_steps) * total_panning_angle / static_cast<double> (number_panning_steps));
  }

  // Private parameter regarding the tilting
  static const long number_tilt_steps = 4352;
  static constexpr double total_tilt_angle = 96.0;
  static constexpr double min_tilt_rel = -96.0;
  static constexpr double max_tilt_rel = 96.0;
  static constexpr double min_tilt_abs = -48.0;
  static constexpr double max_tilt_abs = 48.0;

  // RELATIVE CASE - TILT
  // Degree to hexadecimal
  inline QString convert_tilt_rel_to_hex(const double _t_angle) const {
    if ((_t_angle < min_tilt_rel)||(_t_angle > max_tilt_rel)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      QString tmp = QString::number(0, 16).toUpper();      
      if (tmp.length() > 4)
	return tmp.mid(tmp.length() - 4, 4);
      else
	return tmp;
    }
    QString tmp = QString::number(static_cast<long> (std::round(_t_angle * static_cast<double> (number_tilt_steps) / total_tilt_angle)), 16).toUpper();
    if (tmp.length() > 4)
      return tmp.mid(tmp.length() - 4, 4);
    else
      return tmp;
  }

  // ABSOLUTE CASE - TILT
  // Degree to hexadecimal
  inline QString convert_tilt_abs_to_hex(const double _t_angle) const {
    if ((_t_angle < min_tilt_abs)||(_t_angle > max_tilt_abs)) {
      std::cout << "The pan angle requested is to small or to large!!! The camera will not move !!!" << std::endl;
      QString tmp = QString::number(0, 16).toUpper();      
      if (tmp.length() > 4)
	return tmp.mid(tmp.length() - 4, 4);
      else
	return tmp;
    }
    QString tmp = QString::number(static_cast<long> (std::round(_t_angle * static_cast<double> (number_tilt_steps) / total_tilt_angle)), 16).toUpper();
    if (tmp.length() > 4)
      return tmp.mid(tmp.length() - 4, 4);
    else
      return tmp;
  }

  // Hexadecimal to degree
  inline double convert_tilt_to_deg(const QString _p_hexa) const {
    // Convert hexadecimal to decimal steps
    short int dec_steps = std::stoul(_p_hexa.toStdString(), nullptr, 16);
    // Convert the steps to degree
    return (static_cast<double> (dec_steps) * total_tilt_angle / static_cast<double> (number_tilt_steps));
  }

  /* Camera management */
  // Private member regarding camera shot
  QUrl url_request_one_shot;
  QDir directory_storage;

  // DEFINE IN THE CPP FILE
  // Private member about the camera zoom
  // The input can be:
  //   - optical zoom - oz-{1 ... 26}
  //   - digital zoom - dz-{1 ... 12}
  static const std::map< QString, QString > zoom_position_key_to_hex;
  // Declare the opposit search
  static const std::map< QString, QString > zoom_position_hex_to_key;

  // Private member about the camera focus
  // The input can be:
  //   - focus - f-{inf ... 35} in mm
  static const std::map< QString, QString > focus_position_key_to_hex;
  // Declare the opposit search
  static const std::map< QString, QString > focus_position_hex_to_key;

  // Function to add some delay if needed sometimes
  void delay(int _delay);

public slots:
  // slot to take decision about the transmitted data
  void net_data_transmitted();
};

#endif  // SONYSNCRX550N_H_

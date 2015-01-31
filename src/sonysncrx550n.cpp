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

#include "sonysncrx550n.h"

// qt library
#include <QNetworkRequest>
#include <QDateTime>
#include <QFile>
#include <QStringList>

// stl library
#include <iostream>

// Initialise the value that the zoom position can take
const std::map< QString, QString > SonySNCRX550N::zoom_position_key_to_hex = { {"oz-1", "0000"}, {"oz-2", "1760"}, {"oz-3", "214C"}, {"oz-4", "2722"}, {"oz-5", "2B22"}, {"oz-6", "2E20"}, {"oz-7", "3080"}, {"oz-8", "3278"}, {"oz-9", "3426"}, {"oz-10", "359E"}, {"oz-11", "36EE"}, {"oz-12", "381C"}, {"oz-13", "392E"}, {"oz-14", "3A26"}, {"oz-15", "3B08"}, {"oz-16", "3BD4"}, {"oz-17", "3C8C"}, {"oz-18", "3D2E"}, {"oz-19", "3DBC"}, {"oz-20", "3E58"}, {"oz-21", "3EA2"}, {"oz-22", "3F00"}, {"oz-23", "3F4E"}, {"oz-24", "3F92"}, {"oz25", "3FCC"}, {"dz-1", "4000"}, {"dz-2", "6000"}, {"dz-3", "6A80"}, {"dz-4", "7000"}, {"dz-5", "7300"}, {"dz-6", "7540"}, {"dz-7", "76C0"}, {"dz-8", "7800"}, {"dz-9", "78C0"}, {"dz-10", "7980"}, {"dz-11", "7A00"}, {"dz-12", "7AC0"} };
const std::map< QString, QString > SonySNCRX550N::zoom_position_hex_to_key = { {"0000", "oz-1"}, {"1760", "oz-2"}, {"214C", "oz-3"}, {"2722", "oz-4"}, {"2B22", "oz-5"}, {"2E20", "oz-6"}, {"3080", "oz-7"}, {"3278", "oz-8"}, {"3426", "oz-9"}, {"359E", "oz-10"}, {"36EE", "oz-11"}, {"381C", "oz-12"}, {"392E", "oz-13"}, {"3A26", "oz-14"}, {"3B08", "oz-15"}, {"3BD4", "oz-16"}, {"3C8C", "oz-17"}, {"3D2E", "oz-18"}, {"3DBC", "oz-19"}, {"3E58", "oz-20"}, {"3EA2", "oz-21"}, {"3F00", "oz-22"}, {"3F4E", "oz-23"}, {"3F92", "oz-24"}, {"3FCC", "oz25"}, {"4000", "dz-1"}, {"6000", "dz-2"}, {"6A80", "dz-3"}, {"7000", "dz-4"}, {"7300", "dz-5"}, {"7540", "dz-6"}, {"76C0", "dz-7"}, {"7800", "dz-8"}, {"78C0", "dz-9"}, {"7980", "dz-10"}, {"7A00", "dz-11"}, {"7AC0", "dz-12"} };

const std::map< QString, QString > SonySNCRX550N::focus_position_key_to_hex = { {"f-inf", "1000"}, {"f-7200", "2000"}, {"f-3300", "3000"}, {"f-2000", "4000"}, {"f-1300", "5000"}, {"f-1000", "6000"}, {"f-800", "7000"}, {"f-400", "8000"}, {"f-200", "9000"}, {"f-110", "A000"}, {"f-60", "B000"}, {"f-35", "C000"} };
const std::map< QString, QString > SonySNCRX550N::focus_position_hex_to_key = { {"1000", "f-inf"}, {"2000", "f-7200"}, {"3000", "f-3300"}, {"4000", "f-2000"}, {"5000", "f-1300"}, {"6000", "f-1000"}, {"7000", "f-800"}, {"8000", "f-400"}, {"9000", "f-200"}, {"A000", "f-110"}, {"B000", "f-60"}, {"C000", "f-35"} };

SonySNCRX550N::SonySNCRX550N(const QString& _ip_address, QObject *parent) : 
  QObject(parent) {
  // Set up the ip address
  set_ip_address(_ip_address);
  // Set the original positions and speed
  pan_pos = 0;
  tilt_pos = 0;
  speed = 24;
  zoom_pos = "oz-1";
  focus_pos = "f-1300";
  // Move to the position
  absolute_motion(pan_pos, tilt_pos, speed);
  absolute_zoom(zoom_pos);
}

// Set the IP address
void SonySNCRX550N::set_ip_address(const QString& _ip_address) {
  ip_address = _ip_address;
  // Create url for future http request
  url_request_command.setUrl("http://" + ip_address + "/command/ptzf.cgi?");
  url_request_one_shot.setUrl("http://" + ip_address + "/oneshotimage.jpg");
  // Initialise the network access manager
  net_acc_manager = new QNetworkAccessManager(this);
  // Connect some signal-slot in order to be able to save the data
  connect(net_acc_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(net_data_transmitted(QNetworkReply*)));
}

// Relative motion - The different parameters are given as:
// _p_angle: pan position  | -360 to 360
// _t_angle: tilt position |  -96 to 96
// speed: engine speed     |    1 to 24
void SonySNCRX550N::relative_motion(const long _p_angle, const long _t_angle, const long speed) {
  // Check the speed
  if ((speed < low_speed) || (speed > high_speed)) {
    std::cout << "The speed set is out of range !!! Stopping action now!!!" << std::endl;
    return;
  }
  QString query = convert_panning_rel_to_hex(_p_angle) + "," + convert_tilt_rel_to_hex(_t_angle) + "," + QString::number(speed);
  url_request_command.addQueryItem("relativepantilt", query);
  // Send the request
  network_request(url_request_command);
}

// Absolute motion - The different parameters are given as:
// _p_angle: pan position  | -180 to 180
// _t_angle: tilt position |  -48 to 48
// speed: engine speed     |    1 to 24
void SonySNCRX550N::absolute_motion(const long _p_angle, const long _t_angle, const long speed) {
  // Check the speed
  if ((speed < low_speed) || (speed > high_speed)) {
    std::cout << "The speed set is out of range !!! Stopping action now!!!" << std::endl;
    return;
  }
  QString query = convert_panning_abs_to_hex(_p_angle) + "," + convert_tilt_abs_to_hex(_t_angle) + "," + QString::number(speed);
  url_request_command.addQueryItem("absolutepantilt", query);
  // Send the request
  network_request(url_request_command);
}

// Private function in order to control the zoom
void SonySNCRX550N::absolute_zoom(const QString& _zoom_position) {
  // Convert the zoom to hexadecimal code
  auto hexa_zoom = zoom_position_key_to_hex.find(_zoom_position);
  url_request_command.addQueryItem("absolutezoom", hexa_zoom->second);
  // Send the request
  network_request(url_request_command);
}

// Private function in order to control the focus
void SonySNCRX550N::absolute_focus(const QString& _focus_position) {
  url_request_command.addQueryItem("absolutefocus", _focus_position);
  // Send the request
  network_request(url_request_command);
}

// Store an image
void SonySNCRX550N::grab_image() {
  network_request(url_request_one_shot);
}

void SonySNCRX550N::network_request(const QUrl& _url) {
  // Get the reply for the url request
  net_acc_manager->get(QNetworkRequest(_url));
}

// slot to take decision about the transmitted data
void SonySNCRX550N::net_data_transmitted(QNetworkReply* _p_net_reply) {
  // If the request was to get an image
  if (_p_net_reply->url().toString().contains( url_request_one_shot.toString())) {
    // TODO - PROBABLY GET PAN TILT ZOOM FOCUS PARAMETER TO PUT INSIDE THE FILENAME
    // Get the current time in order to save the image
    QDateTime current_time = QDateTime::currentDateTimeUtc();
    QString filename = QDateTime::currentDateTimeUtc().toString(Qt::ISODate) + ".jpg";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
      std::cout << "Error while writting the image file" << std::endl;
      return;
    }
    // Save the image
    file.write(_p_net_reply->readAll());
    // Close the file
    file.close();
  }
  else if (_p_net_reply->url().toString().contains("relativepantilt")) {
    // Update the position since that the query was completed
    // Get the query which was pending
    QString params = url_request_command.queryItemValue("relativepantilt");
    QStringList params_split = params.split(",", QString::SkipEmptyParts);
    double rel_pan = convert_panning_to_deg(params_split.at(0));
    double rel_tilt = convert_panning_to_deg(params_split.at(1));
    bool ok;
    // Update the position
    speed = static_cast<long> (params_split.at(2).toInt(&ok, 10));
    pan_pos = std::fmod((pan_pos + rel_pan), total_panning_angle);
    tilt_pos += rel_tilt;
    if (tilt_pos > max_tilt_abs)
      tilt_pos = max_tilt_abs;
    else if (tilt_pos < min_tilt_abs)
      tilt_pos = min_tilt_abs;
    // Remove the url query from the list
    url_request_command.removeQueryItem("relativepantilt");
  }
  else if (_p_net_reply->url().toString().contains("absolutepantilt")) {
    // Update the position since that the query was completed
    // Get the query which was pending
    QString params = url_request_command.queryItemValue("absolutepantilt");
    QStringList params_split = params.split(",", QString::SkipEmptyParts);
    pan_pos = convert_panning_to_deg(params_split.at(0));
    tilt_pos = convert_panning_to_deg(params_split.at(1));
    bool ok;
    // Update the position
    speed = static_cast<long> (params_split.at(2).toInt(&ok, 10));
    // Remove the url query from the list
    url_request_command.removeQueryItem("absolutepantilt");
  }
  else if (_p_net_reply->url().toString().contains("absolutezoom")) {
    // Convert hexadecimal to key
    auto key_zoom = zoom_position_hex_to_key.find(url_request_command.queryItemValue("absolutezoom"));
    zoom_pos = key_zoom->second;
    // Remove the url query from the list
    url_request_command.removeQueryItem("absolutezoom");
  }
  else if (_p_net_reply->url().toString().contains("absolutefocus")) {
    auto key_focus = focus_position_hex_to_key.find(url_request_command.queryItemValue("absolutefocus"));
    focus_pos = key_focus->second;
    // Remove the url query from the list
    url_request_command.removeQueryItem("absolutefocus");
  }
  _p_net_reply->deleteLater();
}

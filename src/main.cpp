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

#include <QCoreApplication>

#include "sonysncrx550n.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  SonySNCRX550N sony_cam("192.168.0.100");

  sony_cam.relative_motion(20,20,24);

  sony_cam.grab_image();

  return a.exec();
}


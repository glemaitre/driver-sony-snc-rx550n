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


/**	
 	ImageViewSetting.h 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


#ifndef IMAGEVIEWSETTING_H
#define IMAGEVIEWSETTING_H

#include <QMap>
//#include <QPixmap>
#include <QVector>
#include <QWidget>

//class QToolButton;

class ImageViewSetting
{
public:
    ImageViewSetting();

    void scroll(int dx, int dy);
    void adjust();
    double spanX() const { return maxX - minX; }  // Max width of the widget window
    double spanY() const { return maxY - minY; }  // Max height of the widget window

    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;

private:
    static void adjustAxis(double &min, double &max, int &numTicks);
};

#endif


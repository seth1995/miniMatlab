/**	
 	NN_Plotter.h 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


#ifndef NN_PLOTTER_H
#define NN_PLOTTER_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>

#include "ImageViewSetting.h"  // Needed to define QVector<ImageViewSetting>

class QToolButton;
class ImageViewSetting;

class NN_Plotter : public QWidget
{
    Q_OBJECT;

public:
    NN_Plotter(QWidget *parent = 0);

    void clearCurve(int id);
	ImageViewSetting& getPlotSettings();
    QSize minimumSizeHint() const;
    void setPlotSettings(const ImageViewSetting &settings);
    void setCurveData(int id, const QVector<QPointF> &data);
    QSize sizeHint() const;

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void updateRubberBandRegion();
    void refreshPixmap();
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);

    enum { Margin = 50 };

    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    QMap<int, QVector<QPointF> > curveMap;
    QVector<ImageViewSetting> zoomStack;
    int curZoom;
    bool rubberBandIsShown;
    QRect rubberBandRect;
    QPixmap pixmap;

	//ImageViewSetting *currentSetting;
};

#endif

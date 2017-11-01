/**	
 	DotMatrixEditor.h 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


#ifndef DOTMATRIXEDITOR_H
#define DOTMATRIXEDITOR_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>

#include "ImageViewSetting.h"  // Needed to define QVector<ImageViewSetting>

class QToolButton;
//class ImageViewSetting;

class DotMatrixEditor : public QWidget
{
    Q_OBJECT

public:
    DotMatrixEditor(QWidget *parent = 0);
	~DotMatrixEditor();

    void setPlotSettings(const ImageViewSetting &settings);
    void setCurveData(int id, const QVector<QPointF> &data);
    void clearCurve(int id);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

	void setMatrixGrid(double x0, double y0, double gridsize,
				int xGridNum, int yGridNum);

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
    void drawCoordinateGrid(QPainter *painter);
    void drawCurves(QPainter *painter);

	void mapGridSizeToScene(double gridsize);
	QPointF mapToScene(QPointF realPnt);
	
	void drawDotBox(QPointF pos);
	void fillDotBox(QPointF pos);
	
	void drawMatrixGrid(QPainter *painter);
	void fillMatrixGrid(QPainter *painter);
	
	void redrawDotBox();	// Testing ...

    enum { Margin = 50};

    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    QMap<int, QVector<QPointF> > curveMap;
    QVector<ImageViewSetting> zoomStack;
    int curZoom;
    bool rubberBandIsShown;
    QRect rubberBandRect;
    QPixmap pixmap;

	int GridMatrixStepX;	// In scene coordinates
	int GridMatrixStepY;	// Allows aspect ratio for a matrix square dot
	double UserGridSize;	// Grid size given in user coordinates
		
	int NumGridsX;
	int NumGridsY;
	double LeftMatrixCorner;
	double UpperMatrixCorner;

	int **MatrixValue;
	QRect MatrixRect;
	QRect MovingDotBox;  // Attached to the mouse tip
};

#endif

/**	
 	DotMatrixEditor.cpp 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


#include <QtGui>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "../Main/Macro_Defs.h"
#include "ImageViewSetting.h"
#include "DotMatrixEditor.h"

//#define LINE_INFO   "\n[" << __FILE__ << " : line " << __LINE__ << " : " << __FUNCTION__ << "()]  "

DotMatrixEditor::DotMatrixEditor(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);  // So that mouse move in the Widget can be tracked
    rubberBandIsShown = false;

    zoomInButton = new QToolButton(this);
    zoomInButton->setIcon(QIcon(":/images/zoomin.png"));
    zoomInButton->adjustSize();
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

    zoomOutButton = new QToolButton(this);
    zoomOutButton->setIcon(QIcon(":/images/zoomout.png"));
    zoomOutButton->adjustSize();
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

    setPlotSettings(ImageViewSetting());

	GridMatrixStepX = 20; 	// Adjustable
	GridMatrixStepY = 20;
	UserGridSize = 10.0;

	//findRelativeToSceneRatios();  // Relative span ratios scene relative to user coords
	
	NumGridsX = 1;
	NumGridsY = 1;
	LeftMatrixCorner = 0.0;
	UpperMatrixCorner = 0.0;

	MatrixValue = NULL;

	//qDebug() << LINE_INFO << "done" << endl;
}

DotMatrixEditor::~DotMatrixEditor()
{
	if (MatrixValue) {
		for (int i = 0; i < NumGridsX; i++) {
			delete MatrixValue[i];
		}
		delete MatrixValue;
		MatrixValue = NULL;
	}
}


void DotMatrixEditor::clearCurve(int id)
{
    curveMap.remove(id);
    refreshPixmap();
}

void DotMatrixEditor::drawCurves(QPainter *painter)
{
    static const QColor colorForIds[6] = {
        Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow
    };
	
    //ImageViewSetting settings = zoomStack[curZoom];
	
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);
	
    if (! rect.isValid())
        return;

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

    QMapIterator<int, QVector<QPointF> > i(curveMap);
	
    while (i.hasNext()) 
	{
        i.next();

        int id = i.key();
        QVector<QPointF> data = i.value();
        QPolygonF polyline(data.count());

        for (int j = 0; j < data.count(); ++j) 
		{
#if 0
			// Map from the real coordiantes to the scene coordinates.
            double dx = data[j].x() - settings.minX;
            double dy = data[j].y() - settings.minY;
			
            double x = rect.left() + (dx * (rect.width() - 1)
                                         / settings.spanX());
            double y = rect.bottom() - (dy * (rect.height() - 1)
                                           / settings.spanY());
            polyline[j] = QPointF(x, y);
#endif
			polyline[j] = mapToScene(data[j]);
        }
        painter->setPen(colorForIds[uint(id) % 6]);
        painter->drawPolyline(polyline);
    }
}

/*
  Draw XY coordinates and grid.
*/
void DotMatrixEditor::drawCoordinateGrid(QPainter *painter)
{
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);
    if (! rect.isValid())
        return;

    ImageViewSetting settings = zoomStack[curZoom];
    QPen quiteDark = palette().dark().color().light();
    QPen light = palette().light().color();

    for (int i = 0; i <= settings.numXTicks; ++i) {
        int x = rect.left() + (i * (rect.width() - 1)
                                 / settings.numXTicks);  // Left to right
        double label = settings.minX + (i * settings.spanX()
                                          / settings.numXTicks);
        painter->setPen(quiteDark);
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(light);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom() + 5, 100, 20,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(label));
    }
	
    for (int j = 0; j <= settings.numYTicks; ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1)
                                   / settings.numYTicks); // Bottom to up
        double label = settings.minY + (j * settings.spanY()
                                          / settings.numYTicks);
        painter->setPen(quiteDark);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(light);
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
        painter->drawText(rect.left() - Margin, y - 10, Margin - 5, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label));
    }
	
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

/*
  Draw a grid matrix with upper-left coordinate (x0, y0) and
  numbers of x-grid and y-drid (xGridNum, yGridNum).
*/
void DotMatrixEditor::drawMatrixGrid(QPainter *painter)
{
	QPointF ul_pos = mapToScene(QPointF(LeftMatrixCorner, UpperMatrixCorner)); // Upper-left position

	mapGridSizeToScene(UserGridSize);
	
	int gridMatWidth = NumGridsX * GridMatrixStepX;
	int gridMatHeight = NumGridsY * GridMatrixStepY;
	
    QRect matrixRect(ul_pos.x(), ul_pos.y(), gridMatWidth, gridMatHeight);
	MatrixRect = matrixRect;

	painter->fillRect(MatrixRect, Qt::white); // Fill the dot matrix background

    if (! matrixRect.isValid())
        return;

    //ImageViewSetting settings = zoomStack[curZoom];
	
    QPen quiteDark = palette().dark().color().light();
    QPen light = palette().light().color();

    for (int i = 0; i <= NumGridsX; ++i) {
        int x = matrixRect.left() + (i * (matrixRect.width() - 1)
                                 / NumGridsX);
        painter->setPen(quiteDark);
        painter->drawLine(x, matrixRect.top(), x, matrixRect.bottom());
		/*
        painter->setPen(light);
        painter->drawLine(x, matrixRect.bottom(), x, matrixRect.bottom() + 5);
        */
		/*
        double label = settings.minX + (i * settings.spanX()
                                          / settings.numXTicks);
        painter->drawText(x - 50, matrixRect.bottom() + 5, 100, 20,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(label));
		*/                     
    }
	
    for (int j = 0; j <= NumGridsY; ++j) {
        int y = matrixRect.bottom() - (j * (matrixRect.height() - 1)
                                   / NumGridsY);
        painter->setPen(quiteDark);
        painter->drawLine(matrixRect.left(), y, matrixRect.right(), y);
		/*
        painter->setPen(light);
        painter->drawLine(matrixRect.left() - 5, y, matrixRect.left(), y);
        */
		/*
        double label = settings.minY + (j * settings.spanY()
                                          / settings.numYTicks);
        painter->drawText(rect.left() - Margin, y - 10, Margin - 5, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label));
		*/                          
    }
	
    painter->setPen(light);
    painter->drawRect(matrixRect.adjusted(0, 0, -1, -1));
}

void DotMatrixEditor::drawDotBox(QPointF pos)
{
    //QPainter painter;

	QPointF pos0 = MatrixRect.topLeft();
	int i, j;
	i = ( pos.x() - pos0.x() ) / GridMatrixStepX;
	j = ( pos.y() - pos0.y() ) / GridMatrixStepY;
	
	double x = pos0.x() + i * GridMatrixStepX;
	double y = pos0.y() + j * GridMatrixStepY;

	MovingDotBox = QRect(x, y, GridMatrixStepX, GridMatrixStepY);
}

/*
	Fill or clear the dot box in the matrix grid.
*/
void DotMatrixEditor::fillDotBox(QPointF pos)
{

	QPointF pos0 = MatrixRect.topLeft();
	int i, j;
	i = ( pos.x() - pos0.x() ) / GridMatrixStepX;
	j = ( pos.y() - pos0.y() ) / GridMatrixStepY;

	//qDebug() << LINE_INFO << "i = " << i << "; j = " << j;

	MatrixValue[i][j] = 1 - MatrixValue[i][j];

}

/*
	Fill the matrix grid according to the matrix values.
*/
void DotMatrixEditor::fillMatrixGrid(QPainter *painter)
{
	//qDebug() << LINE_INFO;

	if (! MatrixValue)	return;

	QPointF pos0 = MatrixRect.topLeft();
	QPen light = palette().light().color();
	painter->setPen(light);

	for (int i = 0; i < NumGridsX; i++) {
		for (int j = 0; j < NumGridsY; j++) {
			//qDebug() << LINE_INFO << "i = " << i << "; j = " << j;
			
			if ( MatrixValue[i][j] ) {
				
				double x = pos0.x() + i * GridMatrixStepX;
				double y = pos0.y() + j * GridMatrixStepY;
				
				QRect rect = QRect(x, y, GridMatrixStepX, GridMatrixStepY);
    			rect.adjust(+1, +1, -1, -1);
				painter->fillRect(rect, Qt::gray);
			}
		}
	}
}


void DotMatrixEditor::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        zoomStack[curZoom].scroll(-1, 0);  // Scroll to left
        refreshPixmap();
        break;
    case Qt::Key_Right:
        zoomStack[curZoom].scroll(+1, 0);  // Scroll to right
        refreshPixmap();
        break;
    case Qt::Key_Down:
        zoomStack[curZoom].scroll(0, -1);  // Scroll down
        refreshPixmap();
        break;
    case Qt::Key_Up:
        zoomStack[curZoom].scroll(0, +1);  // Scroll up
        refreshPixmap();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

/*
	Map the grid size from the real coordiantes to the scene coordinates.
*/
void DotMatrixEditor::mapGridSizeToScene(double gridsize)
{
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);
	
    ImageViewSetting settings = zoomStack[curZoom];
	
    GridMatrixStepX = gridsize * (rect.width() - 1) / settings.spanX();
    GridMatrixStepY = gridsize * (rect.height() - 1) / settings.spanY();
}

/*
	Map from the real coordiantes to the scene coordinates.
*/
QPointF DotMatrixEditor::mapToScene(QPointF realPnt)
{
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);
	
    ImageViewSetting settings = zoomStack[curZoom];

    double dx = realPnt.x() - settings.minX;  // Relative to the coordinate origin
    double dy = realPnt.y() - settings.minY;

	// (x, y) are scene coordinates.
    double x = rect.left() + (dx * (rect.width() - 1)
                                 / settings.spanX());
    double y = rect.bottom() - (dy * (rect.height() - 1)
                                   / settings.spanY());

	return QPointF(x, y);
}

QSize DotMatrixEditor::minimumSizeHint() const
{
    return QSize(6 * Margin, 4 * Margin);
}

/*
	Use rubber band to zoom in a rectangular region.
*/
void DotMatrixEditor::mousePressEvent(QMouseEvent *event)
{
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);

    if (event->button() == Qt::LeftButton) 
	{
        if (rect.contains(event->pos())) {
            rubberBandIsShown = true;
            rubberBandRect.setTopLeft(event->pos());
            rubberBandRect.setBottomRight(event->pos());
            updateRubberBandRegion();
            setCursor(Qt::CrossCursor);
        }
    }

	// If mouse moves into the rectangular dot matrix ...
    if (MatrixRect.contains(event->pos())) {
		//qDebug() << "mouse pressed";
		fillDotBox(event->pos());
		refreshPixmap();
    }

}

void DotMatrixEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (rubberBandIsShown) {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
    }

	// If mouse moves into the rectangular dot matrix ...
    if (MatrixRect.contains(event->pos())) {
		// qDebug() << "mouse over";
		drawDotBox(event->pos());
		refreshPixmap();
		//redrawDotBox();
    }
}

void DotMatrixEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && rubberBandIsShown) 
	{
        rubberBandIsShown = false;
        updateRubberBandRegion();
        unsetCursor();

        QRect rect = rubberBandRect.normalized();
		
        if (rect.width() < 4 || rect.height() < 4)
            return;  // Too small to magnify
		
        rect.translate(-Margin, -Margin);

        ImageViewSetting prevSettings = zoomStack[curZoom];
        ImageViewSetting settings;
        double dx = prevSettings.spanX() / (width() - 2 * Margin);
        double dy = prevSettings.spanY() / (height() - 2 * Margin);
        settings.minX = prevSettings.minX + dx * rect.left();
        settings.maxX = prevSettings.minX + dx * rect.right();
        settings.minY = prevSettings.maxY - dy * rect.bottom();
        settings.maxY = prevSettings.maxY - dy * rect.top();
        settings.adjust();

        zoomStack.resize(curZoom + 1);
        zoomStack.append(settings);
        zoomIn();
    }

	
}

void DotMatrixEditor::paintEvent(QPaintEvent * /* event */)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    if (rubberBandIsShown) {
        painter.setPen(palette().light().color());
        painter.drawRect(rubberBandRect.normalized()
                                       .adjusted(0, 0, -1, -1));
    }

    if (hasFocus()) {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().dark().color();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }
}

void DotMatrixEditor::refreshPixmap()
{
    pixmap = QPixmap(size());
    pixmap.fill(this, 0, 0);

    QPainter painter(&pixmap);
    painter.initFrom(this);
#if 0
    drawCoordinateGrid(&painter);
#endif
	if (curveMap.size()) {
	    drawCurves(&painter);
	}

	drawMatrixGrid(&painter);

	fillMatrixGrid(&painter);
	/*
    //QPen light = palette().light().color();
    //painter.setPen(light);
    */
    /*painter.setPen(Qt::red);*/
	int linewidth = 2;
	painter.setPen(QPen(Qt::red, linewidth
		/*, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin*/));
    painter.drawRect(MovingDotBox);
	
	//painter.fillRect(MovingDotBox, Qt::gray);

    update();
}

/*
	This function is under testing ...
*/
void DotMatrixEditor::redrawDotBox()
{
    //pixmap = QPixmap(size());
    //pixmap.fill(this, 0, 0);

    QPainter painter(&pixmap);
    painter.initFrom(this);

    QPen light = palette().light().color();
    painter.setPen(light);
    painter.drawRect(MovingDotBox);
	
    update();
}

void DotMatrixEditor::resizeEvent(QResizeEvent * /* event */)
{
    int x = width() - (zoomInButton->width()
                       + zoomOutButton->width() + 10);
    zoomInButton->move(x, 5);
    zoomOutButton->move(x + zoomInButton->width() + 5, 5);
    refreshPixmap();
}

void DotMatrixEditor::setCurveData(int id, const QVector<QPointF> &data)
{
    curveMap[id] = data;
    refreshPixmap();
}

/*
	The <gridsize> will be mapped to the scene size with aspect ratio
	proportional to the scene window.
*/
void DotMatrixEditor::setMatrixGrid(double x0, double y0, double gridSize,
					int xGridNum, int yGridNum)
{
	//qDebug() << LINE_INFO << endl;
	
	LeftMatrixCorner = x0;
	UpperMatrixCorner = y0;
	UserGridSize = gridSize;

	mapGridSizeToScene(gridSize);
	
	NumGridsX = (xGridNum > 0)? xGridNum : 1;
	NumGridsY = (yGridNum > 0)? yGridNum : 1;

	MatrixValue = new int*[NumGridsX];
	for (int i = 0; i < NumGridsX; i++) {
		MatrixValue[i] = new int[NumGridsY];
	}

	// Initialize the matrix values

	for (int i = 0; i < NumGridsX; i++) {
		for (int j = 0; j < NumGridsY; j++) {
			MatrixValue[i][j] = 0;
		}
	}

	//qDebug() << LINE_INFO << "done";
	
}

void DotMatrixEditor::setPlotSettings(const ImageViewSetting &settings)
{
    zoomStack.clear();
    zoomStack.append(settings);
    curZoom = 0;
    zoomInButton->hide();
    zoomOutButton->hide();
    refreshPixmap();
}

QSize DotMatrixEditor::sizeHint() const
{
    return QSize(12 * Margin, 8 * Margin);
}

void DotMatrixEditor::updateRubberBandRegion()
{
    QRect rect = rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void DotMatrixEditor::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;

    if (event->orientation() == Qt::Horizontal) {
        zoomStack[curZoom].scroll(numTicks, 0);
    } else {
        zoomStack[curZoom].scroll(0, numTicks);
    }
    refreshPixmap();
}

void DotMatrixEditor::zoomIn()
{
    if (curZoom < zoomStack.count() - 1) {
        ++curZoom;
        zoomInButton->setEnabled(curZoom < zoomStack.count() - 1);
        zoomOutButton->setEnabled(true);
        zoomOutButton->show();
        refreshPixmap();
    }
}

void DotMatrixEditor::zoomOut()
{
    if (curZoom > 0) {
        --curZoom;
        zoomOutButton->setEnabled(curZoom > 0);
        zoomInButton->setEnabled(true);
        zoomInButton->show();
        refreshPixmap();
    }
}



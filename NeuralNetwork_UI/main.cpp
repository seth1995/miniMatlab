/**	
 	main.cpp 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


#include <QtGui>

//#include "ImageViewSetting.h"
#include "NN_plotter.h"
#include "DotMatrixEditor.h"

void readFlightCurves(NN_Plotter *plotter, const QString &fileName)
{
    QVector<QPointF> data[6];
    double factX = 0.0013;
    double factY[6] = { 0.0008, 0.1, 0.2, 0.2, 0.1, 0.8 };
    double offsY[6] = { +500, -55, +309, +308, 0, 0 };
    int pos[6] = { 3, 6, 7, 8, 9, 10 };
    QFile file(fileName);
    double offsX = 0.0;

    if (file.open(QIODevice::ReadOnly)) 
	{
        QTextStream in(&file);
		
        while (!in.atEnd()) 
		{
            QString line = in.readLine();
            QStringList coords = line.split(' ',
                                            QString::SkipEmptyParts);
            if (coords.count() >= 6) 
			{
                double x = factX * coords[0].toDouble();
                if (data[0].isEmpty())
                    offsX = x;
                for (int i = 0; i < 6; ++i) {
                    double y = coords[pos[i]].toDouble();
                    data[i].append(QPointF(x - offsX,
                                           factY[i] * (y - offsY[i])));
                }
            }
        }
    }

    plotter->setCurveData(0, data[0]);
    plotter->setCurveData(1, data[1]);
    plotter->setCurveData(2, data[2]);
    plotter->setCurveData(3, data[3]);
    plotter->setCurveData(4, data[4]);
    plotter->setCurveData(5, data[5]);
	
}



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	
    DotMatrixEditor dot_editor;
    dot_editor.setWindowTitle(QObject::tr("Neural Network Plotter"));

    ImageViewSetting rect_settings;
    rect_settings.minX = 0.0;		rect_settings.maxX = 100.0;
    rect_settings.minY = 0.0;		rect_settings.maxY = 100.0;
	
    dot_editor.setPlotSettings(rect_settings);

	// (x0, y0, grid_size, #grids_x, #grids_y); 
	// (x0, y0) are left-upper user coordinates,
	// grid_size is in user coordiantes,
	// (grids_x, grids_y) are numbers of grids in x and y dirctions.
	dot_editor.setMatrixGrid(10, 90, 6, 6, 8);

    dot_editor.setGeometry(100, 40, 600, 600);
    dot_editor.show();

    return app.exec();
}


int main0(int argc, char *argv[])
{
    QApplication app(argc, argv);
	
    NN_Plotter plotter;
    plotter.setWindowTitle(QObject::tr("Neural Network Plotter"));
	
#if 1

    readFlightCurves(&plotter, ":/in1.txt");

#else

    int numPoints = 100;
    QVector<QPointF> points0;
    QVector<QPointF> points1;
    for (int x = 0; x < numPoints; ++x) {
        points0.append(QPointF(x, uint(qrand()) % 100));
        points1.append(QPointF(x, uint(qrand()) % 100));
    }
    plotter.setCurveData(0, points0);
    plotter.setCurveData(1, points1);

    ImageViewSetting settings;
    settings.minX = 0.0;
    settings.maxX = 100.0;
    settings.minY = 0.0;
    settings.maxY = 100.0;
    plotter.setPlotSettings(settings);
#endif

    plotter.show();

    return app.exec();
}

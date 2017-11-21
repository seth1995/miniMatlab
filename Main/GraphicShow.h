#ifndef GRPHICSHOW_H
#define GRAHICSHOW_H



#include <QPainter>
#include <QWidget>
#include "CommandView.h"


QT_BEGIN_NAMESPACE
class QComboBox;
class QToolButton;
class QLabel;
class QImage;
class CommandView;

QT_END_NAMESPACE

class Graphicshow : public QWidget
{
      Q_OBJECT;
	  	
public:

     Graphicshow();
      ~Graphicshow();
private slots:
	void chooseSource();
	//void displayResult();
private:
	void openImage();
	void loadImage(const QString &fileName,QImage *image,QToolButton *button);
	void displayResult(const       QString &fileName,QImage *image,QLabel *label);
	void chooseImage(const QString &title, QImage *image, QToolButton *button);

	QPoint imagePos(const QImage &image) const;
	QToolButton *sourceButton;
	QComboBox *operatorComboBox;
	QLabel *resultLabel;
	QLabel *directionLabel;
	QImage sourceImage;
	QImage resultImage;

	CommandView *CmdWindow;
};
#endif

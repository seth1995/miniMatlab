#include "GraphicShow.h"
#include <QtGui>
using namespace std;


static const QSize picSize(200,200);

Graphicshow::Graphicshow()
{
	//add the interface widget
	sourceButton = new QToolButton;
    sourceButton->setIconSize(picSize);

	operatorComboBox = new QComboBox;
	operatorComboBox-> addItem(tr("Text convert to imgae"));
	operatorComboBox-> addItem(tr("Super Resolution"));
	

	directionLabel = new QLabel;
	resultLabel = new QLabel;
    resultLabel->setMinimumWidth(picSize.width());
	//creat the command viewer;
	
	CmdWindow = new CommandView;
	//set choose image function
	connect(sourceButton, SIGNAL(clicked()), this, SLOT(chooseSource()));
	connect(operatorComboBox, SIGNAL(activated(int)),this, SLOT(refreshResult()));

	//QImage *resultImage;
	//resultImage -> load(":/images/Face2.png");
	//resultImage = QImage(picSize, QImage::Format_ARGB32_Premultiplied);	
	//create layout
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(CmdWindow,0,0,8,8);
	mainLayout->addWidget(sourceButton, 0, 8, 3, 1);
    mainLayout->addWidget(operatorComboBox, 3, 8);
    //mainLayout->addWidget(directionLabel, 1, 2);
    mainLayout->addWidget(resultLabel, 4, 8, 3, 1);

	mainLayout->setSizeConstraint(QLayout::SetFixedSize);

	//initial the image button
	loadImage(":/images/Face2.png", &sourceImage, sourceButton);
	displayResult(":/images/Face1.png",&resultImage,resultLabel);
	
	setLayout(mainLayout);
	setWindowTitle(tr("Smallxie Project"));


	
}



void Graphicshow::loadImage(const     QString &fileName,QImage *image,QToolButton *button)
{
	image->load(fileName);
   *image = image->scaled(picSize, Qt::KeepAspectRatio);

   	QImage fixedImage(picSize, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&fixedImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(fixedImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(imagePos(*image), *image);
	painter.end();
	
	button->setIcon(QPixmap::fromImage(fixedImage));
	*image = fixedImage;
}

void Graphicshow::chooseSource()
{
    chooseImage(tr("Choose Source Image"), &sourceImage, sourceButton);
}

void Graphicshow::displayResult(const     QString &fileName,QImage *image,QLabel *label)
{

	image->load(fileName);
	*image = image->scaled(picSize, Qt::KeepAspectRatio);
	QImage resultImage(picSize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	//painter.drawImage(0,0,resultImage);
	painter.drawImage(imagePos(*image), *image);
	painter.end();

	resultLabel->setPixmap(QPixmap::fromImage(resultImage));
	
}


void Graphicshow::chooseImage(const QString &title, QImage *image, QToolButton *button)
{
	
	QString fileName = QFileDialog::getOpenFileName(this, title);
 	if (!fileName.isEmpty())
	loadImage(fileName, image, button);
}
Graphicshow::~Graphicshow()
{
}

QPoint Graphicshow::imagePos(const QImage &image) const
{
    return QPoint((picSize.width() - image.width()) / 2,
                  (picSize.height() - image.height()) / 2);
}



#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDir>
#include <QComboBox>
#include <QStringList>
#include <QLabel>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QSpinBox>

#include <thread>

#include "pmlib.h"
#include "cameraThread.h"
#include "clickableLabel.h"

// TODO
// laplacian
// edge extraction
//   canny edge linker
//   texture extractors?
//   amp canny internal
// distortion correction

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

using namespace pmlib;

enum sourceEnum {
    se_image,
    //se_video,
    se_camera
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
    const QString imFolder = "../images/";

    QGridLayout *g1, *g2, *g3;
    QGridLayout *sourceGrid;
    QGridLayout *imageGrid;
    QGridLayout *controlGrid;
    QGridLayout *paramGrid;

    QLabel *rawImageDisplayLabel;
    ClickableLabel *processedImageDisplayLabel;
    QLabel *profileImageLabel;
    QComboBox *sourceCB;
    QComboBox *fileCB;
    QSpinBox *cameraIdSB;
    QPushButton *cameraButton;
    bool cameraRunning;
    QMap<sourceEnum, QString> sourceMap;
    bool renderReady;

    QSlider *ROI_x1, *ROI_x2, *ROI_y1, *ROI_y2;
    uint32_t roi_range_x[2];
    uint32_t roi_range_y[2];
    cv::Rect roi_rect;

    QCheckBox *showROIChkBox;
    bool showROI;
    QLabel *rawSizeLabel;
    QLabel *ROISizeLabel;
    QCheckBox *processROIChkBox;
    bool processROI;

    cv::Mat rawImage, processedImage, rawOverlay, rawBlended, profileImage;
    CameraThread *cameraThread;

    QComboBox *processingTypeCB;
    QMap<processingEnum, QString> processingMap;
    processingEnum statePE;

    QComboBox *edgeExtractCB;
    QMap<extractionEnum, QString> extractionMap;
    extractionEnum stateEE;

    QComboBox *rotationCB;
    QMap<rotationEnum, QString> rotationMap;
    rotationEnum stateRE;

    QComboBox *channelIsolateCB;
    QComboBox *channelAmplifyCB;
    QMap<channelEnum, QString> channelMap;

    QLabel *cannyKernelLabel;
    QSpinBox *cannyKernelSB;
    QLabel *cannyRatioLabel;
    QLabel *cannyRatioValueLabel;
    QSlider *cannyRatioSlider;
    QLabel *cannyThrLabel;
    QLabel *cannyThrValueLabel;
    QSlider *cannyThrSlider;
    QLabel *gaussKernelLabel;
    QSpinBox *gaussKernelSB;
    QCheckBox *regionChkBox;

    QLabel *regionThrLabel;
    QLabel *regionThrValueLabel;
    QSlider *regionThrSlider;
    QLabel *seedLabel;

    QLabel *viewAngleLabel;
    QSlider *viewAngleSlider;
    QLabel *cameraElevationLabel;
    QSlider *cameraElevationSlider;
    QLabel *FOVLabel;
    QSlider *FOVxSlider;
    QSlider *FOVySlider;
    QSpinBox *pixelCntXSB;
    QSpinBox *pixelCntYSB;
    QCheckBox *pixelCntFromImageChkBox;

    QMap<sourceEnum, QString> initializeSourceMap();
    QMap<processingEnum, QString> initializeProcessingMap();
    QMap<rotationEnum, QString> initializeRotationMap();
    QMap<channelEnum, QString> initializeChannelMap();
    QMap<extractionEnum, QString> initializeExtractionMap();
    void populateCBs();
    void populateFileCB();
    template<typename KeyType>
    void populateCBWithMap(QMap<KeyType, QString> map, QComboBox *CB);
    void cameraThreadF(cv::VideoCapture &cap);

    void putFrameIntoLabel(QLabel *label, cv::Mat image, bool keepAspectRatio = true);
    void renderLabels();
    void renderProfile();
    void addROIOverlay();

    bool ROI_isValid();
    void rescaleROI();
    void graphRender(cv::Mat &target, std::vector<cv::Vec2d> &real_edge);
    void bresenham(cv::Mat &target, int x0, int y0, int x1, int y1);

public slots:
    void loadSelectedImage(const QString &imPath);
    void onProcessedClick(QPoint pos);
    void onSourceCB(int value);
    void onCameraIdSB(int value);
    void onCameraButton();
    void onCameraFrame(const cv::Mat &frame);
    void onROIChange();
    void onShowROIChkBox(int value);

    void onProcessingCB(int value);
    void onExtractionCB(int value);
    void onRotationCB(int value);
    void onChannelIsolateCB(int value);
    void onProcessROIChkBox(int value);
    void onChannelAmplifyCB(int value);

    void onGaussKernelSB(int value);
    void onCannyThrSlider(int value);
    void onCannyRatioSlider(int value);
    void onCannyKernelSB(int value);
    void onRegionChkBox(int value);
    void onRegionThrSlider(int value);

    void onViewAngleSlider(int value);
    void onCameraElevationSlider(int value);
    void onFOVxSlider(int value);
    void onFOVySlider(int value);
    void onPixelCntXSB(int value);
    void onPixelCntYSB(int value);
    void onPixelCntChkBox(int value);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // DIALOG_H

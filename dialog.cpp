#include "dialog.h"
#include "./ui_dialog.h"


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    qRegisterMetaType<cv::Mat>("cv::Mat");

    // variable setup
    QSlider *ROISliders[] = {ui->ROI_x1, ui->ROI_x2, ui->ROI_y1, ui->ROI_y2};
    sourceMap = initializeSourceMap();
    processingMap = initializeProcessingMap();
    extractionMap = initializeExtractionMap();
    rotationMap = initializeRotationMap();
    channelMap = initializeChannelMap();

    // ui setup
    populateCBs();
    ui->rawImageDisplayLabel->setMaximumHeight(ui->rawImageDisplayLabel->parentWidget()->height()*0.45);
    ui->rawImageDisplayLabel->setMaximumWidth(ui->rawImageDisplayLabel->maximumHeight()*1.5);
    ui->processedImageDisplayLabel->setMaximumHeight(ui->processedImageDisplayLabel->parentWidget()->height()*0.45);
    ui->processedImageDisplayLabel->setMaximumWidth(ui->processedImageDisplayLabel->maximumHeight()*1.5);
    ui->profileImageLabel->setMaximumHeight(ui->processedImageDisplayLabel->parentWidget()->height()*0.45);
    ui->profileImageLabel->setMaximumWidth(ui->processedImageDisplayLabel->maximumHeight()*1.5);
    ui->cannyThrSlider->setMaximum(pmlib::getCannyLowThresholdMax());

    // setup image grid slots
    connect(ui->sourceCB, SIGNAL(currentIndexChanged(int)), this, SLOT(onSourceCB(int)));
    connect(ui->fileCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(loadSelectedImage(QString)));
    for(QSlider *slider : ROISliders){
      connect(slider, SIGNAL(sliderReleased()), this, SLOT(onROIChange()));
    }
    cameraThread = new CameraThread(this);
    cameraRunning = false;
    connect(cameraThread, &CameraThread::cameraFrameCaptured, this, &Dialog::onCameraFrame);
    connect(ui->cameraIdSB, SIGNAL(valueChanged(int)), this, SLOT(onCameraIdSB(int)));
    connect(ui->cameraButton, SIGNAL(clicked()), this, SLOT(onCameraButton()));
    connect(ui->showROIChkBox, SIGNAL(stateChanged(int)), this, SLOT(onShowROIChkBox(int)));
    connect(ui->processedImageDisplayLabel, SIGNAL(imageClicked(QPoint)), this, SLOT(onProcessedClick(QPoint)));

    // setup option grid slots
    connect(ui->processingTypeCB, SIGNAL(currentIndexChanged(int)), this, SLOT(onProcessingCB(int)));
    connect(ui->edgeExtractCB, SIGNAL(currentIndexChanged(int)), this, SLOT(onExtractionCB(int)));
    connect(ui->rotationCB, SIGNAL(currentIndexChanged(int)), this, SLOT(onRotationCB(int)));
    connect(ui->channelIsolateCB, SIGNAL(currentIndexChanged(int)), this, SLOT(onChannelIsolateCB(int)));
    connect(ui->channelAmplifyCB, SIGNAL(currentIndexChanged(int)), this, SLOT(onChannelAmplifyCB(int)));
    connect(ui->processROIChkBox, SIGNAL(stateChanged(int)), this, SLOT(onProcessROIChkBox(int)));

    //setup param grid slots
    connect(ui->gaussKernelSB, SIGNAL(valueChanged(int)), this, SLOT(onGaussKernelSB(int)));
    connect(ui->cannyThrSlider, SIGNAL(valueChanged(int)), this, SLOT(onCannyThrSlider(int)));
    connect(ui->cannyRatioSlider, SIGNAL(valueChanged(int)), this, SLOT(onCannyRatioSlider(int)));
    connect(ui->cannyKernelSB, SIGNAL(valueChanged(int)), this, SLOT(onCannyKernelSB(int)));
    connect(ui->regionChkBox, SIGNAL(stateChanged(int)), this, SLOT(onRegionChkBox(int)));
    connect(ui->regionThrSlider, SIGNAL(valueChanged(int)), this, SLOT(onRegionThrSlider(int)));

    //setup geometry grid slots
    connect(ui->viewAngleSlider, SIGNAL(valueChanged(int)), this, SLOT(onViewAngleSlider(int)));
    connect(ui->cameraElevationSlider, SIGNAL(valueChanged(int)), this, SLOT(onCameraElevationSlider(int)));
    connect(ui->FOVxSlider, SIGNAL(valueChanged(int)), this, SLOT(onFOVxSlider(int)));
    connect(ui->FOVySlider, SIGNAL(valueChanged(int)), this, SLOT(onFOVySlider(int)));
    connect(ui->pixelCntXSB, SIGNAL(valueChanged(int)), this, SLOT(onPixelCntXSB(int)));
    connect(ui->pixelCntYSB, SIGNAL(valueChanged(int)), this, SLOT(onPixelCntYSB(int)));
    connect(ui->pixelCntFromImageChkBox, SIGNAL(stateChanged(int)), this, SLOT(onPixelCntChkBox(int)));


    // set initial variable states
    // some slots need to be called and it can't be via the ui function because the default value is the first one in the enum
    stateRE = re_none;
    onSourceCB(se_image);
    onRegionThrSlider(pmlib::getRegionGrowingThreshold());
    ui->showROIChkBox->setCheckState(Qt::Checked);
    ui->processROIChkBox->setCheckState(Qt::Checked);
    ui->cannyThrSlider->setValue(pmlib::getCannyLowThreshold());
    ui->cannyRatioSlider->setValue(pmlib::getCannyThresholdRatio()*10);
    onProcessingCB(pe_none);
    ui->viewAngleSlider->setValue(ui->viewAngleSlider->maximum()/2);
    ui->cameraElevationSlider->setValue(ui->cameraElevationSlider->maximum()/2);
    ui->FOVxSlider->setValue(ui->FOVxSlider->maximum()/2);
    ui->FOVySlider->setValue(ui->FOVySlider->maximum()/2);
    ui->pixelCntFromImageChkBox->setCheckState(Qt::Checked);
    onExtractionCB(ee_strongest);

    if (ui->sourceCB->currentIndex() == se_image) loadSelectedImage(ui->fileCB->currentText());
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::populateCBs(){
    populateCBWithMap(sourceMap, ui->sourceCB);
    populateCBWithMap(processingMap, ui->processingTypeCB);
    populateCBWithMap(extractionMap, ui->edgeExtractCB);
    populateCBWithMap(rotationMap, ui->rotationCB);
    populateCBWithMap(channelMap, ui->channelIsolateCB);
    populateCBWithMap(channelMap, ui->channelAmplifyCB);
    populateFileCB();
}

void Dialog::populateFileCB()
{
    QDir directory(imFolder);
    QStringList extensionFilter;
    extensionFilter << "*.png" << "*.jpg" << "*.jpeg" << "*.tif" << "*.tiff";
    QStringList fnames = directory.entryList(extensionFilter, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    ui->fileCB->addItems(fnames);
}

template<typename KeyType>
void Dialog::populateCBWithMap(QMap<KeyType, QString> map, QComboBox *CB)
{
    QMapIterator<KeyType, QString> i(map);
    while(i.hasNext()){
        i.next();
        CB->addItem(i.value(), i.key());
    }
}

void Dialog::putFrameIntoLabel(QLabel* label, const cv::Mat frame, bool keepAspectRatio) {
    QImage qImg;
    if (frame.channels() == 3) {
        qImg = QImage((const uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888).rgbSwapped();
    } else if (frame.channels() == 1) {
        qImg = QImage((const uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_Grayscale8);
    }
    QPixmap pixmap = QPixmap::fromImage(qImg);
    if (keepAspectRatio){
      // this throws "Pixmap is a null pixmap" warnings into the console even though it isn't
      // checking pixmap.isNull() fails even when the data is valid
      // peculiar
      pixmap = pixmap.scaled(label->maximumWidth(), label->maximumHeight(), Qt::KeepAspectRatio);
    } else {
      label->setScaledContents(true);
    }
    label->setPixmap(pixmap);
}

void Dialog::renderLabels(){
  if (showROI){
    rawImage.copyTo(rawBlended);
    addROIOverlay();
    putFrameIntoLabel(ui->rawImageDisplayLabel, rawBlended, false);
  } else {
    putFrameIntoLabel(ui->rawImageDisplayLabel, rawImage, false);
  }
  if (processROI){
    process_image(rawImage, processedImage, roi_rect, statePE);
  } else {
    process_image(rawImage, processedImage, cv::Rect(0, 0, rawImage.cols, rawImage.rows), statePE);
  }
  putFrameIntoLabel(ui->processedImageDisplayLabel, processedImage);
  //putFrameIntoLabel(ui->profileImageLabel, processedImage);
  ui->rawSizeLabel->setText(QString("Raw image size: (%1, %2)").arg(QString::number(rawImage.cols), QString::number(rawImage.rows)));
  ui->ROISizeLabel->setText(QString("ROI size: (%1, %2)").arg(QString::number(processedImage.cols), QString::number(processedImage.rows)));
}

void Dialog::renderProfile(){
  if (statePE == pe_none || statePE == pe_gray) return;
  int thr = 100;
  std::vector<cv::Point> edge;
  switch (stateEE){
    case ee_strongest:
      edge = pmlib::edge_extract_strongest(processedImage, dir_e);
      break;
    case ee_strongest_and_thr:
      edge = pmlib::edge_extract_thr_strongest(processedImage, dir_e, thr);
      break;
    case ee_first_thr:
      edge = pmlib::edge_extract_thr_first(processedImage, dir_e, thr);
      break;
    default:
      break;
  }
  pmlib::add_roi_offset(edge, roi_rect);
  std::vector<cv::Vec2d> real_edge = pmlib::mapSS2RS(edge);
  /*for (cv::Point p : edge){
    std::cout << "(" << p.x << ", " << p.y << "),\t";
  }
  for (cv::Vec2d p : real_edge){
    std::cout << "(" << p[0] << ", " << p[1] << "),\t";
  }*/
  graphRender(profileImage, real_edge);
  putFrameIntoLabel(ui->profileImageLabel, profileImage);
}

void Dialog::addROIOverlay(){
  static cv::Scalar rect_color(0, 0, 255); // red
  static unsigned border_width = 10;
  rawImage.copyTo(rawBlended);
  cv::rectangle(rawBlended, roi_rect, rect_color, border_width);
}

bool Dialog::ROI_isValid(){
  return (roi_range_x[0] != roi_range_x[1] && roi_range_y[0] != roi_range_y[1]);
}

void Dialog::rescaleROI(){
  unsigned oldcols = ui->ROI_x1->maximum();
  unsigned oldrows = ui->ROI_y1->maximum();
  ui->ROI_x1->setMaximum(rawImage.cols);
  ui->ROI_x2->setMaximum(rawImage.cols);
  ui->ROI_y1->setMaximum(rawImage.rows);
  ui->ROI_y2->setMaximum(rawImage.rows);
  if ((oldcols != rawImage.cols || oldrows != rawImage.rows) && oldcols && oldrows){
    ui->ROI_x1->setValue(ui->ROI_x1->value()*rawImage.cols/oldcols);
    ui->ROI_x2->setValue(ui->ROI_x2->value()*rawImage.cols/oldcols);
    ui->ROI_y1->setValue(ui->ROI_y1->value()*rawImage.rows/oldrows);
    ui->ROI_y2->setValue(ui->ROI_y2->value()*rawImage.rows/oldrows);
  }
}

void Dialog::graphRender(cv::Mat &target, std::vector<cv::Vec2d> &real_edge){
  //std::cout << "in gr";
  //for (cv::Vec2d p : real_edge){
  //  qInfo() << p[0] << p[1];
  //}
  int th = ui->profileImageLabel->height();
  int tw = ui->profileImageLabel->width();
  target = cv::Mat::zeros(th, tw, CV_8UC1);
  double xmin = 1000000, xmax = -1000000, ymin = 1000000, ymax = -1000000;
  //std::cout << "th " << th << " tw " << tw << "\n";
  for (cv::Vec2d p : real_edge){
    xmin = p[0] < xmin ? p[0] : xmin;
    xmax = p[0] > xmax ? p[0] : xmax;
    ymin = p[1] < ymin ? p[1] : ymin;
    ymax = p[1] > ymax ? p[1] : ymax;
  }
  //std::cout << "minmax " << xmin << " " << xmax << " " << ymin << " " << ymax << "\n";
  double xdif = xmax - xmin, ydif = ymax - ymin;
  //std::cout << "diff " << xdif << " " << ydif << "\n";
  if (xdif > 100*ydif) ydif *= 10;
  if (ydif > 100*xdif) xdif *= 10;
  //std::cout << "diff " << xdif << " " << ydif << "\n";
  int x0 = (int)((real_edge[0][0] - xmin)*tw/xdif);
  int y0 = (int)((real_edge[0][1] - ymin)*th/ydif);
  int x1, y1;
  //std::cout << "x0 " << x0 << " y0 " << y0 << "\n";
  for (cv::Vec2d p : real_edge){
    x1 = (int)((p[0] - xmin)*tw/xdif);
    y1 = (int)((p[1] - ymin)*th/ydif);
    //std::cout << "x1 " << x1 << " y1 " << y1 << "\n";
    bresenham(target, x0, y0, x1, y1);
    x0 = x1;
    y0 = y1;
  }
}

void Dialog::bresenham(cv::Mat &target, int x0, int y0, int x1, int y1){
  int dx = x1 - x0;
  int dy = y1 - y0;
  int p = 2*dy - dx;
  int y = y0;
  for (int x = x0; x <= x1; x++){
    target.at<uchar>(y, x) = 255;
    if (p > 0){
      y++;
      p -= 2*dx;
    }
    p += 2*dy;
  }
}

// ============ SLOTS ===============

void Dialog::loadSelectedImage(const QString &imPath)
{
    rawImage = cv::imread((imFolder + imPath).toStdString());
    if (rawImage.empty()){
        qInfo() << "failed to load image" << imPath;
        return;
    }
    pmlib::enum_rotate(rawImage, stateRE);
    rescaleROI();
    onROIChange();
    onPixelCntChkBox(ui->pixelCntFromImageChkBox->checkState());
}

void Dialog::onProcessedClick(QPoint pos){
  int wl = ui->processedImageDisplayLabel->width();
  int hl = ui->processedImageDisplayLabel->height();
  int wr = processROI ? roi_rect.width : rawImage.cols;
  int hr = processROI ? roi_rect.height : rawImage.rows;
  double label_ar = (double)wl/hl;
  double roi_ar = (double)wr/hr;
  cv::Point seed_point;
  if (label_ar >= roi_ar){
    // maps roi to label like
    // ---------------------------
    // |           |             |
    // |           |             |
    // |           |             |
    // |    roi    |             |
    // |           |             |
    // |           |             |
    // |           |             |
    // ---------------------------
    seed_point = cv::Point(
      pos.x()*hr/hl,
      pos.y()*hr/hl
    );
    if (seed_point.x >= wr) seed_point.x = wr - 1;
  } else {
    // maps roi to label like
    // ---------------------------
    // |                         |
    // |-------------------------|
    // |                         |
    // |           roi           |
    // |                         |
    // |-------------------------|
    // |                         |
    // ---------------------------
    seed_point = cv::Point(
      pos.x()*wr/wl,
      (pos.y() - hl/2)*wr/wl + hr/2
    );
    if (seed_point.y < 0) seed_point.y = 0;
    else if (seed_point.y >= hr) seed_point.y = hr - 1;
  }
  pmlib::setSeedPoint(seed_point);
  //qInfo() << pos.x() << pos.y() << "-" << seed_point.x << seed_point.y;
  ui->seedLabel->setText(QString("Region seed point: (%1, %2)").arg(QString::number(seed_point.x), QString::number(seed_point.y)));
}

void Dialog::onSourceCB(int value)
{
  ui->cameraIdSB->hide();
  ui->cameraButton->hide();
  ui->fileCB->hide();
  if (cameraRunning){
    ui->cameraButton->setText("Start capture");
    cameraRunning = false;
    cameraThread->quit();
  }
  switch(value){
  case se_image:
    ui->fileCB->show();
    loadSelectedImage(ui->fileCB->currentText());
    break;
  //case se_video:
  //  break;
  case se_camera:
    ui->cameraIdSB->show();
    ui->cameraButton->show();
    break;
  }
  onPixelCntChkBox(ui->pixelCntFromImageChkBox->checkState());
}

void Dialog::onCameraIdSB(int value){
  if (value != cameraThread->getCameraId()){
    cameraThread->setCameraId(value);
    if (cameraRunning){
      cameraThread->quit();
      cameraThread->start();
    }
  }
}

void Dialog::onCameraButton(){
  // TODO change label text only if the capture state changes
  if (!cameraRunning){
    ui->cameraButton->setText("Stop capture");
    cameraThread->start();
    cameraRunning = true;
  } else {
    ui->cameraButton->setText("Start capture");
    cameraRunning = false;
    cameraThread->quit();
  }
}

void Dialog::onCameraFrame(const cv::Mat &frame){
  if (!frame.empty() && cameraRunning){
    rawImage = frame;
    rescaleROI();
    onROIChange();
    renderProfile();
  }
}

void Dialog::onROIChange(){
  //qInfo() << ui->ROI_x1->value() << ui->ROI_x2->value() << ui->ROI_y1->value() << ui->ROI_y2->value();
  if (ui->ROI_x1->value() < ui->ROI_x2->value()){
    roi_range_x[0] = ui->ROI_x1->value();
    roi_range_x[1] = ui->ROI_x2->value();
  } else {
    roi_range_x[0] = ui->ROI_x2->value();
    roi_range_x[1] = ui->ROI_x1->value();
  }
  if (ui->ROI_y1->value() > ui->ROI_y2->value()){
    roi_range_y[0] = rawImage.rows - ui->ROI_y1->value();
    roi_range_y[1] = rawImage.rows - ui->ROI_y2->value();
  } else {
    roi_range_y[0] = rawImage.rows - ui->ROI_y2->value();
    roi_range_y[1] = rawImage.rows - ui->ROI_y1->value();
  }
  roi_rect = ROI_isValid() ? cv::Rect(roi_range_x[0], roi_range_y[0], roi_range_x[1] - roi_range_x[0], roi_range_y[1] - roi_range_y[0]) : cv::Rect(0, 0, rawImage.cols, rawImage.rows);
  renderLabels();
  //qInfo() << roi_range_x[0] << roi_range_x[1] << roi_range_y[0] << roi_range_y[1];
}

void Dialog::onShowROIChkBox(int value){
  showROI = (bool)value;
  renderLabels();
}

// ============

void Dialog::onProcessingCB(int value){
  statePE = static_cast<processingEnum>(value);

  ui->gaussKernelLabel->hide();
  ui->gaussKernelSB->hide();
  ui->cannyKernelLabel->hide();
  ui->cannyKernelSB->hide();
  ui->cannyRatioLabel->hide();
  ui->cannyRatioSlider->hide();
  ui->cannyRatioValueLabel->hide();
  ui->cannyThrLabel->hide();
  ui->cannyThrSlider->hide();
  ui->cannyThrValueLabel->hide();
  ui->regionChkBox->hide();
  ui->regionThrLabel->hide();
  ui->regionThrValueLabel->hide();
  ui->regionThrSlider->hide();
  ui->seedLabel->hide();
  switch (statePE){
    case pe_canny:
      ui->cannyKernelLabel->show();
      ui->cannyKernelSB->show();
      ui->cannyRatioLabel->show();
      ui->cannyRatioSlider->show();
      ui->cannyRatioValueLabel->show();
      ui->cannyThrLabel->show();
      ui->cannyThrSlider->show();
      ui->cannyThrValueLabel->show();
      // fall through
    case pe_sobel:
      ui->gaussKernelLabel->show();
      ui->gaussKernelSB->show();
      break;
    case pe_region:
      statePE = pe_none;
      ui->regionChkBox->show();
      ui->regionChkBox->setCheckState(Qt::Unchecked);
      ui->regionThrLabel->show();
      ui->regionThrValueLabel->show();
      ui->regionThrSlider->show();
      ui->seedLabel->show();
      break;
    case pe_gray:
    case pe_none:
    default:
      break;
  }
  renderLabels();
}

void Dialog::onExtractionCB(int value){
  stateEE = static_cast<extractionEnum>(value);
  renderProfile();
}

void Dialog::onRotationCB(int value){
  // TODO remap roi
  rotationEnum newRE = static_cast<rotationEnum>(value);
  pmlib::enum_rotate(rawImage, newRE - stateRE);
  stateRE = newRE;
  rescaleROI();
  onROIChange();
}

void Dialog::onChannelIsolateCB(int value){
  pmlib::setIsolateTargetChannel(static_cast<channelEnum>(value));
  renderLabels();
}

void Dialog::onChannelAmplifyCB(int value){
  pmlib::setAmplifyTargetChannel(static_cast<channelEnum>(value));
  renderLabels();
}

void Dialog::onProcessROIChkBox(int value){
  processROI = (bool)value;
  renderLabels();
}

// ============

void Dialog::onGaussKernelSB(int value){
  if (value % 2 == 0){
    // only allow odd values
    value--;
    ui->gaussKernelSB->setValue(value);
    // this will cause a recursive call, so just return here
    return;
  }
  pmlib::setGaussKernelSize(value);
  renderLabels();
}

void Dialog::onCannyThrSlider(int value){
  pmlib::setCannyLowThreshold(value);
  ui->cannyThrValueLabel->setText(QString::number(value));
  renderLabels();
}

void Dialog::onCannyRatioSlider(int value){
  pmlib::setCannyThresholdRatio((double)value/10.0);
  ui->cannyRatioValueLabel->setText(QString::number((double)value/10.0));
  renderLabels();
}

void Dialog::onCannyKernelSB(int value){
  if (value % 2 == 0){
    // only allow odd values
    value--;
    ui->cannyKernelSB->setValue(value);
    // this will cause a recursive call, so just return here
    return;
  }
  pmlib::setCannyKernelSize(value);
  renderLabels();
}

void Dialog::onRegionChkBox(int value){
  if (value > 0){
    statePE = pe_region;
  } else {
    statePE = pe_none;
  }
  renderLabels();
}

void Dialog::onRegionThrSlider(int value){
  pmlib::setRegionGrowingThreshold(value);
  ui->regionThrValueLabel->setText(QString::number(value));
  if (statePE == pe_region){
    renderLabels();
  }
}

void Dialog::onViewAngleSlider(int value){
  double dvalue = (double)value/ui->viewAngleSlider->maximum()*pmlib::viewAngleMax;
  pmlib::setViewAngle(dvalue);
  ui->viewAngleLabel->setText(
    QString("View angle: %1 rad (%2 deg)").arg(
      QString::number(pmlib::getViewAngle()),
      QString::number(pmlib::getViewAngle()/pmlib::pi*180)
    )
  );
  renderProfile();
}

void Dialog::onCameraElevationSlider(int value){
  pmlib::setCameraElevation((double)value);
  ui->cameraElevationLabel->setText(
    QString("Camera elevation: %1 mm").arg(
      QString::number(pmlib::getCameraElevation()/1000)
    )
  );
  renderProfile();
}

void Dialog::onFOVxSlider(int value){
  double dvalue = (double)value/ui->FOVxSlider->maximum()*pmlib::FOVMax;
  pmlib::setFOVx(dvalue);
  ui->FOVLabel->setText(
    QString("FOV: %1x%2 rad (%3x%4 deg)").arg(
      QString::number(pmlib::getFOVx()),
      QString::number(pmlib::getFOVy()),
      QString::number(pmlib::getFOVx()/pmlib::pi*180),
      QString::number(pmlib::getFOVy()/pmlib::pi*180)
    )
  );
  renderProfile();
}

void Dialog::onFOVySlider(int value){
  double dvalue = (double)value/ui->FOVySlider->maximum()*pmlib::FOVMax;
  pmlib::setFOVy(dvalue);
  ui->FOVLabel->setText(
    QString("FOV: %1x%2 rad (%3x%4 deg)").arg(
      QString::number(pmlib::getFOVx()),
      QString::number(pmlib::getFOVy()),
      QString::number(pmlib::getFOVx()/pmlib::pi*180),
      QString::number(pmlib::getFOVy()/pmlib::pi*180)
    )
  );
  renderProfile();
}

void Dialog::onPixelCntXSB(int value){
  pmlib::setCntx(value);
  renderProfile();
}

void Dialog::onPixelCntYSB(int value){
  pmlib::setCnty(value);
  renderProfile();
}

void Dialog::onPixelCntChkBox(int value){
  if (value > 0){
    if (ui->sourceCB->currentIndex() == se_image){
      ui->pixelCntXSB->setValue(rawImage.cols);
      ui->pixelCntYSB->setValue(rawImage.rows);
    } else if (ui->sourceCB->currentIndex() == se_camera){
      ui->pixelCntXSB->setValue(cameraThread->getFrameWidth());
      ui->pixelCntYSB->setValue(cameraThread->getFrameHeight());
    } else {
      return;
    }
    ui->pixelCntXSB->setEnabled(false);
    ui->pixelCntYSB->setEnabled(false);
  } else {
    ui->pixelCntXSB->setEnabled(true);
    ui->pixelCntYSB->setEnabled(true);
  }
}

// ============ OVERRIDE ===============

void Dialog::closeEvent(QCloseEvent *event)
{
  cameraThread->quit();
  cameraThread->wait();
  QDialog::closeEvent(event);
}

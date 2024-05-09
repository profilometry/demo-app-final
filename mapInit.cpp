#include "dialog.h"

QMap<sourceEnum, QString> Dialog::initializeSourceMap(){
    QMap<sourceEnum, QString> map;
    map[se_image] = "Image folder";
    //map[se_video] = "Video";
    map[se_camera] = "Camera";
    return map;
}

QMap<processingEnum, QString> Dialog::initializeProcessingMap(){
    QMap<processingEnum, QString> map;
    map[pe_none] = "None";
    map[pe_gray] = "Grayscale";
    map[pe_canny] = "Canny edges";
    map[pe_sobel] = "Sobel kernel";
    map[pe_region] = "Region growing";
    return map;
}

QMap<rotationEnum, QString> Dialog::initializeRotationMap(){
    QMap<rotationEnum, QString> map;
    map[re_none] = "None";
    map[re_ccw] = "CCW";
    map[re_180] = "180";
    map[re_cw] = "CW";
    return map;
}

QMap<channelEnum, QString> Dialog::initializeChannelMap(){
    QMap<channelEnum, QString> map;
    map[ce_none] = "None";
    map[ce_red] = "Red";
    map[ce_green] = "Green";
    map[ce_blue] = "Blue";
    map[ce_all] = "All";
    return map;
}

QMap<extractionEnum, QString> Dialog::initializeExtractionMap(){
    QMap<extractionEnum, QString> map;
    map[ee_strongest] = "Strongest";
    map[ee_strongest_and_thr] = "Thresholded strongest";
    map[ee_first_thr] = "First threshold";
    //map[ee_offset_thr] = "Offset threshold";
    return map;
}

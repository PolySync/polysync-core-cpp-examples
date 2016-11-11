#include "VideoProcessor.hpp"

#include <PolySyncVideo.hpp>
#include <PolySyncDTCException.hpp>

#include <QImage>
#include <QPixmap>
#include <iostream>

using namespace std;

VideoProcessor::~VideoProcessor()
{
    disconnectPolySync();
}

void VideoProcessor::slotRun()
{
    connectPolySync();
}

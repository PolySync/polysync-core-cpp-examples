/*
 * Copyright (c) 2016 HARBRICK TECHNOLOGIES, INC
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <PolySyncVideo.hpp>
#include <PolySyncDTCException.hpp>

#include <QObject>

/**
 * @brief The VideoProcessor class
 * This class attempts to access a shared memory segment created by an
 * image device driver.
 *
 * @note Use polysync-sdf-configurator to add a device. In the device's IO, set
 * the shared memory key. This will tell the driver to open a shared memory
 * segment and write data to it. Make sure that the key you pass in when running
 * the example matches the key in your SDF.
 */
class VideoProcessor : public QObject
{
    // Qt Macro allowing for signals/slots
    Q_OBJECT

public:

    /**
     * @brief sharedMemoryKey
     * Key used to access existing shared memory segment created by device.
     */
    ulong _sharedMemoryKey{ 0 };

public slots:

    /**
     * @brief slotRun
     * Contains the busy loop which polls video device for frames, packages
     * the data into a QPixmap, and sends to the VideoViewer object for
     * rendering.
     */
    void slotRun();

signals:

    /**
     * @brief signalPixmap
     * Qt Signal emitted when new data is ready for rendering. This triggers
     * @ref VideoViewer::slotUpdatePixmap, passing the new QPixmap to draw.
     */
    void signalPixmap( const QPixmap & );
};
#endif // VIDEOPROCESSOR_H

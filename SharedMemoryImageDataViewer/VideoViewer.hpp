/*
 * Copyright (c) 2016 PolySync
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
#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QWidget>

class QPixmap;
class QHBoxLayout;
class QLabel;

/**
 * @brief The VideoViewer class
 * The class inherits from QWidget and contains elements necessary to view
 * a video frame.
 */
class VideoViewer : public QWidget
{
    // Qt Macro enabling Qt signals and slots
    Q_OBJECT

public:

    /**
     * @brief VideoViewer ctor
     * Allocates resources for @ref _label and @ref _layout, then adds them
     * to this widget. Calls QWidget::show() to make the widget appear on
     * screen.
     */
    VideoViewer();

public slots:

    /**
     * @brief slotUpdatePixmap
     * Triggered from VideoProcessor when new data is available to render.
     * @param [in] pixmap Updated pixmap to render on @ref _label.
     */
    void slotUpdatePixmap( const QPixmap & pixmap );

private:
    /** @{ GUI Elements */
    QHBoxLayout * _layout{ nullptr };
    QLabel * _label{ nullptr };
    /** @} */
};


#endif // VIDEOVIEWER_H

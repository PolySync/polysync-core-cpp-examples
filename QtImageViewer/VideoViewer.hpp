#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QWidget>

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
     * Allocates resources for @ref _label and @ref _layout, then adds them
     * to this widget. Calls QWidget::show() to make the widget appear on
     * screen.
     */
    VideoViewer();


public slots:

    /**
     * Triggered from VideoProcessor when new data is available to render.
     * @param [in] pixmap Updated pixmap to render on @ref _label.
     */
    void slotUpdatePixmap( const QPixmap & pixmap );


private:

    QHBoxLayout * _layout;

    QLabel * _label;

};


#endif // VIDEOVIEWER_H

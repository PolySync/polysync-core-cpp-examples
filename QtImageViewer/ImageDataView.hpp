#ifndef IMAGE_DATA_VIEW_H
#define IMAGE_DATA_VIEW_H


#include <QWidget>

class QHBoxLayout;
class QLabel;

/**
 * @brief The ImageDataView class
 * This class contains the Qt GUI view. It's job is to receive and render image
 * data.
 */
class ImageDataView : public QWidget
{
    // Qt Macro enabling Qt signals and slots
    Q_OBJECT

public:

    /**
     * Allocates resources for @ref _label and @ref _layout, then adds them
     * to this widget. Calls QWidget::show() to make the widget appear on
     * screen.
     */
    ImageDataView();


public slots:

    /**
     * Triggered from VideoProcessor when new data is available to render.
     * @param [in] pixmap Updated pixmap to render on @ref _label.
     */
    void slotRenderImage( const QImage & pixmap );


private:

    QHBoxLayout * _layout;

    QLabel * _label;

};


#endif // IMAGE_DATA_VIEW_H

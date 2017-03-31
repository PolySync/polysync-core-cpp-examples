#include "ImageDataView.hpp"

#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>


ImageDataView::ImageDataView()
    :
    _layout( new QHBoxLayout( this ) ),
    _label( new QLabel( this ) )
{
    // Set viewing window to an appropriate size.
    resize( 320, 240 );

    // Add layout element to this widget.
    setLayout( _layout );

    // Add label to this widget. The label contains the QPixmap that we update.
    _layout->addWidget( _label );

    // Make this widget/window visible on screen.
    show();
}

void ImageDataView::slotRenderImage( const QImage & image )
{
    auto pixmap = QPixmap::fromImage( image );

    if( not pixmap.isNull() )
    {
        // Update the view
        _label->setPixmap( pixmap );
    }
}

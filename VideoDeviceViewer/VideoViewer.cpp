#include "VideoViewer.hpp"

#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>


VideoViewer::VideoViewer()
    :
    _layout( new QHBoxLayout( this ) ),
    _label( new QLabel( this ) )
{
    // Set viewing window to an appropriate size.
    resize( 640, 480 );

    // Add layout element to this widget.
    setLayout( _layout );
    _label = new QLabel( this );

    // Add label to this widget. The label contains the QPixmap that we update.
    _layout->addWidget( _label );

    // Make this widget/window visible on screen.
    show();
}

void VideoViewer::slotUpdatePixmap( const QPixmap &pixmap )
{
    // Update the view
    _label->setPixmap( pixmap );
}

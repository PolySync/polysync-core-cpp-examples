#include "VideoViewer.hpp"

#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>


VideoViewer::VideoViewer()
{
    resize( 640, 480 );
    _layout = new QHBoxLayout( this );
    setLayout( _layout );

    _label = new QLabel( this );
    _layout->addWidget( _label );

    show();
}

void VideoViewer::slotUpdatePixmap( const QPixmap &pixmap )
{
    _label->setPixmap( pixmap );
}

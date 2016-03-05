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

/***************************************************************************//**
 * @file qfi_ASI.cpp
 * @author  Marek M. Cel <marekcel@marekcel.pl>
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 Marek M. Cel
 *
 * This file is part of QFlightInstruments. You can redistribute and modify it
 * under the terms of GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Further information about the GNU General Public License can also be found
 * on the world wide web at http://www.gnu.org.
 *
 * ---
 *
 * Copyright (C) 2013 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/
#ifndef QFI_ASI_CPP
#define QFI_ASI_CPP
#endif

////////////////////////////////////////////////////////////////////////////////

#include <math.h>

#include "qfi_ASI.h"

////////////////////////////////////////////////////////////////////////////////

qfi_ASI::qfi_ASI( QWidget *parent ) :
    QGraphicsView ( parent ),

    m_scene ( nullptr ),

    m_itemFace ( nullptr ),
    m_itemHand ( nullptr ),
    m_itemCase ( nullptr ),

    m_airspeed ( 0.0 ),

    m_scaleX ( 1.0 ),
    m_scaleY ( 1.0 ),

    m_originalHeight ( 240 ),
    m_originalWidth  ( 240 ),

    m_originalAsiCtr ( 120.0 , 120.0 ),

    m_faceZ ( -20 ),
    m_handZ ( -10 ),
    m_caseZ (  10 )
{
    reset();

    m_scene = new QGraphicsScene( this );
    setScene( m_scene );

    m_scene->clear();

    init();
}

////////////////////////////////////////////////////////////////////////////////

qfi_ASI::~qfi_ASI()
{
    if ( m_scene != nullptr )
    {
        m_scene->clear();
        delete m_scene;
        m_scene = nullptr;
    }

    reset();
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ASI::reinit()
{
    if ( m_scene )
    {
        m_scene->clear();

        init();
    }
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ASI::update()
{
    updateView();
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ASI::setAirspeed( double airspeed )
{
    m_airspeed = airspeed;

    if ( m_airspeed <   0.0 ) m_airspeed =   0.0;
    if ( m_airspeed > 235.0 ) m_airspeed = 235.0;
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ASI::resizeEvent( QResizeEvent *event )
{
    ////////////////////////////////////
    QGraphicsView::resizeEvent( event );
    ////////////////////////////////////

    reinit();
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ASI::init()
{
    m_scaleX = static_cast<double>(width())  / static_cast<double>(m_originalWidth);
    m_scaleY = static_cast<double>(height()) / static_cast<double>(m_originalHeight);

    reset();

    m_itemFace = new QGraphicsSvgItem( ":/images/asi/asi_face.svg" );
    m_itemFace->setCacheMode( QGraphicsItem::NoCache );
    m_itemFace->setZValue( m_faceZ );
    m_itemFace->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_scene->addItem( m_itemFace );

    m_itemHand = new QGraphicsSvgItem( ":/images/asi/asi_hand.svg" );
    m_itemHand->setCacheMode( QGraphicsItem::NoCache );
    m_itemHand->setZValue( m_handZ );
    m_itemHand->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemHand->setTransformOriginPoint( m_originalAsiCtr );
    m_scene->addItem( m_itemHand );

    m_itemCase = new QGraphicsSvgItem( ":/images/asi/asi_case.svg" );
    m_itemCase->setCacheMode( QGraphicsItem::NoCache );
    m_itemCase->setZValue( m_caseZ );
    m_itemCase->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_scene->addItem( m_itemCase );

    centerOn( width() / 2.0 , height() / 2.0 );

    updateView();
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ASI::reset()
{
    m_itemFace = nullptr;
    m_itemHand = nullptr;
    m_itemCase = nullptr;

    m_airspeed = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

void qfi_ASI::updateView()
{
    double angle = 0.0;

    if ( m_airspeed < 40.0 )
    {
        angle = 0.9 * m_airspeed;
    }
    else if ( m_airspeed < 70.0 )
    {
        angle = 36.0 + 1.8 * ( m_airspeed - 40.0 );
    }
    else if ( m_airspeed < 130.0 )
    {
        angle = 90.0 + 2.0 * ( m_airspeed - 70.0 );
    }
    else if ( m_airspeed < 160.0 )
    {
        angle = 210.0 + 1.8 * ( m_airspeed - 130.0 );
    }
    else
    {
        angle = 264.0 + 1.2 * ( m_airspeed - 160.0 );
    }

    m_itemHand->setRotation( angle );

    m_scene->update();
}

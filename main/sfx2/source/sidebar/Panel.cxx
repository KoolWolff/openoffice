/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/

#include "precompiled_sfx2.hxx"

#include "Panel.hxx"
#include "PanelTitleBar.hxx"
#include "PanelDescriptor.hxx"
#include "Theme.hxx"
#include "Paint.hxx"

#include <tools/svborder.hxx>

#include <com/sun/star/ui/XToolPanel.hpp>


using namespace css;
using namespace cssu;

namespace {
    static const char* VerticalStackLayouterName("vertical-stack");
}


namespace sfx2 { namespace sidebar {

Panel::Panel (
    const PanelDescriptor& rPanelDescriptor,
    Window* pParentWindow,
    const ::boost::function<void(void)>& rDeckLayoutTrigger)
    : Window(pParentWindow),
      msLayoutHint(rPanelDescriptor.msLayout),
      mpTitleBar(new PanelTitleBar(rPanelDescriptor.msTitle, pParentWindow, this)),
      mbIsTitleBarOptional(rPanelDescriptor.mbIsTitleBarOptional),
      mxElement(),
      mxVerticalStackLayoutElement(),
      mbIsExpanded(true),
      maDeckLayoutTrigger(rDeckLayoutTrigger)
{
    const sidebar::Paint aPaint (Theme::GetPanelBackground());
    switch(aPaint.GetType())
    {
        case Paint::NoPaint:
        default:
            SetBackground();
            break;

        case Paint::ColorPaint:
        {
            const Color aColor (aPaint.GetColor());
            SetBackground(Wallpaper(aColor));
            break;
        }
        case Paint::GradientPaint:
            SetBackground(Wallpaper(aPaint.GetGradient()));
            break;
    }
}




Panel::~Panel (void)
{
}




void Panel::Dispose (void)
{
    mxVerticalStackLayoutElement = NULL;
    {
        Reference<lang::XComponent> xComponent (mxElement, UNO_QUERY);
        mxElement = NULL;
        if (xComponent.is())
            xComponent->dispose();
    }
    {
        Reference<lang::XComponent> xComponent (mxElementWindow, UNO_QUERY);
        mxElementWindow = NULL;
        if (xComponent.is())
            xComponent->dispose();
    }
}




const ::rtl::OUString& Panel::GetLayoutHint (void) const
{
    return msLayoutHint;
}




TitleBar* Panel::GetTitleBar (void) const
{
    return mpTitleBar;
}




bool Panel::IsTitleBarOptional (void) const
{
    return mbIsTitleBarOptional;
}




void Panel::SetUIElement (const Reference<ui::XUIElement>& rxElement)
{
    mxElement = rxElement;
    if (mxElement.is())
    {
        Reference<ui::XToolPanel> xToolPanel(mxElement->getRealInterface(), UNO_QUERY);
        if (xToolPanel.is())
        {
            mxElementWindow = xToolPanel->getWindow();
            Reference<awt::XWindowPeer> xPeer (mxElementWindow, UNO_QUERY);
            if (xPeer.is())
                xPeer->setBackground(0x00000000);
        }

        if (msLayoutHint.equalsAscii(VerticalStackLayouterName))
            mxVerticalStackLayoutElement.set(mxElement->getRealInterface(), UNO_QUERY);
    }
}




void Panel::SetExpanded (const bool bIsExpanded)
{
    if (mbIsExpanded != bIsExpanded)
    {
        mbIsExpanded = bIsExpanded;
        maDeckLayoutTrigger();
    }
}




bool Panel::IsExpanded (void) const
{
    return mbIsExpanded;
}




void Panel::Paint (const Rectangle& rUpdateArea)
{
    Window::Paint(rUpdateArea);
}




void Panel::SetPosSizePixel (
    long nX,
    long nY,
    long nWidth,
    long nHeight,
    sal_uInt16 nFlags)
{
    Window::SetPosSizePixel(nX, nY, nWidth, nHeight, nFlags);

    if (mxElementWindow.is())
        mxElementWindow->setPosSize(0, 0, nWidth, nHeight, nFlags);
}




Reference<ui::XVerticalStackLayoutElement> Panel::GetVerticalStackElement (void) const
{
    return mxVerticalStackLayoutElement;
}

    
} } // end of namespace sfx2::sidebar

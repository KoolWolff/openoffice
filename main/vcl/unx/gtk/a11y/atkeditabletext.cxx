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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_vcl.hxx"

#include "atkwrapper.hxx"
#include "atktextattributes.hxx"

#include <com/sun/star/accessibility/XAccessibleEditableText.hpp>
#include <com/sun/star/accessibility/TextSegment.hpp>

// #include <functional>
// #include <hash_map>

#include <stdio.h>
#include <string.h>

using namespace ::com::sun::star;

static accessibility::XAccessibleEditableText*
    getEditableText( AtkEditableText *pEditableText ) throw (uno::RuntimeException)
{
    AtkObjectWrapper *pWrap = ATK_OBJECT_WRAPPER( pEditableText );
    if( pWrap )
    {
        if( !pWrap->mpEditableText && pWrap->mpContext )
        {
            uno::Any any = pWrap->mpContext->queryInterface( accessibility::XAccessibleEditableText::static_type(NULL) );
            pWrap->mpEditableText = reinterpret_cast< accessibility::XAccessibleEditableText * > (any.pReserved);
            pWrap->mpEditableText->acquire();
        }
        
        return pWrap->mpEditableText;
    }
    
    return NULL;
}


/*****************************************************************************/

extern "C" { 

static gboolean
editable_text_wrapper_set_run_attributes( AtkEditableText  *text,
                                          AtkAttributeSet  *attribute_set,
                                          gint              nStartOffset,
                                          gint              nEndOffset)
{
    try {
        accessibility::XAccessibleEditableText* pEditableText = getEditableText( text );
        if( pEditableText )
        {
            uno::Sequence< beans::PropertyValue > aAttributeList;

            if( attribute_set_map_to_property_values( attribute_set, aAttributeList ) )
                return pEditableText->setAttributes(nStartOffset, nEndOffset, aAttributeList);
        }
    }
    catch(const uno::Exception& e) {
        g_warning( "Exception in setAttributes()" );
    }

    return FALSE;
}

static void
editable_text_wrapper_set_text_contents( AtkEditableText  *text,
                                         const gchar      *string )
{
    try {
        accessibility::XAccessibleEditableText* pEditableText = getEditableText( text );
        if( pEditableText )
        {
            rtl::OUString aString ( string, strlen(string), RTL_TEXTENCODING_UTF8 );
            pEditableText->setText( aString );
        }
    }
    catch(const uno::Exception& e) {
        g_warning( "Exception in setText()" );
    }
}

static void
editable_text_wrapper_insert_text( AtkEditableText  *text,
                                   const gchar      *string,
                                   gint             length,
                                   gint             *pos )
{
    try {
        accessibility::XAccessibleEditableText* pEditableText = getEditableText( text );
        if( pEditableText )
        {
            rtl::OUString aString ( string, length, RTL_TEXTENCODING_UTF8 );
            if( pEditableText->insertText( aString, *pos ) )
                *pos += length;
        }
    }
    catch(const uno::Exception& e) {
        g_warning( "Exception in insertText()" );
    }
}

static void
editable_text_wrapper_cut_text( AtkEditableText  *text,
                                gint             start,
                                gint             end )
{
    try {
        accessibility::XAccessibleEditableText* pEditableText = getEditableText( text );
        if( pEditableText )
            pEditableText->cutText( start, end );
    }
    catch(const uno::Exception& e) {
        g_warning( "Exception in cutText()" );
    }
}

static void
editable_text_wrapper_delete_text( AtkEditableText  *text,
                                   gint             start,
                                   gint             end )
{
    try {
        accessibility::XAccessibleEditableText* pEditableText = getEditableText( text );
        if( pEditableText )
            pEditableText->deleteText( start, end );
    }
    catch(const uno::Exception& e) {
        g_warning( "Exception in deleteText()" );
    }
}

static void
editable_text_wrapper_paste_text( AtkEditableText  *text,
                                  gint             pos )
{
    try {
        accessibility::XAccessibleEditableText* pEditableText = getEditableText( text );
        if( pEditableText )
            pEditableText->pasteText( pos );
    }
    catch(const uno::Exception& e) {
        g_warning( "Exception in pasteText()" );
    }
}

static void
editable_text_wrapper_copy_text( AtkEditableText  *text,
                                 gint             start,
                                 gint             end )
{
    try {
        accessibility::XAccessibleEditableText* pEditableText = getEditableText( text );
        if( pEditableText )
            pEditableText->copyText( start, end );
    }
    catch(const uno::Exception& e) {
        g_warning( "Exception in copyText()" );
    }
}

} // extern "C"

void
editableTextIfaceInit (AtkEditableTextIface *iface)
{
  g_return_if_fail (iface != NULL);

  iface->set_text_contents = editable_text_wrapper_set_text_contents;
  iface->insert_text = editable_text_wrapper_insert_text;
  iface->copy_text = editable_text_wrapper_copy_text;
  iface->cut_text = editable_text_wrapper_cut_text;
  iface->delete_text = editable_text_wrapper_delete_text;
  iface->paste_text = editable_text_wrapper_paste_text;
  iface->set_run_attributes = editable_text_wrapper_set_run_attributes;
}

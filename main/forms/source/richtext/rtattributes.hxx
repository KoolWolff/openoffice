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



#ifndef FORMS_SOURCE_RICHTEXT_RTATTRIBUTES_HXX
#define FORMS_SOURCE_RICHTEXT_RTATTRIBUTES_HXX

#include <tools/solar.h>
#include <sal/types.h>
#include <svl/poolitem.hxx>

//........................................................................
namespace frm
{
//........................................................................

    //====================================================================
	//= misc
	//====================================================================
    /// the id of an attribute
    typedef sal_Int32   AttributeId;
    /// the "which id" of an item in an SfxItemSet
    typedef sal_uInt16      WhichId;
    /// a SFX slot id
    typedef sal_uInt16      SfxSlotId;
    /// a script type
    typedef sal_uInt16      ScriptType;

	//====================================================================
	//= AttributeCheckState
	//====================================================================
    enum AttributeCheckState
    {
        eChecked,
        eUnchecked,
        eIndetermined
    };

	//====================================================================
	//= AttributeState
	//====================================================================
    struct AttributeState
    {
    private:
        SfxItemHandle*      pItemHandle;

    public:
        AttributeCheckState eSimpleState;

    	//................................................................
        inline          AttributeState( );
        inline explicit AttributeState( AttributeCheckState _eCheckState );
        inline          AttributeState( const AttributeState& _rSource );

        inline AttributeState& operator=( const AttributeState& _rSource );

        inline bool operator==( const AttributeState& _rRHS );

        inline const SfxPoolItem* getItem() const;
        inline void setItem( const SfxPoolItem* _pItem );
    };

	//====================================================================
	//= AttributeState (inline implementation)
	//====================================================================
    //................................................................
    inline AttributeState::AttributeState( )
        :pItemHandle( NULL )
        ,eSimpleState( eIndetermined )
    {
    }

    //................................................................
    inline AttributeState::AttributeState( AttributeCheckState _eCheckState )
        :pItemHandle( NULL )
        ,eSimpleState( _eCheckState )
    {
    }

    //................................................................
    inline AttributeState::AttributeState( const AttributeState& _rSource )
        :pItemHandle( NULL )
        ,eSimpleState( eIndetermined )
    {
        operator=( _rSource );
    }

    //................................................................
    inline AttributeState& AttributeState::operator=( const AttributeState& _rSource )
    {
        if ( &_rSource == this )
            return *this;

        eSimpleState = _rSource.eSimpleState;
        setItem( _rSource.getItem() );
        return *this;
    }

    //................................................................
    inline const SfxPoolItem* AttributeState::getItem() const
    {
        return pItemHandle ? &pItemHandle->GetItem() : NULL;
    }

    //................................................................
    inline void AttributeState::setItem( const SfxPoolItem* _pItem )
    {
        if ( pItemHandle )
            delete pItemHandle;
        if ( _pItem )
            pItemHandle = new SfxItemHandle( *const_cast< SfxPoolItem* >( _pItem ) );
        else
            pItemHandle = NULL;
    }

    //................................................................
    inline bool AttributeState::operator==( const AttributeState& _rRHS )
    {
        if ( eSimpleState != _rRHS.eSimpleState )
            return false;

        if ( pItemHandle && !_rRHS.pItemHandle )
            return false;

        if ( !pItemHandle && _rRHS.pItemHandle )
            return false;

        if ( !pItemHandle && !_rRHS.pItemHandle )
            return true;

        return ( pItemHandle->GetItem() == _rRHS.pItemHandle->GetItem() );
    }

    //====================================================================
	//= IMultiAttributeDispatcher
	//====================================================================
    class IMultiAttributeDispatcher
    {
    public:
        virtual AttributeState  getState( AttributeId _nAttributeId ) const = 0;
        virtual void            executeAttribute( AttributeId _nAttributeId, const SfxPoolItem* _pArgument ) = 0;
    };

//........................................................................
} // namespace frm
//........................................................................

#endif // FORMS_SOURCE_RICHTEXT_RTATTRIBUTES_HXX


#include <iostream>
#include <ctime>
#include <addon.hxx>
#include <string>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/frame/XFrame.hpp>
#include <com/sun/star/frame/XController.hpp>
#include <com/sun/star/frame/XModel.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/sheet/XSpreadsheetDocument.hpp>
#include <com/sun/star/sheet/XSpreadsheets.hpp>
#include <com/sun/star/sheet/XSpreadsheet.hpp>
#include <com/sun/star/table/XCell.hpp>
#include <com/sun/star/text/XSimpleText.hpp>
#include <com/sun/star/text/XTextDocument.hpp>
#include <com/sun/star/text/XTextTable.hpp>
#include <com/sun/star/text/XTextContent.hpp>

#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/frame/XStorable.hpp>
#include <com/sun/star/frame/XDesktop.hpp>

#include <cppuhelper/supportsservice.hxx>
#include <osl/time.h>
#include <rtl/ustring.hxx>

using rtl::OUString;
using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::frame;
using namespace com::sun::star::sheet;
using namespace com::sun::star::table;
using namespace com::sun::star::text;
using com::sun::star::beans::PropertyValue;
using com::sun::star::util::URL;

// This is the service name an Add-On has to implement
#define SERVICE_NAME "com.sun.star.frame.ProtocolHandler"


// Local function to write a string to cell A1

void WriteStringToCell( Reference< XFrame > &rxFrame, OUString aStr )
{
    if ( !rxFrame.is() )
	return;
    
    Reference< XController > xCtrl = rxFrame->getController();
    if ( !xCtrl.is() )
	return;

    Reference< XModel > xModel = xCtrl->getModel();
    if ( !xModel.is() )
	return;

    Reference< XSpreadsheetDocument > xSpreadsheetDocument( xModel, UNO_QUERY );
    if ( !xSpreadsheetDocument.is() )
	return;

    Reference< XSpreadsheets > xSpreadsheets = xSpreadsheetDocument->getSheets();
    if ( !xSpreadsheets->hasByName("Sheet1") )
	return;

    Any aSheet = xSpreadsheets->getByName("Sheet1");

    Reference< XSpreadsheet > xSpreadsheet( aSheet, UNO_QUERY );

    Reference< XCell > xCell = xSpreadsheet->getCellByPosition(0, 0);

    xCell->setFormula(aStr);
    
    printf("DEBUG>>> Wrote \"%s\" to Cell A1\n",
	OUStringToOString( aStr, RTL_TEXTENCODING_ASCII_US ).getStr()); fflush(stdout);
}

void GetCurrDateTime( oslDateTime* pDateTime )
{
    if ( !pDateTime )
	return;
    TimeValue aTimeval;
    osl_getSystemTime( &aTimeval );
    osl_getDateTimeFromTimeValue( &aTimeval, pDateTime );
}

// filling table
void fill_table(Reference <XTextTable> &xTable, int num_of_rows, int num_of_col)
{
    Reference< XText > xText;
    Reference< XTextCursor> xTextCursor;
    Reference<XCell> xCell;

    for (auto cell : xTable->getCellNames())
    {
        xCell = xTable->getCellByName(cell);
        std::string cell_pos = "row_"  + std::to_string(cell[1] - '1' + 1) + 
                               " col_" + std::to_string(cell[0] - 'A' + 1);
        xText = Reference<XText>(xCell,UNO_QUERY);
        xTextCursor = xText->createTextCursor();
        xTextCursor->setString(OUString::createFromAscii(cell_pos.c_str()));
    }
}

// Local function to write Date to cell A1
void WriteCurrDate( Reference< XFrame > &rxFrame )
{
    srand(time(NULL));

    if ( !rxFrame.is() )
    return;

    Reference< XController > xCtrl = rxFrame->getController();
    if ( !xCtrl.is() )
    return;

    Reference< XModel > xModel = xCtrl->getModel();
    if ( !xModel.is() )
    return;
////
    Reference < XTextDocument > xTextDocument (xModel, UNO_QUERY);
    Reference< XText > xText = xTextDocument->getText();
    Reference<XTextRange> xTextRange = xText->getStart();
    Reference< XTextCursor> xTextCursor = xText->createTextCursor();

    int number_of_tables = rand() % 7 + 2;

    for (int i = 0; i < number_of_tables; i++)
    {
        xTextCursor->gotoEnd(false);
        std::string number_of_table = "Table: " + std::to_string(i);
        xTextRange->setString(OUString::createFromAscii(number_of_table.c_str()));
        Reference<XMultiServiceFactory> oDocMSF (xTextDocument,UNO_QUERY);
        Reference <XTextTable> xTable (oDocMSF->createInstance(
                OUString::createFromAscii("com.sun.star.text.TextTable")),UNO_QUERY);

        if ( !xTable.is() )
        return;
        
        int num_of_rows = rand() % 8 + 3;
        int num_of_col = rand() % 4 + 3;

        xTable->initialize(num_of_rows, num_of_col);
        xTextRange = xText->getEnd();

        Reference <XTextContent> xTextContent(xTable,UNO_QUERY);
        xText->insertTextContent(xTextRange, xTextContent,(unsigned char) 0);
        xTextRange->setString(OUString::createFromAscii("\n\n"));
        fill_table(xTable, num_of_rows, num_of_col);
    }

    // char buf[12];
    // oslDateTime aDateTime;
    // GetCurrDateTime( &aDateTime );
    // sprintf(buf, "%04d/%02d/%02d", aDateTime.Year, aDateTime.Month, aDateTime.Day);
    // WriteStringToCell( rxFrame, OUString::createFromAscii(buf) );
}

// Local function to write Time to cell A1
void WriteCurrTime( Reference< XFrame > &rxFrame )
{
    char buf[10];
    oslDateTime aDateTime;
    GetCurrDateTime( &aDateTime );
    sprintf(buf, "%02d%02d%02d", aDateTime.Hours, aDateTime.Minutes, aDateTime.Seconds);
    WriteStringToCell( rxFrame, OUString::createFromAscii(buf) );
}


// XDispatch implementer class "DateTimeWriterDispatchImpl" methods

void SAL_CALL DateTimeWriterDispatchImpl::dispatch( const URL& aURL, const Sequence < PropertyValue >& lArgs ) throw (RuntimeException)
{
    if ( aURL.Protocol.equalsAscii("inco.niocs.test.protocolhandler:") )
    {
	printf("DEBUG>>> DateTimeWriterDispatchImpl::dispatch() called. this = %p, command = %s\n", this,
	    OUStringToOString( aURL.Path, RTL_TEXTENCODING_ASCII_US ).getStr()); fflush(stdout);
        if ( aURL.Path.equalsAscii( "InsertDate" ) )
        {
            WriteCurrDate( mxFrame );
        }
        else if ( aURL.Path.equalsAscii( "InsertTime" ) )
        {
	    WriteCurrTime( mxFrame );
        }
    }
}

void SAL_CALL DateTimeWriterDispatchImpl::addStatusListener( const Reference< XStatusListener >& xControl, const URL& aURL ) throw (RuntimeException)
{
}

void SAL_CALL DateTimeWriterDispatchImpl::removeStatusListener( const Reference< XStatusListener >& xControl, const URL& aURL ) throw (RuntimeException)
{
}



// ProtocolHandler implementation "Addon" class methods

void SAL_CALL Addon::initialize( const Sequence< Any >& aArguments ) throw ( Exception, RuntimeException)
{
    Reference < XFrame > xFrame;
    if ( aArguments.getLength() )
    {
        aArguments[0] >>= xFrame;
        mxFrame = xFrame;
    }
}

Reference< XDispatch > SAL_CALL Addon::queryDispatch( const URL& aURL, const ::rtl::OUString& sTargetFrameName, sal_Int32 nSearchFlags )
                throw( RuntimeException )
{
    Reference < XDispatch > xRet;
    if ( aURL.Protocol.equalsAscii("inco.niocs.test.protocolhandler:") )
    {
	printf("DEBUG>>> Addon::queryDispatch() called. this = %p, command = %s\n", this,
	    OUStringToOString( aURL.Path, RTL_TEXTENCODING_ASCII_US ).getStr()); fflush(stdout);
        if ( aURL.Path.equalsAscii( "InsertDate" ) )
            xRet = new DateTimeWriterDispatchImpl( mxFrame );
        else if ( aURL.Path.equalsAscii( "InsertTime" ) )
            xRet = xRet = new DateTimeWriterDispatchImpl( mxFrame );
    }

    return xRet;
}


Sequence < Reference< XDispatch > > SAL_CALL Addon::queryDispatches( const Sequence < DispatchDescriptor >& seqDescripts )
            throw( RuntimeException )
{
    sal_Int32 nCount = seqDescripts.getLength();
    Sequence < Reference < XDispatch > > lDispatcher( nCount );

    for( sal_Int32 i=0; i<nCount; ++i )
        lDispatcher[i] = queryDispatch( seqDescripts[i].FeatureURL, seqDescripts[i].FrameName, seqDescripts[i].SearchFlags );

    return lDispatcher;
}


// Helper functions for the implementation of UNO component interfaces.
OUString Addon_getImplementationName()
throw (RuntimeException)
{
    return OUString ( IMPLEMENTATION_NAME );
}

Sequence< ::rtl::OUString > SAL_CALL Addon_getSupportedServiceNames()
throw (RuntimeException)
{
    Sequence < ::rtl::OUString > aRet(1);
    ::rtl::OUString* pArray = aRet.getArray();
    pArray[0] =  OUString ( SERVICE_NAME );
    return aRet;
}

Reference< XInterface > SAL_CALL Addon_createInstance( const Reference< XComponentContext > & rContext)
    throw( Exception )
{
    return (cppu::OWeakObject*) new Addon( rContext );
}

// Implementation of the recommended/mandatory interfaces of a UNO component.
// XServiceInfo
::rtl::OUString SAL_CALL Addon::getImplementationName()
    throw (RuntimeException)
{
    return Addon_getImplementationName();
}

sal_Bool SAL_CALL Addon::supportsService( const ::rtl::OUString& rServiceName )
    throw (RuntimeException)
{
    return cppu::supportsService(this, rServiceName);
}

Sequence< ::rtl::OUString > SAL_CALL Addon::getSupportedServiceNames(  )
    throw (RuntimeException)
{
    return Addon_getSupportedServiceNames();
}

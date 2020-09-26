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
#include <com/sun/star/frame/XComponentLoader.hpp>

#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/frame/XStorable.hpp>
#include <com/sun/star/frame/XDesktop.hpp>

#include <cppuhelper/supportsservice.hxx>
#include <cppuhelper/bootstrap.hxx>
#include <osl/time.h>
#include <rtl/ustring.hxx>

using rtl::OUString;
using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::bridge;
using namespace com::sun::star::frame;
using namespace com::sun::star::sheet;
using namespace com::sun::star::table;
using namespace com::sun::star::text;

using com::sun::star::beans::PropertyValue;
using com::sun::star::util::URL;
using namespace cppu;

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

// void GetCurrDateTime( oslDateTime* pDateTime )
// {
//     if ( !pDateTime )
// 	return;
//     TimeValue aTimeval;
//     osl_getSystemTime( &aTimeval );
//     osl_getDateTimeFromTimeValue( &aTimeval, pDateTime );
// }

// filling table
void fillTable(Reference <XTextTable> &xTable, int numOfRows, int numOfCol)
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
void openNewFileWithTables( Reference< XFrame > &rxFrame )
{
    srand(time(NULL));

    if ( !rxFrame.is() )
    return;

///////////////

   Reference< XComponentLoader > rComponentLoader (rxFrame, UNO_QUERY);

   if ( !rComponentLoader.is())
   {
        std::cerr << "Can't open new OOWriter file" << std::endl;
        return;
   }

   //get an instance of the OOowriter document
    Reference< XComponent > xWriterComponent = rComponentLoader->loadComponentFromURL(
        OUString::createFromAscii("private:factory/swriter"),
        OUString::createFromAscii("_blank"),
        0,
        Sequence < ::com::sun::star::beans::PropertyValue >());

    Reference < XTextDocument > xTextDocument (xWriterComponent,UNO_QUERY);
    Reference< XText > xText = xTextDocument->getText();

////////////////

    Reference<XTextRange> xTextRange = xText->getStart();
    Reference< XTextCursor> xTextCursor = xText->createTextCursor();

    int numberOfTables = rand() % 7 + 2;

    for (int i = 0; i < numberOfTables; i++)
    {
        xTextCursor->gotoEnd(false);
        std::string tableNum = "Table: " + std::to_string(i);
        xTextRange->setString(OUString::createFromAscii(tableNum.c_str()));
        Reference<XMultiServiceFactory> oDocMSF (xTextDocument,UNO_QUERY);
        Reference <XTextTable> xTable (oDocMSF->createInstance(
                OUString::createFromAscii("com.sun.star.text.TextTable")),UNO_QUERY);

        if ( !xTable.is() )
        return;
        
        int numOfRows = rand() % 8 + 3;
        int numOfCol = rand() % 4 + 3;

        xTable->initialize(numOfRows, numOfCol);
        xTextRange = xText->getEnd();

        Reference <XTextContent> xTextContent(xTable,UNO_QUERY);
        xText->insertTextContent(xTextRange, xTextContent,(unsigned char) 0);
        xTextRange->setString(OUString::createFromAscii("\n\n"));
        fillTable(xTable, numOfRows, numOfCol);
    }
}

// Local function to write Time to cell A1
void tablesHandling( Reference< XFrame > &rxFrame )
{
    if ( !rxFrame.is() )
    return;

    Reference< XController > xCtrl = rxFrame->getController();
    if ( !xCtrl.is() )
    return;

    Reference< XModel > xModel = xCtrl->getModel();
    if ( !xModel.is() )
    return;

    Reference < XTextDocument > xTextDocument (xModel, UNO_QUERY);

    if ( !xTextDocument.is() )
    {
        std::cerr << "Cant't connect into current writer document" << std::endl;
    }

    Reference< XText > xText = xTextDocument->getText();

    Reference<XTextRange> xTextRange = xText->getStart();
    Reference< XTextCursor> xTextCursor = xText->createTextCursor();

}


// XDispatch implementer class "CreatorAndTablesHandlingDispatchImpl" methods

void SAL_CALL CreatorAndTablesHandlingDispatchImpl::dispatch( const URL& aURL, const Sequence < PropertyValue >& lArgs ) throw (RuntimeException)
{
    if ( aURL.Protocol.equalsAscii("inco.niocs.test.protocolhandler:") )
    {
	printf("DEBUG>>> CreatorAndTablesHandlingDispatchImpl::dispatch() called. this = %p, command = %s\n", this,
	    OUStringToOString( aURL.Path, RTL_TEXTENCODING_ASCII_US ).getStr()); fflush(stdout);
        if ( aURL.Path.equalsAscii( "OpenNew" ) )
        {
            openNewFileWithTables( mxFrame );
        }
        else if ( aURL.Path.equalsAscii( "TablesHandling" ) )
        {
            tablesHandling( mxFrame );
        }
    }
}

void SAL_CALL CreatorAndTablesHandlingDispatchImpl::addStatusListener( const Reference< XStatusListener >& xControl, const URL& aURL ) throw (RuntimeException)
{
}

void SAL_CALL CreatorAndTablesHandlingDispatchImpl::removeStatusListener( const Reference< XStatusListener >& xControl, const URL& aURL ) throw (RuntimeException)
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
        if ( aURL.Path.equalsAscii( "OpenNew" ) )
            xRet = new CreatorAndTablesHandlingDispatchImpl( mxFrame );
        else if ( aURL.Path.equalsAscii( "TablesHandling" ) )
            xRet = xRet = new CreatorAndTablesHandlingDispatchImpl( mxFrame );
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

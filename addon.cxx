#include <iostream>
#include <ctime>
#include <addon.hxx>
#include <string>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/frame/XFrame.hpp>
#include <com/sun/star/frame/XController.hpp>
#include <com/sun/star/frame/XModel.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/lang/IndexOutOfBoundsException.hpp>
#include <com/sun/star/sheet/XSpreadsheetDocument.hpp>
#include <com/sun/star/sheet/XSpreadsheets.hpp>
#include <com/sun/star/sheet/XSpreadsheet.hpp>
#include <com/sun/star/table/XCell.hpp>
#include <com/sun/star/text/XSimpleText.hpp>
#include <com/sun/star/view/XSelectionSupplier.hpp>
#include <com/sun/star/view/XMultiSelectionSupplier.hpp>
#include <com/sun/star/text/XTextDocument.hpp>
#include <com/sun/star/text/XTextTable.hpp>
#include <com/sun/star/text/XTextContent.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <stdexcept>

#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/text/XTextTablesSupplier.hpp>
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
using namespace com::sun::star::container;
using namespace com::sun::star::beans;
using namespace com::sun::star::view;

using com::sun::star::beans::PropertyValue;
using com::sun::star::util::URL;
using namespace cppu;

// This is the service name an Add-On has to implement
#define SERVICE_NAME "com.sun.star.frame.ProtocolHandler"

// filling table
void fillTable(Reference <XTextTable> &xTable)
{
    Reference < XCell > xCell;
    Reference < XText > xText;
    Reference < XTextCursor > xTextCursor;
    Reference < XTextRange > xTextRange;

    for (auto cell : xTable->getCellNames())
    {
        xCell = xTable->getCellByName(cell);
        std::string cell_pos = "row_"  + std::to_string(cell[1] - '1' + 1) + 
                               " col_" + std::to_string(cell[0] - 'A' + 1);
        xText = Reference < XText > (xCell, UNO_QUERY);
        xTextRange = xText->getStart();
        xTextRange->setString(OUString::createFromAscii(cell_pos.c_str()));
    }
}

// Local function to write Date to cell A1
void openNewFileWithTables( Reference< XFrame > &rxFrame )
{
    srand(time(NULL));

    if ( not rxFrame.is() )
    return;

///////////////

   Reference< XComponentLoader > rComponentLoader(rxFrame, UNO_QUERY);

   if ( not rComponentLoader.is() )
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

    int numberOfTables = rand() % 7 + 2;

    for (int i = 0; i < numberOfTables; i++)
    {
        xTextRange->setString(OUString::createFromAscii(("Table: " + std::to_string(i)).c_str()));
        Reference<XMultiServiceFactory> oDocMSF (xTextDocument,UNO_QUERY);
        Reference <XTextTable> xTable (oDocMSF->createInstance(
                OUString::createFromAscii("com.sun.star.text.TextTable")),UNO_QUERY);

        if ( not xTable.is() )
        {
            std::cerr << "Couldn't get table!" << std::endl;
            return;
        }

        int numOfRows = rand() % 8 + 3;
        int numOfCol = rand() % 4 + 3;

        xTable->initialize(numOfRows, numOfCol);
        xTextRange = xText->getEnd();

        Reference <XTextContent> xTextContent(xTable,UNO_QUERY);
        xText->insertTextContent(xTextRange, xTextContent,(unsigned char) 0);
        fillTable(xTable);
        xTextRange->setString(OUString::createFromAscii("\n"));
        xTextRange = xText->getEnd();
    }
}

// transpose table
void transpose(Reference <XTextTable> &xTable)
{
    Reference <XCellRange> xCellRange(xTable, UNO_QUERY);

    if (not xCellRange.is())
    {
        std::cerr << "Some trouble connect to table" << std::endl;
        return;
    }

    try
    {
        Reference <XCell> xCell;
        Reference <XText> xText1, xText2;
        for (int i = 0; ;i++)
        {
            xCell = xCellRange->getCellByPosition(i, i);
            for (int j = 0; j < i; j++)
            {
                xText1 = Reference<XText> (xCellRange->getCellByPosition(j, i), UNO_QUERY);
                xText2 = Reference<XText> (xCellRange->getCellByPosition(i, j), UNO_QUERY);
                auto tmpString = xText1->getString();
                xText1->setString(xText2->getString());
                xText2->setString(tmpString);
            }
        }
    }
    catch (IndexOutOfBoundsException &)
    {
        /* To define table bound. If index (i, i) will exceed acceptable
        value, then further processing of this table is interrupted and
        processing of next table begins, if any.
        */
        // std::cout << "To define size of table\n" << std::endl;
        return;
    }
}

// Local function to write Time to cell A1
void tablesHandling( Reference< XFrame > &rxFrame )
{
    if (not rxFrame.is())
    return;

    Reference<XController> xCtrl = rxFrame->getController();
    if (not xCtrl.is())
    return;

    Reference<XModel> xModel = xCtrl->getModel();
    if (not xModel.is())
    return;

    Reference<XTextDocument> xTextDocument (xModel, UNO_QUERY);

    if (not xTextDocument.is())
    {
        std::cerr << "Cant't connect into current writer document" << std::endl;
        return;
    }

    Reference < XTextTablesSupplier > xTextTablesSupplier(xTextDocument, UNO_QUERY);
    Reference < XNameAccess > xNameAccess = xTextTablesSupplier->getTextTables();

    for (auto name : xNameAccess->getElementNames())
    {
        Any table = xNameAccess->getByName(name);
        Reference <XTextTable> xTable;
        table >>= xTable;
        transpose(xTable);
    }
}


// XDispatch implementer class "CreatorAndTablesHandlingDispatchImpl" methods

void SAL_CALL CreatorAndTablesHandlingDispatchImpl::dispatch( const URL& aURL, const Sequence < PropertyValue >& lArgs )
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

void SAL_CALL CreatorAndTablesHandlingDispatchImpl::addStatusListener( const Reference< XStatusListener >& xControl, const URL& aURL )
{
}

void SAL_CALL CreatorAndTablesHandlingDispatchImpl::removeStatusListener( const Reference< XStatusListener >& xControl, const URL& aURL )
{
}



// ProtocolHandler implementation "Addon" class methods

void SAL_CALL Addon::initialize( const Sequence< Any >& aArguments )
{
    Reference < XFrame > xFrame;
    if ( aArguments.getLength() )
    {
        aArguments[0] >>= xFrame;
        mxFrame = xFrame;
    }
}

Reference< XDispatch > SAL_CALL Addon::queryDispatch( const URL& aURL, const ::rtl::OUString& sTargetFrameName, sal_Int32 nSearchFlags )
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
{
    sal_Int32 nCount = seqDescripts.getLength();
    Sequence < Reference < XDispatch > > lDispatcher( nCount );

    for( sal_Int32 i=0; i<nCount; ++i )
        lDispatcher[i] = queryDispatch( seqDescripts[i].FeatureURL, seqDescripts[i].FrameName, seqDescripts[i].SearchFlags );

    return lDispatcher;
}


// Helper functions for the implementation of UNO component interfaces.
OUString Addon_getImplementationName()
{
    return OUString ( IMPLEMENTATION_NAME );
}

Sequence< ::rtl::OUString > SAL_CALL Addon_getSupportedServiceNames()
{
    Sequence < ::rtl::OUString > aRet(1);
    ::rtl::OUString* pArray = aRet.getArray();
    pArray[0] =  OUString ( SERVICE_NAME );
    return aRet;
}

Reference< XInterface > SAL_CALL Addon_createInstance( const Reference< XComponentContext > & rContext)
{
    return (cppu::OWeakObject*) new Addon( rContext );
}

// Implementation of the recommended/mandatory interfaces of a UNO component.
// XServiceInfo
::rtl::OUString SAL_CALL Addon::getImplementationName()
{
    return Addon_getImplementationName();
}

sal_Bool SAL_CALL Addon::supportsService( const ::rtl::OUString& rServiceName )
{
    return cppu::supportsService(this, rServiceName);
}

Sequence< ::rtl::OUString > SAL_CALL Addon::getSupportedServiceNames(  )
{
    return Addon_getSupportedServiceNames();
}

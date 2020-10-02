#ifndef INCO_NIOCS_TEST_PROTOCOLHANDLEREXTENSION_ADDON_HXX
#define INCO_NIOCS_TEST_PROTOCOLHANDLEREXTENSION_ADDON_HXX

#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/frame/XDispatchProvider.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <cppuhelper/implbase3.hxx>
#include <cppuhelper/implbase1.hxx>

#define IMPLEMENTATION_NAME "inco.niocs.test.protocolhandler"

namespace com
{
    namespace sun
    {
        namespace star
        {
            namespace frame
            {
                class XFrame;
            }
            namespace uno
            {
                class XComponentContext;
            }
        }
    }
}


class CreatorAndTableProcessingDispatchImpl : public cppu::WeakImplHelper1<com::sun::star::frame::XDispatch>
{
private:
    ::com::sun::star::uno::Reference< ::com::sun::star::frame::XFrame > mxFrame;
public:
    CreatorAndTableProcessingDispatchImpl( const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XFrame > &rxFrame )
	: mxFrame( rxFrame )
    {
	   printf("DEBUG>>> Created CreatorAndTablesHandlingDispatchImpl object : %p\n", this); fflush(stdout);
    }

    // XDispatch
    virtual void SAL_CALL dispatch( const ::com::sun::star::util::URL& aURL,
        const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& lArgs );

    virtual void SAL_CALL addStatusListener( const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XStatusListener >& xControl,
        const ::com::sun::star::util::URL& aURL );

    virtual void SAL_CALL removeStatusListener( const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XStatusListener >& xControl,
        const ::com::sun::star::util::URL& aURL );
};


class Addon : public cppu::WeakImplHelper3
<
    com::sun::star::frame::XDispatchProvider,
    com::sun::star::lang::XInitialization,
    com::sun::star::lang::XServiceInfo
>
{
private:
    ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext > mxContext;
    ::com::sun::star::uno::Reference< ::com::sun::star::frame::XFrame > mxFrame;

public:
    Addon( const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext > &rxContext)
        : mxContext( rxContext )
    {
        printf("DEBUG>>> Created Addon object : %p\n", this); fflush(stdout);
    }

    // XDispatchProvider
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::frame::XDispatch >
            SAL_CALL queryDispatch( const ::com::sun::star::util::URL& aURL,
                const ::rtl::OUString& sTargetFrameName, sal_Int32 nSearchFlags );

    virtual ::com::sun::star::uno::Sequence < ::com::sun::star::uno::Reference< ::com::sun::star::frame::XDispatch > >
        SAL_CALL queryDispatches(
            const ::com::sun::star::uno::Sequence < ::com::sun::star::frame::DispatchDescriptor >& seqDescriptor );

    // XInitialization
    virtual void SAL_CALL initialize( const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any >& aArguments );

    // XServiceInfo
    virtual ::rtl::OUString SAL_CALL getImplementationName();

    virtual sal_Bool SAL_CALL supportsService( const ::rtl::OUString& ServiceName );

    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames();
};

::rtl::OUString Addon_getImplementationName();

sal_Bool SAL_CALL Addon_supportsService( const ::rtl::OUString& ServiceName );

::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL Addon_getSupportedServiceNames();

::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >
SAL_CALL Addon_createInstance( const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext > & rContext);


#endif

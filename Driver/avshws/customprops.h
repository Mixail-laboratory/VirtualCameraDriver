#pragma once


#define DEFINE_STD_PROPERTY_SET(SET, PROPERTIES)    \
    DEFINE_KSPROPERTY_SET(                          \
        &SET,                                       \
        SIZEOF_ARRAY(PROPERTIES),                   \
        PROPERTIES,                                 \
        0,                                          \
        NULL)

#define DEFINE_PROP_ITEM_WITH_VALUES( T, ctrl, type, name, values )     \
    DEFINE_KSPROPERTY_ITEM(                                             \
        ctrl,                                                           \
        (&T::Prop<type, T, &T::Get##name>),                             \
        sizeof(KSPROPERTY),                                             \
        0,                                                              \
        (&T::Prop<type, T, &T::Set##name>),                             \
        values, 0, NULL, NULL, 0                                        \
        )


#define DEFINE_PROP_ITEM( T, ctrl, type, name )                         \
    DEFINE_PROP_ITEM_WITH_VALUES( T, ctrl, type, name, NULL )


//  Declare a property's GET handler.
#define DECLARE_PROPERTY_GET_HANDLER( name )      \
    static NTSTATUS                                            \
    Get##name(                                          \
        _In_ PIRP Irp, _In_ PKSIDENTIFIER Request, _Inout_ PVOID Data  \
        );

//  Declare a property's SET handler.
#define DECLARE_PROPERTY_SET_HANDLER( name )      \
    static NTSTATUS                                            \
    Set##name(                                          \
        _In_ PIRP Irp, _In_ PKSIDENTIFIER Request, _Inout_ PVOID Data  \
        );

//  Declare a property's GET and SET handlers.
#define DECLARE_PROPERTY_HANDLERS( name )         \
DECLARE_PROPERTY_GET_HANDLER( name )              \
DECLARE_PROPERTY_SET_HANDLER( name )

// {CB043957-7B35-456E-9B61-5513930F4D8E}
#define STATIC_PROPSETID_VIDCAP_CUSTOMCONTROL 0xcb043957, 0x7b35, 0x456e, 0x9b, 0x61, 0x55, 0x13, 0x93, 0xf, 0x4d, 0x8e
DEFINE_GUIDSTRUCT("CB043957-7B35-456E-9B61-5513930F4D8E", PROPSETID_VIDCAP_CUSTOMCONTROL);
#define PROPSETID_VIDCAP_CUSTOMCONTROL DEFINE_GUIDNAMED(PROPSETID_VIDCAP_CUSTOMCONTROL )

// RGB32
#define STATIC_KSDATAFORMAT_SUBTYPE_RGB32\
    0xe436eb7e, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70
DEFINE_GUIDSTRUCT("e436eb7e-524f-11ce-9f53-0020af0ba770", KSDATAFORMAT_SUBTYPE_RGB32);
#define KSDATAFORMAT_SUBTYPE_RGB32 DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_RGB32)

enum
{
	KSPROPERTY_CUSTOMCONTROL_DUMMY
};
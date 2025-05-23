#pragma once

#include <windows.h>

#include <cassert>
#include <codecvt>
#include <locale>
#include <string>
#include <filesystem>
#include <utility>
#include <format>

#pragma warning(push)
#pragma warning(disable: 4'191)

// NOLINTBEGIN(*-redundant-void-arg, *-reserved-identifier, *-pro-type-reinterpret-cast,
// *-use-using, *-avoid-c-arrays, *-avoid-return-with-void-value)

static auto menuFramework{ GetModuleHandle(L"SKSEMenuFramework") };  // NOLINT(*-err58-cpp)
#define MENU_WINDOW SKSEMenuFramework::Model::WindowInterface*

namespace SKSEMenuFramework {

    inline auto IsInstalled() -> bool {
        constexpr auto dllPath{ "Data/SKSE/Plugins/SKSEMenuFramework.dll" };
        return std::filesystem::exists(dllPath);
    }

    namespace Model {
        class WindowInterface
        {
        public:
            std::atomic<bool> IsOpen{ false };
        };

        typedef void(__stdcall* RenderFunction)();
        using ActionFunction         = void (*)();
        using AddWindowFunction      = Model::WindowInterface* (*)(RenderFunction);
        using AddSectionItemFunction = void (*)(const char* path, RenderFunction rendererFunction);
    }  // namespace Model

    namespace Internal {

        template<class T>
        auto GetFunction(LPCSTR name) -> T {
            return reinterpret_cast<T>(GetProcAddress(menuFramework, name));
        }

        inline std::string key;
    }  // namespace Internal

    inline void AddSectionItem(const std::string_view menu, Model::RenderFunction rendererFunction) {
        static auto func = Internal::GetFunction<Model::AddSectionItemFunction>("AddSectionItem");
        if(func != nullptr) {
            const auto stringTemp{ std::format("{}/{}", Internal::key, menu) };
            return func(stringTemp.c_str(), rendererFunction);
        }
    }

    inline auto AddWindow(Model::RenderFunction rendererFunction) -> Model::WindowInterface* {
        static auto func = Internal::GetFunction<Model::AddWindowFunction>("AddWindow");
        if(func != nullptr) { return func(rendererFunction); }
        return nullptr;
    }

    inline void SetSection(std::string key) { Internal::key = std::move(key); }
}  // namespace SKSEMenuFramework

namespace FontAwesome {
    inline void PushSolid() {
        static auto func = SKSEMenuFramework::Internal::GetFunction<SKSEMenuFramework::Model::ActionFunction>("PushSolid");
        if(func != nullptr) { return func(); }
    }

    inline void PushRegular() {
        static auto func = SKSEMenuFramework::Internal::GetFunction<SKSEMenuFramework::Model::ActionFunction>("PushRegular");
        if(func != nullptr) { return func(); }
    }

    inline void PushBrands() {
        static auto func = SKSEMenuFramework::Internal::GetFunction<SKSEMenuFramework::Model::ActionFunction>("PushBrands");
        if(func != nullptr) { return func(); }
    }

    inline void Pop() {
        static auto func = SKSEMenuFramework::Internal::GetFunction<SKSEMenuFramework::Model::ActionFunction>("Pop");
        if(func != nullptr) { return func(); }
    }

    inline std::string UnicodeToUtf8(unsigned int codepoint) {
        // return std::format("{:c}", codepoint);
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        return converter.to_bytes(codepoint);
    }
}  // namespace FontAwesome

#pragma region Structs

typedef struct ImDrawChannel                ImDrawChannel;
typedef struct ImDrawCmd                    ImDrawCmd;
typedef struct ImDrawData                   ImDrawData;
typedef struct ImDrawList                   ImDrawList;
typedef struct ImDrawListSharedData         ImDrawListSharedData;
typedef struct ImDrawListSplitter           ImDrawListSplitter;
typedef struct ImDrawVert                   ImDrawVert;
typedef struct ImFont                       ImFont;
typedef struct ImFontAtlas                  ImFontAtlas;
typedef struct ImFontBuilderIO              ImFontBuilderIO;
typedef struct ImFontConfig                 ImFontConfig;
typedef struct ImFontGlyph                  ImFontGlyph;
typedef struct ImFontGlyphRangesBuilder     ImFontGlyphRangesBuilder;
typedef struct ImColor                      ImColor;
typedef struct ImGuiContext                 ImGuiContext;
typedef struct ImGuiIO                      ImGuiIO;
typedef struct ImGuiInputTextCallbackData   ImGuiInputTextCallbackData;
typedef struct ImGuiKeyData                 ImGuiKeyData;
typedef struct ImGuiListClipper             ImGuiListClipper;
typedef struct ImGuiOnceUponAFrame          ImGuiOnceUponAFrame;
typedef struct ImGuiPayload                 ImGuiPayload;
typedef struct ImGuiPlatformIO              ImGuiPlatformIO;
typedef struct ImGuiPlatformMonitor         ImGuiPlatformMonitor;
typedef struct ImGuiPlatformImeData         ImGuiPlatformImeData;
typedef struct ImGuiSizeCallbackData        ImGuiSizeCallbackData;
typedef struct ImGuiStorage                 ImGuiStorage;
typedef struct ImGuiStyle                   ImGuiStyle;
typedef struct ImGuiTableSortSpecs          ImGuiTableSortSpecs;
typedef struct ImGuiTableColumnSortSpecs    ImGuiTableColumnSortSpecs;
typedef struct ImGuiTextBuffer              ImGuiTextBuffer;
typedef class ImGuiTextFilter               ImGuiTextFilter;
typedef struct ImGuiViewport                ImGuiViewport;
typedef struct ImGuiWindowClass             ImGuiWindowClass;
typedef struct ImBitVector                  ImBitVector;
typedef struct ImRect                       ImRect;
typedef struct ImDrawDataBuilder            ImDrawDataBuilder;
typedef struct ImGuiColorMod                ImGuiColorMod;
typedef struct ImGuiContextHook             ImGuiContextHook;
typedef struct ImGuiDataVarInfo             ImGuiDataVarInfo;
typedef struct ImGuiDataTypeInfo            ImGuiDataTypeInfo;
typedef struct ImGuiDockContext             ImGuiDockContext;
typedef struct ImGuiDockRequest             ImGuiDockRequest;
typedef struct ImGuiDockNode                ImGuiDockNode;
typedef struct ImGuiDockNodeSettings        ImGuiDockNodeSettings;
typedef struct ImGuiGroupData               ImGuiGroupData;
typedef struct ImGuiInputTextState          ImGuiInputTextState;
typedef struct ImGuiInputTextDeactivateData ImGuiInputTextDeactivateData;
typedef struct ImGuiLastItemData            ImGuiLastItemData;
typedef struct ImGuiLocEntry                ImGuiLocEntry;
typedef struct ImGuiMenuColumns             ImGuiMenuColumns;
typedef struct ImGuiNavItemData             ImGuiNavItemData;
typedef struct ImGuiNavTreeNodeData         ImGuiNavTreeNodeData;
typedef struct ImGuiMetricsConfig           ImGuiMetricsConfig;
typedef struct ImGuiNextWindowData          ImGuiNextWindowData;
typedef struct ImGuiNextItemData            ImGuiNextItemData;
typedef struct ImGuiOldColumnData           ImGuiOldColumnData;
typedef struct ImGuiOldColumns              ImGuiOldColumns;
typedef struct ImGuiPopupData               ImGuiPopupData;
typedef struct ImGuiSettingsHandler         ImGuiSettingsHandler;
typedef struct ImGuiStackSizes              ImGuiStackSizes;
typedef struct ImGuiStyleMod                ImGuiStyleMod;
typedef struct ImGuiTabBar                  ImGuiTabBar;
typedef struct ImGuiTabItem                 ImGuiTabItem;
typedef struct ImGuiTable                   ImGuiTable;
typedef struct ImGuiTableHeaderData         ImGuiTableHeaderData;
typedef struct ImGuiTableColumn             ImGuiTableColumn;
typedef struct ImGuiTableInstanceData       ImGuiTableInstanceData;
typedef struct ImGuiTableTempData           ImGuiTableTempData;
typedef struct ImGuiTableSettings           ImGuiTableSettings;
typedef struct ImGuiTableColumnsSettings    ImGuiTableColumnsSettings;
typedef struct ImGuiTypingSelectState       ImGuiTypingSelectState;
typedef struct ImGuiTypingSelectRequest     ImGuiTypingSelectRequest;
typedef struct ImGuiWindow                  ImGuiWindow;
typedef struct ImGuiWindowDockStyle         ImGuiWindowDockStyle;
typedef struct ImGuiWindowTempData          ImGuiWindowTempData;
typedef struct ImGuiWindowSettings          ImGuiWindowSettings;

typedef struct ImVector_const_charPtr {
    int          Size;
    int          Capacity;
    const char** Data;
} ImVector_const_charPtr;

typedef unsigned int       ImGuiID;
typedef signed char        ImS8;
typedef unsigned char      ImU8;
typedef signed short       ImS16;
typedef unsigned short     ImU16;
typedef signed int         ImS32;
typedef unsigned int       ImU32;
typedef signed long long   ImS64;
typedef unsigned long long ImU64;
struct ImDrawChannel;
struct ImDrawCmd;
struct ImDrawData;
struct ImDrawList;
struct ImDrawListSharedData;
struct ImDrawListSplitter;
struct ImDrawVert;
struct ImFont;
struct ImFontAtlas;
struct ImFontBuilderIO;
struct ImFontConfig;
struct ImFontGlyph;
struct ImFontGlyphRangesBuilder;
struct ImColor;
struct ImGuiContext;
struct ImGuiIO;
struct ImGuiInputTextCallbackData;
struct ImGuiKeyData;
struct ImGuiListClipper;
struct ImGuiOnceUponAFrame;
struct ImGuiPayload;
struct ImGuiPlatformIO;
struct ImGuiPlatformMonitor;
struct ImGuiPlatformImeData;
struct ImGuiSizeCallbackData;
struct ImGuiStorage;
struct ImGuiStyle;
struct ImGuiTableSortSpecs;
struct ImGuiTableColumnSortSpecs;
struct ImGuiTextBuffer;
class ImGuiTextFilter;
struct ImGuiViewport;
struct ImGuiWindowClass;
typedef int            ImGuiCol;
typedef int            ImGuiCond;
typedef int            ImGuiDataType;
typedef int            ImGuiMouseButton;
typedef int            ImGuiMouseCursor;
typedef int            ImGuiStyleVar;
typedef int            ImGuiTableBgTarget;
typedef int            ImDrawFlags;
typedef int            ImDrawListFlags;
typedef int            ImFontAtlasFlags;
typedef int            ImGuiBackendFlags;
typedef int            ImGuiButtonFlags;
typedef int            ImGuiChildFlags;
typedef int            ImGuiColorEditFlags;
typedef int            ImGuiConfigFlags;
typedef int            ImGuiComboFlags;
typedef int            ImGuiDockNodeFlags;
typedef int            ImGuiDragDropFlags;
typedef int            ImGuiFocusedFlags;
typedef int            ImGuiHoveredFlags;
typedef int            ImGuiInputFlags;
typedef int            ImGuiInputTextFlags;
typedef int            ImGuiKeyChord;
typedef int            ImGuiPopupFlags;
typedef int            ImGuiSelectableFlags;
typedef int            ImGuiSliderFlags;
typedef int            ImGuiTabBarFlags;
typedef int            ImGuiTabItemFlags;
typedef int            ImGuiTableFlags;
typedef int            ImGuiTableColumnFlags;
typedef int            ImGuiTableRowFlags;
typedef int            ImGuiTreeNodeFlags;
typedef int            ImGuiViewportFlags;
typedef int            ImGuiWindowFlags;
typedef void*          ImTextureID;
typedef unsigned short ImDrawIdx;
typedef unsigned int   ImWchar32;
typedef unsigned short ImWchar16;
typedef ImWchar16      ImWchar;
typedef int (*ImGuiInputTextCallback)(ImGuiInputTextCallbackData* data);
typedef void (*ImGuiSizeCallback)(ImGuiSizeCallbackData* data);
typedef void* (*ImGuiMemAllocFunc)(size_t sz, void* user_data);
typedef void (*ImGuiMemFreeFunc)(void* ptr, void* user_data);
typedef struct ImVec2 ImVec2;

struct ImVec2 {
    float x, y;
};
typedef struct ImVec4 ImVec4;

struct ImVec4 {
    float x, y, z, w;
};

typedef enum {
    ImGuiWindowFlags_None                      = 0,
    ImGuiWindowFlags_NoTitleBar                = 1 << 0,
    ImGuiWindowFlags_NoResize                  = 1 << 1,
    ImGuiWindowFlags_NoMove                    = 1 << 2,
    ImGuiWindowFlags_NoScrollbar               = 1 << 3,
    ImGuiWindowFlags_NoScrollWithMouse         = 1 << 4,
    ImGuiWindowFlags_NoCollapse                = 1 << 5,
    ImGuiWindowFlags_AlwaysAutoResize          = 1 << 6,
    ImGuiWindowFlags_NoBackground              = 1 << 7,
    ImGuiWindowFlags_NoSavedSettings           = 1 << 8,
    ImGuiWindowFlags_NoMouseInputs             = 1 << 9,
    ImGuiWindowFlags_MenuBar                   = 1 << 10,
    ImGuiWindowFlags_HorizontalScrollbar       = 1 << 11,
    ImGuiWindowFlags_NoFocusOnAppearing        = 1 << 12,
    ImGuiWindowFlags_NoBringToFrontOnFocus     = 1 << 13,
    ImGuiWindowFlags_AlwaysVerticalScrollbar   = 1 << 14,
    ImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,
    ImGuiWindowFlags_NoNavInputs               = 1 << 16,
    ImGuiWindowFlags_NoNavFocus                = 1 << 17,
    ImGuiWindowFlags_UnsavedDocument           = 1 << 18,
    ImGuiWindowFlags_NoDocking                 = 1 << 19,
    ImGuiWindowFlags_NoNav = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
    ImGuiWindowFlags_NoDecoration = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
    ImGuiWindowFlags_NoInputs = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
    ImGuiWindowFlags_NavFlattened = 1 << 23,
    ImGuiWindowFlags_ChildWindow  = 1 << 24,
    ImGuiWindowFlags_Tooltip      = 1 << 25,
    ImGuiWindowFlags_Popup        = 1 << 26,
    ImGuiWindowFlags_Modal        = 1 << 27,
    ImGuiWindowFlags_ChildMenu    = 1 << 28,
    ImGuiWindowFlags_DockNodeHost = 1 << 29,
} ImGuiWindowFlags_;

typedef enum {
    ImGuiChildFlags_None                   = 0,
    ImGuiChildFlags_Border                 = 1 << 0,
    ImGuiChildFlags_AlwaysUseWindowPadding = 1 << 1,
    ImGuiChildFlags_ResizeX                = 1 << 2,
    ImGuiChildFlags_ResizeY                = 1 << 3,
    ImGuiChildFlags_AutoResizeX            = 1 << 4,
    ImGuiChildFlags_AutoResizeY            = 1 << 5,
    ImGuiChildFlags_AlwaysAutoResize       = 1 << 6,
    ImGuiChildFlags_FrameStyle             = 1 << 7,
} ImGuiChildFlags_;

typedef enum {
    ImGuiInputTextFlags_None                = 0,
    ImGuiInputTextFlags_CharsDecimal        = 1 << 0,
    ImGuiInputTextFlags_CharsHexadecimal    = 1 << 1,
    ImGuiInputTextFlags_CharsScientific     = 1 << 2,
    ImGuiInputTextFlags_CharsUppercase      = 1 << 3,
    ImGuiInputTextFlags_CharsNoBlank        = 1 << 4,
    ImGuiInputTextFlags_AllowTabInput       = 1 << 5,
    ImGuiInputTextFlags_EnterReturnsTrue    = 1 << 6,
    ImGuiInputTextFlags_EscapeClearsAll     = 1 << 7,
    ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 8,
    ImGuiInputTextFlags_ReadOnly            = 1 << 9,
    ImGuiInputTextFlags_Password            = 1 << 10,
    ImGuiInputTextFlags_AlwaysOverwrite     = 1 << 11,
    ImGuiInputTextFlags_AutoSelectAll       = 1 << 12,
    ImGuiInputTextFlags_ParseEmptyRefVal    = 1 << 13,
    ImGuiInputTextFlags_DisplayEmptyRefVal  = 1 << 14,
    ImGuiInputTextFlags_NoHorizontalScroll  = 1 << 15,
    ImGuiInputTextFlags_NoUndoRedo          = 1 << 16,
    ImGuiInputTextFlags_CallbackCompletion  = 1 << 17,
    ImGuiInputTextFlags_CallbackHistory     = 1 << 18,
    ImGuiInputTextFlags_CallbackAlways      = 1 << 19,
    ImGuiInputTextFlags_CallbackCharFilter  = 1 << 20,
    ImGuiInputTextFlags_CallbackResize      = 1 << 21,
    ImGuiInputTextFlags_CallbackEdit        = 1 << 22,
} ImGuiInputTextFlags_;

typedef enum {
    ImGuiTreeNodeFlags_None                 = 0,
    ImGuiTreeNodeFlags_Selected             = 1 << 0,
    ImGuiTreeNodeFlags_Framed               = 1 << 1,
    ImGuiTreeNodeFlags_AllowOverlap         = 1 << 2,
    ImGuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,
    ImGuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,
    ImGuiTreeNodeFlags_DefaultOpen          = 1 << 5,
    ImGuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,
    ImGuiTreeNodeFlags_OpenOnArrow          = 1 << 7,
    ImGuiTreeNodeFlags_Leaf                 = 1 << 8,
    ImGuiTreeNodeFlags_Bullet               = 1 << 9,
    ImGuiTreeNodeFlags_FramePadding         = 1 << 10,
    ImGuiTreeNodeFlags_SpanAvailWidth       = 1 << 11,
    ImGuiTreeNodeFlags_SpanFullWidth        = 1 << 12,
    ImGuiTreeNodeFlags_SpanTextWidth        = 1 << 13,
    ImGuiTreeNodeFlags_SpanAllColumns       = 1 << 14,
    ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 15,
    ImGuiTreeNodeFlags_CollapsingHeader = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog,
} ImGuiTreeNodeFlags_;

typedef enum {
    ImGuiPopupFlags_None                    = 0,
    ImGuiPopupFlags_MouseButtonLeft         = 0,
    ImGuiPopupFlags_MouseButtonRight        = 1,
    ImGuiPopupFlags_MouseButtonMiddle       = 2,
    ImGuiPopupFlags_MouseButtonMask_        = 0x1F,
    ImGuiPopupFlags_MouseButtonDefault_     = 1,
    ImGuiPopupFlags_NoReopen                = 1 << 5,
    ImGuiPopupFlags_NoOpenOverExistingPopup = 1 << 7,
    ImGuiPopupFlags_NoOpenOverItems         = 1 << 8,
    ImGuiPopupFlags_AnyPopupId              = 1 << 10,
    ImGuiPopupFlags_AnyPopupLevel           = 1 << 11,
    ImGuiPopupFlags_AnyPopup = ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel,
} ImGuiPopupFlags_;

typedef enum {
    ImGuiSelectableFlags_None             = 0,
    ImGuiSelectableFlags_DontClosePopups  = 1 << 0,
    ImGuiSelectableFlags_SpanAllColumns   = 1 << 1,
    ImGuiSelectableFlags_AllowDoubleClick = 1 << 2,
    ImGuiSelectableFlags_Disabled         = 1 << 3,
    ImGuiSelectableFlags_AllowOverlap     = 1 << 4,
} ImGuiSelectableFlags_;

typedef enum {
    ImGuiComboFlags_None            = 0,
    ImGuiComboFlags_PopupAlignLeft  = 1 << 0,
    ImGuiComboFlags_HeightSmall     = 1 << 1,
    ImGuiComboFlags_HeightRegular   = 1 << 2,
    ImGuiComboFlags_HeightLarge     = 1 << 3,
    ImGuiComboFlags_HeightLargest   = 1 << 4,
    ImGuiComboFlags_NoArrowButton   = 1 << 5,
    ImGuiComboFlags_NoPreview       = 1 << 6,
    ImGuiComboFlags_WidthFitPreview = 1 << 7,
    ImGuiComboFlags_HeightMask_ = ImGuiComboFlags_HeightSmall | ImGuiComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_HeightLargest,
} ImGuiComboFlags_;

typedef enum {
    ImGuiTabBarFlags_None                         = 0,
    ImGuiTabBarFlags_Reorderable                  = 1 << 0,
    ImGuiTabBarFlags_AutoSelectNewTabs            = 1 << 1,
    ImGuiTabBarFlags_TabListPopupButton           = 1 << 2,
    ImGuiTabBarFlags_NoCloseWithMiddleMouseButton = 1 << 3,
    ImGuiTabBarFlags_NoTabListScrollingButtons    = 1 << 4,
    ImGuiTabBarFlags_NoTooltip                    = 1 << 5,
    ImGuiTabBarFlags_FittingPolicyResizeDown      = 1 << 6,
    ImGuiTabBarFlags_FittingPolicyScroll          = 1 << 7,
    ImGuiTabBarFlags_FittingPolicyMask_ = ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_FittingPolicyScroll,
    ImGuiTabBarFlags_FittingPolicyDefault_ = ImGuiTabBarFlags_FittingPolicyResizeDown,
} ImGuiTabBarFlags_;

typedef enum {
    ImGuiTabItemFlags_None                         = 0,
    ImGuiTabItemFlags_UnsavedDocument              = 1 << 0,
    ImGuiTabItemFlags_SetSelected                  = 1 << 1,
    ImGuiTabItemFlags_NoCloseWithMiddleMouseButton = 1 << 2,
    ImGuiTabItemFlags_NoPushId                     = 1 << 3,
    ImGuiTabItemFlags_NoTooltip                    = 1 << 4,
    ImGuiTabItemFlags_NoReorder                    = 1 << 5,
    ImGuiTabItemFlags_Leading                      = 1 << 6,
    ImGuiTabItemFlags_Trailing                     = 1 << 7,
    ImGuiTabItemFlags_NoAssumedClosure             = 1 << 8,
} ImGuiTabItemFlags_;

typedef enum {
    ImGuiFocusedFlags_None             = 0,
    ImGuiFocusedFlags_ChildWindows     = 1 << 0,
    ImGuiFocusedFlags_RootWindow       = 1 << 1,
    ImGuiFocusedFlags_AnyWindow        = 1 << 2,
    ImGuiFocusedFlags_NoPopupHierarchy = 1 << 3,
    ImGuiFocusedFlags_DockHierarchy    = 1 << 4,
    ImGuiFocusedFlags_RootAndChildWindows = ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows,
} ImGuiFocusedFlags_;

typedef enum {
    ImGuiHoveredFlags_None                         = 0,
    ImGuiHoveredFlags_ChildWindows                 = 1 << 0,
    ImGuiHoveredFlags_RootWindow                   = 1 << 1,
    ImGuiHoveredFlags_AnyWindow                    = 1 << 2,
    ImGuiHoveredFlags_NoPopupHierarchy             = 1 << 3,
    ImGuiHoveredFlags_DockHierarchy                = 1 << 4,
    ImGuiHoveredFlags_AllowWhenBlockedByPopup      = 1 << 5,
    ImGuiHoveredFlags_AllowWhenBlockedByActiveItem = 1 << 7,
    ImGuiHoveredFlags_AllowWhenOverlappedByItem    = 1 << 8,
    ImGuiHoveredFlags_AllowWhenOverlappedByWindow  = 1 << 9,
    ImGuiHoveredFlags_AllowWhenDisabled            = 1 << 10,
    ImGuiHoveredFlags_NoNavOverride                = 1 << 11,
    ImGuiHoveredFlags_AllowWhenOverlapped = ImGuiHoveredFlags_AllowWhenOverlappedByItem | ImGuiHoveredFlags_AllowWhenOverlappedByWindow,
    ImGuiHoveredFlags_RectOnly = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped,
    ImGuiHoveredFlags_RootAndChildWindows = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows,
    ImGuiHoveredFlags_ForTooltip    = 1 << 12,
    ImGuiHoveredFlags_Stationary    = 1 << 13,
    ImGuiHoveredFlags_DelayNone     = 1 << 14,
    ImGuiHoveredFlags_DelayShort    = 1 << 15,
    ImGuiHoveredFlags_DelayNormal   = 1 << 16,
    ImGuiHoveredFlags_NoSharedDelay = 1 << 17,
} ImGuiHoveredFlags_;

typedef enum {
    ImGuiDockNodeFlags_None                     = 0,
    ImGuiDockNodeFlags_KeepAliveOnly            = 1 << 0,
    ImGuiDockNodeFlags_NoDockingOverCentralNode = 1 << 2,
    ImGuiDockNodeFlags_PassthruCentralNode      = 1 << 3,
    ImGuiDockNodeFlags_NoDockingSplit           = 1 << 4,
    ImGuiDockNodeFlags_NoResize                 = 1 << 5,
    ImGuiDockNodeFlags_AutoHideTabBar           = 1 << 6,
    ImGuiDockNodeFlags_NoUndocking              = 1 << 7,
} ImGuiDockNodeFlags_;

typedef enum {
    ImGuiDragDropFlags_None                     = 0,
    ImGuiDragDropFlags_SourceNoPreviewTooltip   = 1 << 0,
    ImGuiDragDropFlags_SourceNoDisableHover     = 1 << 1,
    ImGuiDragDropFlags_SourceNoHoldToOpenOthers = 1 << 2,
    ImGuiDragDropFlags_SourceAllowNullID        = 1 << 3,
    ImGuiDragDropFlags_SourceExtern             = 1 << 4,
    ImGuiDragDropFlags_SourceAutoExpirePayload  = 1 << 5,
    ImGuiDragDropFlags_AcceptBeforeDelivery     = 1 << 10,
    ImGuiDragDropFlags_AcceptNoDrawDefaultRect  = 1 << 11,
    ImGuiDragDropFlags_AcceptNoPreviewTooltip   = 1 << 12,
    ImGuiDragDropFlags_AcceptPeekOnly = ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect,
} ImGuiDragDropFlags_;

typedef enum {
    ImGuiDataType_S8,
    ImGuiDataType_U8,
    ImGuiDataType_S16,
    ImGuiDataType_U16,
    ImGuiDataType_S32,
    ImGuiDataType_U32,
    ImGuiDataType_S64,
    ImGuiDataType_U64,
    ImGuiDataType_Float,
    ImGuiDataType_Double,
    ImGuiDataType_COUNT
} ImGuiDataType_;

typedef enum {
    ImGuiDir_None  = -1,
    ImGuiDir_Left  = 0,
    ImGuiDir_Right = 1,
    ImGuiDir_Up    = 2,
    ImGuiDir_Down  = 3,
    ImGuiDir_COUNT = 4,
} ImGuiDir;

typedef enum {
    ImGuiSortDirection_None       = 0,
    ImGuiSortDirection_Ascending  = 1,
    ImGuiSortDirection_Descending = 2,
} ImGuiSortDirection;

typedef enum {
    ImGuiKey_None                = 0,
    ImGuiKey_Tab                 = 512,
    ImGuiKey_LeftArrow           = 513,
    ImGuiKey_RightArrow          = 514,
    ImGuiKey_UpArrow             = 515,
    ImGuiKey_DownArrow           = 516,
    ImGuiKey_PageUp              = 517,
    ImGuiKey_PageDown            = 518,
    ImGuiKey_Home                = 519,
    ImGuiKey_End                 = 520,
    ImGuiKey_Insert              = 521,
    ImGuiKey_Delete              = 522,
    ImGuiKey_Backspace           = 523,
    ImGuiKey_Space               = 524,
    ImGuiKey_Enter               = 525,
    ImGuiKey_Escape              = 526,
    ImGuiKey_LeftCtrl            = 527,
    ImGuiKey_LeftShift           = 528,
    ImGuiKey_LeftAlt             = 529,
    ImGuiKey_LeftSuper           = 530,
    ImGuiKey_RightCtrl           = 531,
    ImGuiKey_RightShift          = 532,
    ImGuiKey_RightAlt            = 533,
    ImGuiKey_RightSuper          = 534,
    ImGuiKey_Menu                = 535,
    ImGuiKey_0                   = 536,
    ImGuiKey_1                   = 537,
    ImGuiKey_2                   = 538,
    ImGuiKey_3                   = 539,
    ImGuiKey_4                   = 540,
    ImGuiKey_5                   = 541,
    ImGuiKey_6                   = 542,
    ImGuiKey_7                   = 543,
    ImGuiKey_8                   = 544,
    ImGuiKey_9                   = 545,
    ImGuiKey_A                   = 546,
    ImGuiKey_B                   = 547,
    ImGuiKey_C                   = 548,
    ImGuiKey_D                   = 549,
    ImGuiKey_E                   = 550,
    ImGuiKey_F                   = 551,
    ImGuiKey_G                   = 552,
    ImGuiKey_H                   = 553,
    ImGuiKey_I                   = 554,
    ImGuiKey_J                   = 555,
    ImGuiKey_K                   = 556,
    ImGuiKey_L                   = 557,
    ImGuiKey_M                   = 558,
    ImGuiKey_N                   = 559,
    ImGuiKey_O                   = 560,
    ImGuiKey_P                   = 561,
    ImGuiKey_Q                   = 562,
    ImGuiKey_R                   = 563,
    ImGuiKey_S                   = 564,
    ImGuiKey_T                   = 565,
    ImGuiKey_U                   = 566,
    ImGuiKey_V                   = 567,
    ImGuiKey_W                   = 568,
    ImGuiKey_X                   = 569,
    ImGuiKey_Y                   = 570,
    ImGuiKey_Z                   = 571,
    ImGuiKey_F1                  = 572,
    ImGuiKey_F2                  = 573,
    ImGuiKey_F3                  = 574,
    ImGuiKey_F4                  = 575,
    ImGuiKey_F5                  = 576,
    ImGuiKey_F6                  = 577,
    ImGuiKey_F7                  = 578,
    ImGuiKey_F8                  = 579,
    ImGuiKey_F9                  = 580,
    ImGuiKey_F10                 = 581,
    ImGuiKey_F11                 = 582,
    ImGuiKey_F12                 = 583,
    ImGuiKey_F13                 = 584,
    ImGuiKey_F14                 = 585,
    ImGuiKey_F15                 = 586,
    ImGuiKey_F16                 = 587,
    ImGuiKey_F17                 = 588,
    ImGuiKey_F18                 = 589,
    ImGuiKey_F19                 = 590,
    ImGuiKey_F20                 = 591,
    ImGuiKey_F21                 = 592,
    ImGuiKey_F22                 = 593,
    ImGuiKey_F23                 = 594,
    ImGuiKey_F24                 = 595,
    ImGuiKey_Apostrophe          = 596,
    ImGuiKey_Comma               = 597,
    ImGuiKey_Minus               = 598,
    ImGuiKey_Period              = 599,
    ImGuiKey_Slash               = 600,
    ImGuiKey_Semicolon           = 601,
    ImGuiKey_Equal               = 602,
    ImGuiKey_LeftBracket         = 603,
    ImGuiKey_Backslash           = 604,
    ImGuiKey_RightBracket        = 605,
    ImGuiKey_GraveAccent         = 606,
    ImGuiKey_CapsLock            = 607,
    ImGuiKey_ScrollLock          = 608,
    ImGuiKey_NumLock             = 609,
    ImGuiKey_PrintScreen         = 610,
    ImGuiKey_Pause               = 611,
    ImGuiKey_Keypad0             = 612,
    ImGuiKey_Keypad1             = 613,
    ImGuiKey_Keypad2             = 614,
    ImGuiKey_Keypad3             = 615,
    ImGuiKey_Keypad4             = 616,
    ImGuiKey_Keypad5             = 617,
    ImGuiKey_Keypad6             = 618,
    ImGuiKey_Keypad7             = 619,
    ImGuiKey_Keypad8             = 620,
    ImGuiKey_Keypad9             = 621,
    ImGuiKey_KeypadDecimal       = 622,
    ImGuiKey_KeypadDivide        = 623,
    ImGuiKey_KeypadMultiply      = 624,
    ImGuiKey_KeypadSubtract      = 625,
    ImGuiKey_KeypadAdd           = 626,
    ImGuiKey_KeypadEnter         = 627,
    ImGuiKey_KeypadEqual         = 628,
    ImGuiKey_AppBack             = 629,
    ImGuiKey_AppForward          = 630,
    ImGuiKey_GamepadStart        = 631,
    ImGuiKey_GamepadBack         = 632,
    ImGuiKey_GamepadFaceLeft     = 633,
    ImGuiKey_GamepadFaceRight    = 634,
    ImGuiKey_GamepadFaceUp       = 635,
    ImGuiKey_GamepadFaceDown     = 636,
    ImGuiKey_GamepadDpadLeft     = 637,
    ImGuiKey_GamepadDpadRight    = 638,
    ImGuiKey_GamepadDpadUp       = 639,
    ImGuiKey_GamepadDpadDown     = 640,
    ImGuiKey_GamepadL1           = 641,
    ImGuiKey_GamepadR1           = 642,
    ImGuiKey_GamepadL2           = 643,
    ImGuiKey_GamepadR2           = 644,
    ImGuiKey_GamepadL3           = 645,
    ImGuiKey_GamepadR3           = 646,
    ImGuiKey_GamepadLStickLeft   = 647,
    ImGuiKey_GamepadLStickRight  = 648,
    ImGuiKey_GamepadLStickUp     = 649,
    ImGuiKey_GamepadLStickDown   = 650,
    ImGuiKey_GamepadRStickLeft   = 651,
    ImGuiKey_GamepadRStickRight  = 652,
    ImGuiKey_GamepadRStickUp     = 653,
    ImGuiKey_GamepadRStickDown   = 654,
    ImGuiKey_MouseLeft           = 655,
    ImGuiKey_MouseRight          = 656,
    ImGuiKey_MouseMiddle         = 657,
    ImGuiKey_MouseX1             = 658,
    ImGuiKey_MouseX2             = 659,
    ImGuiKey_MouseWheelX         = 660,
    ImGuiKey_MouseWheelY         = 661,
    ImGuiKey_ReservedForModCtrl  = 662,
    ImGuiKey_ReservedForModShift = 663,
    ImGuiKey_ReservedForModAlt   = 664,
    ImGuiKey_ReservedForModSuper = 665,
    ImGuiKey_COUNT               = 666,
    ImGuiMod_None                = 0,
    ImGuiMod_Ctrl                = 1 << 12,
    ImGuiMod_Shift               = 1 << 13,
    ImGuiMod_Alt                 = 1 << 14,
    ImGuiMod_Super               = 1 << 15,
    ImGuiMod_Mask_               = 0xF0'00,
    ImGuiKey_NamedKey_BEGIN      = 512,
    ImGuiKey_NamedKey_END        = ImGuiKey_COUNT,
    ImGuiKey_NamedKey_COUNT      = ImGuiKey_NamedKey_END - ImGuiKey_NamedKey_BEGIN,
    ImGuiKey_KeysData_SIZE       = ImGuiKey_NamedKey_COUNT,
    ImGuiKey_KeysData_OFFSET     = ImGuiKey_NamedKey_BEGIN,
} ImGuiKey;

typedef enum {
    ImGuiInputFlags_None                 = 0,
    ImGuiInputFlags_Repeat               = 1 << 0,
    ImGuiInputFlags_RouteActive          = 1 << 10,
    ImGuiInputFlags_RouteFocused         = 1 << 11,
    ImGuiInputFlags_RouteGlobal          = 1 << 12,
    ImGuiInputFlags_RouteAlways          = 1 << 13,
    ImGuiInputFlags_RouteOverFocused     = 1 << 14,
    ImGuiInputFlags_RouteOverActive      = 1 << 15,
    ImGuiInputFlags_RouteUnlessBgFocused = 1 << 16,
    ImGuiInputFlags_RouteFromRootWindow  = 1 << 17,
    ImGuiInputFlags_Tooltip              = 1 << 18,
} ImGuiInputFlags_;

typedef enum {
    ImGuiConfigFlags_None                    = 0,
    ImGuiConfigFlags_NavEnableKeyboard       = 1 << 0,
    ImGuiConfigFlags_NavEnableGamepad        = 1 << 1,
    ImGuiConfigFlags_NavEnableSetMousePos    = 1 << 2,
    ImGuiConfigFlags_NavNoCaptureKeyboard    = 1 << 3,
    ImGuiConfigFlags_NoMouse                 = 1 << 4,
    ImGuiConfigFlags_NoMouseCursorChange     = 1 << 5,
    ImGuiConfigFlags_DockingEnable           = 1 << 6,
    ImGuiConfigFlags_ViewportsEnable         = 1 << 10,
    ImGuiConfigFlags_DpiEnableScaleViewports = 1 << 14,
    ImGuiConfigFlags_DpiEnableScaleFonts     = 1 << 15,
    ImGuiConfigFlags_IsSRGB                  = 1 << 20,
    ImGuiConfigFlags_IsTouchScreen           = 1 << 21,
} ImGuiConfigFlags_;

typedef enum {
    ImGuiBackendFlags_None                    = 0,
    ImGuiBackendFlags_HasGamepad              = 1 << 0,
    ImGuiBackendFlags_HasMouseCursors         = 1 << 1,
    ImGuiBackendFlags_HasSetMousePos          = 1 << 2,
    ImGuiBackendFlags_RendererHasVtxOffset    = 1 << 3,
    ImGuiBackendFlags_PlatformHasViewports    = 1 << 10,
    ImGuiBackendFlags_HasMouseHoveredViewport = 1 << 11,
    ImGuiBackendFlags_RendererHasViewports    = 1 << 12,
} ImGuiBackendFlags_;

typedef enum {
    ImGuiCol_Text,
    ImGuiCol_TextDisabled,
    ImGuiCol_WindowBg,
    ImGuiCol_ChildBg,
    ImGuiCol_PopupBg,
    ImGuiCol_Border,
    ImGuiCol_BorderShadow,
    ImGuiCol_FrameBg,
    ImGuiCol_FrameBgHovered,
    ImGuiCol_FrameBgActive,
    ImGuiCol_TitleBg,
    ImGuiCol_TitleBgActive,
    ImGuiCol_TitleBgCollapsed,
    ImGuiCol_MenuBarBg,
    ImGuiCol_ScrollbarBg,
    ImGuiCol_ScrollbarGrab,
    ImGuiCol_ScrollbarGrabHovered,
    ImGuiCol_ScrollbarGrabActive,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_Header,
    ImGuiCol_HeaderHovered,
    ImGuiCol_HeaderActive,
    ImGuiCol_Separator,
    ImGuiCol_SeparatorHovered,
    ImGuiCol_SeparatorActive,
    ImGuiCol_ResizeGrip,
    ImGuiCol_ResizeGripHovered,
    ImGuiCol_ResizeGripActive,
    ImGuiCol_Tab,
    ImGuiCol_TabHovered,
    ImGuiCol_TabActive,
    ImGuiCol_TabUnfocused,
    ImGuiCol_TabUnfocusedActive,
    ImGuiCol_DockingPreview,
    ImGuiCol_DockingEmptyBg,
    ImGuiCol_PlotLines,
    ImGuiCol_PlotLinesHovered,
    ImGuiCol_PlotHistogram,
    ImGuiCol_PlotHistogramHovered,
    ImGuiCol_TableHeaderBg,
    ImGuiCol_TableBorderStrong,
    ImGuiCol_TableBorderLight,
    ImGuiCol_TableRowBg,
    ImGuiCol_TableRowBgAlt,
    ImGuiCol_TextSelectedBg,
    ImGuiCol_DragDropTarget,
    ImGuiCol_NavHighlight,
    ImGuiCol_NavWindowingHighlight,
    ImGuiCol_NavWindowingDimBg,
    ImGuiCol_ModalWindowDimBg,
    ImGuiCol_COUNT
} ImGuiCol_;

typedef enum {
    ImGuiStyleVar_Alpha,
    ImGuiStyleVar_DisabledAlpha,
    ImGuiStyleVar_WindowPadding,
    ImGuiStyleVar_WindowRounding,
    ImGuiStyleVar_WindowBorderSize,
    ImGuiStyleVar_WindowMinSize,
    ImGuiStyleVar_WindowTitleAlign,
    ImGuiStyleVar_ChildRounding,
    ImGuiStyleVar_ChildBorderSize,
    ImGuiStyleVar_PopupRounding,
    ImGuiStyleVar_PopupBorderSize,
    ImGuiStyleVar_FramePadding,
    ImGuiStyleVar_FrameRounding,
    ImGuiStyleVar_FrameBorderSize,
    ImGuiStyleVar_ItemSpacing,
    ImGuiStyleVar_ItemInnerSpacing,
    ImGuiStyleVar_IndentSpacing,
    ImGuiStyleVar_CellPadding,
    ImGuiStyleVar_ScrollbarSize,
    ImGuiStyleVar_ScrollbarRounding,
    ImGuiStyleVar_GrabMinSize,
    ImGuiStyleVar_GrabRounding,
    ImGuiStyleVar_TabRounding,
    ImGuiStyleVar_TabBorderSize,
    ImGuiStyleVar_TabBarBorderSize,
    ImGuiStyleVar_TableAngledHeadersAngle,
    ImGuiStyleVar_TableAngledHeadersTextAlign,
    ImGuiStyleVar_ButtonTextAlign,
    ImGuiStyleVar_SelectableTextAlign,
    ImGuiStyleVar_SeparatorTextBorderSize,
    ImGuiStyleVar_SeparatorTextAlign,
    ImGuiStyleVar_SeparatorTextPadding,
    ImGuiStyleVar_DockingSeparatorSize,
    ImGuiStyleVar_COUNT
} ImGuiStyleVar_;

typedef enum {
    ImGuiButtonFlags_None              = 0,
    ImGuiButtonFlags_MouseButtonLeft   = 1 << 0,
    ImGuiButtonFlags_MouseButtonRight  = 1 << 1,
    ImGuiButtonFlags_MouseButtonMiddle = 1 << 2,
    ImGuiButtonFlags_MouseButtonMask_ = ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle,
} ImGuiButtonFlags_;

typedef enum {
    ImGuiColorEditFlags_None             = 0,
    ImGuiColorEditFlags_NoAlpha          = 1 << 1,
    ImGuiColorEditFlags_NoPicker         = 1 << 2,
    ImGuiColorEditFlags_NoOptions        = 1 << 3,
    ImGuiColorEditFlags_NoSmallPreview   = 1 << 4,
    ImGuiColorEditFlags_NoInputs         = 1 << 5,
    ImGuiColorEditFlags_NoTooltip        = 1 << 6,
    ImGuiColorEditFlags_NoLabel          = 1 << 7,
    ImGuiColorEditFlags_NoSidePreview    = 1 << 8,
    ImGuiColorEditFlags_NoDragDrop       = 1 << 9,
    ImGuiColorEditFlags_NoBorder         = 1 << 10,
    ImGuiColorEditFlags_AlphaBar         = 1 << 16,
    ImGuiColorEditFlags_AlphaPreview     = 1 << 17,
    ImGuiColorEditFlags_AlphaPreviewHalf = 1 << 18,
    ImGuiColorEditFlags_HDR              = 1 << 19,
    ImGuiColorEditFlags_DisplayRGB       = 1 << 20,
    ImGuiColorEditFlags_DisplayHSV       = 1 << 21,
    ImGuiColorEditFlags_DisplayHex       = 1 << 22,
    ImGuiColorEditFlags_Uint8            = 1 << 23,
    ImGuiColorEditFlags_Float            = 1 << 24,
    ImGuiColorEditFlags_PickerHueBar     = 1 << 25,
    ImGuiColorEditFlags_PickerHueWheel   = 1 << 26,
    ImGuiColorEditFlags_InputRGB         = 1 << 27,
    ImGuiColorEditFlags_InputHSV         = 1 << 28,
    ImGuiColorEditFlags_DefaultOptions_ = ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_PickerHueBar,
    ImGuiColorEditFlags_DisplayMask_ = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex,
    ImGuiColorEditFlags_DataTypeMask_ = ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_Float,
    ImGuiColorEditFlags_PickerMask_ = ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_PickerHueBar,
    ImGuiColorEditFlags_InputMask_ = ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_InputHSV,
} ImGuiColorEditFlags_;

typedef enum {
    ImGuiSliderFlags_None            = 0,
    ImGuiSliderFlags_AlwaysClamp     = 1 << 4,
    ImGuiSliderFlags_Logarithmic     = 1 << 5,
    ImGuiSliderFlags_NoRoundToFormat = 1 << 6,
    ImGuiSliderFlags_NoInput         = 1 << 7,
    ImGuiSliderFlags_InvalidMask_    = 0x70'00'00'0F,
} ImGuiSliderFlags_;

typedef enum {
    ImGuiMouseButton_Left   = 0,
    ImGuiMouseButton_Right  = 1,
    ImGuiMouseButton_Middle = 2,
    ImGuiMouseButton_COUNT  = 5
} ImGuiMouseButton_;

typedef enum {
    ImGuiMouseCursor_None  = -1,
    ImGuiMouseCursor_Arrow = 0,
    ImGuiMouseCursor_TextInput,
    ImGuiMouseCursor_ResizeAll,
    ImGuiMouseCursor_ResizeNS,
    ImGuiMouseCursor_ResizeEW,
    ImGuiMouseCursor_ResizeNESW,
    ImGuiMouseCursor_ResizeNWSE,
    ImGuiMouseCursor_Hand,
    ImGuiMouseCursor_NotAllowed,
    ImGuiMouseCursor_COUNT
} ImGuiMouseCursor_;

typedef enum {
    ImGuiMouseSource_Mouse       = 0,
    ImGuiMouseSource_TouchScreen = 1,
    ImGuiMouseSource_Pen         = 2,
    ImGuiMouseSource_COUNT       = 3,
} ImGuiMouseSource;

typedef enum {
    ImGuiCond_None         = 0,
    ImGuiCond_Always       = 1 << 0,
    ImGuiCond_Once         = 1 << 1,
    ImGuiCond_FirstUseEver = 1 << 2,
    ImGuiCond_Appearing    = 1 << 3,
} ImGuiCond_;

typedef enum {
    ImGuiTableFlags_None              = 0,
    ImGuiTableFlags_Resizable         = 1 << 0,
    ImGuiTableFlags_Reorderable       = 1 << 1,
    ImGuiTableFlags_Hideable          = 1 << 2,
    ImGuiTableFlags_Sortable          = 1 << 3,
    ImGuiTableFlags_NoSavedSettings   = 1 << 4,
    ImGuiTableFlags_ContextMenuInBody = 1 << 5,
    ImGuiTableFlags_RowBg             = 1 << 6,
    ImGuiTableFlags_BordersInnerH     = 1 << 7,
    ImGuiTableFlags_BordersOuterH     = 1 << 8,
    ImGuiTableFlags_BordersInnerV     = 1 << 9,
    ImGuiTableFlags_BordersOuterV     = 1 << 10,
    ImGuiTableFlags_BordersH        = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersOuterH,
    ImGuiTableFlags_BordersV        = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV,
    ImGuiTableFlags_BordersInner    = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersInnerH,
    ImGuiTableFlags_BordersOuter    = ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersOuterH,
    ImGuiTableFlags_Borders         = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter,
    ImGuiTableFlags_NoBordersInBody = 1 << 11,
    ImGuiTableFlags_NoBordersInBodyUntilResize = 1 << 12,
    ImGuiTableFlags_SizingFixedFit             = 1 << 13,
    ImGuiTableFlags_SizingFixedSame            = 2 << 13,
    ImGuiTableFlags_SizingStretchProp          = 3 << 13,
    ImGuiTableFlags_SizingStretchSame          = 4 << 13,
    ImGuiTableFlags_NoHostExtendX              = 1 << 16,
    ImGuiTableFlags_NoHostExtendY              = 1 << 17,
    ImGuiTableFlags_NoKeepColumnsVisible       = 1 << 18,
    ImGuiTableFlags_PreciseWidths              = 1 << 19,
    ImGuiTableFlags_NoClip                     = 1 << 20,
    ImGuiTableFlags_PadOuterX                  = 1 << 21,
    ImGuiTableFlags_NoPadOuterX                = 1 << 22,
    ImGuiTableFlags_NoPadInnerX                = 1 << 23,
    ImGuiTableFlags_ScrollX                    = 1 << 24,
    ImGuiTableFlags_ScrollY                    = 1 << 25,
    ImGuiTableFlags_SortMulti                  = 1 << 26,
    ImGuiTableFlags_SortTristate               = 1 << 27,
    ImGuiTableFlags_HighlightHoveredColumn     = 1 << 28,
    ImGuiTableFlags_SizingMask_ = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_SizingStretchSame,
} ImGuiTableFlags_;

typedef enum {
    ImGuiTableColumnFlags_None                 = 0,
    ImGuiTableColumnFlags_Disabled             = 1 << 0,
    ImGuiTableColumnFlags_DefaultHide          = 1 << 1,
    ImGuiTableColumnFlags_DefaultSort          = 1 << 2,
    ImGuiTableColumnFlags_WidthStretch         = 1 << 3,
    ImGuiTableColumnFlags_WidthFixed           = 1 << 4,
    ImGuiTableColumnFlags_NoResize             = 1 << 5,
    ImGuiTableColumnFlags_NoReorder            = 1 << 6,
    ImGuiTableColumnFlags_NoHide               = 1 << 7,
    ImGuiTableColumnFlags_NoClip               = 1 << 8,
    ImGuiTableColumnFlags_NoSort               = 1 << 9,
    ImGuiTableColumnFlags_NoSortAscending      = 1 << 10,
    ImGuiTableColumnFlags_NoSortDescending     = 1 << 11,
    ImGuiTableColumnFlags_NoHeaderLabel        = 1 << 12,
    ImGuiTableColumnFlags_NoHeaderWidth        = 1 << 13,
    ImGuiTableColumnFlags_PreferSortAscending  = 1 << 14,
    ImGuiTableColumnFlags_PreferSortDescending = 1 << 15,
    ImGuiTableColumnFlags_IndentEnable         = 1 << 16,
    ImGuiTableColumnFlags_IndentDisable        = 1 << 17,
    ImGuiTableColumnFlags_AngledHeader         = 1 << 18,
    ImGuiTableColumnFlags_IsEnabled            = 1 << 24,
    ImGuiTableColumnFlags_IsVisible            = 1 << 25,
    ImGuiTableColumnFlags_IsSorted             = 1 << 26,
    ImGuiTableColumnFlags_IsHovered            = 1 << 27,
    ImGuiTableColumnFlags_WidthMask_ = ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_WidthFixed,
    ImGuiTableColumnFlags_IndentMask_ = ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_IndentDisable,
    ImGuiTableColumnFlags_StatusMask_ = ImGuiTableColumnFlags_IsEnabled | ImGuiTableColumnFlags_IsVisible | ImGuiTableColumnFlags_IsSorted | ImGuiTableColumnFlags_IsHovered,
    ImGuiTableColumnFlags_NoDirectResize_ = 1 << 30,
} ImGuiTableColumnFlags_;

typedef enum {
    ImGuiTableRowFlags_None    = 0,
    ImGuiTableRowFlags_Headers = 1 << 0,
} ImGuiTableRowFlags_;

typedef enum {
    ImGuiTableBgTarget_None   = 0,
    ImGuiTableBgTarget_RowBg0 = 1,
    ImGuiTableBgTarget_RowBg1 = 2,
    ImGuiTableBgTarget_CellBg = 3,
} ImGuiTableBgTarget_;

struct ImGuiTableSortSpecs {
    const ImGuiTableColumnSortSpecs* Specs;
    int                              SpecsCount;
    bool                             SpecsDirty;
};

struct ImGuiTableColumnSortSpecs {
    ImGuiID            ColumnUserID;
    ImS16              ColumnIndex;
    ImS16              SortOrder;
    ImGuiSortDirection SortDirection;
};

struct ImGuiStyle {
    float             Alpha;
    float             DisabledAlpha;
    ImVec2            WindowPadding;
    float             WindowRounding;
    float             WindowBorderSize;
    ImVec2            WindowMinSize;
    ImVec2            WindowTitleAlign;
    ImGuiDir          WindowMenuButtonPosition;
    float             ChildRounding;
    float             ChildBorderSize;
    float             PopupRounding;
    float             PopupBorderSize;
    ImVec2            FramePadding;
    float             FrameRounding;
    float             FrameBorderSize;
    ImVec2            ItemSpacing;
    ImVec2            ItemInnerSpacing;
    ImVec2            CellPadding;
    ImVec2            TouchExtraPadding;
    float             IndentSpacing;
    float             ColumnsMinSpacing;
    float             ScrollbarSize;
    float             ScrollbarRounding;
    float             GrabMinSize;
    float             GrabRounding;
    float             LogSliderDeadzone;
    float             TabRounding;
    float             TabBorderSize;
    float             TabMinWidthForCloseButton;
    float             TabBarBorderSize;
    float             TableAngledHeadersAngle;
    ImVec2            TableAngledHeadersTextAlign;
    ImGuiDir          ColorButtonPosition;
    ImVec2            ButtonTextAlign;
    ImVec2            SelectableTextAlign;
    float             SeparatorTextBorderSize;
    ImVec2            SeparatorTextAlign;
    ImVec2            SeparatorTextPadding;
    ImVec2            DisplayWindowPadding;
    ImVec2            DisplaySafeAreaPadding;
    float             DockingSeparatorSize;
    float             MouseCursorScale;
    bool              AntiAliasedLines;
    bool              AntiAliasedLinesUseTex;
    bool              AntiAliasedFill;
    float             CurveTessellationTol;
    float             CircleTessellationMaxError;
    ImVec4            Colors[ImGuiCol_COUNT];
    float             HoverStationaryDelay;
    float             HoverDelayShort;
    float             HoverDelayNormal;
    ImGuiHoveredFlags HoverFlagsForTooltipMouse;
    ImGuiHoveredFlags HoverFlagsForTooltipNav;
};

struct ImGuiKeyData {
    bool  Down;
    float DownDuration;
    float DownDurationPrev;
    float AnalogValue;
};

typedef struct ImVector_ImWchar {
    int      Size;
    int      Capacity;
    ImWchar* Data;
} ImVector_ImWchar;

struct ImGuiIO {
    ImGuiConfigFlags  ConfigFlags;
    ImGuiBackendFlags BackendFlags;
    ImVec2            DisplaySize;
    float             DeltaTime;
    float             IniSavingRate;
    const char*       IniFilename;
    const char*       LogFilename;
    void*             UserData;
    ImFontAtlas*      Fonts;
    float             FontGlobalScale;
    bool              FontAllowUserScaling;
    ImFont*           FontDefault;
    ImVec2            DisplayFramebufferScale;
    bool              ConfigDockingNoSplit;
    bool              ConfigDockingWithShift;
    bool              ConfigDockingAlwaysTabBar;
    bool              ConfigDockingTransparentPayload;
    bool              ConfigViewportsNoAutoMerge;
    bool              ConfigViewportsNoTaskBarIcon;
    bool              ConfigViewportsNoDecoration;
    bool              ConfigViewportsNoDefaultParent;
    bool              MouseDrawCursor;
    bool              ConfigMacOSXBehaviors;
    bool              ConfigInputTrickleEventQueue;
    bool              ConfigInputTextCursorBlink;
    bool              ConfigInputTextEnterKeepActive;
    bool              ConfigDragClickToInputText;
    bool              ConfigWindowsResizeFromEdges;
    bool              ConfigWindowsMoveFromTitleBarOnly;
    float             ConfigMemoryCompactTimer;
    float             MouseDoubleClickTime;
    float             MouseDoubleClickMaxDist;
    float             MouseDragThreshold;
    float             KeyRepeatDelay;
    float             KeyRepeatRate;
    bool              ConfigDebugIsDebuggerPresent;
    bool              ConfigDebugBeginReturnValueOnce;
    bool              ConfigDebugBeginReturnValueLoop;
    bool              ConfigDebugIgnoreFocusLoss;
    bool              ConfigDebugIniSettings;
    const char*       BackendPlatformName;
    const char*       BackendRendererName;
    void*             BackendPlatformUserData;
    void*             BackendRendererUserData;
    void*             BackendLanguageUserData;
    const char* (*GetClipboardTextFn)(void* user_data);
    void (*SetClipboardTextFn)(void* user_data, const char* text);
    void* ClipboardUserData;
    void (*SetPlatformImeDataFn)(ImGuiViewport* viewport, ImGuiPlatformImeData* data);
    ImWchar          PlatformLocaleDecimalPoint;
    bool             WantCaptureMouse;
    bool             WantCaptureKeyboard;
    bool             WantTextInput;
    bool             WantSetMousePos;
    bool             WantSaveIniSettings;
    bool             NavActive;
    bool             NavVisible;
    float            Framerate;
    int              MetricsRenderVertices;
    int              MetricsRenderIndices;
    int              MetricsRenderWindows;
    int              MetricsActiveWindows;
    ImVec2           MouseDelta;
    ImGuiContext*    Ctx;
    ImVec2           MousePos;
    bool             MouseDown[5];
    float            MouseWheel;
    float            MouseWheelH;
    ImGuiMouseSource MouseSource;
    ImGuiID          MouseHoveredViewport;
    bool             KeyCtrl;
    bool             KeyShift;
    bool             KeyAlt;
    bool             KeySuper;
    ImGuiKeyChord    KeyMods;
    ImGuiKeyData     KeysData[ImGuiKey_KeysData_SIZE];
    bool             WantCaptureMouseUnlessPopupClose;
    ImVec2           MousePosPrev;
    ImVec2           MouseClickedPos[5];
    double           MouseClickedTime[5];
    bool             MouseClicked[5];
    bool             MouseDoubleClicked[5];
    ImU16            MouseClickedCount[5];
    ImU16            MouseClickedLastCount[5];
    bool             MouseReleased[5];
    bool             MouseDownOwned[5];
    bool             MouseDownOwnedUnlessPopupClose[5];
    bool             MouseWheelRequestAxisSwap;
    bool             MouseCtrlLeftAsRightClick;
    float            MouseDownDuration[5];
    float            MouseDownDurationPrev[5];
    ImVec2           MouseDragMaxDistanceAbs[5];
    float            MouseDragMaxDistanceSqr[5];
    float            PenPressure;
    bool             AppFocusLost;
    bool             AppAcceptingEvents;
    ImS8             BackendUsingLegacyKeyArrays;
    bool             BackendUsingLegacyNavInputArray;
    ImWchar16        InputQueueSurrogate;
    ImVector_ImWchar InputQueueCharacters;
};

struct ImGuiInputTextCallbackData {
    ImGuiContext*       Ctx;
    ImGuiInputTextFlags EventFlag;
    ImGuiInputTextFlags Flags;
    void*               UserData;
    ImWchar             EventChar;
    ImGuiKey            EventKey;
    char*               Buf;
    int                 BufTextLen;
    int                 BufSize;
    bool                BufDirty;
    int                 CursorPos;
    int                 SelectionStart;
    int                 SelectionEnd;
};

struct ImGuiSizeCallbackData {
    void*  UserData;
    ImVec2 Pos;
    ImVec2 CurrentSize;
    ImVec2 DesiredSize;
};

struct ImGuiWindowClass {
    ImGuiID            ClassId;
    ImGuiID            ParentViewportId;
    ImGuiID            FocusRouteParentWindowId;
    ImGuiViewportFlags ViewportFlagsOverrideSet;
    ImGuiViewportFlags ViewportFlagsOverrideClear;
    ImGuiTabItemFlags  TabItemFlagsOverrideSet;
    ImGuiDockNodeFlags DockNodeFlagsOverrideSet;
    bool               DockingAlwaysTabBar;
    bool               DockingAllowUnclassed;
};

struct ImGuiPayload {
    void*   Data;
    int     DataSize;
    ImGuiID SourceId;
    ImGuiID SourceParentId;
    int     DataFrameCount;
    char    DataType[32 + 1];
    bool    Preview;
    bool    Delivery;
};

struct ImGuiOnceUponAFrame {
    int RefFrame;
};

struct ImGuiTextRange {
    const char* b;
    const char* e;
};
typedef struct ImGuiTextRange ImGuiTextRange;

typedef struct ImVector_ImGuiTextRange {
    int             Size;
    int             Capacity;
    ImGuiTextRange* Data;
} ImVector_ImGuiTextRange;

class ImGuiTextFilter
{
private:
    struct Data {
        char                    InputBuf[256];
        ImVector_ImGuiTextRange Filters;
        int                     CountGrep;
    };

    Data* data;

    static inline Data* Create(const char* default_filter) {
        using func_t            = Data* (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiText"
                                                                                         "Filter_"
                                                                                         "ImGuiText"
                                                                                         "Filter"));
        return func(default_filter);
    }

    inline void Destroy() const {
        using func_t            = void (*)(Data*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiText"
                                                                                         "Filter_"
                                                                                         "destro"
                                                                                         "y"));
        return func(data);
    }

public:
    explicit ImGuiTextFilter(const char* default_filter = ""):
        data(Create(default_filter)) { }

    ~ImGuiTextFilter() { Destroy(); }

    inline bool Draw(const char* label = "Filter (inc,-exc)", float width = 0.0f) const {
        using func_t            = bool (*)(Data*, const char*, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiText"
                                                                                         "Filter_"
                                                                                         "Draw"));
        return func(data, label, width);
    }

    inline bool PassFilter(const char* text, const char* text_end = nullptr) const {
        using func_t            = bool (*)(Data*, const char*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiText"
                                                                                         "Filter_"
                                                                                         "PassFilte"
                                                                                         "r"));
        return func(data, text, text_end);
    }

    inline void Build() const {
        using func_t            = void (*)(Data*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiText"
                                                                                         "Filter_"
                                                                                         "Build"));
        return func(data);
    }

    inline void Clear() const {
        using func_t            = void (*)(Data*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiText"
                                                                                         "Filter_"
                                                                                         "Clear"));
        return func(data);
    }

    [[nodiscard]] inline bool IsActive() const {
        using func_t            = bool (*)(Data*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiText"
                                                                                         "Filter_"
                                                                                         "IsActiv"
                                                                                         "e"));
        return func(data);
    }
};

typedef struct ImGuiTextRange ImGuiTextRange;

typedef struct ImVector_char {
    int   Size;
    int   Capacity;
    char* Data;
} ImVector_char;

struct ImGuiTextBuffer {
    ImVector_char Buf;
};

struct ImGuiStoragePair {
    ImGuiID key;

    union {
        int   val_i;
        float val_f;
        void* val_p;
    };
};
typedef struct ImGuiStoragePair ImGuiStoragePair;

typedef struct ImVector_ImGuiStoragePair {
    int               Size;
    int               Capacity;
    ImGuiStoragePair* Data;
} ImVector_ImGuiStoragePair;

struct ImGuiStorage {
    ImVector_ImGuiStoragePair Data;
};
typedef struct ImGuiStoragePair ImGuiStoragePair;

struct ImGuiListClipper {
    ImGuiContext* Ctx;
    int           DisplayStart;
    int           DisplayEnd;
    int           ItemsCount;
    float         ItemsHeight;
    float         StartPosY;
    void*         TempData;
};

struct ImColor {
    ImVec4 Value;
};

typedef void (*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);

struct ImDrawCmd {
    ImVec4         ClipRect;
    ImTextureID    TextureId;
    unsigned int   VtxOffset;
    unsigned int   IdxOffset;
    unsigned int   ElemCount;
    ImDrawCallback UserCallback;
    void*          UserCallbackData;
};

struct ImDrawVert {
    ImVec2 pos;
    ImVec2 uv;
    ImU32  col;
};
typedef struct ImDrawCmdHeader ImDrawCmdHeader;

struct ImDrawCmdHeader {
    ImVec4       ClipRect;
    ImTextureID  TextureId;
    unsigned int VtxOffset;
};

typedef struct ImVector_ImDrawCmd {
    int        Size;
    int        Capacity;
    ImDrawCmd* Data;
} ImVector_ImDrawCmd;

typedef struct ImVector_ImDrawIdx {
    int        Size;
    int        Capacity;
    ImDrawIdx* Data;
} ImVector_ImDrawIdx;

struct ImDrawChannel {
    ImVector_ImDrawCmd _CmdBuffer;
    ImVector_ImDrawIdx _IdxBuffer;
};

typedef struct ImVector_ImDrawChannel {
    int            Size;
    int            Capacity;
    ImDrawChannel* Data;
} ImVector_ImDrawChannel;

struct ImDrawListSplitter {
    int                    _Current;
    int                    _Count;
    ImVector_ImDrawChannel _Channels;
};

typedef enum {
    ImDrawFlags_None                    = 0,
    ImDrawFlags_Closed                  = 1 << 0,
    ImDrawFlags_RoundCornersTopLeft     = 1 << 4,
    ImDrawFlags_RoundCornersTopRight    = 1 << 5,
    ImDrawFlags_RoundCornersBottomLeft  = 1 << 6,
    ImDrawFlags_RoundCornersBottomRight = 1 << 7,
    ImDrawFlags_RoundCornersNone        = 1 << 8,
    ImDrawFlags_RoundCornersTop = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight,
    ImDrawFlags_RoundCornersBottom = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight,
    ImDrawFlags_RoundCornersLeft = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersTopLeft,
    ImDrawFlags_RoundCornersRight = ImDrawFlags_RoundCornersBottomRight | ImDrawFlags_RoundCornersTopRight,
    ImDrawFlags_RoundCornersAll = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight,
    ImDrawFlags_RoundCornersDefault_ = ImDrawFlags_RoundCornersAll,
    ImDrawFlags_RoundCornersMask_    = ImDrawFlags_RoundCornersAll | ImDrawFlags_RoundCornersNone,
} ImDrawFlags_;

typedef enum {
    ImDrawListFlags_None                   = 0,
    ImDrawListFlags_AntiAliasedLines       = 1 << 0,
    ImDrawListFlags_AntiAliasedLinesUseTex = 1 << 1,
    ImDrawListFlags_AntiAliasedFill        = 1 << 2,
    ImDrawListFlags_AllowVtxOffset         = 1 << 3,
} ImDrawListFlags_;

typedef struct ImVector_ImDrawVert {
    int         Size;
    int         Capacity;
    ImDrawVert* Data;
} ImVector_ImDrawVert;

typedef struct ImVector_ImVec2 {
    int     Size;
    int     Capacity;
    ImVec2* Data;
} ImVector_ImVec2;

typedef struct ImVector_ImVec4 {
    int     Size;
    int     Capacity;
    ImVec4* Data;
} ImVector_ImVec4;

typedef struct ImVector_ImTextureID {
    int          Size;
    int          Capacity;
    ImTextureID* Data;
} ImVector_ImTextureID;

struct ImDrawList {
    ImVector_ImDrawCmd    CmdBuffer;
    ImVector_ImDrawIdx    IdxBuffer;
    ImVector_ImDrawVert   VtxBuffer;
    ImDrawListFlags       Flags;
    unsigned int          _VtxCurrentIdx;
    ImDrawListSharedData* _Data;
    ImDrawVert*           _VtxWritePtr;
    ImDrawIdx*            _IdxWritePtr;
    ImVector_ImVec2       _Path;
    ImDrawCmdHeader       _CmdHeader;
    ImDrawListSplitter    _Splitter;
    ImVector_ImVec4       _ClipRectStack;
    ImVector_ImTextureID  _TextureIdStack;
    float                 _FringeScale;
    const char*           _OwnerName;
};

typedef struct ImVector_ImDrawListPtr {
    int          Size;
    int          Capacity;
    ImDrawList** Data;
} ImVector_ImDrawListPtr;

struct ImDrawData {
    bool                   Valid;
    int                    CmdListsCount;
    int                    TotalIdxCount;
    int                    TotalVtxCount;
    ImVector_ImDrawListPtr CmdLists;
    ImVec2                 DisplayPos;
    ImVec2                 DisplaySize;
    ImVec2                 FramebufferScale;
    ImGuiViewport*         OwnerViewport;
};

struct ImFontConfig {
    void*          FontData;
    int            FontDataSize;
    bool           FontDataOwnedByAtlas;
    int            FontNo;
    float          SizePixels;
    int            OversampleH;
    int            OversampleV;
    bool           PixelSnapH;
    ImVec2         GlyphExtraSpacing;
    ImVec2         GlyphOffset;
    const ImWchar* GlyphRanges;
    float          GlyphMinAdvanceX;
    float          GlyphMaxAdvanceX;
    bool           MergeMode;
    unsigned int   FontBuilderFlags;
    float          RasterizerMultiply;
    float          RasterizerDensity;
    ImWchar        EllipsisChar;
    char           Name[40];
    ImFont*        DstFont;
};

struct ImFontGlyph {
    unsigned int Colored  : 1;
    unsigned int Visible  : 1;
    unsigned int Codepoint: 30;
    float        AdvanceX;
    float        X0, Y0, X1, Y1;
    float        U0, V0, U1, V1;
};

typedef struct ImVector_ImU32 {
    int    Size;
    int    Capacity;
    ImU32* Data;
} ImVector_ImU32;

struct ImFontGlyphRangesBuilder {
    ImVector_ImU32 UsedChars;
};
typedef struct ImFontAtlasCustomRect ImFontAtlasCustomRect;

struct ImFontAtlasCustomRect {
    unsigned short Width, Height;
    unsigned short X, Y;
    unsigned int   GlyphID;
    float          GlyphAdvanceX;
    ImVec2         GlyphOffset;
    ImFont*        Font;
};

typedef enum {
    ImFontAtlasFlags_None               = 0,
    ImFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,
    ImFontAtlasFlags_NoMouseCursors     = 1 << 1,
    ImFontAtlasFlags_NoBakedLines       = 1 << 2,
} ImFontAtlasFlags_;

typedef struct ImVector_ImFontPtr {
    int      Size;
    int      Capacity;
    ImFont** Data;
} ImVector_ImFontPtr;

typedef struct ImVector_ImFontAtlasCustomRect {
    int                    Size;
    int                    Capacity;
    ImFontAtlasCustomRect* Data;
} ImVector_ImFontAtlasCustomRect;

typedef struct ImVector_ImFontConfig {
    int           Size;
    int           Capacity;
    ImFontConfig* Data;
} ImVector_ImFontConfig;

struct ImFontAtlas {
    ImFontAtlasFlags               Flags;
    ImTextureID                    TexID;
    int                            TexDesiredWidth;
    int                            TexGlyphPadding;
    bool                           Locked;
    void*                          UserData;
    bool                           TexReady;
    bool                           TexPixelsUseColors;
    unsigned char*                 TexPixelsAlpha8;
    unsigned int*                  TexPixelsRGBA32;
    int                            TexWidth;
    int                            TexHeight;
    ImVec2                         TexUvScale;
    ImVec2                         TexUvWhitePixel;
    ImVector_ImFontPtr             Fonts;
    ImVector_ImFontAtlasCustomRect CustomRects;
    ImVector_ImFontConfig          ConfigData;
    ImVec4                         TexUvLines[(63) + 1];
    const ImFontBuilderIO*         FontBuilderIO;
    unsigned int                   FontBuilderFlags;
    int                            PackIdMouseCursors;
    int                            PackIdLines;
};

typedef struct ImVector_float {
    int    Size;
    int    Capacity;
    float* Data;
} ImVector_float;

typedef struct ImVector_ImFontGlyph {
    int          Size;
    int          Capacity;
    ImFontGlyph* Data;
} ImVector_ImFontGlyph;

struct ImFont {
    ImVector_float       IndexAdvanceX;
    float                FallbackAdvanceX;
    float                FontSize;
    ImVector_ImWchar     IndexLookup;
    ImVector_ImFontGlyph Glyphs;
    const ImFontGlyph*   FallbackGlyph;
    ImFontAtlas*         ContainerAtlas;
    const ImFontConfig*  ConfigData;
    short                ConfigDataCount;
    ImWchar              FallbackChar;
    ImWchar              EllipsisChar;
    short                EllipsisCharCount;
    float                EllipsisWidth;
    float                EllipsisCharStep;
    bool                 DirtyLookupTables;
    float                Scale;
    float                Ascent, Descent;
    int                  MetricsTotalSurface;
    ImU8                 Used4kPagesMap[(0xFF'FF + 1) / 4'096 / 8];
};

typedef enum {
    ImGuiViewportFlags_None                = 0,
    ImGuiViewportFlags_IsPlatformWindow    = 1 << 0,
    ImGuiViewportFlags_IsPlatformMonitor   = 1 << 1,
    ImGuiViewportFlags_OwnedByApp          = 1 << 2,
    ImGuiViewportFlags_NoDecoration        = 1 << 3,
    ImGuiViewportFlags_NoTaskBarIcon       = 1 << 4,
    ImGuiViewportFlags_NoFocusOnAppearing  = 1 << 5,
    ImGuiViewportFlags_NoFocusOnClick      = 1 << 6,
    ImGuiViewportFlags_NoInputs            = 1 << 7,
    ImGuiViewportFlags_NoRendererClear     = 1 << 8,
    ImGuiViewportFlags_NoAutoMerge         = 1 << 9,
    ImGuiViewportFlags_TopMost             = 1 << 10,
    ImGuiViewportFlags_CanHostOtherWindows = 1 << 11,
    ImGuiViewportFlags_IsMinimized         = 1 << 12,
    ImGuiViewportFlags_IsFocused           = 1 << 13,
} ImGuiViewportFlags_;

struct ImGuiViewport {
    ImGuiID            ID;
    ImGuiViewportFlags Flags;
    ImVec2             Pos;
    ImVec2             Size;
    ImVec2             WorkPos;
    ImVec2             WorkSize;
    float              DpiScale;
    ImGuiID            ParentViewportId;
    ImDrawData*        DrawData;
    void*              RendererUserData;
    void*              PlatformUserData;
    void*              PlatformHandle;
    void*              PlatformHandleRaw;
    bool               PlatformWindowCreated;
    bool               PlatformRequestMove;
    bool               PlatformRequestResize;
    bool               PlatformRequestClose;
};

typedef struct ImVector_ImGuiPlatformMonitor {
    int                   Size;
    int                   Capacity;
    ImGuiPlatformMonitor* Data;
} ImVector_ImGuiPlatformMonitor;

typedef struct ImVector_ImGuiViewportPtr {
    int             Size;
    int             Capacity;
    ImGuiViewport** Data;
} ImVector_ImGuiViewportPtr;

struct ImGuiPlatformIO {
    void (*Platform_CreateWindow)(ImGuiViewport* vp);
    void (*Platform_DestroyWindow)(ImGuiViewport* vp);
    void (*Platform_ShowWindow)(ImGuiViewport* vp);
    void (*Platform_SetWindowPos)(ImGuiViewport* vp, ImVec2 pos);
    ImVec2 (*Platform_GetWindowPos)(ImGuiViewport* vp);
    void (*Platform_SetWindowSize)(ImGuiViewport* vp, ImVec2 size);
    ImVec2 (*Platform_GetWindowSize)(ImGuiViewport* vp);
    void (*Platform_SetWindowFocus)(ImGuiViewport* vp);
    bool (*Platform_GetWindowFocus)(ImGuiViewport* vp);
    bool (*Platform_GetWindowMinimized)(ImGuiViewport* vp);
    void (*Platform_SetWindowTitle)(ImGuiViewport* vp, const char* str);
    void (*Platform_SetWindowAlpha)(ImGuiViewport* vp, float alpha);
    void (*Platform_UpdateWindow)(ImGuiViewport* vp);
    void (*Platform_RenderWindow)(ImGuiViewport* vp, void* render_arg);
    void (*Platform_SwapBuffers)(ImGuiViewport* vp, void* render_arg);
    float (*Platform_GetWindowDpiScale)(ImGuiViewport* vp);
    void (*Platform_OnChangedViewport)(ImGuiViewport* vp);
    int (*Platform_CreateVkSurface)(ImGuiViewport* vp, ImU64 vk_inst, const void* vk_allocators, ImU64* out_vk_surface);
    void (*Renderer_CreateWindow)(ImGuiViewport* vp);
    void (*Renderer_DestroyWindow)(ImGuiViewport* vp);
    void (*Renderer_SetWindowSize)(ImGuiViewport* vp, ImVec2 size);
    void (*Renderer_RenderWindow)(ImGuiViewport* vp, void* render_arg);
    void (*Renderer_SwapBuffers)(ImGuiViewport* vp, void* render_arg);
    ImVector_ImGuiPlatformMonitor Monitors;
    ImVector_ImGuiViewportPtr     Viewports;
};

struct ImGuiPlatformMonitor {
    ImVec2 MainPos, MainSize;
    ImVec2 WorkPos, WorkSize;
    float  DpiScale;
    void*  PlatformHandle;
};

struct ImGuiPlatformImeData {
    bool   WantVisible;
    ImVec2 InputPos;
    float  InputLineHeight;
};
struct ImBitVector;
struct ImRect;
struct ImDrawDataBuilder;
struct ImDrawListSharedData;
struct ImGuiColorMod;
struct ImGuiContext;
struct ImGuiContextHook;
struct ImGuiDataVarInfo;
struct ImGuiDataTypeInfo;
struct ImGuiDockContext;
struct ImGuiDockRequest;
struct ImGuiDockNode;
struct ImGuiDockNodeSettings;
struct ImGuiGroupData;
struct ImGuiInputTextState;
struct ImGuiInputTextDeactivateData;
struct ImGuiLastItemData;
struct ImGuiLocEntry;
struct ImGuiMenuColumns;
struct ImGuiNavItemData;
struct ImGuiNavTreeNodeData;
struct ImGuiMetricsConfig;
struct ImGuiNextWindowData;
struct ImGuiNextItemData;
struct ImGuiOldColumnData;
struct ImGuiOldColumns;
struct ImGuiPopupData;
struct ImGuiSettingsHandler;
struct ImGuiStackSizes;
struct ImGuiStyleMod;
struct ImGuiTabBar;
struct ImGuiTabItem;
struct ImGuiTable;
struct ImGuiTableHeaderData;
struct ImGuiTableColumn;
struct ImGuiTableInstanceData;
struct ImGuiTableTempData;
struct ImGuiTableSettings;
struct ImGuiTableColumnsSettings;
struct ImGuiTypingSelectState;
struct ImGuiTypingSelectRequest;
struct ImGuiWindow;
struct ImGuiWindowDockStyle;
struct ImGuiWindowTempData;
struct ImGuiWindowSettings;
typedef int ImGuiDataAuthority;
typedef int ImGuiLayoutType;
typedef int ImGuiActivateFlags;
typedef int ImGuiDebugLogFlags;
typedef int ImGuiFocusRequestFlags;
typedef int ImGuiItemFlags;
typedef int ImGuiItemStatusFlags;
typedef int ImGuiOldColumnFlags;
typedef int ImGuiNavHighlightFlags;
typedef int ImGuiNavMoveFlags;
typedef int ImGuiNextItemDataFlags;
typedef int ImGuiNextWindowDataFlags;
typedef int ImGuiScrollFlags;
typedef int ImGuiSeparatorFlags;
typedef int ImGuiTextFlags;
typedef int ImGuiTooltipFlags;
typedef int ImGuiTypingSelectFlags;
typedef int ImGuiWindowRefreshFlags;
typedef void (*ImGuiErrorLogCallback)(void* user_data, const char* fmt, ...);
extern ImGuiContext*         GImGui;
typedef struct StbUndoRecord StbUndoRecord;

struct StbUndoRecord {
    int where;
    int insert_length;
    int delete_length;
    int char_storage;
};
typedef struct StbUndoState StbUndoState;

struct StbUndoState {
    StbUndoRecord undo_rec[99];
    ImWchar       undo_char[999];
    short         undo_point, redo_point;
    int           undo_char_point, redo_char_point;
};
typedef struct STB_TexteditState STB_TexteditState;

struct STB_TexteditState {
    int           cursor;
    int           select_start;
    int           select_end;
    unsigned char insert_mode;
    int           row_count_per_page;
    unsigned char cursor_at_end_of_line;
    unsigned char initialized;
    unsigned char has_preferred_x;
    unsigned char single_line;
    unsigned char padding1, padding2, padding3;
    float         preferred_x;
    StbUndoState  undostate;
};
typedef struct StbTexteditRow StbTexteditRow;

struct StbTexteditRow {
    float x0, x1;
    float baseline_y_delta;
    float ymin, ymax;
    int   num_chars;
};

typedef FILE*         ImFileHandle;
typedef struct ImVec1 ImVec1;

struct ImVec1 {
    float x;
};
typedef struct ImVec2ih ImVec2ih;

struct ImVec2ih {
    short x, y;
};

struct ImRect {
    ImVec2 Min;
    ImVec2 Max;
};

typedef ImU32* ImBitArrayPtr;

struct ImBitVector {
    ImVector_ImU32 Storage;
};

typedef int                   ImPoolIdx;
typedef struct ImGuiTextIndex ImGuiTextIndex;

typedef struct ImVector_int {
    int  Size;
    int  Capacity;
    int* Data;
} ImVector_int;

struct ImGuiTextIndex {
    ImVector_int LineOffsets;
    int          EndOffset;
};

struct ImDrawListSharedData {
    ImVec2          TexUvWhitePixel;
    ImFont*         Font;
    float           FontSize;
    float           CurveTessellationTol;
    float           CircleSegmentMaxError;
    ImVec4          ClipRectFullscreen;
    ImDrawListFlags InitialFlags;
    ImVector_ImVec2 TempBuffer;
    ImVec2          ArcFastVtx[48];
    float           ArcFastRadiusCutoff;
    ImU8            CircleSegmentCounts[64];
    const ImVec4*   TexUvLines;
};

struct ImDrawDataBuilder {
    ImVector_ImDrawListPtr* Layers[2];
    ImVector_ImDrawListPtr  LayerData1;
};

struct ImGuiDataVarInfo {
    ImGuiDataType Type;
    ImU32         Count;
    ImU32         Offset;
};
typedef struct ImGuiDataTypeStorage ImGuiDataTypeStorage;

struct ImGuiDataTypeStorage {
    ImU8 Data[8];
};

struct ImGuiDataTypeInfo {
    size_t      Size;
    const char* Name;
    const char* PrintFmt;
    const char* ScanFmt;
};

typedef enum {
    ImGuiDataType_String = ImGuiDataType_COUNT + 1,
    ImGuiDataType_Pointer,
    ImGuiDataType_ID,
} ImGuiDataTypePrivate_;

typedef enum {
    ImGuiItemFlags_None                     = 0,
    ImGuiItemFlags_NoTabStop                = 1 << 0,
    ImGuiItemFlags_ButtonRepeat             = 1 << 1,
    ImGuiItemFlags_Disabled                 = 1 << 2,
    ImGuiItemFlags_NoNav                    = 1 << 3,
    ImGuiItemFlags_NoNavDefaultFocus        = 1 << 4,
    ImGuiItemFlags_SelectableDontClosePopup = 1 << 5,
    ImGuiItemFlags_MixedValue               = 1 << 6,
    ImGuiItemFlags_ReadOnly                 = 1 << 7,
    ImGuiItemFlags_NoWindowHoverableCheck   = 1 << 8,
    ImGuiItemFlags_AllowOverlap             = 1 << 9,
    ImGuiItemFlags_Inputable                = 1 << 10,
    ImGuiItemFlags_HasSelectionUserData     = 1 << 11,
} ImGuiItemFlags_;

typedef enum {
    ImGuiItemStatusFlags_None             = 0,
    ImGuiItemStatusFlags_HoveredRect      = 1 << 0,
    ImGuiItemStatusFlags_HasDisplayRect   = 1 << 1,
    ImGuiItemStatusFlags_Edited           = 1 << 2,
    ImGuiItemStatusFlags_ToggledSelection = 1 << 3,
    ImGuiItemStatusFlags_ToggledOpen      = 1 << 4,
    ImGuiItemStatusFlags_HasDeactivated   = 1 << 5,
    ImGuiItemStatusFlags_Deactivated      = 1 << 6,
    ImGuiItemStatusFlags_HoveredWindow    = 1 << 7,
    ImGuiItemStatusFlags_Visible          = 1 << 8,
    ImGuiItemStatusFlags_HasClipRect      = 1 << 9,
    ImGuiItemStatusFlags_HasShortcut      = 1 << 10,
} ImGuiItemStatusFlags_;

typedef enum {
    ImGuiHoveredFlags_DelayMask_ = ImGuiHoveredFlags_DelayNone | ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay,
    ImGuiHoveredFlags_AllowedMaskForIsWindowHovered = ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_NoPopupHierarchy | ImGuiHoveredFlags_DockHierarchy | ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_Stationary,
    ImGuiHoveredFlags_AllowedMaskForIsItemHovered = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped | ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_NoNavOverride | ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayMask_,
} ImGuiHoveredFlagsPrivate_;

typedef enum {
    ImGuiInputTextFlags_Multiline            = 1 << 26,
    ImGuiInputTextFlags_NoMarkEdited         = 1 << 27,
    ImGuiInputTextFlags_MergedItem           = 1 << 28,
    ImGuiInputTextFlags_LocalizeDecimalPoint = 1 << 29,
} ImGuiInputTextFlagsPrivate_;

typedef enum {
    ImGuiButtonFlags_PressedOnClick                = 1 << 4,
    ImGuiButtonFlags_PressedOnClickRelease         = 1 << 5,
    ImGuiButtonFlags_PressedOnClickReleaseAnywhere = 1 << 6,
    ImGuiButtonFlags_PressedOnRelease              = 1 << 7,
    ImGuiButtonFlags_PressedOnDoubleClick          = 1 << 8,
    ImGuiButtonFlags_PressedOnDragDropHold         = 1 << 9,
    ImGuiButtonFlags_Repeat                        = 1 << 10,
    ImGuiButtonFlags_FlattenChildren               = 1 << 11,
    ImGuiButtonFlags_AllowOverlap                  = 1 << 12,
    ImGuiButtonFlags_DontClosePopups               = 1 << 13,
    ImGuiButtonFlags_AlignTextBaseLine             = 1 << 15,
    ImGuiButtonFlags_NoKeyModifiers                = 1 << 16,
    ImGuiButtonFlags_NoHoldingActiveId             = 1 << 17,
    ImGuiButtonFlags_NoNavFocus                    = 1 << 18,
    ImGuiButtonFlags_NoHoveredOnFocus              = 1 << 19,
    ImGuiButtonFlags_NoSetKeyOwner                 = 1 << 20,
    ImGuiButtonFlags_NoTestKeyOwner                = 1 << 21,
    ImGuiButtonFlags_PressedOnMask_ = ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnClickReleaseAnywhere | ImGuiButtonFlags_PressedOnRelease | ImGuiButtonFlags_PressedOnDoubleClick | ImGuiButtonFlags_PressedOnDragDropHold,
    ImGuiButtonFlags_PressedOnDefault_ = ImGuiButtonFlags_PressedOnClickRelease,
} ImGuiButtonFlagsPrivate_;

typedef enum {
    ImGuiComboFlags_CustomPreview = 1 << 20,
} ImGuiComboFlagsPrivate_;

typedef enum {
    ImGuiSliderFlags_Vertical = 1 << 20,
    ImGuiSliderFlags_ReadOnly = 1 << 21,
} ImGuiSliderFlagsPrivate_;

typedef enum {
    ImGuiSelectableFlags_NoHoldingActiveID    = 1 << 20,
    ImGuiSelectableFlags_SelectOnNav          = 1 << 21,
    ImGuiSelectableFlags_SelectOnClick        = 1 << 22,
    ImGuiSelectableFlags_SelectOnRelease      = 1 << 23,
    ImGuiSelectableFlags_SpanAvailWidth       = 1 << 24,
    ImGuiSelectableFlags_SetNavIdOnHover      = 1 << 25,
    ImGuiSelectableFlags_NoPadWithHalfSpacing = 1 << 26,
    ImGuiSelectableFlags_NoSetKeyOwner        = 1 << 27,
} ImGuiSelectableFlagsPrivate_;

typedef enum {
    ImGuiTreeNodeFlags_ClipLabelForTrailingButton = 1 << 20,
    ImGuiTreeNodeFlags_UpsideDownArrow            = 1 << 21,
} ImGuiTreeNodeFlagsPrivate_;

typedef enum {
    ImGuiSeparatorFlags_None           = 0,
    ImGuiSeparatorFlags_Horizontal     = 1 << 0,
    ImGuiSeparatorFlags_Vertical       = 1 << 1,
    ImGuiSeparatorFlags_SpanAllColumns = 1 << 2,
} ImGuiSeparatorFlags_;

typedef enum {
    ImGuiFocusRequestFlags_None                = 0,
    ImGuiFocusRequestFlags_RestoreFocusedChild = 1 << 0,
    ImGuiFocusRequestFlags_UnlessBelowModal    = 1 << 1,
} ImGuiFocusRequestFlags_;

typedef enum {
    ImGuiTextFlags_None                       = 0,
    ImGuiTextFlags_NoWidthForLargeClippedText = 1 << 0,
} ImGuiTextFlags_;

typedef enum {
    ImGuiTooltipFlags_None             = 0,
    ImGuiTooltipFlags_OverridePrevious = 1 << 1,
} ImGuiTooltipFlags_;

typedef enum {
    ImGuiLayoutType_Horizontal = 0,
    ImGuiLayoutType_Vertical   = 1
} ImGuiLayoutType_;

typedef enum {
    ImGuiLogType_None = 0,
    ImGuiLogType_TTY,
    ImGuiLogType_File,
    ImGuiLogType_Buffer,
    ImGuiLogType_Clipboard,
} ImGuiLogType;

typedef enum {
    ImGuiAxis_None = -1,
    ImGuiAxis_X    = 0,
    ImGuiAxis_Y    = 1
} ImGuiAxis;

typedef enum {
    ImGuiPlotType_Lines,
    ImGuiPlotType_Histogram,
} ImGuiPlotType;

struct ImGuiColorMod {
    ImGuiCol Col;
    ImVec4   BackupValue;
};

struct ImGuiStyleMod {
    ImGuiStyleVar VarIdx;

    union {
        int   BackupInt[2];
        float BackupFloat[2];
    };
};
typedef struct ImGuiComboPreviewData ImGuiComboPreviewData;

struct ImGuiComboPreviewData {
    ImRect          PreviewRect;
    ImVec2          BackupCursorPos;
    ImVec2          BackupCursorMaxPos;
    ImVec2          BackupCursorPosPrevLine;
    float           BackupPrevLineTextBaseOffset;
    ImGuiLayoutType BackupLayout;
};

struct ImGuiGroupData {
    ImGuiID WindowID;
    ImVec2  BackupCursorPos;
    ImVec2  BackupCursorMaxPos;
    ImVec2  BackupCursorPosPrevLine;
    ImVec1  BackupIndent;
    ImVec1  BackupGroupOffset;
    ImVec2  BackupCurrLineSize;
    float   BackupCurrLineTextBaseOffset;
    ImGuiID BackupActiveIdIsAlive;
    bool    BackupActiveIdPreviousFrameIsAlive;
    bool    BackupHoveredIdIsAlive;
    bool    BackupIsSameLine;
    bool    EmitItem;
};

struct ImGuiMenuColumns {
    ImU32 TotalWidth;
    ImU32 NextTotalWidth;
    ImU16 Spacing;
    ImU16 OffsetIcon;
    ImU16 OffsetLabel;
    ImU16 OffsetShortcut;
    ImU16 OffsetMark;
    ImU16 Widths[4];
};
typedef struct ImGuiInputTextDeactivatedState ImGuiInputTextDeactivatedState;

struct ImGuiInputTextDeactivatedState {
    ImGuiID       ID;
    ImVector_char TextA;
};

struct ImGuiInputTextState {
    ImGuiContext*       Ctx;
    ImGuiID             ID;
    int                 CurLenW, CurLenA;
    ImVector_ImWchar    TextW;
    ImVector_char       TextA;
    ImVector_char       InitialTextA;
    bool                TextAIsValid;
    int                 BufCapacityA;
    float               ScrollX;
    STB_TexteditState   Stb;
    float               CursorAnim;
    bool                CursorFollow;
    bool                SelectedAllMouseLock;
    bool                Edited;
    ImGuiInputTextFlags Flags;
    bool                ReloadUserBuf;
    int                 ReloadSelectionStart;
    int                 ReloadSelectionEnd;
};

typedef enum {
    ImGuiWindowRefreshFlags_None              = 0,
    ImGuiWindowRefreshFlags_TryToAvoidRefresh = 1 << 0,
    ImGuiWindowRefreshFlags_RefreshOnHover    = 1 << 1,
    ImGuiWindowRefreshFlags_RefreshOnFocus    = 1 << 2,
} ImGuiWindowRefreshFlags_;

typedef enum {
    ImGuiNextWindowDataFlags_None              = 0,
    ImGuiNextWindowDataFlags_HasPos            = 1 << 0,
    ImGuiNextWindowDataFlags_HasSize           = 1 << 1,
    ImGuiNextWindowDataFlags_HasContentSize    = 1 << 2,
    ImGuiNextWindowDataFlags_HasCollapsed      = 1 << 3,
    ImGuiNextWindowDataFlags_HasSizeConstraint = 1 << 4,
    ImGuiNextWindowDataFlags_HasFocus          = 1 << 5,
    ImGuiNextWindowDataFlags_HasBgAlpha        = 1 << 6,
    ImGuiNextWindowDataFlags_HasScroll         = 1 << 7,
    ImGuiNextWindowDataFlags_HasChildFlags     = 1 << 8,
    ImGuiNextWindowDataFlags_HasRefreshPolicy  = 1 << 9,
    ImGuiNextWindowDataFlags_HasViewport       = 1 << 10,
    ImGuiNextWindowDataFlags_HasDock           = 1 << 11,
    ImGuiNextWindowDataFlags_HasWindowClass    = 1 << 12,
} ImGuiNextWindowDataFlags_;

struct ImGuiNextWindowData {
    ImGuiNextWindowDataFlags Flags;
    ImGuiCond                PosCond;
    ImGuiCond                SizeCond;
    ImGuiCond                CollapsedCond;
    ImGuiCond                DockCond;
    ImVec2                   PosVal;
    ImVec2                   PosPivotVal;
    ImVec2                   SizeVal;
    ImVec2                   ContentSizeVal;
    ImVec2                   ScrollVal;
    ImGuiChildFlags          ChildFlags;
    bool                     PosUndock;
    bool                     CollapsedVal;
    ImRect                   SizeConstraintRect;
    ImGuiSizeCallback        SizeCallback;
    void*                    SizeCallbackUserData;
    float                    BgAlphaVal;
    ImGuiID                  ViewportId;
    ImGuiID                  DockId;
    ImGuiWindowClass         WindowClass;
    ImVec2                   MenuBarOffsetMinVal;
    ImGuiWindowRefreshFlags  RefreshFlagsVal;
};

typedef ImS64 ImGuiSelectionUserData;

typedef enum {
    ImGuiNextItemDataFlags_None        = 0,
    ImGuiNextItemDataFlags_HasWidth    = 1 << 0,
    ImGuiNextItemDataFlags_HasOpen     = 1 << 1,
    ImGuiNextItemDataFlags_HasShortcut = 1 << 2,
    ImGuiNextItemDataFlags_HasRefVal   = 1 << 3,
} ImGuiNextItemDataFlags_;

struct ImGuiNextItemData {
    ImGuiNextItemDataFlags Flags;
    ImGuiItemFlags         ItemFlags;
    ImGuiSelectionUserData SelectionUserData;
    float                  Width;
    ImGuiKeyChord          Shortcut;
    ImGuiInputFlags        ShortcutFlags;
    bool                   OpenVal;
    ImU8                   OpenCond;
    ImGuiDataTypeStorage   RefVal;
};

struct ImGuiLastItemData {
    ImGuiID              ID;
    ImGuiItemFlags       InFlags;
    ImGuiItemStatusFlags StatusFlags;
    ImRect               Rect;
    ImRect               NavRect;
    ImRect               DisplayRect;
    ImRect               ClipRect;
    ImGuiKeyChord        Shortcut;
};

struct ImGuiNavTreeNodeData {
    ImGuiID        ID;
    ImGuiItemFlags InFlags;
    ImRect         NavRect;
};

struct ImGuiStackSizes {
    short SizeOfIDStack;
    short SizeOfColorStack;
    short SizeOfStyleVarStack;
    short SizeOfFontStack;
    short SizeOfFocusScopeStack;
    short SizeOfGroupStack;
    short SizeOfItemFlagsStack;
    short SizeOfBeginPopupStack;
    short SizeOfDisabledStack;
};
typedef struct ImGuiWindowStackData ImGuiWindowStackData;

struct ImGuiWindowStackData {
    ImGuiWindow*      Window;
    ImGuiLastItemData ParentLastItemDataBackup;
    ImGuiStackSizes   StackSizesOnBegin;
    bool              DisabledOverrideReenable;
};
typedef struct ImGuiShrinkWidthItem ImGuiShrinkWidthItem;

struct ImGuiShrinkWidthItem {
    int   Index;
    float Width;
    float InitialWidth;
};
typedef struct ImGuiPtrOrIndex ImGuiPtrOrIndex;

struct ImGuiPtrOrIndex {
    void* Ptr;
    int   Index;
};

typedef enum {
    ImGuiPopupPositionPolicy_Default,
    ImGuiPopupPositionPolicy_ComboBox,
    ImGuiPopupPositionPolicy_Tooltip,
} ImGuiPopupPositionPolicy;

struct ImGuiPopupData {
    ImGuiID      PopupId;
    ImGuiWindow* Window;
    ImGuiWindow* RestoreNavWindow;
    int          ParentNavLayer;
    int          OpenFrameCount;
    ImGuiID      OpenParentId;
    ImVec2       OpenPopupPos;
    ImVec2       OpenMousePos;
};

typedef struct ImBitArray_ImGuiKey_NamedKey_COUNT__lessImGuiKey_NamedKey_BEGIN {
    ImU32 Storage[(ImGuiKey_NamedKey_COUNT + 31) >> 5];
} ImBitArray_ImGuiKey_NamedKey_COUNT__lessImGuiKey_NamedKey_BEGIN;

typedef ImBitArray_ImGuiKey_NamedKey_COUNT__lessImGuiKey_NamedKey_BEGIN ImBitArrayForNamedKeys;

typedef enum {
    ImGuiInputEventType_None = 0,
    ImGuiInputEventType_MousePos,
    ImGuiInputEventType_MouseWheel,
    ImGuiInputEventType_MouseButton,
    ImGuiInputEventType_MouseViewport,
    ImGuiInputEventType_Key,
    ImGuiInputEventType_Text,
    ImGuiInputEventType_Focus,
    ImGuiInputEventType_COUNT
} ImGuiInputEventType;

typedef enum {
    ImGuiInputSource_None = 0,
    ImGuiInputSource_Mouse,
    ImGuiInputSource_Keyboard,
    ImGuiInputSource_Gamepad,
    ImGuiInputSource_COUNT
} ImGuiInputSource;
typedef struct ImGuiInputEventMousePos ImGuiInputEventMousePos;

struct ImGuiInputEventMousePos {
    float            PosX, PosY;
    ImGuiMouseSource MouseSource;
};
typedef struct ImGuiInputEventMouseWheel ImGuiInputEventMouseWheel;

struct ImGuiInputEventMouseWheel {
    float            WheelX, WheelY;
    ImGuiMouseSource MouseSource;
};
typedef struct ImGuiInputEventMouseButton ImGuiInputEventMouseButton;

struct ImGuiInputEventMouseButton {
    int              Button;
    bool             Down;
    ImGuiMouseSource MouseSource;
};
typedef struct ImGuiInputEventMouseViewport ImGuiInputEventMouseViewport;

struct ImGuiInputEventMouseViewport {
    ImGuiID HoveredViewportID;
};
typedef struct ImGuiInputEventKey ImGuiInputEventKey;

struct ImGuiInputEventKey {
    ImGuiKey Key;
    bool     Down;
    float    AnalogValue;
};
typedef struct ImGuiInputEventText ImGuiInputEventText;

struct ImGuiInputEventText {
    unsigned int Char;
};
typedef struct ImGuiInputEventAppFocused ImGuiInputEventAppFocused;

struct ImGuiInputEventAppFocused {
    bool Focused;
};
typedef struct ImGuiInputEvent ImGuiInputEvent;

struct ImGuiInputEvent {
    ImGuiInputEventType Type;
    ImGuiInputSource    Source;
    ImU32               EventId;

    union {
        ImGuiInputEventMousePos      MousePos;
        ImGuiInputEventMouseWheel    MouseWheel;
        ImGuiInputEventMouseButton   MouseButton;
        ImGuiInputEventMouseViewport MouseViewport;
        ImGuiInputEventKey           Key;
        ImGuiInputEventText          Text;
        ImGuiInputEventAppFocused    AppFocused;
    };

    bool AddedByTestEngine;
};

typedef ImS16                      ImGuiKeyRoutingIndex;
typedef struct ImGuiKeyRoutingData ImGuiKeyRoutingData;

struct ImGuiKeyRoutingData {
    ImGuiKeyRoutingIndex NextEntryIndex;
    ImU16                Mods;
    ImU8                 RoutingCurrScore;
    ImU8                 RoutingNextScore;
    ImGuiID              RoutingCurr;
    ImGuiID              RoutingNext;
};
typedef struct ImGuiKeyRoutingTable ImGuiKeyRoutingTable;

typedef struct ImVector_ImGuiKeyRoutingData {
    int                  Size;
    int                  Capacity;
    ImGuiKeyRoutingData* Data;
} ImVector_ImGuiKeyRoutingData;

struct ImGuiKeyRoutingTable {
    ImGuiKeyRoutingIndex         Index[ImGuiKey_NamedKey_COUNT];
    ImVector_ImGuiKeyRoutingData Entries;
    ImVector_ImGuiKeyRoutingData EntriesNext;
};
typedef struct ImGuiKeyOwnerData ImGuiKeyOwnerData;

struct ImGuiKeyOwnerData {
    ImGuiID OwnerCurr;
    ImGuiID OwnerNext;
    bool    LockThisFrame;
    bool    LockUntilRelease;
};

typedef enum {
    ImGuiInputFlags_RepeatRateDefault                = 1 << 1,
    ImGuiInputFlags_RepeatRateNavMove                = 1 << 2,
    ImGuiInputFlags_RepeatRateNavTweak               = 1 << 3,
    ImGuiInputFlags_RepeatUntilRelease               = 1 << 4,
    ImGuiInputFlags_RepeatUntilKeyModsChange         = 1 << 5,
    ImGuiInputFlags_RepeatUntilKeyModsChangeFromNone = 1 << 6,
    ImGuiInputFlags_RepeatUntilOtherKeyPress         = 1 << 7,
    ImGuiInputFlags_LockThisFrame                    = 1 << 20,
    ImGuiInputFlags_LockUntilRelease                 = 1 << 21,
    ImGuiInputFlags_CondHovered                      = 1 << 22,
    ImGuiInputFlags_CondActive                       = 1 << 23,
    ImGuiInputFlags_CondDefault_ = ImGuiInputFlags_CondHovered | ImGuiInputFlags_CondActive,
    ImGuiInputFlags_RepeatRateMask_ = ImGuiInputFlags_RepeatRateDefault | ImGuiInputFlags_RepeatRateNavMove | ImGuiInputFlags_RepeatRateNavTweak,
    ImGuiInputFlags_RepeatUntilMask_ = ImGuiInputFlags_RepeatUntilRelease | ImGuiInputFlags_RepeatUntilKeyModsChange | ImGuiInputFlags_RepeatUntilKeyModsChangeFromNone | ImGuiInputFlags_RepeatUntilOtherKeyPress,
    ImGuiInputFlags_RepeatMask_ = std::to_underlying(ImGuiInputFlags_Repeat) | ImGuiInputFlags_RepeatRateMask_ | ImGuiInputFlags_RepeatUntilMask_,
    ImGuiInputFlags_CondMask_ = ImGuiInputFlags_CondHovered | ImGuiInputFlags_CondActive,
    ImGuiInputFlags_RouteTypeMask_ = ImGuiInputFlags_RouteActive | ImGuiInputFlags_RouteFocused | ImGuiInputFlags_RouteGlobal | ImGuiInputFlags_RouteAlways,
    ImGuiInputFlags_RouteOptionsMask_ = ImGuiInputFlags_RouteOverFocused | ImGuiInputFlags_RouteOverActive | ImGuiInputFlags_RouteUnlessBgFocused | ImGuiInputFlags_RouteFromRootWindow,
    ImGuiInputFlags_SupportedByIsKeyPressed   = ImGuiInputFlags_RepeatMask_,
    ImGuiInputFlags_SupportedByIsMouseClicked = ImGuiInputFlags_Repeat,
    ImGuiInputFlags_SupportedByShortcut = ImGuiInputFlags_RepeatMask_ | ImGuiInputFlags_RouteTypeMask_ | ImGuiInputFlags_RouteOptionsMask_,
    ImGuiInputFlags_SupportedBySetNextItemShortcut = ImGuiInputFlags_RepeatMask_ | ImGuiInputFlags_RouteTypeMask_ | ImGuiInputFlags_RouteOptionsMask_ | ImGuiInputFlags_Tooltip,
    ImGuiInputFlags_SupportedBySetKeyOwner = ImGuiInputFlags_LockThisFrame | ImGuiInputFlags_LockUntilRelease,
    ImGuiInputFlags_SupportedBySetItemKeyOwner = ImGuiInputFlags_SupportedBySetKeyOwner | ImGuiInputFlags_CondMask_,
} ImGuiInputFlagsPrivate_;
typedef struct ImGuiListClipperRange ImGuiListClipperRange;

struct ImGuiListClipperRange {
    int  Min;
    int  Max;
    bool PosToIndexConvert;
    ImS8 PosToIndexOffsetMin;
    ImS8 PosToIndexOffsetMax;
};
typedef struct ImGuiListClipperData ImGuiListClipperData;

typedef struct ImVector_ImGuiListClipperRange {
    int                    Size;
    int                    Capacity;
    ImGuiListClipperRange* Data;
} ImVector_ImGuiListClipperRange;

struct ImGuiListClipperData {
    ImGuiListClipper*              ListClipper;
    float                          LossynessOffset;
    int                            StepNo;
    int                            ItemsFrozen;
    ImVector_ImGuiListClipperRange Ranges;
};

typedef enum {
    ImGuiActivateFlags_None               = 0,
    ImGuiActivateFlags_PreferInput        = 1 << 0,
    ImGuiActivateFlags_PreferTweak        = 1 << 1,
    ImGuiActivateFlags_TryToPreserveState = 1 << 2,
    ImGuiActivateFlags_FromTabbing        = 1 << 3,
    ImGuiActivateFlags_FromShortcut       = 1 << 4,
} ImGuiActivateFlags_;

typedef enum {
    ImGuiScrollFlags_None               = 0,
    ImGuiScrollFlags_KeepVisibleEdgeX   = 1 << 0,
    ImGuiScrollFlags_KeepVisibleEdgeY   = 1 << 1,
    ImGuiScrollFlags_KeepVisibleCenterX = 1 << 2,
    ImGuiScrollFlags_KeepVisibleCenterY = 1 << 3,
    ImGuiScrollFlags_AlwaysCenterX      = 1 << 4,
    ImGuiScrollFlags_AlwaysCenterY      = 1 << 5,
    ImGuiScrollFlags_NoScrollParent     = 1 << 6,
    ImGuiScrollFlags_MaskX_ = ImGuiScrollFlags_KeepVisibleEdgeX | ImGuiScrollFlags_KeepVisibleCenterX | ImGuiScrollFlags_AlwaysCenterX,
    ImGuiScrollFlags_MaskY_ = ImGuiScrollFlags_KeepVisibleEdgeY | ImGuiScrollFlags_KeepVisibleCenterY | ImGuiScrollFlags_AlwaysCenterY,
} ImGuiScrollFlags_;

typedef enum {
    ImGuiNavHighlightFlags_None       = 0,
    ImGuiNavHighlightFlags_Compact    = 1 << 1,
    ImGuiNavHighlightFlags_AlwaysDraw = 1 << 2,
    ImGuiNavHighlightFlags_NoRounding = 1 << 3,
} ImGuiNavHighlightFlags_;

typedef enum {
    ImGuiNavMoveFlags_None  = 0,
    ImGuiNavMoveFlags_LoopX = 1 << 0,
    ImGuiNavMoveFlags_LoopY = 1 << 1,
    ImGuiNavMoveFlags_WrapX = 1 << 2,
    ImGuiNavMoveFlags_WrapY = 1 << 3,
    ImGuiNavMoveFlags_WrapMask_ = ImGuiNavMoveFlags_LoopX | ImGuiNavMoveFlags_LoopY | ImGuiNavMoveFlags_WrapX | ImGuiNavMoveFlags_WrapY,
    ImGuiNavMoveFlags_AllowCurrentNavId   = 1 << 4,
    ImGuiNavMoveFlags_AlsoScoreVisibleSet = 1 << 5,
    ImGuiNavMoveFlags_ScrollToEdgeY       = 1 << 6,
    ImGuiNavMoveFlags_Forwarded           = 1 << 7,
    ImGuiNavMoveFlags_DebugNoResult       = 1 << 8,
    ImGuiNavMoveFlags_FocusApi            = 1 << 9,
    ImGuiNavMoveFlags_IsTabbing           = 1 << 10,
    ImGuiNavMoveFlags_IsPageMove          = 1 << 11,
    ImGuiNavMoveFlags_Activate            = 1 << 12,
    ImGuiNavMoveFlags_NoSelect            = 1 << 13,
    ImGuiNavMoveFlags_NoSetNavHighlight   = 1 << 14,
    ImGuiNavMoveFlags_NoClearActiveId     = 1 << 15,
} ImGuiNavMoveFlags_;

typedef enum {
    ImGuiNavLayer_Main = 0,
    ImGuiNavLayer_Menu = 1,
    ImGuiNavLayer_COUNT
} ImGuiNavLayer;

struct ImGuiNavItemData {
    ImGuiWindow*           Window;
    ImGuiID                ID;
    ImGuiID                FocusScopeId;
    ImRect                 RectRel;
    ImGuiItemFlags         InFlags;
    float                  DistBox;
    float                  DistCenter;
    float                  DistAxial;
    ImGuiSelectionUserData SelectionUserData;
};
typedef struct ImGuiFocusScopeData ImGuiFocusScopeData;

struct ImGuiFocusScopeData {
    ImGuiID ID;
    ImGuiID WindowID;
};

typedef enum {
    ImGuiTypingSelectFlags_None                = 0,
    ImGuiTypingSelectFlags_AllowBackspace      = 1 << 0,
    ImGuiTypingSelectFlags_AllowSingleCharMode = 1 << 1,
} ImGuiTypingSelectFlags_;

struct ImGuiTypingSelectRequest {
    ImGuiTypingSelectFlags Flags;
    int                    SearchBufferLen;
    const char*            SearchBuffer;
    bool                   SelectRequest;
    bool                   SingleCharMode;
    ImS8                   SingleCharSize;
};

struct ImGuiTypingSelectState {
    ImGuiTypingSelectRequest Request;
    char                     SearchBuffer[64];
    ImGuiID                  FocusScope;
    int                      LastRequestFrame;
    float                    LastRequestTime;
    bool                     SingleCharModeLock;
};

typedef enum {
    ImGuiOldColumnFlags_None                   = 0,
    ImGuiOldColumnFlags_NoBorder               = 1 << 0,
    ImGuiOldColumnFlags_NoResize               = 1 << 1,
    ImGuiOldColumnFlags_NoPreserveWidths       = 1 << 2,
    ImGuiOldColumnFlags_NoForceWithinWindow    = 1 << 3,
    ImGuiOldColumnFlags_GrowParentContentsSize = 1 << 4,
} ImGuiOldColumnFlags_;

struct ImGuiOldColumnData {
    float               OffsetNorm;
    float               OffsetNormBeforeResize;
    ImGuiOldColumnFlags Flags;
    ImRect              ClipRect;
};

typedef struct ImVector_ImGuiOldColumnData {
    int                 Size;
    int                 Capacity;
    ImGuiOldColumnData* Data;
} ImVector_ImGuiOldColumnData;

struct ImGuiOldColumns {
    ImGuiID                     ID;
    ImGuiOldColumnFlags         Flags;
    bool                        IsFirstFrame;
    bool                        IsBeingResized;
    int                         Current;
    int                         Count;
    float                       OffMinX, OffMaxX;
    float                       LineMinY, LineMaxY;
    float                       HostCursorPosY;
    float                       HostCursorMaxPosX;
    ImRect                      HostInitialClipRect;
    ImRect                      HostBackupClipRect;
    ImRect                      HostBackupParentWorkRect;
    ImVector_ImGuiOldColumnData Columns;
    ImDrawListSplitter          Splitter;
};

typedef enum {
    ImGuiDockNodeFlags_DockSpace                 = 1 << 10,
    ImGuiDockNodeFlags_CentralNode               = 1 << 11,
    ImGuiDockNodeFlags_NoTabBar                  = 1 << 12,
    ImGuiDockNodeFlags_HiddenTabBar              = 1 << 13,
    ImGuiDockNodeFlags_NoWindowMenuButton        = 1 << 14,
    ImGuiDockNodeFlags_NoCloseButton             = 1 << 15,
    ImGuiDockNodeFlags_NoResizeX                 = 1 << 16,
    ImGuiDockNodeFlags_NoResizeY                 = 1 << 17,
    ImGuiDockNodeFlags_DockedWindowsInFocusRoute = 1 << 18,
    ImGuiDockNodeFlags_NoDockingSplitOther       = 1 << 19,
    ImGuiDockNodeFlags_NoDockingOverMe           = 1 << 20,
    ImGuiDockNodeFlags_NoDockingOverOther        = 1 << 21,
    ImGuiDockNodeFlags_NoDockingOverEmpty        = 1 << 22,
    ImGuiDockNodeFlags_NoDocking = ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_NoDockingOverEmpty | ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoDockingSplitOther,
    ImGuiDockNodeFlags_SharedFlagsInheritMask_ = ~0,
    ImGuiDockNodeFlags_NoResizeFlagsMask_ = std::to_underlying(ImGuiDockNodeFlags_NoResize) | ImGuiDockNodeFlags_NoResizeX | ImGuiDockNodeFlags_NoResizeY,
    ImGuiDockNodeFlags_LocalFlagsTransferMask_ = std::to_underlying(ImGuiDockNodeFlags_NoDockingSplit) | ImGuiDockNodeFlags_NoResizeFlagsMask_ | ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_CentralNode | ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton,
    ImGuiDockNodeFlags_SavedFlagsMask_ = ImGuiDockNodeFlags_NoResizeFlagsMask_ | ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_CentralNode | ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton,
} ImGuiDockNodeFlagsPrivate_;

typedef enum {
    ImGuiDataAuthority_Auto,
    ImGuiDataAuthority_DockNode,
    ImGuiDataAuthority_Window,
} ImGuiDataAuthority_;

typedef enum {
    ImGuiDockNodeState_Unknown,
    ImGuiDockNodeState_HostWindowHiddenBecauseSingleWindow,
    ImGuiDockNodeState_HostWindowHiddenBecauseWindowsAreResizing,
    ImGuiDockNodeState_HostWindowVisible,
} ImGuiDockNodeState;

typedef struct ImVector_ImGuiWindowPtr {
    int           Size;
    int           Capacity;
    ImGuiWindow** Data;
} ImVector_ImGuiWindowPtr;

struct ImGuiDockNode {
    ImGuiID                 ID;
    ImGuiDockNodeFlags      SharedFlags;
    ImGuiDockNodeFlags      LocalFlags;
    ImGuiDockNodeFlags      LocalFlagsInWindows;
    ImGuiDockNodeFlags      MergedFlags;
    ImGuiDockNodeState      State;
    ImGuiDockNode*          ParentNode;
    ImGuiDockNode*          ChildNodes[2];
    ImVector_ImGuiWindowPtr Windows;
    ImGuiTabBar*            TabBar;
    ImVec2                  Pos;
    ImVec2                  Size;
    ImVec2                  SizeRef;
    ImGuiAxis               SplitAxis;
    ImGuiWindowClass        WindowClass;
    ImU32                   LastBgColor;
    ImGuiWindow*            HostWindow;
    ImGuiWindow*            VisibleWindow;
    ImGuiDockNode*          CentralNode;
    ImGuiDockNode*          OnlyNodeWithWindows;
    int                     CountNodeWithWindows;
    int                     LastFrameAlive;
    int                     LastFrameActive;
    int                     LastFrameFocused;
    ImGuiID                 LastFocusedNodeId;
    ImGuiID                 SelectedTabId;
    ImGuiID                 WantCloseTabId;
    ImGuiID                 RefViewportId;
    ImGuiDataAuthority      AuthorityForPos       : 3;
    ImGuiDataAuthority      AuthorityForSize      : 3;
    ImGuiDataAuthority      AuthorityForViewport  : 3;
    bool                    IsVisible             : 1;
    bool                    IsFocused             : 1;
    bool                    IsBgDrawnThisFrame    : 1;
    bool                    HasCloseButton        : 1;
    bool                    HasWindowMenuButton   : 1;
    bool                    HasCentralNodeChild   : 1;
    bool                    WantCloseAll          : 1;
    bool                    WantLockSizeOnce      : 1;
    bool                    WantMouseMove         : 1;
    bool                    WantHiddenTabBarUpdate: 1;
    bool                    WantHiddenTabBarToggle: 1;
};

typedef enum {
    ImGuiWindowDockStyleCol_Text,
    ImGuiWindowDockStyleCol_Tab,
    ImGuiWindowDockStyleCol_TabHovered,
    ImGuiWindowDockStyleCol_TabActive,
    ImGuiWindowDockStyleCol_TabUnfocused,
    ImGuiWindowDockStyleCol_TabUnfocusedActive,
    ImGuiWindowDockStyleCol_COUNT
} ImGuiWindowDockStyleCol;

struct ImGuiWindowDockStyle {
    ImU32 Colors[ImGuiWindowDockStyleCol_COUNT];
};

typedef struct ImVector_ImGuiDockRequest {
    int               Size;
    int               Capacity;
    ImGuiDockRequest* Data;
} ImVector_ImGuiDockRequest;

typedef struct ImVector_ImGuiDockNodeSettings {
    int                    Size;
    int                    Capacity;
    ImGuiDockNodeSettings* Data;
} ImVector_ImGuiDockNodeSettings;

struct ImGuiDockContext {
    ImGuiStorage                   Nodes;
    ImVector_ImGuiDockRequest      Requests;
    ImVector_ImGuiDockNodeSettings NodesSettings;
    bool                           WantFullRebuild;
};
typedef struct ImGuiViewportP ImGuiViewportP;

struct ImGuiViewportP {
    ImGuiViewport     _ImGuiViewport;
    ImGuiWindow*      Window;
    int               Idx;
    int               LastFrameActive;
    int               LastFocusedStampCount;
    ImGuiID           LastNameHash;
    ImVec2            LastPos;
    float             Alpha;
    float             LastAlpha;
    bool              LastFocusedHadNavWindow;
    short             PlatformMonitor;
    int               BgFgDrawListsLastFrame[2];
    ImDrawList*       BgFgDrawLists[2];
    ImDrawData        DrawDataP;
    ImDrawDataBuilder DrawDataBuilder;
    ImVec2            LastPlatformPos;
    ImVec2            LastPlatformSize;
    ImVec2            LastRendererSize;
    ImVec2            WorkOffsetMin;
    ImVec2            WorkOffsetMax;
    ImVec2            BuildWorkOffsetMin;
    ImVec2            BuildWorkOffsetMax;
};

struct ImGuiWindowSettings {
    ImGuiID  ID;
    ImVec2ih Pos;
    ImVec2ih Size;
    ImVec2ih ViewportPos;
    ImGuiID  ViewportId;
    ImGuiID  DockId;
    ImGuiID  ClassId;
    short    DockOrder;
    bool     Collapsed;
    bool     IsChild;
    bool     WantApply;
    bool     WantDelete;
};

struct ImGuiSettingsHandler {
    const char* TypeName;
    ImGuiID     TypeHash;
    void (*ClearAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler);
    void (*ReadInitFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler);
    void* (*ReadOpenFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name);
    void (*ReadLineFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, void* entry, const char* line);
    void (*ApplyAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler);
    void (*WriteAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf);
    void* UserData;
};

typedef enum {
    ImGuiLocKey_VersionStr                    = 0,
    ImGuiLocKey_TableSizeOne                  = 1,
    ImGuiLocKey_TableSizeAllFit               = 2,
    ImGuiLocKey_TableSizeAllDefault           = 3,
    ImGuiLocKey_TableResetOrder               = 4,
    ImGuiLocKey_WindowingMainMenuBar          = 5,
    ImGuiLocKey_WindowingPopup                = 6,
    ImGuiLocKey_WindowingUntitled             = 7,
    ImGuiLocKey_DockingHideTabBar             = 8,
    ImGuiLocKey_DockingHoldShiftToDock        = 9,
    ImGuiLocKey_DockingDragToUndockOrMoveNode = 10,
    ImGuiLocKey_COUNT                         = 11,
} ImGuiLocKey;

struct ImGuiLocEntry {
    ImGuiLocKey Key;
    const char* Text;
};

typedef enum {
    ImGuiDebugLogFlags_None              = 0,
    ImGuiDebugLogFlags_EventActiveId     = 1 << 0,
    ImGuiDebugLogFlags_EventFocus        = 1 << 1,
    ImGuiDebugLogFlags_EventPopup        = 1 << 2,
    ImGuiDebugLogFlags_EventNav          = 1 << 3,
    ImGuiDebugLogFlags_EventClipper      = 1 << 4,
    ImGuiDebugLogFlags_EventSelection    = 1 << 5,
    ImGuiDebugLogFlags_EventIO           = 1 << 6,
    ImGuiDebugLogFlags_EventInputRouting = 1 << 7,
    ImGuiDebugLogFlags_EventDocking      = 1 << 8,
    ImGuiDebugLogFlags_EventViewport     = 1 << 9,
    ImGuiDebugLogFlags_EventMask_ = ImGuiDebugLogFlags_EventActiveId | ImGuiDebugLogFlags_EventFocus | ImGuiDebugLogFlags_EventPopup | ImGuiDebugLogFlags_EventNav | ImGuiDebugLogFlags_EventClipper | ImGuiDebugLogFlags_EventSelection | ImGuiDebugLogFlags_EventIO | ImGuiDebugLogFlags_EventInputRouting | ImGuiDebugLogFlags_EventDocking | ImGuiDebugLogFlags_EventViewport,
    ImGuiDebugLogFlags_OutputToTTY        = 1 << 20,
    ImGuiDebugLogFlags_OutputToTestEngine = 1 << 21,
} ImGuiDebugLogFlags_;
typedef struct ImGuiDebugAllocEntry ImGuiDebugAllocEntry;

struct ImGuiDebugAllocEntry {
    int   FrameCount;
    ImS16 AllocCount;
    ImS16 FreeCount;
};
typedef struct ImGuiDebugAllocInfo ImGuiDebugAllocInfo;

struct ImGuiDebugAllocInfo {
    int                  TotalAllocCount;
    int                  TotalFreeCount;
    ImS16                LastEntriesIdx;
    ImGuiDebugAllocEntry LastEntriesBuf[6];
};

struct ImGuiMetricsConfig {
    bool    ShowDebugLog;
    bool    ShowIDStackTool;
    bool    ShowWindowsRects;
    bool    ShowWindowsBeginOrder;
    bool    ShowTablesRects;
    bool    ShowDrawCmdMesh;
    bool    ShowDrawCmdBoundingBoxes;
    bool    ShowTextEncodingViewer;
    bool    ShowAtlasTintedWithTextColor;
    bool    ShowDockingNodes;
    int     ShowWindowsRectsType;
    int     ShowTablesRectsType;
    int     HighlightMonitorIdx;
    ImGuiID HighlightViewportID;
};
typedef struct ImGuiStackLevelInfo ImGuiStackLevelInfo;

struct ImGuiStackLevelInfo {
    ImGuiID       ID;
    ImS8          QueryFrameCount;
    bool          QuerySuccess;
    ImGuiDataType DataType: 8;
    char          Desc[57];
};
typedef struct ImGuiIDStackTool ImGuiIDStackTool;

typedef struct ImVector_ImGuiStackLevelInfo {
    int                  Size;
    int                  Capacity;
    ImGuiStackLevelInfo* Data;
} ImVector_ImGuiStackLevelInfo;

struct ImGuiIDStackTool {
    int                          LastActiveFrame;
    int                          StackLevel;
    ImGuiID                      QueryId;
    ImVector_ImGuiStackLevelInfo Results;
    bool                         CopyToClipboardOnCtrlC;
    float                        CopyToClipboardLastTime;
};

typedef void (*ImGuiContextHookCallback)(ImGuiContext* ctx, ImGuiContextHook* hook);

typedef enum {
    ImGuiContextHookType_NewFramePre,
    ImGuiContextHookType_NewFramePost,
    ImGuiContextHookType_EndFramePre,
    ImGuiContextHookType_EndFramePost,
    ImGuiContextHookType_RenderPre,
    ImGuiContextHookType_RenderPost,
    ImGuiContextHookType_Shutdown,
    ImGuiContextHookType_PendingRemoval_
} ImGuiContextHookType;

struct ImGuiContextHook {
    ImGuiID                  HookId;
    ImGuiContextHookType     Type;
    ImGuiID                  Owner;
    ImGuiContextHookCallback Callback;
    void*                    UserData;
};

typedef struct ImVector_ImGuiInputEvent {
    int              Size;
    int              Capacity;
    ImGuiInputEvent* Data;
} ImVector_ImGuiInputEvent;

typedef struct ImVector_ImGuiWindowStackData {
    int                   Size;
    int                   Capacity;
    ImGuiWindowStackData* Data;
} ImVector_ImGuiWindowStackData;

typedef struct ImVector_ImGuiColorMod {
    int            Size;
    int            Capacity;
    ImGuiColorMod* Data;
} ImVector_ImGuiColorMod;

typedef struct ImVector_ImGuiStyleMod {
    int            Size;
    int            Capacity;
    ImGuiStyleMod* Data;
} ImVector_ImGuiStyleMod;

typedef struct ImVector_ImGuiFocusScopeData {
    int                  Size;
    int                  Capacity;
    ImGuiFocusScopeData* Data;
} ImVector_ImGuiFocusScopeData;

typedef struct ImVector_ImGuiItemFlags {
    int             Size;
    int             Capacity;
    ImGuiItemFlags* Data;
} ImVector_ImGuiItemFlags;

typedef struct ImVector_ImGuiGroupData {
    int             Size;
    int             Capacity;
    ImGuiGroupData* Data;
} ImVector_ImGuiGroupData;

typedef struct ImVector_ImGuiPopupData {
    int             Size;
    int             Capacity;
    ImGuiPopupData* Data;
} ImVector_ImGuiPopupData;

typedef struct ImVector_ImGuiNavTreeNodeData {
    int                   Size;
    int                   Capacity;
    ImGuiNavTreeNodeData* Data;
} ImVector_ImGuiNavTreeNodeData;

typedef struct ImVector_ImGuiViewportPPtr {
    int              Size;
    int              Capacity;
    ImGuiViewportP** Data;
} ImVector_ImGuiViewportPPtr;

typedef struct ImVector_unsigned_char {
    int            Size;
    int            Capacity;
    unsigned char* Data;
} ImVector_unsigned_char;

typedef struct ImVector_ImGuiListClipperData {
    int                   Size;
    int                   Capacity;
    ImGuiListClipperData* Data;
} ImVector_ImGuiListClipperData;

typedef struct ImVector_ImGuiTableTempData {
    int                 Size;
    int                 Capacity;
    ImGuiTableTempData* Data;
} ImVector_ImGuiTableTempData;

typedef struct ImVector_ImGuiTable {
    int         Size;
    int         Capacity;
    ImGuiTable* Data;
} ImVector_ImGuiTable;

typedef struct ImPool_ImGuiTable {
    ImVector_ImGuiTable Buf;
    ImGuiStorage        Map;
    ImPoolIdx           FreeIdx;
    ImPoolIdx           AliveCount;
} ImPool_ImGuiTable;

typedef struct ImVector_ImGuiTabBar {
    int          Size;
    int          Capacity;
    ImGuiTabBar* Data;
} ImVector_ImGuiTabBar;

typedef struct ImPool_ImGuiTabBar {
    ImVector_ImGuiTabBar Buf;
    ImGuiStorage         Map;
    ImPoolIdx            FreeIdx;
    ImPoolIdx            AliveCount;
} ImPool_ImGuiTabBar;

typedef struct ImVector_ImGuiPtrOrIndex {
    int              Size;
    int              Capacity;
    ImGuiPtrOrIndex* Data;
} ImVector_ImGuiPtrOrIndex;

typedef struct ImVector_ImGuiShrinkWidthItem {
    int                   Size;
    int                   Capacity;
    ImGuiShrinkWidthItem* Data;
} ImVector_ImGuiShrinkWidthItem;

typedef struct ImVector_ImGuiID {
    int      Size;
    int      Capacity;
    ImGuiID* Data;
} ImVector_ImGuiID;

typedef struct ImVector_ImGuiSettingsHandler {
    int                   Size;
    int                   Capacity;
    ImGuiSettingsHandler* Data;
} ImVector_ImGuiSettingsHandler;

typedef struct ImChunkStream_ImGuiWindowSettings {
    ImVector_char Buf;
} ImChunkStream_ImGuiWindowSettings;

typedef struct ImChunkStream_ImGuiTableSettings {
    ImVector_char Buf;
} ImChunkStream_ImGuiTableSettings;

typedef struct ImVector_ImGuiContextHook {
    int               Size;
    int               Capacity;
    ImGuiContextHook* Data;
} ImVector_ImGuiContextHook;

struct ImGuiContext {
    bool                           Initialized;
    bool                           FontAtlasOwnedByContext;
    ImGuiIO                        IO;
    ImGuiPlatformIO                PlatformIO;
    ImGuiStyle                     Style;
    ImGuiConfigFlags               ConfigFlagsCurrFrame;
    ImGuiConfigFlags               ConfigFlagsLastFrame;
    ImFont*                        Font;
    float                          FontSize;
    float                          FontBaseSize;
    float                          CurrentDpiScale;
    ImDrawListSharedData           DrawListSharedData;
    double                         Time;
    int                            FrameCount;
    int                            FrameCountEnded;
    int                            FrameCountPlatformEnded;
    int                            FrameCountRendered;
    bool                           WithinFrameScope;
    bool                           WithinFrameScopeWithImplicitWindow;
    bool                           WithinEndChild;
    bool                           GcCompactAll;
    bool                           TestEngineHookItems;
    void*                          TestEngine;
    ImVector_ImGuiInputEvent       InputEventsQueue;
    ImVector_ImGuiInputEvent       InputEventsTrail;
    ImGuiMouseSource               InputEventsNextMouseSource;
    ImU32                          InputEventsNextEventId;
    ImVector_ImGuiWindowPtr        Windows;
    ImVector_ImGuiWindowPtr        WindowsFocusOrder;
    ImVector_ImGuiWindowPtr        WindowsTempSortBuffer;
    ImVector_ImGuiWindowStackData  CurrentWindowStack;
    ImGuiStorage                   WindowsById;
    int                            WindowsActiveCount;
    ImVec2                         WindowsHoverPadding;
    ImGuiID                        DebugBreakInWindow;
    ImGuiWindow*                   CurrentWindow;
    ImGuiWindow*                   HoveredWindow;
    ImGuiWindow*                   HoveredWindowUnderMovingWindow;
    ImGuiWindow*                   MovingWindow;
    ImGuiWindow*                   WheelingWindow;
    ImVec2                         WheelingWindowRefMousePos;
    int                            WheelingWindowStartFrame;
    int                            WheelingWindowScrolledFrame;
    float                          WheelingWindowReleaseTimer;
    ImVec2                         WheelingWindowWheelRemainder;
    ImVec2                         WheelingAxisAvg;
    ImGuiID                        DebugHookIdInfo;
    ImGuiID                        HoveredId;
    ImGuiID                        HoveredIdPreviousFrame;
    float                          HoveredIdTimer;
    float                          HoveredIdNotActiveTimer;
    bool                           HoveredIdAllowOverlap;
    bool                           HoveredIdDisabled;
    bool                           ItemUnclipByLog;
    ImGuiID                        ActiveId;
    ImGuiID                        ActiveIdIsAlive;
    float                          ActiveIdTimer;
    bool                           ActiveIdIsJustActivated;
    bool                           ActiveIdAllowOverlap;
    bool                           ActiveIdNoClearOnFocusLoss;
    bool                           ActiveIdHasBeenPressedBefore;
    bool                           ActiveIdHasBeenEditedBefore;
    bool                           ActiveIdHasBeenEditedThisFrame;
    bool                           ActiveIdFromShortcut;
    int                            ActiveIdMouseButton: 8;
    ImVec2                         ActiveIdClickOffset;
    ImGuiWindow*                   ActiveIdWindow;
    ImGuiInputSource               ActiveIdSource;
    ImGuiID                        ActiveIdPreviousFrame;
    bool                           ActiveIdPreviousFrameIsAlive;
    bool                           ActiveIdPreviousFrameHasBeenEditedBefore;
    ImGuiWindow*                   ActiveIdPreviousFrameWindow;
    ImGuiID                        LastActiveId;
    float                          LastActiveIdTimer;
    double                         LastKeyModsChangeTime;
    double                         LastKeyModsChangeFromNoneTime;
    double                         LastKeyboardKeyPressTime;
    ImBitArrayForNamedKeys         KeysMayBeCharInput;
    ImGuiKeyOwnerData              KeysOwnerData[ImGuiKey_NamedKey_COUNT];
    ImGuiKeyRoutingTable           KeysRoutingTable;
    ImU32                          ActiveIdUsingNavDirMask;
    bool                           ActiveIdUsingAllKeyboardKeys;
    ImGuiKeyChord                  DebugBreakInShortcutRouting;
    ImGuiID                        CurrentFocusScopeId;
    ImGuiItemFlags                 CurrentItemFlags;
    ImGuiID                        DebugLocateId;
    ImGuiNextItemData              NextItemData;
    ImGuiLastItemData              LastItemData;
    ImGuiNextWindowData            NextWindowData;
    bool                           DebugShowGroupRects;
    ImGuiCol                       DebugFlashStyleColorIdx;
    ImVector_ImGuiColorMod         ColorStack;
    ImVector_ImGuiStyleMod         StyleVarStack;
    ImVector_ImFontPtr             FontStack;
    ImVector_ImGuiFocusScopeData   FocusScopeStack;
    ImVector_ImGuiItemFlags        ItemFlagsStack;
    ImVector_ImGuiGroupData        GroupStack;
    ImVector_ImGuiPopupData        OpenPopupStack;
    ImVector_ImGuiPopupData        BeginPopupStack;
    ImVector_ImGuiNavTreeNodeData  NavTreeNodeStack;
    ImVector_ImGuiViewportPPtr     Viewports;
    ImGuiViewportP*                CurrentViewport;
    ImGuiViewportP*                MouseViewport;
    ImGuiViewportP*                MouseLastHoveredViewport;
    ImGuiID                        PlatformLastFocusedViewportId;
    ImGuiPlatformMonitor           FallbackMonitor;
    ImRect                         PlatformMonitorsFullWorkRect;
    int                            ViewportCreatedCount;
    int                            PlatformWindowsCreatedCount;
    int                            ViewportFocusedStampCount;
    ImGuiWindow*                   NavWindow;
    ImGuiID                        NavId;
    ImGuiID                        NavFocusScopeId;
    ImGuiID                        NavActivateId;
    ImGuiID                        NavActivateDownId;
    ImGuiID                        NavActivatePressedId;
    ImGuiActivateFlags             NavActivateFlags;
    ImVector_ImGuiFocusScopeData   NavFocusRoute;
    ImGuiID                        NavHighlightActivatedId;
    float                          NavHighlightActivatedTimer;
    ImGuiID                        NavJustMovedToId;
    ImGuiID                        NavJustMovedToFocusScopeId;
    ImGuiKeyChord                  NavJustMovedToKeyMods;
    ImGuiID                        NavNextActivateId;
    ImGuiActivateFlags             NavNextActivateFlags;
    ImGuiInputSource               NavInputSource;
    ImGuiNavLayer                  NavLayer;
    ImGuiSelectionUserData         NavLastValidSelectionUserData;
    bool                           NavIdIsAlive;
    bool                           NavMousePosDirty;
    bool                           NavDisableHighlight;
    bool                           NavDisableMouseHover;
    bool                           NavAnyRequest;
    bool                           NavInitRequest;
    bool                           NavInitRequestFromMove;
    ImGuiNavItemData               NavInitResult;
    bool                           NavMoveSubmitted;
    bool                           NavMoveScoringItems;
    bool                           NavMoveForwardToNextFrame;
    ImGuiNavMoveFlags              NavMoveFlags;
    ImGuiScrollFlags               NavMoveScrollFlags;
    ImGuiKeyChord                  NavMoveKeyMods;
    ImGuiDir                       NavMoveDir;
    ImGuiDir                       NavMoveDirForDebug;
    ImGuiDir                       NavMoveClipDir;
    ImRect                         NavScoringRect;
    ImRect                         NavScoringNoClipRect;
    int                            NavScoringDebugCount;
    int                            NavTabbingDir;
    int                            NavTabbingCounter;
    ImGuiNavItemData               NavMoveResultLocal;
    ImGuiNavItemData               NavMoveResultLocalVisible;
    ImGuiNavItemData               NavMoveResultOther;
    ImGuiNavItemData               NavTabbingResultFirst;
    ImGuiKeyChord                  ConfigNavWindowingKeyNext;
    ImGuiKeyChord                  ConfigNavWindowingKeyPrev;
    ImGuiWindow*                   NavWindowingTarget;
    ImGuiWindow*                   NavWindowingTargetAnim;
    ImGuiWindow*                   NavWindowingListWindow;
    float                          NavWindowingTimer;
    float                          NavWindowingHighlightAlpha;
    bool                           NavWindowingToggleLayer;
    ImGuiKey                       NavWindowingToggleKey;
    ImVec2                         NavWindowingAccumDeltaPos;
    ImVec2                         NavWindowingAccumDeltaSize;
    float                          DimBgRatio;
    bool                           DragDropActive;
    bool                           DragDropWithinSource;
    bool                           DragDropWithinTarget;
    ImGuiDragDropFlags             DragDropSourceFlags;
    int                            DragDropSourceFrameCount;
    int                            DragDropMouseButton;
    ImGuiPayload                   DragDropPayload;
    ImRect                         DragDropTargetRect;
    ImRect                         DragDropTargetClipRect;
    ImGuiID                        DragDropTargetId;
    ImGuiDragDropFlags             DragDropAcceptFlags;
    float                          DragDropAcceptIdCurrRectSurface;
    ImGuiID                        DragDropAcceptIdCurr;
    ImGuiID                        DragDropAcceptIdPrev;
    int                            DragDropAcceptFrameCount;
    ImGuiID                        DragDropHoldJustPressedId;
    ImVector_unsigned_char         DragDropPayloadBufHeap;
    unsigned char                  DragDropPayloadBufLocal[16];
    int                            ClipperTempDataStacked;
    ImVector_ImGuiListClipperData  ClipperTempData;
    ImGuiTable*                    CurrentTable;
    ImGuiID                        DebugBreakInTable;
    int                            TablesTempDataStacked;
    ImVector_ImGuiTableTempData    TablesTempData;
    ImPool_ImGuiTable              Tables;
    ImVector_float                 TablesLastTimeActive;
    ImVector_ImDrawChannel         DrawChannelsTempMergeBuffer;
    ImGuiTabBar*                   CurrentTabBar;
    ImPool_ImGuiTabBar             TabBars;
    ImVector_ImGuiPtrOrIndex       CurrentTabBarStack;
    ImVector_ImGuiShrinkWidthItem  ShrinkWidthBuffer;
    ImGuiID                        HoverItemDelayId;
    ImGuiID                        HoverItemDelayIdPreviousFrame;
    float                          HoverItemDelayTimer;
    float                          HoverItemDelayClearTimer;
    ImGuiID                        HoverItemUnlockedStationaryId;
    ImGuiID                        HoverWindowUnlockedStationaryId;
    ImGuiMouseCursor               MouseCursor;
    float                          MouseStationaryTimer;
    ImVec2                         MouseLastValidPos;
    ImGuiInputTextState            InputTextState;
    ImGuiInputTextDeactivatedState InputTextDeactivatedState;
    ImFont                         InputTextPasswordFont;
    ImGuiID                        TempInputId;
    ImGuiDataTypeStorage           DataTypeZeroValue;
    int                            BeginMenuDepth;
    int                            BeginComboDepth;
    ImGuiColorEditFlags            ColorEditOptions;
    ImGuiID                        ColorEditCurrentID;
    ImGuiID                        ColorEditSavedID;
    float                          ColorEditSavedHue;
    float                          ColorEditSavedSat;
    ImU32                          ColorEditSavedColor;
    ImVec4                         ColorPickerRef;
    ImGuiComboPreviewData          ComboPreviewData;
    ImRect                         WindowResizeBorderExpectedRect;
    bool                           WindowResizeRelativeMode;
    short                          ScrollbarSeekMode;
    float                          ScrollbarClickDeltaToGrabCenter;
    float                          SliderGrabClickOffset;
    float                          SliderCurrentAccum;
    bool                           SliderCurrentAccumDirty;
    bool                           DragCurrentAccumDirty;
    float                          DragCurrentAccum;
    float                          DragSpeedDefaultRatio;
    float                          DisabledAlphaBackup;
    short                          DisabledStackSize;
    short                          LockMarkEdited;
    short                          TooltipOverrideCount;
    ImVector_char                  ClipboardHandlerData;
    ImVector_ImGuiID               MenusIdSubmittedThisFrame;
    ImGuiTypingSelectState         TypingSelectState;
    ImGuiPlatformImeData           PlatformImeData;
    ImGuiPlatformImeData           PlatformImeDataPrev;
    ImGuiID                        PlatformImeViewport;
    ImGuiDockContext               DockContext;
    void (*DockNodeWindowMenuHandler)(ImGuiContext* ctx, ImGuiDockNode* node, ImGuiTabBar* tab_bar);
    bool                              SettingsLoaded;
    float                             SettingsDirtyTimer;
    ImGuiTextBuffer                   SettingsIniData;
    ImVector_ImGuiSettingsHandler     SettingsHandlers;
    ImChunkStream_ImGuiWindowSettings SettingsWindows;
    ImChunkStream_ImGuiTableSettings  SettingsTables;
    ImVector_ImGuiContextHook         Hooks;
    ImGuiID                           HookIdNext;
    const char*                       LocalizationTable[ImGuiLocKey_COUNT];
    bool                              LogEnabled;
    ImGuiLogType                      LogType;
    ImFileHandle                      LogFile;
    ImGuiTextBuffer                   LogBuffer;
    const char*                       LogNextPrefix;
    const char*                       LogNextSuffix;
    float                             LogLinePosY;
    bool                              LogLineFirstItem;
    int                               LogDepthRef;
    int                               LogDepthToExpand;
    int                               LogDepthToExpandDefault;
    ImGuiDebugLogFlags                DebugLogFlags;
    ImGuiTextBuffer                   DebugLogBuf;
    ImGuiTextIndex                    DebugLogIndex;
    ImGuiDebugLogFlags                DebugLogAutoDisableFlags;
    ImU8                              DebugLogAutoDisableFrames;
    ImU8                              DebugLocateFrames;
    bool                              DebugBreakInLocateId;
    ImGuiKeyChord                     DebugBreakKeyChord;
    ImS8                              DebugBeginReturnValueCullDepth;
    bool                              DebugItemPickerActive;
    ImU8                              DebugItemPickerMouseButton;
    ImGuiID                           DebugItemPickerBreakId;
    float                             DebugFlashStyleColorTime;
    ImVec4                            DebugFlashStyleColorBackup;
    ImGuiMetricsConfig                DebugMetricsConfig;
    ImGuiIDStackTool                  DebugIDStackTool;
    ImGuiDebugAllocInfo               DebugAllocInfo;
    ImGuiDockNode*                    DebugHoveredDockNode;
    float                             FramerateSecPerFrame[60];
    int                               FramerateSecPerFrameIdx;
    int                               FramerateSecPerFrameCount;
    float                             FramerateSecPerFrameAccum;
    int                               WantCaptureMouseNextFrame;
    int                               WantCaptureKeyboardNextFrame;
    int                               WantTextInputNextFrame;
    ImVector_char                     TempBuffer;
    char                              TempKeychordName[64];
};

struct ImGuiWindowTempData {
    ImVec2                  CursorPos;
    ImVec2                  CursorPosPrevLine;
    ImVec2                  CursorStartPos;
    ImVec2                  CursorMaxPos;
    ImVec2                  IdealMaxPos;
    ImVec2                  CurrLineSize;
    ImVec2                  PrevLineSize;
    float                   CurrLineTextBaseOffset;
    float                   PrevLineTextBaseOffset;
    bool                    IsSameLine;
    bool                    IsSetPos;
    ImVec1                  Indent;
    ImVec1                  ColumnsOffset;
    ImVec1                  GroupOffset;
    ImVec2                  CursorStartPosLossyness;
    ImGuiNavLayer           NavLayerCurrent;
    short                   NavLayersActiveMask;
    short                   NavLayersActiveMaskNext;
    bool                    NavIsScrollPushableX;
    bool                    NavHideHighlightOneFrame;
    bool                    NavWindowHasScrollY;
    bool                    MenuBarAppending;
    ImVec2                  MenuBarOffset;
    ImGuiMenuColumns        MenuColumns;
    int                     TreeDepth;
    ImU32                   TreeJumpToParentOnPopMask;
    ImVector_ImGuiWindowPtr ChildWindows;
    ImGuiStorage*           StateStorage;
    ImGuiOldColumns*        CurrentColumns;
    int                     CurrentTableIdx;
    ImGuiLayoutType         LayoutType;
    ImGuiLayoutType         ParentLayoutType;
    ImU32                   ModalDimBgColor;
    float                   ItemWidth;
    float                   TextWrapPos;
    ImVector_float          ItemWidthStack;
    ImVector_float          TextWrapPosStack;
};

typedef struct ImVector_ImGuiOldColumns {
    int              Size;
    int              Capacity;
    ImGuiOldColumns* Data;
} ImVector_ImGuiOldColumns;

struct ImGuiWindow {
    ImGuiContext*            Ctx;
    char*                    Name;
    ImGuiID                  ID;
    ImGuiWindowFlags         Flags, FlagsPreviousFrame;
    ImGuiChildFlags          ChildFlags;
    ImGuiWindowClass         WindowClass;
    ImGuiViewportP*          Viewport;
    ImGuiID                  ViewportId;
    ImVec2                   ViewportPos;
    int                      ViewportAllowPlatformMonitorExtend;
    ImVec2                   Pos;
    ImVec2                   Size;
    ImVec2                   SizeFull;
    ImVec2                   ContentSize;
    ImVec2                   ContentSizeIdeal;
    ImVec2                   ContentSizeExplicit;
    ImVec2                   WindowPadding;
    float                    WindowRounding;
    float                    WindowBorderSize;
    float                    TitleBarHeight, MenuBarHeight;
    float                    DecoOuterSizeX1, DecoOuterSizeY1;
    float                    DecoOuterSizeX2, DecoOuterSizeY2;
    float                    DecoInnerSizeX1, DecoInnerSizeY1;
    int                      NameBufLen;
    ImGuiID                  MoveId;
    ImGuiID                  TabId;
    ImGuiID                  ChildId;
    ImGuiID                  PopupId;
    ImVec2                   Scroll;
    ImVec2                   ScrollMax;
    ImVec2                   ScrollTarget;
    ImVec2                   ScrollTargetCenterRatio;
    ImVec2                   ScrollTargetEdgeSnapDist;
    ImVec2                   ScrollbarSizes;
    bool                     ScrollbarX, ScrollbarY;
    bool                     ViewportOwned;
    bool                     Active;
    bool                     WasActive;
    bool                     WriteAccessed;
    bool                     Collapsed;
    bool                     WantCollapseToggle;
    bool                     SkipItems;
    bool                     SkipRefresh;
    bool                     Appearing;
    bool                     Hidden;
    bool                     IsFallbackWindow;
    bool                     IsExplicitChild;
    bool                     HasCloseButton;
    signed char              ResizeBorderHovered;
    signed char              ResizeBorderHeld;
    short                    BeginCount;
    short                    BeginCountPreviousFrame;
    short                    BeginOrderWithinParent;
    short                    BeginOrderWithinContext;
    short                    FocusOrder;
    ImS8                     AutoFitFramesX, AutoFitFramesY;
    bool                     AutoFitOnlyGrows;
    ImGuiDir                 AutoPosLastDirection;
    ImS8                     HiddenFramesCanSkipItems;
    ImS8                     HiddenFramesCannotSkipItems;
    ImS8                     HiddenFramesForRenderOnly;
    ImS8                     DisableInputsFrames;
    ImGuiCond                SetWindowPosAllowFlags      : 8;
    ImGuiCond                SetWindowSizeAllowFlags     : 8;
    ImGuiCond                SetWindowCollapsedAllowFlags: 8;
    ImGuiCond                SetWindowDockAllowFlags     : 8;
    ImVec2                   SetWindowPosVal;
    ImVec2                   SetWindowPosPivot;
    ImVector_ImGuiID         IDStack;
    ImGuiWindowTempData      DC;
    ImRect                   OuterRectClipped;
    ImRect                   InnerRect;
    ImRect                   InnerClipRect;
    ImRect                   WorkRect;
    ImRect                   ParentWorkRect;
    ImRect                   ClipRect;
    ImRect                   ContentRegionRect;
    ImVec2ih                 HitTestHoleSize;
    ImVec2ih                 HitTestHoleOffset;
    int                      LastFrameActive;
    int                      LastFrameJustFocused;
    float                    LastTimeActive;
    float                    ItemWidthDefault;
    ImGuiStorage             StateStorage;
    ImVector_ImGuiOldColumns ColumnsStorage;
    float                    FontWindowScale;
    float                    FontDpiScale;
    int                      SettingsOffset;
    ImDrawList*              DrawList;
    ImDrawList               DrawListInst;
    ImGuiWindow*             ParentWindow;
    ImGuiWindow*             ParentWindowInBeginStack;
    ImGuiWindow*             RootWindow;
    ImGuiWindow*             RootWindowPopupTree;
    ImGuiWindow*             RootWindowDockTree;
    ImGuiWindow*             RootWindowForTitleBarHighlight;
    ImGuiWindow*             RootWindowForNav;
    ImGuiWindow*             ParentWindowForFocusRoute;
    ImGuiWindow*             NavLastChildNavWindow;
    ImGuiID                  NavLastIds[ImGuiNavLayer_COUNT];
    ImRect                   NavRectRel[ImGuiNavLayer_COUNT];
    ImVec2                   NavPreferredScoringPosRel[ImGuiNavLayer_COUNT];
    ImGuiID                  NavRootFocusScopeId;
    int                      MemoryDrawListIdxCapacity;
    int                      MemoryDrawListVtxCapacity;
    bool                     MemoryCompacted;
    bool                     DockIsActive     : 1;
    bool                     DockNodeIsVisible: 1;
    bool                     DockTabIsVisible : 1;
    bool                     DockTabWantClose : 1;
    short                    DockOrder;
    ImGuiWindowDockStyle     DockStyle;
    ImGuiDockNode*           DockNode;
    ImGuiDockNode*           DockNodeAsHost;
    ImGuiID                  DockId;
    ImGuiItemStatusFlags     DockTabItemStatusFlags;
    ImRect                   DockTabItemRect;
};

typedef enum {
    ImGuiTabBarFlags_DockNode     = 1 << 20,
    ImGuiTabBarFlags_IsFocused    = 1 << 21,
    ImGuiTabBarFlags_SaveSettings = 1 << 22,
} ImGuiTabBarFlagsPrivate_;

typedef enum {
    ImGuiTabItemFlags_SectionMask_  = ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_Trailing,
    ImGuiTabItemFlags_NoCloseButton = 1 << 20,
    ImGuiTabItemFlags_Button        = 1 << 21,
    ImGuiTabItemFlags_Unsorted      = 1 << 22,
} ImGuiTabItemFlagsPrivate_;

struct ImGuiTabItem {
    ImGuiID           ID;
    ImGuiTabItemFlags Flags;
    ImGuiWindow*      Window;
    int               LastFrameVisible;
    int               LastFrameSelected;
    float             Offset;
    float             Width;
    float             ContentWidth;
    float             RequestedWidth;
    ImS32             NameOffset;
    ImS16             BeginOrder;
    ImS16             IndexDuringLayout;
    bool              WantClose;
};

typedef struct ImVector_ImGuiTabItem {
    int           Size;
    int           Capacity;
    ImGuiTabItem* Data;
} ImVector_ImGuiTabItem;

struct ImGuiTabBar {
    ImVector_ImGuiTabItem Tabs;
    ImGuiTabBarFlags      Flags;
    ImGuiID               ID;
    ImGuiID               SelectedTabId;
    ImGuiID               NextSelectedTabId;
    ImGuiID               VisibleTabId;
    int                   CurrFrameVisible;
    int                   PrevFrameVisible;
    ImRect                BarRect;
    float                 CurrTabsContentsHeight;
    float                 PrevTabsContentsHeight;
    float                 WidthAllTabs;
    float                 WidthAllTabsIdeal;
    float                 ScrollingAnim;
    float                 ScrollingTarget;
    float                 ScrollingTargetDistToVisibility;
    float                 ScrollingSpeed;
    float                 ScrollingRectMinX;
    float                 ScrollingRectMaxX;
    float                 SeparatorMinX;
    float                 SeparatorMaxX;
    ImGuiID               ReorderRequestTabId;
    ImS16                 ReorderRequestOffset;
    ImS8                  BeginCount;
    bool                  WantLayout;
    bool                  VisibleTabWasSubmitted;
    bool                  TabsAddedNew;
    ImS16                 TabsActiveCount;
    ImS16                 LastTabItemIdx;
    float                 ItemSpacingY;
    ImVec2                FramePadding;
    ImVec2                BackupCursorPos;
    ImGuiTextBuffer       TabsNames;
};

typedef ImS16 ImGuiTableColumnIdx;
typedef ImU16 ImGuiTableDrawChannelIdx;

struct ImGuiTableColumn {
    ImGuiTableColumnFlags    Flags;
    float                    WidthGiven;
    float                    MinX;
    float                    MaxX;
    float                    WidthRequest;
    float                    WidthAuto;
    float                    StretchWeight;
    float                    InitStretchWeightOrWidth;
    ImRect                   ClipRect;
    ImGuiID                  UserID;
    float                    WorkMinX;
    float                    WorkMaxX;
    float                    ItemWidth;
    float                    ContentMaxXFrozen;
    float                    ContentMaxXUnfrozen;
    float                    ContentMaxXHeadersUsed;
    float                    ContentMaxXHeadersIdeal;
    ImS16                    NameOffset;
    ImGuiTableColumnIdx      DisplayOrder;
    ImGuiTableColumnIdx      IndexWithinEnabledSet;
    ImGuiTableColumnIdx      PrevEnabledColumn;
    ImGuiTableColumnIdx      NextEnabledColumn;
    ImGuiTableColumnIdx      SortOrder;
    ImGuiTableDrawChannelIdx DrawChannelCurrent;
    ImGuiTableDrawChannelIdx DrawChannelFrozen;
    ImGuiTableDrawChannelIdx DrawChannelUnfrozen;
    bool                     IsEnabled;
    bool                     IsUserEnabled;
    bool                     IsUserEnabledNextFrame;
    bool                     IsVisibleX;
    bool                     IsVisibleY;
    bool                     IsRequestOutput;
    bool                     IsSkipItems;
    bool                     IsPreserveWidthAuto;
    ImS8                     NavLayerCurrent;
    ImU8                     AutoFitQueue;
    ImU8                     CannotSkipItemsQueue;
    ImU8                     SortDirection           : 2;
    ImU8                     SortDirectionsAvailCount: 2;
    ImU8                     SortDirectionsAvailMask : 4;
    ImU8                     SortDirectionsAvailList;
};
typedef struct ImGuiTableCellData ImGuiTableCellData;

struct ImGuiTableCellData {
    ImU32               BgColor;
    ImGuiTableColumnIdx Column;
};

struct ImGuiTableHeaderData {
    ImGuiTableColumnIdx Index;
    ImU32               TextColor;
    ImU32               BgColor0;
    ImU32               BgColor1;
};

struct ImGuiTableInstanceData {
    ImGuiID TableInstanceID;
    float   LastOuterHeight;
    float   LastTopHeadersRowHeight;
    float   LastFrozenHeight;
    int     HoveredRowLast;
    int     HoveredRowNext;
};

typedef struct ImSpan_ImGuiTableColumn {
    ImGuiTableColumn* Data;
    ImGuiTableColumn* DataEnd;
} ImSpan_ImGuiTableColumn;

typedef struct ImSpan_ImGuiTableColumnIdx {
    ImGuiTableColumnIdx* Data;
    ImGuiTableColumnIdx* DataEnd;
} ImSpan_ImGuiTableColumnIdx;

typedef struct ImSpan_ImGuiTableCellData {
    ImGuiTableCellData* Data;
    ImGuiTableCellData* DataEnd;
} ImSpan_ImGuiTableCellData;

typedef struct ImVector_ImGuiTableInstanceData {
    int                     Size;
    int                     Capacity;
    ImGuiTableInstanceData* Data;
} ImVector_ImGuiTableInstanceData;

typedef struct ImVector_ImGuiTableColumnSortSpecs {
    int                        Size;
    int                        Capacity;
    ImGuiTableColumnSortSpecs* Data;
} ImVector_ImGuiTableColumnSortSpecs;

struct ImGuiTable {
    ImGuiID                            ID;
    ImGuiTableFlags                    Flags;
    void*                              RawData;
    ImGuiTableTempData*                TempData;
    ImSpan_ImGuiTableColumn            Columns;
    ImSpan_ImGuiTableColumnIdx         DisplayOrderToIndex;
    ImSpan_ImGuiTableCellData          RowCellData;
    ImBitArrayPtr                      EnabledMaskByDisplayOrder;
    ImBitArrayPtr                      EnabledMaskByIndex;
    ImBitArrayPtr                      VisibleMaskByIndex;
    ImGuiTableFlags                    SettingsLoadedFlags;
    int                                SettingsOffset;
    int                                LastFrameActive;
    int                                ColumnsCount;
    int                                CurrentRow;
    int                                CurrentColumn;
    ImS16                              InstanceCurrent;
    ImS16                              InstanceInteracted;
    float                              RowPosY1;
    float                              RowPosY2;
    float                              RowMinHeight;
    float                              RowCellPaddingY;
    float                              RowTextBaseline;
    float                              RowIndentOffsetX;
    ImGuiTableRowFlags                 RowFlags    : 16;
    ImGuiTableRowFlags                 LastRowFlags: 16;
    int                                RowBgColorCounter;
    ImU32                              RowBgColor[2];
    ImU32                              BorderColorStrong;
    ImU32                              BorderColorLight;
    float                              BorderX1;
    float                              BorderX2;
    float                              HostIndentX;
    float                              MinColumnWidth;
    float                              OuterPaddingX;
    float                              CellPaddingX;
    float                              CellSpacingX1;
    float                              CellSpacingX2;
    float                              InnerWidth;
    float                              ColumnsGivenWidth;
    float                              ColumnsAutoFitWidth;
    float                              ColumnsStretchSumWeights;
    float                              ResizedColumnNextWidth;
    float                              ResizeLockMinContentsX2;
    float                              RefScale;
    float                              AngledHeadersHeight;
    float                              AngledHeadersSlope;
    ImRect                             OuterRect;
    ImRect                             InnerRect;
    ImRect                             WorkRect;
    ImRect                             InnerClipRect;
    ImRect                             BgClipRect;
    ImRect                             Bg0ClipRectForDrawCmd;
    ImRect                             Bg2ClipRectForDrawCmd;
    ImRect                             HostClipRect;
    ImRect                             HostBackupInnerClipRect;
    ImGuiWindow*                       OuterWindow;
    ImGuiWindow*                       InnerWindow;
    ImGuiTextBuffer                    ColumnsNames;
    ImDrawListSplitter*                DrawSplitter;
    ImGuiTableInstanceData             InstanceDataFirst;
    ImVector_ImGuiTableInstanceData    InstanceDataExtra;
    ImGuiTableColumnSortSpecs          SortSpecsSingle;
    ImVector_ImGuiTableColumnSortSpecs SortSpecsMulti;
    ImGuiTableSortSpecs                SortSpecs;
    ImGuiTableColumnIdx                SortSpecsCount;
    ImGuiTableColumnIdx                ColumnsEnabledCount;
    ImGuiTableColumnIdx                ColumnsEnabledFixedCount;
    ImGuiTableColumnIdx                DeclColumnsCount;
    ImGuiTableColumnIdx                AngledHeadersCount;
    ImGuiTableColumnIdx                HoveredColumnBody;
    ImGuiTableColumnIdx                HoveredColumnBorder;
    ImGuiTableColumnIdx                HighlightColumnHeader;
    ImGuiTableColumnIdx                AutoFitSingleColumn;
    ImGuiTableColumnIdx                ResizedColumn;
    ImGuiTableColumnIdx                LastResizedColumn;
    ImGuiTableColumnIdx                HeldHeaderColumn;
    ImGuiTableColumnIdx                ReorderColumn;
    ImGuiTableColumnIdx                ReorderColumnDir;
    ImGuiTableColumnIdx                LeftMostEnabledColumn;
    ImGuiTableColumnIdx                RightMostEnabledColumn;
    ImGuiTableColumnIdx                LeftMostStretchedColumn;
    ImGuiTableColumnIdx                RightMostStretchedColumn;
    ImGuiTableColumnIdx                ContextPopupColumn;
    ImGuiTableColumnIdx                FreezeRowsRequest;
    ImGuiTableColumnIdx                FreezeRowsCount;
    ImGuiTableColumnIdx                FreezeColumnsRequest;
    ImGuiTableColumnIdx                FreezeColumnsCount;
    ImGuiTableColumnIdx                RowCellDataCurrent;
    ImGuiTableDrawChannelIdx           DummyDrawChannel;
    ImGuiTableDrawChannelIdx           Bg2DrawChannelCurrent;
    ImGuiTableDrawChannelIdx           Bg2DrawChannelUnfrozen;
    bool                               IsLayoutLocked;
    bool                               IsInsideRow;
    bool                               IsInitializing;
    bool                               IsSortSpecsDirty;
    bool                               IsUsingHeaders;
    bool                               IsContextPopupOpen;
    bool                               DisableDefaultContextMenu;
    bool                               IsSettingsRequestLoad;
    bool                               IsSettingsDirty;
    bool                               IsDefaultDisplayOrder;
    bool                               IsResetAllRequest;
    bool                               IsResetDisplayOrderRequest;
    bool                               IsUnfrozenRows;
    bool                               IsDefaultSizingPolicy;
    bool                               IsActiveIdAliveBeforeTable;
    bool                               IsActiveIdInTable;
    bool                               HasScrollbarYCurr;
    bool                               HasScrollbarYPrev;
    bool                               MemoryCompacted;
    bool                               HostSkipItems;
};

typedef struct ImVector_ImGuiTableHeaderData {
    int                   Size;
    int                   Capacity;
    ImGuiTableHeaderData* Data;
} ImVector_ImGuiTableHeaderData;

struct ImGuiTableTempData {
    int                           TableIndex;
    float                         LastTimeActive;
    float                         AngledHeadersExtraWidth;
    ImVector_ImGuiTableHeaderData AngledHeadersRequests;
    ImVec2                        UserOuterSize;
    ImDrawListSplitter            DrawSplitter;
    ImRect                        HostBackupWorkRect;
    ImRect                        HostBackupParentWorkRect;
    ImVec2                        HostBackupPrevLineSize;
    ImVec2                        HostBackupCurrLineSize;
    ImVec2                        HostBackupCursorMaxPos;
    ImVec1                        HostBackupColumnsOffset;
    float                         HostBackupItemWidth;
    int                           HostBackupItemWidthStackSize;
};
typedef struct ImGuiTableColumnSettings ImGuiTableColumnSettings;

struct ImGuiTableColumnSettings {
    float               WidthOrWeight;
    ImGuiID             UserID;
    ImGuiTableColumnIdx Index;
    ImGuiTableColumnIdx DisplayOrder;
    ImGuiTableColumnIdx SortOrder;
    ImU8                SortDirection: 2;
    ImU8                IsEnabled    : 1;
    ImU8                IsStretch    : 1;
};

struct ImGuiTableSettings {
    ImGuiID             ID;
    ImGuiTableFlags     SaveFlags;
    float               RefScale;
    ImGuiTableColumnIdx ColumnsCount;
    ImGuiTableColumnIdx ColumnsCountMax;
    bool                WantApply;
};

struct ImFontBuilderIO {
    bool (*FontBuilder_Build)(ImFontAtlas* atlas);
};

#pragma endregion

namespace ImGui {

    namespace ImVec2Manager {

        inline ImVec2* Create(void) {
            using func_t           = ImVec2* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec2"
                                                                                            "_ImVec"
                                                                                            "2_"
                                                                                            "Nil"));
            return func();
        }

        inline void Destroy(ImVec2* self) {
            using func_t            = void (*)(ImVec2*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec"
                                                                                             "2_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImVec2* Create(float _x, float _y) {
            using func_t           = ImVec2* (*)(float, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec2"
                                                                                            "_ImVec"
                                                                                            "2_"
                                                                                            "Floa"
                                                                                            "t"));
            return func(_x, _y);
        }
    }  // namespace ImVec2Manager

    namespace ImVec4Manager {
        inline ImVec4* Create(void) {
            using func_t           = ImVec4* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec4"
                                                                                            "_ImVec"
                                                                                            "4_"
                                                                                            "Nil"));
            return func();
        }

        inline void Destroy(ImVec4* self) {
            using func_t            = void (*)(ImVec4*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec"
                                                                                             "4_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImVec4* Create(float _x, float _y, float _z, float _w) {
            using func_t           = ImVec4* (*)(float, float, float, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec4"
                                                                                            "_ImVec"
                                                                                            "4_"
                                                                                            "Floa"
                                                                                            "t"));
            return func(_x, _y, _z, _w);
        }
    }  // namespace ImVec4Manager

    inline ImGuiContext* CreateContext(ImFontAtlas* shared_font_atlas = nullptr) {
        using func_t           = ImGuiContext* (*)(ImFontAtlas*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCreateCo"
                                                                                        "ntext"));
        return func(shared_font_atlas);
    }

    inline void DestroyContext(ImGuiContext* ctx = nullptr) {
        using func_t            = void (*)(ImGuiContext*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDestroy"
                                                                                         "Contex"
                                                                                         "t"));
        return func(ctx);
    }

    inline ImGuiContext* GetCurrentContext() {
        using func_t           = ImGuiContext* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurre"
                                                                                        "ntContex"
                                                                                        "t"));
        return func();
    }

    inline void SetCurrentContext(ImGuiContext* ctx) {
        using func_t            = void (*)(ImGuiContext*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetCurr"
                                                                                         "entContex"
                                                                                         "t"));
        return func(ctx);
    }

    inline ImGuiIO* GetIO() {
        using func_t           = ImGuiIO* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetIO"));
        return func();
    }

    inline ImGuiStyle* GetStyle() {
        using func_t           = ImGuiStyle* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetStyl"
                                                                                        "e"));
        return func();
    }

    inline void NewFrame() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNewFram"
                                                                                         "e"));
        return func();
    }

    inline void EndFrame() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndFram"
                                                                                         "e"));
        return func();
    }

    inline void Render() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRende"
                                                                                         "r"));
        return func();
    }

    inline ImDrawData* GetDrawData() {
        using func_t           = ImDrawData* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetDrawD"
                                                                                        "ata"));
        return func();
    }

    inline void ShowDemoWindow(bool* p_open = nullptr) {
        using func_t            = void (*)(bool*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShowDem"
                                                                                         "oWindo"
                                                                                         "w"));
        return func(p_open);
    }

    inline void ShowMetricsWindow(bool* p_open = nullptr) {
        using func_t            = void (*)(bool*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShowMet"
                                                                                         "ricsWindo"
                                                                                         "w"));
        return func(p_open);
    }

    inline void ShowDebugLogWindow(bool* p_open = nullptr) {
        using func_t            = void (*)(bool*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShowDeb"
                                                                                         "ugLogWind"
                                                                                         "ow"));
        return func(p_open);
    }

    inline void ShowIDStackToolWindow(bool* p_open = nullptr) {
        using func_t            = void (*)(bool*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShowIDS"
                                                                                         "tackToolW"
                                                                                         "indow"));
        return func(p_open);
    }

    inline void ShowAboutWindow(bool* p_open = nullptr) {
        using func_t            = void (*)(bool*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShowAbo"
                                                                                         "utWindo"
                                                                                         "w"));
        return func(p_open);
    }

    inline void ShowStyleEditor(ImGuiStyle* ref = nullptr) {
        using func_t            = void (*)(ImGuiStyle*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShowSty"
                                                                                         "leEdito"
                                                                                         "r"));
        return func(ref);
    }

    inline bool ShowStyleSelector(const char* label) {
        using func_t            = bool (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShowSty"
                                                                                         "leSelecto"
                                                                                         "r"));
        return func(label);
    }

    inline void ShowFontSelector(const char* label) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShowFon"
                                                                                         "tSelecto"
                                                                                         "r"));
        return func(label);
    }

    inline void ShowUserGuide() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShowUse"
                                                                                         "rGuide"));
        return func();
    }

    inline const char* GetVersion() {
        using func_t           = const char* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetVersi"
                                                                                        "on"));
        return func();
    }

    inline void StyleColorsDark(ImGuiStyle* dst = nullptr) {
        using func_t            = void (*)(ImGuiStyle*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igStyleCo"
                                                                                         "lorsDar"
                                                                                         "k"));
        return func(dst);
    }

    inline void StyleColorsLight(ImGuiStyle* dst = nullptr) {
        using func_t            = void (*)(ImGuiStyle*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igStyleCo"
                                                                                         "lorsLigh"
                                                                                         "t"));
        return func(dst);
    }

    inline void StyleColorsClassic(ImGuiStyle* dst = nullptr) {
        using func_t            = void (*)(ImGuiStyle*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igStyleCo"
                                                                                         "lorsClass"
                                                                                         "ic"));
        return func(dst);
    }

    inline bool Begin(const char* name, bool* p_open, ImGuiWindowFlags flags) {
        using func_t            = bool (*)(const char*, bool*, ImGuiWindowFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBegi"
                                                                                         "n"));
        return func(name, p_open, flags);
    }

    inline void End() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEnd"));
        return func();
    }

    inline bool BeginChild(const char* str_id, const ImVec2 size = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0) {
        using func_t = bool (*)(const char*, const ImVec2, ImGuiChildFlags, ImGuiWindowFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginCh"
                                                                                         "ild_"
                                                                                         "Str"));
        return func(str_id, size, child_flags, window_flags);
    }

    inline bool BeginChild(ImGuiID id, const ImVec2 size = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0) {
        using func_t = bool (*)(ImGuiID, const ImVec2, ImGuiChildFlags, ImGuiWindowFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginCh"
                                                                                         "ild_ID"));
        return func(id, size, child_flags, window_flags);
    }

    inline void EndChild() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndChil"
                                                                                         "d"));
        return func();
    }

    inline bool IsWindowAppearing() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsWindo"
                                                                                         "wAppearin"
                                                                                         "g"));
        return func();
    }

    inline bool IsWindowCollapsed() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsWindo"
                                                                                         "wCollapse"
                                                                                         "d"));
        return func();
    }

    inline bool IsWindowFocused(ImGuiFocusedFlags flags = 0) {
        using func_t            = bool (*)(ImGuiFocusedFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsWindo"
                                                                                         "wFocuse"
                                                                                         "d"));
        return func(flags);
    }

    inline bool IsWindowHovered(ImGuiHoveredFlags flags = 0) {
        using func_t            = bool (*)(ImGuiHoveredFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsWindo"
                                                                                         "wHovere"
                                                                                         "d"));
        return func(flags);
    }

    inline ImDrawList* GetWindowDrawList() {
        using func_t           = ImDrawList* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWindo"
                                                                                        "wDrawLis"
                                                                                        "t"));
        return func();
    }

    inline float GetWindowDpiScale() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWindo"
                                                                                        "wDpiScal"
                                                                                        "e"));
        return func();
    }

    inline void GetWindowPos(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWind"
                                                                                         "owPos"));
        return func(pOut);
    }

    inline void GetWindowSize(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWind"
                                                                                         "owSize"));
        return func(pOut);
    }

    inline float GetWindowWidth() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWindo"
                                                                                        "wWidth"));
        return func();
    }

    inline float GetWindowHeight() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWindo"
                                                                                        "wHeight"));
        return func();
    }

    inline ImGuiViewport* GetWindowViewport() {
        using func_t           = ImGuiViewport* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWindo"
                                                                                        "wViewpor"
                                                                                        "t"));
        return func();
    }

    inline void SetNextWindowPos(const ImVec2 pos, ImGuiCond cond, const ImVec2 pivot) {
        using func_t            = void (*)(const ImVec2, ImGuiCond, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowPo"
                                                                                         "s"));
        return func(pos, cond, pivot);
    }

    inline void SetNextWindowSize(const ImVec2 size, ImGuiCond cond = 0) {
        using func_t            = void (*)(const ImVec2, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowSiz"
                                                                                         "e"));
        return func(size, cond);
    }

    inline void SetNextWindowSizeConstraints(const ImVec2 size_min, const ImVec2 size_max, ImGuiSizeCallback custom_callback = nullptr, void* custom_callback_data = nullptr) {
        using func_t            = void (*)(const ImVec2, const ImVec2, ImGuiSizeCallback, void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowSiz"
                                                                                         "eConstrai"
                                                                                         "nts"));
        return func(size_min, size_max, custom_callback, custom_callback_data);
    }

    inline void SetNextWindowContentSize(const ImVec2 size) {
        using func_t            = void (*)(const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowCon"
                                                                                         "tentSiz"
                                                                                         "e"));
        return func(size);
    }

    inline void SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0) {
        using func_t            = void (*)(bool, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowCol"
                                                                                         "lapsed"));
        return func(collapsed, cond);
    }

    inline void SetNextWindowFocus() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowFoc"
                                                                                         "us"));
        return func();
    }

    inline void SetNextWindowScroll(const ImVec2 scroll) {
        using func_t            = void (*)(const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowScr"
                                                                                         "oll"));
        return func(scroll);
    }

    inline void SetNextWindowBgAlpha(float alpha) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowBgA"
                                                                                         "lpha"));
        return func(alpha);
    }

    inline void SetNextWindowViewport(ImGuiID viewport_id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowVie"
                                                                                         "wport"));
        return func(viewport_id);
    }

    inline void SetWindowPos(const ImVec2 pos, ImGuiCond cond = 0) {
        using func_t            = void (*)(const ImVec2, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owPos_"
                                                                                         "Vec2"));
        return func(pos, cond);
    }

    inline void SetWindowSize(const ImVec2 size, ImGuiCond cond = 0) {
        using func_t            = void (*)(const ImVec2, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owSize_"
                                                                                         "Vec2"));
        return func(size, cond);
    }

    inline void SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0) {
        using func_t            = void (*)(bool, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owCollaps"
                                                                                         "ed_"
                                                                                         "Bool"));
        return func(collapsed, cond);
    }

    inline void SetWindowFocus() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owFocus_"
                                                                                         "Nil"));
        return func();
    }

    inline void SetWindowFontScale(float scale) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owFontSca"
                                                                                         "le"));
        return func(scale);
    }

    inline void SetWindowPos(const char* name, const ImVec2 pos, ImGuiCond cond = 0) {
        using func_t            = void (*)(const char*, const ImVec2, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owPos_"
                                                                                         "Str"));
        return func(name, pos, cond);
    }

    inline void SetWindowSize(const char* name, const ImVec2 size, ImGuiCond cond = 0) {
        using func_t            = void (*)(const char*, const ImVec2, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owSize_"
                                                                                         "Str"));
        return func(name, size, cond);
    }

    inline void SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond = 0) {
        using func_t            = void (*)(const char*, bool, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owCollaps"
                                                                                         "ed_Str"));
        return func(name, collapsed, cond);
    }

    inline void SetWindowFocus(const char* name) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owFocus_"
                                                                                         "Str"));
        return func(name);
    }

    inline void GetContentRegionAvail(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCont"
                                                                                         "entRegion"
                                                                                         "Avail"));
        return func(pOut);
    }

    inline void GetContentRegionMax(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCont"
                                                                                         "entRegion"
                                                                                         "Max"));
        return func(pOut);
    }

    inline void GetWindowContentRegionMin(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWind"
                                                                                         "owContent"
                                                                                         "RegionMi"
                                                                                         "n"));
        return func(pOut);
    }

    inline void GetWindowContentRegionMax(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWind"
                                                                                         "owContent"
                                                                                         "RegionMa"
                                                                                         "x"));
        return func(pOut);
    }

    inline float GetScrollX() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetScrol"
                                                                                        "lX"));
        return func();
    }

    inline float GetScrollY() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetScrol"
                                                                                        "lY"));
        return func();
    }

    inline void SetScrollX(float scroll_x) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetScro"
                                                                                         "llX_"
                                                                                         "Float"));
        return func(scroll_x);
    }

    inline void SetScrollY(float scroll_y) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetScro"
                                                                                         "llY_"
                                                                                         "Float"));
        return func(scroll_y);
    }

    inline float GetScrollMaxX() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetScrol"
                                                                                        "lMaxX"));
        return func();
    }

    inline float GetScrollMaxY() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetScrol"
                                                                                        "lMaxY"));
        return func();
    }

    inline void SetScrollHereX(float center_x_ratio = 0.5f) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetScro"
                                                                                         "llHere"
                                                                                         "X"));
        return func(center_x_ratio);
    }

    inline void SetScrollHereY(float center_y_ratio = 0.5f) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetScro"
                                                                                         "llHere"
                                                                                         "Y"));
        return func(center_y_ratio);
    }

    inline void SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f) {
        using func_t            = void (*)(float, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetScro"
                                                                                         "llFromPos"
                                                                                         "X_"
                                                                                         "Float"));
        return func(local_x, center_x_ratio);
    }

    inline void SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f) {
        using func_t            = void (*)(float, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetScro"
                                                                                         "llFromPos"
                                                                                         "Y_"
                                                                                         "Float"));
        return func(local_y, center_y_ratio);
    }

    inline void PushFont(ImFont* font) {
        using func_t            = void (*)(ImFont*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushFon"
                                                                                         "t"));
        return func(font);
    }

    inline void PopFont() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopFon"
                                                                                         "t"));
        return func();
    }

    inline void PushStyleColor(ImGuiCol idx, ImU32 col) {
        using func_t            = void (*)(ImGuiCol, ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushSty"
                                                                                         "leColor_"
                                                                                         "U32"));
        return func(idx, col);
    }

    inline void PushStyleColor(ImGuiCol idx, const ImVec4 col) {
        using func_t            = void (*)(ImGuiCol, const ImVec4);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushSty"
                                                                                         "leColor_"
                                                                                         "Vec4"));
        return func(idx, col);
    }

    inline void PopStyleColor(int count = 1) {
        using func_t            = void (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopStyl"
                                                                                         "eColor"));
        return func(count);
    }

    inline void PushStyleVar(ImGuiStyleVar idx, float val) {
        using func_t            = void (*)(ImGuiStyleVar, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushSty"
                                                                                         "leVar_"
                                                                                         "Float"));
        return func(idx, val);
    }

    inline void PushStyleVar(ImGuiStyleVar idx, const ImVec2 val) {
        using func_t            = void (*)(ImGuiStyleVar, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushSty"
                                                                                         "leVar_"
                                                                                         "Vec2"));
        return func(idx, val);
    }

    inline void PopStyleVar(int count = 1) {
        using func_t            = void (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopStyl"
                                                                                         "eVar"));
        return func(count);
    }

    inline void PushTabStop(bool tab_stop) {
        using func_t            = void (*)(bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushTab"
                                                                                         "Stop"));
        return func(tab_stop);
    }

    inline void PopTabStop() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopTabS"
                                                                                         "top"));
        return func();
    }

    inline void PushButtonRepeat(bool repeat) {
        using func_t            = void (*)(bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushBut"
                                                                                         "tonRepea"
                                                                                         "t"));
        return func(repeat);
    }

    inline void PopButtonRepeat() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopButt"
                                                                                         "onRepea"
                                                                                         "t"));
        return func();
    }

    inline void PushItemWidth(float item_width) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushIte"
                                                                                         "mWidth"));
        return func(item_width);
    }

    inline void PopItemWidth() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopItem"
                                                                                         "Width"));
        return func();
    }

    inline void SetNextItemWidth(float item_width) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "ItemWidt"
                                                                                         "h"));
        return func(item_width);
    }

    inline float CalcItemWidth() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCalcItem"
                                                                                        "Width"));
        return func();
    }

    inline void PushTextWrapPos(float wrap_local_pos_x = 0.0f) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushTex"
                                                                                         "tWrapPo"
                                                                                         "s"));
        return func(wrap_local_pos_x);
    }

    inline void PopTextWrapPos() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopText"
                                                                                         "WrapPo"
                                                                                         "s"));
        return func();
    }

    inline ImFont* GetFont() {
        using func_t           = ImFont* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetFon"
                                                                                        "t"));
        return func();
    }

    inline float GetFontSize() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetFontS"
                                                                                        "ize"));
        return func();
    }

    inline void GetFontTexUvWhitePixel(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetFont"
                                                                                         "TexUvWhit"
                                                                                         "ePixel"));
        return func(pOut);
    }

    inline ImU32 GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f) {
        using func_t           = ImU32 (*)(ImGuiCol, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetColor"
                                                                                        "U32_Col"));
        return func(idx, alpha_mul);
    }

    inline ImU32 GetColorU32(const ImVec4 col) {
        using func_t           = ImU32 (*)(const ImVec4);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetColor"
                                                                                        "U32_"
                                                                                        "Vec4"));
        return func(col);
    }

    inline ImU32 GetColorU32(ImU32 col, float alpha_mul = 1.0f) {
        using func_t           = ImU32 (*)(ImU32, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetColor"
                                                                                        "U32_U32"));
        return func(col, alpha_mul);
    }

    inline const ImVec4* GetStyleColorVec4(ImGuiCol idx) {
        using func_t           = const ImVec4* (*)(ImGuiCol);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetStyle"
                                                                                        "ColorVec"
                                                                                        "4"));
        return func(idx);
    }

    inline void GetCursorScreenPos(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurs"
                                                                                         "orScreenP"
                                                                                         "os"));
        return func(pOut);
    }

    inline void SetCursorScreenPos(const ImVec2 pos) {
        using func_t            = void (*)(const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetCurs"
                                                                                         "orScreenP"
                                                                                         "os"));
        return func(pos);
    }

    inline void GetCursorPos(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurs"
                                                                                         "orPos"));
        return func(pOut);
    }

    inline float GetCursorPosX() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurso"
                                                                                        "rPosX"));
        return func();
    }

    inline float GetCursorPosY() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurso"
                                                                                        "rPosY"));
        return func();
    }

    inline void SetCursorPos(const ImVec2 local_pos) {
        using func_t            = void (*)(const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetCurs"
                                                                                         "orPos"));
        return func(local_pos);
    }

    inline void SetCursorPosX(float local_x) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetCurs"
                                                                                         "orPosX"));
        return func(local_x);
    }

    inline void SetCursorPosY(float local_y) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetCurs"
                                                                                         "orPosY"));
        return func(local_y);
    }

    inline void GetCursorStartPos(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurs"
                                                                                         "orStartPo"
                                                                                         "s"));
        return func(pOut);
    }

    inline void Separator() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSeparat"
                                                                                         "or"));
        return func();
    }

    inline void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f) {
        using func_t            = void (*)(float, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSameLin"
                                                                                         "e"));
        return func(offset_from_start_x, spacing);
    }

    inline void NewLine() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNewLin"
                                                                                         "e"));
        return func();
    }

    inline void Spacing() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSpacin"
                                                                                         "g"));
        return func();
    }

    inline void Dummy(const ImVec2 size) {
        using func_t            = void (*)(const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDumm"
                                                                                         "y"));
        return func(size);
    }

    inline void Indent(float indent_w = 0.0f) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInden"
                                                                                         "t"));
        return func(indent_w);
    }

    inline void Unindent(float indent_w = 0.0f) {
        using func_t            = void (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igUninden"
                                                                                         "t"));
        return func(indent_w);
    }

    inline void BeginGroup() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginGr"
                                                                                         "oup"));
        return func();
    }

    inline void EndGroup() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndGrou"
                                                                                         "p"));
        return func();
    }

    inline void AlignTextToFramePadding() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igAlignTe"
                                                                                         "xtToFrame"
                                                                                         "Paddin"
                                                                                         "g"));
        return func();
    }

    inline float GetTextLineHeight() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetTextL"
                                                                                        "ineHeigh"
                                                                                        "t"));
        return func();
    }

    inline float GetTextLineHeightWithSpacing() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetTextL"
                                                                                        "ineHeightW"
                                                                                        "ithSpacin"
                                                                                        "g"));
        return func();
    }

    inline float GetFrameHeight() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetFrame"
                                                                                        "Height"));
        return func();
    }

    inline float GetFrameHeightWithSpacing() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetFrame"
                                                                                        "HeightWith"
                                                                                        "Spacing"));
        return func();
    }

    inline void PushID(const char* str_id) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushID_"
                                                                                         "Str"));
        return func(str_id);
    }

    inline void PushID(const char* str_id_begin, const char* str_id_end) {
        using func_t            = void (*)(const char*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushID_"
                                                                                         "StrStr"));
        return func(str_id_begin, str_id_end);
    }

    inline void PushID(const void* ptr_id) {
        using func_t            = void (*)(const void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushID_"
                                                                                         "Ptr"));
        return func(ptr_id);
    }

    inline void PushID(int int_id) {
        using func_t            = void (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushID_"
                                                                                         "Int"));
        return func(int_id);
    }

    inline void PopID() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopI"
                                                                                         "D"));
        return func();
    }

    inline ImGuiID GetID(const char* str_id) {
        using func_t           = ImGuiID (*)(const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetID_"
                                                                                        "Str"));
        return func(str_id);
    }

    inline ImGuiID GetID(const char* str_id_begin, const char* str_id_end) {
        using func_t           = ImGuiID (*)(const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetID_"
                                                                                        "StrStr"));
        return func(str_id_begin, str_id_end);
    }

    inline ImGuiID GetID(const void* ptr_id) {
        using func_t           = ImGuiID (*)(const void*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetID_"
                                                                                        "Ptr"));
        return func(ptr_id);
    }

    inline void TextUnformatted(const char* text, const char* text_end = nullptr) {
        using func_t            = void (*)(const char*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTextUnf"
                                                                                         "ormatte"
                                                                                         "d"));
        return func(text, text_end);
    }

    inline void Text(const char* fmt, ...) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igText"
                                                                                         "V"));
        va_list            args;
        va_start(args, fmt);
        func(fmt, args);
        va_end(args);
    }

    inline void TextV(const char* fmt, va_list args) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igText"
                                                                                         "V"));
        return func(fmt, args);
    }

    inline void TextColored(const ImVec4 col, const char* fmt, ...) {
        using func_t            = void (*)(const ImVec4, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTextCol"
                                                                                         "oredV"));
        va_list            args;
        va_start(args, fmt);
        func(col, fmt, args);
        va_end(args);
    }

    inline void TextColoredV(const ImVec4 col, const char* fmt, va_list args) {
        using func_t            = void (*)(const ImVec4, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTextCol"
                                                                                         "oredV"));
        return func(col, fmt, args);
    }

    inline void TextDisabled(const char* fmt, ...) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTextDis"
                                                                                         "abledV"));
        va_list            args;
        va_start(args, fmt);
        func(fmt, args);
        va_end(args);
    }

    inline void TextDisabledV(const char* fmt, va_list args) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTextDis"
                                                                                         "abledV"));
        return func(fmt, args);
    }

    inline void TextWrapped(const char* fmt, ...) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTextWra"
                                                                                         "ppedV"));
        va_list            args;
        va_start(args, fmt);
        func(fmt, args);
        va_end(args);
    }

    inline void TextWrappedV(const char* fmt, va_list args) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTextWra"
                                                                                         "ppedV"));
        return func(fmt, args);
    }

    inline void LabelText(const char* label, const char* fmt, ...) {
        using func_t            = void (*)(const char*, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLabelTe"
                                                                                         "xtV"));
        va_list            args;
        va_start(args, fmt);
        func(label, fmt, args);
        va_end(args);
    }

    inline void LabelTextV(const char* label, const char* fmt, va_list args) {
        using func_t            = void (*)(const char*, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLabelTe"
                                                                                         "xtV"));
        return func(label, fmt, args);
    }

    inline void BulletText(const char* fmt, ...) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBulletT"
                                                                                         "extV"));
        va_list            args;
        va_start(args, fmt);
        func(fmt, args);
        va_end(args);
    }

    inline void BulletTextV(const char* fmt, va_list args) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBulletT"
                                                                                         "extV"));
        return func(fmt, args);
    }

    inline void SeparatorText(const char* label) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSeparat"
                                                                                         "orText"));
        return func(label);
    }

    inline bool Button(const char* label, const ImVec2 size = ImVec2(0, 0)) {
        using func_t            = bool (*)(const char*, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igButto"
                                                                                         "n"));
        return func(label, size);
    }

    inline bool SmallButton(const char* label) {
        using func_t            = bool (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSmallBu"
                                                                                         "tton"));
        return func(label);
    }

    inline bool InvisibleButton(const char* str_id, const ImVec2 size, ImGuiButtonFlags flags = 0) {
        using func_t            = bool (*)(const char*, const ImVec2, ImGuiButtonFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInvisib"
                                                                                         "leButto"
                                                                                         "n"));
        return func(str_id, size, flags);
    }

    inline bool ArrowButton(const char* str_id, ImGuiDir dir) {
        using func_t            = bool (*)(const char*, ImGuiDir);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igArrowBu"
                                                                                         "tton"));
        return func(str_id, dir);
    }

    inline bool Checkbox(const char* label, bool* v) {
        using func_t            = bool (*)(const char*, bool*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCheckbo"
                                                                                         "x"));
        return func(label, v);
    }

    inline bool CheckboxFlags(const char* label, int* flags, int flags_value) {
        using func_t            = bool (*)(const char*, int*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCheckbo"
                                                                                         "xFlags_"
                                                                                         "IntPtr"));
        return func(label, flags, flags_value);
    }

    inline bool CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value) {
        using func_t            = bool (*)(const char*, unsigned int*, unsigned int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCheckbo"
                                                                                         "xFlags_"
                                                                                         "UintPt"
                                                                                         "r"));
        return func(label, flags, flags_value);
    }

    inline bool RadioButton(const char* label, bool active) {
        using func_t            = bool (*)(const char*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRadioBu"
                                                                                         "tton_"
                                                                                         "Bool"));
        return func(label, active);
    }

    inline bool RadioButton(const char* label, int* v, int v_button) {
        using func_t            = bool (*)(const char*, int*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRadioBu"
                                                                                         "tton_"
                                                                                         "IntPtr"));
        return func(label, v, v_button);
    }

    inline void ProgressBar(float fraction, const ImVec2 size_arg, const char* overlay) {
        using func_t            = void (*)(float, const ImVec2, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igProgres"
                                                                                         "sBar"));
        return func(fraction, size_arg, overlay);
    }

    inline void Bullet() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBulle"
                                                                                         "t"));
        return func();
    }

    inline void Image(ImTextureID user_texture_id, const ImVec2 image_size, const ImVec2 uv0, const ImVec2 uv1, const ImVec4 tint_col, const ImVec4 border_col) {
        using func_t = void (*)(ImTextureID, const ImVec2, const ImVec2, const ImVec2, const ImVec4, const ImVec4);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImag"
                                                                                         "e"));
        return func(user_texture_id, image_size, uv0, uv1, tint_col, border_col);
    }

    inline bool ImageButton(const char* str_id, ImTextureID user_texture_id, const ImVec2 image_size, const ImVec2 uv0, const ImVec2 uv1, const ImVec4 bg_col, const ImVec4 tint_col) {
        using func_t = bool (*)(const char*, ImTextureID, const ImVec2, const ImVec2, const ImVec2, const ImVec4, const ImVec4);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImageBut"
                                                                                        "ton"));
        return func(str_id, user_texture_id, image_size, uv0, uv1, bg_col, tint_col);
    }

    inline bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0) {
        using func_t            = bool (*)(const char*, const char*, ImGuiComboFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginCo"
                                                                                         "mbo"));
        return func(label, preview_value, flags);
    }

    inline void EndCombo() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndComb"
                                                                                         "o"));
        return func();
    }

    inline bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1) {
        using func_t            = bool (*)(const char*, int*, const char* const[], int, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCombo_"
                                                                                         "Str_"
                                                                                         "arr"));
        return func(label, current_item, items, items_count, popup_max_height_in_items);
    }

    inline bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1) {
        using func_t            = bool (*)(const char*, int*, const char*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCombo_"
                                                                                         "Str"));
        return func(label, current_item, items_separated_by_zeros, popup_max_height_in_items);
    }

    inline bool Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1) {
        using func_t = bool (*)(const char*, int*, const char* (*)(void*, int), void*, int, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCombo_"
                                                                                         "FnStrPt"
                                                                                         "r"));
        return func(label, current_item, getter, user_data, items_count, popup_max_height_in_items);
    }

    inline bool DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, float*, float, float, float, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragFlo"
                                                                                         "at"));
        return func(label, v, v_speed, v_min, v_max, format, flags);
    }

    inline bool DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, float[2], float, float, float, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragFlo"
                                                                                         "at2"));
        return func(label, v, v_speed, v_min, v_max, format, flags);
    }

    inline bool DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, float[3], float, float, float, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragFlo"
                                                                                         "at3"));
        return func(label, v, v_speed, v_min, v_max, format, flags);
    }

    inline bool DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, float[4], float, float, float, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragFlo"
                                                                                         "at4"));
        return func(label, v, v_speed, v_min, v_max, format, flags);
    }

    inline bool DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = nullptr, ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, float*, float*, float, float, float, const char*, const char*, ImGuiSliderFlags);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragFloa"
                                                                                        "tRange2"));
        return func(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, flags);
    }

    inline bool DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, int*, float, int, int, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragIn"
                                                                                         "t"));
        return func(label, v, v_speed, v_min, v_max, format, flags);
    }

    inline bool DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, int[2], float, int, int, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragInt"
                                                                                         "2"));
        return func(label, v, v_speed, v_min, v_max, format, flags);
    }

    inline bool DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, int[3], float, int, int, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragInt"
                                                                                         "3"));
        return func(label, v, v_speed, v_min, v_max, format, flags);
    }

    inline bool DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, int[4], float, int, int, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragInt"
                                                                                         "4"));
        return func(label, v, v_speed, v_min, v_max, format, flags);
    }

    inline bool DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = nullptr, ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, int*, int*, float, int, int, const char*, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragInt"
                                                                                         "Range2"));
        return func(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, flags);
    }

    inline bool DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed = 1.0f, const void* p_min = nullptr, const void* p_max = nullptr, const char* format = nullptr, ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, ImGuiDataType, void*, float, const void*, const void*, const char*, ImGuiSliderFlags);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragScal"
                                                                                        "ar"));
        return func(label, data_type, p_data, v_speed, p_min, p_max, format, flags);
    }

    inline bool DragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags = 1.0f) {
        using func_t = bool (*)(const char*, ImGuiDataType, void*, int, float, const void*, const void*, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragSca"
                                                                                         "larN"));
        return func(label, data_type, p_data, components, v_speed, p_min, p_max, format, flags);
    }

    inline bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, float*, float, float, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderF"
                                                                                         "loat"));
        return func(label, v, v_min, v_max, format, flags);
    }

    inline bool SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, float[2], float, float, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderF"
                                                                                         "loat2"));
        return func(label, v, v_min, v_max, format, flags);
    }

    inline bool SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, float[3], float, float, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderF"
                                                                                         "loat3"));
        return func(label, v, v_min, v_max, format, flags);
    }

    inline bool SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, float[4], float, float, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderF"
                                                                                         "loat4"));
        return func(label, v, v_min, v_max, format, flags);
    }

    inline bool SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0f deg", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, float*, float, float, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderA"
                                                                                         "ngle"));
        return func(label, v_rad, v_degrees_min, v_degrees_max, format, flags);
    }

    inline bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, int*, int, int, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderI"
                                                                                         "nt"));
        return func(label, v, v_min, v_max, format, flags);
    }

    inline bool SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, int[2], int, int, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderI"
                                                                                         "nt2"));
        return func(label, v, v_min, v_max, format, flags);
    }

    inline bool SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, int[3], int, int, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderI"
                                                                                         "nt3"));
        return func(label, v, v_min, v_max, format, flags);
    }

    inline bool SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, int[4], int, int, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderI"
                                                                                         "nt4"));
        return func(label, v, v_min, v_max, format, flags);
    }

    inline bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags = 1.0f) {
        using func_t = bool (*)(const char*, ImGuiDataType, void*, const void*, const void*, const char*, ImGuiSliderFlags);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderSc"
                                                                                        "alar"));
        return func(label, data_type, p_data, p_min, p_max, format, flags);
    }

    inline bool SliderScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags = 1.0f) {
        using func_t = bool (*)(const char*, ImGuiDataType, void*, int, const void*, const void*, const char*, ImGuiSliderFlags);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderSc"
                                                                                        "alarN"));
        return func(label, data_type, p_data, components, p_min, p_max, format, flags);
    }

    inline bool VSliderFloat(const char* label, const ImVec2 size, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, const ImVec2, float*, float, float, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igVSlider"
                                                                                         "Float"));
        return func(label, size, v, v_min, v_max, format, flags);
    }

    inline bool VSliderInt(const char* label, const ImVec2 size, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, const ImVec2, int*, int, int, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igVSlider"
                                                                                         "Int"));
        return func(label, size, v, v_min, v_max, format, flags);
    }

    inline bool VSliderScalar(const char* label, const ImVec2 size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = nullptr, ImGuiSliderFlags flags = 0) {
        using func_t = bool (*)(const char*, const ImVec2, ImGuiDataType, void*, const void*, const void*, const char*, ImGuiSliderFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igVSlider"
                                                                                         "Scalar"));
        return func(label, size, data_type, p_data, p_min, p_max, format, flags);
    }

    inline bool InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr) {
        using func_t = bool (*)(const char*, char*, size_t, ImGuiInputTextFlags, ImGuiInputTextCallback, void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputTe"
                                                                                         "xt"));
        return func(label, buf, buf_size, flags, callback, user_data);
    }

    inline bool InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2 size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data) {
        using func_t = bool (*)(const char*, char*, size_t, const ImVec2, ImGuiInputTextFlags, ImGuiInputTextCallback, void*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputTex"
                                                                                        "tMultilin"
                                                                                        "e"));
        return func(label, buf, buf_size, size, flags, callback, user_data);
    }

    inline bool InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr) {
        using func_t = bool (*)(const char*, const char*, char*, size_t, ImGuiInputTextFlags, ImGuiInputTextCallback, void*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputTex"
                                                                                        "tWithHin"
                                                                                        "t"));
        return func(label, hint, buf, buf_size, flags, callback, user_data);
    }

    inline bool InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
        using func_t = bool (*)(const char*, float*, float, float, const char*, ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputFl"
                                                                                         "oat"));
        return func(label, v, step, step_fast, format, flags);
    }

    inline bool InputFloat2(const char* label, float v[2], const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
        using func_t            = bool (*)(const char*, float[2], const char*, ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputFl"
                                                                                         "oat2"));
        return func(label, v, format, flags);
    }

    inline bool InputFloat3(const char* label, float v[3], const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
        using func_t            = bool (*)(const char*, float[3], const char*, ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputFl"
                                                                                         "oat3"));
        return func(label, v, format, flags);
    }

    inline bool InputFloat4(const char* label, float v[4], const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
        using func_t            = bool (*)(const char*, float[4], const char*, ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputFl"
                                                                                         "oat4"));
        return func(label, v, format, flags);
    }

    inline bool InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0) {
        using func_t            = bool (*)(const char*, int*, int, int, ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputIn"
                                                                                         "t"));
        return func(label, v, step, step_fast, flags);
    }

    inline bool InputInt2(const char* label, int v[2], ImGuiInputTextFlags flags = 0) {
        using func_t            = bool (*)(const char*, int[2], ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputIn"
                                                                                         "t2"));
        return func(label, v, flags);
    }

    inline bool InputInt3(const char* label, int v[3], ImGuiInputTextFlags flags = 0) {
        using func_t            = bool (*)(const char*, int[3], ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputIn"
                                                                                         "t3"));
        return func(label, v, flags);
    }

    inline bool InputInt4(const char* label, int v[4], ImGuiInputTextFlags flags = 0) {
        using func_t            = bool (*)(const char*, int[4], ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputIn"
                                                                                         "t4"));
        return func(label, v, flags);
    }

    inline bool InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0) {
        using func_t = bool (*)(const char*, double*, double, double, const char*, ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputDo"
                                                                                         "uble"));
        return func(label, v, step, step_fast, format, flags);
    }

    inline bool InputScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step = nullptr, const void* p_step_fast = nullptr, const char* format = nullptr, ImGuiInputTextFlags flags = 0) {
        using func_t = bool (*)(const char*, ImGuiDataType, void*, const void*, const void*, const char*, ImGuiInputTextFlags);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputSca"
                                                                                        "lar"));
        return func(label, data_type, p_data, p_step, p_step_fast, format, flags);
    }

    inline bool InputScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_step = nullptr, const void* p_step_fast = nullptr, const char* format = nullptr, ImGuiInputTextFlags flags = 0) {
        using func_t = bool (*)(const char*, ImGuiDataType, void*, int, const void*, const void*, const char*, ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputSc"
                                                                                         "alarN"));
        return func(label, data_type, p_data, components, p_step, p_step_fast, format, flags);
    }

    inline bool ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0) {
        using func_t            = bool (*)(const char*, float, ImGuiColorEditFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorEd"
                                                                                         "it3"));
        return func(label, col[3], flags);
    }

    inline bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0) {
        using func_t            = bool (*)(const char*, float[4], ImGuiColorEditFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorEd"
                                                                                         "it4"));
        return func(label, col, flags);
    }

    inline bool ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0) {
        using func_t            = bool (*)(const char*, float[3], ImGuiColorEditFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorPi"
                                                                                         "cker3"));
        return func(label, col, flags);
    }

    inline bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = nullptr) {
        using func_t = bool (*)(const char*, float[4], ImGuiColorEditFlags, const float*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorPi"
                                                                                         "cker4"));
        return func(label, col, flags, ref_col);
    }

    inline bool ColorButton(const char* desc_id, const ImVec4 col, ImGuiColorEditFlags flags, const ImVec2 size) {
        using func_t = bool (*)(const char*, const ImVec4, ImGuiColorEditFlags, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorBu"
                                                                                         "tton"));
        return func(desc_id, col, flags, size);
    }

    inline void SetColorEditOptions(ImGuiColorEditFlags flags) {
        using func_t            = void (*)(ImGuiColorEditFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetColo"
                                                                                         "rEditOpti"
                                                                                         "ons"));
        return func(flags);
    }

    inline bool TreeNode(const char* label) {
        using func_t            = bool (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "e_Str"));
        return func(label);
    }

    inline bool TreeNode(const char* str_id, const char* fmt, ...) {
        using func_t            = bool (*)(const char*, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eV_Str"));
        va_list            args;
        va_start(args, fmt);
        auto result = func(str_id, fmt, args);
        va_end(args);
        return result;
    }

    inline bool TreeNode(const void* ptr_id, const char* fmt, ...) {
        using func_t            = bool (*)(const void*, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eV_Ptr"));
        va_list            args;
        va_start(args, fmt);
        auto result = func(ptr_id, fmt, args);
        va_end(args);
        return result;
    }

    inline bool TreeNodeV(const char* str_id, const char* fmt, va_list args) {
        using func_t            = bool (*)(const char*, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eV_Str"));
        return func(str_id, fmt, args);
    }

    inline bool TreeNodeV(const void* ptr_id, const char* fmt, va_list args) {
        using func_t            = bool (*)(const void*, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eV_Ptr"));
        return func(ptr_id, fmt, args);
    }

    inline bool TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0) {
        using func_t            = bool (*)(const char*, ImGuiTreeNodeFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eEx_"
                                                                                         "Str"));
        return func(label, flags);
    }

    inline bool TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) {
        using func_t            = bool (*)(const char*, ImGuiTreeNodeFlags, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eExV_"
                                                                                         "Str"));
        va_list            args;
        va_start(args, fmt);
        auto result = func(str_id, flags, fmt, args);
        va_end(args);
        return result;
    }

    inline bool TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) {
        using func_t            = bool (*)(const void*, ImGuiTreeNodeFlags, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eExV_"
                                                                                         "Ptr"));
        va_list            args;
        va_start(args, fmt);
        auto result = func(ptr_id, flags, fmt, args);
        va_end(args);
        return result;
    }

    inline bool TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) {
        using func_t            = bool (*)(const char*, ImGuiTreeNodeFlags, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eExV_"
                                                                                         "Str"));
        return func(str_id, flags, fmt, args);
    }

    inline bool TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) {
        using func_t            = bool (*)(const void*, ImGuiTreeNodeFlags, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eExV_"
                                                                                         "Ptr"));
        return func(ptr_id, flags, fmt, args);
    }

    inline void TreePush(const char* str_id) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreePus"
                                                                                         "h_Str"));
        return func(str_id);
    }

    inline void TreePush(const void* ptr_id) {
        using func_t            = void (*)(const void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreePus"
                                                                                         "h_Ptr"));
        return func(ptr_id);
    }

    inline void TreePop() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreePo"
                                                                                         "p"));
        return func();
    }

    inline float GetTreeNodeToLabelSpacing() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetTreeN"
                                                                                        "odeToLabel"
                                                                                        "Spacing"));
        return func();
    }

    inline bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0) {
        using func_t            = bool (*)(const char*, ImGuiTreeNodeFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCollaps"
                                                                                         "ingHeader"
                                                                                         "_TreeNode"
                                                                                         "Flags"));
        return func(label, flags);
    }

    inline bool CollapsingHeader(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags = 0) {
        using func_t            = bool (*)(const char*, bool*, ImGuiTreeNodeFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCollaps"
                                                                                         "ingHeader"
                                                                                         "_BoolPt"
                                                                                         "r"));
        return func(label, p_visible, flags);
    }

    inline void SetNextItemOpen(bool is_open, ImGuiCond cond = 0) {
        using func_t            = void (*)(bool, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "ItemOpe"
                                                                                         "n"));
        return func(is_open, cond);
    }

    inline bool Selectable(const char* label, bool selected, ImGuiSelectableFlags flags = 0, const ImVec2 size = ImVec2(0, 0)) {
        using func_t            = bool (*)(const char*, bool, ImGuiSelectableFlags, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSelecta"
                                                                                         "ble_"
                                                                                         "Bool"));
        return func(label, selected, flags, size);
    }

    inline bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2 size = ImVec2(0, 0)) {
        using func_t            = bool (*)(const char*, bool*, ImGuiSelectableFlags, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSelecta"
                                                                                         "ble_"
                                                                                         "BoolPt"
                                                                                         "r"));
        return func(label, p_selected, flags, size);
    }

    inline bool BeginListBox(const char* label, const ImVec2 size) {
        using func_t            = bool (*)(const char*, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginLi"
                                                                                         "stBox"));
        return func(label, size);
    }

    inline void EndListBox() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndList"
                                                                                         "Box"));
        return func();
    }

    inline bool ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1) {
        using func_t            = bool (*)(const char*, int*, const char* const[], int, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igListBox"
                                                                                         "_Str_"
                                                                                         "arr"));
        return func(label, current_item, items, items_count, height_in_items);
    }

    inline bool ListBox(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int height_in_items = -1) {
        using func_t = bool (*)(const char*, int*, const char* (*)(void*, int), void*, int, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igListBox"
                                                                                         "_FnStrPt"
                                                                                         "r"));
        return func(label, current_item, getter, user_data, items_count, height_in_items);
    }

    inline void PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float)) {
        using func_t = void (*)(const char*, const float*, int, int, const char*, float, float, ImVec2, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPlotLin"
                                                                                         "es_"
                                                                                         "FloatPt"
                                                                                         "r"));
        return func(label, values, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size, stride);
    }

    inline void PlotLines(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0)) {
        using func_t = void (*)(const char*, float (*)(void*, int), void*, int, int, const char*, float, float, ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPlotLin"
                                                                                         "es_"
                                                                                         "FnFloatPt"
                                                                                         "r"));
        return func(label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
    }

    inline void PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float)) {
        using func_t = void (*)(const char*, const float*, int, int, const char*, float, float, ImVec2, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPlotHis"
                                                                                         "togram_"
                                                                                         "FloatPt"
                                                                                         "r"));
        return func(label, values, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size, stride);
    }

    inline void PlotHistogram(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0)) {
        using func_t = void (*)(const char*, float (*)(void*, int), void*, int, int, const char*, float, float, ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPlotHis"
                                                                                         "togram_"
                                                                                         "FnFloatPt"
                                                                                         "r"));
        return func(label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
    }

    inline void Value(const char* prefix, bool b) {
        using func_t            = void (*)(const char*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igValue_"
                                                                                         "Bool"));
        return func(prefix, b);
    }

    inline void Value(const char* prefix, int v) {
        using func_t            = void (*)(const char*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igValue_"
                                                                                         "Int"));
        return func(prefix, v);
    }

    inline void Value(const char* prefix, unsigned int v) {
        using func_t            = void (*)(const char*, unsigned int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igValue_"
                                                                                         "Uint"));
        return func(prefix, v);
    }

    inline void Value(const char* prefix, float v, const char* float_format = nullptr) {
        using func_t            = void (*)(const char*, float, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igValue_"
                                                                                         "Float"));
        return func(prefix, v, float_format);
    }

    inline bool BeginMenuBar() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginMe"
                                                                                         "nuBar"));
        return func();
    }

    inline void EndMenuBar() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndMenu"
                                                                                         "Bar"));
        return func();
    }

    inline bool BeginMainMenuBar() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginMa"
                                                                                         "inMenuBa"
                                                                                         "r"));
        return func();
    }

    inline void EndMainMenuBar() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndMain"
                                                                                         "MenuBa"
                                                                                         "r"));
        return func();
    }

    inline bool BeginMenu(const char* label, bool enabled = true) {
        using func_t            = bool (*)(const char*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginMe"
                                                                                         "nu"));
        return func(label, enabled);
    }

    inline void EndMenu() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndMen"
                                                                                         "u"));
        return func();
    }

    inline bool MenuItem(const char* label, const char* shortcut = nullptr, bool selected = false, bool enabled = true) {
        using func_t            = bool (*)(const char*, const char*, bool, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igMenuIte"
                                                                                         "m_Bool"));
        return func(label, shortcut, selected, enabled);
    }

    inline bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true) {
        using func_t            = bool (*)(const char*, const char*, bool*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igMenuIte"
                                                                                         "m_"
                                                                                         "BoolPt"
                                                                                         "r"));
        return func(label, shortcut, p_selected, enabled);
    }

    inline bool BeginTooltip() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginTo"
                                                                                         "oltip"));
        return func();
    }

    inline void EndTooltip() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndTool"
                                                                                         "tip"));
        return func();
    }

    inline void SetTooltip(const char* fmt, ...) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetTool"
                                                                                         "tipV"));
        va_list            args;
        va_start(args, fmt);
        func(fmt, args);
        va_end(args);
    }

    inline void SetTooltipV(const char* fmt, va_list args) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetTool"
                                                                                         "tipV"));
        return func(fmt, args);
    }

    inline bool BeginItemTooltip() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginIt"
                                                                                         "emToolti"
                                                                                         "p"));
        return func();
    }

    inline void SetItemTooltip(const char* fmt, ...) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetItem"
                                                                                         "Tooltip"
                                                                                         "V"));
        va_list            args;
        va_start(args, fmt);
        func(fmt, args);
        va_end(args);
    }

    inline void SetItemTooltipV(const char* fmt, va_list args) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetItem"
                                                                                         "Tooltip"
                                                                                         "V"));
        return func(fmt, args);
    }

    inline bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0) {
        using func_t            = bool (*)(const char*, ImGuiWindowFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginPo"
                                                                                         "pup"));
        return func(str_id, flags);
    }

    inline bool BeginPopupModal(const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0) {
        using func_t            = bool (*)(const char*, bool*, ImGuiWindowFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginPo"
                                                                                         "pupModa"
                                                                                         "l"));
        return func(name, p_open, flags);
    }

    inline void EndPopup() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndPopu"
                                                                                         "p"));
        return func();
    }

    inline void OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0) {
        using func_t            = void (*)(const char*, ImGuiPopupFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igOpenPop"
                                                                                         "up_Str"));
        return func(str_id, popup_flags);
    }

    inline void OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0) {
        using func_t            = void (*)(ImGuiID, ImGuiPopupFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igOpenPop"
                                                                                         "up_ID"));
        return func(id, popup_flags);
    }

    inline void OpenPopupOnItemClick(const char* str_id = nullptr, ImGuiPopupFlags popup_flags = 1) {
        using func_t            = void (*)(const char*, ImGuiPopupFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igOpenPop"
                                                                                         "upOnItemC"
                                                                                         "lick"));
        return func(str_id, popup_flags);
    }

    inline void CloseCurrentPopup() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCloseCu"
                                                                                         "rrentPopu"
                                                                                         "p"));
        return func();
    }

    inline bool BeginPopupContextItem(const char* str_id = nullptr, ImGuiPopupFlags popup_flags = 1) {
        using func_t            = bool (*)(const char*, ImGuiPopupFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginPo"
                                                                                         "pupContex"
                                                                                         "tItem"));
        return func(str_id, popup_flags);
    }

    inline bool BeginPopupContextWindow(const char* str_id = nullptr, ImGuiPopupFlags popup_flags = 1) {
        using func_t            = bool (*)(const char*, ImGuiPopupFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginPo"
                                                                                         "pupContex"
                                                                                         "tWindo"
                                                                                         "w"));
        return func(str_id, popup_flags);
    }

    inline bool BeginPopupContextVoid(const char* str_id = nullptr, ImGuiPopupFlags popup_flags = 1) {
        using func_t            = bool (*)(const char*, ImGuiPopupFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginPo"
                                                                                         "pupContex"
                                                                                         "tVoid"));
        return func(str_id, popup_flags);
    }

    inline bool IsPopupOpen(const char* str_id, ImGuiPopupFlags flags = 0) {
        using func_t            = bool (*)(const char*, ImGuiPopupFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsPopup"
                                                                                         "Open_"
                                                                                         "Str"));
        return func(str_id, flags);
    }

    inline bool BeginTable(const char* str_id, int column, ImGuiTableFlags flags = 0, const ImVec2 outer_size = ImVec2(0.0f, 0.0f), float inner_width = 0.0f) {
        using func_t            = bool (*)(const char*, int, ImGuiTableFlags, const ImVec2, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginTa"
                                                                                         "ble"));
        return func(str_id, column, flags, outer_size, inner_width);
    }

    inline void EndTable() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndTabl"
                                                                                         "e"));
        return func();
    }

    inline void TableNextRow(ImGuiTableRowFlags row_flags = 0, float min_row_height = 0.0f) {
        using func_t            = void (*)(ImGuiTableRowFlags, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableNe"
                                                                                         "xtRow"));
        return func(row_flags, min_row_height);
    }

    inline bool TableNextColumn() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableNe"
                                                                                         "xtColum"
                                                                                         "n"));
        return func();
    }

    inline bool TableSetColumnIndex(int column_n) {
        using func_t            = bool (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "tColumnIn"
                                                                                         "dex"));
        return func(column_n);
    }

    inline void TableSetupColumn(const char* label, ImGuiTableColumnFlags flags = 0, float init_width_or_weight = 0.0f, ImGuiID user_id = 0) {
        using func_t            = void (*)(const char*, ImGuiTableColumnFlags, float, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "tupColum"
                                                                                         "n"));
        return func(label, flags, init_width_or_weight, user_id);
    }

    inline void TableSetupScrollFreeze(int cols, int rows) {
        using func_t            = void (*)(int, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "tupScroll"
                                                                                         "Freeze"));
        return func(cols, rows);
    }

    inline void TableHeader(const char* label) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableHe"
                                                                                         "ader"));
        return func(label);
    }

    inline void TableHeadersRow() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableHe"
                                                                                         "adersRo"
                                                                                         "w"));
        return func();
    }

    inline void TableAngledHeadersRow() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableAn"
                                                                                         "gledHeade"
                                                                                         "rsRow"));
        return func();
    }

    inline ImGuiTableSortSpecs* TableGetSortSpecs() {
        using func_t           = ImGuiTableSortSpecs* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "SortSpec"
                                                                                        "s"));
        return func();
    }

    inline int TableGetColumnCount() {
        using func_t           = int (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "ColumnCoun"
                                                                                        "t"));
        return func();
    }

    inline int TableGetColumnIndex() {
        using func_t           = int (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "ColumnInde"
                                                                                        "x"));
        return func();
    }

    inline int TableGetRowIndex() {
        using func_t           = int (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "RowInde"
                                                                                        "x"));
        return func();
    }

    inline const char* TableGetColumnName(int column_n = -1) {
        using func_t           = const char* (*)(int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "ColumnName"
                                                                                        "_Int"));
        return func(column_n);
    }

    inline ImGuiTableColumnFlags TableGetColumnFlags(int column_n = -1) {
        using func_t           = ImGuiTableColumnFlags (*)(int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "ColumnFlag"
                                                                                        "s"));
        return func(column_n);
    }

    inline void TableSetColumnEnabled(int column_n, bool v) {
        using func_t            = void (*)(int, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "tColumnEn"
                                                                                         "abled"));
        return func(column_n, v);
    }

    inline void TableSetBgColor(ImGuiTableBgTarget target, ImU32 color, int column_n = -1) {
        using func_t            = void (*)(ImGuiTableBgTarget, ImU32, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "tBgColo"
                                                                                         "r"));
        return func(target, color, column_n);
    }

    inline void Columns(int count = 1, const char* id = nullptr, bool border = true) {
        using func_t            = void (*)(int, const char*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColumn"
                                                                                         "s"));
        return func(count, id, border);
    }

    inline void NextColumn() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNextCol"
                                                                                         "umn"));
        return func();
    }

    inline int GetColumnIndex() {
        using func_t           = int (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetColum"
                                                                                        "nIndex"));
        return func();
    }

    inline float GetColumnWidth(int column_index = -1) {
        using func_t           = float (*)(int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetColum"
                                                                                        "nWidth"));
        return func(column_index);
    }

    inline void SetColumnWidth(int column_index, float width) {
        using func_t            = void (*)(int, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetColu"
                                                                                         "mnWidt"
                                                                                         "h"));
        return func(column_index, width);
    }

    inline float GetColumnOffset(int column_index = -1) {
        using func_t           = float (*)(int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetColum"
                                                                                        "nOffset"));
        return func(column_index);
    }

    inline void SetColumnOffset(int column_index, float offset_x) {
        using func_t            = void (*)(int, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetColu"
                                                                                         "mnOffse"
                                                                                         "t"));
        return func(column_index, offset_x);
    }

    inline int GetColumnsCount() {
        using func_t           = int (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetColum"
                                                                                        "nsCount"));
        return func();
    }

    inline bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0) {
        using func_t            = bool (*)(const char*, ImGuiTabBarFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginTa"
                                                                                         "bBar"));
        return func(str_id, flags);
    }

    inline void EndTabBar() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndTabB"
                                                                                         "ar"));
        return func();
    }

    inline bool BeginTabItem(const char* label, bool* p_open = nullptr, ImGuiTabItemFlags flags = 0) {
        using func_t            = bool (*)(const char*, bool*, ImGuiTabItemFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginTa"
                                                                                         "bItem"));
        return func(label, p_open, flags);
    }

    inline void EndTabItem() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndTabI"
                                                                                         "tem"));
        return func();
    }

    inline bool TabItemButton(const char* label, ImGuiTabItemFlags flags = 0) {
        using func_t            = bool (*)(const char*, ImGuiTabItemFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabItem"
                                                                                         "Button"));
        return func(label, flags);
    }

    inline void SetTabItemClosed(const char* tab_or_docked_window_label) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetTabI"
                                                                                         "temClose"
                                                                                         "d"));
        return func(tab_or_docked_window_label);
    }

    inline ImGuiID DockSpace(ImGuiID dockspace_id, const ImVec2 size, ImGuiDockNodeFlags flags, const ImGuiWindowClass* window_class) {
        using func_t = ImGuiID (*)(ImGuiID, const ImVec2, ImGuiDockNodeFlags, const ImGuiWindowClass*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockSpac"
                                                                                        "e"));
        return func(dockspace_id, size, flags, window_class);
    }

    inline ImGuiID DockSpaceOverViewport(ImGuiID dockspace_id = 0, const ImGuiViewport* viewport = nullptr, ImGuiDockNodeFlags flags = 0, const ImGuiWindowClass* window_class = nullptr) {
        using func_t = ImGuiID (*)(ImGuiID, const ImGuiViewport*, ImGuiDockNodeFlags, const ImGuiWindowClass*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockSpac"
                                                                                        "eOverViewp"
                                                                                        "ort"));
        return func(dockspace_id, viewport, flags, window_class);
    }

    inline void SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond = 0) {
        using func_t            = void (*)(ImGuiID, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowDoc"
                                                                                         "kID"));
        return func(dock_id, cond);
    }

    inline void SetNextWindowClass(const ImGuiWindowClass* window_class) {
        using func_t            = void (*)(const ImGuiWindowClass*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowCla"
                                                                                         "ss"));
        return func(window_class);
    }

    inline ImGuiID GetWindowDockID() {
        using func_t           = ImGuiID (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWindo"
                                                                                        "wDockID"));
        return func();
    }

    inline bool IsWindowDocked() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsWindo"
                                                                                         "wDocke"
                                                                                         "d"));
        return func();
    }

    inline void LogToTTY(int auto_open_depth = -1) {
        using func_t            = void (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogToTT"
                                                                                         "Y"));
        return func(auto_open_depth);
    }

    inline void LogToFile(int auto_open_depth = -1, const char* filename = nullptr) {
        using func_t            = void (*)(int, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogToFi"
                                                                                         "le"));
        return func(auto_open_depth, filename);
    }

    inline void LogToClipboard(int auto_open_depth = -1) {
        using func_t            = void (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogToCl"
                                                                                         "ipboar"
                                                                                         "d"));
        return func(auto_open_depth);
    }

    inline void LogFinish() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogFini"
                                                                                         "sh"));
        return func();
    }

    inline void LogButtons() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogButt"
                                                                                         "ons"));
        return func();
    }

    inline void LogTextV(const char* fmt, va_list args) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogText"
                                                                                         "V"));
        return func(fmt, args);
    }

    inline bool BeginDragDropSource(ImGuiDragDropFlags flags = 0) {
        using func_t            = bool (*)(ImGuiDragDropFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginDr"
                                                                                         "agDropSou"
                                                                                         "rce"));
        return func(flags);
    }

    inline bool SetDragDropPayload(const char* type, const void* data, size_t sz, ImGuiCond cond = 0) {
        using func_t            = bool (*)(const char*, const void*, size_t, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetDrag"
                                                                                         "DropPaylo"
                                                                                         "ad"));
        return func(type, data, sz, cond);
    }

    inline void EndDragDropSource() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndDrag"
                                                                                         "DropSourc"
                                                                                         "e"));
        return func();
    }

    inline bool BeginDragDropTarget() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginDr"
                                                                                         "agDropTar"
                                                                                         "get"));
        return func();
    }

    inline const ImGuiPayload* AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags = 0) {
        using func_t           = const ImGuiPayload* (*)(const char*, ImGuiDragDropFlags);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igAcceptDr"
                                                                                        "agDropPayl"
                                                                                        "oad"));
        return func(type, flags);
    }

    inline void EndDragDropTarget() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndDrag"
                                                                                         "DropTarge"
                                                                                         "t"));
        return func();
    }

    inline const ImGuiPayload* GetDragDropPayload() {
        using func_t           = const ImGuiPayload* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetDragD"
                                                                                        "ropPayloa"
                                                                                        "d"));
        return func();
    }

    inline void BeginDisabled(bool disabled = true) {
        using func_t            = void (*)(bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginDi"
                                                                                         "sabled"));
        return func(disabled);
    }

    inline void EndDisabled() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndDisa"
                                                                                         "bled"));
        return func();
    }

    inline void PushClipRect(const ImVec2 clip_rect_min, const ImVec2 clip_rect_max, bool intersect_with_current_clip_rect = false) {
        using func_t            = void (*)(const ImVec2, const ImVec2, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushCli"
                                                                                         "pRect"));
        return func(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
    }

    inline void PopClipRect() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopClip"
                                                                                         "Rect"));
        return func();
    }

    inline void SetItemDefaultFocus() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetItem"
                                                                                         "DefaultFo"
                                                                                         "cus"));
        return func();
    }

    inline void SetKeyboardFocusHere(int offset = 0) {
        using func_t            = void (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetKeyb"
                                                                                         "oardFocus"
                                                                                         "Here"));
        return func(offset);
    }

    inline void SetNextItemAllowOverlap() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "ItemAllow"
                                                                                         "Overla"
                                                                                         "p"));
        return func();
    }

    inline bool IsItemHovered(ImGuiHoveredFlags flags = 0) {
        using func_t            = bool (*)(ImGuiHoveredFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemH"
                                                                                         "overed"));
        return func(flags);
    }

    inline bool IsItemActive() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemA"
                                                                                         "ctive"));
        return func();
    }

    inline bool IsItemFocused() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemF"
                                                                                         "ocused"));
        return func();
    }

    inline bool IsItemClicked(ImGuiMouseButton mouse_button = 0) {
        using func_t            = bool (*)(ImGuiMouseButton);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemC"
                                                                                         "licked"));
        return func(mouse_button);
    }

    inline bool IsItemVisible() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemV"
                                                                                         "isible"));
        return func();
    }

    inline bool IsItemEdited() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemE"
                                                                                         "dited"));
        return func();
    }

    inline bool IsItemActivated() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemA"
                                                                                         "ctivate"
                                                                                         "d"));
        return func();
    }

    inline bool IsItemDeactivated() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemD"
                                                                                         "eactivate"
                                                                                         "d"));
        return func();
    }

    inline bool IsItemDeactivatedAfterEdit() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemD"
                                                                                         "eactivate"
                                                                                         "dAfterEdi"
                                                                                         "t"));
        return func();
    }

    inline bool IsItemToggledOpen() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemT"
                                                                                         "oggledOpe"
                                                                                         "n"));
        return func();
    }

    inline bool IsAnyItemHovered() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsAnyIt"
                                                                                         "emHovere"
                                                                                         "d"));
        return func();
    }

    inline bool IsAnyItemActive() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsAnyIt"
                                                                                         "emActiv"
                                                                                         "e"));
        return func();
    }

    inline bool IsAnyItemFocused() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsAnyIt"
                                                                                         "emFocuse"
                                                                                         "d"));
        return func();
    }

    inline ImGuiID GetItemID() {
        using func_t           = ImGuiID (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetItemI"
                                                                                        "D"));
        return func();
    }

    inline void GetItemRectMin(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetItem"
                                                                                         "RectMi"
                                                                                         "n"));
        return func(pOut);
    }

    inline void GetItemRectMax(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetItem"
                                                                                         "RectMa"
                                                                                         "x"));
        return func(pOut);
    }

    inline void GetItemRectSize(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetItem"
                                                                                         "RectSiz"
                                                                                         "e"));
        return func(pOut);
    }

    inline ImGuiViewport* GetMainViewport() {
        using func_t           = ImGuiViewport* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetMainV"
                                                                                        "iewport"));
        return func();
    }

    inline ImDrawList* GetBackgroundDrawList() {
        using func_t           = ImDrawList* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetBackg"
                                                                                        "roundDrawL"
                                                                                        "ist_Nil"));
        return func();
    }

    inline ImDrawList* GetForegroundDrawList() {
        using func_t           = ImDrawList* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetForeg"
                                                                                        "roundDrawL"
                                                                                        "ist_Nil"));
        return func();
    }

    inline ImDrawList* GetBackgroundDrawList(ImGuiViewport* viewport) {
        using func_t           = ImDrawList* (*)(ImGuiViewport*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetBackg"
                                                                                        "roundDrawL"
                                                                                        "ist_"
                                                                                        "ViewportPt"
                                                                                        "r"));
        return func(viewport);
    }

    inline ImDrawList* GetForegroundDrawList(ImGuiViewport* viewport) {
        using func_t           = ImDrawList* (*)(ImGuiViewport*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetForeg"
                                                                                        "roundDrawL"
                                                                                        "ist_"
                                                                                        "ViewportPt"
                                                                                        "r"));
        return func(viewport);
    }

    inline bool IsRectVisible(const ImVec2 size) {
        using func_t            = bool (*)(const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsRectV"
                                                                                         "isible_"
                                                                                         "Nil"));
        return func(size);
    }

    inline bool IsRectVisible(const ImVec2 rect_min, const ImVec2 rect_max) {
        using func_t            = bool (*)(const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsRectV"
                                                                                         "isible_"
                                                                                         "Vec2"));
        return func(rect_min, rect_max);
    }

    inline double GetTime() {
        using func_t           = double (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetTim"
                                                                                        "e"));
        return func();
    }

    inline int GetFrameCount() {
        using func_t           = int (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetFrame"
                                                                                        "Count"));
        return func();
    }

    inline ImDrawListSharedData* GetDrawListSharedData() {
        using func_t           = ImDrawListSharedData* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetDrawL"
                                                                                        "istSharedD"
                                                                                        "ata"));
        return func();
    }

    inline const char* GetStyleColorName(ImGuiCol idx) {
        using func_t           = const char* (*)(ImGuiCol);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetStyle"
                                                                                        "ColorNam"
                                                                                        "e"));
        return func(idx);
    }

    inline void SetStateStorage(ImGuiStorage* storage) {
        using func_t            = void (*)(ImGuiStorage*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetStat"
                                                                                         "eStorag"
                                                                                         "e"));
        return func(storage);
    }

    inline ImGuiStorage* GetStateStorage() {
        using func_t           = ImGuiStorage* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetState"
                                                                                        "Storage"));
        return func();
    }

    inline void CalcTextSize(ImVec2* pOut, const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width) {
        using func_t            = void (*)(ImVec2*, const char*, const char*, bool, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCalcTex"
                                                                                         "tSize"));
        return func(pOut, text, text_end, hide_text_after_double_hash, wrap_width);
    }

    inline void ColorConvertU32ToFloat4(ImVec4* pOut, ImU32 in) {
        using func_t            = void (*)(ImVec4*, ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorCo"
                                                                                         "nvertU32T"
                                                                                         "oFloat"
                                                                                         "4"));
        return func(pOut, in);
    }

    inline ImU32 ColorConvertFloat4ToU32(const ImVec4 in) {
        using func_t           = ImU32 (*)(const ImVec4);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorCon"
                                                                                        "vertFloat4"
                                                                                        "ToU32"));
        return func(in);
    }

    inline void ColorConvertRGBtoHSV(float r, float g, float b, float* out_h, float* out_s, float* out_v) {
        using func_t            = void (*)(float, float, float, float*, float*, float*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorCo"
                                                                                         "nvertRGBt"
                                                                                         "oHSV"));
        return func(r, g, b, out_h, out_s, out_v);
    }

    inline void ColorConvertHSVtoRGB(float h, float s, float v, float* out_r, float* out_g, float* out_b) {
        using func_t            = void (*)(float, float, float, float*, float*, float*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorCo"
                                                                                         "nvertHSVt"
                                                                                         "oRGB"));
        return func(h, s, v, out_r, out_g, out_b);
    }

    inline bool IsKeyDown(ImGuiKey key) {
        using func_t            = bool (*)(ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsKeyDo"
                                                                                         "wn_Nil"));
        return func(key);
    }

    inline bool IsKeyPressed(ImGuiKey key, bool repeat = true) {
        using func_t            = bool (*)(ImGuiKey, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsKeyPr"
                                                                                         "essed_"
                                                                                         "Bool"));
        return func(key, repeat);
    }

    inline bool IsKeyReleased(ImGuiKey key) {
        using func_t            = bool (*)(ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsKeyRe"
                                                                                         "leased_"
                                                                                         "Nil"));
        return func(key);
    }

    inline bool IsKeyChordPressed(ImGuiKeyChord key_chord) {
        using func_t            = bool (*)(ImGuiKeyChord);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsKeyCh"
                                                                                         "ordPresse"
                                                                                         "d_Nil"));
        return func(key_chord);
    }

    inline int GetKeyPressedAmount(ImGuiKey key, float repeat_delay, float rate) {
        using func_t           = int (*)(ImGuiKey, float, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetKeyPr"
                                                                                        "essedAmoun"
                                                                                        "t"));
        return func(key, repeat_delay, rate);
    }

    inline const char* GetKeyName(ImGuiKey key) {
        using func_t           = const char* (*)(ImGuiKey);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetKeyNa"
                                                                                        "me"));
        return func(key);
    }

    inline void SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard) {
        using func_t            = void (*)(bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "FrameWant"
                                                                                         "CaptureKe"
                                                                                         "yboard"));
        return func(want_capture_keyboard);
    }

    inline bool Shortcut(ImGuiKeyChord key_chord, ImGuiInputFlags flags = 0) {
        using func_t            = bool (*)(ImGuiKeyChord, ImGuiInputFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShortcu"
                                                                                         "t_Nil"));
        return func(key_chord, flags);
    }

    inline void SetNextItemShortcut(ImGuiKeyChord key_chord, ImGuiInputFlags flags = 0) {
        using func_t            = void (*)(ImGuiKeyChord, ImGuiInputFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "ItemShort"
                                                                                         "cut"));
        return func(key_chord, flags);
    }

    inline bool IsMouseDown(ImGuiMouseButton button) {
        using func_t            = bool (*)(ImGuiMouseButton);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "Down_"
                                                                                         "Nil"));
        return func(button);
    }

    inline bool IsMouseClicked(ImGuiMouseButton button, bool repeat = false) {
        using func_t            = bool (*)(ImGuiMouseButton, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "Clicked_"
                                                                                         "Bool"));
        return func(button, repeat);
    }

    inline bool IsMouseReleased(ImGuiMouseButton button) {
        using func_t            = bool (*)(ImGuiMouseButton);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "Released_"
                                                                                         "Nil"));
        return func(button);
    }

    inline bool IsMouseDoubleClicked(ImGuiMouseButton button) {
        using func_t            = bool (*)(ImGuiMouseButton);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "DoubleCli"
                                                                                         "cked_"
                                                                                         "Nil"));
        return func(button);
    }

    inline int GetMouseClickedCount(ImGuiMouseButton button) {
        using func_t           = int (*)(ImGuiMouseButton);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetMouse"
                                                                                        "ClickedCou"
                                                                                        "nt"));
        return func(button);
    }

    inline bool IsMouseHoveringRect(const ImVec2 r_min, const ImVec2 r_max, bool clip = true) {
        using func_t            = bool (*)(const ImVec2, const ImVec2, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "HoveringR"
                                                                                         "ect"));
        return func(r_min, r_max, clip);
    }

    inline bool IsMousePosValid(const ImVec2* mouse_pos = nullptr) {
        using func_t            = bool (*)(const ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "PosVali"
                                                                                         "d"));
        return func(mouse_pos);
    }

    inline bool IsAnyMouseDown() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsAnyMo"
                                                                                         "useDow"
                                                                                         "n"));
        return func();
    }

    inline void GetMousePos(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetMous"
                                                                                         "ePos"));
        return func(pOut);
    }

    inline void GetMousePosOnOpeningCurrentPopup(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetMous"
                                                                                         "ePosOnOpe"
                                                                                         "ningCurre"
                                                                                         "ntPopu"
                                                                                         "p"));
        return func(pOut);
    }

    inline bool IsMouseDragging(ImGuiMouseButton button, float lock_threshold = -1.0f) {
        using func_t            = bool (*)(ImGuiMouseButton, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "Draggin"
                                                                                         "g"));
        return func(button, lock_threshold);
    }

    inline void GetMouseDragDelta(ImVec2* pOut, ImGuiMouseButton button, float lock_threshold) {
        using func_t            = void (*)(ImVec2*, ImGuiMouseButton, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetMous"
                                                                                         "eDragDelt"
                                                                                         "a"));
        return func(pOut, button, lock_threshold);
    }

    inline void ResetMouseDragDelta(ImGuiMouseButton button = 0) {
        using func_t            = void (*)(ImGuiMouseButton);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igResetMo"
                                                                                         "useDragDe"
                                                                                         "lta"));
        return func(button);
    }

    inline ImGuiMouseCursor GetMouseCursor() {
        using func_t           = ImGuiMouseCursor (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetMouse"
                                                                                        "Cursor"));
        return func();
    }

    inline void SetMouseCursor(ImGuiMouseCursor cursor_type) {
        using func_t            = void (*)(ImGuiMouseCursor);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetMous"
                                                                                         "eCurso"
                                                                                         "r"));
        return func(cursor_type);
    }

    inline void SetNextFrameWantCaptureMouse(bool want_capture_mouse) {
        using func_t            = void (*)(bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "FrameWant"
                                                                                         "CaptureMo"
                                                                                         "use"));
        return func(want_capture_mouse);
    }

    inline const char* GetClipboardText() {
        using func_t           = const char* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetClipb"
                                                                                        "oardTex"
                                                                                        "t"));
        return func();
    }

    inline void SetClipboardText(const char* text) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetClip"
                                                                                         "boardTex"
                                                                                         "t"));
        return func(text);
    }

    inline void LoadIniSettingsFromDisk(const char* ini_filename) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLoadIni"
                                                                                         "SettingsF"
                                                                                         "romDis"
                                                                                         "k"));
        return func(ini_filename);
    }

    inline void LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size = 0) {
        using func_t            = void (*)(const char*, size_t);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLoadIni"
                                                                                         "SettingsF"
                                                                                         "romMemor"
                                                                                         "y"));
        return func(ini_data, ini_size);
    }

    inline void SaveIniSettingsToDisk(const char* ini_filename) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSaveIni"
                                                                                         "SettingsT"
                                                                                         "oDisk"));
        return func(ini_filename);
    }

    inline const char* SaveIniSettingsToMemory(size_t* out_ini_size = nullptr) {
        using func_t           = const char* (*)(size_t*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSaveIniS"
                                                                                        "ettingsToM"
                                                                                        "emory"));
        return func(out_ini_size);
    }

    inline void DebugTextEncoding(const char* text) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugTe"
                                                                                         "xtEncodin"
                                                                                         "g"));
        return func(text);
    }

    inline void DebugFlashStyleColor(ImGuiCol idx) {
        using func_t            = void (*)(ImGuiCol);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugFl"
                                                                                         "ashStyleC"
                                                                                         "olor"));
        return func(idx);
    }

    inline void DebugStartItemPicker() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugSt"
                                                                                         "artItemPi"
                                                                                         "cker"));
        return func();
    }

    inline bool DebugCheckVersionAndDataLayout(const char* version_str, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_drawvert, size_t sz_drawidx) {
        using func_t = bool (*)(const char*, size_t, size_t, size_t, size_t, size_t, size_t);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugCh"
                                                                                         "eckVersio"
                                                                                         "nAndDataL"
                                                                                         "ayout"));
        return func(version_str, sz_io, sz_style, sz_vec2, sz_vec4, sz_drawvert, sz_drawidx);
    }

    inline void SetAllocatorFunctions(ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data = nullptr) {
        using func_t            = void (*)(ImGuiMemAllocFunc, ImGuiMemFreeFunc, void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetAllo"
                                                                                         "catorFunc"
                                                                                         "tions"));
        return func(alloc_func, free_func, user_data);
    }

    inline void GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func, ImGuiMemFreeFunc* p_free_func, void** p_user_data) {
        using func_t            = void (*)(ImGuiMemAllocFunc*, ImGuiMemFreeFunc*, void**);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetAllo"
                                                                                         "catorFunc"
                                                                                         "tions"));
        return func(p_alloc_func, p_free_func, p_user_data);
    }

    inline void* MemAlloc(size_t size) {
        using func_t           = void* (*)(size_t);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igMemAllo"
                                                                                        "c"));
        return func(size);
    }

    inline void MemFree(void* ptr) {
        using func_t            = void (*)(void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igMemFre"
                                                                                         "e"));
        return func(ptr);
    }

    inline ImGuiPlatformIO* GetPlatformIO() {
        using func_t           = ImGuiPlatformIO* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetPlatf"
                                                                                        "ormIO"));
        return func();
    }

    inline void UpdatePlatformWindows() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igUpdateP"
                                                                                         "latformWi"
                                                                                         "ndows"));
        return func();
    }

    inline void RenderPlatformWindowsDefault(void* platform_render_arg = nullptr, void* renderer_render_arg = nullptr) {
        using func_t            = void (*)(void*, void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderP"
                                                                                         "latformWi"
                                                                                         "ndowsDefa"
                                                                                         "ult"));
        return func(platform_render_arg, renderer_render_arg);
    }

    inline void DestroyPlatformWindows() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDestroy"
                                                                                         "PlatformW"
                                                                                         "indows"));
        return func();
    }

    inline ImGuiViewport* FindViewportByID(ImGuiID id) {
        using func_t           = ImGuiViewport* (*)(ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindView"
                                                                                        "portByI"
                                                                                        "D"));
        return func(id);
    }

    inline ImGuiViewport* FindViewportByPlatformHandle(void* platform_handle) {
        using func_t           = ImGuiViewport* (*)(void*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindView"
                                                                                        "portByPlat"
                                                                                        "formHandl"
                                                                                        "e"));
        return func(platform_handle);
    }

    namespace ImGuiTableSortSpecsManager {
        inline ImGuiTableSortSpecs* Create(void) {
            using func_t           = ImGuiTableSortSpecs* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "ableSo"
                                                                                            "rtSpec"
                                                                                            "s_"
                                                                                            "ImGuiT"
                                                                                            "ableSo"
                                                                                            "rtSpec"
                                                                                            "s"));
            return func();
        }

        inline void Destroy(ImGuiTableSortSpecs* self) {
            using func_t            = void (*)(ImGuiTableSortSpecs*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Table"
                                                                                             "SortS"
                                                                                             "pecs_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiTableSortSpecsManager

    namespace ImGuiTableColumnSortSpecsManager {
        inline ImGuiTableColumnSortSpecs* Create(void) {
            using func_t           = ImGuiTableColumnSortSpecs* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "ableCo"
                                                                                            "lumnSo"
                                                                                            "rtSpec"
                                                                                            "s_"
                                                                                            "ImGuiT"
                                                                                            "ableCo"
                                                                                            "lumnSo"
                                                                                            "rtSpec"
                                                                                            "s"));
            return func();
        }

        inline void Destroy(ImGuiTableColumnSortSpecs* self) {
            using func_t            = void (*)(ImGuiTableColumnSortSpecs*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Table"
                                                                                             "Colum"
                                                                                             "nSort"
                                                                                             "Specs"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiTableColumnSortSpecsManager

    namespace ImGuiStyleManager {

        inline ImGuiStyle* Create(void) {
            using func_t           = ImGuiStyle* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "tyle_"
                                                                                            "ImGuiS"
                                                                                            "tyl"
                                                                                            "e"));
            return func();
        }

        inline void Destroy(ImGuiStyle* self) {
            using func_t            = void (*)(ImGuiStyle*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Style"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }

        inline void ScaleAllSizes(ImGuiStyle* self, float scale_factor) {
            using func_t            = void (*)(ImGuiStyle*, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Style"
                                                                                             "_Scal"
                                                                                             "eAllS"
                                                                                             "ize"
                                                                                             "s"));
            return func(self, scale_factor);
        }

    }  // namespace ImGuiStyleManager

    namespace ImGuiIOManager {
        inline void AddKeyEvent(ImGuiIO* self, ImGuiKey key, bool down) {
            using func_t            = void (*)(ImGuiIO*, ImGuiKey, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddKe"
                                                                                             "yEven"
                                                                                             "t"));
            return func(self, key, down);
        }

        inline void AddKeyAnalogEvent(ImGuiIO* self, ImGuiKey key, bool down, float v) {
            using func_t            = void (*)(ImGuiIO*, ImGuiKey, bool, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddKe"
                                                                                             "yAnal"
                                                                                             "ogEve"
                                                                                             "nt"));
            return func(self, key, down, v);
        }

        inline void AddMousePosEvent(ImGuiIO* self, float x, float y) {
            using func_t            = void (*)(ImGuiIO*, float, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddMo"
                                                                                             "usePo"
                                                                                             "sEven"
                                                                                             "t"));
            return func(self, x, y);
        }

        inline void AddMouseButtonEvent(ImGuiIO* self, int button, bool down) {
            using func_t            = void (*)(ImGuiIO*, int, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddMo"
                                                                                             "useBu"
                                                                                             "ttonE"
                                                                                             "ven"
                                                                                             "t"));
            return func(self, button, down);
        }

        inline void AddMouseWheelEvent(ImGuiIO* self, float wheel_x, float wheel_y) {
            using func_t            = void (*)(ImGuiIO*, float, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddMo"
                                                                                             "useWh"
                                                                                             "eelEv"
                                                                                             "en"
                                                                                             "t"));
            return func(self, wheel_x, wheel_y);
        }

        inline void AddMouseSourceEvent(ImGuiIO* self, ImGuiMouseSource source) {
            using func_t            = void (*)(ImGuiIO*, ImGuiMouseSource);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddMo"
                                                                                             "useSo"
                                                                                             "urceE"
                                                                                             "ven"
                                                                                             "t"));
            return func(self, source);
        }

        inline void AddMouseViewportEvent(ImGuiIO* self, ImGuiID id) {
            using func_t            = void (*)(ImGuiIO*, ImGuiID);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddMo"
                                                                                             "useVi"
                                                                                             "ewpor"
                                                                                             "tEven"
                                                                                             "t"));
            return func(self, id);
        }

        inline void AddFocusEvent(ImGuiIO* self, bool focused) {
            using func_t            = void (*)(ImGuiIO*, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddFo"
                                                                                             "cusEv"
                                                                                             "en"
                                                                                             "t"));
            return func(self, focused);
        }

        inline void AddInputCharacter(ImGuiIO* self, unsigned int c) {
            using func_t            = void (*)(ImGuiIO*, unsigned int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddIn"
                                                                                             "putCh"
                                                                                             "aract"
                                                                                             "er"));
            return func(self, c);
        }

        inline void AddInputCharacterUTF16(ImGuiIO* self, ImWchar16 c) {
            using func_t            = void (*)(ImGuiIO*, ImWchar16);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddIn"
                                                                                             "putCh"
                                                                                             "aract"
                                                                                             "erUTF"
                                                                                             "16"));
            return func(self, c);
        }

        inline void AddInputCharactersUTF8(ImGuiIO* self, const char* str) {
            using func_t            = void (*)(ImGuiIO*, const char*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "AddIn"
                                                                                             "putCh"
                                                                                             "aract"
                                                                                             "ersUT"
                                                                                             "F8"));
            return func(self, str);
        }

        inline void SetKeyEventNativeData(ImGuiIO* self, ImGuiKey key, int native_keycode, int native_scancode, int native_legacy_index) {
            using func_t            = void (*)(ImGuiIO*, ImGuiKey, int, int, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "SetKe"
                                                                                             "yEven"
                                                                                             "tNati"
                                                                                             "veDat"
                                                                                             "a"));
            return func(self, key, native_keycode, native_scancode, native_legacy_index);
        }

        inline void SetAppAcceptingEvents(ImGuiIO* self, bool accepting_events) {
            using func_t            = void (*)(ImGuiIO*, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "SetAp"
                                                                                             "pAcce"
                                                                                             "pting"
                                                                                             "Event"
                                                                                             "s"));
            return func(self, accepting_events);
        }

        inline void ClearEventsQueue(ImGuiIO* self) {
            using func_t            = void (*)(ImGuiIO*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "Clear"
                                                                                             "Event"
                                                                                             "sQueu"
                                                                                             "e"));
            return func(self);
        }

        inline void ClearInputKeys(ImGuiIO* self) {
            using func_t            = void (*)(ImGuiIO*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "Clear"
                                                                                             "Input"
                                                                                             "Key"
                                                                                             "s"));
            return func(self);
        }

        inline ImGuiIO* Create(void) {
            using func_t           = ImGuiIO* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "O_"
                                                                                            "ImGuiI"
                                                                                            "O"));
            return func();
        }

        inline void Destroy(ImGuiIO* self) {
            using func_t            = void (*)(ImGuiIO*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IO_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiIOManager

    namespace ImGuiInputTextCallbackDataManager {

        inline ImGuiInputTextCallbackData* Create(void) {
            using func_t           = ImGuiInputTextCallbackData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "nputTe"
                                                                                            "xtCall"
                                                                                            "backDa"
                                                                                            "ta_"
                                                                                            "ImGuiI"
                                                                                            "nputTe"
                                                                                            "xtCall"
                                                                                            "backDa"
                                                                                            "ta"));
            return func();
        }

        inline void Destroy(ImGuiInputTextCallbackData* self) {
            using func_t            = void (*)(ImGuiInputTextCallbackData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextC"
                                                                                             "allba"
                                                                                             "ckDat"
                                                                                             "a_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void DeleteChars(ImGuiInputTextCallbackData* self, int pos, int bytes_count) {
            using func_t            = void (*)(ImGuiInputTextCallbackData*, int, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextC"
                                                                                             "allba"
                                                                                             "ckDat"
                                                                                             "a_"
                                                                                             "Delet"
                                                                                             "eChar"
                                                                                             "s"));
            return func(self, pos, bytes_count);
        }

        inline void InsertChars(ImGuiInputTextCallbackData* self, int pos, const char* text, const char* text_end) {
            using func_t = void (*)(ImGuiInputTextCallbackData*, int, const char*, const char*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextC"
                                                                                             "allba"
                                                                                             "ckDat"
                                                                                             "a_"
                                                                                             "Inser"
                                                                                             "tChar"
                                                                                             "s"));
            return func(self, pos, text, text_end);
        }

        inline void SelectAll(ImGuiInputTextCallbackData* self) {
            using func_t            = void (*)(ImGuiInputTextCallbackData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextC"
                                                                                             "allba"
                                                                                             "ckDat"
                                                                                             "a_"
                                                                                             "Selec"
                                                                                             "tAl"
                                                                                             "l"));
            return func(self);
        }

        inline void ClearSelection(ImGuiInputTextCallbackData* self) {
            using func_t            = void (*)(ImGuiInputTextCallbackData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextC"
                                                                                             "allba"
                                                                                             "ckDat"
                                                                                             "a_"
                                                                                             "Clear"
                                                                                             "Selec"
                                                                                             "tio"
                                                                                             "n"));
            return func(self);
        }

        inline bool HasSelection(ImGuiInputTextCallbackData* self) {
            using func_t            = bool (*)(ImGuiInputTextCallbackData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextC"
                                                                                             "allba"
                                                                                             "ckDat"
                                                                                             "a_"
                                                                                             "HasSe"
                                                                                             "lecti"
                                                                                             "on"));
            return func(self);
        }

    }  // namespace ImGuiInputTextCallbackDataManager

    namespace ImGuiWindowClassManager {
        inline ImGuiWindowClass* Create(void) {
            using func_t           = ImGuiWindowClass* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiW"
                                                                                            "indowC"
                                                                                            "lass_"
                                                                                            "ImGuiW"
                                                                                            "indowC"
                                                                                            "las"
                                                                                            "s"));
            return func();
        }

        inline void Destroy(ImGuiWindowClass* self) {
            using func_t            = void (*)(ImGuiWindowClass*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Windo"
                                                                                             "wClas"
                                                                                             "s_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiWindowClassManager

    namespace ImGuiPayloadManager {
        inline ImGuiPayload* Create(void) {
            using func_t           = ImGuiPayload* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiP"
                                                                                            "ayload"
                                                                                            "_ImGui"
                                                                                            "Payloa"
                                                                                            "d"));
            return func();
        }

        inline void Destroy(ImGuiPayload* self) {
            using func_t            = void (*)(ImGuiPayload*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Paylo"
                                                                                             "ad_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void Clear(ImGuiPayload* self) {
            using func_t            = void (*)(ImGuiPayload*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Paylo"
                                                                                             "ad_"
                                                                                             "Clea"
                                                                                             "r"));
            return func(self);
        }

        inline bool IsDataType(ImGuiPayload* self, const char* type) {
            using func_t            = bool (*)(ImGuiPayload*, const char*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Paylo"
                                                                                             "ad_"
                                                                                             "IsDat"
                                                                                             "aTyp"
                                                                                             "e"));
            return func(self, type);
        }

        inline bool IsPreview(ImGuiPayload* self) {
            using func_t            = bool (*)(ImGuiPayload*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Paylo"
                                                                                             "ad_"
                                                                                             "IsPre"
                                                                                             "vie"
                                                                                             "w"));
            return func(self);
        }

        inline bool IsDelivery(ImGuiPayload* self) {
            using func_t            = bool (*)(ImGuiPayload*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Paylo"
                                                                                             "ad_"
                                                                                             "IsDel"
                                                                                             "iver"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiPayloadManager

    namespace ImGuiOnceUponAFrameManager {
        inline ImGuiOnceUponAFrame* Create(void) {
            using func_t           = ImGuiOnceUponAFrame* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiO"
                                                                                            "nceUpo"
                                                                                            "nAFram"
                                                                                            "e_"
                                                                                            "ImGuiO"
                                                                                            "nceUpo"
                                                                                            "nAFram"
                                                                                            "e"));
            return func();
        }

        inline void Destroy(ImGuiOnceUponAFrame* self) {
            using func_t            = void (*)(ImGuiOnceUponAFrame*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "OnceU"
                                                                                             "ponAF"
                                                                                             "rame_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiOnceUponAFrameManager

    namespace ImGuiTextRangeManager {
        inline ImGuiTextRange* Create(void) {
            using func_t           = ImGuiTextRange* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "extRan"
                                                                                            "ge_"
                                                                                            "ImGuiT"
                                                                                            "extRan"
                                                                                            "ge_"
                                                                                            "Nil"));
            return func();
        }

        inline void Destroy(ImGuiTextRange* self) {
            using func_t            = void (*)(ImGuiTextRange*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TextR"
                                                                                             "ange_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImGuiTextRange* Create(const char* _b, const char* _e) {
            using func_t           = ImGuiTextRange* (*)(const char*, const char*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "extRan"
                                                                                            "ge_"
                                                                                            "ImGuiT"
                                                                                            "extRan"
                                                                                            "ge_"
                                                                                            "Str"));
            return func(_b, _e);
        }

        inline bool Empty(ImGuiTextRange* self) {
            using func_t            = bool (*)(ImGuiTextRange*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TextR"
                                                                                             "ange_"
                                                                                             "empt"
                                                                                             "y"));
            return func(self);
        }

        inline void Split(ImGuiTextRange* self, char separator, ImVector_ImGuiTextRange* out) {
            using func_t            = void (*)(ImGuiTextRange*, char, ImVector_ImGuiTextRange*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TextR"
                                                                                             "ange_"
                                                                                             "spli"
                                                                                             "t"));
            return func(self, separator, out);
        }
    }  // namespace ImGuiTextRangeManager

    namespace ImGuiTextBufferManager {

        inline ImGuiTextBuffer* Create(void) {
            using func_t           = ImGuiTextBuffer* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "extBuf"
                                                                                            "fer_"
                                                                                            "ImGuiT"
                                                                                            "extBuf"
                                                                                            "fer"));
            return func();
        }

        inline void Destroy(ImGuiTextBuffer* self) {
            using func_t            = void (*)(ImGuiTextBuffer*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TextB"
                                                                                             "uffer"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }

        inline const char* Begin(ImGuiTextBuffer* self) {
            using func_t           = const char* (*)(ImGuiTextBuffer*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "extBuf"
                                                                                            "fer_"
                                                                                            "begi"
                                                                                            "n"));
            return func(self);
        }

        inline const char* End(ImGuiTextBuffer* self) {
            using func_t           = const char* (*)(ImGuiTextBuffer*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "extBuf"
                                                                                            "fer_"
                                                                                            "end"));
            return func(self);
        }

        inline int Size(ImGuiTextBuffer* self) {
            using func_t           = int (*)(ImGuiTextBuffer*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "extBuf"
                                                                                            "fer_"
                                                                                            "siz"
                                                                                            "e"));
            return func(self);
        }

        inline bool Empty(ImGuiTextBuffer* self) {
            using func_t            = bool (*)(ImGuiTextBuffer*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TextB"
                                                                                             "uffer"
                                                                                             "_empt"
                                                                                             "y"));
            return func(self);
        }

        inline void Clear(ImGuiTextBuffer* self) {
            using func_t            = void (*)(ImGuiTextBuffer*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TextB"
                                                                                             "uffer"
                                                                                             "_clea"
                                                                                             "r"));
            return func(self);
        }

        inline void Reserve(ImGuiTextBuffer* self, int capacity) {
            using func_t            = void (*)(ImGuiTextBuffer*, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TextB"
                                                                                             "uffer"
                                                                                             "_rese"
                                                                                             "rv"
                                                                                             "e"));
            return func(self, capacity);
        }

        inline const char* c_str(ImGuiTextBuffer* self) {
            using func_t           = const char* (*)(ImGuiTextBuffer*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "extBuf"
                                                                                            "fer_c_"
                                                                                            "str"));
            return func(self);
        }

        inline void Append(ImGuiTextBuffer* self, const char* str, const char* str_end) {
            using func_t            = void (*)(ImGuiTextBuffer*, const char*, const char*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TextB"
                                                                                             "uffer"
                                                                                             "_appe"
                                                                                             "nd"));
            return func(self, str, str_end);
        }

        inline void Appendfv(ImGuiTextBuffer* self, const char* fmt, va_list args) {
            using func_t            = void (*)(ImGuiTextBuffer*, const char*, va_list);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TextB"
                                                                                             "uffer"
                                                                                             "_appe"
                                                                                             "ndf"
                                                                                             "v"));
            return func(self, fmt, args);
        }

    }  // namespace ImGuiTextBufferManager

    namespace ImGuiStoragePairManger {
        inline ImGuiStoragePair* Create(ImGuiID _key, int _val) {
            using func_t           = ImGuiStoragePair* (*)(ImGuiID, int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "torage"
                                                                                            "Pair_"
                                                                                            "ImGuiS"
                                                                                            "torage"
                                                                                            "Pair_"
                                                                                            "Int"));
            return func(_key, _val);
        }

        inline void Destroy(ImGuiStoragePair* self) {
            using func_t            = void (*)(ImGuiStoragePair*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stora"
                                                                                             "gePai"
                                                                                             "r_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImGuiStoragePair* Create(ImGuiID _key, float _val) {
            using func_t           = ImGuiStoragePair* (*)(ImGuiID, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "torage"
                                                                                            "Pair_"
                                                                                            "ImGuiS"
                                                                                            "torage"
                                                                                            "Pair_"
                                                                                            "Floa"
                                                                                            "t"));
            return func(_key, _val);
        }

        inline ImGuiStoragePair* Create(ImGuiID _key, void* _val) {
            using func_t           = ImGuiStoragePair* (*)(ImGuiID, void*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "torage"
                                                                                            "Pair_"
                                                                                            "ImGuiS"
                                                                                            "torage"
                                                                                            "Pair_"
                                                                                            "Ptr"));
            return func(_key, _val);
        }
    }  // namespace ImGuiStoragePairManger

    namespace ImGuiStorageManger {
        inline void Clear(ImGuiStorage* self) {
            using func_t            = void (*)(ImGuiStorage*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stora"
                                                                                             "ge_"
                                                                                             "Clea"
                                                                                             "r"));
            return func(self);
        }

        inline int GetInt(ImGuiStorage* self, ImGuiID key, int default_val) {
            using func_t           = int (*)(ImGuiStorage*, ImGuiID, int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "torage"
                                                                                            "_GetIn"
                                                                                            "t"));
            return func(self, key, default_val);
        }

        inline void SetInt(ImGuiStorage* self, ImGuiID key, int val) {
            using func_t            = void (*)(ImGuiStorage*, ImGuiID, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stora"
                                                                                             "ge_"
                                                                                             "SetIn"
                                                                                             "t"));
            return func(self, key, val);
        }

        inline bool GetBool(ImGuiStorage* self, ImGuiID key, bool default_val) {
            using func_t            = bool (*)(ImGuiStorage*, ImGuiID, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stora"
                                                                                             "ge_"
                                                                                             "GetBo"
                                                                                             "ol"));
            return func(self, key, default_val);
        }

        inline void SetBool(ImGuiStorage* self, ImGuiID key, bool val) {
            using func_t            = void (*)(ImGuiStorage*, ImGuiID, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stora"
                                                                                             "ge_"
                                                                                             "SetBo"
                                                                                             "ol"));
            return func(self, key, val);
        }

        inline float GetFloat(ImGuiStorage* self, ImGuiID key, float default_val) {
            using func_t           = float (*)(ImGuiStorage*, ImGuiID, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "torage"
                                                                                            "_GetFl"
                                                                                            "oat"));
            return func(self, key, default_val);
        }

        inline void SetFloat(ImGuiStorage* self, ImGuiID key, float val) {
            using func_t            = void (*)(ImGuiStorage*, ImGuiID, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stora"
                                                                                             "ge_"
                                                                                             "SetFl"
                                                                                             "oa"
                                                                                             "t"));
            return func(self, key, val);
        }

        inline void* GetVoidPtr(ImGuiStorage* self, ImGuiID key) {
            using func_t           = void* (*)(ImGuiStorage*, ImGuiID);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "torage"
                                                                                            "_GetVo"
                                                                                            "idPt"
                                                                                            "r"));
            return func(self, key);
        }

        inline void SetVoidPtr(ImGuiStorage* self, ImGuiID key, void* val) {
            using func_t            = void (*)(ImGuiStorage*, ImGuiID, void*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stora"
                                                                                             "ge_"
                                                                                             "SetVo"
                                                                                             "idPt"
                                                                                             "r"));
            return func(self, key, val);
        }

        inline int* GetIntRef(ImGuiStorage* self, ImGuiID key, int default_val) {
            using func_t            = int* (*)(ImGuiStorage*, ImGuiID, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stora"
                                                                                             "ge_"
                                                                                             "GetIn"
                                                                                             "tRe"
                                                                                             "f"));
            return func(self, key, default_val);
        }

        inline bool* GetBoolRef(ImGuiStorage* self, ImGuiID key, bool default_val) {
            using func_t           = bool* (*)(ImGuiStorage*, ImGuiID, bool);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "torage"
                                                                                            "_GetBo"
                                                                                            "olRe"
                                                                                            "f"));
            return func(self, key, default_val);
        }

        inline float* GetFloatRef(ImGuiStorage* self, ImGuiID key, float default_val) {
            using func_t           = float* (*)(ImGuiStorage*, ImGuiID, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "torage"
                                                                                            "_GetFl"
                                                                                            "oatRe"
                                                                                            "f"));
            return func(self, key, default_val);
        }

        inline void** GetVoidPtrRef(ImGuiStorage* self, ImGuiID key, void* default_val) {
            using func_t           = void** (*)(ImGuiStorage*, ImGuiID, void*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "torage"
                                                                                            "_GetVo"
                                                                                            "idPtrR"
                                                                                            "ef"));
            return func(self, key, default_val);
        }

        inline void BuildSortByKey(ImGuiStorage* self) {
            using func_t            = void (*)(ImGuiStorage*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stora"
                                                                                             "ge_"
                                                                                             "Build"
                                                                                             "SortB"
                                                                                             "yKe"
                                                                                             "y"));
            return func(self);
        }

        inline void SetAllInt(ImGuiStorage* self, int val) {
            using func_t            = void (*)(ImGuiStorage*, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stora"
                                                                                             "ge_"
                                                                                             "SetAl"
                                                                                             "lIn"
                                                                                             "t"));
            return func(self, val);
        }
    }  // namespace ImGuiStorageManger

    namespace ImGuiListClipperManager {

        inline ImGuiListClipper* Create(void) {
            using func_t           = ImGuiListClipper* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiL"
                                                                                            "istCli"
                                                                                            "pper_"
                                                                                            "ImGuiL"
                                                                                            "istCli"
                                                                                            "ppe"
                                                                                            "r"));
            return func();
        }

        inline void Destroy(ImGuiListClipper* self) {
            using func_t            = void (*)(ImGuiListClipper*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "ListC"
                                                                                             "lippe"
                                                                                             "r_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void Begin(ImGuiListClipper* self, int items_count, float items_height) {
            using func_t            = void (*)(ImGuiListClipper*, int, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "ListC"
                                                                                             "lippe"
                                                                                             "r_"
                                                                                             "Begi"
                                                                                             "n"));
            return func(self, items_count, items_height);
        }

        inline void End(ImGuiListClipper* self) {
            using func_t            = void (*)(ImGuiListClipper*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "ListC"
                                                                                             "lippe"
                                                                                             "r_"
                                                                                             "En"
                                                                                             "d"));
            return func(self);
        }

        inline bool Step(ImGuiListClipper* self) {
            using func_t            = bool (*)(ImGuiListClipper*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "ListC"
                                                                                             "lippe"
                                                                                             "r_"
                                                                                             "Ste"
                                                                                             "p"));
            return func(self);
        }

        inline void IncludeItemByIndex(ImGuiListClipper* self, int item_index) {
            using func_t            = void (*)(ImGuiListClipper*, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "ListC"
                                                                                             "lippe"
                                                                                             "r_"
                                                                                             "Inclu"
                                                                                             "deIte"
                                                                                             "mByIn"
                                                                                             "de"
                                                                                             "x"));
            return func(self, item_index);
        }

        inline void IncludeItemsByIndex(ImGuiListClipper* self, int item_begin, int item_end) {
            using func_t            = void (*)(ImGuiListClipper*, int, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "ListC"
                                                                                             "lippe"
                                                                                             "r_"
                                                                                             "Inclu"
                                                                                             "deIte"
                                                                                             "msByI"
                                                                                             "nde"
                                                                                             "x"));
            return func(self, item_begin, item_end);
        }
    }  // namespace ImGuiListClipperManager

    namespace ImColorManager {
        inline ImColor* Create(void) {
            using func_t           = ImColor* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImColo"
                                                                                            "r_"
                                                                                            "ImColo"
                                                                                            "r_"
                                                                                            "Nil"));
            return func();
        }

        inline void Destroy(ImColor* self) {
            using func_t            = void (*)(ImColor*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImCol"
                                                                                             "or_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImColor* Create(float r, float g, float b, float a) {
            using func_t           = ImColor* (*)(float, float, float, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImColo"
                                                                                            "r_"
                                                                                            "ImColo"
                                                                                            "r_"
                                                                                            "Floa"
                                                                                            "t"));
            return func(r, g, b, a);
        }

        inline ImColor* Create(const ImVec4 col) {
            using func_t           = ImColor* (*)(const ImVec4);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImColo"
                                                                                            "r_"
                                                                                            "ImColo"
                                                                                            "r_"
                                                                                            "Vec"
                                                                                            "4"));
            return func(col);
        }

        inline ImColor* Create(int r, int g, int b, int a) {
            using func_t           = ImColor* (*)(int, int, int, int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImColo"
                                                                                            "r_"
                                                                                            "ImColo"
                                                                                            "r_"
                                                                                            "Int"));
            return func(r, g, b, a);
        }

        inline ImColor* Create(ImU32 rgba) {
            using func_t           = ImColor* (*)(ImU32);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImColo"
                                                                                            "r_"
                                                                                            "ImColo"
                                                                                            "r_"
                                                                                            "U32"));
            return func(rgba);
        }

        inline void SetHSV(ImColor* self, float h, float s, float v, float a) {
            using func_t            = void (*)(ImColor*, float, float, float, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImCol"
                                                                                             "or_"
                                                                                             "SetHS"
                                                                                             "V"));
            return func(self, h, s, v, a);
        }

        inline void HSV(ImColor* pOut, float h, float s, float v, float a) {
            using func_t            = void (*)(ImColor*, float, float, float, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImCol"
                                                                                             "or_"
                                                                                             "HS"
                                                                                             "V"));
            return func(pOut, h, s, v, a);
        }
    }  // namespace ImColorManager

    namespace ImDrawCmdManager {
        inline ImDrawCmd* Create(void) {
            using func_t           = ImDrawCmd* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "Cmd_"
                                                                                            "ImDraw"
                                                                                            "Cmd"));
            return func();
        }

        inline void Destroy(ImDrawCmd* self) {
            using func_t            = void (*)(ImDrawCmd*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wCmd_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImTextureID GetTexID(ImDrawCmd* self) {
            using func_t           = ImTextureID (*)(ImDrawCmd*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "Cmd_"
                                                                                            "GetTex"
                                                                                            "ID"));
            return func(self);
        }
    }  // namespace ImDrawCmdManager

    namespace ImDrawListSplitterManager {
        inline ImDrawListSplitter* Create(void) {
            using func_t           = ImDrawListSplitter* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "ListSp"
                                                                                            "litter"
                                                                                            "_ImDra"
                                                                                            "wListS"
                                                                                            "plitte"
                                                                                            "r"));
            return func();
        }

        inline void Destroy(ImDrawListSplitter* self) {
            using func_t            = void (*)(ImDrawListSplitter*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "Split"
                                                                                             "ter_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void Clear(ImDrawListSplitter* self) {
            using func_t            = void (*)(ImDrawListSplitter*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "Split"
                                                                                             "ter_"
                                                                                             "Clea"
                                                                                             "r"));
            return func(self);
        }

        inline void ClearFreeMemory(ImDrawListSplitter* self) {
            using func_t            = void (*)(ImDrawListSplitter*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "Split"
                                                                                             "ter_"
                                                                                             "Clear"
                                                                                             "FreeM"
                                                                                             "emor"
                                                                                             "y"));
            return func(self);
        }

        inline void Split(ImDrawListSplitter* self, ImDrawList* draw_list, int count) {
            using func_t            = void (*)(ImDrawListSplitter*, ImDrawList*, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "Split"
                                                                                             "ter_"
                                                                                             "Spli"
                                                                                             "t"));
            return func(self, draw_list, count);
        }

        inline void Merge(ImDrawListSplitter* self, ImDrawList* draw_list) {
            using func_t            = void (*)(ImDrawListSplitter*, ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "Split"
                                                                                             "ter_"
                                                                                             "Merg"
                                                                                             "e"));
            return func(self, draw_list);
        }

        inline void SetCurrentChannel(ImDrawListSplitter* self, ImDrawList* draw_list, int channel_idx) {
            using func_t            = void (*)(ImDrawListSplitter*, ImDrawList*, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "Split"
                                                                                             "ter_"
                                                                                             "SetCu"
                                                                                             "rrent"
                                                                                             "Chann"
                                                                                             "el"));
            return func(self, draw_list, channel_idx);
        }
    }  // namespace ImDrawListSplitterManager

    namespace ImDrawListManager {

        inline ImDrawList* Create(ImDrawListSharedData* shared_data) {
            using func_t           = ImDrawList* (*)(ImDrawListSharedData*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "List_"
                                                                                            "ImDraw"
                                                                                            "Lis"
                                                                                            "t"));
            return func(shared_data);
        }

        inline void Destroy(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }

        inline void PushClipRect(ImDrawList* self, const ImVec2 clip_rect_min, const ImVec2 clip_rect_max, bool intersect_with_current_clip_rect) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, const ImVec2, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Push"
                                                                                             "ClipR"
                                                                                             "ec"
                                                                                             "t"));
            return func(self, clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
        }

        inline void PushClipRectFullScreen(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Push"
                                                                                             "ClipR"
                                                                                             "ectFu"
                                                                                             "llScr"
                                                                                             "ee"
                                                                                             "n"));
            return func(self);
        }

        inline void PopClipRect(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_PopC"
                                                                                             "lipRe"
                                                                                             "ct"));
            return func(self);
        }

        inline void PushTextureID(ImDrawList* self, ImTextureID texture_id) {
            using func_t            = void (*)(ImDrawList*, ImTextureID);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Push"
                                                                                             "Textu"
                                                                                             "reI"
                                                                                             "D"));
            return func(self, texture_id);
        }

        inline void PopTextureID(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_PopT"
                                                                                             "extur"
                                                                                             "eI"
                                                                                             "D"));
            return func(self);
        }

        inline void GetClipRectMin(ImVec2* pOut, ImDrawList* self) {
            using func_t            = void (*)(ImVec2*, ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_GetC"
                                                                                             "lipRe"
                                                                                             "ctMi"
                                                                                             "n"));
            return func(pOut, self);
        }

        inline void GetClipRectMax(ImVec2* pOut, ImDrawList* self) {
            using func_t            = void (*)(ImVec2*, ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_GetC"
                                                                                             "lipRe"
                                                                                             "ctMa"
                                                                                             "x"));
            return func(pOut, self);
        }

        inline void AddLine(ImDrawList* self, const ImVec2 p1, const ImVec2 p2, ImU32 col, float thickness) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, ImU32, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddL"
                                                                                             "in"
                                                                                             "e"));
            return func(self, p1, p2, col, thickness);
        }

        inline void AddRect(ImDrawList* self, const ImVec2 p_min, const ImVec2 p_max, ImU32 col, float rounding, ImDrawFlags flags, float thickness) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, ImU32, float, ImDrawFlags, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddR"
                                                                                             "ec"
                                                                                             "t"));
            return func(self, p_min, p_max, col, rounding, flags, thickness);
        }

        inline void AddRectFilled(ImDrawList* self, const ImVec2 p_min, const ImVec2 p_max, ImU32 col, float rounding, ImDrawFlags flags) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, ImU32, float, ImDrawFlags);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddR"
                                                                                             "ectFi"
                                                                                             "lle"
                                                                                             "d"));
            return func(self, p_min, p_max, col, rounding, flags);
        }

        inline void AddRectFilledMultiColor(ImDrawList* self, const ImVec2 p_min, const ImVec2 p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, ImU32, ImU32, ImU32, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddR"
                                                                                             "ectFi"
                                                                                             "lledM"
                                                                                             "ultiC"
                                                                                             "olo"
                                                                                             "r"));
            return func(self, p_min, p_max, col_upr_left, col_upr_right, col_bot_right, col_bot_left);
        }

        inline void AddQuad(ImDrawList* self, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, ImU32 col, float thickness) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, const ImVec2, const ImVec2, ImU32, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddQ"
                                                                                             "ua"
                                                                                             "d"));
            return func(self, p1, p2, p3, p4, col, thickness);
        }

        inline void AddQuadFilled(ImDrawList* self, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, ImU32 col) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, const ImVec2, const ImVec2, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddQ"
                                                                                             "uadFi"
                                                                                             "lle"
                                                                                             "d"));
            return func(self, p1, p2, p3, p4, col);
        }

        inline void AddTriangle(ImDrawList* self, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, ImU32 col, float thickness) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, const ImVec2, ImU32, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddT"
                                                                                             "riang"
                                                                                             "le"));
            return func(self, p1, p2, p3, col, thickness);
        }

        inline void AddTriangleFilled(ImDrawList* self, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, ImU32 col) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, const ImVec2, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddT"
                                                                                             "riang"
                                                                                             "leFil"
                                                                                             "le"
                                                                                             "d"));
            return func(self, p1, p2, p3, col);
        }

        inline void AddCircle(ImDrawList* self, const ImVec2 center, float radius, ImU32 col, int num_segments, float thickness) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, float, ImU32, int, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddC"
                                                                                             "ircl"
                                                                                             "e"));
            return func(self, center, radius, col, num_segments, thickness);
        }

        inline void AddCircleFilled(ImDrawList* self, const ImVec2 center, float radius, ImU32 col, int num_segments) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, float, ImU32, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddC"
                                                                                             "ircle"
                                                                                             "Fille"
                                                                                             "d"));
            return func(self, center, radius, col, num_segments);
        }

        inline void AddNgon(ImDrawList* self, const ImVec2 center, float radius, ImU32 col, int num_segments, float thickness) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, float, ImU32, int, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddN"
                                                                                             "go"
                                                                                             "n"));
            return func(self, center, radius, col, num_segments, thickness);
        }

        inline void AddNgonFilled(ImDrawList* self, const ImVec2 center, float radius, ImU32 col, int num_segments) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, float, ImU32, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddN"
                                                                                             "gonFi"
                                                                                             "lle"
                                                                                             "d"));
            return func(self, center, radius, col, num_segments);
        }

        inline void AddEllipse(ImDrawList* self, const ImVec2 center, const ImVec2 radius, ImU32 col, float rot, int num_segments, float thickness) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, ImU32, float, int, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddE"
                                                                                             "llips"
                                                                                             "e"));
            return func(self, center, radius, col, rot, num_segments, thickness);
        }

        inline void AddEllipseFilled(ImDrawList* self, const ImVec2 center, const ImVec2 radius, ImU32 col, float rot, int num_segments) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, ImU32, float, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddE"
                                                                                             "llips"
                                                                                             "eFill"
                                                                                             "ed"));
            return func(self, center, radius, col, rot, num_segments);
        }

        inline void AddText_Vec2(ImDrawList* self, const ImVec2 pos, ImU32 col, const char* text_begin, const char* text_end) {
            using func_t = void (*)(ImDrawList*, const ImVec2, ImU32, const char*, const char*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddT"
                                                                                             "ext_"
                                                                                             "Vec"
                                                                                             "2"));
            return func(self, pos, col, text_begin, text_end);
        }

        inline void AddText_FontPtr(ImDrawList* self, const ImFont* font, float font_size, const ImVec2 pos, ImU32 col, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect) {
            using func_t = void (*)(ImDrawList*, const ImFont*, float, const ImVec2, ImU32, const char*, const char*, float, const ImVec4*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddT"
                                                                                             "ext_"
                                                                                             "FontP"
                                                                                             "tr"));
            return func(self, font, font_size, pos, col, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
        }

        inline void AddBezierCubic(ImDrawList* self, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, ImU32 col, float thickness, int num_segments) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, const ImVec2, const ImVec2, ImU32, float, int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "List_"
                                                                                            "AddBez"
                                                                                            "ierCub"
                                                                                            "ic"));
            return func(self, p1, p2, p3, p4, col, thickness, num_segments);
        }

        inline void AddBezierQuadratic(ImDrawList* self, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, ImU32 col, float thickness, int num_segments) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, const ImVec2, ImU32, float, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddB"
                                                                                             "ezier"
                                                                                             "Quadr"
                                                                                             "ati"
                                                                                             "c"));
            return func(self, p1, p2, p3, col, thickness, num_segments);
        }

        inline void AddPolyline(ImDrawList* self, const ImVec2* points, int num_points, ImU32 col, ImDrawFlags flags, float thickness) {
            using func_t = void (*)(ImDrawList*, const ImVec2*, int, ImU32, ImDrawFlags, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddP"
                                                                                             "olyli"
                                                                                             "ne"));
            return func(self, points, num_points, col, flags, thickness);
        }

        inline void AddConvexPolyFilled(ImDrawList* self, const ImVec2* points, int num_points, ImU32 col) {
            using func_t            = void (*)(ImDrawList*, const ImVec2*, int, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddC"
                                                                                             "onvex"
                                                                                             "PolyF"
                                                                                             "ille"
                                                                                             "d"));
            return func(self, points, num_points, col);
        }

        inline void AddConcavePolyFilled(ImDrawList* self, const ImVec2* points, int num_points, ImU32 col) {
            using func_t            = void (*)(ImDrawList*, const ImVec2*, int, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddC"
                                                                                             "oncav"
                                                                                             "ePoly"
                                                                                             "Fille"
                                                                                             "d"));
            return func(self, points, num_points, col);
        }

        inline void AddImage(ImDrawList* self, ImTextureID user_texture_id, const ImVec2 p_min, const ImVec2 p_max, const ImVec2 uv_min, const ImVec2 uv_max, ImU32 col) {
            using func_t = void (*)(ImDrawList*, ImTextureID, const ImVec2, const ImVec2, const ImVec2, const ImVec2, ImU32);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "List_"
                                                                                            "AddIma"
                                                                                            "ge"));
            return func(self, user_texture_id, p_min, p_max, uv_min, uv_max, col);
        }

        inline void AddImageQuad(ImDrawList* self, ImTextureID user_texture_id, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, const ImVec2 uv1, const ImVec2 uv2, const ImVec2 uv3, const ImVec2 uv4, ImU32 col) {
            using func_t = void (*)(ImDrawList*, ImTextureID, const ImVec2, const ImVec2, const ImVec2, const ImVec2, const ImVec2, const ImVec2, const ImVec2, const ImVec2, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddI"
                                                                                             "mageQ"
                                                                                             "ua"
                                                                                             "d"));
            return func(self, user_texture_id, p1, p2, p3, p4, uv1, uv2, uv3, uv4, col);
        }

        inline void AddImageRounded(ImDrawList* self, ImTextureID user_texture_id, const ImVec2 p_min, const ImVec2 p_max, const ImVec2 uv_min, const ImVec2 uv_max, ImU32 col, float rounding, ImDrawFlags flags) {
            using func_t = void (*)(ImDrawList*, ImTextureID, const ImVec2, const ImVec2, const ImVec2, const ImVec2, ImU32, float, ImDrawFlags);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddI"
                                                                                             "mageR"
                                                                                             "ounde"
                                                                                             "d"));
            return func(self, user_texture_id, p_min, p_max, uv_min, uv_max, col, rounding, flags);
        }

        inline void PathClear(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "Clea"
                                                                                             "r"));
            return func(self);
        }

        inline void PathLineTo(ImDrawList* self, const ImVec2 pos) {
            using func_t            = void (*)(ImDrawList*, const ImVec2);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "LineT"
                                                                                             "o"));
            return func(self, pos);
        }

        inline void PathLineToMergeDuplicate(ImDrawList* self, const ImVec2 pos) {
            using func_t            = void (*)(ImDrawList*, const ImVec2);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "LineT"
                                                                                             "oMerg"
                                                                                             "eDupl"
                                                                                             "icat"
                                                                                             "e"));
            return func(self, pos);
        }

        inline void PathFillConvex(ImDrawList* self, ImU32 col) {
            using func_t            = void (*)(ImDrawList*, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "FillC"
                                                                                             "onve"
                                                                                             "x"));
            return func(self, col);
        }

        inline void PathFillConcave(ImDrawList* self, ImU32 col) {
            using func_t            = void (*)(ImDrawList*, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "FillC"
                                                                                             "oncav"
                                                                                             "e"));
            return func(self, col);
        }

        inline void PathStroke(ImDrawList* self, ImU32 col, ImDrawFlags flags, float thickness) {
            using func_t            = void (*)(ImDrawList*, ImU32, ImDrawFlags, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "Strok"
                                                                                             "e"));
            return func(self, col, flags, thickness);
        }

        inline void PathArcTo(ImDrawList* self, const ImVec2 center, float radius, float a_min, float a_max, int num_segments) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, float, float, float, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "ArcT"
                                                                                             "o"));
            return func(self, center, radius, a_min, a_max, num_segments);
        }

        inline void PathArcToFast(ImDrawList* self, const ImVec2 center, float radius, int a_min_of_12, int a_max_of_12) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, float, int, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "ArcTo"
                                                                                             "Fas"
                                                                                             "t"));
            return func(self, center, radius, a_min_of_12, a_max_of_12);
        }

        inline void PathEllipticalArcTo(ImDrawList* self, const ImVec2 center, const ImVec2 radius, float rot, float a_min, float a_max, int num_segments) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, float, float, float, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "Ellip"
                                                                                             "tical"
                                                                                             "ArcT"
                                                                                             "o"));
            return func(self, center, radius, rot, a_min, a_max, num_segments);
        }

        inline void PathBezierCubicCurveTo(ImDrawList* self, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, int num_segments) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, const ImVec2, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "Bezie"
                                                                                             "rCubi"
                                                                                             "cCurv"
                                                                                             "eT"
                                                                                             "o"));
            return func(self, p2, p3, p4, num_segments);
        }

        inline void PathBezierQuadraticCurveTo(ImDrawList* self, const ImVec2 p2, const ImVec2 p3, int num_segments) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, const ImVec2, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "Bezie"
                                                                                             "rQuad"
                                                                                             "ratic"
                                                                                             "Curve"
                                                                                             "To"));
            return func(self, p2, p3, num_segments);
        }

        inline void PathRect(ImDrawList* self, const ImVec2 rect_min, const ImVec2 rect_max, float rounding, ImDrawFlags flags) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, float, ImDrawFlags);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Path"
                                                                                             "Rec"
                                                                                             "t"));
            return func(self, rect_min, rect_max, rounding, flags);
        }

        inline void AddCallback(ImDrawList* self, ImDrawCallback callback, void* callback_data) {
            using func_t            = void (*)(ImDrawList*, ImDrawCallback, void*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddC"
                                                                                             "allba"
                                                                                             "ck"));
            return func(self, callback, callback_data);
        }

        inline void AddDrawCmd(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_AddD"
                                                                                             "rawCm"
                                                                                             "d"));
            return func(self);
        }

        inline ImDrawList* CloneOutput(ImDrawList* self) {
            using func_t           = ImDrawList* (*)(ImDrawList*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "List_"
                                                                                            "CloneO"
                                                                                            "utpu"
                                                                                            "t"));
            return func(self);
        }

        inline void ChannelsSplit(ImDrawList* self, int count) {
            using func_t            = void (*)(ImDrawList*, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Chan"
                                                                                             "nelsS"
                                                                                             "pli"
                                                                                             "t"));
            return func(self, count);
        }

        inline void ChannelsMerge(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Chan"
                                                                                             "nelsM"
                                                                                             "erg"
                                                                                             "e"));
            return func(self);
        }

        inline void ChannelsSetCurrent(ImDrawList* self, int n) {
            using func_t            = void (*)(ImDrawList*, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Chan"
                                                                                             "nelsS"
                                                                                             "etCur"
                                                                                             "ren"
                                                                                             "t"));
            return func(self, n);
        }

        inline void PrimReserve(ImDrawList* self, int idx_count, int vtx_count) {
            using func_t            = void (*)(ImDrawList*, int, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Prim"
                                                                                             "Reser"
                                                                                             "ve"));
            return func(self, idx_count, vtx_count);
        }

        inline void PrimUnreserve(ImDrawList* self, int idx_count, int vtx_count) {
            using func_t            = void (*)(ImDrawList*, int, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Prim"
                                                                                             "Unres"
                                                                                             "erv"
                                                                                             "e"));
            return func(self, idx_count, vtx_count);
        }

        inline void PrimRect(ImDrawList* self, const ImVec2 a, const ImVec2 b, ImU32 col) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, const ImVec2, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Prim"
                                                                                             "Rec"
                                                                                             "t"));
            return func(self, a, b, col);
        }

        inline void PrimRectUV(ImDrawList* self, const ImVec2 a, const ImVec2 b, const ImVec2 uv_a, const ImVec2 uv_b, ImU32 col) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, const ImVec2, const ImVec2, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Prim"
                                                                                             "RectU"
                                                                                             "V"));
            return func(self, a, b, uv_a, uv_b, col);
        }

        inline void PrimQuadUV(ImDrawList* self, const ImVec2 a, const ImVec2 b, const ImVec2 c, const ImVec2 d, const ImVec2 uv_a, const ImVec2 uv_b, const ImVec2 uv_c, const ImVec2 uv_d, ImU32 col) {
            using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, const ImVec2, const ImVec2, const ImVec2, const ImVec2, const ImVec2, const ImVec2, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Prim"
                                                                                             "QuadU"
                                                                                             "V"));
            return func(self, a, b, c, d, uv_a, uv_b, uv_c, uv_d, col);
        }

        inline void PrimWriteVtx(ImDrawList* self, const ImVec2 pos, const ImVec2 uv, ImU32 col) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, const ImVec2, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Prim"
                                                                                             "Write"
                                                                                             "Vt"
                                                                                             "x"));
            return func(self, pos, uv, col);
        }

        inline void PrimWriteIdx(ImDrawList* self, ImDrawIdx idx) {
            using func_t            = void (*)(ImDrawList*, ImDrawIdx);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Prim"
                                                                                             "Write"
                                                                                             "Id"
                                                                                             "x"));
            return func(self, idx);
        }

        inline void PrimVtx(ImDrawList* self, const ImVec2 pos, const ImVec2 uv, ImU32 col) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, const ImVec2, ImU32);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "_Prim"
                                                                                             "Vt"
                                                                                             "x"));
            return func(self, pos, uv, col);
        }

        inline void _ResetForNewFrame(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "__"
                                                                                             "Reset"
                                                                                             "ForNe"
                                                                                             "wFram"
                                                                                             "e"));
            return func(self);
        }

        inline void _ClearFreeMemory(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "__"
                                                                                             "Clear"
                                                                                             "FreeM"
                                                                                             "emor"
                                                                                             "y"));
            return func(self);
        }

        inline void _PopUnusedDrawCmd(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "__"
                                                                                             "PopUn"
                                                                                             "usedD"
                                                                                             "rawCm"
                                                                                             "d"));
            return func(self);
        }

        inline void _TryMergeDrawCmds(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "__"
                                                                                             "TryMe"
                                                                                             "rgeDr"
                                                                                             "awCmd"
                                                                                             "s"));
            return func(self);
        }

        inline void _OnChangedClipRect(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "__"
                                                                                             "OnCha"
                                                                                             "ngedC"
                                                                                             "lipRe"
                                                                                             "ct"));
            return func(self);
        }

        inline void _OnChangedTextureID(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "__"
                                                                                             "OnCha"
                                                                                             "ngedT"
                                                                                             "extur"
                                                                                             "eI"
                                                                                             "D"));
            return func(self);
        }

        inline void _OnChangedVtxOffset(ImDrawList* self) {
            using func_t            = void (*)(ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "__"
                                                                                             "OnCha"
                                                                                             "ngedV"
                                                                                             "txOff"
                                                                                             "se"
                                                                                             "t"));
            return func(self);
        }

        inline int _CalcCircleAutoSegmentCount(ImDrawList* self, float radius) {
            using func_t           = int (*)(ImDrawList*, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "List__"
                                                                                            "CalcCi"
                                                                                            "rcleAu"
                                                                                            "toSegm"
                                                                                            "entCou"
                                                                                            "nt"));
            return func(self, radius);
        }

        inline void _PathArcToFastEx(ImDrawList* self, const ImVec2 center, float radius, int a_min_sample, int a_max_sample, int a_step) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, float, int, int, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "__"
                                                                                             "PathA"
                                                                                             "rcToF"
                                                                                             "astE"
                                                                                             "x"));
            return func(self, center, radius, a_min_sample, a_max_sample, a_step);
        }

        inline void _PathArcToN(ImDrawList* self, const ImVec2 center, float radius, float a_min, float a_max, int num_segments) {
            using func_t            = void (*)(ImDrawList*, const ImVec2, float, float, float, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "__"
                                                                                             "PathA"
                                                                                             "rcTo"
                                                                                             "N"));
            return func(self, center, radius, a_min, a_max, num_segments);
        }
    }  // namespace ImDrawListManager

    namespace ImDrawDataManager {

        inline ImDrawData* Create(void) {
            using func_t           = ImDrawData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "Data_"
                                                                                            "ImDraw"
                                                                                            "Dat"
                                                                                            "a"));
            return func();
        }

        inline void Destroy(ImDrawData* self) {
            using func_t            = void (*)(ImDrawData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wData"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }

        inline void Clear(ImDrawData* self) {
            using func_t            = void (*)(ImDrawData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wData"
                                                                                             "_Clea"
                                                                                             "r"));
            return func(self);
        }

        inline void AddDrawList(ImDrawData* self, ImDrawList* draw_list) {
            using func_t            = void (*)(ImDrawData*, ImDrawList*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wData"
                                                                                             "_AddD"
                                                                                             "rawLi"
                                                                                             "st"));
            return func(self, draw_list);
        }

        inline void DeIndexAllBuffers(ImDrawData* self) {
            using func_t            = void (*)(ImDrawData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wData"
                                                                                             "_DeIn"
                                                                                             "dexAl"
                                                                                             "lBuff"
                                                                                             "er"
                                                                                             "s"));
            return func(self);
        }

        inline void ScaleClipRects(ImDrawData* self, const ImVec2 fb_scale) {
            using func_t            = void (*)(ImDrawData*, const ImVec2);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wData"
                                                                                             "_Scal"
                                                                                             "eClip"
                                                                                             "Rect"
                                                                                             "s"));
            return func(self, fb_scale);
        }

    }  // namespace ImDrawDataManager

    namespace ImFontConfigManager {

        inline ImFontConfig* Create(void) {
            using func_t           = ImFontConfig* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Config"
                                                                                            "_ImFon"
                                                                                            "tConfi"
                                                                                            "g"));
            return func();
        }

        inline void Destroy(ImFontConfig* self) {
            using func_t            = void (*)(ImFontConfig*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tConf"
                                                                                             "ig_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImFontConfigManager

    namespace ImFontGlyphRangesBuilderManager {

        inline ImFontGlyphRangesBuilder* Create(void) {
            using func_t           = ImFontGlyphRangesBuilder* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "GlyphR"
                                                                                            "angesB"
                                                                                            "uilder"
                                                                                            "_ImFon"
                                                                                            "tGlyph"
                                                                                            "Ranges"
                                                                                            "Builde"
                                                                                            "r"));
            return func();
        }

        inline void Destroy(ImFontGlyphRangesBuilder* self) {
            using func_t            = void (*)(ImFontGlyphRangesBuilder*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tGlyp"
                                                                                             "hRang"
                                                                                             "esBui"
                                                                                             "lder_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void Clear(ImFontGlyphRangesBuilder* self) {
            using func_t            = void (*)(ImFontGlyphRangesBuilder*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tGlyp"
                                                                                             "hRang"
                                                                                             "esBui"
                                                                                             "lder_"
                                                                                             "Clea"
                                                                                             "r"));
            return func(self);
        }

        inline bool GetBit(ImFontGlyphRangesBuilder* self, size_t n) {
            using func_t            = bool (*)(ImFontGlyphRangesBuilder*, size_t);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tGlyp"
                                                                                             "hRang"
                                                                                             "esBui"
                                                                                             "lder_"
                                                                                             "GetBi"
                                                                                             "t"));
            return func(self, n);
        }

        inline void SetBit(ImFontGlyphRangesBuilder* self, size_t n) {
            using func_t            = void (*)(ImFontGlyphRangesBuilder*, size_t);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tGlyp"
                                                                                             "hRang"
                                                                                             "esBui"
                                                                                             "lder_"
                                                                                             "SetBi"
                                                                                             "t"));
            return func(self, n);
        }

        inline void AddChar(ImFontGlyphRangesBuilder* self, ImWchar c) {
            using func_t            = void (*)(ImFontGlyphRangesBuilder*, ImWchar);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tGlyp"
                                                                                             "hRang"
                                                                                             "esBui"
                                                                                             "lder_"
                                                                                             "AddCh"
                                                                                             "ar"));
            return func(self, c);
        }

        inline void AddText(ImFontGlyphRangesBuilder* self, const char* text, const char* text_end) {
            using func_t            = void (*)(ImFontGlyphRangesBuilder*, const char*, const char*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tGlyp"
                                                                                             "hRang"
                                                                                             "esBui"
                                                                                             "lder_"
                                                                                             "AddTe"
                                                                                             "xt"));
            return func(self, text, text_end);
        }

        inline void AddRanges(ImFontGlyphRangesBuilder* self, const ImWchar* ranges) {
            using func_t            = void (*)(ImFontGlyphRangesBuilder*, const ImWchar*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tGlyp"
                                                                                             "hRang"
                                                                                             "esBui"
                                                                                             "lder_"
                                                                                             "AddRa"
                                                                                             "nge"
                                                                                             "s"));
            return func(self, ranges);
        }

        inline void BuildRanges(ImFontGlyphRangesBuilder* self, ImVector_ImWchar* out_ranges) {
            using func_t            = void (*)(ImFontGlyphRangesBuilder*, ImVector_ImWchar*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tGlyp"
                                                                                             "hRang"
                                                                                             "esBui"
                                                                                             "lder_"
                                                                                             "Build"
                                                                                             "Range"
                                                                                             "s"));
            return func(self, out_ranges);
        }
    }  // namespace ImFontGlyphRangesBuilderManager

    namespace ImFontAtlasCustomRectManager {
        inline ImFontAtlasCustomRect* Create(void) {
            using func_t           = ImFontAtlasCustomRect* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "AtlasC"
                                                                                            "ustomR"
                                                                                            "ect_"
                                                                                            "ImFont"
                                                                                            "AtlasC"
                                                                                            "ustomR"
                                                                                            "ect"));
            return func();
        }

        inline void Destroy(ImFontAtlasCustomRect* self) {
            using func_t            = void (*)(ImFontAtlasCustomRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "sCust"
                                                                                             "omRec"
                                                                                             "t_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline bool IsPacked(ImFontAtlasCustomRect* self) {
            using func_t            = bool (*)(ImFontAtlasCustomRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "sCust"
                                                                                             "omRec"
                                                                                             "t_"
                                                                                             "IsPac"
                                                                                             "ke"
                                                                                             "d"));
            return func(self);
        }
    }  // namespace ImFontAtlasCustomRectManager

    namespace ImFontAtlasManager {
        inline ImFontAtlas* Create(void) {
            using func_t           = ImFontAtlas* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "ImFont"
                                                                                            "Atla"
                                                                                            "s"));
            return func();
        }

        inline void Destroy(ImFontAtlas* self) {
            using func_t            = void (*)(ImFontAtlas*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImFont* AddFont(ImFontAtlas* self, const ImFontConfig* font_cfg) {
            using func_t           = ImFont* (*)(ImFontAtlas*, const ImFontConfig*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "AddFon"
                                                                                            "t"));
            return func(self, font_cfg);
        }

        inline ImFont* AddFontDefault(ImFontAtlas* self, const ImFontConfig* font_cfg) {
            using func_t           = ImFont* (*)(ImFontAtlas*, const ImFontConfig*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "AddFon"
                                                                                            "tDefau"
                                                                                            "lt"));
            return func(self, font_cfg);
        }

        inline ImFont* AddFontFromFileTTF(ImFontAtlas* self, const char* filename, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges) {
            using func_t = ImFont* (*)(ImFontAtlas*, const char*, float, const ImFontConfig*, const ImWchar*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "AddFon"
                                                                                            "tFromF"
                                                                                            "ileTT"
                                                                                            "F"));
            return func(self, filename, size_pixels, font_cfg, glyph_ranges);
        }

        inline ImFont* AddFontFromMemoryTTF(ImFontAtlas* self, void* font_data, int font_data_size, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges) {
            using func_t = ImFont* (*)(ImFontAtlas*, void*, int, float, const ImFontConfig*, const ImWchar*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "AddFon"
                                                                                            "tFromM"
                                                                                            "emoryT"
                                                                                            "TF"));
            return func(self, font_data, font_data_size, size_pixels, font_cfg, glyph_ranges);
        }

        inline ImFont* AddFontFromMemoryCompressedTTF(ImFontAtlas* self, const void* compressed_font_data, int compressed_font_data_size, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges) {
            using func_t = ImFont* (*)(ImFontAtlas*, const void*, int, float, const ImFontConfig*, const ImWchar*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "AddFon"
                                                                                            "tFromM"
                                                                                            "emoryC"
                                                                                            "ompres"
                                                                                            "sedTT"
                                                                                            "F"));
            return func(self, compressed_font_data, compressed_font_data_size, size_pixels, font_cfg, glyph_ranges);
        }

        inline ImFont* AddFontFromMemoryCompressedBase85TTF(ImFontAtlas* self, const char* compressed_font_data_base85, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges) {
            using func_t = ImFont* (*)(ImFontAtlas*, const char*, float, const ImFontConfig*, const ImWchar*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "AddFon"
                                                                                            "tFromM"
                                                                                            "emoryC"
                                                                                            "ompres"
                                                                                            "sedBas"
                                                                                            "e85TT"
                                                                                            "F"));
            return func(self, compressed_font_data_base85, size_pixels, font_cfg, glyph_ranges);
        }

        inline void ClearInputData(ImFontAtlas* self) {
            using func_t            = void (*)(ImFontAtlas*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "Clear"
                                                                                             "Input"
                                                                                             "Dat"
                                                                                             "a"));
            return func(self);
        }

        inline void ClearTexData(ImFontAtlas* self) {
            using func_t            = void (*)(ImFontAtlas*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "Clear"
                                                                                             "TexDa"
                                                                                             "ta"));
            return func(self);
        }

        inline void ClearFonts(ImFontAtlas* self) {
            using func_t            = void (*)(ImFontAtlas*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "Clear"
                                                                                             "Font"
                                                                                             "s"));
            return func(self);
        }

        inline void Clear(ImFontAtlas* self) {
            using func_t            = void (*)(ImFontAtlas*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "Clea"
                                                                                             "r"));
            return func(self);
        }

        inline bool Build(ImFontAtlas* self) {
            using func_t            = bool (*)(ImFontAtlas*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "Buil"
                                                                                             "d"));
            return func(self);
        }

        inline void GetTexDataAsAlpha8(ImFontAtlas* self, unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel) {
            using func_t            = void (*)(ImFontAtlas*, unsigned char**, int*, int*, int*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "GetTe"
                                                                                             "xData"
                                                                                             "AsAlp"
                                                                                             "ha"
                                                                                             "8"));
            return func(self, out_pixels, out_width, out_height, out_bytes_per_pixel);
        }

        inline void GetTexDataAsRGBA32(ImFontAtlas* self, unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel) {
            using func_t            = void (*)(ImFontAtlas*, unsigned char**, int*, int*, int*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "GetTe"
                                                                                             "xData"
                                                                                             "AsRGB"
                                                                                             "A3"
                                                                                             "2"));
            return func(self, out_pixels, out_width, out_height, out_bytes_per_pixel);
        }

        inline bool IsBuilt(ImFontAtlas* self) {
            using func_t            = bool (*)(ImFontAtlas*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "IsBui"
                                                                                             "lt"));
            return func(self);
        }

        inline void SetTexID(ImFontAtlas* self, ImTextureID id) {
            using func_t            = void (*)(ImFontAtlas*, ImTextureID);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "SetTe"
                                                                                             "xI"
                                                                                             "D"));
            return func(self, id);
        }

        inline const ImWchar* GetGlyphRangesDefault(ImFontAtlas* self) {
            using func_t           = const ImWchar* (*)(ImFontAtlas*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "GetGly"
                                                                                            "phRang"
                                                                                            "esDefa"
                                                                                            "ult"));
            return func(self);
        }

        inline const ImWchar* GetGlyphRangesGreek(ImFontAtlas* self) {
            using func_t           = const ImWchar* (*)(ImFontAtlas*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "GetGly"
                                                                                            "phRang"
                                                                                            "esGree"
                                                                                            "k"));
            return func(self);
        }

        inline const ImWchar* GetGlyphRangesKorean(ImFontAtlas* self) {
            using func_t           = const ImWchar* (*)(ImFontAtlas*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "GetGly"
                                                                                            "phRang"
                                                                                            "esKore"
                                                                                            "an"));
            return func(self);
        }

        inline const ImWchar* GetGlyphRangesJapanese(ImFontAtlas* self) {
            using func_t           = const ImWchar* (*)(ImFontAtlas*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "GetGly"
                                                                                            "phRang"
                                                                                            "esJapa"
                                                                                            "nes"
                                                                                            "e"));
            return func(self);
        }

        inline const ImWchar* GetGlyphRangesChineseFull(ImFontAtlas* self) {
            using func_t           = const ImWchar* (*)(ImFontAtlas*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "GetGly"
                                                                                            "phRang"
                                                                                            "esChin"
                                                                                            "eseFul"
                                                                                            "l"));
            return func(self);
        }

        inline const ImWchar* GetGlyphRangesChineseSimplifiedCommon(ImFontAtlas* self) {
            using func_t           = const ImWchar* (*)(ImFontAtlas*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "GetGly"
                                                                                            "phRang"
                                                                                            "esChin"
                                                                                            "eseSim"
                                                                                            "plifie"
                                                                                            "dCommo"
                                                                                            "n"));
            return func(self);
        }

        inline const ImWchar* GetGlyphRangesCyrillic(ImFontAtlas* self) {
            using func_t           = const ImWchar* (*)(ImFontAtlas*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "GetGly"
                                                                                            "phRang"
                                                                                            "esCyri"
                                                                                            "lli"
                                                                                            "c"));
            return func(self);
        }

        inline const ImWchar* GetGlyphRangesThai(ImFontAtlas* self) {
            using func_t           = const ImWchar* (*)(ImFontAtlas*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "GetGly"
                                                                                            "phRang"
                                                                                            "esTha"
                                                                                            "i"));
            return func(self);
        }

        inline const ImWchar* GetGlyphRangesVietnamese(ImFontAtlas* self) {
            using func_t           = const ImWchar* (*)(ImFontAtlas*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "GetGly"
                                                                                            "phRang"
                                                                                            "esViet"
                                                                                            "names"
                                                                                            "e"));
            return func(self);
        }

        inline int AddCustomRectRegular(ImFontAtlas* self, int width, int height) {
            using func_t           = int (*)(ImFontAtlas*, int, int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "AddCus"
                                                                                            "tomRec"
                                                                                            "tRegul"
                                                                                            "ar"));
            return func(self, width, height);
        }

        inline int AddCustomRectFontGlyph(ImFontAtlas* self, ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2 offset) {
            using func_t = int (*)(ImFontAtlas*, ImFont*, ImWchar, int, int, float, const ImVec2);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "AddCus"
                                                                                            "tomRec"
                                                                                            "tFontG"
                                                                                            "lyp"
                                                                                            "h"));
            return func(self, font, id, width, height, advance_x, offset);
        }

        inline ImFontAtlasCustomRect* GetCustomRectByIndex(ImFontAtlas* self, int index) {
            using func_t           = ImFontAtlasCustomRect* (*)(ImFontAtlas*, int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "Atlas_"
                                                                                            "GetCus"
                                                                                            "tomRec"
                                                                                            "tByInd"
                                                                                            "ex"));
            return func(self, index);
        }

        inline void CalcCustomRectUV(ImFontAtlas* self, const ImFontAtlasCustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max) {
            using func_t = void (*)(ImFontAtlas*, const ImFontAtlasCustomRect*, ImVec2*, ImVec2*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "CalcC"
                                                                                             "ustom"
                                                                                             "RectU"
                                                                                             "V"));
            return func(self, rect, out_uv_min, out_uv_max);
        }

        inline bool GetMouseCursorTexData(ImFontAtlas* self, ImGuiMouseCursor cursor, ImVec2* out_offset, ImVec2* out_size, ImVec2 out_uv_border[2], ImVec2 out_uv_fill[2]) {
            using func_t = bool (*)(ImFontAtlas*, ImGuiMouseCursor, ImVec2*, ImVec2*, ImVec2[2], ImVec2[2]);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "tAtla"
                                                                                             "s_"
                                                                                             "GetMo"
                                                                                             "useCu"
                                                                                             "rsorT"
                                                                                             "exDat"
                                                                                             "a"));
            return func(self, cursor, out_offset, out_size, out_uv_border, out_uv_fill);
        }
    }  // namespace ImFontAtlasManager

    namespace ImFontManger {

        inline ImFont* Create(void) {
            using func_t           = ImFont* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "_ImFon"
                                                                                            "t"));
            return func();
        }

        inline void Destroy(ImFont* self) {
            using func_t            = void (*)(ImFont*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline const ImFontGlyph* FindGlyph(ImFont* self, ImWchar c) {
            using func_t           = const ImFontGlyph* (*)(ImFont*, ImWchar);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "_FindG"
                                                                                            "lyp"
                                                                                            "h"));
            return func(self, c);
        }

        inline const ImFontGlyph* FindGlyphNoFallback(ImFont* self, ImWchar c) {
            using func_t           = const ImFontGlyph* (*)(ImFont*, ImWchar);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "_FindG"
                                                                                            "lyphNo"
                                                                                            "Fallba"
                                                                                            "ck"));
            return func(self, c);
        }

        inline float GetCharAdvance(ImFont* self, ImWchar c) {
            using func_t           = float (*)(ImFont*, ImWchar);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "_GetCh"
                                                                                            "arAdva"
                                                                                            "nce"));
            return func(self, c);
        }

        inline bool IsLoaded(ImFont* self) {
            using func_t            = bool (*)(ImFont*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "IsLoa"
                                                                                             "de"
                                                                                             "d"));
            return func(self);
        }

        inline const char* GetDebugName(ImFont* self) {
            using func_t           = const char* (*)(ImFont*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "_GetDe"
                                                                                            "bugNam"
                                                                                            "e"));
            return func(self);
        }

        inline void CalcTextSizeA(ImVec2* pOut, ImFont* self, float size, float max_width, float wrap_width, const char* text_begin, const char* text_end, const char** remaining) {
            using func_t = void (*)(ImVec2*, ImFont*, float, float, float, const char*, const char*, const char**);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "CalcT"
                                                                                             "extSi"
                                                                                             "ze"
                                                                                             "A"));
            return func(pOut, self, size, max_width, wrap_width, text_begin, text_end, remaining);
        }

        inline const char* CalcWordWrapPositionA(ImFont* self, float scale, const char* text, const char* text_end, float wrap_width) {
            using func_t = const char* (*)(ImFont*, float, const char*, const char*, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFont"
                                                                                            "_CalcW"
                                                                                            "ordWra"
                                                                                            "pPosit"
                                                                                            "ion"
                                                                                            "A"));
            return func(self, scale, text, text_end, wrap_width);
        }

        inline void RenderChar(ImFont* self, ImDrawList* draw_list, float size, const ImVec2 pos, ImU32 col, ImWchar c) {
            using func_t = void (*)(ImFont*, ImDrawList*, float, const ImVec2, ImU32, ImWchar);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "Rende"
                                                                                             "rCha"
                                                                                             "r"));
            return func(self, draw_list, size, pos, col, c);
        }

        inline void RenderText(ImFont* self, ImDrawList* draw_list, float size, const ImVec2 pos, ImU32 col, const ImVec4 clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip) {
            using func_t = void (*)(ImFont*, ImDrawList*, float, const ImVec2, ImU32, const ImVec4, const char*, const char*, float, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "Rende"
                                                                                             "rTex"
                                                                                             "t"));
            return func(self, draw_list, size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip);
        }

        inline void BuildLookupTable(ImFont* self) {
            using func_t            = void (*)(ImFont*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "Build"
                                                                                             "Looku"
                                                                                             "pTabl"
                                                                                             "e"));
            return func(self);
        }

        inline void ClearOutputData(ImFont* self) {
            using func_t            = void (*)(ImFont*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "Clear"
                                                                                             "Outpu"
                                                                                             "tDat"
                                                                                             "a"));
            return func(self);
        }

        inline void GrowIndex(ImFont* self, int new_size) {
            using func_t            = void (*)(ImFont*, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "GrowI"
                                                                                             "nde"
                                                                                             "x"));
            return func(self, new_size);
        }

        inline void AddGlyph(ImFont* self, const ImFontConfig* src_cfg, ImWchar c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x) {
            using func_t = void (*)(ImFont*, const ImFontConfig*, ImWchar, float, float, float, float, float, float, float, float, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "AddGl"
                                                                                             "yp"
                                                                                             "h"));
            return func(self, src_cfg, c, x0, y0, x1, y1, u0, v0, u1, v1, advance_x);
        }

        inline void AddRemapChar(ImFont* self, ImWchar dst, ImWchar src, bool overwrite_dst) {
            using func_t            = void (*)(ImFont*, ImWchar, ImWchar, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "AddRe"
                                                                                             "mapCh"
                                                                                             "ar"));
            return func(self, dst, src, overwrite_dst);
        }

        inline void SetGlyphVisible(ImFont* self, ImWchar c, bool visible) {
            using func_t            = void (*)(ImFont*, ImWchar, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "SetGl"
                                                                                             "yphVi"
                                                                                             "sibl"
                                                                                             "e"));
            return func(self, c, visible);
        }

        inline bool IsGlyphRangeUnused(ImFont* self, unsigned int c_begin, unsigned int c_last) {
            using func_t            = bool (*)(ImFont*, unsigned int, unsigned int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImFon"
                                                                                             "t_"
                                                                                             "IsGly"
                                                                                             "phRan"
                                                                                             "geUnu"
                                                                                             "se"
                                                                                             "d"));
            return func(self, c_begin, c_last);
        }
    }  // namespace ImFontManger

    namespace ImGuiViewportManager {

        inline ImGuiViewport* Create(void) {
            using func_t           = ImGuiViewport* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiV"
                                                                                            "iewpor"
                                                                                            "t_"
                                                                                            "ImGuiV"
                                                                                            "iewpor"
                                                                                            "t"));
            return func();
        }

        inline void Destroy(ImGuiViewport* self) {
            using func_t            = void (*)(ImGuiViewport*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ort_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void GetCenter(ImVec2* pOut, ImGuiViewport* self) {
            using func_t            = void (*)(ImVec2*, ImGuiViewport*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ort_"
                                                                                             "GetCe"
                                                                                             "nte"
                                                                                             "r"));
            return func(pOut, self);
        }

        inline void GetWorkCenter(ImVec2* pOut, ImGuiViewport* self) {
            using func_t            = void (*)(ImVec2*, ImGuiViewport*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ort_"
                                                                                             "GetWo"
                                                                                             "rkCen"
                                                                                             "te"
                                                                                             "r"));
            return func(pOut, self);
        }

    }  // namespace ImGuiViewportManager

    namespace ImGuiPlatformIOManager {

        inline ImGuiPlatformIO* Create(void) {
            using func_t           = ImGuiPlatformIO* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiP"
                                                                                            "latfor"
                                                                                            "mIO_"
                                                                                            "ImGuiP"
                                                                                            "latfor"
                                                                                            "mIO"));
            return func();
        }

        inline void Destroy(ImGuiPlatformIO* self) {
            using func_t            = void (*)(ImGuiPlatformIO*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Platf"
                                                                                             "ormIO"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiPlatformIOManager

    namespace ImGuiPlatformMonitorManager {
        inline ImGuiPlatformMonitor* Create(void) {
            using func_t           = ImGuiPlatformMonitor* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiP"
                                                                                            "latfor"
                                                                                            "mMonit"
                                                                                            "or_"
                                                                                            "ImGuiP"
                                                                                            "latfor"
                                                                                            "mMonit"
                                                                                            "or"));
            return func();
        }

        inline void Destroy(ImGuiPlatformMonitor* self) {
            using func_t            = void (*)(ImGuiPlatformMonitor*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Platf"
                                                                                             "ormMo"
                                                                                             "nitor"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiPlatformMonitorManager

    namespace ImGuiPlatformImeDataManager {
        inline ImGuiPlatformImeData* Create(void) {
            using func_t           = ImGuiPlatformImeData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiP"
                                                                                            "latfor"
                                                                                            "mImeDa"
                                                                                            "ta_"
                                                                                            "ImGuiP"
                                                                                            "latfor"
                                                                                            "mImeDa"
                                                                                            "ta"));
            return func();
        }

        inline void Destroy(ImGuiPlatformImeData* self) {
            using func_t            = void (*)(ImGuiPlatformImeData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Platf"
                                                                                             "ormIm"
                                                                                             "eData"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiPlatformImeDataManager

    inline ImGuiID ImHashData(const void* data, size_t data_size, ImGuiID seed) {
        using func_t           = ImGuiID (*)(const void*, size_t, ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImHashDa"
                                                                                        "ta"));
        return func(data, data_size, seed);
    }

    inline ImGuiID ImHashStr(const char* data, size_t data_size, ImGuiID seed) {
        using func_t           = ImGuiID (*)(const char*, size_t, ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImHashSt"
                                                                                        "r"));
        return func(data, data_size, seed);
    }

    inline void ImQsort(void* base, size_t count, size_t size_of_element, int (*compare_func)(void const*, void const*)) {
        using func_t = void (*)(void*, size_t, size_t, int (*)(void const*, void const*));
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImQsor"
                                                                                         "t"));
        return func(base, count, size_of_element, compare_func);
    }

    inline ImU32 ImAlphaBlendColors(ImU32 col_a, ImU32 col_b) {
        using func_t           = ImU32 (*)(ImU32, ImU32);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImAlphaB"
                                                                                        "lendColor"
                                                                                        "s"));
        return func(col_a, col_b);
    }

    inline bool ImIsPowerOfTwo(int v) {
        using func_t            = bool (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImIsPow"
                                                                                         "erOfTwo_"
                                                                                         "Int"));
        return func(v);
    }

    inline bool ImIsPowerOfTwo(ImU64 v) {
        using func_t            = bool (*)(ImU64);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImIsPow"
                                                                                         "erOfTwo_"
                                                                                         "U64"));
        return func(v);
    }

    inline int ImUpperPowerOfTwo(int v) {
        using func_t           = int (*)(int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImUpperP"
                                                                                        "owerOfTw"
                                                                                        "o"));
        return func(v);
    }

    inline int ImStricmp(const char* str1, const char* str2) {
        using func_t           = int (*)(const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStricm"
                                                                                        "p"));
        return func(str1, str2);
    }

    inline int ImStrnicmp(const char* str1, const char* str2, size_t count) {
        using func_t           = int (*)(const char*, const char*, size_t);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStrnic"
                                                                                        "mp"));
        return func(str1, str2, count);
    }

    inline void ImStrncpy(char* dst, const char* src, size_t count) {
        using func_t            = void (*)(char*, const char*, size_t);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStrnc"
                                                                                         "py"));
        return func(dst, src, count);
    }

    inline char* ImStrdup(const char* str) {
        using func_t           = char* (*)(const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStrdu"
                                                                                        "p"));
        return func(str);
    }

    inline char* ImStrdupcpy(char* dst, size_t* p_dst_size, const char* str) {
        using func_t           = char* (*)(char*, size_t*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStrdup"
                                                                                        "cpy"));
        return func(dst, p_dst_size, str);
    }

    inline const char* ImStrchrRange(const char* str_begin, const char* str_end, char c) {
        using func_t           = const char* (*)(const char*, const char*, char);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStrchr"
                                                                                        "Range"));
        return func(str_begin, str_end, c);
    }

    inline const char* ImStreolRange(const char* str, const char* str_end) {
        using func_t           = const char* (*)(const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStreol"
                                                                                        "Range"));
        return func(str, str_end);
    }

    inline const char* ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end) {
        using func_t = const char* (*)(const char*, const char*, const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStrist"
                                                                                        "r"));
        return func(haystack, haystack_end, needle, needle_end);
    }

    inline void ImStrTrimBlanks(char* str) {
        using func_t            = void (*)(char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStrTr"
                                                                                         "imBlank"
                                                                                         "s"));
        return func(str);
    }

    inline const char* ImStrSkipBlank(const char* str) {
        using func_t           = const char* (*)(const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStrSki"
                                                                                        "pBlank"));
        return func(str);
    }

    inline int ImStrlenW(const ImWchar* str) {
        using func_t           = int (*)(const ImWchar*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStrlen"
                                                                                        "W"));
        return func(str);
    }

    inline const ImWchar* ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin) {
        using func_t           = const ImWchar* (*)(const ImWchar*, const ImWchar*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImStrbol"
                                                                                        "W"));
        return func(buf_mid_line, buf_begin);
    }

    inline char ImToUpper(char c) {
        using func_t            = char (*)(char);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImToUpp"
                                                                                         "er"));
        return func(c);
    }

    inline bool ImCharIsBlankA(char c) {
        using func_t            = bool (*)(char);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImCharI"
                                                                                         "sBlank"
                                                                                         "A"));
        return func(c);
    }

    inline bool ImCharIsBlankW(unsigned int c) {
        using func_t            = bool (*)(unsigned int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImCharI"
                                                                                         "sBlank"
                                                                                         "W"));
        return func(c);
    }

    inline int ImFormatString(char* buf, size_t buf_size, const char* fmt, ...) {
        using func_t           = int (*)(char*, size_t, const char*, va_list);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFormat"
                                                                                        "StringV"));
        va_list           args;
        va_start(args, fmt);
        int i = func(buf, buf_size, fmt, args);
        va_end(args);
        return i;
    }

    inline int ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args) {
        using func_t           = int (*)(char*, size_t, const char*, va_list);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFormat"
                                                                                        "StringV"));
        return func(buf, buf_size, fmt, args);
    }

    inline void ImFormatStringToTempBuffer(const char** out_buf, const char** out_buf_end, const char* fmt, ...) {
        using func_t            = void (*)(const char**, const char**, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImForma"
                                                                                         "tStringTo"
                                                                                         "TempBuffe"
                                                                                         "rV"));
        va_list            args;
        va_start(args, fmt);
        func(out_buf, out_buf_end, fmt, args);
        va_end(args);
    }

    inline void ImFormatStringToTempBufferV(const char** out_buf, const char** out_buf_end, const char* fmt, va_list args) {
        using func_t            = void (*)(const char**, const char**, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImForma"
                                                                                         "tStringTo"
                                                                                         "TempBuffe"
                                                                                         "rV"));
        return func(out_buf, out_buf_end, fmt, args);
    }

    inline const char* ImParseFormatFindStart(const char* format) {
        using func_t           = const char* (*)(const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImParseF"
                                                                                        "ormatFindS"
                                                                                        "tart"));
        return func(format);
    }

    inline const char* ImParseFormatFindEnd(const char* format) {
        using func_t           = const char* (*)(const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImParseF"
                                                                                        "ormatFindE"
                                                                                        "nd"));
        return func(format);
    }

    inline const char* ImParseFormatTrimDecorations(const char* format, char* buf, size_t buf_size) {
        using func_t           = const char* (*)(const char*, char*, size_t);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImParseF"
                                                                                        "ormatTrimD"
                                                                                        "ecoration"
                                                                                        "s"));
        return func(format, buf, buf_size);
    }

    inline void ImParseFormatSanitizeForPrinting(const char* fmt_in, char* fmt_out, size_t fmt_out_size) {
        using func_t            = void (*)(const char*, char*, size_t);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImParse"
                                                                                         "FormatSan"
                                                                                         "itizeForP"
                                                                                         "rintin"
                                                                                         "g"));
        return func(fmt_in, fmt_out, fmt_out_size);
    }

    inline const char* ImParseFormatSanitizeForScanning(const char* fmt_in, char* fmt_out, size_t fmt_out_size) {
        using func_t           = const char* (*)(const char*, char*, size_t);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImParseF"
                                                                                        "ormatSanit"
                                                                                        "izeForScan"
                                                                                        "ning"));
        return func(fmt_in, fmt_out, fmt_out_size);
    }

    inline int ImParseFormatPrecision(const char* format, int default_value) {
        using func_t           = int (*)(const char*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImParseF"
                                                                                        "ormatPreci"
                                                                                        "sion"));
        return func(format, default_value);
    }

    inline const char* ImTextCharToUtf8(char out_buf[5], unsigned int c) {
        using func_t           = const char* (*)(char[5], unsigned int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTextCh"
                                                                                        "arToUtf"
                                                                                        "8"));
        return func(out_buf, c);
    }

    inline int ImTextStrToUtf8(char* out_buf, int out_buf_size, const ImWchar* in_text, const ImWchar* in_text_end) {
        using func_t           = int (*)(char*, int, const ImWchar*, const ImWchar*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTextSt"
                                                                                        "rToUtf8"));
        return func(out_buf, out_buf_size, in_text, in_text_end);
    }

    inline int ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end) {
        using func_t           = int (*)(unsigned int*, const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTextCh"
                                                                                        "arFromUtf"
                                                                                        "8"));
        return func(out_char, in_text, in_text_end);
    }

    inline int ImTextStrFromUtf8(ImWchar* out_buf, int out_buf_size, const char* in_text, const char* in_text_end, const char** in_remaining) {
        using func_t           = int (*)(ImWchar*, int, const char*, const char*, const char**);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTextSt"
                                                                                        "rFromUtf"
                                                                                        "8"));
        return func(out_buf, out_buf_size, in_text, in_text_end, in_remaining);
    }

    inline int ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end) {
        using func_t           = int (*)(const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTextCo"
                                                                                        "untCharsFr"
                                                                                        "omUtf8"));
        return func(in_text, in_text_end);
    }

    inline int ImTextCountUtf8BytesFromChar(const char* in_text, const char* in_text_end) {
        using func_t           = int (*)(const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTextCo"
                                                                                        "untUtf8Byt"
                                                                                        "esFromCha"
                                                                                        "r"));
        return func(in_text, in_text_end);
    }

    inline int ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end) {
        using func_t           = int (*)(const ImWchar*, const ImWchar*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTextCo"
                                                                                        "untUtf8Byt"
                                                                                        "esFromSt"
                                                                                        "r"));
        return func(in_text, in_text_end);
    }

    inline const char* ImTextFindPreviousUtf8Codepoint(const char* in_text_start, const char* in_text_curr) {
        using func_t           = const char* (*)(const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTextFi"
                                                                                        "ndPrevious"
                                                                                        "Utf8Codepo"
                                                                                        "int"));
        return func(in_text_start, in_text_curr);
    }

    inline int ImTextCountLines(const char* in_text, const char* in_text_end) {
        using func_t           = int (*)(const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTextCo"
                                                                                        "untLine"
                                                                                        "s"));
        return func(in_text, in_text_end);
    }

    inline ImFileHandle ImFileOpen(const char* filename, const char* mode) {
        using func_t           = ImFileHandle (*)(const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFileOp"
                                                                                        "en"));
        return func(filename, mode);
    }

    inline bool ImFileClose(ImFileHandle file) {
        using func_t            = bool (*)(ImFileHandle);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFileC"
                                                                                         "lose"));
        return func(file);
    }

    inline ImU64 ImFileGetSize(ImFileHandle file) {
        using func_t           = ImU64 (*)(ImFileHandle);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFileGe"
                                                                                        "tSize"));
        return func(file);
    }

    inline ImU64 ImFileRead(void* data, ImU64 size, ImU64 count, ImFileHandle file) {
        using func_t           = ImU64 (*)(void*, ImU64, ImU64, ImFileHandle);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFileRe"
                                                                                        "ad"));
        return func(data, size, count, file);
    }

    inline ImU64 ImFileWrite(const void* data, ImU64 size, ImU64 count, ImFileHandle file) {
        using func_t           = ImU64 (*)(const void*, ImU64, ImU64, ImFileHandle);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFileWr"
                                                                                        "ite"));
        return func(data, size, count, file);
    }

    inline void* ImFileLoadToMemory(const char* filename, const char* mode, size_t* out_file_size, int padding_bytes) {
        using func_t           = void* (*)(const char*, const char*, size_t*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFileLo"
                                                                                        "adToMemor"
                                                                                        "y"));
        return func(filename, mode, out_file_size, padding_bytes);
    }

    inline float ImPow(float x, float y) {
        using func_t           = float (*)(float, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImPow_"
                                                                                        "Float"));
        return func(x, y);
    }

    inline double ImPow(double x, double y) {
        using func_t           = double (*)(double, double);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImPow_"
                                                                                        "double"));
        return func(x, y);
    }

    inline float ImLog(float x) {
        using func_t           = float (*)(float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImLog_"
                                                                                        "Float"));
        return func(x);
    }

    inline double ImLog(double x) {
        using func_t           = double (*)(double);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImLog_"
                                                                                        "double"));
        return func(x);
    }

    inline int ImAbs(int x) {
        using func_t           = int (*)(int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImAbs_"
                                                                                        "Int"));
        return func(x);
    }

    inline float ImAbs(float x) {
        using func_t           = float (*)(float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImAbs_"
                                                                                        "Float"));
        return func(x);
    }

    inline double ImAbs(double x) {
        using func_t           = double (*)(double);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImAbs_"
                                                                                        "double"));
        return func(x);
    }

    inline float ImSign(float x) {
        using func_t           = float (*)(float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImSign_"
                                                                                        "Float"));
        return func(x);
    }

    inline double ImSign(double x) {
        using func_t           = double (*)(double);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImSign_"
                                                                                        "double"));
        return func(x);
    }

    inline float ImRsqrt(float x) {
        using func_t           = float (*)(float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImRsqrt_"
                                                                                        "Float"));
        return func(x);
    }

    inline double ImRsqrt(double x) {
        using func_t           = double (*)(double);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImRsqrt_"
                                                                                        "double"));
        return func(x);
    }

    inline void ImMin(ImVec2* pOut, const ImVec2 lhs, const ImVec2 rhs) {
        using func_t            = void (*)(ImVec2*, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImMi"
                                                                                         "n"));
        return func(pOut, lhs, rhs);
    }

    inline void ImMax(ImVec2* pOut, const ImVec2 lhs, const ImVec2 rhs) {
        using func_t            = void (*)(ImVec2*, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImMa"
                                                                                         "x"));
        return func(pOut, lhs, rhs);
    }

    inline void ImClamp(ImVec2* pOut, const ImVec2 v, const ImVec2 mn, const ImVec2 mx) {
        using func_t            = void (*)(ImVec2*, const ImVec2, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImClam"
                                                                                         "p"));
        return func(pOut, v, mn, mx);
    }

    inline void ImLerp(ImVec2* pOut, const ImVec2 a, const ImVec2 b, float t) {
        using func_t            = void (*)(ImVec2*, const ImVec2, const ImVec2, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImLerp_"
                                                                                         "Vec2Floa"
                                                                                         "t"));
        return func(pOut, a, b, t);
    }

    inline void ImLerp(ImVec2* pOut, const ImVec2 a, const ImVec2 b, const ImVec2 t) {
        using func_t            = void (*)(ImVec2*, const ImVec2, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImLerp_"
                                                                                         "Vec2Vec"
                                                                                         "2"));
        return func(pOut, a, b, t);
    }

    inline void ImLerp(ImVec4* pOut, const ImVec4 a, const ImVec4 b, float t) {
        using func_t            = void (*)(ImVec4*, const ImVec4, const ImVec4, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImLerp_"
                                                                                         "Vec4"));
        return func(pOut, a, b, t);
    }

    inline float ImSaturate(float f) {
        using func_t           = float (*)(float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImSatura"
                                                                                        "te"));
        return func(f);
    }

    inline float ImLengthSqr(const ImVec2 lhs) {
        using func_t           = float (*)(const ImVec2);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImLength"
                                                                                        "Sqr_"
                                                                                        "Vec2"));
        return func(lhs);
    }

    inline float ImLengthSqr(const ImVec4 lhs) {
        using func_t           = float (*)(const ImVec4);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImLength"
                                                                                        "Sqr_"
                                                                                        "Vec4"));
        return func(lhs);
    }

    inline float ImInvLength(const ImVec2 lhs, float fail_value) {
        using func_t           = float (*)(const ImVec2, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImInvLen"
                                                                                        "gth"));
        return func(lhs, fail_value);
    }

    inline float ImTrunc(float f) {
        using func_t           = float (*)(float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTrunc_"
                                                                                        "Float"));
        return func(f);
    }

    inline void ImTrunc(ImVec2* pOut, const ImVec2 v) {
        using func_t            = void (*)(ImVec2*, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTrunc"
                                                                                         "_Vec2"));
        return func(pOut, v);
    }

    inline float ImFloor(float f) {
        using func_t           = float (*)(float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFloor_"
                                                                                        "Float"));
        return func(f);
    }

    inline void ImFloor(ImVec2* pOut, const ImVec2 v) {
        using func_t            = void (*)(ImVec2*, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFloor"
                                                                                         "_Vec2"));
        return func(pOut, v);
    }

    inline int ImModPositive(int a, int b) {
        using func_t           = int (*)(int, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImModPos"
                                                                                        "itive"));
        return func(a, b);
    }

    inline float ImDot(const ImVec2 a, const ImVec2 b) {
        using func_t           = float (*)(const ImVec2, const ImVec2);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImDot"));
        return func(a, b);
    }

    inline void ImRotate(ImVec2* pOut, const ImVec2 v, float cos_a, float sin_a) {
        using func_t            = void (*)(ImVec2*, const ImVec2, float, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImRotat"
                                                                                         "e"));
        return func(pOut, v, cos_a, sin_a);
    }

    inline float ImLinearSweep(float current, float target, float speed) {
        using func_t           = float (*)(float, float, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImLinear"
                                                                                        "Sweep"));
        return func(current, target, speed);
    }

    inline void ImMul(ImVec2* pOut, const ImVec2 lhs, const ImVec2 rhs) {
        using func_t            = void (*)(ImVec2*, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImMu"
                                                                                         "l"));
        return func(pOut, lhs, rhs);
    }

    inline bool ImIsFloatAboveGuaranteedIntegerPrecision(float f) {
        using func_t            = bool (*)(float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImIsFlo"
                                                                                         "atAboveGu"
                                                                                         "aranteedI"
                                                                                         "ntegerPre"
                                                                                         "cision"));
        return func(f);
    }

    inline float ImExponentialMovingAverage(float avg, float sample, int n) {
        using func_t           = float (*)(float, float, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImExpone"
                                                                                        "ntialMovin"
                                                                                        "gAverag"
                                                                                        "e"));
        return func(avg, sample, n);
    }

    inline void ImBezierCubicCalc(ImVec2* pOut, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, float t) {
        using func_t = void (*)(ImVec2*, const ImVec2, const ImVec2, const ImVec2, const ImVec2, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImBezie"
                                                                                         "rCubicCal"
                                                                                         "c"));
        return func(pOut, p1, p2, p3, p4, t);
    }

    inline void ImBezierCubicClosestPoint(ImVec2* pOut, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, const ImVec2 p, int num_segments) {
        using func_t = void (*)(ImVec2*, const ImVec2, const ImVec2, const ImVec2, const ImVec2, const ImVec2, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImBezie"
                                                                                         "rCubicClo"
                                                                                         "sestPoin"
                                                                                         "t"));
        return func(pOut, p1, p2, p3, p4, p, num_segments);
    }

    inline void ImBezierCubicClosestPointCasteljau(ImVec2* pOut, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, const ImVec2 p, float tess_tol) {
        using func_t = void (*)(ImVec2*, const ImVec2, const ImVec2, const ImVec2, const ImVec2, const ImVec2, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImBezie"
                                                                                         "rCubicClo"
                                                                                         "sestPoint"
                                                                                         "Castelja"
                                                                                         "u"));
        return func(pOut, p1, p2, p3, p4, p, tess_tol);
    }

    inline void ImBezierQuadraticCalc(ImVec2* pOut, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, float t) {
        using func_t = void (*)(ImVec2*, const ImVec2, const ImVec2, const ImVec2, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImBezie"
                                                                                         "rQuadrati"
                                                                                         "cCalc"));
        return func(pOut, p1, p2, p3, t);
    }

    inline void ImLineClosestPoint(ImVec2* pOut, const ImVec2 a, const ImVec2 b, const ImVec2 p) {
        using func_t            = void (*)(ImVec2*, const ImVec2, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImLineC"
                                                                                         "losestPoi"
                                                                                         "nt"));
        return func(pOut, a, b, p);
    }

    inline bool ImTriangleContainsPoint(const ImVec2 a, const ImVec2 b, const ImVec2 c, const ImVec2 p) {
        using func_t            = bool (*)(const ImVec2, const ImVec2, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTrian"
                                                                                         "gleContai"
                                                                                         "nsPoin"
                                                                                         "t"));
        return func(a, b, c, p);
    }

    inline void ImTriangleClosestPoint(ImVec2* pOut, const ImVec2 a, const ImVec2 b, const ImVec2 c, const ImVec2 p) {
        using func_t = void (*)(ImVec2*, const ImVec2, const ImVec2, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTrian"
                                                                                         "gleCloses"
                                                                                         "tPoint"));
        return func(pOut, a, b, c, p);
    }

    inline void ImTriangleBarycentricCoords(const ImVec2 a, const ImVec2 b, const ImVec2 c, const ImVec2 p, float* out_u, float* out_v, float* out_w) {
        using func_t = void (*)(const ImVec2, const ImVec2, const ImVec2, const ImVec2, float*, float*, float*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTrian"
                                                                                         "gleBaryce"
                                                                                         "ntricCoor"
                                                                                         "ds"));
        return func(a, b, c, p, out_u, out_v, out_w);
    }

    inline float ImTriangleArea(const ImVec2 a, const ImVec2 b, const ImVec2 c) {
        using func_t           = float (*)(const ImVec2, const ImVec2, const ImVec2);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTriang"
                                                                                        "leArea"));
        return func(a, b, c);
    }

    inline bool ImTriangleIsClockwise(const ImVec2 a, const ImVec2 b, const ImVec2 c) {
        using func_t            = bool (*)(const ImVec2, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImTrian"
                                                                                         "gleIsCloc"
                                                                                         "kwise"));
        return func(a, b, c);
    }

    namespace ImVec1Manager {

        inline ImVec1* Create(void) {
            using func_t           = ImVec1* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec1"
                                                                                            "_ImVec"
                                                                                            "1_"
                                                                                            "Nil"));
            return func();
        }

        inline void Destroy(ImVec1* self) {
            using func_t            = void (*)(ImVec1*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec"
                                                                                             "1_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImVec1* Create(float _x) {
            using func_t           = ImVec1* (*)(float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec1"
                                                                                            "_ImVec"
                                                                                            "1_"
                                                                                            "Floa"
                                                                                            "t"));
            return func(_x);
        }
    }  // namespace ImVec1Manager

    namespace ImVec2ihManager {
        inline ImVec2ih* Create(void) {
            using func_t           = ImVec2ih* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec2"
                                                                                            "ih_"
                                                                                            "ImVec2"
                                                                                            "ih_"
                                                                                            "Nil"));
            return func();
        }

        inline void Destroy(ImVec2ih* self) {
            using func_t            = void (*)(ImVec2ih*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec"
                                                                                             "2ih_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImVec2ih* Create(short _x, short _y) {
            using func_t           = ImVec2ih* (*)(short, short);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec2"
                                                                                            "ih_"
                                                                                            "ImVec2"
                                                                                            "ih_"
                                                                                            "shor"
                                                                                            "t"));
            return func(_x, _y);
        }

        inline ImVec2ih* Create(const ImVec2 rhs) {
            using func_t           = ImVec2ih* (*)(const ImVec2);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec2"
                                                                                            "ih_"
                                                                                            "ImVec2"
                                                                                            "ih_"
                                                                                            "Vec"
                                                                                            "2"));
            return func(rhs);
        }
    }  // namespace ImVec2ihManager

    namespace ImRectManager {

        inline ImRect* ImRect_Nil(void) {
            using func_t           = ImRect* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRect"
                                                                                            "_ImRec"
                                                                                            "t_"
                                                                                            "Nil"));
            return func();
        }

        inline void destroy(ImRect* self) {
            using func_t            = void (*)(ImRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImRect* ImRect_Vec2(const ImVec2 min, const ImVec2 max) {
            using func_t           = ImRect* (*)(const ImVec2, const ImVec2);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRect"
                                                                                            "_ImRec"
                                                                                            "t_"
                                                                                            "Vec"
                                                                                            "2"));
            return func(min, max);
        }

        inline ImRect* ImRect_Vec4(const ImVec4 v) {
            using func_t           = ImRect* (*)(const ImVec4);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRect"
                                                                                            "_ImRec"
                                                                                            "t_"
                                                                                            "Vec"
                                                                                            "4"));
            return func(v);
        }

        inline ImRect* ImRect_Float(float x1, float y1, float x2, float y2) {
            using func_t           = ImRect* (*)(float, float, float, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRect"
                                                                                            "_ImRec"
                                                                                            "t_"
                                                                                            "Floa"
                                                                                            "t"));
            return func(x1, y1, x2, y2);
        }

        inline void GetCenter(ImVec2* pOut, ImRect* self) {
            using func_t            = void (*)(ImVec2*, ImRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "GetCe"
                                                                                             "nte"
                                                                                             "r"));
            return func(pOut, self);
        }

        inline void GetSize(ImVec2* pOut, ImRect* self) {
            using func_t            = void (*)(ImVec2*, ImRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "GetSi"
                                                                                             "ze"));
            return func(pOut, self);
        }

        inline float GetWidth(ImRect* self) {
            using func_t           = float (*)(ImRect*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRect"
                                                                                            "_GetWi"
                                                                                            "dth"));
            return func(self);
        }

        inline float GetHeight(ImRect* self) {
            using func_t           = float (*)(ImRect*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRect"
                                                                                            "_GetHe"
                                                                                            "igh"
                                                                                            "t"));
            return func(self);
        }

        inline float GetArea(ImRect* self) {
            using func_t           = float (*)(ImRect*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRect"
                                                                                            "_GetAr"
                                                                                            "ea"));
            return func(self);
        }

        inline void GetTL(ImVec2* pOut, ImRect* self) {
            using func_t            = void (*)(ImVec2*, ImRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "GetT"
                                                                                             "L"));
            return func(pOut, self);
        }

        inline void GetTR(ImVec2* pOut, ImRect* self) {
            using func_t            = void (*)(ImVec2*, ImRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "GetT"
                                                                                             "R"));
            return func(pOut, self);
        }

        inline void GetBL(ImVec2* pOut, ImRect* self) {
            using func_t            = void (*)(ImVec2*, ImRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "GetB"
                                                                                             "L"));
            return func(pOut, self);
        }

        inline void GetBR(ImVec2* pOut, ImRect* self) {
            using func_t            = void (*)(ImVec2*, ImRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "GetB"
                                                                                             "R"));
            return func(pOut, self);
        }

        inline bool Contains_Vec2(ImRect* self, const ImVec2 p) {
            using func_t            = bool (*)(ImRect*, const ImVec2);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Conta"
                                                                                             "ins_"
                                                                                             "Vec"
                                                                                             "2"));
            return func(self, p);
        }

        inline bool Contains_Rect(ImRect* self, const ImRect r) {
            using func_t            = bool (*)(ImRect*, const ImRect);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Conta"
                                                                                             "ins_"
                                                                                             "Rec"
                                                                                             "t"));
            return func(self, r);
        }

        inline bool ContainsWithPad(ImRect* self, const ImVec2 p, const ImVec2 pad) {
            using func_t            = bool (*)(ImRect*, const ImVec2, const ImVec2);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Conta"
                                                                                             "insWi"
                                                                                             "thPa"
                                                                                             "d"));
            return func(self, p, pad);
        }

        inline bool Overlaps(ImRect* self, const ImRect r) {
            using func_t            = bool (*)(ImRect*, const ImRect);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Overl"
                                                                                             "ap"
                                                                                             "s"));
            return func(self, r);
        }

        inline void Add_Vec2(ImRect* self, const ImVec2 p) {
            using func_t            = void (*)(ImRect*, const ImVec2);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Add_"
                                                                                             "Vec"
                                                                                             "2"));
            return func(self, p);
        }

        inline void Add_Rect(ImRect* self, const ImRect r) {
            using func_t            = void (*)(ImRect*, const ImRect);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Add_"
                                                                                             "Rec"
                                                                                             "t"));
            return func(self, r);
        }

        inline void Expand_Float(ImRect* self, const float amount) {
            using func_t            = void (*)(ImRect*, const float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Expan"
                                                                                             "d_"
                                                                                             "Floa"
                                                                                             "t"));
            return func(self, amount);
        }

        inline void Expand_Vec2(ImRect* self, const ImVec2 amount) {
            using func_t            = void (*)(ImRect*, const ImVec2);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Expan"
                                                                                             "d_"
                                                                                             "Vec"
                                                                                             "2"));
            return func(self, amount);
        }

        inline void Translate(ImRect* self, const ImVec2 d) {
            using func_t            = void (*)(ImRect*, const ImVec2);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Trans"
                                                                                             "lat"
                                                                                             "e"));
            return func(self, d);
        }

        inline void TranslateX(ImRect* self, float dx) {
            using func_t            = void (*)(ImRect*, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Trans"
                                                                                             "late"
                                                                                             "X"));
            return func(self, dx);
        }

        inline void TranslateY(ImRect* self, float dy) {
            using func_t            = void (*)(ImRect*, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Trans"
                                                                                             "late"
                                                                                             "Y"));
            return func(self, dy);
        }

        inline void ClipWith(ImRect* self, const ImRect r) {
            using func_t            = void (*)(ImRect*, const ImRect);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "ClipW"
                                                                                             "it"
                                                                                             "h"));
            return func(self, r);
        }

        inline void ClipWithFull(ImRect* self, const ImRect r) {
            using func_t            = void (*)(ImRect*, const ImRect);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "ClipW"
                                                                                             "ithFu"
                                                                                             "ll"));
            return func(self, r);
        }

        inline void Floor(ImRect* self) {
            using func_t            = void (*)(ImRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "Floo"
                                                                                             "r"));
            return func(self);
        }

        inline bool IsInverted(ImRect* self) {
            using func_t            = bool (*)(ImRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "IsInv"
                                                                                             "erte"
                                                                                             "d"));
            return func(self);
        }

        inline void ToVec4(ImVec4* pOut, ImRect* self) {
            using func_t            = void (*)(ImVec4*, ImRect*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImRec"
                                                                                             "t_"
                                                                                             "ToVec"
                                                                                             "4"));
            return func(pOut, self);
        }
    }  // namespace ImRectManager

    inline size_t ImBitArrayGetStorageSizeInBytes(int bitcount) {
        using func_t           = size_t (*)(int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImBitArr"
                                                                                        "ayGetStora"
                                                                                        "geSizeInBy"
                                                                                        "tes"));
        return func(bitcount);
    }

    inline void ImBitArrayClearAllBits(ImU32* arr, int bitcount) {
        using func_t            = void (*)(ImU32*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImBitAr"
                                                                                         "rayClearA"
                                                                                         "llBits"));
        return func(arr, bitcount);
    }

    inline bool ImBitArrayTestBit(const ImU32* arr, int n) {
        using func_t            = bool (*)(const ImU32*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImBitAr"
                                                                                         "rayTestBi"
                                                                                         "t"));
        return func(arr, n);
    }

    inline void ImBitArrayClearBit(ImU32* arr, int n) {
        using func_t            = void (*)(ImU32*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImBitAr"
                                                                                         "rayClearB"
                                                                                         "it"));
        return func(arr, n);
    }

    inline void ImBitArraySetBit(ImU32* arr, int n) {
        using func_t            = void (*)(ImU32*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImBitAr"
                                                                                         "raySetBi"
                                                                                         "t"));
        return func(arr, n);
    }

    inline void ImBitArraySetBitRange(ImU32* arr, int n, int n2) {
        using func_t            = void (*)(ImU32*, int, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImBitAr"
                                                                                         "raySetBit"
                                                                                         "Range"));
        return func(arr, n, n2);
    }

    inline void ImBitVectorCreate(ImBitVector* self, int sz) {
        using func_t            = void (*)(ImBitVector*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImBitVect"
                                                                                         "or_"
                                                                                         "Create"));
        return func(self, sz);
    }

    inline void ImBitVectorClear(ImBitVector* self) {
        using func_t            = void (*)(ImBitVector*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImBitVect"
                                                                                         "or_"
                                                                                         "Clear"));
        return func(self);
    }

    inline bool ImBitVectorTestBit(ImBitVector* self, int n) {
        using func_t            = bool (*)(ImBitVector*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImBitVect"
                                                                                         "or_"
                                                                                         "TestBi"
                                                                                         "t"));
        return func(self, n);
    }

    inline void ImBitVectorSetBit(ImBitVector* self, int n) {
        using func_t            = void (*)(ImBitVector*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImBitVect"
                                                                                         "or_"
                                                                                         "SetBit"));
        return func(self, n);
    }

    inline void ImBitVectorClearBit(ImBitVector* self, int n) {
        using func_t            = void (*)(ImBitVector*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImBitVect"
                                                                                         "or_"
                                                                                         "ClearBi"
                                                                                         "t"));
        return func(self, n);
    }

    inline void ImGuiTextIndexClear(ImGuiTextIndex* self) {
        using func_t            = void (*)(ImGuiTextIndex*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiText"
                                                                                         "Index_"
                                                                                         "clear"));
        return func(self);
    }

    inline int ImGuiTextIndexSize(ImGuiTextIndex* self) {
        using func_t           = int (*)(ImGuiTextIndex*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiTextI"
                                                                                        "ndex_"
                                                                                        "size"));
        return func(self);
    }

    inline const char* ImGuiTextIndexGetLineBegin(ImGuiTextIndex* self, const char* base, int n) {
        using func_t           = const char* (*)(ImGuiTextIndex*, const char*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiTextI"
                                                                                        "ndex_get_"
                                                                                        "line_"
                                                                                        "begin"));
        return func(self, base, n);
    }

    inline const char* ImGuiTextIndexGetLineEnd(ImGuiTextIndex* self, const char* base, int n) {
        using func_t           = const char* (*)(ImGuiTextIndex*, const char*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiTextI"
                                                                                        "ndex_get_"
                                                                                        "line_"
                                                                                        "end"));
        return func(self, base, n);
    }

    inline void ImGuiTextIndexAppend(ImGuiTextIndex* self, const char* base, int old_size, int new_size) {
        using func_t            = void (*)(ImGuiTextIndex*, const char*, int, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiText"
                                                                                         "Index_"
                                                                                         "append"));
        return func(self, base, old_size, new_size);
    }

    namespace ImDrawListSharedDataManager {
        inline ImDrawListSharedData* Create(void) {
            using func_t           = ImDrawListSharedData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "ListSh"
                                                                                            "aredDa"
                                                                                            "ta_"
                                                                                            "ImDraw"
                                                                                            "ListSh"
                                                                                            "aredDa"
                                                                                            "ta"));
            return func();
        }

        inline void Destroy(ImDrawListSharedData* self) {
            using func_t            = void (*)(ImDrawListSharedData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "Share"
                                                                                             "dData"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }

        inline void SetCircleTessellationMaxError(ImDrawListSharedData* self, float max_error) {
            using func_t            = void (*)(ImDrawListSharedData*, float);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wList"
                                                                                             "Share"
                                                                                             "dData"
                                                                                             "_SetC"
                                                                                             "ircle"
                                                                                             "Tesse"
                                                                                             "llati"
                                                                                             "onMax"
                                                                                             "Erro"
                                                                                             "r"));
            return func(self, max_error);
        }
    }  // namespace ImDrawListSharedDataManager

    namespace ImDrawDataBuilderManager {
        inline ImDrawDataBuilder* Create(void) {
            using func_t           = ImDrawDataBuilder* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDraw"
                                                                                            "DataBu"
                                                                                            "ilder_"
                                                                                            "ImDraw"
                                                                                            "DataBu"
                                                                                            "ilde"
                                                                                            "r"));
            return func();
        }

        inline void Destroy(ImDrawDataBuilder* self) {
            using func_t            = void (*)(ImDrawDataBuilder*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImDra"
                                                                                             "wData"
                                                                                             "Build"
                                                                                             "er_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImDrawDataBuilderManager

    namespace ImGuiDataVarInfoManager {
        inline void* GetVarPtr(ImGuiDataVarInfo* self, void* parent) {
            using func_t           = void* (*)(ImGuiDataVarInfo*, void*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiD"
                                                                                            "ataVar"
                                                                                            "Info_"
                                                                                            "GetVar"
                                                                                            "Ptr"));
            return func(self, parent);
        }
    }  // namespace ImGuiDataVarInfoManager

    namespace ImGuiStyleModManager {
        inline ImGuiStyleMod* Create(ImGuiStyleVar idx, int v) {
            using func_t           = ImGuiStyleMod* (*)(ImGuiStyleVar, int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "tyleMo"
                                                                                            "d_"
                                                                                            "ImGuiS"
                                                                                            "tyleMo"
                                                                                            "d_"
                                                                                            "Int"));
            return func(idx, v);
        }

        inline void Destroy(ImGuiStyleMod* self) {
            using func_t            = void (*)(ImGuiStyleMod*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Style"
                                                                                             "Mod_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImGuiStyleMod* Create(ImGuiStyleVar idx, float v) {
            using func_t           = ImGuiStyleMod* (*)(ImGuiStyleVar, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "tyleMo"
                                                                                            "d_"
                                                                                            "ImGuiS"
                                                                                            "tyleMo"
                                                                                            "d_"
                                                                                            "Floa"
                                                                                            "t"));
            return func(idx, v);
        }

        inline ImGuiStyleMod* Create(ImGuiStyleVar idx, ImVec2 v) {
            using func_t           = ImGuiStyleMod* (*)(ImGuiStyleVar, ImVec2);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "tyleMo"
                                                                                            "d_"
                                                                                            "ImGuiS"
                                                                                            "tyleMo"
                                                                                            "d_"
                                                                                            "Vec"
                                                                                            "2"));
            return func(idx, v);
        }
    }  // namespace ImGuiStyleModManager

    namespace ImGuiComboPreviewDataManager {

        inline ImGuiComboPreviewData* Create(void) {
            using func_t           = ImGuiComboPreviewData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiC"
                                                                                            "omboPr"
                                                                                            "eviewD"
                                                                                            "ata_"
                                                                                            "ImGuiC"
                                                                                            "omboPr"
                                                                                            "eviewD"
                                                                                            "ata"));
            return func();
        }

        inline void Destroy(ImGuiComboPreviewData* self) {
            using func_t            = void (*)(ImGuiComboPreviewData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Combo"
                                                                                             "Previ"
                                                                                             "ewDat"
                                                                                             "a_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiComboPreviewDataManager

    namespace ImGuiMenuColumnsManager {

        inline ImGuiMenuColumns* Craete(void) {
            using func_t           = ImGuiMenuColumns* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiM"
                                                                                            "enuCol"
                                                                                            "umns_"
                                                                                            "ImGuiM"
                                                                                            "enuCol"
                                                                                            "umn"
                                                                                            "s"));
            return func();
        }

        inline void Destroy(ImGuiMenuColumns* self) {
            using func_t            = void (*)(ImGuiMenuColumns*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "MenuC"
                                                                                             "olumn"
                                                                                             "s_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void Update(ImGuiMenuColumns* self, float spacing, bool window_reappearing) {
            using func_t            = void (*)(ImGuiMenuColumns*, float, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "MenuC"
                                                                                             "olumn"
                                                                                             "s_"
                                                                                             "Updat"
                                                                                             "e"));
            return func(self, spacing, window_reappearing);
        }

        inline float DeclColumns(ImGuiMenuColumns* self, float w_icon, float w_label, float w_shortcut, float w_mark) {
            using func_t           = float (*)(ImGuiMenuColumns*, float, float, float, float);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiM"
                                                                                            "enuCol"
                                                                                            "umns_"
                                                                                            "DeclCo"
                                                                                            "lumn"
                                                                                            "s"));
            return func(self, w_icon, w_label, w_shortcut, w_mark);
        }

        inline void CalcNextTotalWidth(ImGuiMenuColumns* self, bool update_offsets) {
            using func_t            = void (*)(ImGuiMenuColumns*, bool);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "MenuC"
                                                                                             "olumn"
                                                                                             "s_"
                                                                                             "CalcN"
                                                                                             "extTo"
                                                                                             "talWi"
                                                                                             "dt"
                                                                                             "h"));
            return func(self, update_offsets);
        }
    }  // namespace ImGuiMenuColumnsManager

    namespace ImGuiInputTextDeactivatedStateManager {
        inline ImGuiInputTextDeactivatedState* Create(void) {
            using func_t           = ImGuiInputTextDeactivatedState* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "nputTe"
                                                                                            "xtDeac"
                                                                                            "tivate"
                                                                                            "dState"
                                                                                            "_ImGui"
                                                                                            "InputT"
                                                                                            "extDea"
                                                                                            "ctivat"
                                                                                            "edStat"
                                                                                            "e"));
            return func();
        }

        inline void Destroy(ImGuiInputTextDeactivatedState* self) {
            using func_t            = void (*)(ImGuiInputTextDeactivatedState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextD"
                                                                                             "eacti"
                                                                                             "vated"
                                                                                             "State"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiInputTextDeactivatedStateManager

    namespace ImGuiInputTextDeactivatedStateManager {

        inline void ClearFreeMemory(ImGuiInputTextDeactivatedState* self) {
            using func_t            = void (*)(ImGuiInputTextDeactivatedState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextD"
                                                                                             "eacti"
                                                                                             "vated"
                                                                                             "State"
                                                                                             "_Clea"
                                                                                             "rFree"
                                                                                             "Memor"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiInputTextDeactivatedStateManager

    namespace ImGuiInputTextStateManager {

        inline ImGuiInputTextState* Create(void) {
            using func_t           = ImGuiInputTextState* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "nputTe"
                                                                                            "xtStat"
                                                                                            "e_"
                                                                                            "ImGuiI"
                                                                                            "nputTe"
                                                                                            "xtStat"
                                                                                            "e"));
            return func();
        }

        inline void Destroy(ImGuiInputTextState* self) {
            using func_t            = void (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void ClearText(ImGuiInputTextState* self) {
            using func_t            = void (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "Clear"
                                                                                             "Tex"
                                                                                             "t"));
            return func(self);
        }

        inline void ClearFreeMemory(ImGuiInputTextState* self) {
            using func_t            = void (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "Clear"
                                                                                             "FreeM"
                                                                                             "emor"
                                                                                             "y"));
            return func(self);
        }

        inline int GetUndoAvailCount(ImGuiInputTextState* self) {
            using func_t           = int (*)(ImGuiInputTextState*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "nputTe"
                                                                                            "xtStat"
                                                                                            "e_"
                                                                                            "GetUnd"
                                                                                            "oAvail"
                                                                                            "Coun"
                                                                                            "t"));
            return func(self);
        }

        inline int GetRedoAvailCount(ImGuiInputTextState* self) {
            using func_t           = int (*)(ImGuiInputTextState*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "nputTe"
                                                                                            "xtStat"
                                                                                            "e_"
                                                                                            "GetRed"
                                                                                            "oAvail"
                                                                                            "Coun"
                                                                                            "t"));
            return func(self);
        }

        inline void OnKeyPressed(ImGuiInputTextState* self, int key) {
            using func_t            = void (*)(ImGuiInputTextState*, int);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "OnKey"
                                                                                             "Press"
                                                                                             "ed"));
            return func(self, key);
        }

        inline void CursorAnimReset(ImGuiInputTextState* self) {
            using func_t            = void (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "Curso"
                                                                                             "rAnim"
                                                                                             "Rese"
                                                                                             "t"));
            return func(self);
        }

        inline void CursorClamp(ImGuiInputTextState* self) {
            using func_t            = void (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "Curso"
                                                                                             "rClam"
                                                                                             "p"));
            return func(self);
        }

        inline bool HasSelection(ImGuiInputTextState* self) {
            using func_t            = bool (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "HasSe"
                                                                                             "lecti"
                                                                                             "on"));
            return func(self);
        }

        inline void ClearSelection(ImGuiInputTextState* self) {
            using func_t            = void (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "Clear"
                                                                                             "Selec"
                                                                                             "tio"
                                                                                             "n"));
            return func(self);
        }

        inline int GetCursorPos(ImGuiInputTextState* self) {
            using func_t           = int (*)(ImGuiInputTextState*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "nputTe"
                                                                                            "xtStat"
                                                                                            "e_"
                                                                                            "GetCur"
                                                                                            "sorPo"
                                                                                            "s"));
            return func(self);
        }

        inline int GetSelectionStart(ImGuiInputTextState* self) {
            using func_t           = int (*)(ImGuiInputTextState*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "nputTe"
                                                                                            "xtStat"
                                                                                            "e_"
                                                                                            "GetSel"
                                                                                            "ection"
                                                                                            "Star"
                                                                                            "t"));
            return func(self);
        }

        inline int GetSelectionEnd(ImGuiInputTextState* self) {
            using func_t           = int (*)(ImGuiInputTextState*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "nputTe"
                                                                                            "xtStat"
                                                                                            "e_"
                                                                                            "GetSel"
                                                                                            "ection"
                                                                                            "End"));
            return func(self);
        }

        inline void SelectAll(ImGuiInputTextState* self) {
            using func_t            = void (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "Selec"
                                                                                             "tAl"
                                                                                             "l"));
            return func(self);
        }

        inline void ReloadUserBufAndSelectAll(ImGuiInputTextState* self) {
            using func_t            = void (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "Reloa"
                                                                                             "dUser"
                                                                                             "BufAn"
                                                                                             "dSele"
                                                                                             "ctAl"
                                                                                             "l"));
            return func(self);
        }

        inline void ReloadUserBufAndKeepSelection(ImGuiInputTextState* self) {
            using func_t            = void (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "Reloa"
                                                                                             "dUser"
                                                                                             "BufAn"
                                                                                             "dKeep"
                                                                                             "Selec"
                                                                                             "tio"
                                                                                             "n"));
            return func(self);
        }

        inline void ReloadUserBufAndMoveToEnd(ImGuiInputTextState* self) {
            using func_t            = void (*)(ImGuiInputTextState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "TextS"
                                                                                             "tate_"
                                                                                             "Reloa"
                                                                                             "dUser"
                                                                                             "BufAn"
                                                                                             "dMove"
                                                                                             "ToEn"
                                                                                             "d"));
            return func(self);
        }

    }  // namespace ImGuiInputTextStateManager

    namespace ImGuiNextWindowDataManager {
        inline ImGuiNextWindowData* Create(void) {
            using func_t           = ImGuiNextWindowData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiN"
                                                                                            "extWin"
                                                                                            "dowDat"
                                                                                            "a_"
                                                                                            "ImGuiN"
                                                                                            "extWin"
                                                                                            "dowDat"
                                                                                            "a"));
            return func();
        }

        inline void Destroy(ImGuiNextWindowData* self) {
            using func_t            = void (*)(ImGuiNextWindowData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "NextW"
                                                                                             "indow"
                                                                                             "Data_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void ClearFlags(ImGuiNextWindowData* self) {
            using func_t            = void (*)(ImGuiNextWindowData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "NextW"
                                                                                             "indow"
                                                                                             "Data_"
                                                                                             "Clear"
                                                                                             "Flag"
                                                                                             "s"));
            return func(self);
        }
    }  // namespace ImGuiNextWindowDataManager

    namespace ImGuiNextItemDataManager {
        inline ImGuiNextItemData* Create(void) {
            using func_t           = ImGuiNextItemData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiN"
                                                                                            "extIte"
                                                                                            "mData_"
                                                                                            "ImGuiN"
                                                                                            "extIte"
                                                                                            "mDat"
                                                                                            "a"));
            return func();
        }

        inline void Destroy(ImGuiNextItemData* self) {
            using func_t            = void (*)(ImGuiNextItemData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "NextI"
                                                                                             "temDa"
                                                                                             "ta_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void ClearFlags(ImGuiNextItemData* self) {
            using func_t            = void (*)(ImGuiNextItemData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "NextI"
                                                                                             "temDa"
                                                                                             "ta_"
                                                                                             "Clear"
                                                                                             "Flag"
                                                                                             "s"));
            return func(self);
        }
    }  // namespace ImGuiNextItemDataManager

    namespace ImGuiLastItemDataManager {

        inline ImGuiLastItemData* Create(void) {
            using func_t           = ImGuiLastItemData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiL"
                                                                                            "astIte"
                                                                                            "mData_"
                                                                                            "ImGuiL"
                                                                                            "astIte"
                                                                                            "mDat"
                                                                                            "a"));
            return func();
        }

        inline void Destroy(ImGuiLastItemData* self) {
            using func_t            = void (*)(ImGuiLastItemData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "LastI"
                                                                                             "temDa"
                                                                                             "ta_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiLastItemDataManager

    namespace ImGuiStackSizesManager {

        inline ImGuiStackSizes* Create(void) {
            using func_t           = ImGuiStackSizes* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "tackSi"
                                                                                            "zes_"
                                                                                            "ImGuiS"
                                                                                            "tackSi"
                                                                                            "zes"));
            return func();
        }

        inline void Destroy(ImGuiStackSizes* self) {
            using func_t            = void (*)(ImGuiStackSizes*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stack"
                                                                                             "Sizes"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }

        inline void SetToContextState(ImGuiStackSizes* self, ImGuiContext* ctx) {
            using func_t            = void (*)(ImGuiStackSizes*, ImGuiContext*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stack"
                                                                                             "Sizes"
                                                                                             "_SetT"
                                                                                             "oCont"
                                                                                             "extSt"
                                                                                             "at"
                                                                                             "e"));
            return func(self, ctx);
        }

        inline void CompareWithContextState(ImGuiStackSizes* self, ImGuiContext* ctx) {
            using func_t            = void (*)(ImGuiStackSizes*, ImGuiContext*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stack"
                                                                                             "Sizes"
                                                                                             "_Comp"
                                                                                             "areWi"
                                                                                             "thCon"
                                                                                             "textS"
                                                                                             "tat"
                                                                                             "e"));
            return func(self, ctx);
        }
    }  // namespace ImGuiStackSizesManager

    namespace ImGuiPtrOrIndexManager {
        inline ImGuiPtrOrIndex* Create(void* ptr) {
            using func_t           = ImGuiPtrOrIndex* (*)(void*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiP"
                                                                                            "trOrIn"
                                                                                            "dex_"
                                                                                            "ImGuiP"
                                                                                            "trOrIn"
                                                                                            "dex_"
                                                                                            "Ptr"));
            return func(ptr);
        }

        inline void Destroy(ImGuiPtrOrIndex* self) {
            using func_t            = void (*)(ImGuiPtrOrIndex*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "PtrOr"
                                                                                             "Index"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }

        inline ImGuiPtrOrIndex* Create(int index) {
            using func_t           = ImGuiPtrOrIndex* (*)(int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiP"
                                                                                            "trOrIn"
                                                                                            "dex_"
                                                                                            "ImGuiP"
                                                                                            "trOrIn"
                                                                                            "dex_"
                                                                                            "Int"));
            return func(index);
        }
    }  // namespace ImGuiPtrOrIndexManager

    namespace ImGuiPopupDataManager {

        inline ImGuiPopupData* Create(void) {
            using func_t           = ImGuiPopupData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiP"
                                                                                            "opupDa"
                                                                                            "ta_"
                                                                                            "ImGuiP"
                                                                                            "opupDa"
                                                                                            "ta"));
            return func();
        }

        inline void Destroy(ImGuiPopupData* self) {
            using func_t            = void (*)(ImGuiPopupData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Popup"
                                                                                             "Data_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiPopupDataManager

    namespace ImGuiInputEventManager {

        inline ImGuiInputEvent* Create(void) {
            using func_t           = ImGuiInputEvent* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "nputEv"
                                                                                            "ent_"
                                                                                            "ImGuiI"
                                                                                            "nputEv"
                                                                                            "ent"));
            return func();
        }

        inline void Destroy(ImGuiInputEvent* self) {
            using func_t            = void (*)(ImGuiInputEvent*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Input"
                                                                                             "Event"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiInputEventManager

    namespace ImGuiKeyRoutingDataManager {

        inline ImGuiKeyRoutingData* Create(void) {
            using func_t           = ImGuiKeyRoutingData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiK"
                                                                                            "eyRout"
                                                                                            "ingDat"
                                                                                            "a_"
                                                                                            "ImGuiK"
                                                                                            "eyRout"
                                                                                            "ingDat"
                                                                                            "a"));
            return func();
        }

        inline void Destroy(ImGuiKeyRoutingData* self) {
            using func_t            = void (*)(ImGuiKeyRoutingData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "KeyRo"
                                                                                             "uting"
                                                                                             "Data_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiKeyRoutingDataManager

    namespace ImGuiKeyRoutingTableManager {
        inline ImGuiKeyRoutingTable* Create(void) {
            using func_t           = ImGuiKeyRoutingTable* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiK"
                                                                                            "eyRout"
                                                                                            "ingTab"
                                                                                            "le_"
                                                                                            "ImGuiK"
                                                                                            "eyRout"
                                                                                            "ingTab"
                                                                                            "le"));
            return func();
        }

        inline void Destroy(ImGuiKeyRoutingTable* self) {
            using func_t            = void (*)(ImGuiKeyRoutingTable*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "KeyRo"
                                                                                             "uting"
                                                                                             "Table"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }

        inline void Clear(ImGuiKeyRoutingTable* self) {
            using func_t            = void (*)(ImGuiKeyRoutingTable*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "KeyRo"
                                                                                             "uting"
                                                                                             "Table"
                                                                                             "_Clea"
                                                                                             "r"));
            return func(self);
        }
    }  // namespace ImGuiKeyRoutingTableManager

    namespace ImGuiKeyOwnerDataManager {
        inline ImGuiKeyOwnerData* Create(void) {
            using func_t           = ImGuiKeyOwnerData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiK"
                                                                                            "eyOwne"
                                                                                            "rData_"
                                                                                            "ImGuiK"
                                                                                            "eyOwne"
                                                                                            "rDat"
                                                                                            "a"));
            return func();
        }

        inline void Destroy(ImGuiKeyOwnerData* self) {
            using func_t            = void (*)(ImGuiKeyOwnerData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "KeyOw"
                                                                                             "nerDa"
                                                                                             "ta_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiKeyOwnerDataManager

    namespace ImGuiListClipperRangeManager {
        inline ImGuiListClipperRange FromIndices(int min, int max) {
            using func_t           = ImGuiListClipperRange (*)(int, int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiL"
                                                                                            "istCli"
                                                                                            "pperRa"
                                                                                            "nge_"
                                                                                            "FromIn"
                                                                                            "dice"
                                                                                            "s"));
            return func(min, max);
        }

        inline ImGuiListClipperRange FromPositions(float y1, float y2, int off_min, int off_max) {
            using func_t           = ImGuiListClipperRange (*)(float, float, int, int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiL"
                                                                                            "istCli"
                                                                                            "pperRa"
                                                                                            "nge_"
                                                                                            "FromPo"
                                                                                            "sition"
                                                                                            "s"));
            return func(y1, y2, off_min, off_max);
        }
    }  // namespace ImGuiListClipperRangeManager

    namespace ImGuiListClipperDataManager {

        inline ImGuiListClipperData* Create(void) {
            using func_t           = ImGuiListClipperData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiL"
                                                                                            "istCli"
                                                                                            "pperDa"
                                                                                            "ta_"
                                                                                            "ImGuiL"
                                                                                            "istCli"
                                                                                            "pperDa"
                                                                                            "ta"));
            return func();
        }

        inline void Destroy(ImGuiListClipperData* self) {
            using func_t            = void (*)(ImGuiListClipperData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "ListC"
                                                                                             "lippe"
                                                                                             "rData"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }

        inline void Reset(ImGuiListClipperData* self, ImGuiListClipper* clipper) {
            using func_t            = void (*)(ImGuiListClipperData*, ImGuiListClipper*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "ListC"
                                                                                             "lippe"
                                                                                             "rData"
                                                                                             "_Rese"
                                                                                             "t"));
            return func(self, clipper);
        }
    }  // namespace ImGuiListClipperDataManager

    namespace ImGuiNavItemDataManager {

        inline ImGuiNavItemData* Create(void) {
            using func_t           = ImGuiNavItemData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiN"
                                                                                            "avItem"
                                                                                            "Data_"
                                                                                            "ImGuiN"
                                                                                            "avItem"
                                                                                            "Dat"
                                                                                            "a"));
            return func();
        }

        inline void Destroy(ImGuiNavItemData* self) {
            using func_t            = void (*)(ImGuiNavItemData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "NavIt"
                                                                                             "emDat"
                                                                                             "a_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void Clear(ImGuiNavItemData* self) {
            using func_t            = void (*)(ImGuiNavItemData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "NavIt"
                                                                                             "emDat"
                                                                                             "a_"
                                                                                             "Clea"
                                                                                             "r"));
            return func(self);
        }
    }  // namespace ImGuiNavItemDataManager

    namespace ImGuiTypingSelectStateManager {
        inline ImGuiTypingSelectState* Create(void) {
            using func_t           = ImGuiTypingSelectState* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "ypingS"
                                                                                            "electS"
                                                                                            "tate_"
                                                                                            "ImGuiT"
                                                                                            "ypingS"
                                                                                            "electS"
                                                                                            "tat"
                                                                                            "e"));
            return func();
        }

        inline void Destroy(ImGuiTypingSelectState* self) {
            using func_t            = void (*)(ImGuiTypingSelectState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Typin"
                                                                                             "gSele"
                                                                                             "ctSta"
                                                                                             "te_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void Clear(ImGuiTypingSelectState* self) {
            using func_t            = void (*)(ImGuiTypingSelectState*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Typin"
                                                                                             "gSele"
                                                                                             "ctSta"
                                                                                             "te_"
                                                                                             "Clea"
                                                                                             "r"));
            return func(self);
        }
    }  // namespace ImGuiTypingSelectStateManager

    namespace ImGuiOldColumnDataManager {
        inline ImGuiOldColumnData* Create(void) {
            using func_t           = ImGuiOldColumnData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiO"
                                                                                            "ldColu"
                                                                                            "mnData"
                                                                                            "_ImGui"
                                                                                            "OldCol"
                                                                                            "umnDat"
                                                                                            "a"));
            return func();
        }

        inline void Destroy(ImGuiOldColumnData* self) {
            using func_t            = void (*)(ImGuiOldColumnData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "OldCo"
                                                                                             "lumnD"
                                                                                             "ata_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiOldColumnDataManager

    namespace ImGuiOldColumnsManager {

        inline ImGuiOldColumns* Create(void) {
            using func_t           = ImGuiOldColumns* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiO"
                                                                                            "ldColu"
                                                                                            "mns_"
                                                                                            "ImGuiO"
                                                                                            "ldColu"
                                                                                            "mns"));
            return func();
        }

        inline void Destroy(ImGuiOldColumns* self) {
            using func_t            = void (*)(ImGuiOldColumns*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "OldCo"
                                                                                             "lumns"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiOldColumnsManager

    namespace ImGuiDockNodeManager {

        inline ImGuiDockNode* Create(ImGuiID id) {
            using func_t           = ImGuiDockNode* (*)(ImGuiID);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiD"
                                                                                            "ockNod"
                                                                                            "e_"
                                                                                            "ImGuiD"
                                                                                            "ockNod"
                                                                                            "e"));
            return func(id);
        }

        inline void Destroy(ImGuiDockNode* self) {
            using func_t            = void (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline bool IsRootNode(ImGuiDockNode* self) {
            using func_t            = bool (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "IsRoo"
                                                                                             "tNod"
                                                                                             "e"));
            return func(self);
        }

        inline bool IsDockSpace(ImGuiDockNode* self) {
            using func_t            = bool (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "IsDoc"
                                                                                             "kSpac"
                                                                                             "e"));
            return func(self);
        }

        inline bool IsFloatingNode(ImGuiDockNode* self) {
            using func_t            = bool (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "IsFlo"
                                                                                             "ating"
                                                                                             "Nod"
                                                                                             "e"));
            return func(self);
        }

        inline bool IsCentralNode(ImGuiDockNode* self) {
            using func_t            = bool (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "IsCen"
                                                                                             "tralN"
                                                                                             "od"
                                                                                             "e"));
            return func(self);
        }

        inline bool IsHiddenTabBar(ImGuiDockNode* self) {
            using func_t            = bool (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "IsHid"
                                                                                             "denTa"
                                                                                             "bBa"
                                                                                             "r"));
            return func(self);
        }

        inline bool IsNoTabBar(ImGuiDockNode* self) {
            using func_t            = bool (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "IsNoT"
                                                                                             "abBa"
                                                                                             "r"));
            return func(self);
        }

        inline bool IsSplitNode(ImGuiDockNode* self) {
            using func_t            = bool (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "IsSpl"
                                                                                             "itNod"
                                                                                             "e"));
            return func(self);
        }

        inline bool IsLeafNode(ImGuiDockNode* self) {
            using func_t            = bool (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "IsLea"
                                                                                             "fNod"
                                                                                             "e"));
            return func(self);
        }

        inline bool IsEmpty(ImGuiDockNode* self) {
            using func_t            = bool (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "IsEmp"
                                                                                             "ty"));
            return func(self);
        }

        inline void Rect(ImRect* pOut, ImGuiDockNode* self) {
            using func_t            = void (*)(ImRect*, ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "Rec"
                                                                                             "t"));
            return func(pOut, self);
        }

        inline void SetLocalFlags(ImGuiDockNode* self, ImGuiDockNodeFlags flags) {
            using func_t            = void (*)(ImGuiDockNode*, ImGuiDockNodeFlags);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "SetLo"
                                                                                             "calFl"
                                                                                             "ag"
                                                                                             "s"));
            return func(self, flags);
        }

        inline void UpdateMergedFlags(ImGuiDockNode* self) {
            using func_t            = void (*)(ImGuiDockNode*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockN"
                                                                                             "ode_"
                                                                                             "Updat"
                                                                                             "eMerg"
                                                                                             "edFla"
                                                                                             "gs"));
            return func(self);
        }
    }  // namespace ImGuiDockNodeManager

    namespace ImGuiDockContextManager {

        inline ImGuiDockContext* Create(void) {
            using func_t           = ImGuiDockContext* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiD"
                                                                                            "ockCon"
                                                                                            "text_"
                                                                                            "ImGuiD"
                                                                                            "ockCon"
                                                                                            "tex"
                                                                                            "t"));
            return func();
        }

        inline void Destroy(ImGuiDockContext* self) {
            using func_t            = void (*)(ImGuiDockContext*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "DockC"
                                                                                             "ontex"
                                                                                             "t_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiDockContextManager

    namespace ImGuiViewportPManager {

        inline ImGuiViewportP* Create(void) {
            using func_t           = ImGuiViewportP* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiV"
                                                                                            "iewpor"
                                                                                            "tP_"
                                                                                            "ImGuiV"
                                                                                            "iewpor"
                                                                                            "tP"));
            return func();
        }

        inline void Destroy(ImGuiViewportP* self) {
            using func_t            = void (*)(ImGuiViewportP*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ortP_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void ClearRequestFlags(ImGuiViewportP* self) {
            using func_t            = void (*)(ImGuiViewportP*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ortP_"
                                                                                             "Clear"
                                                                                             "Reque"
                                                                                             "stFla"
                                                                                             "gs"));
            return func(self);
        }

        inline void CalcWorkRectPos(ImVec2* pOut, ImGuiViewportP* self, const ImVec2 off_min) {
            using func_t            = void (*)(ImVec2*, ImGuiViewportP*, const ImVec2);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ortP_"
                                                                                             "CalcW"
                                                                                             "orkRe"
                                                                                             "ctPo"
                                                                                             "s"));
            return func(pOut, self, off_min);
        }

        inline void CalcWorkRectSize(ImVec2* pOut, ImGuiViewportP* self, const ImVec2 off_min, const ImVec2 off_max) {
            using func_t = void (*)(ImVec2*, ImGuiViewportP*, const ImVec2, const ImVec2);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ortP_"
                                                                                             "CalcW"
                                                                                             "orkRe"
                                                                                             "ctSiz"
                                                                                             "e"));
            return func(pOut, self, off_min, off_max);
        }

        inline void UpdateWorkRect(ImGuiViewportP* self) {
            using func_t            = void (*)(ImGuiViewportP*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ortP_"
                                                                                             "Updat"
                                                                                             "eWork"
                                                                                             "Rec"
                                                                                             "t"));
            return func(self);
        }

        inline void GetMainRect(ImRect* pOut, ImGuiViewportP* self) {
            using func_t            = void (*)(ImRect*, ImGuiViewportP*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ortP_"
                                                                                             "GetMa"
                                                                                             "inRec"
                                                                                             "t"));
            return func(pOut, self);
        }

        inline void GetWorkRect(ImRect* pOut, ImGuiViewportP* self) {
            using func_t            = void (*)(ImRect*, ImGuiViewportP*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ortP_"
                                                                                             "GetWo"
                                                                                             "rkRec"
                                                                                             "t"));
            return func(pOut, self);
        }

        inline void GetBuildWorkRect(ImRect* pOut, ImGuiViewportP* self) {
            using func_t            = void (*)(ImRect*, ImGuiViewportP*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Viewp"
                                                                                             "ortP_"
                                                                                             "GetBu"
                                                                                             "ildWo"
                                                                                             "rkRec"
                                                                                             "t"));
            return func(pOut, self);
        }
    }  // namespace ImGuiViewportPManager

    namespace ImGuiWindowSettingsManager {

        inline ImGuiWindowSettings* Create(void) {
            using func_t           = ImGuiWindowSettings* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiW"
                                                                                            "indowS"
                                                                                            "etting"
                                                                                            "s_"
                                                                                            "ImGuiW"
                                                                                            "indowS"
                                                                                            "etting"
                                                                                            "s"));
            return func();
        }

        inline void Destroy(ImGuiWindowSettings* self) {
            using func_t            = void (*)(ImGuiWindowSettings*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Windo"
                                                                                             "wSett"
                                                                                             "ings_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline char* GetName(ImGuiWindowSettings* self) {
            using func_t           = char* (*)(ImGuiWindowSettings*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiW"
                                                                                            "indowS"
                                                                                            "etting"
                                                                                            "s_"
                                                                                            "GetNam"
                                                                                            "e"));
            return func(self);
        }
    }  // namespace ImGuiWindowSettingsManager

    namespace ImGuiSettingsHandlerManager {

        inline ImGuiSettingsHandler* Create(void) {
            using func_t           = ImGuiSettingsHandler* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "etting"
                                                                                            "sHandl"
                                                                                            "er_"
                                                                                            "ImGuiS"
                                                                                            "etting"
                                                                                            "sHandl"
                                                                                            "er"));
            return func();
        }

        inline void Destroy(ImGuiSettingsHandler* self) {
            using func_t            = void (*)(ImGuiSettingsHandler*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Setti"
                                                                                             "ngsHa"
                                                                                             "ndler"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiSettingsHandlerManager

    namespace ImGuiDebugAllocInfoManager {

        inline ImGuiDebugAllocInfo* Create(void) {
            using func_t           = ImGuiDebugAllocInfo* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiD"
                                                                                            "ebugAl"
                                                                                            "locInf"
                                                                                            "o_"
                                                                                            "ImGuiD"
                                                                                            "ebugAl"
                                                                                            "locInf"
                                                                                            "o"));
            return func();
        }

        inline void Destroy(ImGuiDebugAllocInfo* self) {
            using func_t            = void (*)(ImGuiDebugAllocInfo*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Debug"
                                                                                             "Alloc"
                                                                                             "Info_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiDebugAllocInfoManager

    namespace ImGuiStackLevelInfoManager {

        inline ImGuiStackLevelInfo* Create(void) {
            using func_t           = ImGuiStackLevelInfo* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiS"
                                                                                            "tackLe"
                                                                                            "velInf"
                                                                                            "o_"
                                                                                            "ImGuiS"
                                                                                            "tackLe"
                                                                                            "velInf"
                                                                                            "o"));
            return func();
        }

        inline void Destroy(ImGuiStackLevelInfo* self) {
            using func_t            = void (*)(ImGuiStackLevelInfo*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Stack"
                                                                                             "Level"
                                                                                             "Info_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiStackLevelInfoManager

    namespace ImGuiIDStackToolManager {
        inline ImGuiIDStackTool* Create(void) {
            using func_t           = ImGuiIDStackTool* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiI"
                                                                                            "DStack"
                                                                                            "Tool_"
                                                                                            "ImGuiI"
                                                                                            "DStack"
                                                                                            "Too"
                                                                                            "l"));
            return func();
        }

        inline void Destroy(ImGuiIDStackTool* self) {
            using func_t            = void (*)(ImGuiIDStackTool*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "IDSta"
                                                                                             "ckToo"
                                                                                             "l_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiIDStackToolManager

    namespace ImGuiContextHookManager {

        inline ImGuiContextHook* Create(void) {
            using func_t           = ImGuiContextHook* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiC"
                                                                                            "ontext"
                                                                                            "Hook_"
                                                                                            "ImGuiC"
                                                                                            "ontext"
                                                                                            "Hoo"
                                                                                            "k"));
            return func();
        }

        inline void Destroy(ImGuiContextHook* self) {
            using func_t            = void (*)(ImGuiContextHook*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Conte"
                                                                                             "xtHoo"
                                                                                             "k_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiContextHookManager

    namespace ImGuiContextManager {

        inline ImGuiContext* Create(ImFontAtlas* shared_font_atlas) {
            using func_t           = ImGuiContext* (*)(ImFontAtlas*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiC"
                                                                                            "ontext"
                                                                                            "_ImGui"
                                                                                            "Contex"
                                                                                            "t"));
            return func(shared_font_atlas);
        }

        inline void Destroy(ImGuiContext* self) {
            using func_t            = void (*)(ImGuiContext*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Conte"
                                                                                             "xt_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiContextManager

    namespace ImGuiWindowManager {

        inline ImGuiWindow* Create(ImGuiContext* context, const char* name) {
            using func_t           = ImGuiWindow* (*)(ImGuiContext*, const char*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiW"
                                                                                            "indow_"
                                                                                            "ImGuiW"
                                                                                            "indo"
                                                                                            "w"));
            return func(context, name);
        }

        inline void Destroy(ImGuiWindow* self) {
            using func_t            = void (*)(ImGuiWindow*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Windo"
                                                                                             "w_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImGuiID GetID_Str(ImGuiWindow* self, const char* str, const char* str_end) {
            using func_t           = ImGuiID (*)(ImGuiWindow*, const char*, const char*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiW"
                                                                                            "indow_"
                                                                                            "GetID_"
                                                                                            "Str"));
            return func(self, str, str_end);
        }

        inline ImGuiID GetID_Ptr(ImGuiWindow* self, const void* ptr) {
            using func_t           = ImGuiID (*)(ImGuiWindow*, const void*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiW"
                                                                                            "indow_"
                                                                                            "GetID_"
                                                                                            "Ptr"));
            return func(self, ptr);
        }

        inline ImGuiID GetID_Int(ImGuiWindow* self, int n) {
            using func_t           = ImGuiID (*)(ImGuiWindow*, int);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiW"
                                                                                            "indow_"
                                                                                            "GetID_"
                                                                                            "Int"));
            return func(self, n);
        }

        inline ImGuiID GetIDFromRectangle(ImGuiWindow* self, const ImRect r_abs) {
            using func_t           = ImGuiID (*)(ImGuiWindow*, const ImRect);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiW"
                                                                                            "indow_"
                                                                                            "GetIDF"
                                                                                            "romRec"
                                                                                            "tangl"
                                                                                            "e"));
            return func(self, r_abs);
        }

        inline void Rect(ImRect* pOut, ImGuiWindow* self) {
            using func_t            = void (*)(ImRect*, ImGuiWindow*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Windo"
                                                                                             "w_"
                                                                                             "Rec"
                                                                                             "t"));
            return func(pOut, self);
        }

        inline float CalcFontSize(ImGuiWindow* self) {
            using func_t           = float (*)(ImGuiWindow*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiW"
                                                                                            "indow_"
                                                                                            "CalcFo"
                                                                                            "ntSiz"
                                                                                            "e"));
            return func(self);
        }

        inline void TitleBarRect(ImRect* pOut, ImGuiWindow* self) {
            using func_t            = void (*)(ImRect*, ImGuiWindow*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Windo"
                                                                                             "w_"
                                                                                             "Title"
                                                                                             "BarRe"
                                                                                             "ct"));
            return func(pOut, self);
        }

        inline void MenuBarRect(ImRect* pOut, ImGuiWindow* self) {
            using func_t            = void (*)(ImRect*, ImGuiWindow*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Windo"
                                                                                             "w_"
                                                                                             "MenuB"
                                                                                             "arRec"
                                                                                             "t"));
            return func(pOut, self);
        }
    }  // namespace ImGuiWindowManager

    namespace ImGuiTabItemManager {

        inline ImGuiTabItem* Create(void) {
            using func_t           = ImGuiTabItem* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "abItem"
                                                                                            "_ImGui"
                                                                                            "TabIte"
                                                                                            "m"));
            return func();
        }

        inline void Destroy(ImGuiTabItem* self) {
            using func_t            = void (*)(ImGuiTabItem*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TabIt"
                                                                                             "em_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiTabItemManager

    namespace ImGuiTabBarManager {
        inline ImGuiTabBar* Create(void) {
            using func_t           = ImGuiTabBar* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "abBar_"
                                                                                            "ImGuiT"
                                                                                            "abBa"
                                                                                            "r"));
            return func();
        }

        inline void Destroy(ImGuiTabBar* self) {
            using func_t            = void (*)(ImGuiTabBar*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "TabBa"
                                                                                             "r_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiTabBarManager

    namespace ImGuiTableColumnManager {
        inline ImGuiTableColumn* Create(void) {
            using func_t           = ImGuiTableColumn* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "ableCo"
                                                                                            "lumn_"
                                                                                            "ImGuiT"
                                                                                            "ableCo"
                                                                                            "lum"
                                                                                            "n"));
            return func();
        }

        inline void Destroy(ImGuiTableColumn* self) {
            using func_t            = void (*)(ImGuiTableColumn*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Table"
                                                                                             "Colum"
                                                                                             "n_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiTableColumnManager

    namespace ImGuiTableInstanceDataManager {
        inline ImGuiTableInstanceData* Create(void) {
            using func_t           = ImGuiTableInstanceData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "ableIn"
                                                                                            "stance"
                                                                                            "Data_"
                                                                                            "ImGuiT"
                                                                                            "ableIn"
                                                                                            "stance"
                                                                                            "Dat"
                                                                                            "a"));
            return func();
        }

        inline void Destroy(ImGuiTableInstanceData* self) {
            using func_t            = void (*)(ImGuiTableInstanceData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Table"
                                                                                             "Insta"
                                                                                             "nceDa"
                                                                                             "ta_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiTableInstanceDataManager

    namespace ImGuiTableManager {
        inline ImGuiTable* Create(void) {
            using func_t           = ImGuiTable* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "able_"
                                                                                            "ImGuiT"
                                                                                            "abl"
                                                                                            "e"));
            return func();
        }

        inline void Destroy(ImGuiTable* self) {
            using func_t            = void (*)(ImGuiTable*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Table"
                                                                                             "_dest"
                                                                                             "ro"
                                                                                             "y"));
            return func(self);
        }
    }  // namespace ImGuiTableManager

    namespace ImGuiTableTempDataManager {
        inline ImGuiTableTempData* Create(void) {
            using func_t           = ImGuiTableTempData* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "ableTe"
                                                                                            "mpData"
                                                                                            "_ImGui"
                                                                                            "TableT"
                                                                                            "empDat"
                                                                                            "a"));
            return func();
        }

        inline void Destroy(ImGuiTableTempData* self) {
            using func_t            = void (*)(ImGuiTableTempData*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Table"
                                                                                             "TempD"
                                                                                             "ata_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiTableTempDataManager

    namespace ImGuiTableColumnSettingsManager {
        inline ImGuiTableColumnSettings* Create(void) {
            using func_t           = ImGuiTableColumnSettings* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "ableCo"
                                                                                            "lumnSe"
                                                                                            "ttings"
                                                                                            "_ImGui"
                                                                                            "TableC"
                                                                                            "olumnS"
                                                                                            "etting"
                                                                                            "s"));
            return func();
        }

        inline void Destroy(ImGuiTableColumnSettings* self) {
            using func_t            = void (*)(ImGuiTableColumnSettings*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Table"
                                                                                             "Colum"
                                                                                             "nSett"
                                                                                             "ings_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }
    }  // namespace ImGuiTableColumnSettingsManager

    namespace ImGuiTableSettingsManager {
        inline ImGuiTableSettings* Create(void) {
            using func_t           = ImGuiTableSettings* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "ableSe"
                                                                                            "ttings"
                                                                                            "_ImGui"
                                                                                            "TableS"
                                                                                            "etting"
                                                                                            "s"));
            return func();
        }

        inline void Destroy(ImGuiTableSettings* self) {
            using func_t            = void (*)(ImGuiTableSettings*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGui"
                                                                                             "Table"
                                                                                             "Setti"
                                                                                             "ngs_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline ImGuiTableColumnSettings* GetColumnSettings(ImGuiTableSettings* self) {
            using func_t           = ImGuiTableColumnSettings* (*)(ImGuiTableSettings*);
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiT"
                                                                                            "ableSe"
                                                                                            "ttings"
                                                                                            "_GetCo"
                                                                                            "lumnSe"
                                                                                            "tting"
                                                                                            "s"));
            return func(self);
        }
    }  // namespace ImGuiTableSettingsManager

    inline ImGuiWindow* GetCurrentWindowRead() {
        using func_t           = ImGuiWindow* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurre"
                                                                                        "ntWindowRe"
                                                                                        "ad"));
        return func();
    }

    inline ImGuiWindow* GetCurrentWindow() {
        using func_t           = ImGuiWindow* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurre"
                                                                                        "ntWindo"
                                                                                        "w"));
        return func();
    }

    inline ImGuiWindow* FindWindowByID(ImGuiID id) {
        using func_t           = ImGuiWindow* (*)(ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindWind"
                                                                                        "owByID"));
        return func(id);
    }

    inline ImGuiWindow* FindWindowByName(const char* name) {
        using func_t           = ImGuiWindow* (*)(const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindWind"
                                                                                        "owByNam"
                                                                                        "e"));
        return func(name);
    }

    inline void UpdateWindowParentAndRootLinks(ImGuiWindow* window, ImGuiWindowFlags flags, ImGuiWindow* parent_window) {
        using func_t            = void (*)(ImGuiWindow*, ImGuiWindowFlags, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igUpdateW"
                                                                                         "indowPare"
                                                                                         "ntAndRoot"
                                                                                         "Links"));
        return func(window, flags, parent_window);
    }

    inline void UpdateWindowSkipRefresh(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igUpdateW"
                                                                                         "indowSkip"
                                                                                         "Refres"
                                                                                         "h"));
        return func(window);
    }

    inline void CalcWindowNextAutoFitSize(ImVec2* pOut, ImGuiWindow* window) {
        using func_t            = void (*)(ImVec2*, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCalcWin"
                                                                                         "dowNextAu"
                                                                                         "toFitSiz"
                                                                                         "e"));
        return func(pOut, window);
    }

    inline bool IsWindowChildOf(ImGuiWindow* window, ImGuiWindow* potential_parent, bool popup_hierarchy, bool dock_hierarchy) {
        using func_t            = bool (*)(ImGuiWindow*, ImGuiWindow*, bool, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsWindo"
                                                                                         "wChildO"
                                                                                         "f"));
        return func(window, potential_parent, popup_hierarchy, dock_hierarchy);
    }

    inline bool IsWindowWithinBeginStackOf(ImGuiWindow* window, ImGuiWindow* potential_parent) {
        using func_t            = bool (*)(ImGuiWindow*, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsWindo"
                                                                                         "wWithinBe"
                                                                                         "ginStackO"
                                                                                         "f"));
        return func(window, potential_parent);
    }

    inline bool IsWindowAbove(ImGuiWindow* potential_above, ImGuiWindow* potential_below) {
        using func_t            = bool (*)(ImGuiWindow*, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsWindo"
                                                                                         "wAbove"));
        return func(potential_above, potential_below);
    }

    inline bool IsWindowNavFocusable(ImGuiWindow* window) {
        using func_t            = bool (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsWindo"
                                                                                         "wNavFocus"
                                                                                         "able"));
        return func(window);
    }

    inline void SetWindowPos(ImGuiWindow* window, const ImVec2 pos, ImGuiCond cond) {
        using func_t            = void (*)(ImGuiWindow*, const ImVec2, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owPos_"
                                                                                         "WindowPt"
                                                                                         "r"));
        return func(window, pos, cond);
    }

    inline void SetWindowSize(ImGuiWindow* window, const ImVec2 size, ImGuiCond cond) {
        using func_t            = void (*)(ImGuiWindow*, const ImVec2, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owSize_"
                                                                                         "WindowPt"
                                                                                         "r"));
        return func(window, size, cond);
    }

    inline void SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond) {
        using func_t            = void (*)(ImGuiWindow*, bool, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owCollaps"
                                                                                         "ed_"
                                                                                         "WindowPt"
                                                                                         "r"));
        return func(window, collapsed, cond);
    }

    inline void SetWindowHitTestHole(ImGuiWindow* window, const ImVec2 pos, const ImVec2 size) {
        using func_t            = void (*)(ImGuiWindow*, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owHitTest"
                                                                                         "Hole"));
        return func(window, pos, size);
    }

    inline void SetWindowHiddenAndSkipItemsForCurrentFrame(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owHiddenA"
                                                                                         "ndSkipIte"
                                                                                         "msForCurr"
                                                                                         "entFram"
                                                                                         "e"));
        return func(window);
    }

    inline void SetWindowParentWindowForFocusRoute(ImGuiWindow* window, ImGuiWindow* parent_window) {
        using func_t            = void (*)(ImGuiWindow*, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owParentW"
                                                                                         "indowForF"
                                                                                         "ocusRout"
                                                                                         "e"));
        return func(window, parent_window);
    }

    inline void WindowRectAbsToRel(ImRect* pOut, ImGuiWindow* window, const ImRect r) {
        using func_t            = void (*)(ImRect*, ImGuiWindow*, const ImRect);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igWindowR"
                                                                                         "ectAbsToR"
                                                                                         "el"));
        return func(pOut, window, r);
    }

    inline void WindowRectRelToAbs(ImRect* pOut, ImGuiWindow* window, const ImRect r) {
        using func_t            = void (*)(ImRect*, ImGuiWindow*, const ImRect);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igWindowR"
                                                                                         "ectRelToA"
                                                                                         "bs"));
        return func(pOut, window, r);
    }

    inline void WindowPosRelToAbs(ImVec2* pOut, ImGuiWindow* window, const ImVec2 p) {
        using func_t            = void (*)(ImVec2*, ImGuiWindow*, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igWindowP"
                                                                                         "osRelToAb"
                                                                                         "s"));
        return func(pOut, window, p);
    }

    inline void FocusWindow(ImGuiWindow* window, ImGuiFocusRequestFlags flags) {
        using func_t            = void (*)(ImGuiWindow*, ImGuiFocusRequestFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFocusWi"
                                                                                         "ndow"));
        return func(window, flags);
    }

    inline void FocusTopMostWindowUnderOne(ImGuiWindow* under_this_window, ImGuiWindow* ignore_window, ImGuiViewport* filter_viewport, ImGuiFocusRequestFlags flags) {
        using func_t = void (*)(ImGuiWindow*, ImGuiWindow*, ImGuiViewport*, ImGuiFocusRequestFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFocusTo"
                                                                                         "pMostWind"
                                                                                         "owUnderOn"
                                                                                         "e"));
        return func(under_this_window, ignore_window, filter_viewport, flags);
    }

    inline void BringWindowToFocusFront(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBringWi"
                                                                                         "ndowToFoc"
                                                                                         "usFron"
                                                                                         "t"));
        return func(window);
    }

    inline void BringWindowToDisplayFront(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBringWi"
                                                                                         "ndowToDis"
                                                                                         "playFron"
                                                                                         "t"));
        return func(window);
    }

    inline void BringWindowToDisplayBack(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBringWi"
                                                                                         "ndowToDis"
                                                                                         "playBac"
                                                                                         "k"));
        return func(window);
    }

    inline void BringWindowToDisplayBehind(ImGuiWindow* window, ImGuiWindow* above_window) {
        using func_t            = void (*)(ImGuiWindow*, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBringWi"
                                                                                         "ndowToDis"
                                                                                         "playBehin"
                                                                                         "d"));
        return func(window, above_window);
    }

    inline int FindWindowDisplayIndex(ImGuiWindow* window) {
        using func_t           = int (*)(ImGuiWindow*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindWind"
                                                                                        "owDisplayI"
                                                                                        "ndex"));
        return func(window);
    }

    inline ImGuiWindow* FindBottomMostVisibleWindowWithinBeginStack(ImGuiWindow* window) {
        using func_t           = ImGuiWindow* (*)(ImGuiWindow*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindBott"
                                                                                        "omMostVisi"
                                                                                        "bleWindowW"
                                                                                        "ithinBegin"
                                                                                        "Stack"));
        return func(window);
    }

    inline void SetNextWindowRefreshPolicy(ImGuiWindowRefreshFlags flags) {
        using func_t            = void (*)(ImGuiWindowRefreshFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "WindowRef"
                                                                                         "reshPolic"
                                                                                         "y"));
        return func(flags);
    }

    inline void SetCurrentFont(ImFont* font) {
        using func_t            = void (*)(ImFont*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetCurr"
                                                                                         "entFon"
                                                                                         "t"));
        return func(font);
    }

    inline ImFont* GetDefaultFont() {
        using func_t           = ImFont* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetDefau"
                                                                                        "ltFont"));
        return func();
    }

    inline ImDrawList* GetForegroundDrawList(ImGuiWindow* window) {
        using func_t           = ImDrawList* (*)(ImGuiWindow*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetForeg"
                                                                                        "roundDrawL"
                                                                                        "ist_"
                                                                                        "WindowPt"
                                                                                        "r"));
        return func(window);
    }

    inline void AddDrawListToDrawDataEx(ImDrawData* draw_data, ImVector_ImDrawListPtr* out_list, ImDrawList* draw_list) {
        using func_t            = void (*)(ImDrawData*, ImVector_ImDrawListPtr*, ImDrawList*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igAddDraw"
                                                                                         "ListToDra"
                                                                                         "wDataE"
                                                                                         "x"));
        return func(draw_data, out_list, draw_list);
    }

    inline void Initialize() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInitial"
                                                                                         "ize"));
        return func();
    }

    inline void Shutdown() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShutdow"
                                                                                         "n"));
        return func();
    }

    inline void UpdateInputEvents(bool trickle_fast_inputs) {
        using func_t            = void (*)(bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igUpdateI"
                                                                                         "nputEvent"
                                                                                         "s"));
        return func(trickle_fast_inputs);
    }

    inline void UpdateHoveredWindowAndCaptureFlags() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igUpdateH"
                                                                                         "overedWin"
                                                                                         "dowAndCap"
                                                                                         "tureFlag"
                                                                                         "s"));
        return func();
    }

    inline void FindHoveredWindowEx(const ImVec2 pos, bool find_first_and_in_any_viewport, ImGuiWindow** out_hovered_window, ImGuiWindow** out_hovered_window_under_moving_window) {
        using func_t            = void (*)(const ImVec2, bool, ImGuiWindow**, ImGuiWindow**);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindHov"
                                                                                         "eredWindo"
                                                                                         "wEx"));
        return func(pos, find_first_and_in_any_viewport, out_hovered_window, out_hovered_window_under_moving_window);
    }

    inline void StartMouseMovingWindow(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igStartMo"
                                                                                         "useMoving"
                                                                                         "Window"));
        return func(window);
    }

    inline void StartMouseMovingWindowOrNode(ImGuiWindow* window, ImGuiDockNode* node, bool undock) {
        using func_t            = void (*)(ImGuiWindow*, ImGuiDockNode*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igStartMo"
                                                                                         "useMoving"
                                                                                         "WindowOrN"
                                                                                         "ode"));
        return func(window, node, undock);
    }

    inline void UpdateMouseMovingWindowNewFrame() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igUpdateM"
                                                                                         "ouseMovin"
                                                                                         "gWindowNe"
                                                                                         "wFrame"));
        return func();
    }

    inline void UpdateMouseMovingWindowEndFrame() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igUpdateM"
                                                                                         "ouseMovin"
                                                                                         "gWindowEn"
                                                                                         "dFrame"));
        return func();
    }

    inline ImGuiID AddContextHook(ImGuiContext* context, const ImGuiContextHook* hook) {
        using func_t           = ImGuiID (*)(ImGuiContext*, const ImGuiContextHook*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igAddConte"
                                                                                        "xtHook"));
        return func(context, hook);
    }

    inline void RemoveContextHook(ImGuiContext* context, ImGuiID hook_to_remove) {
        using func_t            = void (*)(ImGuiContext*, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRemoveC"
                                                                                         "ontextHoo"
                                                                                         "k"));
        return func(context, hook_to_remove);
    }

    inline void CallContextHooks(ImGuiContext* context, ImGuiContextHookType type) {
        using func_t            = void (*)(ImGuiContext*, ImGuiContextHookType);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCallCon"
                                                                                         "textHook"
                                                                                         "s"));
        return func(context, type);
    }

    inline void TranslateWindowsInViewport(ImGuiViewportP* viewport, const ImVec2 old_pos, const ImVec2 new_pos) {
        using func_t            = void (*)(ImGuiViewportP*, const ImVec2, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTransla"
                                                                                         "teWindows"
                                                                                         "InViewpor"
                                                                                         "t"));
        return func(viewport, old_pos, new_pos);
    }

    inline void ScaleWindowsInViewport(ImGuiViewportP* viewport, float scale) {
        using func_t            = void (*)(ImGuiViewportP*, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igScaleWi"
                                                                                         "ndowsInVi"
                                                                                         "ewport"));
        return func(viewport, scale);
    }

    inline void DestroyPlatformWindow(ImGuiViewportP* viewport) {
        using func_t            = void (*)(ImGuiViewportP*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDestroy"
                                                                                         "PlatformW"
                                                                                         "indow"));
        return func(viewport);
    }

    inline void SetWindowViewport(ImGuiWindow* window, ImGuiViewportP* viewport) {
        using func_t            = void (*)(ImGuiWindow*, ImGuiViewportP*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owViewpor"
                                                                                         "t"));
        return func(window, viewport);
    }

    inline void SetCurrentViewport(ImGuiWindow* window, ImGuiViewportP* viewport) {
        using func_t            = void (*)(ImGuiWindow*, ImGuiViewportP*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetCurr"
                                                                                         "entViewpo"
                                                                                         "rt"));
        return func(window, viewport);
    }

    inline const ImGuiPlatformMonitor* GetViewportPlatformMonitor(ImGuiViewport* viewport) {
        using func_t           = const ImGuiPlatformMonitor* (*)(ImGuiViewport*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetViewp"
                                                                                        "ortPlatfor"
                                                                                        "mMonito"
                                                                                        "r"));
        return func(viewport);
    }

    inline ImGuiViewportP* FindHoveredViewportFromPlatformWindowStack(const ImVec2 mouse_platform_pos) {
        using func_t           = ImGuiViewportP* (*)(const ImVec2);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindHove"
                                                                                        "redViewpor"
                                                                                        "tFromPlatf"
                                                                                        "ormWindowS"
                                                                                        "tack"));
        return func(mouse_platform_pos);
    }

    inline void MarkIniSettingsDirty() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igMarkIni"
                                                                                         "SettingsD"
                                                                                         "irty_"
                                                                                         "Nil"));
        return func();
    }

    inline void MarkIniSettingsDirty(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igMarkIni"
                                                                                         "SettingsD"
                                                                                         "irty_"
                                                                                         "WindowPt"
                                                                                         "r"));
        return func(window);
    }

    inline void ClearIniSettings() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igClearIn"
                                                                                         "iSetting"
                                                                                         "s"));
        return func();
    }

    inline void AddSettingsHandler(const ImGuiSettingsHandler* handler) {
        using func_t            = void (*)(const ImGuiSettingsHandler*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igAddSett"
                                                                                         "ingsHandl"
                                                                                         "er"));
        return func(handler);
    }

    inline void RemoveSettingsHandler(const char* type_name) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRemoveS"
                                                                                         "ettingsHa"
                                                                                         "ndler"));
        return func(type_name);
    }

    inline ImGuiSettingsHandler* FindSettingsHandler(const char* type_name) {
        using func_t           = ImGuiSettingsHandler* (*)(const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindSett"
                                                                                        "ingsHandle"
                                                                                        "r"));
        return func(type_name);
    }

    inline ImGuiWindowSettings* CreateNewWindowSettings(const char* name) {
        using func_t           = ImGuiWindowSettings* (*)(const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCreateNe"
                                                                                        "wWindowSet"
                                                                                        "tings"));
        return func(name);
    }

    inline ImGuiWindowSettings* FindWindowSettingsByID(ImGuiID id) {
        using func_t           = ImGuiWindowSettings* (*)(ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindWind"
                                                                                        "owSettings"
                                                                                        "ByID"));
        return func(id);
    }

    inline ImGuiWindowSettings* FindWindowSettingsByWindow(ImGuiWindow* window) {
        using func_t           = ImGuiWindowSettings* (*)(ImGuiWindow*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindWind"
                                                                                        "owSettings"
                                                                                        "ByWindo"
                                                                                        "w"));
        return func(window);
    }

    inline void ClearWindowSettings(const char* name) {
        using func_t            = void (*)(const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igClearWi"
                                                                                         "ndowSetti"
                                                                                         "ngs"));
        return func(name);
    }

    inline void LocalizeRegisterEntries(const ImGuiLocEntry* entries, int count) {
        using func_t            = void (*)(const ImGuiLocEntry*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLocaliz"
                                                                                         "eRegister"
                                                                                         "Entrie"
                                                                                         "s"));
        return func(entries, count);
    }

    inline const char* LocalizeGetMsg(ImGuiLocKey key) {
        using func_t           = const char* (*)(ImGuiLocKey);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLocalize"
                                                                                        "GetMsg"));
        return func(key);
    }

    inline void SetScrollX(ImGuiWindow* window, float scroll_x) {
        using func_t            = void (*)(ImGuiWindow*, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetScro"
                                                                                         "llX_"
                                                                                         "WindowPt"
                                                                                         "r"));
        return func(window, scroll_x);
    }

    inline void SetScrollY(ImGuiWindow* window, float scroll_y) {
        using func_t            = void (*)(ImGuiWindow*, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetScro"
                                                                                         "llY_"
                                                                                         "WindowPt"
                                                                                         "r"));
        return func(window, scroll_y);
    }

    inline void SetScrollFromPosX(ImGuiWindow* window, float local_x, float center_x_ratio) {
        using func_t            = void (*)(ImGuiWindow*, float, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetScro"
                                                                                         "llFromPos"
                                                                                         "X_"
                                                                                         "WindowPt"
                                                                                         "r"));
        return func(window, local_x, center_x_ratio);
    }

    inline void SetScrollFromPosY(ImGuiWindow* window, float local_y, float center_y_ratio) {
        using func_t            = void (*)(ImGuiWindow*, float, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetScro"
                                                                                         "llFromPos"
                                                                                         "Y_"
                                                                                         "WindowPt"
                                                                                         "r"));
        return func(window, local_y, center_y_ratio);
    }

    inline void ScrollToItem(ImGuiScrollFlags flags) {
        using func_t            = void (*)(ImGuiScrollFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igScrollT"
                                                                                         "oItem"));
        return func(flags);
    }

    inline void ScrollToRect(ImGuiWindow* window, const ImRect rect, ImGuiScrollFlags flags) {
        using func_t            = void (*)(ImGuiWindow*, const ImRect, ImGuiScrollFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igScrollT"
                                                                                         "oRect"));
        return func(window, rect, flags);
    }

    inline void ScrollToRectEx(ImVec2* pOut, ImGuiWindow* window, const ImRect rect, ImGuiScrollFlags flags) {
        using func_t            = void (*)(ImVec2*, ImGuiWindow*, const ImRect, ImGuiScrollFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igScrollT"
                                                                                         "oRectE"
                                                                                         "x"));
        return func(pOut, window, rect, flags);
    }

    inline void ScrollToBringRectIntoView(ImGuiWindow* window, const ImRect rect) {
        using func_t            = void (*)(ImGuiWindow*, const ImRect);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igScrollT"
                                                                                         "oBringRec"
                                                                                         "tIntoVie"
                                                                                         "w"));
        return func(window, rect);
    }

    inline ImGuiItemStatusFlags GetItemStatusFlags() {
        using func_t           = ImGuiItemStatusFlags (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetItemS"
                                                                                        "tatusFlag"
                                                                                        "s"));
        return func();
    }

    inline ImGuiItemFlags GetItemFlags() {
        using func_t           = ImGuiItemFlags (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetItemF"
                                                                                        "lags"));
        return func();
    }

    inline ImGuiID GetActiveID() {
        using func_t           = ImGuiID (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetActiv"
                                                                                        "eID"));
        return func();
    }

    inline ImGuiID GetFocusID() {
        using func_t           = ImGuiID (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetFocus"
                                                                                        "ID"));
        return func();
    }

    inline void SetActiveID(ImGuiID id, ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiID, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetActi"
                                                                                         "veID"));
        return func(id, window);
    }

    inline void SetFocusID(ImGuiID id, ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiID, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetFocu"
                                                                                         "sID"));
        return func(id, window);
    }

    inline void ClearActiveID() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igClearAc"
                                                                                         "tiveID"));
        return func();
    }

    inline ImGuiID GetHoveredID() {
        using func_t           = ImGuiID (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetHover"
                                                                                        "edID"));
        return func();
    }

    inline void SetHoveredID(ImGuiID id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetHove"
                                                                                         "redID"));
        return func(id);
    }

    inline void KeepAliveID(ImGuiID id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igKeepAli"
                                                                                         "veID"));
        return func(id);
    }

    inline void MarkItemEdited(ImGuiID id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igMarkIte"
                                                                                         "mEdite"
                                                                                         "d"));
        return func(id);
    }

    inline void PushOverrideID(ImGuiID id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushOve"
                                                                                         "rrideI"
                                                                                         "D"));
        return func(id);
    }

    inline ImGuiID GetIDWithSeed(const char* str_id_begin, const char* str_id_end, ImGuiID seed) {
        using func_t           = ImGuiID (*)(const char*, const char*, ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetIDWit"
                                                                                        "hSeed_"
                                                                                        "Str"));
        return func(str_id_begin, str_id_end, seed);
    }

    inline ImGuiID GetIDWithSeed(int n, ImGuiID seed) {
        using func_t           = ImGuiID (*)(int, ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetIDWit"
                                                                                        "hSeed_"
                                                                                        "Int"));
        return func(n, seed);
    }

    inline void ItemSize(const ImVec2 size, float text_baseline_y) {
        using func_t            = void (*)(const ImVec2, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igItemSiz"
                                                                                         "e_Vec2"));
        return func(size, text_baseline_y);
    }

    inline void ItemSize(const ImRect bb, float text_baseline_y) {
        using func_t            = void (*)(const ImRect, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igItemSiz"
                                                                                         "e_Rect"));
        return func(bb, text_baseline_y);
    }

    inline bool ItemAdd(const ImRect bb, ImGuiID id, const ImRect* nav_bb, ImGuiItemFlags extra_flags) {
        using func_t            = bool (*)(const ImRect, ImGuiID, const ImRect*, ImGuiItemFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igItemAd"
                                                                                         "d"));
        return func(bb, id, nav_bb, extra_flags);
    }

    inline bool ItemHoverable(const ImRect bb, ImGuiID id, ImGuiItemFlags item_flags) {
        using func_t            = bool (*)(const ImRect, ImGuiID, ImGuiItemFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igItemHov"
                                                                                         "erable"));
        return func(bb, id, item_flags);
    }

    inline bool IsWindowContentHoverable(ImGuiWindow* window, ImGuiHoveredFlags flags) {
        using func_t            = bool (*)(ImGuiWindow*, ImGuiHoveredFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsWindo"
                                                                                         "wContentH"
                                                                                         "overabl"
                                                                                         "e"));
        return func(window, flags);
    }

    inline bool IsClippedEx(const ImRect bb, ImGuiID id) {
        using func_t            = bool (*)(const ImRect, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsClipp"
                                                                                         "edEx"));
        return func(bb, id);
    }

    inline void SetLastItemData(ImGuiID item_id, ImGuiItemFlags in_flags, ImGuiItemStatusFlags status_flags, const ImRect item_rect) {
        using func_t = void (*)(ImGuiID, ImGuiItemFlags, ImGuiItemStatusFlags, const ImRect);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetLast"
                                                                                         "ItemDat"
                                                                                         "a"));
        return func(item_id, in_flags, status_flags, item_rect);
    }

    inline void CalcItemSize(ImVec2* pOut, ImVec2 size, float default_w, float default_h) {
        using func_t            = void (*)(ImVec2*, ImVec2, float, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCalcIte"
                                                                                         "mSize"));
        return func(pOut, size, default_w, default_h);
    }

    inline float CalcWrapWidthForPos(const ImVec2 pos, float wrap_pos_x) {
        using func_t           = float (*)(const ImVec2, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCalcWrap"
                                                                                        "WidthForPo"
                                                                                        "s"));
        return func(pos, wrap_pos_x);
    }

    inline void PushMultiItemsWidths(int components, float width_full) {
        using func_t            = void (*)(int, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushMul"
                                                                                         "tiItemsWi"
                                                                                         "dths"));
        return func(components, width_full);
    }

    inline bool IsItemToggledSelection() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsItemT"
                                                                                         "oggledSel"
                                                                                         "ection"));
        return func();
    }

    inline void GetContentRegionMaxAbs(ImVec2* pOut) {
        using func_t            = void (*)(ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCont"
                                                                                         "entRegion"
                                                                                         "MaxAbs"));
        return func(pOut);
    }

    inline void ShrinkWidths(ImGuiShrinkWidthItem* items, int count, float width_excess) {
        using func_t            = void (*)(ImGuiShrinkWidthItem*, int, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShrinkW"
                                                                                         "idths"));
        return func(items, count, width_excess);
    }

    inline void PushItemFlag(ImGuiItemFlags option, bool enabled) {
        using func_t            = void (*)(ImGuiItemFlags, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushIte"
                                                                                         "mFlag"));
        return func(option, enabled);
    }

    inline void PopItemFlag() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopItem"
                                                                                         "Flag"));
        return func();
    }

    inline const ImGuiDataVarInfo* GetStyleVarInfo(ImGuiStyleVar idx) {
        using func_t           = const ImGuiDataVarInfo* (*)(ImGuiStyleVar);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetStyle"
                                                                                        "VarInfo"));
        return func(idx);
    }

    inline void BeginDisabledOverrideReenable() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginDi"
                                                                                         "sabledOve"
                                                                                         "rrideReen"
                                                                                         "able"));
        return func();
    }

    inline void EndDisabledOverrideReenable() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndDisa"
                                                                                         "bledOverr"
                                                                                         "ideReenab"
                                                                                         "le"));
        return func();
    }

    inline void LogBegin(ImGuiLogType type, int auto_open_depth) {
        using func_t            = void (*)(ImGuiLogType, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogBegi"
                                                                                         "n"));
        return func(type, auto_open_depth);
    }

    inline void LogToBuffer(int auto_open_depth) {
        using func_t            = void (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogToBu"
                                                                                         "ffer"));
        return func(auto_open_depth);
    }

    inline void LogRenderedText(const ImVec2* ref_pos, const char* text, const char* text_end) {
        using func_t            = void (*)(const ImVec2*, const char*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogRend"
                                                                                         "eredTex"
                                                                                         "t"));
        return func(ref_pos, text, text_end);
    }

    inline void LogSetNextTextDecoration(const char* prefix, const char* suffix) {
        using func_t            = void (*)(const char*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogSetN"
                                                                                         "extTextDe"
                                                                                         "coratio"
                                                                                         "n"));
        return func(prefix, suffix);
    }

    inline bool BeginChildEx(const char* name, ImGuiID id, const ImVec2 size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags) {
        using func_t = bool (*)(const char*, ImGuiID, const ImVec2, ImGuiChildFlags, ImGuiWindowFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginCh"
                                                                                         "ildEx"));
        return func(name, id, size_arg, child_flags, window_flags);
    }

    inline void OpenPopupEx(ImGuiID id, ImGuiPopupFlags popup_flags) {
        using func_t            = void (*)(ImGuiID, ImGuiPopupFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igOpenPop"
                                                                                         "upEx"));
        return func(id, popup_flags);
    }

    inline void ClosePopupToLevel(int remaining, bool restore_focus_to_window_under_popup) {
        using func_t            = void (*)(int, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igClosePo"
                                                                                         "pupToLeve"
                                                                                         "l"));
        return func(remaining, restore_focus_to_window_under_popup);
    }

    inline void ClosePopupsOverWindow(ImGuiWindow* ref_window, bool restore_focus_to_window_under_popup) {
        using func_t            = void (*)(ImGuiWindow*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igClosePo"
                                                                                         "pupsOverW"
                                                                                         "indow"));
        return func(ref_window, restore_focus_to_window_under_popup);
    }

    inline void ClosePopupsExceptModals() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igClosePo"
                                                                                         "pupsExcep"
                                                                                         "tModal"
                                                                                         "s"));
        return func();
    }

    inline bool IsPopupOpen(ImGuiID id, ImGuiPopupFlags popup_flags) {
        using func_t            = bool (*)(ImGuiID, ImGuiPopupFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsPopup"
                                                                                         "Open_"
                                                                                         "ID"));
        return func(id, popup_flags);
    }

    inline bool BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags) {
        using func_t            = bool (*)(ImGuiID, ImGuiWindowFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginPo"
                                                                                         "pupEx"));
        return func(id, extra_flags);
    }

    inline bool BeginTooltipEx(ImGuiTooltipFlags tooltip_flags, ImGuiWindowFlags extra_window_flags) {
        using func_t            = bool (*)(ImGuiTooltipFlags, ImGuiWindowFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginTo"
                                                                                         "oltipE"
                                                                                         "x"));
        return func(tooltip_flags, extra_window_flags);
    }

    inline bool BeginTooltipHidden() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginTo"
                                                                                         "oltipHidd"
                                                                                         "en"));
        return func();
    }

    inline void GetPopupAllowedExtentRect(ImRect* pOut, ImGuiWindow* window) {
        using func_t            = void (*)(ImRect*, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetPopu"
                                                                                         "pAllowedE"
                                                                                         "xtentRec"
                                                                                         "t"));
        return func(pOut, window);
    }

    inline ImGuiWindow* GetTopMostPopupModal() {
        using func_t           = ImGuiWindow* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetTopMo"
                                                                                        "stPopupMod"
                                                                                        "al"));
        return func();
    }

    inline ImGuiWindow* GetTopMostAndVisiblePopupModal() {
        using func_t           = ImGuiWindow* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetTopMo"
                                                                                        "stAndVisib"
                                                                                        "lePopupMod"
                                                                                        "al"));
        return func();
    }

    inline ImGuiWindow* FindBlockingModal(ImGuiWindow* window) {
        using func_t           = ImGuiWindow* (*)(ImGuiWindow*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindBloc"
                                                                                        "kingModa"
                                                                                        "l"));
        return func(window);
    }

    inline void FindBestWindowPosForPopup(ImVec2* pOut, ImGuiWindow* window) {
        using func_t            = void (*)(ImVec2*, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindBes"
                                                                                         "tWindowPo"
                                                                                         "sForPopu"
                                                                                         "p"));
        return func(pOut, window);
    }

    inline void FindBestWindowPosForPopupEx(ImVec2* pOut, const ImVec2 ref_pos, const ImVec2 size, ImGuiDir* last_dir, const ImRect r_outer, const ImRect r_avoid, ImGuiPopupPositionPolicy policy) {
        using func_t = void (*)(ImVec2*, const ImVec2, const ImVec2, ImGuiDir*, const ImRect, const ImRect, ImGuiPopupPositionPolicy);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindBes"
                                                                                         "tWindowPo"
                                                                                         "sForPopup"
                                                                                         "Ex"));
        return func(pOut, ref_pos, size, last_dir, r_outer, r_avoid, policy);
    }

    inline bool BeginViewportSideBar(const char* name, ImGuiViewport* viewport, ImGuiDir dir, float size, ImGuiWindowFlags window_flags) {
        using func_t = bool (*)(const char*, ImGuiViewport*, ImGuiDir, float, ImGuiWindowFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginVi"
                                                                                         "ewportSid"
                                                                                         "eBar"));
        return func(name, viewport, dir, size, window_flags);
    }

    inline bool BeginMenuEx(const char* label, const char* icon, bool enabled) {
        using func_t            = bool (*)(const char*, const char*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginMe"
                                                                                         "nuEx"));
        return func(label, icon, enabled);
    }

    inline bool MenuItemEx(const char* label, const char* icon, const char* shortcut, bool selected, bool enabled) {
        using func_t            = bool (*)(const char*, const char*, const char*, bool, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igMenuIte"
                                                                                         "mEx"));
        return func(label, icon, shortcut, selected, enabled);
    }

    inline bool BeginComboPopup(ImGuiID popup_id, const ImRect bb, ImGuiComboFlags flags) {
        using func_t            = bool (*)(ImGuiID, const ImRect, ImGuiComboFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginCo"
                                                                                         "mboPopu"
                                                                                         "p"));
        return func(popup_id, bb, flags);
    }

    inline bool BeginComboPreview() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginCo"
                                                                                         "mboPrevie"
                                                                                         "w"));
        return func();
    }

    inline void EndComboPreview() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndComb"
                                                                                         "oPrevie"
                                                                                         "w"));
        return func();
    }

    inline void NavInitWindow(ImGuiWindow* window, bool force_reinit) {
        using func_t            = void (*)(ImGuiWindow*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavInit"
                                                                                         "Window"));
        return func(window, force_reinit);
    }

    inline void NavInitRequestApplyResult() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavInit"
                                                                                         "RequestAp"
                                                                                         "plyResul"
                                                                                         "t"));
        return func();
    }

    inline bool NavMoveRequestButNoResultYet() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavMove"
                                                                                         "RequestBu"
                                                                                         "tNoResult"
                                                                                         "Yet"));
        return func();
    }

    inline void NavMoveRequestSubmit(ImGuiDir move_dir, ImGuiDir clip_dir, ImGuiNavMoveFlags move_flags, ImGuiScrollFlags scroll_flags) {
        using func_t            = void (*)(ImGuiDir, ImGuiDir, ImGuiNavMoveFlags, ImGuiScrollFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavMove"
                                                                                         "RequestSu"
                                                                                         "bmit"));
        return func(move_dir, clip_dir, move_flags, scroll_flags);
    }

    inline void NavMoveRequestForward(ImGuiDir move_dir, ImGuiDir clip_dir, ImGuiNavMoveFlags move_flags, ImGuiScrollFlags scroll_flags) {
        using func_t            = void (*)(ImGuiDir, ImGuiDir, ImGuiNavMoveFlags, ImGuiScrollFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavMove"
                                                                                         "RequestFo"
                                                                                         "rward"));
        return func(move_dir, clip_dir, move_flags, scroll_flags);
    }

    inline void NavMoveRequestResolveWithLastItem(ImGuiNavItemData* result) {
        using func_t            = void (*)(ImGuiNavItemData*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavMove"
                                                                                         "RequestRe"
                                                                                         "solveWith"
                                                                                         "LastIte"
                                                                                         "m"));
        return func(result);
    }

    inline void NavMoveRequestResolveWithPastTreeNode(ImGuiNavItemData* result, ImGuiNavTreeNodeData* tree_node_data) {
        using func_t            = void (*)(ImGuiNavItemData*, ImGuiNavTreeNodeData*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavMove"
                                                                                         "RequestRe"
                                                                                         "solveWith"
                                                                                         "PastTreeN"
                                                                                         "ode"));
        return func(result, tree_node_data);
    }

    inline void NavMoveRequestCancel() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavMove"
                                                                                         "RequestCa"
                                                                                         "ncel"));
        return func();
    }

    inline void NavMoveRequestApplyResult() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavMove"
                                                                                         "RequestAp"
                                                                                         "plyResul"
                                                                                         "t"));
        return func();
    }

    inline void NavMoveRequestTryWrapping(ImGuiWindow* window, ImGuiNavMoveFlags move_flags) {
        using func_t            = void (*)(ImGuiWindow*, ImGuiNavMoveFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavMove"
                                                                                         "RequestTr"
                                                                                         "yWrappin"
                                                                                         "g"));
        return func(window, move_flags);
    }

    inline void NavHighlightActivated(ImGuiID id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavHigh"
                                                                                         "lightActi"
                                                                                         "vated"));
        return func(id);
    }

    inline void NavClearPreferredPosForAxis(ImGuiAxis axis) {
        using func_t            = void (*)(ImGuiAxis);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavClea"
                                                                                         "rPreferre"
                                                                                         "dPosForAx"
                                                                                         "is"));
        return func(axis);
    }

    inline void NavRestoreHighlightAfterMove() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavRest"
                                                                                         "oreHighli"
                                                                                         "ghtAfterM"
                                                                                         "ove"));
        return func();
    }

    inline void NavUpdateCurrentWindowIsScrollPushableX() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igNavUpda"
                                                                                         "teCurrent"
                                                                                         "WindowIsS"
                                                                                         "crollPush"
                                                                                         "ableX"));
        return func();
    }

    inline void SetNavWindow(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNavW"
                                                                                         "indow"));
        return func(window);
    }

    inline void SetNavID(ImGuiID id, ImGuiNavLayer nav_layer, ImGuiID focus_scope_id, const ImRect rect_rel) {
        using func_t            = void (*)(ImGuiID, ImGuiNavLayer, ImGuiID, const ImRect);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNavI"
                                                                                         "D"));
        return func(id, nav_layer, focus_scope_id, rect_rel);
    }

    inline void SetNavFocusScope(ImGuiID focus_scope_id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNavF"
                                                                                         "ocusScop"
                                                                                         "e"));
        return func(focus_scope_id);
    }

    inline void FocusItem() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFocusIt"
                                                                                         "em"));
        return func();
    }

    inline void ActivateItemByID(ImGuiID id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igActivat"
                                                                                         "eItemByI"
                                                                                         "D"));
        return func(id);
    }

    inline bool IsNamedKey(ImGuiKey key) {
        using func_t            = bool (*)(ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsNamed"
                                                                                         "Key"));
        return func(key);
    }

    inline bool IsNamedKeyOrMod(ImGuiKey key) {
        using func_t            = bool (*)(ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsNamed"
                                                                                         "KeyOrMo"
                                                                                         "d"));
        return func(key);
    }

    inline bool IsLegacyKey(ImGuiKey key) {
        using func_t            = bool (*)(ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsLegac"
                                                                                         "yKey"));
        return func(key);
    }

    inline bool IsKeyboardKey(ImGuiKey key) {
        using func_t            = bool (*)(ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsKeybo"
                                                                                         "ardKey"));
        return func(key);
    }

    inline bool IsGamepadKey(ImGuiKey key) {
        using func_t            = bool (*)(ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsGamep"
                                                                                         "adKey"));
        return func(key);
    }

    inline bool IsMouseKey(ImGuiKey key) {
        using func_t            = bool (*)(ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "Key"));
        return func(key);
    }

    inline bool IsAliasKey(ImGuiKey key) {
        using func_t            = bool (*)(ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsAlias"
                                                                                         "Key"));
        return func(key);
    }

    inline bool IsModKey(ImGuiKey key) {
        using func_t            = bool (*)(ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsModKe"
                                                                                         "y"));
        return func(key);
    }

    inline ImGuiKeyChord FixupKeyChord(ImGuiKeyChord key_chord) {
        using func_t           = ImGuiKeyChord (*)(ImGuiKeyChord);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFixupKey"
                                                                                        "Chord"));
        return func(key_chord);
    }

    inline ImGuiKey ConvertSingleModFlagToKey(ImGuiKey key) {
        using func_t           = ImGuiKey (*)(ImGuiKey);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igConvertS"
                                                                                        "ingleModFl"
                                                                                        "agToKey"));
        return func(key);
    }

    inline ImGuiKeyData* GetKeyData(ImGuiContext* ctx, ImGuiKey key) {
        using func_t           = ImGuiKeyData* (*)(ImGuiContext*, ImGuiKey);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetKeyDa"
                                                                                        "ta_"
                                                                                        "ContextPt"
                                                                                        "r"));
        return func(ctx, key);
    }

    inline ImGuiKeyData* GetKeyData(ImGuiKey key) {
        using func_t           = ImGuiKeyData* (*)(ImGuiKey);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetKeyDa"
                                                                                        "ta_Key"));
        return func(key);
    }

    inline const char* GetKeyChordName(ImGuiKeyChord key_chord) {
        using func_t           = const char* (*)(ImGuiKeyChord);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetKeyCh"
                                                                                        "ordName"));
        return func(key_chord);
    }

    inline ImGuiKey MouseButtonToKey(ImGuiMouseButton button) {
        using func_t           = ImGuiKey (*)(ImGuiMouseButton);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igMouseBut"
                                                                                        "tonToKe"
                                                                                        "y"));
        return func(button);
    }

    inline bool IsMouseDragPastThreshold(ImGuiMouseButton button, float lock_threshold) {
        using func_t            = bool (*)(ImGuiMouseButton, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "DragPastT"
                                                                                         "hreshol"
                                                                                         "d"));
        return func(button, lock_threshold);
    }

    inline void GetKeyMagnitude2d(ImVec2* pOut, ImGuiKey key_left, ImGuiKey key_right, ImGuiKey key_up, ImGuiKey key_down) {
        using func_t            = void (*)(ImVec2*, ImGuiKey, ImGuiKey, ImGuiKey, ImGuiKey);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetKeyM"
                                                                                         "agnitude2"
                                                                                         "d"));
        return func(pOut, key_left, key_right, key_up, key_down);
    }

    inline float GetNavTweakPressedAmount(ImGuiAxis axis) {
        using func_t           = float (*)(ImGuiAxis);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetNavTw"
                                                                                        "eakPressed"
                                                                                        "Amount"));
        return func(axis);
    }

    inline int CalcTypematicRepeatAmount(float t0, float t1, float repeat_delay, float repeat_rate) {
        using func_t           = int (*)(float, float, float, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCalcType"
                                                                                        "maticRepea"
                                                                                        "tAmount"));
        return func(t0, t1, repeat_delay, repeat_rate);
    }

    inline void GetTypematicRepeatRate(ImGuiInputFlags flags, float* repeat_delay, float* repeat_rate) {
        using func_t            = void (*)(ImGuiInputFlags, float*, float*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetType"
                                                                                         "maticRepe"
                                                                                         "atRate"));
        return func(flags, repeat_delay, repeat_rate);
    }

    inline void TeleportMousePos(const ImVec2 pos) {
        using func_t            = void (*)(const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTelepor"
                                                                                         "tMousePo"
                                                                                         "s"));
        return func(pos);
    }

    inline void SetActiveIdUsingAllKeyboardKeys() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetActi"
                                                                                         "veIdUsing"
                                                                                         "AllKeyboa"
                                                                                         "rdKeys"));
        return func();
    }

    inline bool IsActiveIdUsingNavDir(ImGuiDir dir) {
        using func_t            = bool (*)(ImGuiDir);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsActiv"
                                                                                         "eIdUsingN"
                                                                                         "avDir"));
        return func(dir);
    }

    inline ImGuiID GetKeyOwner(ImGuiKey key) {
        using func_t           = ImGuiID (*)(ImGuiKey);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetKeyOw"
                                                                                        "ner"));
        return func(key);
    }

    inline void SetKeyOwner(ImGuiKey key, ImGuiID owner_id, ImGuiInputFlags flags) {
        using func_t            = void (*)(ImGuiKey, ImGuiID, ImGuiInputFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetKeyO"
                                                                                         "wner"));
        return func(key, owner_id, flags);
    }

    inline void SetKeyOwnersForKeyChord(ImGuiKeyChord key, ImGuiID owner_id, ImGuiInputFlags flags) {
        using func_t            = void (*)(ImGuiKeyChord, ImGuiID, ImGuiInputFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetKeyO"
                                                                                         "wnersForK"
                                                                                         "eyChor"
                                                                                         "d"));
        return func(key, owner_id, flags);
    }

    inline void SetItemKeyOwner(ImGuiKey key, ImGuiInputFlags flags) {
        using func_t            = void (*)(ImGuiKey, ImGuiInputFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetItem"
                                                                                         "KeyOwne"
                                                                                         "r"));
        return func(key, flags);
    }

    inline bool TestKeyOwner(ImGuiKey key, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiKey, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTestKey"
                                                                                         "Owner"));
        return func(key, owner_id);
    }

    inline ImGuiKeyOwnerData* GetKeyOwnerData(ImGuiContext* ctx, ImGuiKey key) {
        using func_t           = ImGuiKeyOwnerData* (*)(ImGuiContext*, ImGuiKey);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetKeyOw"
                                                                                        "nerData"));
        return func(ctx, key);
    }

    inline bool IsKeyDown(ImGuiKey key, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiKey, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsKeyDo"
                                                                                         "wn_ID"));
        return func(key, owner_id);
    }

    inline bool IsKeyPressed(ImGuiKey key, ImGuiInputFlags flags, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiKey, ImGuiInputFlags, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsKeyPr"
                                                                                         "essed_"
                                                                                         "InputFlag"
                                                                                         "s"));
        return func(key, flags, owner_id);
    }

    inline bool IsKeyReleased(ImGuiKey key, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiKey, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsKeyRe"
                                                                                         "leased_"
                                                                                         "ID"));
        return func(key, owner_id);
    }

    inline bool IsKeyChordPressed(ImGuiKeyChord key_chord, ImGuiInputFlags flags, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiKeyChord, ImGuiInputFlags, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsKeyCh"
                                                                                         "ordPresse"
                                                                                         "d_"
                                                                                         "InputFlag"
                                                                                         "s"));
        return func(key_chord, flags, owner_id);
    }

    inline bool IsMouseDown(ImGuiMouseButton button, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiMouseButton, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "Down_"
                                                                                         "ID"));
        return func(button, owner_id);
    }

    inline bool IsMouseClicked(ImGuiMouseButton button, ImGuiInputFlags flags, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiMouseButton, ImGuiInputFlags, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "Clicked_"
                                                                                         "InputFlag"
                                                                                         "s"));
        return func(button, flags, owner_id);
    }

    inline bool IsMouseReleased(ImGuiMouseButton button, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiMouseButton, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "Released_"
                                                                                         "ID"));
        return func(button, owner_id);
    }

    inline bool IsMouseDoubleClicked(ImGuiMouseButton button, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiMouseButton, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsMouse"
                                                                                         "DoubleCli"
                                                                                         "cked_"
                                                                                         "ID"));
        return func(button, owner_id);
    }

    inline bool Shortcut(ImGuiKeyChord key_chord, ImGuiInputFlags flags, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiKeyChord, ImGuiInputFlags, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShortcu"
                                                                                         "t_ID"));
        return func(key_chord, flags, owner_id);
    }

    inline bool SetShortcutRouting(ImGuiKeyChord key_chord, ImGuiInputFlags flags, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiKeyChord, ImGuiInputFlags, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetShor"
                                                                                         "tcutRouti"
                                                                                         "ng"));
        return func(key_chord, flags, owner_id);
    }

    inline bool TestShortcutRouting(ImGuiKeyChord key_chord, ImGuiID owner_id) {
        using func_t            = bool (*)(ImGuiKeyChord, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTestSho"
                                                                                         "rtcutRout"
                                                                                         "ing"));
        return func(key_chord, owner_id);
    }

    inline ImGuiKeyRoutingData* GetShortcutRoutingData(ImGuiKeyChord key_chord) {
        using func_t           = ImGuiKeyRoutingData* (*)(ImGuiKeyChord);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetShort"
                                                                                        "cutRouting"
                                                                                        "Data"));
        return func(key_chord);
    }

    inline void DockContextInitialize(ImGuiContext* ctx) {
        using func_t            = void (*)(ImGuiContext*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textIniti"
                                                                                         "alize"));
        return func(ctx);
    }

    inline void DockContextShutdown(ImGuiContext* ctx) {
        using func_t            = void (*)(ImGuiContext*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textShutd"
                                                                                         "own"));
        return func(ctx);
    }

    inline void DockContextClearNodes(ImGuiContext* ctx, ImGuiID root_id, bool clear_settings_refs) {
        using func_t            = void (*)(ImGuiContext*, ImGuiID, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textClear"
                                                                                         "Nodes"));
        return func(ctx, root_id, clear_settings_refs);
    }

    inline void DockContextRebuildNodes(ImGuiContext* ctx) {
        using func_t            = void (*)(ImGuiContext*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textRebui"
                                                                                         "ldNode"
                                                                                         "s"));
        return func(ctx);
    }

    inline void DockContextNewFrameUpdateUndocking(ImGuiContext* ctx) {
        using func_t            = void (*)(ImGuiContext*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textNewFr"
                                                                                         "ameUpdate"
                                                                                         "Undockin"
                                                                                         "g"));
        return func(ctx);
    }

    inline void DockContextNewFrameUpdateDocking(ImGuiContext* ctx) {
        using func_t            = void (*)(ImGuiContext*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textNewFr"
                                                                                         "ameUpdate"
                                                                                         "Dockin"
                                                                                         "g"));
        return func(ctx);
    }

    inline void DockContextEndFrame(ImGuiContext* ctx) {
        using func_t            = void (*)(ImGuiContext*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textEndFr"
                                                                                         "ame"));
        return func(ctx);
    }

    inline ImGuiID DockContextGenNodeID(ImGuiContext* ctx) {
        using func_t           = ImGuiID (*)(ImGuiContext*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCont"
                                                                                        "extGenNode"
                                                                                        "ID"));
        return func(ctx);
    }

    inline void DockContextQueueDock(ImGuiContext* ctx, ImGuiWindow* target, ImGuiDockNode* target_node, ImGuiWindow* payload, ImGuiDir split_dir, float split_ratio, bool split_outer) {
        using func_t = void (*)(ImGuiContext*, ImGuiWindow*, ImGuiDockNode*, ImGuiWindow*, ImGuiDir, float, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textQueue"
                                                                                         "Dock"));
        return func(ctx, target, target_node, payload, split_dir, split_ratio, split_outer);
    }

    inline void DockContextQueueUndockWindow(ImGuiContext* ctx, ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiContext*, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textQueue"
                                                                                         "UndockWin"
                                                                                         "dow"));
        return func(ctx, window);
    }

    inline void DockContextQueueUndockNode(ImGuiContext* ctx, ImGuiDockNode* node) {
        using func_t            = void (*)(ImGuiContext*, ImGuiDockNode*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textQueue"
                                                                                         "UndockNod"
                                                                                         "e"));
        return func(ctx, node);
    }

    inline void DockContextProcessUndockWindow(ImGuiContext* ctx, ImGuiWindow* window, bool clear_persistent_docking_ref) {
        using func_t            = void (*)(ImGuiContext*, ImGuiWindow*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textProce"
                                                                                         "ssUndockW"
                                                                                         "indow"));
        return func(ctx, window, clear_persistent_docking_ref);
    }

    inline void DockContextProcessUndockNode(ImGuiContext* ctx, ImGuiDockNode* node) {
        using func_t            = void (*)(ImGuiContext*, ImGuiDockNode*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textProce"
                                                                                         "ssUndockN"
                                                                                         "ode"));
        return func(ctx, node);
    }

    inline bool DockContextCalcDropPosForDocking(ImGuiWindow* target, ImGuiDockNode* target_node, ImGuiWindow* payload_window, ImGuiDockNode* payload_node, ImGuiDir split_dir, bool split_outer, ImVec2* out_pos) {
        using func_t = bool (*)(ImGuiWindow*, ImGuiDockNode*, ImGuiWindow*, ImGuiDockNode*, ImGuiDir, bool, ImVec2*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCon"
                                                                                         "textCalcD"
                                                                                         "ropPosFor"
                                                                                         "Dockin"
                                                                                         "g"));
        return func(target, target_node, payload_window, payload_node, split_dir, split_outer, out_pos);
    }

    inline ImGuiDockNode* DockContextFindNodeByID(ImGuiContext* ctx, ImGuiID id) {
        using func_t           = ImGuiDockNode* (*)(ImGuiContext*, ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockCont"
                                                                                        "extFindNod"
                                                                                        "eByID"));
        return func(ctx, id);
    }

    inline void DockNodeWindowMenuHandler(ImGuiContext* ctx, ImGuiDockNode* node, ImGuiTabBar* tab_bar) {
        using func_t            = void (*)(ImGuiContext*, ImGuiDockNode*, ImGuiTabBar*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockNod"
                                                                                         "eWindowMe"
                                                                                         "nuHandler"
                                                                                         "_Defaul"
                                                                                         "t"));
        return func(ctx, node, tab_bar);
    }

    inline bool DockNodeBeginAmendTabBar(ImGuiDockNode* node) {
        using func_t            = bool (*)(ImGuiDockNode*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockNod"
                                                                                         "eBeginAme"
                                                                                         "ndTabBa"
                                                                                         "r"));
        return func(node);
    }

    inline void DockNodeEndAmendTabBar() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockNod"
                                                                                         "eEndAmend"
                                                                                         "TabBar"));
        return func();
    }

    inline ImGuiDockNode* DockNodeGetRootNode(ImGuiDockNode* node) {
        using func_t           = ImGuiDockNode* (*)(ImGuiDockNode*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockNode"
                                                                                        "GetRootNod"
                                                                                        "e"));
        return func(node);
    }

    inline bool DockNodeIsInHierarchyOf(ImGuiDockNode* node, ImGuiDockNode* parent) {
        using func_t            = bool (*)(ImGuiDockNode*, ImGuiDockNode*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockNod"
                                                                                         "eIsInHier"
                                                                                         "archyO"
                                                                                         "f"));
        return func(node, parent);
    }

    inline int DockNodeGetDepth(const ImGuiDockNode* node) {
        using func_t           = int (*)(const ImGuiDockNode*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockNode"
                                                                                        "GetDept"
                                                                                        "h"));
        return func(node);
    }

    inline ImGuiID DockNodeGetWindowMenuButtonId(const ImGuiDockNode* node) {
        using func_t           = ImGuiID (*)(const ImGuiDockNode*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockNode"
                                                                                        "GetWindowM"
                                                                                        "enuButtonI"
                                                                                        "d"));
        return func(node);
    }

    inline ImGuiDockNode* GetWindowDockNode() {
        using func_t           = ImGuiDockNode* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWindo"
                                                                                        "wDockNod"
                                                                                        "e"));
        return func();
    }

    inline bool GetWindowAlwaysWantOwnTabBar(ImGuiWindow* window) {
        using func_t            = bool (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWind"
                                                                                         "owAlwaysW"
                                                                                         "antOwnTab"
                                                                                         "Bar"));
        return func(window);
    }

    inline void BeginDocked(ImGuiWindow* window, bool* p_open) {
        using func_t            = void (*)(ImGuiWindow*, bool*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginDo"
                                                                                         "cked"));
        return func(window, p_open);
    }

    inline void BeginDockableDragDropSource(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginDo"
                                                                                         "ckableDra"
                                                                                         "gDropSour"
                                                                                         "ce"));
        return func(window);
    }

    inline void BeginDockableDragDropTarget(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginDo"
                                                                                         "ckableDra"
                                                                                         "gDropTarg"
                                                                                         "et"));
        return func(window);
    }

    inline void SetWindowDock(ImGuiWindow* window, ImGuiID dock_id, ImGuiCond cond) {
        using func_t            = void (*)(ImGuiWindow*, ImGuiID, ImGuiCond);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owDock"));
        return func(window, dock_id, cond);
    }

    inline void DockBuilderDockWindow(const char* window_name, ImGuiID node_id) {
        using func_t            = void (*)(const char*, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBui"
                                                                                         "lderDockW"
                                                                                         "indow"));
        return func(window_name, node_id);
    }

    inline ImGuiDockNode* DockBuilderGetNode(ImGuiID node_id) {
        using func_t           = ImGuiDockNode* (*)(ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBuil"
                                                                                        "derGetNod"
                                                                                        "e"));
        return func(node_id);
    }

    inline ImGuiDockNode* DockBuilderGetCentralNode(ImGuiID node_id) {
        using func_t           = ImGuiDockNode* (*)(ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBuil"
                                                                                        "derGetCent"
                                                                                        "ralNode"));
        return func(node_id);
    }

    inline ImGuiID DockBuilderAddNode(ImGuiID node_id, ImGuiDockNodeFlags flags) {
        using func_t           = ImGuiID (*)(ImGuiID, ImGuiDockNodeFlags);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBuil"
                                                                                        "derAddNod"
                                                                                        "e"));
        return func(node_id, flags);
    }

    inline void DockBuilderRemoveNode(ImGuiID node_id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBui"
                                                                                         "lderRemov"
                                                                                         "eNode"));
        return func(node_id);
    }

    inline void DockBuilderRemoveNodeDockedWindows(ImGuiID node_id, bool clear_settings_refs) {
        using func_t            = void (*)(ImGuiID, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBui"
                                                                                         "lderRemov"
                                                                                         "eNodeDock"
                                                                                         "edWindow"
                                                                                         "s"));
        return func(node_id, clear_settings_refs);
    }

    inline void DockBuilderRemoveNodeChildNodes(ImGuiID node_id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBui"
                                                                                         "lderRemov"
                                                                                         "eNodeChil"
                                                                                         "dNodes"));
        return func(node_id);
    }

    inline void DockBuilderSetNodePos(ImGuiID node_id, ImVec2 pos) {
        using func_t            = void (*)(ImGuiID, ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBui"
                                                                                         "lderSetNo"
                                                                                         "dePos"));
        return func(node_id, pos);
    }

    inline void DockBuilderSetNodeSize(ImGuiID node_id, ImVec2 size) {
        using func_t            = void (*)(ImGuiID, ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBui"
                                                                                         "lderSetNo"
                                                                                         "deSize"));
        return func(node_id, size);
    }

    inline ImGuiID DockBuilderSplitNode(ImGuiID node_id, ImGuiDir split_dir, float size_ratio_for_node_at_dir, ImGuiID* out_id_at_dir, ImGuiID* out_id_at_opposite_dir) {
        using func_t           = ImGuiID (*)(ImGuiID, ImGuiDir, float, ImGuiID*, ImGuiID*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBuil"
                                                                                        "derSplitNo"
                                                                                        "de"));
        return func(node_id, split_dir, size_ratio_for_node_at_dir, out_id_at_dir, out_id_at_opposite_dir);
    }

    inline void DockBuilderCopyDockSpace(ImGuiID src_dockspace_id, ImGuiID dst_dockspace_id, ImVector_const_charPtr* in_window_remap_pairs) {
        using func_t            = void (*)(ImGuiID, ImGuiID, ImVector_const_charPtr*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBui"
                                                                                         "lderCopyD"
                                                                                         "ockSpac"
                                                                                         "e"));
        return func(src_dockspace_id, dst_dockspace_id, in_window_remap_pairs);
    }

    inline void DockBuilderCopyNode(ImGuiID src_node_id, ImGuiID dst_node_id, ImVector_ImGuiID* out_node_remap_pairs) {
        using func_t            = void (*)(ImGuiID, ImGuiID, ImVector_ImGuiID*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBui"
                                                                                         "lderCopyN"
                                                                                         "ode"));
        return func(src_node_id, dst_node_id, out_node_remap_pairs);
    }

    inline void DockBuilderCopyWindowSettings(const char* src_name, const char* dst_name) {
        using func_t            = void (*)(const char*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBui"
                                                                                         "lderCopyW"
                                                                                         "indowSett"
                                                                                         "ings"));
        return func(src_name, dst_name);
    }

    inline void DockBuilderFinish(ImGuiID node_id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDockBui"
                                                                                         "lderFinis"
                                                                                         "h"));
        return func(node_id);
    }

    inline void PushFocusScope(ImGuiID id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushFoc"
                                                                                         "usScop"
                                                                                         "e"));
        return func(id);
    }

    inline void PopFocusScope() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopFocu"
                                                                                         "sScope"));
        return func();
    }

    inline ImGuiID GetCurrentFocusScope() {
        using func_t           = ImGuiID (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurre"
                                                                                        "ntFocusSco"
                                                                                        "pe"));
        return func();
    }

    inline bool IsDragDropActive() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsDragD"
                                                                                         "ropActiv"
                                                                                         "e"));
        return func();
    }

    inline bool BeginDragDropTargetCustom(const ImRect bb, ImGuiID id) {
        using func_t            = bool (*)(const ImRect, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginDr"
                                                                                         "agDropTar"
                                                                                         "getCusto"
                                                                                         "m"));
        return func(bb, id);
    }

    inline void ClearDragDrop() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igClearDr"
                                                                                         "agDrop"));
        return func();
    }

    inline bool IsDragDropPayloadBeingAccepted() {
        using func_t            = bool (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igIsDragD"
                                                                                         "ropPayloa"
                                                                                         "dBeingAcc"
                                                                                         "epted"));
        return func();
    }

    inline void RenderDragDropTargetRect(const ImRect bb, const ImRect item_clip_rect) {
        using func_t            = void (*)(const ImRect, const ImRect);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderD"
                                                                                         "ragDropTa"
                                                                                         "rgetRec"
                                                                                         "t"));
        return func(bb, item_clip_rect);
    }

    inline ImGuiTypingSelectRequest* GetTypingSelectRequest(ImGuiTypingSelectFlags flags) {
        using func_t           = ImGuiTypingSelectRequest* (*)(ImGuiTypingSelectFlags);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetTypin"
                                                                                        "gSelectReq"
                                                                                        "uest"));
        return func(flags);
    }

    inline int TypingSelectFindMatch(ImGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data, int nav_item_idx) {
        using func_t = int (*)(ImGuiTypingSelectRequest*, int, const char* (*)(void*, int), void*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTypingSe"
                                                                                        "lectFindMa"
                                                                                        "tch"));
        return func(req, items_count, get_item_name_func, user_data, nav_item_idx);
    }

    inline int TypingSelectFindNextSingleCharMatch(ImGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data, int nav_item_idx) {
        using func_t = int (*)(ImGuiTypingSelectRequest*, int, const char* (*)(void*, int), void*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTypingSe"
                                                                                        "lectFindNe"
                                                                                        "xtSingleCh"
                                                                                        "arMatch"));
        return func(req, items_count, get_item_name_func, user_data, nav_item_idx);
    }

    inline int TypingSelectFindBestLeadingMatch(ImGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data) {
        using func_t = int (*)(ImGuiTypingSelectRequest*, int, const char* (*)(void*, int), void*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTypingSe"
                                                                                        "lectFindBe"
                                                                                        "stLeadingM"
                                                                                        "atch"));
        return func(req, items_count, get_item_name_func, user_data);
    }

    inline void SetWindowClipRectBeforeSetChannel(ImGuiWindow* window, const ImRect clip_rect) {
        using func_t            = void (*)(ImGuiWindow*, const ImRect);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetWind"
                                                                                         "owClipRec"
                                                                                         "tBeforeSe"
                                                                                         "tChanne"
                                                                                         "l"));
        return func(window, clip_rect);
    }

    inline void BeginColumns(const char* str_id, int count, ImGuiOldColumnFlags flags) {
        using func_t            = void (*)(const char*, int, ImGuiOldColumnFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginCo"
                                                                                         "lumns"));
        return func(str_id, count, flags);
    }

    inline void EndColumns() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igEndColu"
                                                                                         "mns"));
        return func();
    }

    inline void PushColumnClipRect(int column_index) {
        using func_t            = void (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushCol"
                                                                                         "umnClipRe"
                                                                                         "ct"));
        return func(column_index);
    }

    inline void PushColumnsBackground() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPushCol"
                                                                                         "umnsBackg"
                                                                                         "round"));
        return func();
    }

    inline void PopColumnsBackground() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPopColu"
                                                                                         "mnsBackgr"
                                                                                         "ound"));
        return func();
    }

    inline ImGuiID GetColumnsID(const char* str_id, int count) {
        using func_t           = ImGuiID (*)(const char*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetColum"
                                                                                        "nsID"));
        return func(str_id, count);
    }

    inline ImGuiOldColumns* FindOrCreateColumns(ImGuiWindow* window, ImGuiID id) {
        using func_t           = ImGuiOldColumns* (*)(ImGuiWindow*, ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindOrCr"
                                                                                        "eateColumn"
                                                                                        "s"));
        return func(window, id);
    }

    inline float GetColumnOffsetFromNorm(const ImGuiOldColumns* columns, float offset_norm) {
        using func_t           = float (*)(const ImGuiOldColumns*, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetColum"
                                                                                        "nOffsetFro"
                                                                                        "mNorm"));
        return func(columns, offset_norm);
    }

    inline float GetColumnNormFromOffset(const ImGuiOldColumns* columns, float offset) {
        using func_t           = float (*)(const ImGuiOldColumns*, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetColum"
                                                                                        "nNormFromO"
                                                                                        "ffset"));
        return func(columns, offset);
    }

    inline void TableOpenContextMenu(int column_n) {
        using func_t            = void (*)(int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableOp"
                                                                                         "enContext"
                                                                                         "Menu"));
        return func(column_n);
    }

    inline void TableSetColumnWidth(int column_n, float width) {
        using func_t            = void (*)(int, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "tColumnWi"
                                                                                         "dth"));
        return func(column_n, width);
    }

    inline void TableSetColumnSortDirection(int column_n, ImGuiSortDirection sort_direction, bool append_to_sort_specs) {
        using func_t            = void (*)(int, ImGuiSortDirection, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "tColumnSo"
                                                                                         "rtDirecti"
                                                                                         "on"));
        return func(column_n, sort_direction, append_to_sort_specs);
    }

    inline int TableGetHoveredColumn() {
        using func_t           = int (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "HoveredCol"
                                                                                        "umn"));
        return func();
    }

    inline int TableGetHoveredRow() {
        using func_t           = int (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "HoveredRo"
                                                                                        "w"));
        return func();
    }

    inline float TableGetHeaderRowHeight() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "HeaderRowH"
                                                                                        "eight"));
        return func();
    }

    inline float TableGetHeaderAngledMaxLabelWidth() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "HeaderAngl"
                                                                                        "edMaxLabel"
                                                                                        "Width"));
        return func();
    }

    inline void TablePushBackgroundChannel() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTablePu"
                                                                                         "shBackgro"
                                                                                         "undChanne"
                                                                                         "l"));
        return func();
    }

    inline void TablePopBackgroundChannel() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTablePo"
                                                                                         "pBackgrou"
                                                                                         "ndChanne"
                                                                                         "l"));
        return func();
    }

    inline void TableAngledHeadersRowEx(ImGuiID row_id, float angle, float max_label_width, const ImGuiTableHeaderData* data, int data_count) {
        using func_t            = void (*)(ImGuiID, float, float, const ImGuiTableHeaderData*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableAn"
                                                                                         "gledHeade"
                                                                                         "rsRowE"
                                                                                         "x"));
        return func(row_id, angle, max_label_width, data, data_count);
    }

    inline ImGuiTable* GetCurrentTable() {
        using func_t           = ImGuiTable* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurre"
                                                                                        "ntTable"));
        return func();
    }

    inline ImGuiTable* TableFindByID(ImGuiID id) {
        using func_t           = ImGuiTable* (*)(ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableFin"
                                                                                        "dByID"));
        return func(id);
    }

    inline bool BeginTableEx(const char* name, ImGuiID id, int columns_count, ImGuiTableFlags flags, const ImVec2 outer_size, float inner_width) {
        using func_t = bool (*)(const char*, ImGuiID, int, ImGuiTableFlags, const ImVec2, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginTa"
                                                                                         "bleEx"));
        return func(name, id, columns_count, flags, outer_size, inner_width);
    }

    inline void TableBeginInitMemory(ImGuiTable* table, int columns_count) {
        using func_t            = void (*)(ImGuiTable*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableBe"
                                                                                         "ginInitMe"
                                                                                         "mory"));
        return func(table, columns_count);
    }

    inline void TableBeginApplyRequests(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableBe"
                                                                                         "ginApplyR"
                                                                                         "equest"
                                                                                         "s"));
        return func(table);
    }

    inline void TableSetupDrawChannels(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "tupDrawCh"
                                                                                         "annels"));
        return func(table);
    }

    inline void TableUpdateLayout(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableUp"
                                                                                         "dateLayou"
                                                                                         "t"));
        return func(table);
    }

    inline void TableUpdateBorders(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableUp"
                                                                                         "dateBorde"
                                                                                         "rs"));
        return func(table);
    }

    inline void TableUpdateColumnsWeightFromWidth(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableUp"
                                                                                         "dateColum"
                                                                                         "nsWeightF"
                                                                                         "romWidt"
                                                                                         "h"));
        return func(table);
    }

    inline void TableDrawBorders(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableDr"
                                                                                         "awBorder"
                                                                                         "s"));
        return func(table);
    }

    inline void TableDrawDefaultContextMenu(ImGuiTable* table, ImGuiTableFlags flags_for_section_to_display) {
        using func_t            = void (*)(ImGuiTable*, ImGuiTableFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableDr"
                                                                                         "awDefault"
                                                                                         "ContextMe"
                                                                                         "nu"));
        return func(table, flags_for_section_to_display);
    }

    inline bool TableBeginContextMenuPopup(ImGuiTable* table) {
        using func_t            = bool (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableBe"
                                                                                         "ginContex"
                                                                                         "tMenuPopu"
                                                                                         "p"));
        return func(table);
    }

    inline void TableMergeDrawChannels(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableMe"
                                                                                         "rgeDrawCh"
                                                                                         "annels"));
        return func(table);
    }

    inline ImGuiTableInstanceData* TableGetInstanceData(ImGuiTable* table, int instance_no) {
        using func_t           = ImGuiTableInstanceData* (*)(ImGuiTable*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "InstanceDa"
                                                                                        "ta"));
        return func(table, instance_no);
    }

    inline ImGuiID TableGetInstanceID(ImGuiTable* table, int instance_no) {
        using func_t           = ImGuiID (*)(ImGuiTable*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "InstanceI"
                                                                                        "D"));
        return func(table, instance_no);
    }

    inline void TableSortSpecsSanitize(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSo"
                                                                                         "rtSpecsSa"
                                                                                         "nitize"));
        return func(table);
    }

    inline void TableSortSpecsBuild(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSo"
                                                                                         "rtSpecsBu"
                                                                                         "ild"));
        return func(table);
    }

    inline ImGuiSortDirection TableGetColumnNextSortDirection(ImGuiTableColumn* column) {
        using func_t           = ImGuiSortDirection (*)(ImGuiTableColumn*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "ColumnNext"
                                                                                        "SortDirect"
                                                                                        "ion"));
        return func(column);
    }

    inline void TableFixColumnSortDirection(ImGuiTable* table, ImGuiTableColumn* column) {
        using func_t            = void (*)(ImGuiTable*, ImGuiTableColumn*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableFi"
                                                                                         "xColumnSo"
                                                                                         "rtDirecti"
                                                                                         "on"));
        return func(table, column);
    }

    inline float TableGetColumnWidthAuto(ImGuiTable* table, ImGuiTableColumn* column) {
        using func_t           = float (*)(ImGuiTable*, ImGuiTableColumn*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "ColumnWidt"
                                                                                        "hAuto"));
        return func(table, column);
    }

    inline void TableBeginRow(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableBe"
                                                                                         "ginRow"));
        return func(table);
    }

    inline void TableEndRow(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableEn"
                                                                                         "dRow"));
        return func(table);
    }

    inline void TableBeginCell(ImGuiTable* table, int column_n) {
        using func_t            = void (*)(ImGuiTable*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableBe"
                                                                                         "ginCel"
                                                                                         "l"));
        return func(table, column_n);
    }

    inline void TableEndCell(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableEn"
                                                                                         "dCell"));
        return func(table);
    }

    inline void TableGetCellBgRect(ImRect* pOut, const ImGuiTable* table, int column_n) {
        using func_t            = void (*)(ImRect*, const ImGuiTable*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGe"
                                                                                         "tCellBgRe"
                                                                                         "ct"));
        return func(pOut, table, column_n);
    }

    inline const char* TableGetColumnName(const ImGuiTable* table, int column_n) {
        using func_t           = const char* (*)(const ImGuiTable*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "ColumnName"
                                                                                        "_TablePt"
                                                                                        "r"));
        return func(table, column_n);
    }

    inline ImGuiID TableGetColumnResizeID(ImGuiTable* table, int column_n, int instance_no) {
        using func_t           = ImGuiID (*)(ImGuiTable*, int, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "ColumnResi"
                                                                                        "zeID"));
        return func(table, column_n, instance_no);
    }

    inline float TableGetMaxColumnWidth(const ImGuiTable* table, int column_n) {
        using func_t           = float (*)(const ImGuiTable*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "MaxColumnW"
                                                                                        "idth"));
        return func(table, column_n);
    }

    inline void TableSetColumnWidthAutoSingle(ImGuiTable* table, int column_n) {
        using func_t            = void (*)(ImGuiTable*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "tColumnWi"
                                                                                         "dthAutoSi"
                                                                                         "ngle"));
        return func(table, column_n);
    }

    inline void TableSetColumnWidthAutoAll(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "tColumnWi"
                                                                                         "dthAutoAl"
                                                                                         "l"));
        return func(table);
    }

    inline void TableRemove(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableRe"
                                                                                         "move"));
        return func(table);
    }

    inline void TableGcCompactTransientBuffers(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGc"
                                                                                         "CompactTr"
                                                                                         "ansientBu"
                                                                                         "ffers_"
                                                                                         "TablePt"
                                                                                         "r"));
        return func(table);
    }

    inline void TableGcCompactTransientBuffers(ImGuiTableTempData* table) {
        using func_t            = void (*)(ImGuiTableTempData*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGc"
                                                                                         "CompactTr"
                                                                                         "ansientBu"
                                                                                         "ffers_"
                                                                                         "TableTemp"
                                                                                         "DataPt"
                                                                                         "r"));
        return func(table);
    }

    inline void TableGcCompactSettings() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGc"
                                                                                         "CompactSe"
                                                                                         "ttings"));
        return func();
    }

    inline void TableLoadSettings(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableLo"
                                                                                         "adSetting"
                                                                                         "s"));
        return func(table);
    }

    inline void TableSaveSettings(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSa"
                                                                                         "veSetting"
                                                                                         "s"));
        return func(table);
    }

    inline void TableResetSettings(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableRe"
                                                                                         "setSettin"
                                                                                         "gs"));
        return func(table);
    }

    inline ImGuiTableSettings* TableGetBoundSettings(ImGuiTable* table) {
        using func_t           = ImGuiTableSettings* (*)(ImGuiTable*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableGet"
                                                                                        "BoundSetti"
                                                                                        "ngs"));
        return func(table);
    }

    inline void TableSettingsAddSettingsHandler() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSe"
                                                                                         "ttingsAdd"
                                                                                         "SettingsH"
                                                                                         "andler"));
        return func();
    }

    inline ImGuiTableSettings* TableSettingsCreate(ImGuiID id, int columns_count) {
        using func_t           = ImGuiTableSettings* (*)(ImGuiID, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSet"
                                                                                        "tingsCreat"
                                                                                        "e"));
        return func(id, columns_count);
    }

    inline ImGuiTableSettings* TableSettingsFindByID(ImGuiID id) {
        using func_t           = ImGuiTableSettings* (*)(ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTableSet"
                                                                                        "tingsFindB"
                                                                                        "yID"));
        return func(id);
    }

    inline ImGuiTabBar* GetCurrentTabBar() {
        using func_t           = ImGuiTabBar* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetCurre"
                                                                                        "ntTabBa"
                                                                                        "r"));
        return func();
    }

    inline bool BeginTabBarEx(ImGuiTabBar* tab_bar, const ImRect bb, ImGuiTabBarFlags flags) {
        using func_t            = bool (*)(ImGuiTabBar*, const ImRect, ImGuiTabBarFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igBeginTa"
                                                                                         "bBarEx"));
        return func(tab_bar, bb, flags);
    }

    inline ImGuiTabItem* TabBarFindTabByID(ImGuiTabBar* tab_bar, ImGuiID tab_id) {
        using func_t           = ImGuiTabItem* (*)(ImGuiTabBar*, ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarFi"
                                                                                        "ndTabByI"
                                                                                        "D"));
        return func(tab_bar, tab_id);
    }

    inline ImGuiTabItem* TabBarFindTabByOrder(ImGuiTabBar* tab_bar, int order) {
        using func_t           = ImGuiTabItem* (*)(ImGuiTabBar*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarFi"
                                                                                        "ndTabByOrd"
                                                                                        "er"));
        return func(tab_bar, order);
    }

    inline ImGuiTabItem* TabBarFindMostRecentlySelectedTabForActiveWindow(ImGuiTabBar* tab_bar) {
        using func_t           = ImGuiTabItem* (*)(ImGuiTabBar*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarFi"
                                                                                        "ndMostRece"
                                                                                        "ntlySelect"
                                                                                        "edTabForAc"
                                                                                        "tiveWindo"
                                                                                        "w"));
        return func(tab_bar);
    }

    inline ImGuiTabItem* TabBarGetCurrentTab(ImGuiTabBar* tab_bar) {
        using func_t           = ImGuiTabItem* (*)(ImGuiTabBar*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarGe"
                                                                                        "tCurrentTa"
                                                                                        "b"));
        return func(tab_bar);
    }

    inline int TabBarGetTabOrder(ImGuiTabBar* tab_bar, ImGuiTabItem* tab) {
        using func_t           = int (*)(ImGuiTabBar*, ImGuiTabItem*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarGe"
                                                                                        "tTabOrde"
                                                                                        "r"));
        return func(tab_bar, tab);
    }

    inline const char* TabBarGetTabName(ImGuiTabBar* tab_bar, ImGuiTabItem* tab) {
        using func_t           = const char* (*)(ImGuiTabBar*, ImGuiTabItem*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarGe"
                                                                                        "tTabNam"
                                                                                        "e"));
        return func(tab_bar, tab);
    }

    inline void TabBarAddTab(ImGuiTabBar* tab_bar, ImGuiTabItemFlags tab_flags, ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiTabBar*, ImGuiTabItemFlags, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarA"
                                                                                         "ddTab"));
        return func(tab_bar, tab_flags, window);
    }

    inline void TabBarRemoveTab(ImGuiTabBar* tab_bar, ImGuiID tab_id) {
        using func_t            = void (*)(ImGuiTabBar*, ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarR"
                                                                                         "emoveTa"
                                                                                         "b"));
        return func(tab_bar, tab_id);
    }

    inline void TabBarCloseTab(ImGuiTabBar* tab_bar, ImGuiTabItem* tab) {
        using func_t            = void (*)(ImGuiTabBar*, ImGuiTabItem*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarC"
                                                                                         "loseTa"
                                                                                         "b"));
        return func(tab_bar, tab);
    }

    inline void TabBarQueueFocus(ImGuiTabBar* tab_bar, ImGuiTabItem* tab) {
        using func_t            = void (*)(ImGuiTabBar*, ImGuiTabItem*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarQ"
                                                                                         "ueueFocu"
                                                                                         "s"));
        return func(tab_bar, tab);
    }

    inline void TabBarQueueReorder(ImGuiTabBar* tab_bar, ImGuiTabItem* tab, int offset) {
        using func_t            = void (*)(ImGuiTabBar*, ImGuiTabItem*, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarQ"
                                                                                         "ueueReord"
                                                                                         "er"));
        return func(tab_bar, tab, offset);
    }

    inline void TabBarQueueReorderFromMousePos(ImGuiTabBar* tab_bar, ImGuiTabItem* tab, ImVec2 mouse_pos) {
        using func_t            = void (*)(ImGuiTabBar*, ImGuiTabItem*, ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarQ"
                                                                                         "ueueReord"
                                                                                         "erFromMou"
                                                                                         "sePos"));
        return func(tab_bar, tab, mouse_pos);
    }

    inline bool TabBarProcessReorder(ImGuiTabBar* tab_bar) {
        using func_t            = bool (*)(ImGuiTabBar*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabBarP"
                                                                                         "rocessReo"
                                                                                         "rder"));
        return func(tab_bar);
    }

    inline bool TabItemEx(ImGuiTabBar* tab_bar, const char* label, bool* p_open, ImGuiTabItemFlags flags, ImGuiWindow* docked_window) {
        using func_t = bool (*)(ImGuiTabBar*, const char*, bool*, ImGuiTabItemFlags, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabItem"
                                                                                         "Ex"));
        return func(tab_bar, label, p_open, flags, docked_window);
    }

    inline void TabItemCalcSize(ImVec2* pOut, const char* label, bool has_close_button_or_unsaved_marker) {
        using func_t            = void (*)(ImVec2*, const char*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabItem"
                                                                                         "CalcSize_"
                                                                                         "Str"));
        return func(pOut, label, has_close_button_or_unsaved_marker);
    }

    inline void TabItemCalcSize(ImVec2* pOut, ImGuiWindow* window) {
        using func_t            = void (*)(ImVec2*, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabItem"
                                                                                         "CalcSize_"
                                                                                         "WindowPt"
                                                                                         "r"));
        return func(pOut, window);
    }

    inline void TabItemBackground(ImDrawList* draw_list, const ImRect bb, ImGuiTabItemFlags flags, ImU32 col) {
        using func_t            = void (*)(ImDrawList*, const ImRect, ImGuiTabItemFlags, ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabItem"
                                                                                         "Backgroun"
                                                                                         "d"));
        return func(draw_list, bb, flags, col);
    }

    inline void TabItemLabelAndCloseButton(ImDrawList* draw_list, const ImRect bb, ImGuiTabItemFlags flags, ImVec2 frame_padding, const char* label, ImGuiID tab_id, ImGuiID close_button_id, bool is_contents_visible, bool* out_just_closed, bool* out_text_clipped) {
        using func_t = void (*)(ImDrawList*, const ImRect, ImGuiTabItemFlags, ImVec2, const char*, ImGuiID, ImGuiID, bool, bool*, bool*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTabItem"
                                                                                         "LabelAndC"
                                                                                         "loseButto"
                                                                                         "n"));
        return func(draw_list, bb, flags, frame_padding, label, tab_id, close_button_id, is_contents_visible, out_just_closed, out_text_clipped);
    }

    inline void RenderText(ImVec2 pos, const char* text, const char* text_end, bool hide_text_after_hash) {
        using func_t            = void (*)(ImVec2, const char*, const char*, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderT"
                                                                                         "ext"));
        return func(pos, text, text_end, hide_text_after_hash);
    }

    inline void RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width) {
        using func_t            = void (*)(ImVec2, const char*, const char*, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderT"
                                                                                         "extWrappe"
                                                                                         "d"));
        return func(pos, text, text_end, wrap_width);
    }

    inline void RenderTextClipped(const ImVec2 pos_min, const ImVec2 pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2 align, const ImRect* clip_rect) {
        using func_t = void (*)(const ImVec2, const ImVec2, const char*, const char*, const ImVec2*, const ImVec2, const ImRect*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderTe"
                                                                                        "xtClippe"
                                                                                        "d"));
        return func(pos_min, pos_max, text, text_end, text_size_if_known, align, clip_rect);
    }

    inline void RenderTextClippedEx(ImDrawList* draw_list, const ImVec2 pos_min, const ImVec2 pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2 align, const ImRect* clip_rect) {
        using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, const char*, const char*, const ImVec2*, const ImVec2, const ImRect*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderT"
                                                                                         "extClippe"
                                                                                         "dEx"));
        return func(draw_list, pos_min, pos_max, text, text_end, text_size_if_known, align, clip_rect);
    }

    inline void RenderTextEllipsis(ImDrawList* draw_list, const ImVec2 pos_min, const ImVec2 pos_max, float clip_max_x, float ellipsis_max_x, const char* text, const char* text_end, const ImVec2* text_size_if_known) {
        using func_t = void (*)(ImDrawList*, const ImVec2, const ImVec2, float, float, const char*, const char*, const ImVec2*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderTe"
                                                                                        "xtEllipsi"
                                                                                        "s"));
        return func(draw_list, pos_min, pos_max, clip_max_x, ellipsis_max_x, text, text_end, text_size_if_known);
    }

    inline void RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding) {
        using func_t            = void (*)(ImVec2, ImVec2, ImU32, bool, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderF"
                                                                                         "rame"));
        return func(p_min, p_max, fill_col, border, rounding);
    }

    inline void RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding) {
        using func_t            = void (*)(ImVec2, ImVec2, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderF"
                                                                                         "rameBorde"
                                                                                         "r"));
        return func(p_min, p_max, rounding);
    }

    inline void RenderColorRectWithAlphaCheckerboard(ImDrawList* draw_list, ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float grid_step, ImVec2 grid_off, float rounding, ImDrawFlags flags) {
        using func_t = void (*)(ImDrawList*, ImVec2, ImVec2, ImU32, float, ImVec2, float, ImDrawFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderC"
                                                                                         "olorRectW"
                                                                                         "ithAlphaC"
                                                                                         "heckerboa"
                                                                                         "rd"));
        return func(draw_list, p_min, p_max, fill_col, grid_step, grid_off, rounding, flags);
    }

    inline void RenderNavHighlight(const ImRect bb, ImGuiID id, ImGuiNavHighlightFlags flags) {
        using func_t            = void (*)(const ImRect, ImGuiID, ImGuiNavHighlightFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderN"
                                                                                         "avHighlig"
                                                                                         "ht"));
        return func(bb, id, flags);
    }

    inline const char* FindRenderedTextEnd(const char* text, const char* text_end) {
        using func_t           = const char* (*)(const char*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igFindRend"
                                                                                        "eredTextEn"
                                                                                        "d"));
        return func(text, text_end);
    }

    inline void RenderMouseCursor(ImVec2 pos, float scale, ImGuiMouseCursor mouse_cursor, ImU32 col_fill, ImU32 col_border, ImU32 col_shadow) {
        using func_t            = void (*)(ImVec2, float, ImGuiMouseCursor, ImU32, ImU32, ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderM"
                                                                                         "ouseCurso"
                                                                                         "r"));
        return func(pos, scale, mouse_cursor, col_fill, col_border, col_shadow);
    }

    inline void RenderArrow(ImDrawList* draw_list, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale) {
        using func_t            = void (*)(ImDrawList*, ImVec2, ImU32, ImGuiDir, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderA"
                                                                                         "rrow"));
        return func(draw_list, pos, col, dir, scale);
    }

    inline void RenderBullet(ImDrawList* draw_list, ImVec2 pos, ImU32 col) {
        using func_t            = void (*)(ImDrawList*, ImVec2, ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderB"
                                                                                         "ullet"));
        return func(draw_list, pos, col);
    }

    inline void RenderCheckMark(ImDrawList* draw_list, ImVec2 pos, ImU32 col, float sz) {
        using func_t            = void (*)(ImDrawList*, ImVec2, ImU32, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderC"
                                                                                         "heckMar"
                                                                                         "k"));
        return func(draw_list, pos, col, sz);
    }

    inline void RenderArrowPointingAt(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col) {
        using func_t            = void (*)(ImDrawList*, ImVec2, ImVec2, ImGuiDir, ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderA"
                                                                                         "rrowPoint"
                                                                                         "ingAt"));
        return func(draw_list, pos, half_sz, direction, col);
    }

    inline void RenderArrowDockMenu(ImDrawList* draw_list, ImVec2 p_min, float sz, ImU32 col) {
        using func_t            = void (*)(ImDrawList*, ImVec2, float, ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderA"
                                                                                         "rrowDockM"
                                                                                         "enu"));
        return func(draw_list, p_min, sz, col);
    }

    inline void RenderRectFilledRangeH(ImDrawList* draw_list, const ImRect rect, ImU32 col, float x_start_norm, float x_end_norm, float rounding) {
        using func_t            = void (*)(ImDrawList*, const ImRect, ImU32, float, float, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderR"
                                                                                         "ectFilled"
                                                                                         "RangeH"));
        return func(draw_list, rect, col, x_start_norm, x_end_norm, rounding);
    }

    inline void RenderRectFilledWithHole(ImDrawList* draw_list, const ImRect outer, const ImRect inner, ImU32 col, float rounding) {
        using func_t            = void (*)(ImDrawList*, const ImRect, const ImRect, ImU32, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igRenderR"
                                                                                         "ectFilled"
                                                                                         "WithHol"
                                                                                         "e"));
        return func(draw_list, outer, inner, col, rounding);
    }

    inline ImDrawFlags CalcRoundingFlagsForRectInRect(const ImRect r_in, const ImRect r_outer, float threshold) {
        using func_t           = ImDrawFlags (*)(const ImRect, const ImRect, float);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCalcRoun"
                                                                                        "dingFlagsF"
                                                                                        "orRectInRe"
                                                                                        "ct"));
        return func(r_in, r_outer, threshold);
    }

    inline void TextEx(const char* text, const char* text_end, ImGuiTextFlags flags) {
        using func_t            = void (*)(const char*, const char*, ImGuiTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTextE"
                                                                                         "x"));
        return func(text, text_end, flags);
    }

    inline bool ButtonEx(const char* label, const ImVec2 size_arg, ImGuiButtonFlags flags) {
        using func_t            = bool (*)(const char*, const ImVec2, ImGuiButtonFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igButtonE"
                                                                                         "x"));
        return func(label, size_arg, flags);
    }

    inline bool ArrowButtonEx(const char* str_id, ImGuiDir dir, ImVec2 size_arg, ImGuiButtonFlags flags) {
        using func_t            = bool (*)(const char*, ImGuiDir, ImVec2, ImGuiButtonFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igArrowBu"
                                                                                         "ttonEx"));
        return func(str_id, dir, size_arg, flags);
    }

    inline bool ImageButtonEx(ImGuiID id, ImTextureID texture_id, const ImVec2 image_size, const ImVec2 uv0, const ImVec2 uv1, const ImVec4 bg_col, const ImVec4 tint_col, ImGuiButtonFlags flags) {
        using func_t = bool (*)(ImGuiID, ImTextureID, const ImVec2, const ImVec2, const ImVec2, const ImVec4, const ImVec4, ImGuiButtonFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImageBu"
                                                                                         "ttonEx"));
        return func(id, texture_id, image_size, uv0, uv1, bg_col, tint_col, flags);
    }

    inline void SeparatorEx(ImGuiSeparatorFlags flags, float thickness) {
        using func_t            = void (*)(ImGuiSeparatorFlags, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSeparat"
                                                                                         "orEx"));
        return func(flags, thickness);
    }

    inline void SeparatorTextEx(ImGuiID id, const char* label, const char* label_end, float extra_width) {
        using func_t            = void (*)(ImGuiID, const char*, const char*, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSeparat"
                                                                                         "orTextE"
                                                                                         "x"));
        return func(id, label, label_end, extra_width);
    }

    inline bool CheckboxFlags(const char* label, ImS64* flags, ImS64 flags_value) {
        using func_t            = bool (*)(const char*, ImS64*, ImS64);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCheckbo"
                                                                                         "xFlags_"
                                                                                         "S64Ptr"));
        return func(label, flags, flags_value);
    }

    inline bool CheckboxFlags(const char* label, ImU64* flags, ImU64 flags_value) {
        using func_t            = bool (*)(const char*, ImU64*, ImU64);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCheckbo"
                                                                                         "xFlags_"
                                                                                         "U64Ptr"));
        return func(label, flags, flags_value);
    }

    inline bool CloseButton(ImGuiID id, const ImVec2 pos) {
        using func_t            = bool (*)(ImGuiID, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCloseBu"
                                                                                         "tton"));
        return func(id, pos);
    }

    inline bool CollapseButton(ImGuiID id, const ImVec2 pos, ImGuiDockNode* dock_node) {
        using func_t            = bool (*)(ImGuiID, const ImVec2, ImGuiDockNode*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igCollaps"
                                                                                         "eButto"
                                                                                         "n"));
        return func(id, pos, dock_node);
    }

    inline void Scrollbar(ImGuiAxis axis) {
        using func_t            = void (*)(ImGuiAxis);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igScrollb"
                                                                                         "ar"));
        return func(axis);
    }

    inline bool ScrollbarEx(const ImRect bb, ImGuiID id, ImGuiAxis axis, ImS64* p_scroll_v, ImS64 avail_v, ImS64 contents_v, ImDrawFlags flags) {
        using func_t = bool (*)(const ImRect, ImGuiID, ImGuiAxis, ImS64*, ImS64, ImS64, ImDrawFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igScrollb"
                                                                                         "arEx"));
        return func(bb, id, axis, p_scroll_v, avail_v, contents_v, flags);
    }

    inline void GetWindowScrollbarRect(ImRect* pOut, ImGuiWindow* window, ImGuiAxis axis) {
        using func_t            = void (*)(ImRect*, ImGuiWindow*, ImGuiAxis);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWind"
                                                                                         "owScrollb"
                                                                                         "arRect"));
        return func(pOut, window, axis);
    }

    inline ImGuiID GetWindowScrollbarID(ImGuiWindow* window, ImGuiAxis axis) {
        using func_t           = ImGuiID (*)(ImGuiWindow*, ImGuiAxis);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWindo"
                                                                                        "wScrollbar"
                                                                                        "ID"));
        return func(window, axis);
    }

    inline ImGuiID GetWindowResizeCornerID(ImGuiWindow* window, int n) {
        using func_t           = ImGuiID (*)(ImGuiWindow*, int);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWindo"
                                                                                        "wResizeCor"
                                                                                        "nerID"));
        return func(window, n);
    }

    inline ImGuiID GetWindowResizeBorderID(ImGuiWindow* window, ImGuiDir dir) {
        using func_t           = ImGuiID (*)(ImGuiWindow*, ImGuiDir);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetWindo"
                                                                                        "wResizeBor"
                                                                                        "derID"));
        return func(window, dir);
    }

    inline bool ButtonBehavior(const ImRect bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags) {
        using func_t            = bool (*)(const ImRect, ImGuiID, bool*, bool*, ImGuiButtonFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igButtonB"
                                                                                         "ehavio"
                                                                                         "r"));
        return func(bb, id, out_hovered, out_held, flags);
    }

    inline bool DragBehavior(ImGuiID id, ImGuiDataType data_type, void* p_v, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags) {
        using func_t = bool (*)(ImGuiID, ImGuiDataType, void*, float, const void*, const void*, const char*, ImGuiSliderFlags);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDragBeha"
                                                                                        "vior"));
        return func(id, data_type, p_v, v_speed, p_min, p_max, format, flags);
    }

    inline bool SliderBehavior(const ImRect bb, ImGuiID id, ImGuiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb) {
        using func_t = bool (*)(const ImRect, ImGuiID, ImGuiDataType, void*, const void*, const void*, const char*, ImGuiSliderFlags, ImRect*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSliderB"
                                                                                         "ehavio"
                                                                                         "r"));
        return func(bb, id, data_type, p_v, p_min, p_max, format, flags, out_grab_bb);
    }

    inline bool SplitterBehavior(const ImRect bb, ImGuiID id, ImGuiAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend, float hover_visibility_delay, ImU32 bg_col) {
        using func_t = bool (*)(const ImRect, ImGuiID, ImGuiAxis, float*, float*, float, float, float, float, ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSplitte"
                                                                                         "rBehavio"
                                                                                         "r"));
        return func(bb, id, axis, size1, size2, min_size1, min_size2, hover_extend, hover_visibility_delay, bg_col);
    }

    inline bool TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end) {
        using func_t            = bool (*)(ImGuiID, ImGuiTreeNodeFlags, const char*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eBehavio"
                                                                                         "r"));
        return func(id, flags, label, label_end);
    }

    inline void TreePushOverrideID(ImGuiID id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreePus"
                                                                                         "hOverride"
                                                                                         "ID"));
        return func(id);
    }

    inline void TreeNodeSetOpen(ImGuiID id, bool open) {
        using func_t            = void (*)(ImGuiID, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eSetOpe"
                                                                                         "n"));
        return func(id, open);
    }

    inline bool TreeNodeUpdateNextOpen(ImGuiID id, ImGuiTreeNodeFlags flags) {
        using func_t            = bool (*)(ImGuiID, ImGuiTreeNodeFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTreeNod"
                                                                                         "eUpdateNe"
                                                                                         "xtOpen"));
        return func(id, flags);
    }

    inline void SetNextItemSelectionUserData(ImGuiSelectionUserData selection_user_data) {
        using func_t            = void (*)(ImGuiSelectionUserData);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "ItemSelec"
                                                                                         "tionUserD"
                                                                                         "ata"));
        return func(selection_user_data);
    }

    inline const ImGuiDataTypeInfo* DataTypeGetInfo(ImGuiDataType data_type) {
        using func_t           = const ImGuiDataTypeInfo* (*)(ImGuiDataType);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDataType"
                                                                                        "GetInfo"));
        return func(data_type);
    }

    inline int DataTypeFormatString(char* buf, int buf_size, ImGuiDataType data_type, const void* p_data, const char* format) {
        using func_t           = int (*)(char*, int, ImGuiDataType, const void*, const char*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDataType"
                                                                                        "FormatStri"
                                                                                        "ng"));
        return func(buf, buf_size, data_type, p_data, format);
    }

    inline void DataTypeApplyOp(ImGuiDataType data_type, int op, void* output, const void* arg_1, const void* arg_2) {
        using func_t            = void (*)(ImGuiDataType, int, void*, const void*, const void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDataTyp"
                                                                                         "eApplyO"
                                                                                         "p"));
        return func(data_type, op, output, arg_1, arg_2);
    }

    inline bool DataTypeApplyFromText(const char* buf, ImGuiDataType data_type, void* p_data, const char* format, void* p_data_when_empty) {
        using func_t            = bool (*)(const char*, ImGuiDataType, void*, const char*, void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDataTyp"
                                                                                         "eApplyFro"
                                                                                         "mText"));
        return func(buf, data_type, p_data, format, p_data_when_empty);
    }

    inline int DataTypeCompare(ImGuiDataType data_type, const void* arg_1, const void* arg_2) {
        using func_t           = int (*)(ImGuiDataType, const void*, const void*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDataType"
                                                                                        "Compare"));
        return func(data_type, arg_1, arg_2);
    }

    inline bool DataTypeClamp(ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max) {
        using func_t            = bool (*)(ImGuiDataType, void*, const void*, const void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDataTyp"
                                                                                         "eClamp"));
        return func(data_type, p_data, p_min, p_max);
    }

    inline bool InputTextEx(const char* label, const char* hint, char* buf, int buf_size, const ImVec2 size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data) {
        using func_t = bool (*)(const char*, const char*, char*, int, const ImVec2, ImGuiInputTextFlags, ImGuiInputTextCallback, void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputTe"
                                                                                         "xtEx"));
        return func(label, hint, buf, buf_size, size_arg, flags, callback, user_data);
    }

    inline void InputTextDeactivateHook(ImGuiID id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igInputTe"
                                                                                         "xtDeactiv"
                                                                                         "ateHoo"
                                                                                         "k"));
        return func(id);
    }

    inline bool TempInputText(const ImRect bb, ImGuiID id, const char* label, char* buf, int buf_size, ImGuiInputTextFlags flags) {
        using func_t = bool (*)(const ImRect, ImGuiID, const char*, char*, int, ImGuiInputTextFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTempInp"
                                                                                         "utText"));
        return func(bb, id, label, buf, buf_size, flags);
    }

    inline bool TempInputScalar(const ImRect bb, ImGuiID id, const char* label, ImGuiDataType data_type, void* p_data, const char* format, const void* p_clamp_min, const void* p_clamp_max) {
        using func_t = bool (*)(const ImRect, ImGuiID, const char*, ImGuiDataType, void*, const char*, const void*, const void*);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTempInpu"
                                                                                        "tScalar"));
        return func(bb, id, label, data_type, p_data, format, p_clamp_min, p_clamp_max);
    }

    inline bool TempInputIsActive(ImGuiID id) {
        using func_t            = bool (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igTempInp"
                                                                                         "utIsActiv"
                                                                                         "e"));
        return func(id);
    }

    inline ImGuiInputTextState* GetInputTextState(ImGuiID id) {
        using func_t           = ImGuiInputTextState* (*)(ImGuiID);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGetInput"
                                                                                        "TextStat"
                                                                                        "e"));
        return func(id);
    }

    inline void SetNextItemRefVal(ImGuiDataType data_type, void* p_data) {
        using func_t            = void (*)(ImGuiDataType, void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igSetNext"
                                                                                         "ItemRefVa"
                                                                                         "l"));
        return func(data_type, p_data);
    }

    inline void ColorTooltip(const char* text, const float* col, ImGuiColorEditFlags flags) {
        using func_t            = void (*)(const char*, const float*, ImGuiColorEditFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorTo"
                                                                                         "oltip"));
        return func(text, col, flags);
    }

    inline void ColorEditOptionsPopup(const float* col, ImGuiColorEditFlags flags) {
        using func_t            = void (*)(const float*, ImGuiColorEditFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorEd"
                                                                                         "itOptions"
                                                                                         "Popup"));
        return func(col, flags);
    }

    inline void ColorPickerOptionsPopup(const float* ref_col, ImGuiColorEditFlags flags) {
        using func_t            = void (*)(const float*, ImGuiColorEditFlags);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igColorPi"
                                                                                         "ckerOptio"
                                                                                         "nsPopu"
                                                                                         "p"));
        return func(ref_col, flags);
    }

    inline int PlotEx(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, const ImVec2 size_arg) {
        using func_t = int (*)(ImGuiPlotType, const char*, float (*)(void*, int), void*, int, int, const char*, float, float, const ImVec2);
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igPlotE"
                                                                                        "x"));
        return func(plot_type, label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, size_arg);
    }

    inline void ShadeVertsLinearColorGradientKeepAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1) {
        using func_t            = void (*)(ImDrawList*, int, int, ImVec2, ImVec2, ImU32, ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShadeVe"
                                                                                         "rtsLinear"
                                                                                         "ColorGrad"
                                                                                         "ientKeepA"
                                                                                         "lpha"));
        return func(draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, col0, col1);
    }

    inline void ShadeVertsLinearUV(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2 a, const ImVec2 b, const ImVec2 uv_a, const ImVec2 uv_b, bool clamp) {
        using func_t = void (*)(ImDrawList*, int, int, const ImVec2, const ImVec2, const ImVec2, const ImVec2, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShadeVe"
                                                                                         "rtsLinear"
                                                                                         "UV"));
        return func(draw_list, vert_start_idx, vert_end_idx, a, b, uv_a, uv_b, clamp);
    }

    inline void ShadeVertsTransformPos(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2 pivot_in, float cos_a, float sin_a, const ImVec2 pivot_out) {
        using func_t = void (*)(ImDrawList*, int, int, const ImVec2, float, float, const ImVec2);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShadeVe"
                                                                                         "rtsTransf"
                                                                                         "ormPos"));
        return func(draw_list, vert_start_idx, vert_end_idx, pivot_in, cos_a, sin_a, pivot_out);
    }

    inline void GcCompactTransientMiscBuffers() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGcCompa"
                                                                                         "ctTransie"
                                                                                         "ntMiscBuf"
                                                                                         "fers"));
        return func();
    }

    inline void GcCompactTransientWindowBuffers(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGcCompa"
                                                                                         "ctTransie"
                                                                                         "ntWindowB"
                                                                                         "uffers"));
        return func(window);
    }

    inline void GcAwakeTransientWindowBuffers(ImGuiWindow* window) {
        using func_t            = void (*)(ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGcAwake"
                                                                                         "Transient"
                                                                                         "WindowBuf"
                                                                                         "fers"));
        return func(window);
    }

    inline void DebugLog(const char* fmt, ...) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugLo"
                                                                                         "gV"));
        va_list            args;
        va_start(args, fmt);
        func(fmt, args);
        va_end(args);
    }

    inline void DebugLogV(const char* fmt, va_list args) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugLo"
                                                                                         "gV"));
        return func(fmt, args);
    }

    inline void DebugAllocHook(ImGuiDebugAllocInfo* info, int frame_count, void* ptr, size_t size) {
        using func_t            = void (*)(ImGuiDebugAllocInfo*, int, void*, size_t);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugAl"
                                                                                         "locHoo"
                                                                                         "k"));
        return func(info, frame_count, ptr, size);
    }

    inline void ErrorCheckEndFrameRecover(ImGuiErrorLogCallback log_callback, void* user_data) {
        using func_t            = void (*)(ImGuiErrorLogCallback, void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igErrorCh"
                                                                                         "eckEndFra"
                                                                                         "meRecove"
                                                                                         "r"));
        return func(log_callback, user_data);
    }

    inline void ErrorCheckEndWindowRecover(ImGuiErrorLogCallback log_callback, void* user_data) {
        using func_t            = void (*)(ImGuiErrorLogCallback, void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igErrorCh"
                                                                                         "eckEndWin"
                                                                                         "dowRecove"
                                                                                         "r"));
        return func(log_callback, user_data);
    }

    inline void ErrorCheckUsingSetCursorPosToExtendParentBoundaries() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igErrorCh"
                                                                                         "eckUsingS"
                                                                                         "etCursorP"
                                                                                         "osToExten"
                                                                                         "dParentBo"
                                                                                         "undarie"
                                                                                         "s"));
        return func();
    }

    inline void DebugDrawCursorPos(ImU32 col) {
        using func_t            = void (*)(ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugDr"
                                                                                         "awCursorP"
                                                                                         "os"));
        return func(col);
    }

    inline void DebugDrawLineExtents(ImU32 col) {
        using func_t            = void (*)(ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugDr"
                                                                                         "awLineExt"
                                                                                         "ents"));
        return func(col);
    }

    inline void DebugDrawItemRect(ImU32 col) {
        using func_t            = void (*)(ImU32);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugDr"
                                                                                         "awItemRec"
                                                                                         "t"));
        return func(col);
    }

    inline void DebugTextUnformattedWithLocateItem(const char* line_begin, const char* line_end) {
        using func_t            = void (*)(const char*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugTe"
                                                                                         "xtUnforma"
                                                                                         "ttedWithL"
                                                                                         "ocateIte"
                                                                                         "m"));
        return func(line_begin, line_end);
    }

    inline void DebugLocateItem(ImGuiID target_id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugLo"
                                                                                         "cateIte"
                                                                                         "m"));
        return func(target_id);
    }

    inline void DebugLocateItemOnHover(ImGuiID target_id) {
        using func_t            = void (*)(ImGuiID);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugLo"
                                                                                         "cateItemO"
                                                                                         "nHover"));
        return func(target_id);
    }

    inline void DebugLocateItemResolveWithLastItem() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugLo"
                                                                                         "cateItemR"
                                                                                         "esolveWit"
                                                                                         "hLastIte"
                                                                                         "m"));
        return func();
    }

    inline void DebugBreakClearData() {
        using func_t            = void (*)();
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugBr"
                                                                                         "eakClearD"
                                                                                         "ata"));
        return func();
    }

    inline bool DebugBreakButton(const char* label, const char* description_of_location) {
        using func_t            = bool (*)(const char*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugBr"
                                                                                         "eakButto"
                                                                                         "n"));
        return func(label, description_of_location);
    }

    inline void DebugBreakButtonTooltip(bool keyboard_only, const char* description_of_location) {
        using func_t            = void (*)(bool, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugBr"
                                                                                         "eakButton"
                                                                                         "Toolti"
                                                                                         "p"));
        return func(keyboard_only, description_of_location);
    }

    inline void ShowFontAtlas(ImFontAtlas* atlas) {
        using func_t            = void (*)(ImFontAtlas*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igShowFon"
                                                                                         "tAtlas"));
        return func(atlas);
    }

    inline void DebugHookIdInfo(ImGuiID id, ImGuiDataType data_type, const void* data_id, const void* data_id_end) {
        using func_t            = void (*)(ImGuiID, ImGuiDataType, const void*, const void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugHo"
                                                                                         "okIdInf"
                                                                                         "o"));
        return func(id, data_type, data_id, data_id_end);
    }

    inline void DebugNodeColumns(ImGuiOldColumns* columns) {
        using func_t            = void (*)(ImGuiOldColumns*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deColumn"
                                                                                         "s"));
        return func(columns);
    }

    inline void DebugNodeDockNode(ImGuiDockNode* node, const char* label) {
        using func_t            = void (*)(ImGuiDockNode*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deDockNod"
                                                                                         "e"));
        return func(node, label);
    }

    inline void DebugNodeDrawList(ImGuiWindow* window, ImGuiViewportP* viewport, const ImDrawList* draw_list, const char* label) {
        using func_t = void (*)(ImGuiWindow*, ImGuiViewportP*, const ImDrawList*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deDrawLis"
                                                                                         "t"));
        return func(window, viewport, draw_list, label);
    }

    inline void DebugNodeDrawCmdShowMeshAndBoundingBox(ImDrawList* out_draw_list, const ImDrawList* draw_list, const ImDrawCmd* draw_cmd, bool show_mesh, bool show_aabb) {
        using func_t = void (*)(ImDrawList*, const ImDrawList*, const ImDrawCmd*, bool, bool);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deDrawCmd"
                                                                                         "ShowMeshA"
                                                                                         "ndBoundin"
                                                                                         "gBox"));
        return func(out_draw_list, draw_list, draw_cmd, show_mesh, show_aabb);
    }

    inline void DebugNodeFont(ImFont* font) {
        using func_t            = void (*)(ImFont*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deFont"));
        return func(font);
    }

    inline void DebugNodeFontGlyph(ImFont* font, const ImFontGlyph* glyph) {
        using func_t            = void (*)(ImFont*, const ImFontGlyph*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deFontGly"
                                                                                         "ph"));
        return func(font, glyph);
    }

    inline void DebugNodeStorage(ImGuiStorage* storage, const char* label) {
        using func_t            = void (*)(ImGuiStorage*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deStorag"
                                                                                         "e"));
        return func(storage, label);
    }

    inline void DebugNodeTabBar(ImGuiTabBar* tab_bar, const char* label) {
        using func_t            = void (*)(ImGuiTabBar*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deTabBa"
                                                                                         "r"));
        return func(tab_bar, label);
    }

    inline void DebugNodeTable(ImGuiTable* table) {
        using func_t            = void (*)(ImGuiTable*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deTabl"
                                                                                         "e"));
        return func(table);
    }

    inline void DebugNodeTableSettings(ImGuiTableSettings* settings) {
        using func_t            = void (*)(ImGuiTableSettings*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deTableSe"
                                                                                         "ttings"));
        return func(settings);
    }

    inline void DebugNodeInputTextState(ImGuiInputTextState* state) {
        using func_t            = void (*)(ImGuiInputTextState*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deInputTe"
                                                                                         "xtStat"
                                                                                         "e"));
        return func(state);
    }

    inline void DebugNodeTypingSelectState(ImGuiTypingSelectState* state) {
        using func_t            = void (*)(ImGuiTypingSelectState*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deTypingS"
                                                                                         "electStat"
                                                                                         "e"));
        return func(state);
    }

    inline void DebugNodeWindow(ImGuiWindow* window, const char* label) {
        using func_t            = void (*)(ImGuiWindow*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deWindo"
                                                                                         "w"));
        return func(window, label);
    }

    inline void DebugNodeWindowSettings(ImGuiWindowSettings* settings) {
        using func_t            = void (*)(ImGuiWindowSettings*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deWindowS"
                                                                                         "etting"
                                                                                         "s"));
        return func(settings);
    }

    inline void DebugNodeWindowsList(ImVector_ImGuiWindowPtr* windows, const char* label) {
        using func_t            = void (*)(ImVector_ImGuiWindowPtr*, const char*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deWindows"
                                                                                         "List"));
        return func(windows, label);
    }

    inline void DebugNodeWindowsListByBeginStackParent(ImGuiWindow** windows, int windows_size, ImGuiWindow* parent_in_begin_stack) {
        using func_t            = void (*)(ImGuiWindow**, int, ImGuiWindow*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deWindows"
                                                                                         "ListByBeg"
                                                                                         "inStackPa"
                                                                                         "rent"));
        return func(windows, windows_size, parent_in_begin_stack);
    }

    inline void DebugNodeViewport(ImGuiViewportP* viewport) {
        using func_t            = void (*)(ImGuiViewportP*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugNo"
                                                                                         "deViewpor"
                                                                                         "t"));
        return func(viewport);
    }

    inline void DebugRenderKeyboardPreview(ImDrawList* draw_list) {
        using func_t            = void (*)(ImDrawList*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugRe"
                                                                                         "nderKeybo"
                                                                                         "ardPrevie"
                                                                                         "w"));
        return func(draw_list);
    }

    inline void DebugRenderViewportThumbnail(ImDrawList* draw_list, ImGuiViewportP* viewport, const ImRect bb) {
        using func_t            = void (*)(ImDrawList*, ImGuiViewportP*, const ImRect);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igDebugRe"
                                                                                         "nderViewp"
                                                                                         "ortThumbn"
                                                                                         "ail"));
        return func(draw_list, viewport, bb);
    }

    inline const ImFontBuilderIO* ImFontAtlasGetBuilderForStbTruetype() {
        using func_t           = const ImFontBuilderIO* (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFontAt"
                                                                                        "lasGetBuil"
                                                                                        "derForStbT"
                                                                                        "ruetype"));
        return func();
    }

    inline void ImFontAtlasUpdateConfigDataPointers(ImFontAtlas* atlas) {
        using func_t            = void (*)(ImFontAtlas*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFontA"
                                                                                         "tlasUpdat"
                                                                                         "eConfigDa"
                                                                                         "taPointer"
                                                                                         "s"));
        return func(atlas);
    }

    inline void ImFontAtlasBuildInit(ImFontAtlas* atlas) {
        using func_t            = void (*)(ImFontAtlas*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFontA"
                                                                                         "tlasBuild"
                                                                                         "Init"));
        return func(atlas);
    }

    inline void ImFontAtlasBuildSetupFont(ImFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent) {
        using func_t            = void (*)(ImFontAtlas*, ImFont*, ImFontConfig*, float, float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFontA"
                                                                                         "tlasBuild"
                                                                                         "SetupFon"
                                                                                         "t"));
        return func(atlas, font, font_config, ascent, descent);
    }

    inline void ImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas, void* stbrp_context_opaque) {
        using func_t            = void (*)(ImFontAtlas*, void*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFontA"
                                                                                         "tlasBuild"
                                                                                         "PackCusto"
                                                                                         "mRects"));
        return func(atlas, stbrp_context_opaque);
    }

    inline void ImFontAtlasBuildFinish(ImFontAtlas* atlas) {
        using func_t            = void (*)(ImFontAtlas*);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFontA"
                                                                                         "tlasBuild"
                                                                                         "Finish"));
        return func(atlas);
    }

    inline void ImFontAtlasBuildRender8bppRectFromString(ImFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned char in_marker_pixel_value) {
        using func_t = void (*)(ImFontAtlas*, int, int, int, int, const char*, char, unsigned char);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFontA"
                                                                                         "tlasBuild"
                                                                                         "Render8bp"
                                                                                         "pRectFrom"
                                                                                         "String"));
        return func(atlas, x, y, w, h, in_str, in_marker_char, in_marker_pixel_value);
    }

    inline void ImFontAtlasBuildRender32bppRectFromString(ImFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned int in_marker_pixel_value) {
        using func_t = void (*)(ImFontAtlas*, int, int, int, int, const char*, char, unsigned int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFontA"
                                                                                         "tlasBuild"
                                                                                         "Render32b"
                                                                                         "ppRectFro"
                                                                                         "mStrin"
                                                                                         "g"));
        return func(atlas, x, y, w, h, in_str, in_marker_char, in_marker_pixel_value);
    }

    inline void ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_multiply_factor) {
        using func_t            = void (*)(unsigned char[256], float);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFontA"
                                                                                         "tlasBuild"
                                                                                         "MultiplyC"
                                                                                         "alcLookup"
                                                                                         "Table"));
        return func(out_table, in_multiply_factor);
    }

    inline void ImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride) {
        using func_t = void (*)(const unsigned char[256], unsigned char*, int, int, int, int, int);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igImFontA"
                                                                                         "tlasBuild"
                                                                                         "MultiplyR"
                                                                                         "ectAlpha"
                                                                                         "8"));
        return func(table, pixels, x, y, w, h, stride);
    }

    /////////////////////////////manual written functions
    inline void LogText(CONST char* fmt, ...) {
        using func_t            = void (*)(const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igLogText"
                                                                                         "V"));
        va_list            args;
        va_start(args, fmt);
        func(fmt, args);
        va_end(args);
    }

    inline void ImGuiTextBufferAppend(struct ImGuiTextBuffer* buffer, const char* fmt, ...) {
        using func_t            = void (*)(ImGuiTextBuffer*, const char*, va_list);
        static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImGuiText"
                                                                                         "Buffer_"
                                                                                         "appendf"
                                                                                         "v"));
        va_list            args;
        va_start(args, fmt);
        func(buffer, fmt, args);
        va_end(args);
    }

    inline float GET_FLT_MAX() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGET_FLT_"
                                                                                        "MAX"));
        return func();
    }

    inline float GET_FLT_MIN() {
        using func_t           = float (*)();
        const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "igGET_FLT_"
                                                                                        "MIN"));
        return func();
    }

    namespace ImVector_ImWcharManager {

        inline ImVector_ImWchar* Create() {
            using func_t           = ImVector_ImWchar* (*)();
            const auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVect"
                                                                                            "or_"
                                                                                            "ImWcha"
                                                                                            "r_"
                                                                                            "creat"
                                                                                            "e"));
            return func();
        }

        inline void Destroy(ImVector_ImWchar* self) {
            using func_t            = void (*)(ImVector_ImWchar*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec"
                                                                                             "tor_"
                                                                                             "ImWch"
                                                                                             "ar_"
                                                                                             "destr"
                                                                                             "oy"));
            return func(self);
        }

        inline void Init(ImVector_ImWchar* p) {
            using func_t            = void (*)(ImVector_ImWchar*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec"
                                                                                             "tor_"
                                                                                             "ImWch"
                                                                                             "ar_"
                                                                                             "Ini"
                                                                                             "t"));
            return func(p);
        }

        inline void UnInit(ImVector_ImWchar* p) {
            using func_t            = void (*)(ImVector_ImWchar*);
            static auto* const func = reinterpret_cast<func_t>(GetProcAddress(menuFramework, "ImVec"
                                                                                             "tor_"
                                                                                             "ImWch"
                                                                                             "ar_"
                                                                                             "UnIni"
                                                                                             "t"));
            return func(p);
        }
    }  // namespace ImVector_ImWcharManager

}  // namespace ImGui

// NOLINTEND(*-redundant-void-arg, *-reserved-identifier, *-pro-type-reinterpret-cast, *-use-using,
// *-avoid-c-arrays, *-avoid-return-with-void-value)
#pragma warning(pop)

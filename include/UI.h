//
// Created by judah on 23-02-2025.
//

#ifndef UI_H
#define UI_H
#include "SKSE-MCP/SKSEMenuFramework.h"

namespace UI {
    inline const std::string plus{ FontAwesome::UnicodeToUtf8(0x2b) };
    inline const std::string cross{ FontAwesome::UnicodeToUtf8(0x58) };
    inline const std::string question{ FontAwesome::UnicodeToUtf8(0x3f) };
}  // namespace UI

class SMFRenderer
{
public:
    static SMFRenderer& GetSingleton();
    static void __stdcall Register();
    static void __stdcall RenderExclusions();
#ifndef NDEBUG
    static void __stdcall RenderDebug();
#endif
    static void __stdcall RenderEditPresets();

    static void RenderJsonEditor(std::string_view filePath, rapidjson::Document& doc) noexcept;
    static void RenderJsonValue(const char* uniqueID, std::string_view key, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& alloc) noexcept;
    static void RenderJsonObject(const char* uniqueID, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& alloc) noexcept;
    static void RenderJsonArray(const char* uniqueID, std::string_view key, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& alloc) noexcept;

    SMFRenderer(const SMFRenderer&)             = delete;
    SMFRenderer(SMFRenderer&&)                  = delete;
    SMFRenderer& operator= (const SMFRenderer&) = delete;
    SMFRenderer& operator= (SMFRenderer&&)      = delete;

private:
    static SMFRenderer Singleton;
    SMFRenderer()  = default;
    ~SMFRenderer() = default;
};
#endif  // UI_H

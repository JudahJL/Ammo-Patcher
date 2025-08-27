//
// Created by judah on 17-02-2025.
//

#ifndef FILEPTRMANAGER_H
#define FILEPTRMANAGER_H

#pragma warning(push)
#pragma warning(disable: 4'996)

class FilePtrManager
{
public:
    explicit FilePtrManager(const char* path, const char* mode = "rb") noexcept:
        err(fopen_s(&fp, path, mode)) {
        // ReSharper disable CppDeprecatedEntity
        if(err != 0) {
            SPDLOG_ERROR("Warning: Failed to open file '{}' pointer. Error: {}", path, strerror(err));
        }
    }

    explicit FilePtrManager(const wchar_t* path, const wchar_t* mode = L"rb") noexcept:
        err(_wfopen_s(&fp, path, mode)) {
        if(err != 0) {
            wchar_t buffer[2'048];

            swprintf_s(buffer, std::size(buffer), L"Failed to open file '%ls' pointer. Error: %ls", path,
                       _wcserror(err));
            SPDLOG_ERROR(buffer);
        }
    }

    ~FilePtrManager() {
        if(fp) {
            if(fclose(fp) == EOF) {
                SPDLOG_ERROR("Failed to close file pointer: {}", strerror(errno));
            }
        }
    }

    FilePtrManager(const FilePtrManager&)             = delete;
    FilePtrManager& operator= (const FilePtrManager&) = delete;

    FilePtrManager(FilePtrManager&& other) noexcept:
        fp(other.fp), err(other.err) {
        other.fp  = nullptr;
        other.err = 0;
    }

    FilePtrManager& operator= (FilePtrManager&& other) noexcept {
        if(this != &other) {
            if(fp) {
                if(fclose(fp) == EOF) {
                    SPDLOG_ERROR("Failed to close file pointer in move constructor: {}", strerror(errno));
                }
            }
            fp        = other.fp;
            err       = other.err;
            other.fp  = nullptr;
            other.err = 0;
        }
        return *this;
    }

    [[nodiscard]] FILE* get() noexcept { return fp; }

    [[nodiscard]] FILE* get() const noexcept { return fp; }

    [[nodiscard]] errno_t error() const noexcept { return err; }

    [[nodiscard]] explicit operator bool () const noexcept {
        return fp != nullptr && err == 0;
    }

private:
    FILE*   fp{};
    errno_t err{};
};

#pragma warning(pop)

#endif  //FILEPTRMANAGER_H

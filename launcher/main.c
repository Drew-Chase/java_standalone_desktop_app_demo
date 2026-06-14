#include <stdio.h>
#include <sys/stat.h>
#include <windows.h>

#define message(message) MessageBoxW(nullptr, message, L"Standalone Application Demo", MB_OK)

bool file_exists(const char *filename) {
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

int main(const int argc, const char *argv[]) {
    // Determine the executable path so we can derive both the working
    // directory and the jar name (jar shares the binary's base name).
    char binary_path[MAX_PATH];
    GetModuleFileNameA(nullptr, binary_path, MAX_PATH);

    char *last_separator = strrchr(binary_path, '\\');
    char *binary_name;
    if (last_separator != nullptr) {
        *last_separator = '\0';        // truncate to the directory
        binary_name = last_separator + 1;  // file name (e.g. "my-cool-app.exe")
    } else {
        binary_name = binary_path;
    }
    const LPCSTR cwd = binary_path;

    // Strip the ".exe" extension to obtain the base name used for the jar.
    char base_name[MAX_PATH];
    strncpy(base_name, binary_name, MAX_PATH - 1);
    base_name[MAX_PATH - 1] = '\0';
    char *dot = strrchr(base_name, '.');
    if (dot != nullptr) {
        *dot = '\0';
    }

    // Build the relative jar path: ./bin/<base_name>.jar
    char jar_path[MAX_PATH];
    snprintf(jar_path, sizeof(jar_path), "./bin/%s.jar", base_name);

    // file_exists is relative to the current process working directory, which
    // is not guaranteed to be the executable's directory, so check against the
    // resolved absolute path.
    char jar_full_path[MAX_PATH];
    snprintf(jar_full_path, sizeof(jar_full_path), "%s\\bin\\%s.jar", cwd, base_name);
    if (!file_exists(jar_full_path)) {
        wchar_t error_message[MAX_PATH + 64];
        _snwprintf(error_message, sizeof(error_message) / sizeof(wchar_t),
                   L"Launch jar does not exist: %hs", jar_full_path);
        message(error_message);
        return 1;
    }

    char java_full_path[MAX_PATH];
    snprintf(java_full_path, sizeof(java_full_path), "%s\\jre\\bin\\java.exe", cwd);
    if (!file_exists(java_full_path)) {
        message(L"Unable to find the java executable at ./jre/bin/java.exe, please check your installation.");
        return 1;
    }

    STARTUPINFO startup_info = {0};
    PROCESS_INFORMATION process_information;
    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    ZeroMemory(&process_information, sizeof(process_information));

    char command_line[MAX_PATH * 2];
    snprintf(command_line, sizeof(command_line), "./jre/bin/javaw.exe -jar %s", jar_path);
    for (int i = 1; i < argc; i++) {
        // Offset +1 to skip the executable name
        strcat(command_line, " ");
        strcat(command_line, argv[i]);
    }

    if (!CreateProcess(
        nullptr,
        command_line,
        nullptr,
        nullptr,
        false,
        0,
        nullptr,
        cwd,
        &startup_info,
        &process_information
    )) {
        message(L"Unable to launch the application");
        return 1;
    }
}

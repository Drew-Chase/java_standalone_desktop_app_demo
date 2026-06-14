#include <sys/stat.h>
#include <windows.h>

#define message(message) MessageBoxW(nullptr, message, L"Standalone Application Demo", MB_OK)

bool file_exists(const char *filename) {
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

int main(const int argc, const char *argv[]) {
    if (!file_exists("./bin/app_demo.jar")) {
        message(L"Launch jar does not exist");
        return 1;
    }
    if (!file_exists("./jre/bin/java.exe")) {
        message(L"Unable to find the java executable at ./jre/bin/java.exe, please check your installation.");
        return 1;
    }

    STARTUPINFO startup_info = {0};
    PROCESS_INFORMATION process_information;
    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    ZeroMemory(&process_information, sizeof(process_information));


    // Sets the current working directory to the directory of the executable
    char binary_path[MAX_PATH];
    GetModuleFileNameA(nullptr, binary_path, MAX_PATH);
    char *last_separator = strrchr(binary_path, '\\');
    if (last_separator != nullptr) {
        *last_separator = '\0';
    }
    const LPCSTR cwd = binary_path;

    char command_line[MAX_PATH * 2] = "./jre/bin/javaw.exe -jar ./bin/app_demo.jar";
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

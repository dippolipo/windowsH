#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <Tchar.h>
#include <iostream>

HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;    // Handle to read from the console.

int printCharA(char c, int x, int y, short unsigned int fColor, short unsigned int bColor);
int printCharW(wchar_t c, int x, int y, short unsigned int fColor, short unsigned int bColor);
void hideCursor();
void showCursor(uint8_t cursorSize); //cursor size must be between 1 and 100
void setupConsole(uint8_t width, uint8_t height);
void disableResize();
void enableResize();

int _tmain(int argc, _TCHAR* argv[]) {
    setupConsole(80, 50 );
    hideCursor();
    disableResize();
    showCursor(1);
    // Set up the character buffer:
    CHAR_INFO consoleBuffer[80 * 50];
    COORD bufferSize = { 80, 50 };
    COORD characterPos = { 0, 0 };
    SMALL_RECT writeArea = { 0, 0, 79, 49 };

    std::string daw;
    std::cin >> daw;

    std::cout << daw;

    SetConsoleCursorPosition(wHnd, { 10, 10 });
    std::cin >> daw;

    std::cout << daw;
    while (1);
    return 0;
}

int printCharW(wchar_t c, int x, int y, short unsigned int fColor, short unsigned int bColor) {
    // Set up the character:
    CHAR_INFO letter;
    letter.Char.UnicodeChar = c;

    letter.Attributes =
        fColor | FOREGROUND_INTENSITY |
        bColor | BACKGROUND_INTENSITY;

    // Set up the positions:
    COORD charBufSize = { 1, 1 };
    COORD characterPos = { 0, 0 };
    SMALL_RECT writeArea = { x, y, x, y };

    // Write the character:
    WriteConsoleOutputW(wHnd, &letter, charBufSize, characterPos, &writeArea);
    printf("\n");
    return 0;
}

int printCharA(char c, int x, int y, short unsigned int fColor, short unsigned int bColor) {
    // Set up the character:
    CHAR_INFO letter;
    letter.Char.AsciiChar = c;

    letter.Attributes =
        fColor | FOREGROUND_INTENSITY |
        bColor | BACKGROUND_INTENSITY;

    // Set up the positions:
    COORD charBufSize = { 1, 1 };
    COORD characterPos = { 0, 0 };
    SMALL_RECT writeArea = { x, y, x, y };

    // Write the character:
    WriteConsoleOutputA(wHnd, &letter, charBufSize, characterPos, &writeArea);
    printf("\n");
    return 0;
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;   // Size can be 1-100, we set it to 1 for minimal visibility
    cursorInfo.bVisible = FALSE; // Set cursor visibility to false

    SetConsoleCursorInfo(wHnd, &cursorInfo);
}
void showCursor(uint8_t cursorSize) {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = cursorSize;   // Size can be 1-100, we set it to 1 for minimal visibility
    cursorInfo.bVisible = TRUE; // Set cursor visibility to false

    SetConsoleCursorInfo(wHnd, &cursorInfo);
}

void setupConsole(uint8_t width, uint8_t height) {
    // Set up the handles for reading/writing:
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);

    // Set the console output code page to UTF-8:
    SetConsoleOutputCP(CP_UTF8);

    // Change the window title:
    SetConsoleTitle(TEXT("Win32 Console Control Demo"));

    // Set up the required window size:
    SMALL_RECT windowSize = { 0, 0, width - 1, height - 1 };

    // Change the console window size:
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

    // Create a COORD to hold the buffer size:
    COORD bufferSize = { width, height };

    // Change the internal buffer size:
    SetConsoleScreenBufferSize(wHnd, bufferSize);
}

void disableResize() {
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style &= ~WS_SIZEBOX;  // Remove the resize border
    style &= ~WS_MAXIMIZEBOX;  // Disable maximize button
    SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void enableResize() {
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);

    // Add the resize border and maximize button
    style |= WS_SIZEBOX;  // Set the resize border bit
    style |= WS_MAXIMIZEBOX;  // Set the maximize button bit

    SetWindowLong(consoleWindow, GWL_STYLE, style);
}
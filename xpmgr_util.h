#pragma once

#include <windows.h>

/* Tiny helpers so both binaries can avoid libstdc++. */

static int xpmgr_streq(const char* a, const char* b) {
    if (!a || !b) {
        return 0;
    }
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

static void xpmgr_write(const char* text) {
    DWORD length = 0;
    DWORD wrote = 0;
    HANDLE out;
    if (!text) {
        return;
    }
    while (text[length]) {
        length++;
    }
    out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == NULL || out == INVALID_HANDLE_VALUE) {
        return;
    }
    WriteFile(out, text, length, &wrote, NULL);
}

static void xpmgr_write_uint(unsigned long value) {
    char digits[16];
    char text[16];
    int count = 0;
    int index = 0;
    if (value == 0) {
        xpmgr_write("0");
        return;
    }
    while (value && count < 16) {
        digits[count++] = (char)('0' + (value % 10));
        value /= 10;
    }
    while (count > 0) {
        text[index++] = digits[--count];
    }
    text[index] = 0;
    xpmgr_write(text);
}

static void xpmgr_write_wide(const wchar_t* text) {
    char bytes[260];
    int index = 0;
    if (!text) {
        return;
    }
    while (text[index] && index < 259) {
        wchar_t ch = text[index];
        bytes[index] = (ch >= 32 && ch < 127) ? (char)ch : '?';
        index++;
    }
    bytes[index] = 0;
    xpmgr_write(bytes);
}

static void xpmgr_free(void* block) {
    if (block) {
        HeapFree(GetProcessHeap(), 0, block);
    }
}

static wchar_t* xpmgr_widen(const char* text) {
    int chars;
    wchar_t* wide;
    if (!text) {
        return NULL;
    }
    chars = MultiByteToWideChar(CP_ACP, 0, text, -1, NULL, 0);
    if (chars <= 0) {
        return NULL;
    }
    wide = (wchar_t*)HeapAlloc(GetProcessHeap(), 0, (SIZE_T)chars * sizeof(wchar_t));
    if (!wide) {
        return NULL;
    }
    if (MultiByteToWideChar(CP_ACP, 0, text, -1, wide, chars) <= 0) {
        HeapFree(GetProcessHeap(), 0, wide);
        return NULL;
    }
    return wide;
}

static char* xpmgr_narrow(const wchar_t* text) {
    int bytes;
    char* narrow;
    UINT codepage = CP_UTF8;
    if (!text) {
        return NULL;
    }
    /* Windows 98, Me, and NT 4 reject CP_UTF8. These strings are ASCII. */
    bytes = WideCharToMultiByte(codepage, 0, text, -1, NULL, 0, NULL, NULL);
    if (bytes <= 0) {
        codepage = CP_ACP;
        bytes = WideCharToMultiByte(codepage, 0, text, -1, NULL, 0, NULL, NULL);
        if (bytes <= 0) {
            return NULL;
        }
    }
    narrow = (char*)HeapAlloc(GetProcessHeap(), 0, (SIZE_T)bytes);
    if (!narrow) {
        return NULL;
    }
    if (WideCharToMultiByte(codepage, 0, text, -1, narrow, bytes, NULL, NULL) <= 0) {
        HeapFree(GetProcessHeap(), 0, narrow);
        return NULL;
    }
    return narrow;
}

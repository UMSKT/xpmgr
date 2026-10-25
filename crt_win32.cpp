/* Freestanding startup for the Pentium / Office XP host-floor x86 build.
   Linked with -nostdlib so libgcc, libstdc++, and the MinGW CRT stay out. */

#include <windows.h>

extern int main(int argc, char** argv);

typedef void (*xpmgr_ctor)(void);

extern "C" xpmgr_ctor __init_array_start[] __attribute__((weak));
extern "C" xpmgr_ctor __init_array_end[] __attribute__((weak));

extern "C" void* memset(void* dst, int value, size_t count) {
    unsigned char* bytes = (unsigned char*)dst;
    unsigned char fill = (unsigned char)value;
    while (count--) {
        *bytes++ = fill;
    }
    return dst;
}

extern "C" void* memcpy(void* dst, const void* src, size_t count) {
    unsigned char* out = (unsigned char*)dst;
    const unsigned char* in = (const unsigned char*)src;
    while (count--) {
        *out++ = *in++;
    }
    return dst;
}

extern "C" void* memmove(void* dst, const void* src, size_t count) {
    unsigned char* out = (unsigned char*)dst;
    const unsigned char* in = (const unsigned char*)src;
    size_t index;
    if (out == in || count == 0) {
        return dst;
    }
    if (out < in) {
        for (index = 0; index < count; index++) {
            out[index] = in[index];
        }
    } else {
        index = count;
        while (index > 0) {
            index--;
            out[index] = in[index];
        }
    }
    return dst;
}

extern "C" int memcmp(const void* left, const void* right, size_t count) {
    const unsigned char* a = (const unsigned char*)left;
    const unsigned char* b = (const unsigned char*)right;
    while (count--) {
        if (*a != *b) {
            return *a - *b;
        }
        a++;
        b++;
    }
    return 0;
}

/* GCC calls this when a frame is at least one page. EAX is the frame size
   and must be preserved; the caller subtracts it from ESP afterwards. */
extern "C" __attribute__((naked)) void __chkstk_ms(void) {
    __asm__ __volatile__(
        "pushl %ecx\n\t"
        "pushl %eax\n\t"
        "cmpl $0x1000, %eax\n\t"
        "leal 12(%esp), %ecx\n\t"
        "jb 1f\n\t"
        "2:\n\t"
        "subl $0x1000, %ecx\n\t"
        "testl %eax, (%ecx)\n\t"
        "subl $0x1000, %eax\n\t"
        "cmpl $0x1000, %eax\n\t"
        "ja 2b\n\t"
        "1:\n\t"
        "subl %eax, %ecx\n\t"
        "testl %eax, (%ecx)\n\t"
        "popl %eax\n\t"
        "popl %ecx\n\t"
        "ret\n\t"
    );
}

extern "C" void __cxa_pure_virtual(void) {
    ExitProcess(1);
}

extern "C" void __main(void) {
    xpmgr_ctor* ctor;
    if (!__init_array_start || !__init_array_end) {
        return;
    }
    for (ctor = __init_array_start; ctor < __init_array_end; ctor++) {
        if (*ctor) {
            (*ctor)();
        }
    }
}

static int is_space(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

extern "C" void mainCRTStartup(void) {
    char* command = GetCommandLineA();
    char* buffer;
    char** argv;
    int length = 0;
    int argc = 0;
    char* cursor;
    HANDLE heap = GetProcessHeap();

    if (!command) {
        command = (char*)"";
    }
    while (command[length]) {
        length++;
    }

    buffer = (char*)HeapAlloc(heap, 0, (SIZE_T)length + 1);
    argv = (char**)HeapAlloc(heap, HEAP_ZERO_MEMORY, (SIZE_T)(length + 2) * sizeof(char*));
    if (!buffer || !argv) {
        ExitProcess(1);
    }
    for (int index = 0; index <= length; index++) {
        buffer[index] = command[index];
    }

    cursor = buffer;
    while (*cursor) {
        char* out;
        int quoted = 0;
        int more;
        while (is_space(*cursor)) {
            cursor++;
        }
        if (!*cursor) {
            break;
        }
        argv[argc++] = cursor;
        out = cursor;
        while (*cursor) {
            if (*cursor == '"') {
                quoted = !quoted;
                cursor++;
                continue;
            }
            if (!quoted && is_space(*cursor)) {
                break;
            }
            *out++ = *cursor++;
        }
        more = *cursor != 0;
        *out = 0;
        if (more) {
            cursor++;
        }
    }
    argv[argc] = NULL;

    ExitProcess((UINT)main(argc, argv));
}

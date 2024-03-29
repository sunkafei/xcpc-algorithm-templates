// 每次读取/输出一行，适用于处理交互题。
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <array>
using namespace std;
namespace io {
    constexpr int MAXBUFFER = 1024 * 1024 * 8;
    char ibuffer[MAXBUFFER], *iptr, obuffer[MAXBUFFER], *optr;
    inline void start_reading() { // 开始读取新的一行
        fgets(ibuffer, sizeof(ibuffer), stdin);
        iptr = ibuffer;
    }
    inline void start_writing() { //开始输出新的一行
        optr = obuffer;
    }
    inline int read_int() { //读入有符号整数
        char* nxt;
        int ret = strtol(iptr, &nxt, 10);
        iptr = nxt;
        return ret;
    }
    inline double read_double() noexcept { // 读入浮点数
        char* nxt;
        double ret = strtod(iptr, &nxt);
        iptr = nxt;
        return ret;
    }
    inline void write_int(int val) { //输出有符号整数，输出完一行后需要调用flush。
        char tmp[32], *now = tmp + 20;
        int length = 1;
        if (val < 0) {
            *optr++ = '-';
            val *= -1;
        }
        *now = ' ';
        do {
            *--now = '0' + val % 10;
            val /= 10;
            length += 1;
        } while (val > 0);
        memcpy(optr, now, length);
        optr += length;
    }
    inline void flush() {
        if (optr != obuffer) {
            optr[-1] = '\n';
        }
        fwrite(obuffer, 1, optr - obuffer, stdout);
        fflush(stdout);
    }
}
int main() {
    io::start_reading();
    double val = io::read_double();
    printf("%.6f\n", val);
    return 0;
}
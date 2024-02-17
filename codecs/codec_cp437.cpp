/* Copyright (c) 2024 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "codec_cp437.h"
#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
static const char hexchars[] = "0123456789ABCDEF";

// Convert IBM437 character codes 0x00 - 0xFF into Unicode.
static ushort const cp437ToUnicode[256] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013,
    0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001c, 0x001b, 0x007f, 0x001d, 0x001e, 0x001f, 0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
    0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003a, 0x003b,
    0x003c, 0x003d, 0x003e, 0x003f, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f, 0x0060, 0x0061, 0x0062, 0x0063,
    0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
    0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x001a, 0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5, 0x00e7, 0x00ea, 0x00eb, 0x00e8, 0x00ef,
    0x00ee, 0x00ec, 0x00c4, 0x00c5, 0x00c9, 0x00e6, 0x00c6, 0x00f4, 0x00f6, 0x00f2, 0x00fb, 0x00f9, 0x00ff, 0x00d6, 0x00dc, 0x00a2, 0x00a3, 0x00a5, 0x20a7, 0x0192,
    0x00e1, 0x00ed, 0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00ba, 0x00bf, 0x2310, 0x00ac, 0x00bd, 0x00bc, 0x00a1, 0x00ab, 0x00bb, 0x2591, 0x2592, 0x2593, 0x2502,
    0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255d, 0x255c, 0x255b, 0x2510, 0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x255e, 0x255f,
    0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x2567, 0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b, 0x256a, 0x2518, 0x250c, 0x2588,
    0x2584, 0x258c, 0x2590, 0x2580, 0x03b1, 0x00df, 0x0393, 0x03c0, 0x03a3, 0x03c3, 0x03bc, 0x03c4, 0x03a6, 0x0398, 0x03a9, 0x03b4, 0x221e, 0x03c6, 0x03b5, 0x2229,
    0x2261, 0x00b1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00f7, 0x2248, 0x00b0, 0x2219, 0x00b7, 0x221a, 0x207f, 0x00b2, 0x25a0, 0x00a0};

// Convert Unicode 0x0000 - 0x00FF into IBM437.
static unsigned char const cp437FromUnicode[256] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x7f, 0x1b, 0x1a, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d,
    0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x1c, '?',  '?',
    '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
    '?',  '?',  '?',  '?',  0xff, 0xad, 0x9b, 0x9c, '?',  0x9d, '?',  0x15, '?',  '?',  0xa6, 0xae, 0xaa, '?',  '?',  '?',  0xf8, 0xf1, 0xfd, '?',  '?',  '?',
    0x14, 0xfa, '?',  '?',  0xa7, 0xaf, 0xac, 0xab, '?',  0xa8, '?',  '?',  '?',  '?',  0x8e, 0x8f, 0x92, 0x80, '?',  0x90, '?',  '?',  '?',  '?',  '?',  '?',
    '?',  0xa5, '?',  '?',  '?',  '?',  0x99, '?',  '?',  '?',  '?',  '?',  0x9a, '?',  '?',  0xe1, 0x85, 0xa0, 0x83, '?',  0x84, 0x86, 0x91, 0x87, 0x8a, 0x82,
    0x88, 0x89, 0x8d, 0xa1, 0x8c, 0x8b, '?',  0xa4, 0x95, 0xa2, 0x93, '?',  0x94, 0xf6, '?',  0x97, 0xa3, 0x96, 0x81, '?',  '?',  0x98};

codec_cp437::codec_cp437()
{
}

codec_cp437::~codec_cp437()
{
}

QByteArray codec_cp437::name() const
{
    return "IBM437";
}

QList<QByteArray> codec_cp437::aliases() const
{
    QList<QByteArray> list;
    list << "CP437";

    return list;
}

int codec_cp437::mibEnum() const
{
    return 437;
}

QString codec_cp437::convertToUnicode(const char *in, int length, ConverterState *state) const
{
    Q_UNUSED(state)

    QString str;
    if (length >= 6 && in[0] == '8' && in[1] == '0' && in[length - 4] == 'F' && in[length - 3] == 'F' && in[length - 2] == 'F' && in[length - 1] == 'F') {
        // UCS-2 string embedded within a 437-encoded string.
        int nibble = 0;
        int value = 0;
        int digit;
        in += 2;
        length -= 6;
        while (length-- > 0) {
            char ch = *in++;
            if (ch >= '0' && ch <= '9') digit = ch - '0';
            else if (ch >= 'A' && ch <= 'F') digit = ch - 'A' + 10;
            else if (ch >= 'a' && ch <= 'f') digit = ch - 'a' + 10;
            else continue;
            value = value * 16 + digit;
            ++nibble;
            if (nibble >= 4) {
                str += QChar((ushort)value);
                nibble = 0;
                value = 0;
            }
        }

    } else {
        // Regular 437-encoded string.
        while (length-- > 0) str += QChar((unsigned int)cp437ToUnicode[*in++ & 0xFF]);
    }
    return str;
}

QByteArray codec_cp437::convertFromUnicode(const QChar *in, int length, ConverterState *state) const
{
    Q_UNUSED(state)

    QByteArray result;
    unsigned int ch;
    char *out;

    // Determine if the string should be encoded using the UCS-2 hack.
    bool non437 = false;
    for (qint32 posn = 0; !non437 && posn < length; ++posn) {
        ch = in[posn].unicode();
        if (ch >= 0x0100) non437 = true;
        else if (cp437FromUnicode[ch] == '?' && ch != '?') non437 = true;
    }
    if (non437) {
        // There is a non CP437 character in this string, so use UCS-2.
        result.resize(length * 4 + 6);
        out = result.data();
        *out++ = '8';
        *out++ = '0';
        while (length-- > 0) {
            uint ch = in->unicode();
            ++in;
            *out++ = hexchars[(ch >> 12) & 0x0F];
            *out++ = hexchars[(ch >> 8) & 0x0F];
            *out++ = hexchars[(ch >> 4) & 0x0F];
            *out++ = hexchars[ch & 0x0F];
        }
        *out++ = 'F';
        *out++ = 'F';
        *out++ = 'F';
        *out = 'F';
        return result;
    }

    // If we get here, we can guarantee that the string only contains
    // valid CP437 code points between 0x0000 and 0x00FF.
    result.resize(length);
    out = result.data();
    while (length-- > 0) {
        *out++ = (char)cp437FromUnicode[in->unicode()];
        ++in;
    }
    return result;
}
#endif

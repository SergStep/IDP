#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <iomanip>
#include <codecvt>

// wrapper for wstring/wbuffer convert
template<class Facet>
struct deletable_facet : Facet
{
    template<class ...Args>
    deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
};

int main (int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Using: " << argv[1] << "utf_file_path \n";
        return 0;
    }
    std::string filePath(argv[1]);

    std::string fileData;
    std::ifstream(filePath) >> fileData;

    std::cout << "UTF 8: \n";
    for (int  c : fileData)
    {
        std::cout << " u8" << std::hex << std::setw(1) << std::setfill('0') << c;
    }
    std::cout << std::endl;

    // using standard (locale-independent) codecvt facet
    std::wstring_convert<
        deletable_facet<std::codecvt<char16_t, char, std::mbstate_t>>,
        char16_t> conv16;
    std::u16string str16 = conv16.from_bytes(fileData);

    std::cout << "UTF 16: \n";
    for (char16_t c : str16)
    {
        std::cout << " u" << std::hex << std::setw(4) << std::setfill('0') << c;
    }
    std::cout << std::endl;

    // wifstream are using codecvt<wchar_t, char, mbstate_t>
    std::wifstream inputStream(filePath);
    // this locale's codecvt converts to UCS4 code(on the Linux systems).
    // associates loc to the stream as the new locale object to be used with locale-sensitive operations
    inputStream.imbue(std::locale("en_US.UTF-8"));
    std::cout << "UTF 32: \n";
    for (wchar_t c; inputStream >> c; )
    {
        std::cout << " U" << std::hex << std::setw(4) << std::setfill('0') << c;
    }
    std::cout << std::endl;
}

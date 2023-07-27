
#include <iostream>
#include <string>
#include "zip.h"

static size_t on_unzip_callback(void* arg, unsigned long long offset, const void* data, size_t size)
{
  std::string* str = dynamic_cast<std::string*>((std::string*)arg);
  str->append((const char*)data, size);
  
  return size;
}

#define USAGE           \
    "Usage: ./program <input zip file>\n"

int main(int argc, char* argv[])
{   
  if (argc != 2) {
    std::cerr << USAGE << std::endl;
    return -1;
  }

  struct zip_t *zip = zip_open(argv[1], 0, 'r');
  
  std::cout << "total files: " << zip_total_entries(zip) << std::endl;
  int i, total = zip_total_entries(zip);
  
  std::string stream;
  for (i = 0; i < total; i++) {
    stream.clear();
    if (zip_entry_openbyindex(zip, i) != 0) {
      std::cerr << "zip_entry_openbyindex failed(index=" << i << ")" << std::endl;
      if (zip) zip_close(zip);
      return -1;
    }

    if (zip_entry_extract(zip, on_unzip_callback, &stream) != 0) {
      std::cerr << "zip_entry_extract failed(index=" << i << ")" << std::endl;
      if (zip) {
        zip_entry_close(zip);
        zip_close(zip);
      }
      return -1;
    }
    std::cout << "metadata[index, name, size] = [" << i << ", "
                                                    << zip_entry_name(zip) << ", "
                                                    << stream.size() << "]" << std::endl;        
    zip_entry_close(zip);
  }

  zip_close(zip);
      
  return 0;
}
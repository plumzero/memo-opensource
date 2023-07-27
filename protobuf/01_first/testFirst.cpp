
#include <fstream>
#include <cstdio>
#include <cassert>
#include <string>
#include <utility>

#include "addressbook.pb.h"

int main(int argc, char* argv[])
{   
    (void) argc;
    (void) argv;

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    std::string strAddressBook;
    {
        printf("========== 开始序列化测试 ==========\n");
        tutorial::AddressBook address_book;
        tutorial::Person * person;
        tutorial::Person::PhoneNumber * phonenumber;
        
        // first people
        person = address_book.add_people();
        person->set_id(10001);
        person->set_name("Jim"); 
        person->set_email("Jim@google.com");
            
        phonenumber = person->add_phones();
        phonenumber->set_number("17601001773");
        
        phonenumber = person->add_phones();
        phonenumber->set_number("13261009856");
        phonenumber->set_type(tutorial::Person::PhoneType::Person_PhoneType_WORK);
        
        // second people
        person = address_book.add_people();
        person->set_id(10002);
        person->set_name("Sam");
        person->set_email("Sam@qq.com");
        
        phonenumber = person->add_phones();
        phonenumber->set_number("15660195460");
        phonenumber->set_type(tutorial::Person::PhoneType::Person_PhoneType_MOBILE);
        
        // third people
        person = address_book.add_people();
        person->set_id(10003);
        person->set_name("Tom");
        person->set_email("Tom@163.com");
        
        phonenumber = person->add_phones();
        phonenumber->set_number("13656452151");
        phonenumber->set_type(tutorial::Person::PhoneType::Person_PhoneType_HOME);
        
        assert(address_book.SerializeToString(&strAddressBook));
        
        printf("stream size=%lu\n", strAddressBook.size());
    }   // end namespace

    {
        printf("========== 开始结构化测试 ==========\n");
        tutorial::AddressBook address_book;
        assert(address_book.ParseFromString(strAddressBook));
        
        printf("people_size=%d\n", address_book.people_size());
        
        decltype(std::declval<tutorial::AddressBook>().people_size()) i;
        for (i = 0; i < address_book.people_size(); i++) {
            const tutorial::Person& person = address_book.people(i);
            printf("id: %d, name: %s, email: %s\n", person.id(), person.name().c_str(), person.email().c_str());
            
            decltype(std::declval<tutorial::Person>().phones_size()) j;
            for (j = 0; j < person.phones_size(); j++) {
                const tutorial::Person::PhoneNumber & phonenumber = person.phones(j);
                switch(phonenumber.type())
                {
                    case tutorial::Person::MOBILE:
                        printf("Mobile phone:");
                    break;
                    case tutorial::Person::HOME:
                        printf("Home phone:");
                    break;
                    case tutorial::Person::WORK:
                        printf("Work phone:");
                    break;
                    default:
                        printf("Unknown phone:");
                    break;
                }
                printf(" %s\n", phonenumber.number().c_str());
            }           
        }
    }   // end namespace
    
    return 0;
}

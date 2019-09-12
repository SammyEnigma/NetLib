#ifndef _MAIL_BOX
#define _MAIL_BOX


#include <string>

class MailBox
{
public:

    MailBox();
    MailBox(const MailBox &other);
    ~MailBox();
    void operator=(const MailBox& other);
    void SetDirectory(std::string dir);
    void SetStatus(std::string st);
    void SetAccount(std::string ac);
    void SetUid(std::string uid);
    std::string GetDirectory();
    std::string GetStatus();
    std::string GetAccount();
    std::string GetUid();
private:
    std::string directory;
    std::string status;
    std::string account;
    std::string UID;
};

#endif

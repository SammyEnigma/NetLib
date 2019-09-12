#include "MailBox.hpp"

MailBox::MailBox()
{

}

MailBox::~MailBox()
{

}

MailBox::MailBox(const MailBox& other)
{
    directory = other.directory;
    status = other.status;
    account = other.account;
    UID = other.UID;
}

void MailBox::operator=(const MailBox& other)
{
    directory = other.directory;
    status = other.status;
    account = other.account;
    UID = other.UID;
}

void MailBox::SetDirectory(std::string dir)
{
    directory = dir;
}

void MailBox::SetStatus(std::string st)
{
    status = st;
}

void MailBox::SetAccount(std::string ac)
{
    account = ac;
}

void MailBox::SetUid(std::string uid)
{
   UID = uid;
}

std::string MailBox::GetDirectory()
{
    return directory;
}

std::string MailBox::GetStatus()
{
    return status;
}

std::string MailBox::GetAccount()
{
    return account;
}

std::string MailBox::GetUid()
{
    return UID;
}


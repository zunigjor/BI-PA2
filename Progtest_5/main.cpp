#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#endif /* __PROGTEST */
using namespace std;
#ifndef __PROGTEST__
class CTimeStamp {
public:
    CTimeStamp(int year, int month, int day, int hour, int minute, int sec);
    int Compare(const CTimeStamp &x) const;
    friend ostream &operator<<(ostream &os, const CTimeStamp &x);
private:
    int m_Year;
    int m_Month;
    int m_Day;
    int m_Hour;
    int m_Minute;
    int m_Sec;
};

CTimeStamp::CTimeStamp(int year, int month, int day, int hour, int minute, int sec)
: m_Year(year), m_Month(month), m_Day(day), m_Hour(hour), m_Minute(minute),m_Sec(sec){}

/**
 * Compare this and parameter x.
 * @param x
 * @return -1 if this < x / 0 if this == x / 1 if this > x
 */
int CTimeStamp::Compare(const CTimeStamp &x) const {
    if (m_Year < x.m_Year){
        return -1;
    } else if (m_Year > x.m_Year){
        return 1;
    } else {
        if (m_Month < x.m_Month){
            return -1;
        } else if (m_Month > x.m_Month){
            return 1;
        } else {
            if (m_Day < x.m_Day){
                return -1;
            } else if (m_Day > x.m_Day){
                return 1;
            } else {
                if (m_Hour < x.m_Hour){
                    return -1;
                } else if (m_Hour > x.m_Hour){
                    return 1;
                } else {
                    if (m_Minute < x.m_Minute){
                        return -1;
                    } else if (m_Minute > x.m_Minute){
                        return 1;
                    } else {
                        if (m_Sec < x.m_Sec){
                            return -1;
                        } else if (m_Sec > x.m_Sec){
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        }
    }
}

ostream &operator<<(ostream &os, const CTimeStamp &x) {
    os << x.m_Year << "-";
    os << setfill('0') << setw(2) << x.m_Month;
    os << "-" << x.m_Day;
    os << " ";
    os << x.m_Hour << ":" << x.m_Minute << ":" << x.m_Sec;
    return os;
}

////====================================================================================================================
class CMailBody {
public:
    CMailBody(int size, const char *src);
    CMailBody(const CMailBody & src);
    ~CMailBody();
    CMailBody & operator = (const CMailBody &);
    // copy cons/op=/destructor is correctly implemented in the testing environment
    friend ostream &operator<<(ostream &os, const CMailBody &x) {
        return os << "mail body: " << x.m_Size << " B";
    }
private:
    int m_Size;
    char *m_Data;
};

CMailBody::CMailBody(int size, const char *src):m_Size(size), m_Data(new char [strlen(src) + 1]){
    memcpy(m_Data, src, strlen(src) + 1);
}

CMailBody::CMailBody(const CMailBody &src):m_Size(src.m_Size), m_Data(new char [strlen(src.m_Data) + 1]){
    memcpy(m_Data, src.m_Data, strlen(src.m_Data) + 1);
}

CMailBody::~CMailBody() {
    delete [] m_Data;
}

CMailBody &CMailBody::operator=(const CMailBody & src) {
    if (this == &src) return *this;
    delete [] m_Data;
    m_Data = new char [strlen(src.m_Data) + 1];
    memcpy(m_Data,src.m_Data, strlen(src.m_Data) + 1);
    m_Size = src.m_Size;
    return *this;
}

////====================================================================================================================
class CAttach {
public:
    CAttach(int x): m_X(x), m_RefCnt(1) {}
    void AddRef() const {m_RefCnt++;}
    void Release() const {
        if (!--m_RefCnt)
            delete this;
    }
private:
    int m_X;
    mutable int m_RefCnt;
    CAttach(const CAttach &x);
    CAttach &operator=(const CAttach &x);
    ~CAttach() = default;
    friend ostream &operator<<(ostream &os, const CAttach &x) {
        return os << "attachment: " << x.m_X << " B";
    }
};
#endif /* __PROGTEST__, DO NOT remove */
////====================================================================================================================
class CMail {
public:
    CMail(const CTimeStamp &timeStamp, const string &from, const CMailBody &body, const CAttach *attach);
    CMail(const CMail & src);
    ~CMail();
    const string &From() const;
    const CMailBody &Body() const;
    const CTimeStamp &TimeStamp() const;
    const CAttach *Attachment() const;
    friend ostream &operator<<(ostream &os, const CMail &x);
    friend bool cmpMail(const CMail * a, const CMail * b);
private:
    string m_From;
    CTimeStamp m_TimeStamp;
    CMailBody m_MailBody;
    const CAttach * m_Attachment;
};
const string &CMail::From() const {
    return m_From;
}
const CMailBody &CMail::Body() const {
    return m_MailBody;
}
const CTimeStamp &CMail::TimeStamp() const {
    return m_TimeStamp;
}
const CAttach *CMail::Attachment() const {
    return m_Attachment;
}
ostream &operator<<(ostream &os, const CMail &x) {
    os << x.m_TimeStamp << " " << x.m_From << " " << x.m_MailBody;
    if (x.m_Attachment != nullptr)
        os << " + " << (*x.m_Attachment);
    return os;
}

CMail::CMail(const CTimeStamp &timeStamp, const string &from, const CMailBody &body, const CAttach *attach)
:m_From(from), m_TimeStamp(timeStamp), m_MailBody(body), m_Attachment(attach){
    if (m_Attachment != nullptr) m_Attachment->AddRef();
}

CMail::CMail(const CMail &src):m_From(src.m_From), m_TimeStamp(src.m_TimeStamp), m_MailBody(src.m_MailBody), m_Attachment(src.m_Attachment){
    if (m_Attachment != nullptr) m_Attachment->AddRef();
}

////====================================================================================================================
CMail::~CMail(){
    if(m_Attachment != nullptr) m_Attachment->Release();
}

bool cmpMail(const CMail * a, const CMail * b) {
    return a->m_TimeStamp.Compare(b->m_TimeStamp) < 0;
}

////====================================================================================================================
class CMailFolder {
public:
    CMailFolder(const string & folderName);
    CMailFolder(const CMailFolder & src);
    ~CMailFolder();
    CMailFolder & operator = (const CMailFolder & src);
    bool Delivery(const CMail &mail);
    const string &Name() const;
    void MergeMailFolder(CMailFolder * src);
    list<CMail> ListMail(const CTimeStamp &from, const CTimeStamp &to) const;
    set<string> ListAddr(const CTimeStamp &from, const CTimeStamp &to) const;
private:
    string m_Name;
    list<CMail *> m_Mails;
};

CMailFolder::CMailFolder(const string & folderName):m_Name(folderName){}

CMailFolder::CMailFolder(const CMailFolder &src)
:m_Name(src.m_Name),m_Mails(src.m_Mails){}

CMailFolder::~CMailFolder(){
    for (auto it : m_Mails)
        delete it;
}

bool CMailFolder::Delivery(const CMail &mail) {
    CMail * newMailPtr = new CMail(mail);
    auto mailsIt = lower_bound(m_Mails.begin(),m_Mails.end(),newMailPtr,cmpMail);
    m_Mails.insert(mailsIt, newMailPtr);
    return true;
}

const string & CMailFolder::Name(void) const {
    return m_Name;
}

void CMailFolder::MergeMailFolder(CMailFolder * src) {
    this->m_Mails.merge(src->m_Mails,cmpMail);
}

list<CMail> CMailFolder::ListMail(const CTimeStamp &from, const CTimeStamp &to) const {
    list<CMail> mails;
    CMail * fromPtr = new CMail(from,"",CMailBody(0,""), nullptr);
    auto fromIt = lower_bound(m_Mails.begin(),m_Mails.end(),fromPtr,cmpMail);
    delete fromPtr;
    CMail * toPtr = new CMail(to,"",CMailBody(0,""), nullptr);
    auto toIt = upper_bound(m_Mails.begin(),m_Mails.end(), toPtr,cmpMail);
    delete toPtr;
    for (auto i = fromIt; i != toIt; ++i) {
        mails.push_back((**i));
    }
    return mails;
}

set<string> CMailFolder::ListAddr(const CTimeStamp &from, const CTimeStamp &to) const {
    set<string> addresses;
    CMail * fromPtr = new CMail(from,"",CMailBody(0,""), nullptr);
    auto fromIt = lower_bound(m_Mails.begin(),m_Mails.end(),fromPtr,cmpMail);
    delete fromPtr;
    CMail * toPtr = new CMail(to,"",CMailBody(0,""), nullptr);
    auto toIt = upper_bound(m_Mails.begin(),m_Mails.end(),toPtr,cmpMail);
    delete toPtr;
    for (auto i = fromIt; i != toIt; ++i) {
        addresses.insert((*i)->From());
    }
    return addresses;
}

CMailFolder &CMailFolder::operator=(const CMailFolder & src) {
    if (this == &src) return *this;
    m_Name = src.Name();
    m_Mails.clear();
    m_Mails = src.m_Mails;
    return *this;
}

bool cmpMailFolder(const CMailFolder * a, const CMailFolder * b){
    return a->Name() < b->Name();
}

////====================================================================================================================
class CMailBox {
public:
    CMailBox(void);
    ~CMailBox(void);
    bool Delivery(const CMail &mail);
    bool NewFolder(const string &folderName);
    bool MoveMail(const string &fromFolder, const string &toFolder);
    list<CMail> ListMail(const string &folderName, const CTimeStamp &from, const CTimeStamp &to) const;
    set<string> ListAddr(const CTimeStamp &from, const CTimeStamp &to) const;
    bool findFolder(const string & folderName, vector<CMailFolder *>::iterator & it);
    bool findFolder(const string & folderName, vector<CMailFolder *>::const_iterator & it) const;
private:
    CMailFolder * m_InboxFolder;
    vector<CMailFolder *> m_MailFolders;
};

CMailBox::CMailBox(void):m_InboxFolder(new CMailFolder("inbox")){
    m_MailFolders.push_back(m_InboxFolder);
}

CMailBox::~CMailBox(){
    for (auto it : m_MailFolders) {
        delete it;
    }
}

bool CMailBox::Delivery(const CMail &mail) {
    return m_InboxFolder->Delivery(mail);
}

bool CMailBox::NewFolder(const string &folderName) {
    CMailFolder * newMailFolderPtr = new CMailFolder(folderName);
    vector<CMailFolder *>::iterator folderIt;
    if(!findFolder(folderName,folderIt)){
        m_MailFolders.insert(folderIt, newMailFolderPtr);
        return true;
    }
    delete newMailFolderPtr;
    return false;
}

bool CMailBox::MoveMail(const string &fromFolder, const string &toFolder) {
    vector<CMailFolder *>::iterator fromFolderIt;
    vector<CMailFolder *>::iterator toFolderIt;
    if (findFolder(fromFolder,fromFolderIt) && findFolder(toFolder,toFolderIt)){
        (*toFolderIt)->MergeMailFolder(*fromFolderIt);
        return true;
    }
    return false;
}

list<CMail> CMailBox::ListMail(const string &folderName, const CTimeStamp &from, const CTimeStamp &to) const {
    list<CMail> mails;
    vector<CMailFolder *>::const_iterator mailFolderIt;
    bool found = findFolder(folderName, mailFolderIt);
    if (found){
        mails = (*mailFolderIt)->ListMail(from,to);
    }
    return mails;
}

set<string> CMailBox::ListAddr(const CTimeStamp &from, const CTimeStamp &to) const {
    set<string> addresses;
    for (auto m_MailFolder : m_MailFolders) {
        set<string> addr = m_MailFolder->ListAddr(from,to);
        addresses.insert(addr.begin(),addr.end());
    }
    return addresses;
}

bool CMailBox::findFolder(const string &folderName, vector<CMailFolder *>::iterator & it){
    CMailFolder tmpFolder(folderName);
    it = lower_bound(m_MailFolders.begin(), m_MailFolders.end(), &tmpFolder, cmpMailFolder);
    return it != m_MailFolders.end() && (*it)->Name() == folderName;
}

bool CMailBox::findFolder(const string &folderName, vector<CMailFolder *>::const_iterator &it) const {
    CMailFolder tmpFolder(folderName);
    it = lower_bound(m_MailFolders.begin(), m_MailFolders.end(), &tmpFolder, cmpMailFolder);
    return it != m_MailFolders.end() && (*it)->Name() == folderName;
}

////====================================================================================================================
#ifndef __PROGTEST__

static string showMail(const list<CMail> &l) {
    ostringstream oss;
    for (const auto &x : l)
        oss << x << endl;
    return oss.str();
}

static string showUsers(const set<string> &s) {
    ostringstream oss;
    for (const auto &x : s)
        oss << x << endl;
    return oss.str();
}

int main(void) {
    list<CMail> mailList;
    set<string> users;
    CAttach *att;

    CMailBox m0;
    assert (m0.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 15, 24, 13), "user1@fit.cvut.cz", CMailBody(14, "mail content 1"), nullptr)));
    assert (m0.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 15, 26, 23), "user2@fit.cvut.cz", CMailBody(22, "some different content"),
                  nullptr)));
    att = new CAttach(200);
    assert (m0.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 11, 23, 43), "boss1@fit.cvut.cz", CMailBody(14, "urgent message"), att)));
    assert (m0.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 18, 52, 27), "user1@fit.cvut.cz", CMailBody(14, "mail content 2"), att)));
    att->Release();
    att = new CAttach(97);
    assert (m0.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 16, 12, 48), "boss1@fit.cvut.cz", CMailBody(24, "even more urgent message"),
                  att)));
    att->Release();
    assert (showMail(m0.ListMail("inbox",
                                 CTimeStamp(2000, 1, 1, 0, 0, 0),
                                 CTimeStamp(2050, 12, 31, 23, 59, 59))) ==
            "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n"
            "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n"
            "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n");
    assert (showMail(m0.ListMail("inbox",
                                 CTimeStamp(2014, 3, 31, 15, 26, 23),
                                 CTimeStamp(2014, 3, 31, 16, 12, 48))) ==
            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n");

    assert (showUsers(m0.ListAddr(CTimeStamp(2000, 1, 1, 0, 0, 0),
                                  CTimeStamp(2050, 12, 31, 23, 59, 59))) == "boss1@fit.cvut.cz\n"
                                                                            "user1@fit.cvut.cz\n"
                                                                            "user2@fit.cvut.cz\n");
    assert (showUsers(m0.ListAddr(CTimeStamp(2014, 3, 31, 15, 26, 23),
                                  CTimeStamp(2014, 3, 31, 16, 12, 48))) == "boss1@fit.cvut.cz\n"
                                                                           "user2@fit.cvut.cz\n");

    CMailBox m1;
    assert (m1.NewFolder("work"));
    assert (m1.NewFolder("spam"));
    assert (!m1.NewFolder("spam"));
    assert (m1.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 15, 24, 13), "user1@fit.cvut.cz", CMailBody(14, "mail content 1"), nullptr)));
    att = new CAttach(500);
    assert (m1.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 15, 26, 23), "user2@fit.cvut.cz", CMailBody(22, "some different content"),
                  att)));
    att->Release();
    assert (m1.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 11, 23, 43), "boss1@fit.cvut.cz", CMailBody(14, "urgent message"), nullptr)));
    att = new CAttach(468);
    assert (m1.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 18, 52, 27), "user1@fit.cvut.cz", CMailBody(14, "mail content 2"), att)));
    att->Release();
    assert (m1.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 16, 12, 48), "boss1@fit.cvut.cz", CMailBody(24, "even more urgent message"),
                  nullptr)));
    assert (showMail(m1.ListMail("inbox",
                                 CTimeStamp(2000, 1, 1, 0, 0, 0),
                                 CTimeStamp(2050, 12, 31, 23, 59, 59))) ==
            "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
            "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
            "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n");
    assert (showMail(m1.ListMail("work",
                                 CTimeStamp(2000, 1, 1, 0, 0, 0),
                                 CTimeStamp(2050, 12, 31, 23, 59, 59))) == "");
    assert (m1.MoveMail("inbox", "work"));
    assert (showMail(m1.ListMail("inbox",
                                 CTimeStamp(2000, 1, 1, 0, 0, 0),
                                 CTimeStamp(2050, 12, 31, 23, 59, 59))) == "");
    assert (showMail(m1.ListMail("work",
                                 CTimeStamp(2000, 1, 1, 0, 0, 0),
                                 CTimeStamp(2050, 12, 31, 23, 59, 59))) ==
            "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
            "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
            "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n");
    assert (m1.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 19, 24, 13), "user2@fit.cvut.cz", CMailBody(14, "mail content 4"), nullptr)));
    att = new CAttach(234);
    assert (m1.Delivery(
            CMail(CTimeStamp(2014, 3, 31, 13, 26, 23), "user3@fit.cvut.cz", CMailBody(9, "complains"), att)));
    att->Release();
    assert (showMail(m1.ListMail("inbox",
                                 CTimeStamp(2000, 1, 1, 0, 0, 0),
                                 CTimeStamp(2050, 12, 31, 23, 59, 59))) ==
            "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
            "2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n");
    assert (showMail(m1.ListMail("work",
                                 CTimeStamp(2000, 1, 1, 0, 0, 0),
                                 CTimeStamp(2050, 12, 31, 23, 59, 59))) ==
            "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
            "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
            "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n");
    assert (m1.MoveMail("inbox", "work"));
    assert (showMail(m1.ListMail("inbox",
                                 CTimeStamp(2000, 1, 1, 0, 0, 0),
                                 CTimeStamp(2050, 12, 31, 23, 59, 59))) == "");
    assert (showMail(m1.ListMail("work",
                                 CTimeStamp(2000, 1, 1, 0, 0, 0),
                                 CTimeStamp(2050, 12, 31, 23, 59, 59))) ==
            "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
            "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
            "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
            "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n"
            "2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n");
    return 0;
}

#endif /* __PROGTEST__ */
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

using namespace std;
#endif /* __PROGTEST__ */

const int INIT_MEMORY_SIZE = 64;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////// CMyStr
class CMyStr {
public:
    CMyStr();
    CMyStr(const char * src);
    CMyStr(CMyStr & src);
    ~CMyStr();
    char * c_str() const;
private:
    char * m_Data;
    size_t m_Len;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////// CMyStr
CMyStr::CMyStr():m_Data(nullptr),m_Len(0){}
CMyStr::CMyStr(const char * src): m_Data(new char [strlen(src) + 1]), m_Len(strlen(src)){
    memcpy(m_Data, src, strlen(src) + 1);
}
CMyStr::CMyStr(CMyStr & src):m_Data(new char [strlen(src.m_Data) + 1]),m_Len(src.m_Len) { // deep copy
    memcpy(m_Data, src.m_Data, strlen(src.m_Data) + 1);
}
CMyStr::~CMyStr() {
    delete [] m_Data;
}
char * CMyStr::c_str() const {
    return m_Data;
}

////////////////////////////////////////////////////////////////////////////////////////////////// CTransaction & CTrans
class CTransaction {
public:
    CTransaction();
    CTransaction(const char *debAccID, const char *credAccID, unsigned int amount, const char *signature);
    CTransaction(const CTransaction & x);
    ~CTransaction();
    CTransaction & operator = (const CTransaction & x);
    const char * getDebitID();
    const char * getCreditID();
    unsigned int getAmount();
    const char * getSignature();
private:
    struct CTrans{
        CTrans();
        CTrans(CTrans & src);
        CTrans(const char *debAccID, const char *credAccID, unsigned int amount, const char *signature);
        ~CTrans();
        CMyStr m_DebAccID;      // Account from which the transaction comes
        CMyStr m_CredAccId;     // Account to which the transaction goes to
        unsigned int m_Amount;  // Amount of money being transferred;
        CMyStr m_Signature;     // Signature
        size_t m_RefCount;  // Number of references to this transaction
    };
    CTrans * m_TransData;
    void attach (CTrans * p);
    void detach();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////// CTrans

CTransaction::CTrans::CTrans()
:m_DebAccID(),m_CredAccId(),m_Amount(0),m_Signature(),m_RefCount(1)
{}

CTransaction::CTrans::CTrans(const char *debAccID, const char *credAccID, unsigned int amount, const char *signature)
:m_DebAccID(debAccID),m_CredAccId(credAccID),m_Amount(amount),m_Signature(signature),m_RefCount(1){}

CTransaction::CTrans::CTrans(CTransaction::CTrans & src)
:m_DebAccID(src.m_DebAccID),m_CredAccId(src.m_CredAccId),m_Amount(src.m_Amount),m_Signature(src.m_Signature),m_RefCount(1)
{}

CTransaction::CTrans::~CTrans(){}

/////////////////////////////////////////////////////////////////////////////////////////////////////////// CTransaction

CTransaction::CTransaction(){
    m_TransData = new CTrans();
}
CTransaction::CTransaction(const char *debAccID, const char *credAccID, unsigned int amount, const char *signature){
    m_TransData = new CTrans(debAccID, credAccID, amount, signature);
}
CTransaction::CTransaction(const CTransaction &x) {
    attach(x.m_TransData);
}
CTransaction::~CTransaction() {
    detach();
}
CTransaction &CTransaction::operator=(const CTransaction &x) {
    if (this == &x) return *this;
    detach ( );
    attach (x.m_TransData);
    return *this;
}
void CTransaction::attach(CTransaction::CTrans * p) {
    m_TransData = p;
    m_TransData->m_RefCount++;
}
void CTransaction::detach() {
    if (--m_TransData->m_RefCount == 0)
        delete m_TransData;
}
const char *CTransaction::getDebitID() {
    return m_TransData->m_DebAccID.c_str();
}
const char *CTransaction::getCreditID() {
    return m_TransData->m_CredAccId.c_str();
}
unsigned int CTransaction::getAmount() {
    return m_TransData->m_Amount;
}
const char *CTransaction::getSignature() {
    return m_TransData->m_Signature.c_str();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////// CAccount & CAcc
class CAccount {
public:
    CAccount();
    CAccount(const char * accID, int initialBalance);
    CAccount(const CAccount & x);
    ~CAccount();
    int Balance();
    CAccount & operator = (const CAccount & x);
    friend ostream & operator << (ostream & os, const CAccount & x);
    void TrimAccount();
    void AddTransaction(const char *debAccID, const char *credAccID, unsigned int amount, const char *signature);
    const char * getAccID();
private:
    struct CAcc{
        CAcc();
        CAcc(CAcc & src);
        CAcc(const char *accID, int initialBalance);
        ~CAcc();
        void getMoreMemory();
        CMyStr m_AccID;
        int m_InitialBalance;
        int m_CurrentBalance;
        CTransaction * m_Transactions;
        size_t m_NumOfTransactions;
        size_t m_MaxSize;
        size_t m_RefCount;
    };
    CAcc * m_AccData;
    void attach(CAcc * p);
    void detach();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////// CAccount:CAcc

CAccount::CAcc::CAcc()
:m_AccID(),m_InitialBalance(0),m_CurrentBalance(0),
m_Transactions(new CTransaction [INIT_MEMORY_SIZE]),m_NumOfTransactions(0),m_MaxSize(INIT_MEMORY_SIZE), m_RefCount(1)
{}

CAccount::CAcc::CAcc(CAcc & src)
:m_AccID(src.m_AccID), m_InitialBalance(src.m_InitialBalance), m_CurrentBalance(src.m_CurrentBalance),
m_Transactions(new CTransaction [src.m_MaxSize]),m_NumOfTransactions(src.m_NumOfTransactions),
m_MaxSize(src.m_MaxSize),m_RefCount(src.m_RefCount){
    for (size_t i = 0; i < src.m_NumOfTransactions; ++i) {
        m_Transactions[i] = CTransaction(src.m_Transactions[i]);
    }
}

CAccount::CAcc::CAcc(const char *accID, int initialBalance)
:m_AccID(accID), m_InitialBalance(initialBalance), m_CurrentBalance(initialBalance),
m_Transactions(new CTransaction [INIT_MEMORY_SIZE]),m_NumOfTransactions(0),m_MaxSize(INIT_MEMORY_SIZE), m_RefCount(1)
{}

CAccount::CAcc::~CAcc() {
    delete [] m_Transactions;
}

void CAccount::CAcc::getMoreMemory() {
    m_MaxSize *= 2;
    CTransaction * tmp = new CTransaction [m_MaxSize];
    for (size_t i = 0; i < m_NumOfTransactions; ++i) {
        tmp[i] = m_Transactions[i];
    }
    delete [] m_Transactions;
    m_Transactions = tmp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// CAccount
CAccount::CAccount(){
    m_AccData = new CAcc();
}

CAccount::CAccount(const char *accID, int initialBalance)
: m_AccData(new CAcc(accID, initialBalance)){}

CAccount::CAccount(const CAccount &x) {
    attach(x.m_AccData);
}

CAccount::~CAccount(){
    detach();
}

int CAccount::Balance() {
    return m_AccData->m_CurrentBalance;
}

ostream &operator << (ostream & os, const CAccount & x) {
    os << x.m_AccData->m_AccID.c_str() << ":" << endl;
    os << "   " << x.m_AccData->m_InitialBalance << endl;
    for (size_t i = 0; i < x.m_AccData->m_NumOfTransactions; ++i) {
        if (strcmp(x.m_AccData->m_Transactions[i].getDebitID(), x.m_AccData->m_AccID.c_str()) == 0){
            os << " - " << x.m_AccData->m_Transactions[i].getAmount();
            os << ", to: " << x.m_AccData->m_Transactions[i].getCreditID();
            os << ", sign: " << x.m_AccData->m_Transactions[i].getSignature() << endl;
        } else {
            os << " + " << x.m_AccData->m_Transactions[i].getAmount();
            os << ", from: " << x.m_AccData->m_Transactions[i].getDebitID();
            os << ", sign: " << x.m_AccData->m_Transactions[i].getSignature() << endl;
        }
    }
    os << " = " << x.m_AccData->m_CurrentBalance << endl;
    return os;
}

void CAccount::TrimAccount() {
    m_AccData->m_InitialBalance = m_AccData->m_CurrentBalance;
    delete [] m_AccData->m_Transactions;
    m_AccData->m_NumOfTransactions = 0;
    m_AccData->m_MaxSize = INIT_MEMORY_SIZE;
    m_AccData->m_Transactions = new CTransaction [m_AccData->m_MaxSize];
}

void CAccount::AddTransaction(const char *debAccID, const char *credAccID, unsigned int amount, const char *signature) {
    if (m_AccData->m_RefCount > 1){
        CAcc * tmp = m_AccData;
        detach();
        m_AccData = new CAcc(*tmp);
    }
    m_AccData->m_Transactions[m_AccData->m_NumOfTransactions] = CTransaction(debAccID,credAccID,amount,signature);
    m_AccData->m_NumOfTransactions++;
    if (m_AccData->m_NumOfTransactions >= m_AccData->m_MaxSize){
        m_AccData->getMoreMemory();
    }
    if (strcmp(debAccID, m_AccData->m_AccID.c_str()) == 0)
        m_AccData->m_CurrentBalance -= (int)amount;
    else
        m_AccData->m_CurrentBalance += (int)amount;
}

CAccount &CAccount::operator = (const CAccount & x){
    if (this == &x) return *this;
    detach ( );
    attach (x.m_AccData);
    return *this;
}

void CAccount::attach(CAccount::CAcc *p) {
    m_AccData = p;
    m_AccData->m_RefCount++;
}

void CAccount::detach() {
    if (--m_AccData->m_RefCount == 0)
        delete m_AccData;
}

const char *CAccount::getAccID() {
    return m_AccData->m_AccID.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////// CBank & CBankData
class CBank {
public:
    CBank();
    CBank(CBank &);
    ~CBank();
    CBank & operator = (const CBank &);
    bool NewAccount(const char *accID, int initialBalance);
    bool Transaction(const char *debAccID, const char *credAccID, unsigned int amount, const char *signature);
    bool TrimAccount(const char *accID);
    CAccount & Account(const char *accID);
    int FindAccount(const char *accID);
private:
    struct CBankData {
        CBankData();
        CBankData(CBankData &);
        ~CBankData();
        void getMoreMemory();
        CAccount * m_Accounts; // pointer to saved account pointers
        size_t m_NumOfAccounts; // current amount of saved accounts
        size_t m_MaxSize; // maximum number of saved accounts
        size_t m_RefCount; // Number of references to this bank
    };
    CBankData * m_BankData;
    void attach(CBankData * p);
    void detach();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////// CBank::CBankData
CBank::CBankData::CBankData()
:m_Accounts(new CAccount [INIT_MEMORY_SIZE]),m_NumOfAccounts(0),m_MaxSize(INIT_MEMORY_SIZE),m_RefCount(1){}

CBank::CBankData::CBankData(CBankData & src)
:m_Accounts(new CAccount [src.m_MaxSize]),m_NumOfAccounts(src.m_NumOfAccounts),m_MaxSize(src.m_MaxSize),m_RefCount(src.m_RefCount){
    for (size_t i = 0; i < src.m_NumOfAccounts; ++i) {
        m_Accounts[i] = CAccount(src.m_Accounts[i]);
    }
}

CBank::CBankData::~CBankData(){
    delete [] m_Accounts;
}

void CBank::CBankData::getMoreMemory() {
    m_MaxSize *= 2;
    CAccount * tmp = new CAccount [m_MaxSize];
    for (size_t i = 0; i < m_NumOfAccounts; ++i) {
        tmp[i] = this->m_Accounts[i];
    }
    delete [] m_Accounts;
    m_Accounts = tmp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CBank
CBank::CBank(){
    m_BankData = new CBankData();
}

CBank::CBank(CBank & x){
    attach(x.m_BankData);
}

CBank::~CBank() {
    detach();
}

CBank & CBank::operator = (const CBank & x) {
    if (this == &x)
        return *this;
    detach();
    attach(x.m_BankData);
    return *this;
}

bool CBank::NewAccount(const char *accID, int initialBalance) {
    if (FindAccount(accID) != -1)
        return false;
    if (m_BankData->m_RefCount > 1){
        CBankData * tmp = m_BankData;
        detach();
        m_BankData = new CBankData(*tmp);
    }
    m_BankData->m_Accounts[m_BankData->m_NumOfAccounts] = CAccount(accID,initialBalance);
    m_BankData->m_NumOfAccounts++;
    if (m_BankData->m_NumOfAccounts >= m_BankData->m_MaxSize)
        m_BankData->getMoreMemory();
    return true;
}
bool CBank::Transaction(const char *debAccID, const char *credAccID, unsigned int amount, const char *signature) { // todo
    if(strcmp(debAccID,credAccID) == 0)
    return false;
    int i = FindAccount(debAccID);
    int j = FindAccount(credAccID);
    if (i == -1 || j == -1) {
        return false;
    }
    if (m_BankData->m_RefCount > 1){
        CBankData * tmp = m_BankData;
        detach();
        m_BankData = new CBankData(*tmp);
    }
    m_BankData->m_Accounts[i].AddTransaction(debAccID,credAccID,amount,signature);
    m_BankData->m_Accounts[j].AddTransaction(debAccID,credAccID,amount,signature);
    return true;
}
bool CBank::TrimAccount(const char *accID) {
    int i = FindAccount(accID);
    if (i == -1)
        return false;
    m_BankData->m_Accounts[i].TrimAccount();
    return true;
}
CAccount &CBank::Account(const char *accID) {
    int i = FindAccount(accID);
    if (i == -1)
        throw "Account not found.";
    return m_BankData->m_Accounts[i];
}
int CBank::FindAccount(const char *accID) {
    for (size_t i = 0; i < m_BankData->m_NumOfAccounts; ++i) {
        if (strcmp(m_BankData->m_Accounts[i].getAccID(), accID) == 0 )
            return i;
    }
    return -1;
}
void CBank::attach(CBank::CBankData *p) {
    m_BankData = p;
    m_BankData->m_RefCount++;
}
void CBank::detach() {
    if (--m_BankData->m_RefCount == 0)
        delete m_BankData;
}

#ifndef __PROGTEST__

int main(void) {
    ostringstream os;
    char accCpy[100], debCpy[100], credCpy[100], signCpy[100];
    CBank x0;
    assert (x0.NewAccount("123456", 1000));
    assert (x0.NewAccount("987654", -500));
    assert (x0.Transaction("123456", "987654", 300, "XAbG5uKz6E="));
    assert (x0.Transaction("123456", "987654", 2890, "AbG5uKz6E="));
    assert (x0.NewAccount("111111", 5000));
    assert (x0.Transaction("111111", "987654", 290, "Okh6e+8rAiuT5="));
    assert (x0.Account("123456").Balance() == -2190);
    assert (x0.Account("987654").Balance() == 2980);
    assert (x0.Account("111111").Balance() == 4710);
    os.str("");
    os << x0.Account("123456");
    assert (!strcmp(os.str().c_str(),
                    "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n"));
    os.str("");
    os << x0.Account("987654");
    assert (!strcmp(os.str().c_str(),
                    "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 290, from: 111111, sign: Okh6e+8rAiuT5=\n = 2980\n"));
    os.str("");
    os << x0.Account("111111");
    assert (!strcmp(os.str().c_str(), "111111:\n   5000\n - 290, to: 987654, sign: Okh6e+8rAiuT5=\n = 4710\n"));
    assert (x0.TrimAccount("987654"));
    assert (x0.Transaction("111111", "987654", 123, "asdf78wrnASDT3W"));
    os.str("");
    os << x0.Account("987654");
    assert (!strcmp(os.str().c_str(), "987654:\n   2980\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 3103\n"));

    CBank x2;
    strncpy(accCpy, "123456", sizeof(accCpy));
    assert (x2.NewAccount(accCpy, 1000));
    strncpy(accCpy, "987654", sizeof(accCpy));
    assert (x2.NewAccount(accCpy, -500));
    strncpy(debCpy, "123456", sizeof(debCpy));
    strncpy(credCpy, "987654", sizeof(credCpy));
    strncpy(signCpy, "XAbG5uKz6E=", sizeof(signCpy));
    assert (x2.Transaction(debCpy, credCpy, 300, signCpy));
    strncpy(debCpy, "123456", sizeof(debCpy));
    strncpy(credCpy, "987654", sizeof(credCpy));
    strncpy(signCpy, "AbG5uKz6E=", sizeof(signCpy));
    assert (x2.Transaction(debCpy, credCpy, 2890, signCpy));
    strncpy(accCpy, "111111", sizeof(accCpy));
    assert (x2.NewAccount(accCpy, 5000));
    strncpy(debCpy, "111111", sizeof(debCpy));
    strncpy(credCpy, "987654", sizeof(credCpy));
    strncpy(signCpy, "Okh6e+8rAiuT5=", sizeof(signCpy));
    assert (x2.Transaction(debCpy, credCpy, 2890, signCpy));
    assert (x2.Account("123456").Balance() == -2190);
    assert (x2.Account("987654").Balance() == 5580);
    assert (x2.Account("111111").Balance() == 2110);
    os.str("");
    os << x2.Account("123456");
    assert (!strcmp(os.str().c_str(),
                    "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n"));
    os.str("");
    os << x2.Account("987654");
    assert (!strcmp(os.str().c_str(),
                    "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n = 5580\n"));
    os.str("");
    os << x2.Account("111111");
    assert (!strcmp(os.str().c_str(), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n = 2110\n"));
    assert (x2.TrimAccount("987654"));
    strncpy(debCpy, "111111", sizeof(debCpy));
    strncpy(credCpy, "987654", sizeof(credCpy));
    strncpy(signCpy, "asdf78wrnASDT3W", sizeof(signCpy));
    assert (x2.Transaction(debCpy, credCpy, 123, signCpy));
    os.str("");
    os << x2.Account("987654");
    assert (!strcmp(os.str().c_str(), "987654:\n   5580\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n"));

    CBank x4;
    assert (x4.NewAccount("123456", 1000));
    assert (x4.NewAccount("987654", -500));
    assert (!x4.NewAccount("123456", 3000));
    assert (!x4.Transaction("123456", "666", 100, "123nr6dfqkwbv5"));
    assert (!x4.Transaction("666", "123456", 100, "34dGD74JsdfKGH"));
    assert (!x4.Transaction("123456", "123456", 100, "Juaw7Jasdkjb5"));
    try {
        x4.Account("666").Balance();
        assert ("Missing exception !!" == NULL);
    }
    catch (...) {
    }
    try {
        os << x4.Account("666").Balance();
        assert ("Missing exception !!" == NULL);
    }
    catch (...) {
    }
    assert (!x4.TrimAccount("666"));

    CBank x6;
    assert (x6.NewAccount("123456", 1000));
    assert (x6.NewAccount("987654", -500));
    assert (x6.Transaction("123456", "987654", 300, "XAbG5uKz6E="));
    assert (x6.Transaction("123456", "987654", 2890, "AbG5uKz6E="));
    assert (x6.NewAccount("111111", 5000));
    assert (x6.Transaction("111111", "987654", 2890, "Okh6e+8rAiuT5="));
    CBank x7(x6);
    assert (x6.Transaction("111111", "987654", 123, "asdf78wrnASDT3W"));
    assert (x7.Transaction("111111", "987654", 789, "SGDFTYE3sdfsd3W"));
    assert (x6.NewAccount("99999999", 7000));
    assert (x6.Transaction("111111", "99999999", 3789, "aher5asdVsAD"));
    assert (x6.TrimAccount("111111"));
    assert (x6.Transaction("123456", "111111", 221, "Q23wr234ER=="));
    os.str("");
    os << x6.Account("111111");
    assert (!strcmp(os.str().c_str(), "111111:\n   -1802\n + 221, from: 123456, sign: Q23wr234ER==\n = -1581\n"));
    os.str("");
    os << x6.Account("99999999");
    assert (!strcmp(os.str().c_str(), "99999999:\n   7000\n + 3789, from: 111111, sign: aher5asdVsAD\n = 10789\n"));
    os.str("");
    os << x6.Account("987654");
    assert (!strcmp(os.str().c_str(),
                    "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n"));
    os.str("");
    os << x7.Account("111111");
    assert (!strcmp(os.str().c_str(),
                    "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n"));
    try {
        os << x7.Account("99999999").Balance();
        assert ("Missing exception !!" == NULL);
    }
    catch (...) {
    }
    os.str("");
    os << x7.Account("987654");
    assert (!strcmp(os.str().c_str(),
                    "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 789, from: 111111, sign: SGDFTYE3sdfsd3W\n = 6369\n"));

    CBank x8;
    CBank x9;
    assert (x8.NewAccount("123456", 1000));
    assert (x8.NewAccount("987654", -500));
    assert (x8.Transaction("123456", "987654", 300, "XAbG5uKz6E="));
    assert (x8.Transaction("123456", "987654", 2890, "AbG5uKz6E="));
    assert (x8.NewAccount("111111", 5000));
    assert (x8.Transaction("111111", "987654", 2890, "Okh6e+8rAiuT5="));
    x9 = x8;
    assert (x8.Transaction("111111", "987654", 123, "asdf78wrnASDT3W"));
    assert (x9.Transaction("111111", "987654", 789, "SGDFTYE3sdfsd3W"));
    assert (x8.NewAccount("99999999", 7000));
    assert (x8.Transaction("111111", "99999999", 3789, "aher5asdVsAD"));
    assert (x8.TrimAccount("111111"));
    os.str("");
    os << x8.Account("111111");
    assert (!strcmp(os.str().c_str(), "111111:\n   -1802\n = -1802\n"));
    os.str("");
    os << x9.Account("111111");
    assert (!strcmp(os.str().c_str(),
                    "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n"));

    return 0;
}

#endif /* __PROGTEST__ */
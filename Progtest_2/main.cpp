#ifndef __PROGTEST__
//////////////////////////////////////////////////////////////////////////////////////////////////////////////// Include
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////////////////////////////////// Namespace
using namespace std;
#endif /* __PROGTEST__ */
///////////////////////////////////////////////////////////////////////////////////////////////////////// SPerson struct
/* SPerson struct
 * Encapsulates data about a person.
 */
struct SPerson{
    /* SPerson struct constructor
     * Sets up the a person data structure.
     * @param[in] name Persons name.
     * @param[in] addr Persons address.
     * @param[in] account Persons account.
     */
    SPerson(const string &name, const string &addr, const string &account);
    string m_Name;
    string m_Address;
    string m_Account;
    int m_IncomeSum;
    int m_ExpenseSum;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////// SPerson methods
SPerson::SPerson(const string &name, const string &addr, const string &account)
:m_Name(name), m_Address(addr), m_Account(account), m_IncomeSum(0), m_ExpenseSum(0){}
/////////////////////////////////////////////////////////////////////////////////////////// SPerson comparator functions
/* SPerson name and address comparator function. Serves for comparing people with lower_bound().
 */
bool cmpNameAddress(const SPerson * a, const SPerson * b){
    return a->m_Name == b->m_Name ? (a->m_Address < b->m_Address) : a->m_Name < b->m_Name;
}
/* SPerson account comparator function. Serves for comparing people with lower_bound().
 */
bool cmpAccount(const SPerson * a, const SPerson * b){
    return a->m_Account < b->m_Account;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////// CIterator class
/* CIterator class
 * Represents an iterator which traverses the tax register database.
 */
class CIterator {
public:
    /* CIterator class constructor.
     * @param[in] data Vector of SPerson pointers sorted by name and address.
     */
    CIterator(const vector<SPerson*>& data);
    /* Indicates whether the iterator has reached the end.
     * @return True if the iterator is at the end of the tax register, false otherwise.
     */
    bool AtEnd(void) const;
    /* Moves the iterator by one position forward.
     */
    void Next(void);
    /* @return Returns the name of the person being pointed at by the iterator.
     */
    string Name(void) const;
    /* @return Returns the address of the person being pointed at by the iterator.
     */
    string Addr(void) const;
    /* @return Returns the account of the person being pointed at by the iterator.
     */
    string Account(void) const;
private:
    const vector<SPerson*>& m_Database;
    vector<SPerson*>::const_iterator m_Iterator;
};

/////////////////////////////////////////////////////////////////////////////////////////// CIterator method definitions
CIterator::CIterator(const vector<SPerson *>& data): m_Database(data){
    m_Iterator = m_Database.begin();
}
bool CIterator::AtEnd(void) const {
    return m_Iterator == m_Database.end();
}
void CIterator::Next(void) {
    m_Iterator++;
}
string CIterator::Name(void) const {
    return (*m_Iterator)->m_Name;
}
string CIterator::Addr(void) const {
    return (*m_Iterator)->m_Address;
}
string CIterator::Account(void) const {
    return (*m_Iterator)->m_Account;
}
///////////////////////////////////////////////////////////////////////////////////////////////////// CTaxRegister class
/* CTaxRegister class
 * Represents a database of people and their accounts.
 */
class CTaxRegister {
public:
    /* CTaxRegister destructor. Used to free used memory.
     */
    ~CTaxRegister();
    /* Adds a person to the database.
     * @param[in] name Name of the person.
     * @param[in] addr Address of the person.
     * @param[in] account Account of the person.
     * @return True if the person was added without any issues. False otherwise.
     */
    bool Birth(const string &name, const string &addr, const string &account);
    /* Removes a person from the database.
     * @param[in] name Name of the person.
     * @param[in] addr Address of the person.
     * @return True if the person was successfully deleted. False otherwise.
     */
    bool Death(const string &name, const string &addr);
    /* Adds an amount of income to a person specified by the account parameter.
     * @param[in] account Account's name.
     * @param[in] amount Amount to be added to account's income.
     * @return True if the amount was successfully added. False otherwise.
     */
    bool Income(const string &account, int amount);
    /* Adds an amount of income to a person specified by the name and address parameters.
     * @param[in] name Person's name.
     * @param[in] addr Person's address.
     * @param[in] amount Amount to be added to account's income.
     * @return True if the amount was successfully added. False otherwise.
     */
    bool Income(const string &name, const string &addr, int amount);
    /* Adds an amount of expense to a person specified by the account parameter.
     * @param[in] account Account's name.
     * @param[in] amount Amount to be added to account's expense.
     * @return True if the amount was successfully added. False otherwise.
     */
    bool Expense(const string &account, int amount);
    /* Adds an amount of expense to a person specified by the name and address parameters.
     * @param[in] name Person's name.
     * @param[in] addr Person's address.
     * @param[in] amount Amount to be added to account's expense.
     * @return True if the amount was successfully added. False otherwise.
     */
    bool Expense(const string &name, const string &addr, int amount);
    /* Looks up a person by their name and address. Saves the rest of the information to the output parameters.
     * @param[in] name Searched person's name.
     * @param[in] addr Searched person's address.
     * @param[out] account Searched person's account name.
     * @param[out] sumIncome Searched person's income sum.
     * @param[out] sumExpense Searched person's expense sum.
     * @return True if the person was found. False otherwise.
     */
    bool Audit(const string &name, const string &addr, string &account, int &sumIncome, int &sumExpense) const;
    /* @return Returns an iterator to iterate through the tax register.
     */
    CIterator ListByName(void) const;
    /* Finds a person in the register by their name and address.
     * @param[in] name Searched person's name.
     * @param[in] addr Searched person's address.
     * @return Pointer to the found person. Nullptr if the person wasn't found.
     */
    SPerson * Find(const string &name, const string &addr) const;
    /* Finds a person in the register by their account.
     * @param[in] account Searched person's account.
     * @return Pointer to the found person. Nullptr if the person wasn't found.
     */
    SPerson * Find(const string &account) const ;
private:
    vector<SPerson*> m_NameAddrDataSorted;
    vector<SPerson*> m_AccountDataSorted;
};
/////////////////////////////////////////////////////////////////////////////////////////////////// CTaxRegister Methods
CTaxRegister::~CTaxRegister() {
    for (auto it : m_AccountDataSorted) {
        delete it;
    }
}

bool CTaxRegister::Birth(const string &name, const string &addr, const string &account) {
    SPerson * person = new SPerson(name,addr,account);
    auto nameAddrIterator = lower_bound(m_NameAddrDataSorted.begin(), m_NameAddrDataSorted.end(), person, cmpNameAddress);
    if (nameAddrIterator != m_NameAddrDataSorted.end() && // lower_bound found something
        (*nameAddrIterator)->m_Name == person->m_Name && // compare the name of the found found thing
        (*nameAddrIterator)->m_Address == person->m_Address){ // compare the address of the found thing
        delete person;
        return false;
    }
    auto accountIterator = lower_bound(m_AccountDataSorted.begin(), m_AccountDataSorted.end(), person, cmpAccount);
    if (accountIterator != m_AccountDataSorted.end() && // lower_bound found somthing
        (*accountIterator)->m_Account == person->m_Account){ // compare account name of the found thing
        delete person;
        return false;
    }
    m_NameAddrDataSorted.insert(nameAddrIterator, person);
    m_AccountDataSorted.insert(accountIterator, person);
    return true;
}

bool CTaxRegister::Death(const string &name, const string &addr) {
    SPerson person(name,addr, "");
    auto nameIterator = lower_bound(m_NameAddrDataSorted.begin(), m_NameAddrDataSorted.end(), &person, cmpNameAddress);
    if (nameIterator == m_NameAddrDataSorted.end() || (*nameIterator)->m_Name != person.m_Name || (*nameIterator)->m_Address != person.m_Address){
        return false;
    }
    person.m_Account = (*nameIterator)->m_Account;
    auto accountIterator = lower_bound(m_AccountDataSorted.begin(), m_AccountDataSorted.end(), &person, cmpAccount);
    if (accountIterator == m_AccountDataSorted.end() || (*accountIterator)->m_Account != person.m_Account){
        return false;
    }
    delete (*accountIterator);
    m_NameAddrDataSorted.erase(nameIterator);
    m_AccountDataSorted.erase(accountIterator);
    return true;
}

bool CTaxRegister::Income(const string &account, int amount) {
    SPerson * person = Find(account);
    if (person == nullptr)
        return false;
    person->m_IncomeSum += amount;
    return true;
}

bool CTaxRegister::Income(const string &name, const string &addr, int amount) {
    SPerson * person = Find(name,addr);
    if (person == nullptr)
        return false;
    person->m_IncomeSum += amount;
    return true;
}

bool CTaxRegister::Expense(const string &account, int amount) {
    SPerson * person = Find(account);
    if(person == nullptr)
        return false;
    person->m_ExpenseSum += amount;
    return true;
}

bool CTaxRegister::Expense(const string &name, const string &addr, int amount) {
    SPerson * person = Find(name,addr);
    if (person == nullptr)
        return false;
    person->m_ExpenseSum += amount;
    return true;
}

bool CTaxRegister::Audit(const string &name, const string &addr, string &account, int &sumIncome, int &sumExpense) const {
    SPerson * person = Find(name,addr);
    if (person == nullptr)
        return false;
    account = person->m_Account;
    sumIncome = person->m_IncomeSum;
    sumExpense = person->m_ExpenseSum;
    return true;
}

CIterator CTaxRegister::ListByName(void) const {
    return CIterator(m_NameAddrDataSorted);
}
SPerson * CTaxRegister::Find(const string &name, const string &addr) const {
    SPerson person(name,addr, "");
    auto it = lower_bound(m_NameAddrDataSorted.begin(),m_NameAddrDataSorted.end(),&person,cmpNameAddress);
    if (it == m_NameAddrDataSorted.end() || (*it)->m_Name != person.m_Name || (*it)->m_Address != person.m_Address){
        return nullptr;
    }
    return (*it);
}

SPerson * CTaxRegister::Find(const string &account) const {
    SPerson person("","",account);
    auto it = lower_bound(m_AccountDataSorted.begin(), m_AccountDataSorted.end(),&person,cmpAccount);
    if (it == m_AccountDataSorted.end() || (*it)->m_Account != person.m_Account){
        return nullptr;
    }
    return (*it);
}

#ifndef __PROGTEST__

int main(void) {
    string acct;
    int sumIncome, sumExpense;
    CTaxRegister b1;
    assert (b1.Birth("John Smith", "Oak Road 23", "123/456/789"));
    assert (b1.Birth("Jane Hacker", "Main Street 17", "Xuj5#94"));
    assert (b1.Birth("Peter Hacker", "Main Street 17", "634oddT"));
    assert (b1.Birth("John Smith", "Main Street 17", "Z343rwZ"));
    assert (b1.Income("Xuj5#94", 1000));
    assert (b1.Income("634oddT", 2000));
    assert (b1.Income("123/456/789", 3000));
    assert (b1.Income("634oddT", 4000));
    assert (b1.Income("Peter Hacker", "Main Street 17", 2000));
    assert (b1.Expense("Jane Hacker", "Main Street 17", 2000));
    assert (b1.Expense("John Smith", "Main Street 17", 500));
    assert (b1.Expense("Jane Hacker", "Main Street 17", 1000));
    assert (b1.Expense("Xuj5#94", 1300));
    assert (b1.Audit("John Smith", "Oak Road 23", acct, sumIncome, sumExpense));
    assert (acct == "123/456/789");
    assert (sumIncome == 3000);
    assert (sumExpense == 0);
    assert (b1.Audit("Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense));
    assert (acct == "Xuj5#94");
    assert (sumIncome == 1000);
    assert (sumExpense == 4300);
    assert (b1.Audit("Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense));
    assert (acct == "634oddT");
    assert (sumIncome == 8000);
    assert (sumExpense == 0);
    assert (b1.Audit("John Smith", "Main Street 17", acct, sumIncome, sumExpense));
    assert (acct == "Z343rwZ");
    assert (sumIncome == 0);
    assert (sumExpense == 500);
    CIterator it = b1.ListByName();
    assert (!it.AtEnd()
            && it.Name() == "Jane Hacker"
            && it.Addr() == "Main Street 17"
            && it.Account() == "Xuj5#94");
    it.Next();
    assert (!it.AtEnd()
            && it.Name() == "John Smith"
            && it.Addr() == "Main Street 17"
            && it.Account() == "Z343rwZ");
    it.Next();
    assert (!it.AtEnd()
            && it.Name() == "John Smith"
            && it.Addr() == "Oak Road 23"
            && it.Account() == "123/456/789");
    it.Next();
    assert (!it.AtEnd()
            && it.Name() == "Peter Hacker"
            && it.Addr() == "Main Street 17"
            && it.Account() == "634oddT");
    it.Next();
    assert (it.AtEnd());
    assert (b1.Death("John Smith", "Main Street 17"));

    CTaxRegister b2;
    assert (b2.Birth("John Smith", "Oak Road 23", "123/456/789"));
    assert (b2.Birth("Jane Hacker", "Main Street 17", "Xuj5#94"));
    assert (!b2.Income("634oddT", 4000));
    assert (!b2.Expense("John Smith", "Main Street 18", 500));
    assert (!b2.Audit("John Nowak", "Second Street 23", acct, sumIncome, sumExpense));
    assert (!b2.Death("Peter Nowak", "5-th Avenue"));
    assert (!b2.Birth("Jane Hacker", "Main Street 17", "4et689A"));
    assert (!b2.Birth("Joe Hacker", "Elm Street 23", "Xuj5#94"));
    assert (b2.Death("Jane Hacker", "Main Street 17"));
    assert (b2.Birth("Joe Hacker", "Elm Street 23", "Xuj5#94"));
    assert (b2.Audit("Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense));
    assert (acct == "Xuj5#94");
    assert (sumIncome == 0);
    assert (sumExpense == 0);
    assert (!b2.Birth("Joe Hacker", "Elm Street 23", "AAj5#94"));
    return 0;
}

#endif /* __PROGTEST__ */
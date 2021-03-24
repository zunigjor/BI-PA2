#ifndef __PROGTEST__
#include <cctype>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <set>
#include <list>
#include <forward_list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <deque>
#include <optional>
#include <variant>
#include <any>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */
template<typename _Type, typename _Comparator = equal_to<typename _Type::value_type> >
class CSearch {
public:
    CSearch(){};
    CSearch(_Comparator comparator): m_Comparator(comparator){};
    CSearch(const CSearch &src) = delete;
    CSearch &operator=(const CSearch &src) = delete;
    void Add(int id, const _Type &needle){
        m_Data.insert(make_pair(id,needle));
    }
    set<int> Search(const _Type &hayHeap) const {
        set<int> result;
        for (auto & it : m_Data)
            if (search(hayHeap.begin(), hayHeap.end(), it.second.begin(), it.second.end(), m_Comparator) != hayHeap.end())
                result.insert(it.first);
        return result;
    }
private:
    _Comparator m_Comparator;
    map<int,_Type> m_Data;
};
//-------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
class CharComparator {
public:
    CharComparator(bool caseSensitive = true)
    :m_CaseSensitive(caseSensitive) {}
    bool operator()(const char &a, const char &b) const {
        return m_CaseSensitive ? a == b : toupper(a) == toupper(b);
    }
private:
    bool m_CaseSensitive;
};
//-------------------------------------------------------------------------------------------------
bool upperCaseCompare(const char &a, const char &b) {
    return toupper(a) == toupper(b);
}
//-------------------------------------------------------------------------------------------------
int main(void) {
    CSearch<string> test1;
    test1.Add(0, "hello");
    test1.Add(1, "world");
    test1.Add(2, "rld");
    test1.Add(3, "ell");
    test1.Add(4, "hell");
    test1.Search("hello world!");
    assert (test1.Search("hello world!") == (set<int>{0, 1, 2, 3, 4}));
    assert (test1.Search("hEllo world!") == (set<int>{1, 2}));

    CSearch<string, bool (*)(const char &, const char &)> test2(upperCaseCompare);
    test2.Add(0, "hello");
    test2.Add(1, "world");
    test2.Add(2, "rld");
    test2.Add(3, "ell");
    test2.Add(4, "hell");
    assert (test2.Search("HeLlO WoRlD!") == (set<int>{0, 1, 2, 3, 4}));
    assert (test2.Search("hallo world!") == (set<int>{1, 2}));

    CSearch<string, CharComparator> test3(CharComparator(false));
    test3.Add(0, "hello");
    test3.Add(1, "world");
    test3.Add(2, "rld");
    test3.Add(3, "ell");
    test3.Add(4, "hell");
    assert (test3.Search("heLLo world!") == (set<int>{0, 1, 2, 3, 4}));
    assert (test3.Search("Well, templates are hell") == (set<int>{3, 4}));

    CSearch<vector<int> > test4;
    test4.Add(0, {1, 6, 1, 6, 9, 12});
    test4.Add(1, {9, 12, 7});
    assert (test4.Search(vector<int>{1, 6, 1, 6, 1, 6, 9, 12, 8}) == (set<int>{0}));
    assert (test4.Search(vector<int>{1, 1, 6, 1, 6, 8, 12, 8}) == (set<int>{}));

    CSearch<vector<string> > test5;
    test5.Add(0, {"Prague", "Bern", "Rome"});
    test5.Add(1, {"London", "Prague", "Bern"});
    test5.Add(2, {"London", "Rome"});
    assert (test5.Search(vector<string>{"Berlin", "London", "Prague", "Bern", "Rome", "Moscow"}) == (set<int>{0, 1}));

    CSearch<deque<int> > test6;
    test6.Add(0, {1, 6, 1, 6, 9, 12});
    test6.Add(1, {9, 12, 7});
    assert (test6.Search(deque<int>{1, 6, 1, 6, 1, 6, 9, 12, 8}) == (set<int>{0}));

    CSearch<forward_list<int> > test7;
    test7.Add(0, {1, 6, 1, 6, 9, 12});
    test7.Add(1, {9, 12, 7});
    assert (test7.Search(forward_list<int>{1, 6, 1, 6, 1, 6, 9, 12, 8}) == (set<int>{0}));

    CSearch<list<int> > test8;
    test8.Add(0, {1, 6, 1, 6, 9, 12});
    test8.Add(1, {9, 12, 7});
    assert (test8.Search(list<int>{1, 6, 1, 6, 1, 6, 9, 12, 8}) == (set<int>{0}));
    return 0;
}

#endif /* __PROGTEST__ */
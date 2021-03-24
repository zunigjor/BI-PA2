#ifndef __PROGTEST__
//////// Include ////////
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
//////// Namespace ////////
using namespace std;
#endif /* __PROGTEST__ */
//////// Class declarations ////////
class CSymbol;
class CHuffTreeNode;
class CHuffFile;
//////// Class definitions ////////
/////// CSybmbol ///////
/*
 * CSymbol class
 * Takes care of reading UTF-8 symbols.
 * Holds pointer to CHuffTreeNode whose symbol data it holds.
 */
class CSymbol{
public:
    /*
     *  CSymbol constructor
     *  @param Pointer to CHuffTree node whose data it should hold.
     */
    explicit CSymbol(CHuffTreeNode *);
    /*
     * Reads a utf-8 symbol.
     */
    bool readSymbol();
    /*
     * Writes a utf-8 symbol.
     */
    void writeSymbol();
    /*
     *
     */
    bool toSymbolArr();
    /*
     * Returns the state of a bit in n-th position of the utf-8 symbol bit set represented by uint32_t.
     * @param bit position number
     * @return state of bit in the asked position/
     */
    bool getBit(unsigned int);
    /*
     * Sets the bit at the n-th position to the given boolean state.
     */
    void setBit(unsigned int, bool);
    /*
     * Loads a specific amount of bits based on the m_NumOfBytes.
     * @return true if everything went fine.
     *         false otherwise.
     */
    bool loadBits();
    /*
     * Checks if the loaded UTF-8 symbol is valid.
     * @return true valid
     *         false not valid
     */
    bool valid();
private:
    CHuffTreeNode * m_HuffTreeNode;
    int m_NumOfBytes;
    uint32_t m_SymbolVal; // (31)0000.0000/0000.0000/0000.0000/0000.0000(0)
    uint8_t m_SymbolArr [4];
};
//////// CHuffTreeNode ////////
/*
 * CHuffTreeNode class.
 * Creates an interface to save and work with a tree which holds information about the used Huffman code.
 * Also holds a pointer to the CHuffFile which this tree belongs to.
 */
class CHuffTreeNode{
public:
    explicit CHuffTreeNode(CHuffFile *);
    /*
     * Method which recursively builds a huff code tree.
     * @return true if everything went fine,
     *         false otherwise.
     */
    bool buildTree();
    /*
     * Deletes tree and frees allocated memory.
     */
    void deleteTree();
    /*
     * Checks whether the current node is a leaf.
     * Leaves do not have any children.
     */
    bool isLeaf() const;
    /*
     * Method which helps the recursion to go to the left subtree.
     * @return pointer to left subtree.
     */
    CHuffTreeNode * goLeft() const;
    /*
     * Method which helps the recursion to go to the right subtree.
     * @return pointer to right subtree.
     */
    CHuffTreeNode * goRight() const;
    /*
     * Returns a pointer to the parent CHuffFile class.
     */
    CHuffFile * getFile();
    /*
     * Returns a symbol;
     */
    CSymbol getSymbol() const;
protected:
    CSymbol m_Symbol; // Symbol which is being saved UTF-8
    CHuffFile * m_HuffFile;
    CHuffTreeNode * m_left_0;
    CHuffTreeNode * m_right_1;
};
//////// CHuffFile ////////
/*
 * CHuffFile class
 * Creates an interface for handling Huffman compression algorithm.
 */
class CHuffFile {
public:
    /*
     * Constructor takes in name of input and output files.
     * Opens them and allocates memory for Huffman code tree.
     * @param Path to input file.
     * @param Path to output file
     */
    CHuffFile(const char *, const char *);
    /*
     * Destructor closes used files.
     */
    ~CHuffFile();
    /*
     * Main method used for decompressing the huff file.
     * @return true if the file was successfully decompressed
     *         false otherwise
     */
    bool decompressFile();
    /*
     *  Method which checks both the input and output files.
     *  @return true if everything went fine
     *          false otherwise
     */
    bool checkFiles();
    /*
     * Method which takes care of building a Huffman code tree.
     * @return true if everything went fine
     *         false otherwise
     */
    bool buildTree();
    /*
     * Method which initializes the first byte.
     * @return true if everything went fine
     */
    bool initMemory();
    /*
     * Returns the state of the m_eof bit, which indicates if the last char has been read.
     * @return state of m_eof bit.
     */
    bool eof();
    /*
     * Returns the state of the m_good bit, which indicates if any file operations have failed or not.
     * @return state of m_good bit.
     */
    bool good();
    /*
     * Returns the state of the m_last bit, which indicates if the last bit has been read.
     * @return state of m_last bit.
     */
    bool last();
    /*
     * Method which takes care of decompressing the message and deciding what chunk is being read.
     */
    bool decompressMessage();
    /*
     * Method reads the next byte from input file stream and saves it into bit array with a length of 8.
     * also resets the counter of which bit is being read.
     * Checks for end of file. Sets eof flag to true when last byte was read.
     * @return returns 1 if everything went okay, 0 if file is not accessible.
     */
    bool loadNextCharToBits();
    /*
     * Method which returns the next bit. Similar to ifs.get() which works with chars.
     * This works with bits.
     * Handles errors through setting up m_good bit.
     * @return true if the next bit is 1, false if the next bit is 0.
     */
    bool getNextBit();
    /*
     * Method which recursively traverses the tree while reading bits from input file.
     * streams chars into output file
     * @return true if everything went fine,
     *         false otherwise.
     */
    bool traverseTree(const CHuffTreeNode *);
    /*
     * Method which takes care of reading a chunk of 4096 symbols.
     * @return true if everything went fine,
     *         false otherwise.
     */
    bool readBigChunk();
    /*
     * Method which takes care of reading a chunk of a specific amount of symbols given by the next 12 bits.
     * @return true if everything went fine,
     *         false otherwise.
     */
    bool readSmallChunk();
    void writeToOutput(const char);
protected:
    CHuffTreeNode * m_HuffCodeTree; // Huffman code tree
    bool m_CharInBits[8]{};
    int m_CurrentReadBit;
    bool m_EofBit;
    bool m_LastBit;
    bool m_GoodBit;
    ifstream m_InputFileStream;
    ofstream m_OutputFileStream;
    int m_NumOfSymbolsToRead;
    int m_NumOfNodes;
};


//////// CHuffFile methods ////////
CHuffFile::CHuffFile(const char * iF, const char * oF)
: m_HuffCodeTree(new CHuffTreeNode(this)),
  m_CurrentReadBit(0),
  m_EofBit(false),
  m_LastBit(false),
  m_GoodBit(true),
  m_NumOfSymbolsToRead(0),
  m_NumOfNodes(0){
    m_InputFileStream.open(iF, ios::in | ios::binary);
    m_OutputFileStream.open(oF, ios::out | ios::binary);
}
CHuffFile::~CHuffFile(){
    m_InputFileStream.close();
    m_OutputFileStream.close();
    m_HuffCodeTree->deleteTree();
}
bool CHuffFile::decompressFile(){
    if (!checkFiles())
        return false;
    if (!buildTree())
        return false;
    if (!decompressMessage())
        return false;
    return true;
}
bool CHuffFile::checkFiles(){
    return !(!m_InputFileStream || !m_InputFileStream.good() || !m_InputFileStream.is_open()
             || !m_OutputFileStream || !m_OutputFileStream.good() || !m_OutputFileStream.is_open());
}
bool CHuffFile::buildTree() {
    if (!initMemory())
        return false;
    if (!m_HuffCodeTree->buildTree())
        return false;
    return true;
}
bool CHuffFile::initMemory(){
    bool status = loadNextCharToBits();
    if (!status) // something went wrong
        return false;
    if (status && eof()) // The byte was read but its also the last byte.
        return false;
    return true;
}
bool CHuffFile::loadNextCharToBits(){
    if (!m_InputFileStream || m_InputFileStream.fail() || !m_InputFileStream.is_open()){
        m_GoodBit = false;
        return false;     // something went wrong
    }
    if (m_EofBit)
        return true;
    char c = m_InputFileStream.get();
    bool bit;
    int j = 0;
    for (int i = 7; i >= 0; --i) {
        bit = (c >> i) & 1;
        m_CharInBits[j] = bit;
        j++;
    }
    m_CurrentReadBit = 0;
    m_InputFileStream.peek();
    m_EofBit = m_InputFileStream.eof();
    return true; // everything is fine
}
bool CHuffFile::getNextBit(){
    bool nextBit;
    if (eof() && m_CurrentReadBit == 7){
        m_LastBit = true;
    }
    if (m_CurrentReadBit >= 7){
        nextBit = m_CharInBits[m_CurrentReadBit];
        for (bool & m_charInBit : m_CharInBits)
            m_charInBit = false;
        loadNextCharToBits();
        return nextBit;
    }
    nextBit = m_CharInBits[m_CurrentReadBit];
    m_CurrentReadBit++;
    return nextBit;
}
bool CHuffFile::decompressMessage(){
    checkFiles();
    if (!m_OutputFileStream.is_open())
        return false;
    while (!last()){
        bool decisionBit = getNextBit();
        if (last())
            return false;
        //cout << endl << "DECISION BIT: " << decisionBit << endl;
        if (decisionBit == 1) { // bit 1 indicates a chunk of 4096 symbols
            if (!readBigChunk())
                return false;
        } else { // the decision bit is 0, it indicates that a smaller chunk is being read.
            if (!readSmallChunk())
                return false;
        }
    }
    return true;
}
bool CHuffFile::readBigChunk(){
    for (int i = 0; i < 4096; ++i){
        if (last())
            return false;
        if (!traverseTree(m_HuffCodeTree))
            return false;
    }
    return true;
}
bool CHuffFile::readSmallChunk(){
    int numOfSymbols = 0;
    for (int i = 0; i < 12; ++i) {
        bool nextBit = getNextBit();
        //cout << nextBit << flush;
        if (!good()) // Something went wrong with the file or the bit was last.
            return false;
        numOfSymbols <<= 1;
        if (nextBit) numOfSymbols |= 1;
    }
    if (numOfSymbols == 0)
        return true;
    //cout << endl << "smjall chunk of " << numOfSymbols << " symbols." << endl;
    for (int j = 0; j < numOfSymbols; ++j) {
        if (!traverseTree(m_HuffCodeTree))
            return false;
        if (last() && j != numOfSymbols-1)
            return false;
    }
    m_LastBit = true;
    return true;
}
bool CHuffFile::traverseTree(const CHuffTreeNode * x){
    if (x->isLeaf()){
        x->getSymbol().writeSymbol();
    } else {
        bool nextBit = getNextBit();
        if (!m_OutputFileStream || m_OutputFileStream.fail() || !m_OutputFileStream.is_open())
            return false;
        if (!good())
            return false;
        if (nextBit == 0){ // Next read bit is a 0 so the recursion continues to the left tree.
            //cout << "0" << flush;
            if (!traverseTree(x->goLeft()))
                return false;
        } else { // Next read bit is 1 so the recursion continues to the right tree.
            //cout << "1" << flush;
            if (!traverseTree(x->goRight()))
                return false;
        }
    }
    return true;
}
bool CHuffFile::eof(){
    return m_EofBit;
}
bool CHuffFile::good(){
    return m_GoodBit;
}
bool CHuffFile::last(){
    return m_LastBit;
}
void CHuffFile::writeToOutput(const char x) {
    m_OutputFileStream << x;
}

//////// CHuffTreeNode methods ////////
CHuffTreeNode::CHuffTreeNode(CHuffFile * huffFile)
:m_Symbol(CSymbol(this)),m_HuffFile(huffFile),m_left_0(nullptr), m_right_1(nullptr){
}
bool CHuffTreeNode::buildTree() {
    bool decisionBit = m_HuffFile->getNextBit();
    if (!m_HuffFile->good() || m_HuffFile->last()) // If something went wrong with the file or the last bit has been read.
        return false; // Return false, the last bit of the input is too soon.
    if (decisionBit == 0){ // The next decision bit is 0 therefore the recursion needs to go deeper.
        m_left_0 = new CHuffTreeNode(m_HuffFile);
        if (!m_left_0->buildTree())
            return false;
        m_right_1 = new CHuffTreeNode(m_HuffFile);
        if (!m_right_1->buildTree())
            return false;
    } else if (decisionBit == 1){ // The next N bits are to be saved inside the node as a symbol to be later read.
        if (!m_Symbol.readSymbol())
            return false;
    }
    return true;
}
void CHuffTreeNode::deleteTree(){
    if (m_left_0 != nullptr)
        m_left_0->deleteTree();
    if (m_right_1 != nullptr)
        m_right_1->deleteTree();
    delete this;
}
bool CHuffTreeNode::isLeaf() const {
    return (m_left_0 == nullptr && m_right_1 == nullptr);
}
CHuffTreeNode * CHuffTreeNode::goLeft() const{
    return m_left_0;
}
CHuffTreeNode * CHuffTreeNode::goRight() const {
    return m_right_1;
}
CHuffFile *CHuffTreeNode::getFile() {
    return m_HuffFile;
}
CSymbol CHuffTreeNode::getSymbol() const {
    return m_Symbol;
}

//////// CSymbol methods ////////
CSymbol::CSymbol(CHuffTreeNode * x): m_HuffTreeNode(x), m_NumOfBytes(0), m_SymbolVal(0){}
bool CSymbol::readSymbol(){
    /* UTF-8 table
     *    1B         2B          3B         4B
     * 0xxx xx.xx |                                     load 5, check 0, load 2
     * 110x xx.xx | 10xx xxxx |                         load 5, check 110, load 10
     * 1110 xx.xx | 10xx xxxx | 10xx xxxx |             load 5, check 1110, load 18
     * 1111 0x.xx | 10xx xxxx | 10xx xxxx | 10xx xxxx | load 5, check 11110, load 26
     * 31   27.   | 23   19   | 15   11   | 7    3
     *  30   26   |  22   18  |  14   10  |  6    2
     *   29   25  |   21   17 |   13   9  |   5    1
     *    28  .24 |    20   16|    12   8 |    4    0
     */
    for (int i = 0; i < 6; ++i) {
        bool nextBit = m_HuffTreeNode->getFile()->getNextBit();
        if (!m_HuffTreeNode->getFile()->good() || m_HuffTreeNode->getFile()->last())
            return false;
        setBit(31-i,nextBit);
    }
    if (getBit(31)){ // (1....)
        if (getBit(30)){ // (11...)
            if (getBit(29)){ // (111...)
                if (getBit(28)){ // (1111...)
                    if (getBit(27)){ // (11111...) => not valid
                        return false;
                    } else { // (11110...) => 4 bytes;
                        m_NumOfBytes = 4;
                        if (!loadBits())
                            return false;
                    }
                } else { // (1110...) => 3 bytes
                    m_NumOfBytes = 3;
                    if(!loadBits())
                        return false;
                }
            } else { // (110...) => 2 bytes
                m_NumOfBytes = 2;
                if(!loadBits())
                    return false;
            }
        } else { // (10...) => not valid
            return false;
        }
    } else { // (0...) => ascii 1 byte
        m_NumOfBytes = 1;
        if (!loadBits())
            return false;
    }
    return true;
}
void CSymbol::writeSymbol(){
    for (int i=0; i < m_NumOfBytes; i++) {
        this->m_HuffTreeNode->getFile()->writeToOutput(m_SymbolArr[i]);
    }
}
bool CSymbol::toSymbolArr() {
    m_SymbolArr[0] = (m_SymbolVal >> 24) & 0xFF;
    m_SymbolArr[1] = (m_SymbolVal >> 16) & 0xFF;
    m_SymbolArr[2] = (m_SymbolVal >> 8) & 0xFF;
    m_SymbolArr[3] = m_SymbolVal & 0xFF;
    return true;
}
bool CSymbol::getBit(unsigned int bit){
    return 1 == ((m_SymbolVal >> bit) & 1);
}
void CSymbol::setBit(unsigned int pos, bool val){
    m_SymbolVal = (m_SymbolVal & (~(1 << pos))) | (val << pos);
}
bool CSymbol::valid(){
    if (m_NumOfBytes == 1){
        if (getBit(31))
            return false;
    } else if (m_NumOfBytes == 2){
        if ((!getBit(23) || getBit(22)))
            return false;
    } else if (m_NumOfBytes == 3){
        if ((!getBit(23) || getBit(22) || !getBit(15) || getBit(14))
        || (m_SymbolArr [0] == 0xE0u && (m_SymbolArr[1] < 0xA0u || m_SymbolArr[1] > 0xBFu)) // UTF-8 special case
        || (m_SymbolArr [0] == 0xEDu && (m_SymbolArr[1] < 0x80u || m_SymbolArr[1] > 0x9Fu))) // UTF-8 special case
            return false;
    } else if (m_NumOfBytes == 4){
        if ((!getBit(23) || getBit(22) || !getBit(15) || getBit(14) || !getBit(7) || getBit(6))
        || (m_SymbolArr[0] == 0xF0u && (m_SymbolArr[1] < 0x90u || m_SymbolArr[1] > 0xBFu))
        || (m_SymbolArr[0] == 0xF4u && (m_SymbolArr[1] < 0x80u || m_SymbolArr[1] > 0x8Fu)))
            return false;
    }
    return true;
}
bool CSymbol::loadBits() {
    if (m_NumOfBytes == 1){
        for (int i = 0; i < 2; ++i) { // Loaded (0xxxx) now I read 7 bits
            bool nextBit = m_HuffTreeNode->getFile()->getNextBit();
            if (!m_HuffTreeNode->getFile()->good() || m_HuffTreeNode->getFile()->last()) return false;
            setBit(25-i,nextBit);
        }
    } else if (m_NumOfBytes == 2){ // Loaded (110xx), now I read 13 bits.
        for (int i = 0; i < 10; ++i) {
            bool nextBit = m_HuffTreeNode->getFile()->getNextBit();
            if (!m_HuffTreeNode->getFile()->good() || m_HuffTreeNode->getFile()->last()) return false;
            setBit(25-i,nextBit);
        }
    } else if (m_NumOfBytes == 3){ // Loaded (1110x), now I read 20 bits.
        for (int i = 0; i < 18; ++i) {
            bool nextBit = m_HuffTreeNode->getFile()->getNextBit();
            if (!m_HuffTreeNode->getFile()->good() || m_HuffTreeNode->getFile()->last()) return false;
            setBit(25-i,nextBit);
        }
    } else if (m_NumOfBytes == 4) { // Loaded (11110) now I read 27 bits.
        for (int i = 0; i < 26; ++i) {
            bool nextBit = m_HuffTreeNode->getFile()->getNextBit();
            if (!m_HuffTreeNode->getFile()->good() || m_HuffTreeNode->getFile()->last()) return false;
            setBit(25-i,nextBit);
        }
    }
    if (!toSymbolArr())
        return false;
    if(!valid())
        return false;
    return true;
}
//////// Progtest solution function ////////
/*
 * Function which takes in path to Huffman compressed file and decodes it into plaintext.
 * @return true when everything went fine
 * @return false when an error occurred
 */
bool decompressFile(const char *inFileName, const char *outFileName) {
    CHuffFile huffFile(inFileName, outFileName);
    return huffFile.decompressFile();
}
bool compressFile(const char *inFileName, const char *outFileName) {
    // keep this dummy implementation (no bonus) or implement the compression (bonus)
    return false;
}
#ifndef __PROGTEST__
bool identicalFiles(const char *fileName1, const char *fileName2) {
    ifstream file1;
    ifstream file2;
    file1.open(fileName1, ios::binary | ios::in);
    file2.open(fileName2, ios::binary | ios::in);
    if (!file1 || !file2){return false;}
    if (file1.is_open() && file2.is_open()) {
        char ch1;
        char ch2;
        while (true) {
            file1.get(ch1);
            file2.get(ch2);
            if (file1.eof() && file2.eof() && ch1 == ch2) {
                file1.close();
                file2.close();
                return true;
            } else if (file1.eof() || file2.eof() || ch1 != ch2) {
                file1.close();
                file2.close();
                return false;
            }
        }
    }
    file1.close();
    file2.close();
    return false;
}
//////// Main ////////
int main() {
   assert (decompressFile("tests/test0.huf", "tempfile"));
   assert (identicalFiles("tests/test0.orig", "tempfile"));

   assert (decompressFile("tests/test1.huf", "tempfile"));
   assert (identicalFiles("tests/test1.orig", "tempfile"));

   assert (decompressFile("tests/test2.huf", "tempfile"));
   assert (identicalFiles("tests/test2.orig", "tempfile"));

   assert (decompressFile("tests/test3.huf", "tempfile"));
   assert (identicalFiles("tests/test3.orig", "tempfile"));

   assert (decompressFile("tests/test4.huf", "tempfile"));
   assert (identicalFiles("tests/test4.orig", "tempfile"));

   assert (!decompressFile("tests/test5.huf", "tempfile"));
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    assert (decompressFile("tests/extra0.huf", "tempfile"));
    assert (identicalFiles("tests/extra0.orig", "tempfile"));

    assert (decompressFile("tests/extra1.huf", "tempfile"));
    assert (identicalFiles("tests/extra1.orig", "tempfile"));

    assert (decompressFile("tests/extra2.huf", "tempfile"));
    assert (identicalFiles("tests/extra2.orig", "tempfile"));

    assert (decompressFile("tests/extra3.huf", "tempfile"));
    assert (identicalFiles("tests/extra3.orig", "tempfile"));

    assert (decompressFile("tests/extra4.huf", "tempfile"));
    assert (identicalFiles("tests/extra4.orig", "tempfile"));

    assert (decompressFile("tests/extra5.huf", "tempfile"));
    assert (identicalFiles("tests/extra5.orig", "tempfile"));

    assert (decompressFile("tests/extra6.huf", "tempfile"));
    assert (identicalFiles("tests/extra6.orig", "tempfile"));

    assert (decompressFile("tests/extra7.huf", "tempfile"));
    assert (identicalFiles("tests/extra7.orig", "tempfile"));

    assert (decompressFile("tests/extra8.huf", "tempfile"));
    assert (identicalFiles("tests/extra8.orig", "tempfile"));

    assert (decompressFile("tests/extra9.huf", "tempfile"));
    assert (identicalFiles("tests/extra9.orig", "tempfile"))
    return 0;
}

#endif /* __PROGTEST__ */
#include <iostream>
#include <fstream>
#include <string>
#include "Stack.h"
#include "Queue.h"
#include <cctype>
#include <cstring>
#include "bintree.h"

using namespace std;

typedef int Rank;

class Bitmap {
private:
    unsigned char* M;
    Rank N, _sz;
    void init(Rank n) { M = new unsigned char[N = (n + 7) / 8]; memset(M, 0, N); _sz = 0; }
    void expand(Rank k) {
        if (k < 8 * N) return;
        Rank oldN = N; unsigned char* oldM = M;
        init(2 * k);
        memcpy(M, oldM, oldN);
        delete[] oldM;
    }
public:
    Bitmap(Rank n = 8) { init(n); }
    Bitmap(const Bitmap& other) {
        M = new unsigned char[N = other.N];
        memcpy(M, other.M, N);
        _sz = other._sz;
    }
    Bitmap& operator=(const Bitmap& other) {
        if (this == &other) return *this;
        unsigned char* newM = new unsigned char[other.N];
        memcpy(newM, other.M, other.N);
        delete[] M;
        M = newM; N = other.N; _sz = other._sz;
        return *this;
    }
    ~Bitmap() { delete[] M; M = nullptr; _sz = 0; }
    Rank size() const { return _sz; }
    void set(Rank k) { expand(k); _sz++; M[k >> 3] |= (0x80 >> (k & 0x07)); }
    void clear(Rank k) { expand(k); if (_sz > 0) _sz--; M[k >> 3] &= ~(0x80 >> (k & 0x07)); }
    bool test(Rank k) const { return M[k >> 3] & (0x80 >> (k & 0x07)); }
    char* bits2string(Rank n) const {
        char* s = new char[n + 1]; s[n] = '\0';
        for (Rank i = 0; i < n; i++) s[i] = test(i) ? '1' : '0';
        return s;
    }
};

struct HuffCode { Bitmap bits; Rank len; HuffCode(): bits(8), len(0) {} void push(bool b) { if (b) bits.set(len); else bits.clear(len); len++; } string str() const { char* s = bits.bits2string(len); string r(s); delete[] s; return r; } };

struct HuffNode {
    char ch;
    int freq;
    HuffNode* left;
    HuffNode* right;
    HuffNode(char c, int f): ch(c), freq(f), left(nullptr), right(nullptr) {}
    HuffNode(HuffNode* l, HuffNode* r): ch('\0'), freq((l?l->freq:0)+(r?r->freq:0)), left(l), right(r) {}
};

struct Cmp {
    bool operator()(const HuffNode* a, const HuffNode* b) const { return a->freq > b->freq; }
};

class HuffTree {
private:
    HuffNode* _root;
    void release(HuffNode* x) { if (!x) return; release(x->left); release(x->right); delete x; }
    void dfs(HuffNode* x, HuffCode& cur, Vector<HuffCode>& codes) { if (!x) return; if (!x->left && !x->right) { codes[x->ch - 'a'] = cur; return; } cur.push(false); dfs(x->left, cur, codes); cur.len--; cur.push(true); dfs(x->right, cur, codes); cur.len--; }
public:
    HuffTree(): _root(nullptr) {}
    ~HuffTree() { release(_root); }
    void build(const Vector<int>& freq) {
        Vector<HuffNode*> nodes(0, 0, (HuffNode*)nullptr);
        for (int i = 0; i < 26; ++i) if (freq[i] > 0) { nodes.insert(nodes.size(), new HuffNode('a' + i, freq[i])); }
        if (nodes.size() == 0) return;
        auto pick_min = [&](Vector<HuffNode*>& arr){ int idx=-1; int best=1<<30; for(int i=0;i<arr.size();++i){ if(arr[i] && arr[i]->freq < best){ best=arr[i]->freq; idx=i; } } return idx; };
        while (nodes.size() > 1) {
            int i1 = pick_min(nodes); HuffNode* a = nodes.remove(i1);
            int i2 = pick_min(nodes); HuffNode* b = nodes.remove(i2);
            nodes.insert(nodes.size(), new HuffNode(a, b));
        }
        _root = nodes[0];
    }
    void makeCodes(Vector<HuffCode>& codes) { codes = Vector<HuffCode>(26, 26, HuffCode()); HuffCode cur; dfs(_root, cur, codes); }
};

static string read_text_or_sample(const string& path) {
    ifstream fin(path, ios::in | ios::binary);
    if (fin) { string s; fin.seekg(0, ios::end); size_t n = fin.tellg(); fin.seekg(0); s.resize(n); fin.read(&s[0], n); return s; }
    string s = "I have a dream that one day this nation will rise up and live out the true meaning of its creed: we hold these truths to be self-evident, that all men are created equal. I have a dream.";
    return s;
}

static Vector<int> letter_freq(const string& s) { Vector<int> f(26, 26, 0); for (unsigned char c : s) { char x = tolower(c); if (x >= 'a' && x <= 'z') f[x - 'a'] = f[x - 'a'] + 1; } return f; }

static string encode_word(const string& w, const Vector<HuffCode>& codes) { string r; for (size_t i = 0; i < w.size(); ++i) { char x = tolower(static_cast<unsigned char>(w[i])); if (x >= 'a' && x <= 'z') { const HuffCode& hc = codes[x - 'a']; if (hc.len > 0) r += hc.str(); } } return r; }

int main() {
    cout << "Huffman Coding for letters (exp2)" << endl;
    string text = read_text_or_sample("exp2/I_have_a_dream.txt");
    auto freq = letter_freq(text);
    HuffTree ht; ht.build(freq);
    Vector<HuffCode> codes(26, 26, HuffCode()); ht.makeCodes(codes);
    cout << "Codes for letters present:" << endl;
    for (int i = 0; i < 26; ++i) { if (codes[i].len > 0) cout << char('a' + i) << ": " << codes[i].str() << endl; }
    Vector<string> words(4, 4, string()); words[0] = "dream"; words[1] = "have"; words[2] = "martin"; words[3] = "king";
    for (int i = 0; i < words.size(); ++i) cout << words[i] << " => " << encode_word(words[i], codes) << endl;
    return 0;
}
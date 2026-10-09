#include <iostream>
#include <vector>
#include <string>
#include <string>
#include <stdexcept>
#include <random>
#include <numeric>
 
using namespace std;
using ll = long long;
ll mulMod(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}
ll extendedGCD(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll gcd = extendedGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}
ll modInverse(ll a, ll m) {
    ll x, y;
    ll g = extendedGCD(a, m, x, y);
    if (g != 1) throw runtime_error("Modular inverse does not exist");
    return ((x % m) + m) % m;
}
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}
struct PublicKey {
    ll n; // modulus
    ll e; // public exponenttt
};
struct PrivateKey {
    ll n; // modulus
    ll d; // private exponent
};
struct KeyPair {
    PublicKey pub;
    PrivateKey prv;
};
KeyPair generateKeys(ll p, ll q) {
    if (!isPrime(p) || !isPrime(q))
        throw invalid_argument("Both p and q must be prime");
    if (p == q)
        throw invalid_argument("p and q must be distinct");
    ll n = p * q;
    ll phi = (p - 1) * (q - 1);

    ll e = 65537;
    if (e >= phi || gcd(e, phi) != 1) {
        for (e = 3; e < phi; e += 2) {
            if (gcd(e, phi) == 1) break;
        }
    }
    ll d = modInverse(e, phi);
    return { {n, e}, {n, d} };
}
ll encryptBlock(ll m, const PublicKey &pub) {
    return mulMod(m, pub.e, pub.n);
}
ll decryptBlock(ll c, const PrivateKey &priv) {
    return mulMod(c, priv.d, priv.n);
}
vector<ll> encryptMessage(const string &message, const PublicKey &pub) {
     vector<ll> cipher;
     cipher.reserve(message.size());
    for (unsigned char c : message) {
        if (static_cast<ll>(c) >= pub.n)
            throw runtime_error( "I'afraid there is a dismatch — you need to try larger primes so n exceeds all character codes.");
        cipher.push_back(encryptBlock(static_cast<ll>(c), pub));
    }
    return cipher;
}
     string decryptMessage(const vector<ll> &cipher, const PrivateKey &priv) {
         string message;
    message.reserve(cipher.size());
    for (ll c : cipher) {
        ll m = decryptBlock(c, priv);
        message.push_back(static_cast<char>(m));
    }
    return message;
}
void printKeys(const KeyPair &keys) {
         cout << "Public key  (n, e): (" << keys.pub.n << ", " << keys.pub.e << ")\n";
         cout << "Private key (n, d): (" << keys.prv.n << ", " << keys.prv.d << ")\n";
}
int main() {
    system("chcp 65001 > nul");
        cout << " \033[1;3;38;2;255;182;193m    RSA Cryptography Simulator 🩷     \033[0m \n";
    ll p, q;
         cout<< "Hello there! So welcome to my mini mini project. Now please enter your name"<< "\n";
         string Name;
        cout << "\033[38;5;218m Name: \033[0m"<< Name;
        getline(cin, Name);
         cout << "Nice name "<< Name<< "!"<<"\n";
         cout << "So now, I want you to enter two distinct prime numbers (p and q), such as 61 53: "<<"\n";
         cin >> p >> q;

    KeyPair keys;
    try {
        keys = generateKeys(p, q);
    } catch (const exception &ex) {
            cerr << "Error generating keys: " << ex.what() << "\n";
        return 1;
    }
    
         cout << "\033[38;5;218mHere are your generated keys:\033[0m\n";
    printKeys(keys);

         cin.ignore();
         string message;
         cout << "\nNow all you have to do is enter a message to encrypt: ";
         getline(cin, message);

    try {
            vector<ll> cipher = encryptMessage(message, keys.pub);

             cout << "\nEncrypted (cipher blocks):\n";
        for (ll c : cipher) cout << c << " ";
             cout << "\n";

             string decrypted = decryptMessage(cipher, keys.prv);
             cout << "\n\033[38;5;218mDecrypted message: \033[0m" << decrypted << "\n";

        if (decrypted == message){
                 cout << "\n\033[38;2;230;205;250mRound-trip successful — decrypted text matches original.\033[0m\n";
                 cout << "\033[3m  XOXO, Alae\033[0m";
        }
        else {
                 cout << "\n\033[38;5;218mI'm afraid there is a dismatch — you need to try larger primes so n exceeds all character codes.\033[0m\n";
                 cout<<"😿My bad...";
        }
    } catch (const exception &ex) {
             cerr << "Error: " <<"\033[38;5;218m"<< ex.what() <<"\033[0m"<<"\n"; 
                 cout<<"😿sorry but you need to try again...";
        return 1;
    }  
    return 0;
}

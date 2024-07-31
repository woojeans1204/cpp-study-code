#include <iostream>
#include <cstring>

using namespace std;
const int NAME_LEN_MAX = 20;

class Account {
    private:
        int id;
        char *name;
        int money;
    public:
        Account(int i, char *n, int m) : id(i), name(n), money(m) {
        }
        Account(Account &copy) {
            id = copy.id;
            name = new char[strlen(copy.name)+1];
            strcpy(name, copy.name);
            money = copy.money;
        }
        ~Account() {
            delete []name;
        }
        void show_info() const{
            cout << "계좌ID: " << id << endl;
            cout << "이름: " << name << endl;
            cout << "잔액: " << money << endl;
        }
        int get_id(void) const{
            return id;
        }
        void deposit(int money) {
            this->money += money;
        }
        int withdraw(int money) {
            if (this->money < money) return 0;
            this->money -= money;
            return money;
        }
};

class NormalAccount : public Account {
    private:
        int rate;
    public:
        NormalAccount(int i, char *n, int m, int r) : Account(i, n, m), rate(r) {
        }   
        virtual void deposit(int money) {
            Account::deposit(money);
            Account::deposit(money*rate/100);
        }
};

class HighCreditAccount : public NormalAccount {
    private:
        int plus;
    public:
        HighCreditAccount(int i, char *n, int m, int r, int l) : NormalAccount(i, n, m, r), plus(l) {

        }
        virtual void deposit(int money) {
            NormalAccount::deposit(money);
            Account::deposit(money*plus/100);
        }
};

class AccountHandler {
    private:
        Account *user[100]; // 100개의 계정 저장 가능  
        int user_n; // 유저 수
    
    public:
        AccountHandler(void) :user_n(0) {
        }
        ~AccountHandler() {
            for (int i = 0; i < user_n; i++)
                delete user[i];
        }
        void create(void) { // 계좌 개설
            int id;
            char name[NAME_LEN_MAX+1];
            int money;
            int d, r, l;
            cout << "[계좌종류선택]" << endl;
            cout << "1.보통예금계좌 2.신용신뢰계좌" << endl;
            cout << "선택: ";
            cin >> d;
            if (d == 1) 
                cout << "[보통예금계좌 개설]" << endl;
            else
                cout << "[신용신뢰계좌 개설]" << endl;
            cout << "계좌ID: ";
            cin >> id;
            cout << "이 름: ";
            cin >> name;
            cout << "입금액: ";
            cin >> money;
            cout << "이자율: ";
            cin >> r;
            if (d == 2) {
                cout << "신용등급(1toA, 2toB, 3toC): ";
                cin >> l;
                if (l == 1) l = 7;
                else if (l == 2) l = 4;
                else l = 2;
            }

            char *tmp = new char[strlen(name)+1];
            strcpy(tmp, name);
            user[user_n++] = d == 1 ? new NormalAccount(id, tmp, money, r) : new HighCreditAccount(id, tmp, money, r, l);
        }

        void deposit(void) { // 계좌 입금
            int idx, m;

            cout << "[입    금]\n";
            cout << "계좌ID: ";
            cin >> idx;

            for (int i = 0; i < user_n; i++) {
                if (idx != (user[i]->get_id()))
                    continue;
                cout << "입금액: ";
                cin >> m;
                user[i]->deposit(m);
                cout << "입금완료\n";
                return;
            }
            cout << "유효하지 않은 ID입니다\n";
            return;
        }

        void withdraw(void) { // 계좌 출금
            int idx, m;

            cout << "[출    금]\n";
            cout << "계좌ID: ";
            cin >> idx;
            for (int i = 0; i < user_n; i++) {
                if (idx != (user[i]->get_id()))
                    continue;
                cout << "출금액: ";
                cin >> m;
                if (!user[i]->withdraw(m)) cout << "출금 완료\n";
                else cout << "출금액이 부족합니다\n";
                return;
            }
            cout << "유효하지 않은 ID입니다\n";
            return;
        }

        void print(void) { // 계좌정보 출력
            int i;
            for (i = 0; i < user_n; i++) {
                user[i]->show_info();
            }
        }
};

int main() {
    int p;
    AccountHandler h;
    while (true) {
        cout << "-----Menu------\n";
        cout << "1. 계좌개설\n";
        cout << "2. 입 금\n";
        cout << "3. 출 금\n";
        cout << "4. 계좌정보 전체 출력\n";
        cout << "5. 프로그램 종료\n";
        cout << "선택: ";
        cin >> p;
        switch (p) {
            case 1:
                h.create();
                break;
            case 2:
                h.deposit();
                break;
            case 3:
                h.withdraw();
                break;
            case 4:
                h.print();
                break;
            case 5:
                cout << "프로그램이 종료되었습니다\n";
                return 0;
            default:
                cout << "잘못된 선택입니다\n";
        }
    }
}
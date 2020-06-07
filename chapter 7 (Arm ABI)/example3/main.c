struct myStruct1Word {
        int a;
};

struct myStruct2Word {
        int a;
        int b;
};
struct myStruct7Word{
        int a;
        int b;
        int c;
        int d;
        int e;
        int f;
        char g;
};

extern struct myStruct1Word callee1();
extern struct myStruct2Word callee2();
extern struct myStruct7Word callee7();

int caller(){
        struct myStruct1Word s;
        struct myStruct2Word t;
        struct myStruct7Word u;

        s = callee1();
        t = callee2();
        u = callee7();
        return s.a + t.a + u.a;
}

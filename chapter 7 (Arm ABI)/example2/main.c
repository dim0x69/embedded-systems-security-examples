struct myStruct1Word {
        int a;
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

extern void callee1(struct myStruct1Word);
extern void callee7(struct myStruct7Word);

void caller(){
        struct myStruct1Word s;
        s.a = 1111;
        callee1(s);
        struct myStruct7Word t;
        t.a = 2222;
        t.g = 0xFF;
        callee7(t);
}

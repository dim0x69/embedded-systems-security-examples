struct s {
        int a;
        int b;
};

struct s __attribute__((cmse_nonsecure_entry)) sum(struct s a){
        return (struct s) {a.a + 3, a.b + 4};

}

int main(void){
        struct s mySt;
        mySt.a = 1;
        mySt.b = 2;
        struct s mySt2 = sum(mySt);
        return mySt.a + mySt.b + mySt2.a + mySt2.b;
;
}

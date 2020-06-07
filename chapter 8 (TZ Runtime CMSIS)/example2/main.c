struct s {
        int a;
        int b;
};

int __attribute__((cmse_nonsecure_entry)) sum(struct s a){
        return  a.a + a.b;
}


int main(void){
        struct s mySt;
        mySt.a = 1;
        mySt.b = 2;
        int mySt2 = sum(mySt);
        return mySt2;

}

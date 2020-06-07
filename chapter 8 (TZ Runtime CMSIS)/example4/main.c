struct s {
        int a;
        int b;
};



typedef int __attribute__((cmse_nonsecure_call)) nsfunc(struct s);
int secure(nsfunc* callback){
        struct s a;
        a.a = 1;
        a.b = 2;
        int b = callback(a);
        return a.a + b + a.b + b;
}

void main(void){}

struct s {
        int a;
        int b;
};

typedef struct s __attribute__((cmse_nonsecure_call)) nsfunc(struct s);
int secure(nsfunc* callback){
        struct s a = {1,2};
        struct s b = callback(a);
        return a.a + b.a + a.b + b.b;
}

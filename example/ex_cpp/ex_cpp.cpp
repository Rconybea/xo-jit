struct env_type;

struct closure_type {
    double (*fnptr)(env_type * env, double x);
    env_type * envptr;
};

double
sqrt(double x) {
    return x/100;
}

double
wrap_sqrt(env_type * env, double x) {
    return ::sqrt(x);
}

double twice(env_type * env, closure_type fnclosure, double x) {
    double tmp1 = (*fnclosure.fnptr)(fnclosure.envptr, x);
    double tmp2 = (*fnclosure.fnptr)(fnclosure.envptr, tmp1);

    return tmp2;
}

closure_type make_some_closure()
{
    closure_type closure;
    closure.fnptr = &wrap_sqrt;
    closure.envptr = nullptr;

    return closure;
}

int main() {
    closure_type closure = make_some_closure();

    double y = twice(nullptr, closure, 4.0);

    //std::cout << "y=" << y << std::endl;
}

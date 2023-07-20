/*
** EPITECH PROJECT, 2023
** f4
** File description:
** f4
*/

int make_calculations(int a, int b, int c)
{
    a = a + b;
    a = a  + b;
    a = a +  b;
    a = a+ b;
    a = a +b;
    a= a + b;
    a =a + b;
    b |= (a == b) && (b == c);
    b |= (a == b) &&(b == c);
    b |= (a == b)&& (b == c);
    return (a + b + c);
}

int multiline(int a, int b, int c)
{
    b |= (a == b * c + a / c)
        || (b * a != c / a);
    b <<= 3;
    b |= (a == b * c + a / c) &&
        (b * a != c / a);
    b <<=3;
    b |= (a == b * c + a / c)
        &&(b * a != c / a);
    b<<= 3;
    b |= (a == b * c + a / c)||
        (b * a != (c / a));
    return (a + b + c);
}

int comma(int a, int b, int c)
{
    a = (a, b + c);
    a = (a,b + c);
    a = (a ,b + c);
    a = (a , b + c);
    return a;
}

int ternary(int a, int b, int c)
{
    a = a ? b : c;
    a = a? b : c;
    a = a ?b : c;
    a = a?b : c;
    a = a ? b :c;
    a = a ? b  :c;
    a = a ? b: c;
    a = a ? b:c;
    a = a?b:c;
    return a;
}

int main(void)
{
    return make_calculations(1, 2, 3);
}

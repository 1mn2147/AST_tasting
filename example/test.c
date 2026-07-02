int any(int a, int b)
{
    if(a>b){
        return -1;
    }
    else if(a==b){
        return 0;
    }
    else{
        return 1;
    }
}

int main(void)
{
    any(1,2);
}


int int_digit_length(int number) {
    int digit_length;
    while(number != 0){
        number /= 10;
        ++digit_length;
    }
    return digit_length;
}
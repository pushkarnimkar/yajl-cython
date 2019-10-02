int
addition(int* vals, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += vals[i];
    }
    return sum;
}

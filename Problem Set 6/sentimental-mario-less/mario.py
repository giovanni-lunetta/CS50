from cs50 import get_int
# TODO
# This is basically a do while loop but for python
while True:
    input = get_int("Height: ")
    if input >= 1 and input <= 8:
        break

for i in range(1, input + 1):
    for j in range(1, input + 1):
        if (j <= input - i):
            print(' ', end='')
        else:
            print('#', end='')
    print()
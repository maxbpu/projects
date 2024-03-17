from constants import *
from Connect4 import *

# Application code
if __name__ == '__main__':
    print("Welcome to the Game of Connect4!")
    c1 = None
    c2 = None
    a1 = None
    a2 = None
    a3 = None
    b1 = None
    b2 = None
    b3 = None
    while(True):
        print(COLORS[2] + "Please choose the first player:")
        print("1. Human")
        print("2. AI Algorithm")
        print("Please enter 1 or 2")
        n = int(sys.stdin.readline())
        if type(n) != int or (n not in [1,2]):
            print("That is not a valid option. Please try again.")
        else:
            if n ==2:
                while(True):
                    print(COLORS[2] + "Please select the algorithm you want the computer to use:")
                    print("1. Min-Max Algorithm")
                    print("2. Min-Max Algorithm with Alpha Beta Pruning")
                    print("Please enter 1 or 2")
                    c1 = int(sys.stdin.readline())
                    if type(c1) != int or (c1 not in [1,2]):
                        print("That is not a valid option. Please try again.")
                    else:
                        break
                while(True):
                    print(COLORS[2] + "Please enter the value for a1 (between 1 and -1):")
                    a1 = int(sys.stdin.readline())
                    if type(a1) != int or (a1 > 1 or a1 < -1):
                        print("That is not a valid option. Please try again.")
                    else:
                        break
                while(True):
                    print(COLORS[2] + "Please enter the value for a2 (between 1 and -1):")
                    a2 = int(sys.stdin.readline())
                    if type(a2) != int or (a2 > 1 or a2 < -1):
                        print("That is not a valid option. Please try again.")
                    else:
                        break
                while(True):
                    print(COLORS[2] + "Please enter the value for a3 (between 1 and -1):")
                    a3 = int(sys.stdin.readline())
                    if type(a3) != int or (a3 > 1 or a3 < -1):
                        print("That is not a valid option. Please try again.")
                    else:
                        break
            break
    while(True):
        print("Please choose the second player:")
        print("1. Human")
        print("2. AI Algorithm")
        print("Please enter 1 or 2")
        m = int(sys.stdin.readline())
        if type(m) != int or (m not in [1,2]):
            print("That is not a valid option. Please try again.")
        else:
            if m ==2:
                while(True):
                    print(COLORS[2] + "Please select the algorithm you want the computer to use:")
                    print("1. Min-Max Algorithm")
                    print("2. Min-Max Algorithm with Alpha Beta Pruning")
                    print("Please enter 1 or 2")
                    c2 = int(sys.stdin.readline())
                    if type(c2) != int or (c2 not in [1,2]):
                        print("That is not a valid option. Please try again.")
                    else:
                        break
                while(True):
                    print(COLORS[2] + "Please enter the value for b1 (between 1 and -1):")
                    b1 = int(sys.stdin.readline())
                    if type(b1) != int or (b1 > 1 or b1 < -1):
                        print("That is not a valid option. Please try again.")
                    else:
                        break
                while(True):
                    print(COLORS[2] + "Please enter the value for b2 (between 1 and -1):")
                    b2 = int(sys.stdin.readline())
                    if type(b2) != int or (b2 > 1 or b2 < -1):
                        print("That is not a valid option. Please try again.")
                    else:
                        break
                while(True):
                    print(COLORS[2] + "Please enter the value for b3 (between 1 and -1):")
                    b3 = int(sys.stdin.readline())
                    if type(b3) != int or (b3 > 1 or b3 < -1):
                        print("That is not a valid option. Please try again.")
                    else:
                        break
            break
    Connect4().update(n, m, c1, c2, a1, a2, a3, b1, b2, b3)
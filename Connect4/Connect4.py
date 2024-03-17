import sys
from models import *
from algorithms import *
from alphabeta import *



class Connect4():

    def __init__(self):
        self._board = Board()
        self._players = ["Player 1", "Player 2"]
        self._currentplayer = 0
        self._nextplayer = 1
    
    def update(self, p1, p2, c1, c2, a1, a2, a3, b1, b2, b3):
        result = (None, None)
        while(True):
            player = self._currentplayer
            if player == 0:
                current = p1
            else:
                current = p2
            if current == 1:
                print(COLORS[2] + self._players[player] + ", it is your turn!")
                while(True):
                    print(COLORS[2] +"Please enter which column of the Board you want to drop your piece in! (from 0 - 6)")
                    Board.draw(self._board)
                    n = int(sys.stdin.readline())
                    if type(n) != int or (n>6) or (n <0):
                        print(COLORS[2] +"That is not a valid column number. Please try again.")
                    else:
                        break                   
            else:
                print(COLORS[2]+"It is " + self._players[player] + "'s turn. Please wait while the computer selects the move.")
                if player == 0:
                    c = c1
                    q1 = a1
                    q2 = a2
                    q3 = a3
                else:
                    c = c2
                    q1 = b1
                    q2 = b2
                    q3 = b3
                if c == 1:
                    n = choosebyminmax(self._board.boardrep(), self._currentplayer, result[1], q1, q2, q3)
                else:
                    n = choosebyalphabeta(self._board.boardrep(), self._currentplayer, result[1], q1, q2, q3)
            result = Board.findm(self._board, n, self._currentplayer)

            if result[0]: # legal move
                if Board.check_win(self._board, n, result[1]): # n is the x, result[1] is the y
                    print(COLORS[2]+ "That was a winning move!")
                    print(self._players[self._currentplayer] + " is the winner!")
                    Board.draw(self._board)
                    break
                else:
                    temp = self._currentplayer
                    self._currentplayer = self._nextplayer
                    self._nextplayer = temp

                    print("Nice Move! This is how the board looks now:")
                    Board.draw(self._board)
            else:
                print("Oops! This column is already full. You cannot put another piece here. Please try again!")

        

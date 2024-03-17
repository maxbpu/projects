import math
from models import *
from constants import *
import random

OPS = [0,1,2,3,4,5,6]

def check_win(pieces, x, y): 

        color = pieces[x][y]

        count = 0
        for i in range(BOARD_WIDTH):
            if pieces[i][y] == color:
                count += 1
            else:
                count = 0
            if count == 4:
                return (True, color)
        
        count = 0
        for i in range(BOARD_HEIGHT):
            if pieces[x][i] == color:
                count += 1
            else:
                count = 0
            if count == 4:
                return (True, color)
            
        #check /
        count = 0
                
        if x==y:
            for i in range(BOARD_HEIGHT):
                if pieces[i][i]==color:
                    count += 1
                else:
                    count = 0
                if count==4:
                    return (True, color)
        elif x>y:
            low_x = x - y
            low_y = 0
            for i in range(BOARD_WIDTH-low_x):
                if pieces[low_x+i][low_y+i]==color:
                    count += 1
                else: 
                    count = 0
                if count==4:
                    return (True, color)
        else:
            low_x = 0
            low_y = y - x
            for i in range(BOARD_HEIGHT-low_y):
                if pieces[low_x+i][low_y+i] ==color:
                    count += 1
                else:
                    count = 0
                if count==4:
                    return (True, color)
        

        #check \
        count = 0

        
        if x+y==6:
            for i in range(BOARD_HEIGHT):
                if pieces[i][BOARD_HEIGHT-i-1] ==color:
                    count += 1
                else:
                    count = 0
                if count==4:
                    return (True, color)
        elif x+y<6:
            low_x = 0
            low_y = x+y
            for i in range(low_y+1):
                if pieces[i][low_y-i]==color:
                    count += 1
                else:
                    count = 0
                if count==4:
                    return (True, color)
        else:
            low_y = BOARD_HEIGHT-1
            low_x = x + y - low_y
            for i in range(BOARD_WIDTH-low_x):
                if pieces[low_x+i][low_y-i] ==color:
                    count += 1
                else:
                    count = 0
                if count==4:
                    return (True, color)
       

        return (False, -1)

def findtop(board, o):
  for y in range(len(board[o])):
      if board[o][y] == 2:
        return y-1
  return -1

def copy(board):
    new = []
    for x in range(len(board)):
        sublist = []
        for y in range(len(board[x])):
            sublist.append(board[x][y])
        new.append(sublist)
    return new

def reverse(board):
  board1 = []
  l = len(board)
  for x in range(l):
    board1.append(board[l-x-1])
  return board1


def draw(board):
  for x in range(len(board)):
      for y in range(len(board[x])):
          if board[x][y] == 2:
            return False
  return True

def countpieces(board, player):
  count = 0
  for x in range(len(board)):
      for y in range(len(board[x])):
          if board[x][y] == player:
            count+=1
  return count


def terminal(board, og, lastmove, player):
  if lastmove is not None:
    y= findtop(board, lastmove)
    if not (y == -1):
      (win, p) = check_win(board, lastmove, y)
      return draw(board) or win
    else:
      return False
  else:
    return False
###CHECK x, y#######

def value(board, og, lastmove):
  y= findtop(board, lastmove)
  if draw(board):
    return 0
  (win, p) = check_win(board, lastmove, y)
  if win and p == og:
    return 1
  else:
    return -1

def apply(board, x, player):
  board1 = copy(board)
  if board[x][-1] == 2:
    for y in range(len(board1[x])):
        if board1[x][y] == 2:
          board1[x][y] = player
          break
    return (True, board1)
  else:
    return (False, board1)
  

def findleftx(board):
  for x in range(len(board)):
    for y in range(len(board[x])):
      if board[x][y] !=2:
        return x
  return -1

def findrightx(board):
  board1 = copy(board)
  board1 = reverse(board1)
  for x in range(len(board)):
    for y in range(len(board[x])):
      if board1[x][y] !=2:
        return BOARD_WIDTH - x -1
  return -1

def evaluate1(board, player):
  x1 = findleftx(board)
  if x1 == -1:
    return 0
  x2 = findrightx(board)
  y1 = 0
  ytemp = 0
  for x in range(len(board)):
    ytemp = findtop(board, x)
    if y1 < ytemp:
      y1 = ytemp
  y2 = 0
  total = (x2-x1+1)*(y1-y2+1)
  totalcount = countpieces(board, player)
  return (totalcount/total)

def checkvertical(board, player):
  chainmax = 0
  for x in range(BOARD_WIDTH):
    chain = 0
    for y in range(BOARD_HEIGHT):
      if board[x][y] == player:
        chain +=1
      else:
        if chain > chainmax:
          chainmax = chain
        chain = 0
    if chain >chainmax:
      chainmax = chain
  return chainmax

def checkhorizontal(board, player):
  chainmax = 0
  for y in range(BOARD_HEIGHT):
    chain = 0
    for x in range(BOARD_WIDTH):
      if board[x][y] == player:
        chain +=1
      else:
        if chain > chainmax:
          chainmax = chain
        chain = 0
    if chain >chainmax:
      chainmax = chain
  return chainmax

def checkforward(board, player):
  yinit = 3
  chainmax = 0
  while yinit >=0:
    x = 0
    y = yinit
    chain = 0
    while y <= BOARD_HEIGHT -1 and x <= BOARD_WIDTH -1:
      if board[x][y] == player:
        chain +=1
      else:
        if chain > chainmax:
          chainmax = chain
        chain = 0
      y+=1
      x+=1
    yinit -=1
    if chain >chainmax:
      chainmax = chain

  xinit = 1
  while xinit <= BOARD_WIDTH -1:
    x = xinit
    y = 0
    chain = 0
    while y <= BOARD_HEIGHT -1 and x <= BOARD_WIDTH -1:
      if board[x][y] == player:
        chain +=1
      else:
        if chain > chainmax:
          chainmax = chain
        chain = 0
      y+=1
      x+=1
    xinit +=1
    if chain >chainmax:
      chainmax = chain
  return chainmax

def checkbackward(board, player):
  yinit = 3
  chainmax = 0
  while yinit >=0:
    x = BOARD_HEIGHT-1
    y = yinit
    chain = 0
    while y <= BOARD_HEIGHT -1 and x >=0:
      if board[x][y] == player:
        chain +=1
      else:
        if chain > chainmax:
          chainmax = chain
        chain = 0
      y+=1
      x-=1
    yinit -=1
    if chain >chainmax:
      chainmax = chain

  xinit = BOARD_HEIGHT-1
  while xinit >=0:
    x = xinit
    y = 0
    chain = 0
    while y <= BOARD_HEIGHT -1 and x >=0:
      if board[x][y] == player:
        chain +=1
      else:
        if chain > chainmax:
          chainmax = chain
        chain = 0
      y+=1
      x-=1
    xinit -=1
    if chain >chainmax:
      chainmax = chain
  return chainmax


def evaluate2(board, player):
  h = checkhorizontal(board, player)
  v = checkvertical(board, player)
  f = checkforward(board, player)
  b = checkbackward(board, player)
  return max(h, v, f, b)

def evaluate(board, player, a1, a2, a3):
  m1 = evaluate1(board, player)
  m2 = evaluate2(board, player)
  m3 = evaluate2(board, 1-player)
  v = (a1*m1 + a2*m2 + a3*m3)
  if v <10 and v >1:
    v = v/10
  elif v < 100:
    v = v/100
  return v


def maxturn(board, lastmove, ops, player, og, depth, a1, a2, a3):
  bestop = -1
  if terminal(board, og, lastmove, player):
    return (value(board, og, lastmove), lastmove)
  elif depth >5:
    val = evaluate(board, og, a1, a2, a3)
    return ((val,lastmove))
  else:
    val = -math.inf
    for x in ops:
      (possible, board1) = apply(board, x, player)
      if possible:
        (val1, y) = minturn(board1, x, ops, (1-player), og, depth+1, a1, a2, a3)
        if val1 > val:
          val = val1
          bestop = x
        elif val1 == val:
          c = random.randint(0,1)
          if c == 0:
            bestop = x
    return((val, bestop))

def minturn(board, lastmove, ops, player, og, depth, a1, a2, a3):
  bestop = -1
  if terminal(board, og, lastmove, player):
    return (value(board, og, lastmove), lastmove)
  elif depth >5:
    val = evaluate(board, og, a1, a2, a3)
    return ((val,lastmove))
  else:
    val = math.inf
    for x in ops:
      (possible, board1) = apply(board, x, player)
      if possible:
        (val1, y) = maxturn(board1, x, ops, (1-player), og, depth+1, a1, a2, a3)
        if val1 < val:
          val = val1
          bestop = x
        elif val1 == val:
          c = random.randint(0,1)
          if c == 0:
            bestop = x
    return((val, bestop))


def choosebyminmax(board, player, lastmove, a1, a2, a3):
  (val, bestop) = maxturn(board, lastmove, OPS, player, player, 0, a1, a2, a3)
  return bestop



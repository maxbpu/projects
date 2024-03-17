from constants import *

class Piece():

    def __init__(self, color):
        self._color = color # 0, 1, 2, or 3 -> blue, red, white, yellow
        #self._valid = False # if True then is the last piece dropped

    def change_color(self, color):
        self._color = color
    
    def print_color(self):
        return COLORSSTR[self._color] # string of color names
    
    def get_color(self):
        return self._color # int for color


class Board():

    def __init__(self):
        pieces = []
        for i in range(BOARD_WIDTH):
            sub_list_pieces = []
            for j in range(BOARD_HEIGHT):
                new_piece = Piece(2)
                sub_list_pieces.append(new_piece)
            pieces.append(sub_list_pieces)

        self._height = BOARD_HEIGHT
        self._width = BOARD_WIDTH
        self._pieces = pieces # 2D list of pieces
        self._boardcolor = 3 # not using right now
    
    def get_pieces(self):
        return self._pieces
    
    def boardrep(self):
        new = []
        for x in range(len(self._pieces)):
            sublist = []
            for y in range(len(self._pieces[x])):
                sublist.append(self._pieces[x][y].get_color())
            new.append(sublist)

        return new 

    """
    Drop a piece to column x 
    If column x is full - return (False, None)
    If column x is not full - return (True, v) where v is the current height of x after dropping
    """
    def findm(self, x, color): 
        col = self._pieces[x]
        for v in range(BOARD_HEIGHT):
            if self._pieces[x][v].get_color()==2:
                self._pieces[x][v].change_color(color)
                return (True, v)
        return(False, None)


    """
    Check if dropping piece (x, y) leads to a winning situation
    """
    def check_win(self, x, y): 

        color = self._pieces[x][y].get_color()
        
        #check -
        count = 0
        for i in range(BOARD_WIDTH):
            if self._pieces[i][y].get_color() == color:
                #color1 = self._pieces[i][y].get_color()
                #print("- " +COLORSSTR[color1]+" "+str(color1))
                count += 1
            else:
                count = 0
            if count == 4:
                return True
        
        #check |
        count = 0
        for i in range(BOARD_HEIGHT):
            if self._pieces[x][i].get_color() == color:
                #color1 = self._pieces[x][i].get_color()
                #print("| " +COLORSSTR[color1]+" "+str(color1))
                count += 1
            else:
                count = 0
            if count == 4:
                return True
            
        #check /
        count = 0
                
        if x==y:
            for i in range(BOARD_HEIGHT):
                if self._pieces[i][i].get_color()==color:
                    #color1 = self._pieces[i][i].get_color()
                    #print("/ "+COLORSSTR[color1]+" "+str(color1))
                    count += 1
                else:
                    count = 0
                if count==4:
                    return True
        elif x>y:
            low_x = x - y
            low_y = 0
            for i in range(BOARD_WIDTH-low_x):
                if self._pieces[low_x+i][low_y+i].get_color()==color:
                    #color1 = self._pieces[low_x+i][low_y+i].get_color()
                    #print("/ "+COLORSSTR[color1]+" "+str(color1))
                    count += 1
                else:
                    count = 0
                if count==4:
                    return True
        else:
            low_x = 0
            low_y = y - x
            for i in range(BOARD_HEIGHT-low_y):
                if self._pieces[low_x+i][low_y+i].get_color()==color:
                    #color1 = self._pieces[low_x+i][low_y+i].get_color()
                    #print("/ "+COLORSSTR[color1]+" "+str(color1))
                    count += 1
                else:
                    count = 0
                if count==4:
                    return True
        

        #check \
        count = 0

        
        if x+y==6:
            for i in range(BOARD_HEIGHT):
                if self._pieces[i][BOARD_HEIGHT-i-1].get_color()==color:
                    #color1 = self._pieces[i][BOARD_HEIGHT-i-1].get_color()
                    #print("\ "+COLORSSTR[color1]+" "+str(color1))
                    count += 1
                else:
                    count = 0
                if count==4:
                    return True
        elif x+y<6:
            low_x = 0
            low_y = x+y
            for i in range(low_y+1):
                if self._pieces[i][low_y-i].get_color()==color:
                    #color1 = self._pieces[i][low_y-i].get_color()
                    #print("\ "+COLORSSTR[color1]+" "+str(color1))
                    count += 1
                else:
                    count = 0
                if count==4:
                    return True
        else:
            low_y = BOARD_HEIGHT-1
            low_x = x + y - low_y
            for i in range(BOARD_WIDTH-low_x):
                if self._pieces[low_x+i][low_y-i].get_color()==color:
                    #color1 = self._pieces[low_x+i][low_y-i].get_color()
                    #print("\ "+COLORSSTR[color1]+" "+str(color1))
                    count += 1
                else:
                    count = 0
                if count==4:
                    return True
       

        return False

    
    def draw(self):
        for i in range(BOARD_HEIGHT):
            temp = ""
            for j in range(BOARD_WIDTH):
                c = self._pieces[j][BOARD_HEIGHT-i-1].get_color()
                temp += COLORS[c]+"x  "
            print (temp)

            
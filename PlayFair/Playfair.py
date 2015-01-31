import re

table = [['*', '*', '*', '*', '*'],
         ['*', '*', '*', '*', '*'],
         ['*', '*', '*', '*', '*'],
         ['*', '*', '*', '*', '*'],
         ['*', '*', '*', '*', '*']]

no_j = [0] * 50
no_x = [0] * 50
j = 0
r = 0


def playfair(secret, plaintext):
    secret = input("Enter a keyword: ")
    create_table(secret)
    return encode(cleanup(plaintext))



def print_table():
    for row in range(5):
        for col in range(5):
            print table[row][col],

        print ""
        


def clear_table():
    for row in range(5):
        for col in range(5):
            table[row][col] = "*"



def table_has_letter(letter):
    for row in range(5):
        for col in range(5):
            if letter == table[row][column]:
                return true

    return false



def create_table(secret):
    clear_table()
    secret = secret.upper()
    secret = secret.replace(" ", "")
    keyword = ""
    alpha = "ABCDEFGHIKLMNOPQRSTUVWXYZ"
    z = 0
    

    for x in range(len(secret)):
        if secret[x] == 'J':
            secret = secret.replace('J', 'I')
            
        if keyword.find(secret[x]) == -1:
            keyword = keyword + secret[x]

    x = 0

    for x in range(len(alpha)):
        if keyword.find(alpha[x]) == -1:
            keyword = keyword + alpha[x]

    x = 0

    for row in range(5):
        for col in range(5):
            table[row][col] = keyword[x]
            x=x+1



def find_letter(letter):
    for x in range(5):
        for y in range(5):
            if table[x][y] == letter:
                return x, y



def encode_pair(a, b):
    for row in range(5):
        for col in range(5):
            if table[row][col] == a:
                a_row = row
                a_col = col
                if a == b:
                    b_row = row
                    b_col = col

            elif table[row][col] == b:
                if a == b:
                    a_row = row
                    a_col = col
                b_row = row
                b_col = col


    if a_row == b_row:
        if a_col == 4:
            a = table[a_row][0]

            if b_col != 4:
                b = table[b_row][b_col+1]

            else:
                b = table[b_row][0]

        elif b_col == 4:
            b = table[b_row][0]

            if a_col != 4:
                a = table[a_row][a_col+1]

            else:
                a = table[a_row][0]

        else:
            a = table[a_row][a_col+1]
            b = table[b_row][b_col+1]
    
    elif a_col == b_col:
        if a_row == 4:
            a = table[0][a_col]

            if b_row != 4:
                b = table[b_row+1][b_col]

            else:
                b = table[0][b_col]
            

        elif b_row == 4:
            b = table[0][b_col]

            if a_row != 4:
                a = table[a_row+1][a_col]

            else:
                a = table[0][a_col]

        else:
            if a_row == 4:
                a = table[0][a_col]

            else:
                a = table[a_row+1][a_col]

            if b_row == 4:
                b = table[0][b_col]

            else:
                b = table[b_row+1][b_col]

    else:
        a = table[a_row][b_col]
        b = table[b_row][a_col]

    return a+b



def cleanup(plaintext):
    plaintext = plaintext.replace(' ', '')
    plaintext = plaintext.upper()
    plaintext = plaintext.replace('J', 'I')
    plaintext = fix_one_problem(plaintext)
    

    return plaintext



def encode(plaintext):
    ciphertext = ""
    
    for x in range(0, len(plaintext), 2):
        ciphertext = ciphertext + encode_pair(plaintext[x], plaintext[x+1])

    return ciphertext



def fix_one_problem(plaintext):
    problem = 0
    restart = True

    while restart:
        restart = False
        new = ""

        if len(plaintext) % 2 == 1:
            for x in range(len(plaintext)):
                if x % 2 == 0 and x + 1 < len(plaintext):
                    new = new + plaintext[x]
                    if plaintext[x] == plaintext[x+1] and plaintext[x] != 'X':
                        new = new + 'X'
                        new = new + plaintext[x+1]
                        problem = problem + 1

                    elif plaintext[x] == plaintext[x+1] and plaintext[x] == 'X':
                        new = new + 'Q'
                        new = new + plaintext[x+1]
                        problem = problem + 1
                        
                    else:
                        new = new + plaintext[x+1]

                elif x % 2 == 0 and x + 1 == len(plaintext):
                    if plaintext[-1] == plaintext[-2] and len(plaintext) > 1:
                        new = new + 'X'
                        new = new + plaintext[-1]

                    else:
                        new = new + plaintext[-1]

        else:
            for x in range(0, len(plaintext), 2):
                new = new + plaintext[x]
                
                if plaintext[x] == plaintext[x+1] and plaintext[x] != 'X':
                    new = new + 'X'
                    new = new + plaintext[x+1]
                    problem = problem + 1

                elif plaintext[x] == plaintext[x+1] and plaintext[x] == 'X':
                    new = new + 'Q'
                    new = new + plaintext[x+1]
                    problem = problem + 1

                else:
                    new = new + plaintext[x+1]

        x = 0

        if len(plaintext) % 2 == 0:        
            if plaintext[x] == plaintext[x+1]:
                plaintext = new
                restart = True
            
    if len(new) % 2 == 1 and plaintext[-1] != 'Z':
        new = new + 'Z'
        problem = problem + 1

    elif len(new) % 2 == 1 and plaintext[-1] == 'Z':
        new = new + 'Q'
        problem = problem + 1

    if problem > 0:
        return new

    
    return plaintext



def playfair(secret, plaintext):
    create_table(secret)
    print print_table()
    return encode(cleanup(plaintext))


def decode_pair(a,b):
    for row in range(5):
        for col in range(5):
            if table[row][col] == a:
                a_row = row
                a_col = col
                if a == b:
                    b_row = row
                    b_col = col

            elif table[row][col] == b:
                if a == b:
                    a_row = row
                    a_col = col
                b_row = row
                b_col = col

    if a_row == b_row:
        if a_col == 0:
            a = table[a_row][4]

            if b_col != 0:
                b = table[b_row][b_col-1]

            else:
                b = table[b_row][4]

        elif b_col == 0:
            b = table[b_row][4]

            if a_col != 0:
                a = table[a_row][a_col-1]

            else:
                a = table[a_row][4]

        else:
            a = table[a_row][a_col-1]
            b = table[b_row][b_col-1]
    
    elif a_col == b_col:
        if a_col == 0:
            a = table[4][a_col]

            if b_col != 0:
                b = table[b_row-1][b_col]

            else:
                b = table[4][b_col]
            

        elif b_col == 0:
            b = table[4][b_col]

            if a_col != 0:
                a = table[a_row-1][a_col]

            else:
                a = table[4][a_col]

        else:
            if a_row == 0:
                a = table[4][a_col]

            else:
                a = table[a_row-1][a_col]

            if b_row == 0:
                b = table[4][b_col]

            else:
                b = table[b_row-1][b_col]


    else:
        a = table[a_row][b_col]
        b = table[b_row][a_col]

    return a+b



def playfair_decode(secret, ciphertext, count, maximum):
    plaintext = ""
    check = False
    for x in range(0, len(ciphertext), 2):
        plaintext = plaintext + decode_pair(ciphertext[x], ciphertext[x+1])

    if plaintext[-1] == 'Z' or plaintext[-1] == 'Q':
        plaintext = plaintext.replace(plaintext[-1], '')

    x = 0
    new = ""
    i = 0
    
    if r == 0:
        plaintext = plaintext.replace('X', '')

    for x in range(len(plaintext)):
        if x == count[i] and i < maximum:
            new = new + ' '
            i = i + 1

        new = new + plaintext[x]

    x = 0
    temp = ""

    if j > 0:
        new = new.replace('I', 'J')

    temp2 = ""
    if r > 0:
        temp = new.split('X')
        b = 0
        for i in temp:
            temp2 = temp2 + temp[b]
            b = b+1
            
            if len(temp2) == no_x[x]:
                check = True
                temp2 = temp2 + 'X'
                x = x+1
                
    if check == True:
        return temp2

    return new

if __name__ == "__main__":
    s = raw_input("Enter the secret key: ")
    p = raw_input("Enter the plaintext: ")
    count = [0] * 50
    i = 0
    t = 0
    
    for x in range(len(p)):
        if p[x] == ' ':
            count[i] = x - t
            i = i+1
            t = t+1

        if p[x] == 'J' or p[x] == 'j':
            no_j[j] = x
            j = j + 1

        if p[x] == 'X' or p[x] == 'x':
            no_x[r] = x
            r = r + 1
                    
    c = playfair(s, p)
    print "Ciphertext: ", c
    print "Decoded text: ", playfair_decode(s, c, count, i)
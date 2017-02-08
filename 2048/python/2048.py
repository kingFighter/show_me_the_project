# -*- coding: utf-8 -*-
import curses
from random import randrange, choice
from collections import defaultdict

letterCodes = [ord(ch) for ch in 'WASDRQwasdrq']
actions = ['Up', 'Left', 'Down', 'Right', 'Restart', 'Exit']
actionsDict = dict(zip(letterCodes, actions * 2))

def getUserAction(k):
    ch = 'N'
    while ch not in actionsDict:
        ch = k.getch()
    return actionsDict[ch]

def transpose(field):
    return [list(row) for row in zip(*field)]

def invert(field):
    return [row[::-1] for row in field]

class GameField(object):
    def __init__(self, height = 4, width = 4, winScore = 2048):
        self.height = height
        self.width = width
        self.winScore = winScore
        self.score = 0
        self.highestScore = 0
        self.reset()
        
    def reset(self):
        if self.score > self.highestScore:
            self.highestScore = self.score
        self.score = 0
        self.field = [[0 for i in range(self.width)] for j in range(self.height)]
        self.spawn()
        self.spawn()

    def move(self, direction):
        def moveLeft(row):
            def tighten(row):
                newRow = [i for i in row if i != 0]
                newRow += [0 for i in range(len(row) - len(newRow))]
                return newRow
            def merge(row):
                newRow = []
                pair = False
                for i in range(len(row)):
                    if pair:
                        newRow.append(2 * row[i])
                        self.score += 2 * row[i]
                        pair = False
                    else:
                        if i + 1 < len(row) and row[i] == row[i + 1]:
                            pair = True
                            newRow.append(0)
                        else:
                            newRow.append(row[i])
                return newRow
            return tighten(merge(tighten(row)))
        
        moves = {}
        moves['Left'] = lambda field: [moveLeft(row) for row in field]
        moves['Right'] = lambda field:  invert(moves['Left'](invert(field)))
        moves['Up'] = lambda field: transpose(moves['Left'](transpose(field)))
        moves['Down'] = lambda field: transpose(moves['Right'](transpose(field)))
        
        if direction in moves:
            if self.canMove(direction):
                self.field = moves[direction](self.field)
                self.spawn()
                return True
            else:
                return False
    def isWin(self):
        return any(any(i >= self.winScore for i in row) for row in self.field)

    def isGameOver(self):
        return not any(self.canMove(move) for move in actions)
            
    def draw(self, screen):
        helpStr1 = '(W)Up (S)Down (A)Left (D)Right'
        helpStr2 = '     (R)Restart (Q)Exit'
        gameoverStr = '           GAME OVER'
        winStr = '          YOU WIN!'
        def cast(str):
            screen.addstr(str + '\n')

        def drawHorSeparator():
            line = '+' + ('+------' * self.width + '+')[1:]
            cast(line)

        def drawRow(row):
            cast(''.join('|{: ^5} '.format(num) if num > 0 else '|      ' for num in row) + '|')

        screen.clear()
        cast('SCORE: ' + str(self.score))
        if 0 != self.highestScore:
            cast('HGHSCORE: ' + str(self.highestScore))
        for row in self.field:
            drawHorSeparator()
            drawRow(row)
        drawHorSeparator()
        if self.isWin():
            cast(winStr)
        else:
            if self.isGameOver():
                cast(gameoverStr)
            else:
                cast(helpStr1)
        cast(helpStr2)

    def spawn(self):
        newEle = 4 if randrange(100) >= 90 else 2
        (i, j) = choice([(i, j) for i in range(self.width) for j in range(self.height) if self.field[i][j] == 0])
        self.field[i][j] = newEle

    def canMove(self, direction):
        def canMoveLeft(row):
            for i in range(len(row) - 1):
                if row[i] == 0 and row[i + 1] != 0:
                    return True
                elif row[i] != 0 and row[i] == row[i + 1]:
                    return True
            return False

        check = {}
        check['Left'] = lambda field: any(canMoveLeft(row) for row in field)
        check['Right'] = lambda field:  check['Left'](invert(field))
        check['Up'] = lambda field:  check['Left'](transpose(field))
        check['Down'] = lambda field:  check['Right'](transpose(field))
        
        if direction in check:
            return check[direction](self.field)
        else:
            return False

def main(stdscr):
    def init():
        gameField.reset()
        return 'Game'

    def notGame(state):
        gameField.draw(stdscr)
        action = getUserAction(stdscr)
        responses = defaultdict(lambda: state)
        responses['Restart'], responses['Exit'] = 'Init', 'Exit'
        return responses[action]

    def game():
        gameField.draw(stdscr)
        action = getUserAction(stdscr)
        if action == 'Restart':
            return 'Init'
        if action == 'Exit':
            return 'Exit'
        if gameField.move(action):
            if gameField.isWin():
                return 'Win'
            elif gameField.isGameOver():
                return 'Gameover'
        return 'Game'

    stateActions = {
        'Init': init,
        'Win': lambda: notGame('Win'),
        'Gameover': lambda: notGame('Gameover'),
        'Game': game
    }

    curses.use_default_colors()
    gameField = GameField()
    state = 'Init'
    while state != 'Exit':
        state = stateActions[state]()

curses.wrapper(main)

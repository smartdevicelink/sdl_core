#! /usr/bin/python3
import re
import getopt
import sys


def man():
    #print manual to use script
    print('''
Script that removes code under undefined #ifdef and #ifndef directives

Usage:
    -h --help \t\t\t: Show this hint
    -d <ARG> --define <ARG>\t: define variable
    -u <ARG> --undef <ARG>\t: Undefine variable
    -i <ARG> --input <ARG>\t: setup path to input file. MANDATORY!

Example:
    python3 define_parser.py -d CUSTOMER_FORD -d CUSTOMR_VAZ -u CUSTOMER_PASA -i ./test.txt
    ''')

#List of defined variables
defined = []

#List of undefined variables
undefined = []

#Input file name
input_file_name = ""


class Stack(list):
    #Simple stack realisation
    #if stack eis empty it will return None value
    def push(self, item):
        self.append(item)

    def pop(self):
        if len(self) == 0:
            return None
        return super(Stack, self).pop()

    def size(self):
        return len(self)

    def head(self):
        if len(self) == 0:
            return None
        return self[self.size() - 1]

    def foot(self):
        #acces to first element of stack
        if len(self) == 0:
            return None
        return self[0]


class Logger:
    #class to log some text into file
    # file will be appended by output info
    def __init__(self, name, pre=""):
        self.file = open(name, "a+")
        self.pre = pre

    def __del__(self):
        self.file.close()

    def log(self, logger_message):
        string = self.pre + ":" + logger_message + "\n"
        self.file.write(string)


class TokenType:
    #enum
    code_token = 1
    ifdef_token = 2
    if_token = 3
    ifndef_token = 4
    else_token = 5
    endif_token = 6
    comment_token = 7
    Name = 8  #Stub


token_specification = [
    #regexp to split code to tokens
    (TokenType.comment_token, "(/\*.*?\*/)"),  #multiline comment block)
    (TokenType.comment_token, "(//.*\n)"),  #oneline comment block
    (TokenType.ifdef_token, "([ |\t]*#[ |\t]*ifdef[ |\t]*([A-Za-z_][A-Za-z0-9_]*)\n)"),  #ifdef token
    (TokenType.Name, "[\a]"),  #stub to save order of groups and TokenType enum
    (TokenType.ifndef_token, "([ |\t]*#[ |\t]*ifndef[ |\t]*([A-Za-z_][A-Za-z0-9_]*)\n)"),  #ifndef token
    (TokenType.Name, "[\a]"),  #stub to save order of groups and TokenType enum
    (TokenType.if_token, "([ |\t]*#[ |\t]*if[ |\t]*.*\n)"),  #if token
    (TokenType.else_token, "([ |\t]*#[ |\t]*else[ |\t]*\n?)"),  #else token
    (TokenType.endif_token, "([ |\t]*#[ |\t]*endif[ |\t]*/?/?.*\n?)"),  #endif token
    (TokenType.code_token, "(.*)")  # any other code block
]


def parce_type(group):
    #return type of token from group.
    # group is output of "re" module
    assert (len(group) == len(token_specification))
    for i in range(len(group)):
        if group[i] is not None:
            #order of tokens in group is the same as in token_specification
            txt = group[i]
            if txt == '':
                txt = "\n"
            token_type = token_specification[i][0]
            name = None
            if ((token_type == TokenType.ifdef_token) or
                    (token_type == TokenType.ifndef_token)):
                name = group[i + 1]
                if (name not in defined) and (name not in undefined):
                    token_type = TokenType.if_token
                    name = None
            return txt, token_type, name


class Token:
    #class than holds text of token, type of token and name of ifdef name if exist
    def __init__(self, group):
        self.text, self.type, self.name = parce_type(group)
        if self.name is not None:
            self.text += self.name


def join_code_tokens(tokens):
    # Join multiple code tokens to one code token
    result = []
    last_code_token = None
    for token in tokens:
        if token.type == TokenType.code_token:
            if last_code_token is None:
                last_code_token = token
            else:
                last_code_token.text += token.text
        else:
            if last_code_token is not None:
                result.append(last_code_token)
                last_code_token = None
            result.append(token)
    if last_code_token is not None:
        result.append(last_code_token)
    return result


def split_to_tokens(text):
    # Split raw text to tokens
    regexp = "|".join(expr[1] for expr in token_specification)
    exp = re.compile(regexp)
    result = []
    found = exp.finditer(text)
    for x in found:
        x = Token(x.groups())
        result.append(x)
    return join_code_tokens(result[:-1]) # -1 os to remove extra space in the end of file


class State:
    # Class holds state of named ifdefs.
    # Class know if it allowed printing inside state or not
    def __init__(self, customer=None, not_flag=False):
        self.customer = customer
        self.not_flag = not_flag

    def is_printable(self):
        if self.customer in defined:
            if not self.not_flag:
                return True
            else:
                return False
        elif self.customer in undefined:
            if not self.not_flag:
                return False
            else:
                return True


def remove_undefined(tokens, logger=Logger("Log.txt")):
    # removes undefined code
    # ifdefs that are not in defined ot undefined ignores
    state_stack = Stack()  # stack for defined and undefined blocks
    ifstack = Stack()  #stack for all if's and ifdef's blocks
    for x in tokens:
        if x.type == TokenType.ifdef_token:
            state = State(x.name, False)
            state_stack.push(state)
            ifstack.append(x.name)
            continue

        elif x.type == TokenType.ifndef_token:
            state = State(x.name, True)
            state_stack.push(state)
            ifstack.append(x.name)
            continue

        elif x.type == TokenType.if_token:
            ifstack.append(None)

        elif x.type == TokenType.endif_token:
            if ifstack.size() == 0:
                logger.log("Warning! Bad document structure. Probably extra #endif")
            name = ifstack.pop()
            if name is not None:
                state = state_stack.pop()
                if state.customer != name:
                    #Warning! Some thing is going wrong
                    logger.log("Warning! System Error!" +
                               "Please contact to akutsan@luxoft.com. Attach input file, " +
                               "defined and undefined variables")
                continue
        elif x.type == TokenType.else_token:
            if ifstack.size() == 0:
                logger.log("Warning! Bad document structure. Probably #else without #if token")
            name = ifstack.head()
            if name is not None:
                state = state_stack.head()
                state.not_flag = not state.not_flag
                continue

        state = state_stack.foot()
        printable = True
        if state is not None:
            for state in state_stack:
                if not state.is_printable():
                    printable = False
                    break
        if printable:
            print(x.text, end="")


options, remainder = getopt.getopt(sys.argv[1:], 'd:u:h:i:', ["define=", "undef=", "help", "input="])

if len(options) == 0:
    man()
    exit()

for opt, arg in options:
    if opt in ("-h", "--help"):
        man()
        exit()
    elif opt in ("-d", "--define"):
        defined.append(arg)
    elif opt in ("-u", "--undef"):
        undefined.append(arg)
    elif opt in ("-i", "--input"):
        input_file_name = arg

if input_file_name == "":
    man()
    exit()

f = open(input_file_name, "r+")
file_text = f.read()
f.close()
file_tokens = split_to_tokens(file_text)
remove_undefined(file_tokens)

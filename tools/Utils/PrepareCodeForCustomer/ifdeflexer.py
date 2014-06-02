#! /usr/bin/python

import re

class Fatal(BaseException):
    # With this exception we can't continue process file
    def __init__(self, text):
        super(Fatal, self).__init__()
        self.text = text

class Error(BaseException):
    # With this exception  we can finish processing
    # But result probably will be wrong
    def __init__(self, text):
        super(Error, self).__init__()
        self.text = text

class Warning(BaseException):
    # Some minor warning in document structure
    def __init__(self, text):
        super(Error, self).__init__()
        self.text = text

class SystemError(Fatal):
    # Some thing very bad
    def __init__(self, text):
        super(SystemError, self).__init__()


class ParceError(Error):
    def __init__(self, text):
        super(ParceError, self).__init__(text)

class BadStructure(Fatal):
    def __init__(self, text):
        super(BadStructure, self).__init__(text)



class TokenType: #enum
    code_token = 1
    ifdef_token = 2
    if_token = 3
    ifndef_token = 4
    else_token = 5
    endif_token = 6
    comment_token = 7


class Token:
    def __init__(self, group):
        self.text, self.type = Token.parce_type(group)

    token_specification = [
        (TokenType.comment_token, "(/\*.*?\*/)"), #multiline comment block)
        (TokenType.comment_token,  "(//.*\n)"), #oneline comment block
        #(TokenType.ifdef_token, "([ |\t]*#[ |\t]*ifdef[ |\t]*[A-Za-z_][A-Za-z0-9_]*)"), #ifdef token
        #(TokenType.ifndef_token, "([ |\t]*#[ |\t]*ifndef[ |\t]*[A-Za-z_][A-Za-z0-9_]*)"), #ifndef token
        (TokenType.if_token, "([ |\t]*#[ |\t]*if[ |\t]*.*\n)"), #if token
        (TokenType.else_token, "([ |\t]*#[ |\t]*else[ |\t]*\n?)"), #else token
        (TokenType.endif_token, "([ |\t]*#[ |\t]*endif[ |\t]*/?/?.*\n?)"), #endif token
        (TokenType.code_token,"(.*)")# any other code block
    ]

    @staticmethod
    def parce_type(group):
        i = 0
        assert (len(group) == len(Token.token_specification))
        for i in range(len(group)):
            if (group[i] != None):
                txt = group[i]
                if (txt == ''):
                    txt = "\n"
                return txt, Token.token_specification[i][0]

def join_tokens(tokens):
    result = []
    last_code_token = None
    for token in tokens:
        if (token.type == TokenType.code_token):
            if (last_code_token == None):
                last_code_token = token
            else:
                last_code_token.text += token.text
        else:
            if (last_code_token != None):
                result.append(last_code_token)
                last_code_token = None
            result.append(token)
    if (last_code_token != None):
        result.append(last_code_token)
    return result

def split_to_tokens(text):
    regexp  = "|".join(expr[1] for expr in Token.token_specification)
    exp = re.compile(regexp)
    result = []
    for x in exp.finditer(text):
        x = Token(x.groups())
        result.append(x)
    return join_tokens(result) #join__code_tokens(result)


class SyntaxBlock(object):
    def __init__(self):
        self.sub_bloks = []

    def addSubBlock(self, sub_block):
        assert isinstance(sub_block, SyntaxBlock)
        self.sub_bloks.append(sub_block)
    def Code(self,customer_name):
        text = ""
        for block in self.sub_bloks:
            text += block.Code(customer_name)
        return text

class CodeLine(SyntaxBlock):
    def __init__(self, lexema):
        super(CodeLine, self).__init__()
        assert isinstance(lexema, Token)
        self.lexema = lexema
        #assert (lexema.type == TokenType.code_token)

    def addSubBlock(self,lexema):
        raise SystemError("Can't add subblock in CodeLine syntax block")

    def Code(self, customer_name):
        return self.lexema.text


class IfStatement(SyntaxBlock):
    def __init__(self):
        super(IfStatement, self).__init__();


class CustomerIfDefStatement(SyntaxBlock):
    def __init__(self, customer_name):
        super(CustomerIfDefStatement, self).__init__();
        self.customer_name = customer_name
        self.else_blocks = []
    def addElseBlock(self, sub_block):
        assert isinstance(sub_block, SyntaxBlock)
        self.else_blocks.append(sub_block)

    def Code(self,customer_name):
        text = ""
        if (customer_name == self.customer_name):
            for block in self.sub_bloks:
                text += block.Code(customer_name)
        else:
            for block in self.else_blocks:
                text += block.Code(customer_name)
        return text

class CustomerIfNDefStatement(SyntaxBlock):
    def __init__(self, customer_name):
        super(CustomerIfNDefStatement, self).__init__();
        self.customer_name = customer_name
        self.else_blocks = []

    def addElseBlock(self, sub_block):
        assert isinstance(sub_block, SyntaxBlock)
        self.else_blocks.append(sub_block)
    def Code(self,customer_name):
        text = ""
        if (customer_name != self.customer_name):
            for block in self.sub_bloks:
                text += block.Code(customer_name)
        else:
            for block in self.else_blocks:
                text += block.Code(customer_name)
        return text


class LexicalParcer:

    @staticmethod
    def parceSyntaxBlock(tokens):
        result = SyntaxBlock()
        while (len(tokens) > 0):
            element = tokens[len(tokens)-1]
            assert isinstance(element, Token)
            if (element.type == TokenType.code_token):
                code_line = CodeLine(tokens.pop())
                result.addSubBlock(code_line)

            elif (element.type == TokenType.comment_token):
                code_line = CodeLine(tokens.pop())
                result.addSubBlock(code_line)

            elif (element.type == TokenType.if_token):
                if_satatment = LexicalParcer.parceIf(tokens)
                result.addSubBlock(if_satatment)

            # elif (element.type == TokenType.ifndef_token):
            #     ifndef_satatment = LexicalParcer.parceIfndef(tokens)
            #     result.append(ifndef_satatment)

            elif (element.type == TokenType.else_token):
                raise ParceError("else without if")

            elif (element.type == TokenType.endif_token):
                raise ParceError("endif without if")

            else:
                raise SystemError("Invalid TokenType WTF?")
        return result

    @staticmethod
    def parceIf(tokens):
        element = tokens[len(tokens)-1]
        assert element.type == TokenType.if_token
        ifdef_customer_regexp = "[ |\t]*#[ |\t]*ifdef[ |\t]*CUSTOMER_([A-Za-z0-9_].*)?"
        ifndef_customer_regexp = "[ |\t]*#[ |\t]*ifndef[ |\t]*CUSTOMER_([A-Za-z0-9_].*)?"
        ifdef_match = re.search(ifdef_customer_regexp, element.text)
        ifndef_match = re.search(ifndef_customer_regexp, element.text)
        customer_name = None
        if (ifdef_match != None):
            if(len(ifdef_match.groups()) > 0):
                customer_name = ifdef_match.groups()[0]
                return LexicalParcer.parceCustomerIfdef(tokens, customer_name)
            else:
                raise SystemError("Can't get customer name?")
        if (ifndef_match != None):
            if(len(ifndef_match.groups()) > 0):
                customer_name = ifndef_match.groups()[0]
                return LexicalParcer.parceCustomerIfdef(tokens, customer_name, ifdef = False)
            else:
                raise SystemError("Can't get customer name?")
        result = IfStatement()
        code_line = CodeLine(tokens.pop())
        result.addSubBlock(code_line)
        while (len(tokens) > 0):
            element = tokens[len(tokens) - 1]
            assert isinstance(element, Token)

            if (element.type == TokenType.code_token):
                code_line = CodeLine(tokens.pop())
                result.addSubBlock(code_line)

            elif (element.type == TokenType.comment_token):
                code_line = CodeLine(tokens.pop())
                result.addSubBlock(code_line)

            elif (element.type == TokenType.if_token):
                if_satatment = LexicalParcer.parceIf(tokens)
                result.addSubBlock(if_satatment)

            elif (element.type == TokenType.else_token):
                code_line = CodeLine(tokens.pop())
                result.addSubBlock(code_line)

            elif (element.type == TokenType.endif_token):
                code_line = CodeLine(tokens.pop())
                result.addSubBlock(code_line)
                return result
            else:
                raise SystemError("Invalid TokenType?")
        raise ParceError("If without else statement?")


    @staticmethod
    def parceCustomerIfdef(tokens, customer_name, ifdef = True):
        element = tokens[len(tokens)-1]
        assert element.type == TokenType.if_token
        if (ifdef):
            result = CustomerIfDefStatement(customer_name)
        else:
            result = CustomerIfNDefStatement(customer_name)
        tokens.pop()
        else_ = False
        while (len(tokens) > 0):
            element = tokens[len(tokens)-1]
            assert isinstance(element, Token)
            if (element.type == TokenType.code_token):
                code_line = CodeLine(tokens.pop())
                result.addElseBlock(code_line) if else_ else result.addSubBlock(code_line)

            elif (element.type == TokenType.comment_token):
                code_line = CodeLine(tokens.pop())
                result.addElseBlock(code_line) if else_ else result.addSubBlock(code_line)

            elif (element.type == TokenType.if_token):
                if_satatment = LexicalParcer.parceIf(tokens)
                result.addElseBlock(if_satatment) if else_ else result.addSubBlock(if_satatment)

            elif (element.type == TokenType.else_token):
                if (else_):
                    raise SystemError("Parcer should stop parse if statement on first else")
                else_ = True
                tokens.pop()

            elif (element.type == TokenType.endif_token):
                tokens.pop()
                return result

            else:
                raise SystemError("Invalid TokenType?")
        raise ParceError("If without else statement?")
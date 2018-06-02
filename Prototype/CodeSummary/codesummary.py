# A code summary script for counting code lines.
import fileinput
import os

# base class for code summary of one file
class CodeSummary:
    def __init__(self, filename):
        self.filename = filename
        self.count = 0

    def IsLineOmit(self, line):
        return len(line) == 0

    def CalCodeLines(self):
        file = open(self.filename, 'r')
        for line in file.readlines():
            line = line.strip() # remove whitespace at front/end of line.
            if self.IsLineOmit(line):
                continue
            self.count += 1

    def PrintResult(self):
        print('Code lines of ', self.filename, ':', self.count)

# subclass of c file code summary
class CppCodeSummary(CodeSummary):
    # todo: cpp file line empty or with comments. not include /*..*/ now.
    def IsLineOmit(self, line):
        return len(line) == 0 or line.startswith('//')

# subclass of py file code summary
class PyCodeSummary(CodeSummary):
    # todo: not include '''/""" comment
    def IsLineOmit(self, line):
        return len(line) == 0 or line.startswith('#')

def CalCodeLinesForFile(filename):

    filetext = os.path.splitext(filename)
    cFileList = ['.h','.c','.hpp','.hxx','.cpp','.cxx']
    pyFileList = ['.py']
    qualifiedFileList = cFileList + pyFileList

    if filetext[1] not in qualifiedFileList:
        return 0

    # do we have other ways to declare codeSum var???
    codeSum = CodeSummary(filename)

    if filetext[1] in cFileList:
        codeSum = CppCodeSummary(filename)
    elif filetext[1] == '.py':
        codeSum = PyCodeSummary(filename)

    codeSum.CalCodeLines()
    codeSum.PrintResult()

    return codeSum.count

def CalCodeLinesForDir(fileSpec):
    wholeCount = 0
    for root, dirs, files in os.walk(fileSpec):
        for file in files:
            filename = os.path.join(root,file)
            wholeCount += CalCodeLinesForFile(filename)
    print('whole code lines of this repo is: ', wholeCount)


if __name__ == '__main__':
    fileSpec = input('please specify folder or file to calculate code lines:')
    #fileSpec = 'C:\Users\Administrator\Desktop\test.cpp'
    if os.path.isfile(fileSpec):
        CalCodeLinesForFile(fileSpec)
    elif os.path.isdir(fileSpec):
        CalCodeLinesForDir(fileSpec)


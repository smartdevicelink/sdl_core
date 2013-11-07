class CodeBlock:
    indent = 0
    def __init__(self, out):
        self.out = out
        if type(out) == file:
            self.indent = 0
        elif type(out) == CodeBlock:
            self.indent = out.indent
    def __enter__(self):
        self.indent += 1
        return self
    def __exit__(self, exc_type, exc_value, traceback):
        if self.indent >= 1:
            self.indent -= 1
    def write(self, s):
        self.out.write('  ' * self.indent + s)
        
# vim: set ts=4 sw=4 et:
		

import sys
#print(len(sys.argv), sys.argv)
args = sys.argv[1:]


	

class Validator:
	customer_tamplate = "CUSTOMER_"
	ifdef_ = "#ifdef "
	startif_ = "#if"
	endif_ = "#endif"
	
	def __init__(self, customer_name):
		if (customer_name == None ):
			return
		self.customer_name = customer_name
		self.my_customer_block = False
		self.other_customer_block = False
		self.if_deep = 0
		
	def re_init(self, file_name, customer_name):
		self.file_lines = None
		self.customer_name = customer_name
		file_to_read = open(file_name,"rb")
		self.file_lines = file_to_read.readlines
		
	def validate(self, line):
		if (self.if_deep < 0):
			raise BaseException("deep < 0")
			
		if (self.other_customer_block and self.my_customer_block):
			raise BaseException("other_customer_block == other_customer_block")
		
		if (self.other_customer_block == True):
			if (self.any_customer_line(line) == True):
				raise BaseException("Recurcive customers bloks")
			if (self.start_if(line) == True):
				self.if_deep += 1
				return False
			if (self.end_if(line) == True):
				if (self.if_deep == 0):
					self.other_customer_block = False
					return False
				self.if_deep -= 1
				return False
			return False
		
		if (self.my_customer_block == True):
			if (self.any_customer_line(line) == True):
				raise BaseException("Recurcive customers bloks") 
			if (self.start_if(line) == True):
				self.if_deep += 1
				return True
			if (self.end_if(line) == True):
				if (self.if_deep == 0):
					self.my_customer_block = False
					return False
				self.if_deep -= 1
				return True
			return True
		
		if (self.my_customer_line(line) == True):
			if (self.if_deep > 0):
				raise BaseException("Can't start customer section with not null @if deep")
			self.my_customer_block = True
			self.if_deep = 0
			return False
			
		if (self.other_customer_line(line) == True):
			if (self.if_deep > 0):
				raise BaseException("Can't start customer section with not null @if deep")
			self.other_customer_block = True
			self.if_deep = 0
			return False
		return True
		
	
	def my_customer_line(self, line):
		ifdefline = self.ifdef_ + self.customer_tamplate + self.customer_name
		if ( line.find(ifdefline) == -1):
			return False
		return True
	
	def any_customer_line(self, line):
		ifdefline = self.ifdef_ + self.customer_tamplate
		if ( line.find(ifdefline) == -1):
			return False
		return True
		
	def other_customer_line(self, line):
		ifdefline = self.ifdef_ + self.customer_tamplate
		pos = line.find(ifdefline)
		if ( pos == -1):
			return False
		last = line[pos + len(ifdefline):]
		if (last.find(self.customer_name) == 0):
			return False
		return True
		
	def start_if(self, line):
		if (line.find(self.startif_) != -1):
			return True
		return False

	def end_if(self, line):
		if (line.find(self.endif_) != -1):
			return True
		return False

'''
	def chek_ready(self):
		if (self.file_lines == None):
			raise BaseException("Validator is not ready")
'''
class Remover:
	def __init__(self, file_name, customer_name):
		input_file = open(file_name, "rb")
		self.lines = input_file.readlines()
		input_file.close()
		self.validator = Validator(customer_name)
		
	def get_parsed_lines(self):
		v = self.validator
		output = []
		for line in self.lines:
			if v.validate(line):
				output.append(line)
		return output
		
	
	

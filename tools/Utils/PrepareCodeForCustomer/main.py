import sys
import customer_prepare

def is_diff(lines1, lines2):
	length1 = len(lines1)
	length2 = len(lines2)
	if (length1 != length2):
		return True
	for i in range(length1):
		l1 = lines1[i]
		l2 = lines2[i]
		if (l1 != l2):
			return True
	return False

def save_lines(f_name, lines):
	f = open(f_name,"w+")
	for l in lines:
		f.write(l)
	f.close()
	 
def main():
	print "RUN"
	customer = sys.argv[1]
	files = sys.argv[2:]
	for input_file in files:
		print "Process: ", input_file,
		r = customer_prepare.Remover(input_file, customer)
		parced_lines = r.get_parsed_lines()
		input_lines = r.lines
		#save_lines("out", parced_lines)
		#save_lines("inp", input_lines)
		if is_diff(input_lines, parced_lines):
			print "Changes"
			save_lines(input_file, parced_lines)
		else :
			print ""
	return 0

if __name__ == '__main__':
	main()

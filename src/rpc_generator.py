import os.path
import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv[1])
print 'Argument List:', str(sys.argv[2])

# Args
# 1 Class name "ButtonPress"
# 2 lowercase with underscores for filename/includes "button_press"
# 3 HMI function ID "Buttons_ButtonPress"
# 4 HMI class def "ButtonsButtonPress"
# 5 Hmi class def lower case for filename/includes "buttons_button_press" 

d = {}

#Mobile Variable 

d['mobileClassRequest'] = str(sys.argv[1]) + "Request"
d['mobileClassResponse'] = str(sys.argv[1]) + "Response"

d['mobile_request_header_filename'] = str(sys.argv[2]) + "_request.h"
d['mobile_response_header_filename'] = str(sys.argv[2]) + "_response.h"

d['mobile_request_filename'] = str(sys.argv[2]) + "_request.cc"
d['mobile_response_filename'] = str(sys.argv[2]) + "_response.cc"

d['UPPERCASE_INCLUDE'] = str(sys.argv[2]).upper()

#HMI Variables

d['HMIGroup_FunctionName'] = str(sys.argv[3])

d['HMIClassRequest'] = str(sys.argv[4]) + "Request"

d['HMIClassResponse'] = str(sys.argv[4]) + "Response"

d['hmigroup_function_name'] = str(sys.argv[5])

d['hmi_request_header_filename'] = str(sys.argv[5]) + "_request.h" 

d['hmi_response_header_filename'] = str(sys.argv[5]) + "_response.h" 

d['HMI_UPPERCASE_INCLUDE'] = str(sys.argv[5]).upper()

#Create Mobile Class definitions

sourceDir = os.path.dirname(os.path.realpath(__file__))

#Mobile Request and Response Implementations

headerPathFromSrc = "components/application_manager/src/commands/mobile/"

with open("mobile_request_template", 'r') as ftemp:
    templateString = ftemp.read()
with open(os.path.join(sourceDir, headerPathFromSrc,str(sys.argv[2])+"_request.cc"), 'w') as f:
    f.write(templateString.format(**d))

with open("mobile_response_template", 'r') as ftemp:
    templateString = ftemp.read()
with open(os.path.join(sourceDir, headerPathFromSrc,str(sys.argv[2])+"_response.cc"), 'w') as f:
    f.write(templateString.format(**d))

#Mobile Request and Response headers

headerPathFromSrc = "components/application_manager/include/application_manager/commands/mobile/"

with open("mobile_request_template_header", 'r') as ftemp:
    templateString = ftemp.read()
with open(os.path.join(sourceDir, headerPathFromSrc, str(sys.argv[2])+"_request.h"),'w') as f:
    f.write(templateString.format(**d))

with open("mobile_response_template_header", 'r') as ftemp:
    templateString = ftemp.read()
with open(os.path.join(sourceDir, headerPathFromSrc,str(sys.argv[2])+"_response.h"), 'w') as f:
    f.write(templateString.format(**d))

#Create HMI Class Definitions

#Hmi Req and Res headers

headerPathFromSrc = "components/application_manager/src/commands/hmi/"

with open("hmi_request_template", 'r') as ftemp:
    templateString = ftemp.read()
with open(os.path.join(sourceDir, headerPathFromSrc,str(sys.argv[5])+"_request.cc"), 'w') as f:
    f.write(templateString.format(**d))

with open("hmi_response_template", 'r') as ftemp:
    templateString = ftemp.read()
with open(os.path.join(sourceDir, headerPathFromSrc,str(sys.argv[5])+"_response.cc"), 'w') as f:
    f.write(templateString.format(**d))

headerPathFromSrc = "components/application_manager/include/application_manager/commands/hmi/"

with open("hmi_request_template_header", 'r') as ftemp:
    templateString = ftemp.read()
with open(os.path.join(sourceDir, headerPathFromSrc,str(sys.argv[5])+"_request.h"), 'w') as f:
    f.write(templateString.format(**d))

with open("hmi_response_template_header", 'r') as ftemp:
    templateString = ftemp.read()
with open(os.path.join(sourceDir, headerPathFromSrc,str(sys.argv[5])+"_response.h"), 'w') as f:
    f.write(templateString.format(**d))


#with open("interface", 'r') as ftemp:
#    templateString = ftemp.read()
#with open(str(sys.argv[2])+"_response.cc", 'w') as f:
#    f.write(templateString.format(**d))
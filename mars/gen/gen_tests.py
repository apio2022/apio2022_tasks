import os

data=open('data','w')

data.write("@subtask samples\n")
data.write("manual sample-01.in\n")
data.write("manual sample-02.in\n\n")

for i in range(1,11):
	f=""
	try:
		f=open("subtask-"+str(i)+".txt","r")
	except:
		continue
	data.write("@subtask subtask"+str(i)+'\n')
	if i == 1:
		data.write("@include samples\n")
	else:
		data.write("@include subtask"+str(i-1)+"\n")
	tests=[]
	curtest = 0
	curfile = 0
	for st in f:
		st=st.strip()
		if st=="":
			continue
		tests.append(st)
	for test in tests:
		if curtest % 10 == 0:
			curfile=curfile+1
			data.write("manual "+str(i)+"-"+str(curfile)+".in\n")
			os.system("echo "+str(min(10,len(tests)-curtest))+" > manual/"+str(i)+"-"+str(curfile)+".in")
		os.system("./gen.exe "+test+" >> manual/"+str(i)+"-"+str(curfile)+".in")
		curtest+=1
	data.write('\n')


def csvFixer(filename,title):

    f = open(filename,"r")
    
    xvals = ["x"+title]
    yvals = ["y"+title]

    open_bracket_count = 0
    closed_bracket_count = 0

    for line in f:
        txt = " ".join(line.split(","))
        txt = " ".join(txt.split(";"))
        txt = txt.split()
        for t in txt:
            if t == "}":
                closed_bracket_count+=1
            if open_bracket_count == 2 and closed_bracket_count == 0:
                xvals.append(t)
            elif open_bracket_count == 3 and closed_bracket_count == 1:
                yvals.append(t)
            if t == "{":
                open_bracket_count+=1
            
    f.close()
                    
    if len(xvals) != len(yvals):
        print("Number of x and y values are different")

    return [xvals, yvals]


for ch in range(8):
    data = []
    for event in range(6):
        coords = csvFixer("firstSix/ch" + str(ch) + "/event_" + str(event), str(event))
        data.append(coords[0])
        data.append(coords[1])
    f = open("firstSix/ch" + str(ch) + "/total.csv","w")
    for n in range(len(data[0])):
        string = ""
        for value in range(len(data)):
            string = string + data[value][n] + ","
        f.write(string[0:-1] + "\n")
    f.close()
        
        
        

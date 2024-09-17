
def TGraph_to_list(filename,title):

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



def TH1F_to_list(filename, mod):

    f = open(filename,"r")

    bins = {}
    for i in range(0,750,mod):
        bins[i]=0

    for line in f:
        if line[15:28] == "SetBinContent":
            str_coord = line[29:-3].split(",")
            coord = [int(str_coord[0]), int(str_coord[1])]
            bins[int(coord[0]/mod)*mod]+=coord[1]
            
    f.close()             

    return bins



def firstSix_csv(run):
    for ch in range(8):
        data = []
        for event in range(6):
            coords = TGraph_to_list("run_" + run + "/firstSix_ch" + str(ch) + "_event" + str(event), str(event))
            data.append(coords[0])
            data.append(coords[1])
        f = open("run_" + run + "/firstSix_ch" + str(ch) + "_total.csv","w")
        for n in range(len(data[0])):
            string = ""
            for value in range(len(data)):
                string = string + data[value][n] + ","
            f.write(string[0:-1] + "\n")
        f.close()


        
def maxHist_csv(run, mod):
    data = []
    for ch in range(8):
        bins = TH1F_to_list("run_" + run + "/maxHist_ch" + str(ch) + ".csv", mod)
        data.append(bins)
    f = open("run_" + run + "/maxHist_total.csv","w")
    for i in range(0,750,mod):
        string = str(i) + ","
        for d in data:
            string = string + str(d[i]) + ","
        f.write(string[0:-1] + "\n")
    f.close()
        

    
def main():
    #firstSix_csv('186')
    #maxHist_csv('186',5)
    runs = ["186","189","192","194","196","198","200","175","171","167","150","155","158","163","329","331","333","337","340","343","346","347","350","321","319","317","314","311"]
    for run in runs:
        try:
            firstSix_csv(run)
            maxHist_csv(run,5)
        except:
            print("run " + run + " seg faulted")
            continue


main()
        
        
        

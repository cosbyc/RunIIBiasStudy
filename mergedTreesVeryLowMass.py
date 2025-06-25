import os, commands, glob, sys

def processCmd(cmd, quite = 0):
    status, output = commands.getstatusoutput(cmd)
    if (status !=0 and not quite):
        print 'Error in processing command:\n   ['+cmd+']'
        print 'Output:\n   ['+output+'] \n'
    return output


startdir = os.getcwd()

print("Change jobsdir to what it is for YOU, then delete this line.  Also adjust paths later in this script."); sys.exit(1)
jobsdir='/afs/cern.ch/work/c/ccosby/CMSSW_8_1_0/src/BiasStudy/test_Aug24_22_ptCut'
os.chdir(jobsdir) 



#massPoints = ["0.235", "0.246", "0.259", "0.272", "0.286", "0.301", "0.316", "0.332", "0.349", "0.367", "0.386", "0.405", "0.426", "0.448", "0.471", "0.495", "0.634", "0.667", "0.701", "0.899", "1.152", "1.211", "1.273", "1.338", "1.406", "1.478", "1.553", "1.632", "1.716", "1.803", "1.895", "1.992", "2.094", "2.2", "2.313", "2.43", "2.554", "4.201", "4.415", "4.641", "4.877", "5.126", "5.388", "5.663", "5.951", "6.255", "6.574", "6.909", "7.262", "7.632", "8.022", "8.431", "0.235", "0.246", "0.259", "0.272", "0.286", "0.301", "0.316", "0.332", "0.349", "0.367", "0.386", "0.405", "0.426", "0.448", "0.471", "0.495", "0.634", "0.667", "0.701", "0.899", "1.152", "1.211", "1.273", "1.338", "1.406", "1.478", "1.553", "1.632", "1.716", "1.803", "1.895", "1.992", "2.094", "2.2", "2.313", "2.43", "2.554", "4.201", "4.415", "4.641", "4.877", "5.126", "5.388", "5.663", "5.951", "6.255", "6.574", "6.909", "7.262", "7.632", "8.022", "8.431"]

#massPoints = ["1.211", "1.338", "1.478", "1.632", "1.803", "1.992", "2.2", "2.43", "4.415", "4.877", "5.388", "5.951", "6.574", "7.262"]
massPoints = ["1.331"]


for year in ['2017', '2018']:
    for pdf1 in ['0', '1']:
        for pdf2 in ['0','1']:
            for mass in massPoints:

                if (not os.path.isdir('/afs/cern.ch/work/c/ccosby/CMSSW_8_1_0/src/BiasStudy/mergedTrees/lowMassTrees'+year+'_PDF'+pdf1+'_PDF'+pdf2+'/')):
                    cmd = 'mkdir /afs/cern.ch/work/c/ccosby/CMSSW_8_1_0/src/BiasStudy/mergedTrees/lowMassTrees'+year+'_PDF'+pdf1+'_PDF'+pdf2+'/'
                    processCmd(cmd)
                massTitle = mass.replace('.', '_')
                outfile = 'fitDiagnostics'+mass+'.root'
                cmd = 'hadd -f ../mergedTrees/lowMassTrees'+year+'_PDF'+pdf1+'_PDF'+pdf2+'/'+outfile+' '
                for file in glob.glob(jobsdir+"/dir_m"+massTitle+"*"+year+"*/fitDiagnosticsPDF"+pdf1+"_PDF"+pdf2+"_*.root"):
                    print file
                    cmd += ' '+file
                print cmd
                processCmd(cmd)
        
         
os.chdir(startdir)
 

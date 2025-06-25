import os, commands, glob, sys

# define function for processing the external os commands
def processCmd(cmd, quite = 0):
    status, output = commands.getstatusoutput(cmd)
    if (status !=0 and not quite):
        print 'Error in processing command:\n   ['+cmd+']'
        print 'Output:\n   ['+output+'] \n'
    return output

print("Change jobsdir to what it is for YOU, then delete this line."); sys.exit(1)
jobsdir='/afs/cern.ch/work/c/ccosby/CMSSW_8_1_0/src/BiasStudy/sigma8Window_060523_sig/'

if (not os.path.isdir(jobsdir)):
    cmd = 'mkdir '+jobsdir
    processCmd(cmd)

f= open("massPoint.txt","w+")

for file in glob.glob("Datacards_lowMass/*.txt"):

    parse = file
    cut, parse = parse.split('/')
    card, year, mass, index = parse.split("_", 3)
    year = year.split("I")[0]
    mass = mass.split("m")[1]
    # cut trailing zeroes (combine loses them, causes problems)
    while (mass[-1] == "0"):
        mass=mass[:-1]
    # alternate mass string to use in paths/file names
    massTitle = mass.replace(".","_")
    index = index.split(".")[0]
    print parse
        
    # save mass value in a list for later
    if (year == "2017"):
        f.write("\""+ mass+'\", ')
    else:
        f.write(mass+', ')


    seed = 101
    while (seed<=110):
    

        # make a working directory for each year/mass point/seed 
        condorDir = jobsdir+'dir_m'+massTitle+'_'+year+'_'+str(seed)+'/'
        if (not os.path.isdir(condorDir)):
            cmd = 'mkdir '+condorDir
            processCmd(cmd)

        # move text datacard to job dir
        cmd = 'cp Datacards_lowMass/'+parse+ " "+condorDir
        print cmd
        processCmd(cmd)

        # move workspace to job dir
        cmd = 'cp Datacards_lowMass/dpWorkspace'+year+'IterV3_'+index+'.root ' +condorDir
        print cmd
        processCmd(cmd)

        startdir = os.getcwd()

        # add job specific parameters to template script
        cmd = 'cat domultifitbias_lowMass_Multi.sh.template | sed "s?DIRECTORY?'+condorDir+'?g" | sed "s?CARD?'+parse+'?g" | sed "s?YEAR?'+year+'?g" | sed "s?MASS?'+mass+'?g" | sed "s?SEED?'+str(seed)+'?g" > '+condorDir+'domultifitbias_lowMass_'+year+'catA_m'+massTitle+'_'+str(seed)+'.sh'
        print (cmd)
        processCmd(cmd)

        # add job specific parameters to condor configuration
        cmd = 'cat condor_job_setup.sub | sed "s?run.sh?domultifitbias_lowMass_'+year+'catA_m'+massTitle+'_'+str(seed)+'.sh?g" | sed "s?OUT?'+condorDir+'?g" > '+condorDir+'condor_job_setup_'+year+'_'+massTitle+'_'+str(seed)+'.sub'
        print(cmd)
        processCmd(cmd)




        # submit job to condor
        os.chdir(condorDir)
        result = processCmd('condor_submit condor_job_setup_'+year+"_"+massTitle+'_'+str(seed)+'.sub')
        print result
        os.chdir(startdir)

        seed+=1

f.close()

##################################################


Instructions for running bias study.  All scripts need to be updated you point to YOUR working directory. This workspace is a template for what was used in the 2017+2018 dimuon scouting, in CMSSW_8_1_0 (on CentOS 7...).  Modifications may be needed to run in newer CMSSW versions and machines. Be careful in general for hard-coded elements that applied to the Run-II analysis that may not be transferrable to your measurement (years, pdf-indexes, etc.)

1. Move all datacards and workspaces (for the sig+bkg models and masses you are studying) into "./Datacards_lowMass/".
   - If the names do not match the pattern 'dpWorkspace'+year+'IterV3_'+index+'.root" and 'dpCard_'+year+'IterV3_m'+mass+'_'+index+'.txt', either change the names or modify the job submission script.
      - Examples:
	- dpCard_2018IterV3_m0.272_30.txt
	- dpWorkspace2017IterV3_340.root

2. Choose a condor job template and modify it to fit your needs. Check hard coded elements, including PDF names in workspaces!
    - Use `domultifitbias_lowMass_Multi.sh.template` for the self bias of 4 different PDFs (expects cards for two seperate years currently)
    - Use `domultifitbias_lowMass_catA.sh.template` for the self/cross bias of 2 different PDFs.
    - Adjust '--expectSignal' argument in toy generation depending on whether you want injected signal or not.

3. Modify condor submit script `Submitjobs_veryLowMass.py`:
   - Update 'jobsdir' to reference your workspace.
   - Make sure it includes your desired template for the combine commands.

4. Submit jobs. Make sure your grid certificate is valid.

```
python Submitjobs_veryLowMass.py   
```

5. Once jobs are complete, sum histograms with 'mergedTreesVeryLowMass.py' or 'mergedTreesVeryLowMassMulti.py'
   - Change jobsdir to whatever you set it to in the submission script.
   - For some reason I did it stupidly and it needs a list of strings corresponding to the mass of each value you are running...
   - The submit job script will generate "massPoint.txt", so you can copy it over from there, but you should probably rewrite this whole part.

```
python mergedTreesVeryLowMass.py
```

6. Produce bias plots.
   - Run `$ root -lqb makeBiasPlotMulti.C` for plotting the bias of 4 PDFs on the same plot.
   - Run `$ root -lqb makeBiasPlot.C` for plotting for the self/cross bias of 2 PDFs
   - DO NOT FORGET to change the bias calculation function at the top of the script to reflect your true injected signal.
   - This file will also need a list of strings corresponding to each mass point, as well as a list of integers.  Both are in massPoint.txt.

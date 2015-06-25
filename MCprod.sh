#!/bin/bash

nevents=10000
job=Py8PtGun_upsilon_cfi

while test $# -gt 0; do
	case "$1" in 
		-n|--nevents)
                        shift
			nevents=$1 
			shift
			;;

		-j|--job)
			shift
			job=$1
			shift
			;;

		*)
			break
			;;
	esac
done

#echo "cmsDriver.py GeneratorInterface/Pythia8Interface/python/$job.py --fileout file:$job.root --mc --eventcontent AODSIM --datatier GEN-SIM --conditions auto:mc --beamspot NominalCollision2015 --step GEN --magField 38T_PostLS1 --python_filename $job.py -n $nevents >& test.log"

cmsDriver.py GeneratorInterface/Pythia8Interface/python/$job.py --fileout file:$job.root --mc --eventcontent AODSIM --datatier GEN-SIM --conditions auto:mc --beamspot NominalCollision2015 --step GEN --magField 38T_PostLS1 --python_filename $job.py -n $nevents >& test.log
